/*
 * Copyright (c) 2022 Salesforce, Inc.
 * All rights reserved.
 * SPDX-License-Identifier: BSD-3-Clause
 * For full license text, see the LICENSE.txt file in the repo root or https://opensource.org/licenses/BSD-3-Clause
 */
#include <iostream>
#include "minitrace.h"

#include "tests.common.h"
#include "tests.utils.h"
#include "tests.git.h"

int main()
{
	// Perforce setup emits events before the trace file is opened.
	MTR_INSTANT("Tests", "Before initialization");
	MTR_INSTANT_I("Tests", "Before initialization with argument", "value", 1);

	// Windows tracing uses critical sections that must be initialized first.
	mtr_init("p4-fusion-test-trace.json");

	TEST_REPORT("Utils", TestUtils());
	TEST_REPORT("GitAPI", TestGitAPI());

	SUCCESS("All test cases passed");
	mtr_shutdown();
	return 0;
}
