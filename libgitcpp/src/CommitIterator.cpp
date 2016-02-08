#include "CommitIterator.h"
#include "Commit.h"
#include "error.h"

CommitIterator::CommitIterator(const Repository& repo, bool end /* = false */):
  repo(repo), end(end) {
  git_revwalk* revwalk = nullptr;
  int error = git_revwalk_new(&revwalk, repo.get());
  walk = std::shared_ptr<git_revwalk>(revwalk, git_revwalk_free);
  throw_on_error(error);
  git_revwalk_sorting(revwalk, GIT_SORT_TOPOLOGICAL | GIT_SORT_TIME);
  git_revwalk_push_head(revwalk);
  error = git_revwalk_next(&oid, walk.get());
  throw_on_error(error);
}


Commit CommitIterator::operator*(void) const {
  git_commit* commit = nullptr;
  int error = git_commit_lookup(&commit, repo.get(), &oid);
  Commit comm(commit);
  throw_on_error(error);
  return comm;
}

const CommitIterator& CommitIterator::operator++(void) {
  if (filter == nullptr) return next();
  while (true) {
    next();
    const Commit& commit = operator*();
    if (filter(commit) || end) break;
  }
  return *this;
}

const CommitIterator& CommitIterator::next() {
  int error = git_revwalk_next(&oid, walk.get());
  if (error == GIT_ITEROVER) {
    end = true;
    return *this;
  }
  throw_on_error(error);
  return *this;
}

CommitIterator& CommitIterator::setFilter(filterfn filter) {
  this->filter = filter;
  if (filter != nullptr) {
    // move to the first valid commit
    const Commit& head = operator*();
    if (filter(head)) return *this;
    operator++();
  }
  return *this;
}

bool CommitIterator::operator!=(const CommitIterator& iter) const {
  if (end && iter.end) return false;
  // Boolean xor
  if (end != iter.end) return true;
  return operator*() != *iter;
}
