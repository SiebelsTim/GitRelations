#pragma once
#include "git2.h"
#include <string>
#include <memory>
#include <vector>

class Repository;
class Signature;
class Oid;
class Diff;

/**
 * An object representing a commit
*/
class Commit {
public:
  /**
   * Creates commit
   *
   * This should not be invoked yourself, use Repository to fetch commits
   * @param ref libgit commit datastructure.
  */
  explicit Commit(git_commit* ref);
  /**
   * Returns the summary of this commit
   * @return Summary of the commit
  */
  std::string summary() const;
  /**
   * Returns the message of this commit
   * @return message of the commit
  */
  std::string message() const;
  /**
   * Returns the committer of this commit
   * @return committer of the commit
  */
  Signature committer() const;
  /**
   * Returns the author of this commit
   * @return author of the commit
  */
  Signature author() const;
  /**
   * Returns the time of this commit
   * @return seconds since epoch
  */
  time_t time() const;

  /**
   * Returns the number of parents of this commit
   * @return number of parents
  */
  unsigned int parentCount() const;
  /**
   * Returns the parent at position n for this commit
   * @throws GitException on error or if n >= parentCount
   * @param n position
   * @return Summary of the commit
  */
  Commit parent(unsigned int n) const;
  /**
   * Returns the OID of this commit
   *
   * this is used to identify a commit
   * @return OID of the commit
  */
  Oid oid() const;

  /**
   * Returns the Diff for this commit to its parent
   * @throws GitException on error
   * @return Diff
  */
  Diff getDiff() const;
  /**
   * Returns the Diff for this commit to a different commit
   *
   * This is equivalent to `git diff this..comm`
   * @throws GitException on error
   * @return git diff this..comm
  */
  Diff getDiff(const Commit& comm) const;

  /**
   * Returns the underlying git_commit structure for this commit
   *
   * one should never use this to initialize a Commit object
   * @return git_commit data structure
  */
  git_commit* get() const;
  /**
   * Returns the Repository containing this commit
   *
   * One should never free this
   * @return git_repository data structure
  */
  git_repository* getRepo() const;

  std::vector<std::string> getAffectedFiles() const;

  /**
   * Compares two commits
   *
   * Returns true if both OIDs are equal.
   * @param comm Commit to compare to
   * @return true if commits are equal
  */
  bool operator==(const Commit& comm) const;
  /**
   * Compares two commits
   *
   * Returns true if OIDs are different.
   * Is equivalent to `NOT operator==()`
   * @param comm Commit to compare to
   * @return git diff this..comm
  */
  bool operator!=(const Commit& comm) const;

private:
  std::shared_ptr<git_commit> commit = nullptr;
};
