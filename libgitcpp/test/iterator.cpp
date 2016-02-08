#include "CommitIterator.h"
#include "gtest/gtest.h"
#include "Repository.h"
#include "Commit.h"
/*
TEST(iterator, create) {
  Repository repo("../test/testrepo");
  CommitIterator iter(repo);
  Commit comm = *iter;
  ASSERT_EQ("Add README\n", comm.message());
}

TEST(iterator, iterate) {
  Repository repo("../test/testrepo");
  const std::string expect[] = {
    "Add README\n",
    "Add some more lines\n",
    "initial\n"
  };
  int i = 0;
  auto iterator = repo.iter();
  for (auto iter = iterator.begin(); iter != iterator.end(); ++iter) {
    ASSERT_EQ(expect[i++], (*iter).message());
  }
  ASSERT_EQ(i, 3);
}

TEST(iterator, iterate_rangedfor) {
  Repository repo("../test/testrepo");
  const std::string expect[] = {
    "Add README\n",
    "Add some more lines\n",
    "initial\n"
  };
  int i = 0;
  for (auto commit : repo.iter()) {
    ASSERT_EQ(expect[i++], commit.message());
  }
  ASSERT_EQ(i, 3);
}

TEST(iterator, filter) {
  Repository repo("../test/testrepo");
  const std::string expect[] = {
    "Add README\n",
    "Add some more lines\n"
  };
  int i = 0;
  auto iter = repo.iter();
  iter.setFilter([](const Commit& commit) {
    const std::string msg = commit.message();
    return msg[0] == 'A';
  });
  for (auto commit : iter) {
    ASSERT_EQ(expect[i++], commit.message());
  }
  ASSERT_EQ(i, 2);
}*/

TEST(iterator, filter2) {
  Repository repo("../test/testrepo");
  const std::string expect[] = {
    "initial\n"
  };
  int i = 0;
  auto iter = repo.iter();
  iter.setFilter([](const Commit& commit) {
    const std::string msg = commit.message();
    return msg[0] != 'A';
  });
  for (auto commit : iter) {
    ASSERT_EQ(expect[i++], commit.message());
  }
  ASSERT_EQ(i, 1);
}
