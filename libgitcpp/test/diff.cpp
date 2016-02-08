#include "gtest/gtest.h"
#include "Oid.h"
#include "Commit.h"
#include "Repository.h"
#include "Reference.h"
#include "Diff.h"

TEST(diff, stats) {
  Repository repo("../test/testrepo");
  Diff diff = repo.head().toCommit().getDiff();
  ASSERT_EQ(diff.linesDeleted(), 0);
  ASSERT_EQ(diff.linesAdded(), 2);
  ASSERT_EQ(diff.filesChanged(), 2);
}

TEST(diff, differentCommits) {
  Repository repo("../test/testrepo");
  Commit head = repo.head().toCommit();
  // git diff HEAD..HEAD^^
  Diff head_headupup_diff = head.getDiff(head.parent(0).parent(0));
  ASSERT_EQ(head_headupup_diff.linesDeleted(), 4);
  ASSERT_EQ(head_headupup_diff.linesAdded(), 0);
  ASSERT_EQ(head_headupup_diff.filesChanged(), 3);

  // git diff HEAD^..HEAD^^
  Diff headup_headupup_diff(head.parent(0), head.parent(0).parent(0));
  ASSERT_EQ(headup_headupup_diff.linesDeleted(), 2);
  ASSERT_EQ(headup_headupup_diff.linesAdded(), 0);
  ASSERT_EQ(headup_headupup_diff.filesChanged(), 1);
}
