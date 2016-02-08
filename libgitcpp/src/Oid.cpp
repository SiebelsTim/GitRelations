#include "Oid.h"

Oid::Oid(const git_oid* oid):
  oid(oid) {}

std::string Oid::toString() const {
  return std::string(git_oid_tostr_s(oid));
}

const git_oid* Oid::get() const {
  return oid;
}

bool Oid::operator==(const Oid& oid) const {
  return toString() == oid.toString();
}

bool Oid::operator!=(const Oid& oid) const {
  return !operator==(oid);
}
