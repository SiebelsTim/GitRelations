#include "Reference.h"
#include "Repository.h"
#include "Commit.h"
#include "error.h"

Reference::Reference(git_reference* ref, const Repository& repo):
  reference(ref, git_reference_free), repo(repo) {}

std::string Reference::name() const {
  const char* name = git_reference_name(get());
  return std::string(name);
}

bool Reference::isBranch() const{
  return git_reference_is_branch(get());
}

bool Reference::isNote() const{
  return git_reference_is_note(get());
}

bool Reference::isTag() const{
  return git_reference_is_tag(get());
}

git_reference* Reference::get() const {
  return reference.get();
}

Commit Reference::toCommit() const {
  git_commit* commit = nullptr;
  git_oid oid;
  int error = git_reference_name_to_id(&oid, repo.get(), name().c_str());
  throw_on_error(error);

  error = git_commit_lookup(&commit, repo.get(), &oid);
  Commit ret(commit);
  throw_on_error(error);
  return ret;
}

bool Reference::operator==(const Reference& ref) {
  return name() == ref.name();
}

bool Reference::operator!=(const Reference& ref) {
  return !operator==(ref);
}
