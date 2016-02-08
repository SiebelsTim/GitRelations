#pragma once
#include "git2.h"
#include "Repository.h"

class Commit;

/**
 * An Iterator to traverse commits of a specific repository
*/
class CommitIterator {
  std::shared_ptr<git_revwalk> walk;
  const Repository& repo;
  git_oid oid;
  bool end;
  filterfn filter = nullptr;
public:
  /**
   * Creates an iterator to iterate over a list of commits
   *
   * @param repo Repository to fetch commits from
   * @param end if true, the created CommitIterator represents an after end value
   * @throws GitException on error
  */
  explicit CommitIterator(const Repository& repo, bool end = false);
  /**
   * Fetches current Commit
   * @return commit pointed to currently
  */
  Commit operator*(void) const;
  /**
   * Advances the pointer. Moves the iterator forward.
   * Consumes as many entries until the set filter function returns true.
   * Consumes one entry if no filter is set.
   * @return this
  */
  const CommitIterator& operator++(void);
  /**
   * Compares position of two CommitIterators
   *
   * Used to check if a CommitIterator reached end
   * @param iter Iterator to compare to
   * @return true if not equal
  */
  bool operator!=(const CommitIterator& iter) const;

  /**
   * Sets a filter function. Pass nullptr to unset the filter.
   * @param filter filter function. The functions is supposed to return true to
   *               accept an element.
   * @return this
  */
  CommitIterator& setFilter(filterfn filter);

private:
  /**
   * Implementation of operator++ for advancing one element.
   * operator++ handles filtering
   * @return this
  */
  const CommitIterator& next();
};
