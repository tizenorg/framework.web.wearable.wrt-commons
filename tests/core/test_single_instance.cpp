/*
 * Copyright (c) 2013 Samsung Electronics Co., Ltd All Rights Reserved
 *
 *    Licensed under the Apache License, Version 2.0 (the "License");
 *    you may not use this file except in compliance with the License.
 *    You may obtain a copy of the License at
 *
 *        http://www.apache.org/licenses/LICENSE-2.0
 *
 *    Unless required by applicable law or agreed to in writing, software
 *    distributed under the License is distributed on an "AS IS" BASIS,
 *    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *    See the License for the specific language governing permissions and
 *    limitations under the License.
 */
/*
 * @file        test_single_instance.cpp
 * @author      Slawomir Pajak (s.pajak@partner.samsung.com)
 * @version     1.0
 * @brief       This file is the implementation file of single instance tests
 */
#include <dpl/test/test_runner.h>
#include <dpl/single_instance.h>

RUNNER_TEST_GROUP_INIT(DPL)

using namespace DPL;

/*
Name: SingleInstance_lock_release
Description: tests single instance simple case
Expected: all operations succeed, no exceptions
*/
RUNNER_TEST(SingleInstance_lock_release)
{
    SingleInstance instance;
    RUNNER_ASSERT(instance.TryLock("testLockFile"));

    instance.Release();
    //Multiple call should be fine.
    instance.Release();

}
