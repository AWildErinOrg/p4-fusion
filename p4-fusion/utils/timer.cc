/*
 * Copyright (c) 2022 Salesforce, Inc.
 * All rights reserved.
 * SPDX-License-Identifier: BSD-3-Clause
 * For full license text, see the LICENSE.txt file in the repo root or https://opensource.org/licenses/BSD-3-Clause
 */
#include "timer.h"

namespace P4Fusion
{

const std::chrono::high_resolution_clock Timer::s_Clock;

Timer::Timer()
    : m_StartTime(Now())
{
}

} // namespace P4Fusion
