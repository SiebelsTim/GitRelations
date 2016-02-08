#include "Diff.h"
#include "Commit.h"
#include "error.h"

Diff::Diff(git_diff* diff):
  diff(diff, git_diff_free) {}

Diff::Diff(const Commit& comm, const Commit& comm2) {
  git_tree *commit_tree = nullptr, *parent_tree = nullptr;
  int error = 0;
  error = git_commit_tree(&commit_tree, comm.get());
  throw_on_error(error);
  error = git_commit_tree(&parent_tree, comm2.get());
  throw_on_error(error);

  git_diff *d = nullptr;
  error = git_diff_tree_to_tree(
            &d, comm.getRepo(), commit_tree, parent_tree, nullptr);
  throw_on_error(error);
  // TODO: Make this exception safe
  git_tree_free(commit_tree);
  git_tree_free(parent_tree);
  diff = std::shared_ptr<git_diff>(d, git_diff_free);
}

git_diff* Diff::get() const {
  return diff.get();
}

std::shared_ptr<git_diff_stats> Diff::getStats() const {
  git_diff_stats* stats = nullptr;
  int error = git_diff_get_stats(&stats, get());
  throw_on_error(error);
  return std::shared_ptr<git_diff_stats>(stats, git_diff_stats_free);
}

size_t Diff::linesAdded() const {
  std::shared_ptr<git_diff_stats> stats = getStats();
  return git_diff_stats_insertions(stats.get());
}

size_t Diff::linesDeleted() const {
  std::shared_ptr<git_diff_stats> stats = getStats();
  return git_diff_stats_deletions(stats.get());
}

size_t Diff::filesChanged() const {
  std::shared_ptr<git_diff_stats> stats = getStats();
  return git_diff_stats_files_changed(stats.get());
}
