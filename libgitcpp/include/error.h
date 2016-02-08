#pragma once

#include "GitException.h"

/**
 * Converts a giterr into a GitException
 * @param error errorcode
 * @throws GitException when errorcode is < 0
*/
inline void throw_on_error(const int error) {
  if (error < 0) {
    const git_error *e = giterr_last();
    throw GitException(error, e);
  }
}
