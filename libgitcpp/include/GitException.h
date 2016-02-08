#pragma once

#include <exception>

/**
 * An object containing an error from libgit
*/
class GitException : std::exception {
private:
  int errnum;
  const git_error* error;
public:
  /**
   * Constructs a GitException containing a git_error
   * @param errnum error number returned from git_* function
   * @param error current git_error
  */
  GitException(int errnum, const git_error* error):
    errnum(errnum), error(error) {
  }

  /**
   * Gets the message from the error
   * @return message
  */
  const char* what() const noexcept {
    return error->message;
  }

  /**
   * Returns the error number from the error
   * @return error number
  */
  int errornum() const {
    return errnum;
  }

  /**
   * Returns the klass from the error
   * @return klass
  */
  int klass() const {
    return error->klass;
  }
};
