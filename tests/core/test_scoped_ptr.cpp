/*
 * Copyright (c) 2011 Samsung Electronics Co., Ltd All Rights Reserved
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
 * @file        test_scoped_ptr.cpp
 * @author      Przemyslaw Dobrowolski (p.dobrowolsk@samsung.com)
 * @version     1.0
 * @brief       This file is the implementation file of test scoped ptr
 */
#include <dpl/test/test_runner.h>
#include <dpl/scoped_ptr.h>

RUNNER_TEST_GROUP_INIT(DPL)

/*
Name: ScopedPtr_Zero
Description: Checks if operator! works
Expected: resource should be not set
*/
RUNNER_TEST(ScopedPtr_Zero)
{
    DPL::ScopedPtr<char> ptr;

    RUNNER_ASSERT(!ptr);
    RUNNER_ASSERT(!!!ptr);
}

/*
Name: ScopedPtr_NonZero
Description: Checks if operator! works
Expected: resource should be set
*/
RUNNER_TEST(ScopedPtr_NonZero)
{
    DPL::ScopedPtr<char> ptr(new char(7));

    RUNNER_ASSERT(ptr);
    RUNNER_ASSERT(!!ptr);
}

/*
Name: ScopedPtr_Reset
Description: Checks reseting scoped ptr
Expected: resource should be not set after reset
*/
RUNNER_TEST(ScopedPtr_Reset)
{
    DPL::ScopedPtr<char> ptr(new char(7));
    ptr.Reset();

    RUNNER_ASSERT(!ptr);

    ptr.Reset(new char);
    RUNNER_ASSERT(ptr);
}

/*
Name: ScopedPtr_Operators
Description: Checks access operator
Expected: address of resource should be same as this, received from Get() method
*/
RUNNER_TEST(ScopedPtr_Operators)
{
    DPL::ScopedPtr<char> ptr(new char(7));

    RUNNER_ASSERT(*ptr == *ptr.Get());
}
