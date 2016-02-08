#pragma once
#include "git2.h"
#include <string>
#include <memory>

class Repository;
class Commit;
/**
 * Object representing a git Reference
*/
class Reference {
public:
  /**
   * Constructs a reference from  git_reference datastructure
   *
   * One should not invoke this himself
   * @param ref git_reference datastructure
   * @param repo Repository containing the reference
  */
  explicit Reference(git_reference* ref, const Repository& repo);
  /**
   * Gets a human readable name for the reference
   * @return name
  */
  std::string name() const;
  /**
   * Returns true if this reference is a branch
   * @return true if branch
  */
  bool isBranch() const;
  /**
   * Returns true if this reference is a Note
   * @return true if Note
  */
  bool isNote() const;
  /**
   * Returns true if this reference is a tag
   * @return true if tag
  */
  bool isTag() const;
  /**
   * Converts this Reference into a Commit
   * @return Commit
  */
  Commit toCommit() const;
  /**
   * Returns the Repository containing this reference
   * @return Repository
  */
  Repository getRepo() const;
  /**
   * Returns the underlying git_reference datastructure
   *
   * One should not free this, nor use it to create a Reference object
   * @return underlying git_reference
  */
  git_reference* get() const;

  /**
   * Compares two References.
   * @param ref Reference to compare to
   * @return true if equal
  */
  bool operator==(const Reference& ref);
  /**
   * Compares two References.
   * @param ref Reference to compare to
   * @return true if not equal
  */
  bool operator!=(const Reference& ref);

private:
  std::shared_ptr<git_reference> reference;
  const Repository& repo;
};
