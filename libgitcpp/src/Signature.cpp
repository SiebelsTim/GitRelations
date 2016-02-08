#include "Signature.h"

Signature::Signature(const git_signature* sig):
  signature(sig) {}

const git_signature* Signature::get() const {
  return signature;
}

std::string Signature::name() const {
  return std::string(get()->name);
}

std::string Signature::mail() const {
  return std::string(get()->email);
}

git_time Signature::time() const {
  return get()->when;
}

bool Signature::operator==(const Signature& sig) {
  return name() == sig.name() &&
         mail() == sig.mail() &&
         time().time == sig.time().time;
}

bool Signature::operator!=(const Signature& sig) {
  return !operator==(sig);
}
