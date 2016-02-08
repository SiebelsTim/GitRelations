#include "gtest/gtest.h"
#include "Repository.h"
#include "Commit.h"
#include "Reference.h"
#include "Oid.h"

TEST(oid, headcommit) {
  Repository repo("../test/testrepo");
  Oid head = repo.head().toCommit().oid();
  ASSERT_EQ(head.toString(), "8f670e454b128d593b7bf9c1a80f46f8029818f9");
}
