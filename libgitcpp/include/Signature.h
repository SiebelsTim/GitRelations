#pragma once
#include "git2.h"
#include <string>

/**
 * Object representing a person associated with time
 * This represents author, commiter etc.
*/
class Signature {
public:
  /**
   * Creates a Signature from git_signature Object
   *
   * One should not invoke it himself, rather use Commit::getAuthor() and friends
   * @param sig git_signature datastructure
  */
  explicit Signature(const git_signature* sig);
  /**
   * Gets the underlying git_signature datastructure
   *
   * One should never free this, nor use it to instantiate a signature object
  */
  const git_signature* get() const;
  /**
   * Returns the name of the person
   * @return name
  */
  std::string name() const;
  /**
   * Returns the mail of the person
   * @return mail
  */
  std::string mail() const;
  /**
   * Returns the time the action was done
   * @return time
  */
  git_time time() const;

  /**
   * Compares two signatures
   * @param sig Signature to compare to
   * @return true if equal
  */
  bool operator==(const Signature& sig);

  /**
   * Compares two signatures
   * @param sig Signature to compare to
   * @return true if not equal
  */
  bool operator!=(const Signature& sig);
private:
  const git_signature* signature;
};
