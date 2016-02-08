#pragma once
#include "git2.h"
#include "CommitX.h"
#include <string>
#include <vector>
#include <memory>

class Reference;
class CommitIterator;
class Commit;


/**
 * Object representing a Repository
*/
class Repository {
public:
  class iterator;
  /**
   * Opens up a repository from path
   * @param path path to Repository
   * @throws on error or if repo can't be found
  */
  Repository(const std::string& path);
  /**
   * Copy constructor
   * @param repo copy
  */
  Repository(const Repository& repo);
  /**
   * Closes the libgit handle
  */
  ~Repository();
  std::string getName() const;
  Repository& operator=(const Repository& repo) {
    this->repo = repo.repo;
    return *this;
  }
  /**
   * This returns a vector containing all commits with parent(0)
   *
   * This is a very naive approach and genereally shouldn't be used.
   * Use the iterator instead. This can also be very resource heavy on large
   * repositories
   * @return vector of commits
  */
  std::vector<Commit> getAllCommits() const;
  std::vector<CommitX*> getAllCommitsX(int since = 0, int until = 0) const;
  /**
   * Gets the underlying git_repository datastructure
   *
   * One should never free this
  */
  git_repository* get() const;
  /**
   * Returns whatever HEAD points to
   * @return HEAD
  */
  Reference head() const;

  /**
   * Compares two Repositories
   * @param repo Repository to compare to
   * @return true if both objects point to the same path
  */
  bool operator==(const Repository& repo) const;

  /**
   * Compares two Repositories
   * @param repo Repository to compare to
   * @return true if both objects point to a different path
  */
  bool operator!=(const Repository& repo) const;

  /**
   * Returns an iterator to iterate over all commits from HEAD on
   * @return iterator
  */
  iterator iter() const;
private:
  std::shared_ptr<git_repository> repo = nullptr;
  const std::string m_path;
};



typedef bool(*filterfn)(const Commit&);

/**
 * Iterator to traverse all commits of a repository
*/
class Repository::iterator {
  const Repository& repo;
  filterfn filter = nullptr;
public:
  /**
   * Creates the iterator from a Repository
   *
   * You should generally use Repository::iter()
   * @param repo Repository to iterate over
  */
  iterator(const Repository& repo): repo(repo) {}
  /**
   * Creates the iterator from a Repository and attaches a filter function
   * @param repo Repository to iterate over
   * @param filterfn filter function
  */
  void setFilter(filterfn fn) {
    filter = fn;
  }
  /**
   * Returns a CommitIterator to do the actual iteration
   * @return iterator
  */
  CommitIterator begin() const;
  /**
   * Returns a CommitIterator that represents beyond end
   * @return end iterator
  */
  CommitIterator end() const;
};
