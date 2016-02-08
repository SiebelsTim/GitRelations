#include "Commit.h"
#include "Repository.h"
#include "Oid.h"
#include "Signature.h"
#include "Diff.h"
#include "error.h"

Commit::Commit(git_commit* commit):
  commit(commit, git_commit_free) {}

git_commit* Commit::get() const {
  return commit.get();
}

std::string Commit::summary() const {
  const char* ret = git_commit_summary(get());
  return std::string(ret);
}

std::string Commit::message() const {
  return std::string(git_commit_message(get()));
}

Signature Commit::committer() const {
  const git_signature* sig = git_commit_committer(get());
  return Signature(sig);
}

Signature Commit::author() const {
  const git_signature* sig = git_commit_author(get());
  return Signature(sig);
}

time_t Commit::time() const {
  return git_commit_time(get());
}

Oid Commit::oid() const {
  const git_oid* oid = git_commit_id(get());
  return Oid(oid);
}

unsigned int Commit::parentCount() const {
  return git_commit_parentcount(get());
}

/**
 * A commit can have multiple parents
 * n specifies the nth commit, from 0 to `parentCount`
*/
Commit Commit::parent(unsigned int n) const {
  git_commit* ret = nullptr;
  int error = git_commit_parent(&ret, get(), n);
  Commit comm(ret);
  throw_on_error(error);
  return comm;
}



git_repository* Commit::getRepo() const {
  git_repository* repo = git_commit_owner(get());
  return repo;
}

Diff Commit::getDiff() const {
  return Diff(parent(0), *this);
}

Diff Commit::getDiff(const Commit& commit) const {
  return Diff(*this, commit);
}

std::vector<std::string> Commit::getAffectedFiles() const {
  git_tree* tree = nullptr;
  git_tree* tree2 = nullptr;
  int error = git_commit_tree(&tree, get());
  throw_on_error(error);
  
  try {
    error = git_commit_tree(&tree2, parent(0).get());
  } catch (GitException e) {
    tree2 = nullptr; // probably initial commit
  }
  git_diff* diff = nullptr;
  git_diff_tree_to_tree(&diff, getRepo(), tree2, tree, 0);  
 
  std::vector<std::string> ret;
  git_diff_foreach(diff,
  [](const git_diff_delta* entry, float progress, void* payload) {
    std::string str = entry->old_file.path;
    ((std::vector<std::string>*)payload)->push_back(str);
    return 0;
  }, nullptr, nullptr, &ret);
  git_tree_free(tree);
  git_tree_free(tree2);
  git_diff_free(diff);
  return ret; 
}

bool Commit::operator==(const Commit& comm) const {
  if (commit.get() == nullptr && get() == nullptr) return true;
  else if (commit.get() == nullptr || get() == nullptr) return false;
  return oid() == comm.oid();
}

bool Commit::operator!=(const Commit& comm) const {
  return !(*this == comm);
}
