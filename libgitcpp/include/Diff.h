#pragma once
#include "git2.h"
#include <memory>

class Commit;

/**
 * Object represents a diff on two commits
 *
 * This object is useful for getting differences between two commits
*/
class Diff {
public:
  /**
   * Creates a Diff object
   *
   * One should not invoke this yourself. You can get a diff object from
   * a Commit
   * @param diff git_diff datastructure
  */
  explicit Diff(git_diff* diff);
  /**
   * Creates a Diff object from two commits
   *
   * equivalent to `git diff comm..comm2`
   * @param comm Commit 1
   * @param comm2 Commit 2
  */
  Diff(const Commit& comm, const Commit& comm2);
  /**
   * Gets the underlying git_diff datastructure
   *
   * One should never free this, nor use this to instantiate a Diff object
   * @return git_diff datastructure
  */
  git_diff* get() const;

  /**
   * Fetches lines added in this diff
   * @return lines added
  */
  size_t linesAdded() const;
  /**
   * Fetches lines deleted in this diff
   * @return lines deleted
  */
  size_t linesDeleted() const;
  /**
   * Fetches files changed in this diff
   * @return files changed
  */
  size_t filesChanged() const;
private:
  std::shared_ptr<git_diff_stats> getStats() const;
  std::shared_ptr<git_diff> diff;
};
