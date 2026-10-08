// Copyright 2013, Google Inc.
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
//
//     * Redistributions of source code must retain the above copyright
// notice, this list of conditions and the following disclaimer.
//     * Redistributions in binary form must reproduce the above
// copyright notice, this list of conditions and the following disclaimer
// in the documentation and/or other materials provided with the
// distribution.
//     * Neither the name of Google Inc. nor the names of its
// contributors may be used to endorse or promote products derived from
// this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

// Verifies that a failure to create the premature-exit file does not abort
// tests.

#include <stdio.h>
#include <stdlib.h>

#include <string>

#include "gtest/gtest.h"

TEST(PrematureExitFileFailureTest, RunsTestDespiteFileOpenFailure) {
  SUCCEED();
}

int main(int argc, char** argv) {
#ifdef GTEST_HAS_DEATH_TEST
  // The executable is a regular file, so a path beneath it cannot be opened.
  const std::string marker_path = std::string(argv[0]) + "/premature_exit";
  FILE* marker = testing::internal::posix::FOpen(marker_path.c_str(), "w");
  if (marker != nullptr) {
    fclose(marker);
    fprintf(stderr, "The premature-exit path must be inaccessible.\n");
    return 1;
  }
#ifdef GTEST_OS_WINDOWS
  if (_putenv_s("TEST_PREMATURE_EXIT_FILE", marker_path.c_str()) != 0) {
#else
  if (setenv("TEST_PREMATURE_EXIT_FILE", marker_path.c_str(), 1) != 0) {
#endif
    fprintf(stderr, "Cannot configure the premature-exit path.\n");
    return 1;
  }
#endif
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
