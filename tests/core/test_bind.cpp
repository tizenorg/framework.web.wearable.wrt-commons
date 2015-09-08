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
 * @file        test_bind.cpp
 * @author      Zbigniew Kostrzewa (z.kostrzewa@samsung.com)
 * @version     1.0
 * @brief       This file is the implementation file of test bind
 */
#include <functional>

#include <dpl/test/test_runner.h>
#include <dpl/bind.h>

namespace {
const int MAGIC_INT = 42;
const double MAGIC_DOUBLE = 42.42;

struct BindTestBase {
    BindTestBase() : called{false} {}

    bool called;
};

struct BindNoArguments : BindTestBase {
    void foo() { called = true; }
};

struct BindWithArguments : BindTestBase {
    void foo(int, double) { called = true; }
};

struct BindWithArgumentsAndResult : BindTestBase {
    int foo(int, double) { called = true; return MAGIC_INT; }
};
}

RUNNER_TEST_GROUP_INIT(DPL)

/*
Name: Bind_NoArguments
Description: tests binding method without arguments
Expected: bound method called
*/
RUNNER_TEST(Bind_NoArguments)
{
    BindNoArguments test;

    std::function<void()> delegate = DPL::Bind(&BindNoArguments::foo, &test);
    delegate();

    RUNNER_ASSERT(test.called);
}

/*
Name: Bind_WithArguments
Description: tests binding method with arguments
Expected: bound method called
*/
RUNNER_TEST(Bind_WithArguments)
{
    BindWithArguments test;

    std::function<void(int, double)> delegate =
        DPL::Bind(&BindWithArguments::foo, &test);
    delegate(MAGIC_INT, MAGIC_DOUBLE);

    RUNNER_ASSERT(test.called);
}

/*
Name: Bind_WithArgumentsAndResult
Description: tests binding method with arguments and result
Expected: bound method called
*/
RUNNER_TEST(Bind_WithArgumentsAndResult)
{
    BindWithArgumentsAndResult test;

    std::function<int(int, double)> delegate =
        DPL::Bind(&BindWithArgumentsAndResult::foo, &test);
    int result = delegate(MAGIC_INT, MAGIC_DOUBLE);

    RUNNER_ASSERT(test.called);
    RUNNER_ASSERT(MAGIC_INT == result);
}
