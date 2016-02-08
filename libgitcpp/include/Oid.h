#pragma once
#include "git2.h"
#include <string>

/**
 * An Object representing a Oid
 *
 * A OID is a unique hash identifying a git object
*/
class Oid {
public:
  /**
   * Constructs a OID from git_oid object
   *
   * One should not create this yourself. Use Commit::oid() for this.
   * @param ref git_oid
  */
  explicit Oid(const git_oid* ref);
  /**
   * Converts the OID to a human-readable string
   * @return hash
  */
  std::string toString() const;
  /**
   * returns the underlying git_oid datastructure
   * @return git_oid datastructure
  */
  const git_oid* get() const;

  /**
   * Compares two OIDs
   *
   * True if both OID have the same hash
   * @return true if equal
  */
  bool operator==(const Oid& oid) const;

  /**
   * Compares two OIDs
   *
   * True if both OID have the same hash
   * @return true if not equal
  */
  bool operator!=(const Oid& oid) const;
private:
  const git_oid* oid = nullptr;
};
