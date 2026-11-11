/*
 * Copyright (c) 2022 Salesforce, Inc.
 * All rights reserved.
 * SPDX-License-Identifier: BSD-3-Clause
 * For full license text, see the LICENSE.txt file in the repo root or https://opensource.org/licenses/BSD-3-Clause
 */
#pragma once

#include "tests.common.h"
#include "git_api.h"
#include "git2.h"

int TestGitAPI()
{
	TEST_START();

	GitAPI git(false);

	TEST(git.InitializeRepository("test-repo"), true);
	git.CreateIndex();
	git.AddFileToIndex("foo.txt", { 'x', 'y', 'z' }, false);
	std::string commitSHA1 = git.Commit(
	    "//a/b/c/...",
	    "12345678",
	    "test.user",
	    "test@user",
	    0,
	    "Test description for CL 12345678\n\nDetails",
	    10000000,
	    "");
	TEST(git.IsHEADExists(), true);
	TEST(git.IsRepositoryClonedFrom("//a/b/c/..."), true);
	TEST(git.IsRepositoryClonedFrom("//a/b/c/d/..."), false);
	TEST(git.IsRepositoryClonedFrom("//x/y/z/..."), false);
	TEST(git.DetectLatestCL(), "12345678");

	// Preserve the description without adding a CL prefix, and retain resume metadata.
	git_repository* repo = nullptr;
	TEST(git_repository_open(&repo, "test-repo"), 0);
	if (repo)
	{
		git_oid oid;
		TEST(git_oid_fromstr(&oid, commitSHA1.c_str()), 0);
		git_commit* commit = nullptr;
		TEST(git_commit_lookup(&commit, repo, &oid), 0);
		if (commit)
		{
			TEST(std::string(git_commit_message(commit)), "Test description for CL 12345678\n\nDetails\n[p4-fusion: depot-paths = \"//a/b/c/\": change = 12345678]");
			TEST(std::string(git_commit_author(commit)->name), "test.user");
			TEST(std::string(git_commit_committer(commit)->name), "test.user");
			TEST(std::string(git_commit_committer(commit)->email), "test@user");
			git_commit_free(commit);
		}
		git_repository_free(repo);
	}

	git.RemoveFileFromIndex("foo.txt");
	std::string commitSHA2 = git.Commit(
	    "//a/b/c/...",
	    "12345679",
	    "test.user.2",
	    "test2@user",
	    0,
	    "Test description",
	    20000000,
	    "");
	TEST(git.IsHEADExists(), true);
	TEST(git.IsRepositoryClonedFrom("//a/b/c/..."), true);
	TEST(git.IsRepositoryClonedFrom("//a/b/c/d/..."), false);
	TEST(git.IsRepositoryClonedFrom("//x/y/z/..."), false);
	TEST(git.DetectLatestCL(), "12345679");

	// CL refs are enabled by default, so both commits should be resolvable by CL number.
	TEST(git.ResolveCL("12345678"), commitSHA1);
	TEST(git.ResolveCL("12345679"), commitSHA2);
	TEST(git.ResolveCL("99999999"), std::string(""));

	git.CloseIndex();

	// With CL refs disabled, no "refs/cl/<cl>" reference should be created.
	GitAPI gitNoRefs(false, false);
	TEST(gitNoRefs.InitializeRepository("test-repo-no-cl-refs"), true);
	gitNoRefs.CreateIndex();
	gitNoRefs.AddFileToIndex("foo.txt", { 'x', 'y', 'z' }, false);
	gitNoRefs.Commit(
	    "//a/b/c/...",
	    "22345678",
	    "test.user",
	    "test@user",
	    0,
	    "Test description",
	    10000000,
	    "");
	TEST(gitNoRefs.ResolveCL("22345678"), std::string(""));
	gitNoRefs.CloseIndex();

	TEST_END();
	return TEST_EXIT_CODE();
}
