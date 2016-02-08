#include "gtest/gtest.h"
#include "Repository.h"
#include "Commit.h"
#include "Reference.h"
#include "Signature.h"
#include "Oid.h"
#include "GitException.h"
#include <ctime>

TEST(commit, headcommit) {
  Repository repo("../test/testrepo");
  Commit head = repo.head().toCommit();
  ASSERT_EQ(head.message(), "Add subfolder\n");
}

TEST(commit, author) {
  Repository repo("../test/testrepo");
  Commit head = repo.head().toCommit();
  Signature author = head.author();
  Signature committer = head.committer();
  ASSERT_EQ(author.name(), "Tim Siebels");
  ASSERT_EQ(author.mail(), "tim_siebels_aurich@yahoo.de");
  ASSERT_EQ(committer.name(), "Tim Siebels");
  ASSERT_EQ(committer.mail(), "tim_siebels_aurich@yahoo.de");
}

TEST(commit, time) {
  Repository repo("../test/testrepo");
  Commit head = repo.head().toCommit();
  Signature author = head.author();
  ASSERT_EQ(author.time().time, 1448469335);
  time_t time = author.time().time;
  ASSERT_STREQ(std::ctime(&time), "Wed Nov 25 17:35:35 2015\n");

  ASSERT_EQ(head.time(), 1448469351);
  time = head.time();
  ASSERT_STREQ(std::ctime(&time), "Wed Nov 25 17:35:51 2015\n");
}

TEST(commit, parent) {
  Repository repo("../test/testrepo");
  Commit head = repo.head().toCommit();
  ASSERT_EQ(head.parentCount(), 1);
  ASSERT_EQ(head.parent(0).author().name(), "Tim Siebels");
  ASSERT_THROW(head.parent(1), GitException);
}

TEST(commit, summary) {
  Repository repo("../test/testrepo");
  Commit head = repo.head().toCommit();
  ASSERT_EQ(head.summary(), "Add subfolder");
}

TEST(commit, traversal) {
  Repository repo("../test/testrepo");
  Commit head = repo.head().toCommit();
  Commit& commit = head;
  const std::string expect[] = {
    "8f670e454b128d593b7bf9c1a80f46f8029818f9",
    "ea02e4840044d39a8889beb05c3ed7c97da41ec9",
    "8f9a04c7a15606e2bed98101a907ebd192c91428",
    "1a228f0e63ed64bc7d1c20946a28aef5ba2b4445"
  };
  int i = 0;
  while (true) {
    EXPECT_EQ(commit.oid().toString(), expect[i++]);
    if (commit.parentCount() == 0) break;
    commit = commit.parent(0);
  }
}

TEST(commit, tree) {
  Repository repo("../test/testrepo");
  Commit head = repo.head().toCommit();
  auto vector = head.getAffectedFiles();
  EXPECT_TRUE(std::find(vector.begin(), vector.end(), "file.txt") != vector.end());
  EXPECT_TRUE(std::find(vector.begin(), vector.end(), "subfolder/file") != vector.end());
}

