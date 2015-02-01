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
 * @file        bind.h
 * @author      Zbigniew Kostrzewa (z.kostrzewa@samsung.com)
 * @version     1.0
 * @brief       This file is the header file of bind
 */

/*
 * Remarks:
 * - Current implementation supports binding member functions (methods) only.
 * - Currently up to 8 std::placeholders are supported.
 * - Bound delegates are of type std::function<...>. This implies that
 *   passing arguments at bind-time is not supported (arguments MAY ONLY be
 *   passed at call-time).
 *
 * Usage:
 * - For usage see tests/core/test_bind.cpp. In general, usage comes down to:
 *   std::function<ResultType(Args...)> delegate =
 *      DPL::Bind(&ObjectType::MethodName, &object);
 */

#ifndef DPL_BIND_H_
#define DPL_BIND_H_

#include <functional>

#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/arithmetic/add.hpp>
#include <boost/preprocessor/repetition/repeat.hpp>

namespace DPL {
namespace detail {
template<size_t>
struct PlaceholdersBindHelper;

#define DPL_PLACEHOLDERS_LIST_(z, n, t) ,BOOST_PP_CAT(std::placeholders::_,    \
                                                      BOOST_PP_ADD(n, 1))
#define DPL_PLACEHOLDERS_(count) BOOST_PP_REPEAT(count,                        \
                                                 DPL_PLACEHOLDERS_LIST_,       \
                                                 count)
#define DPL_PLACEHOLDERS_BIND_HELPER_(count)                                   \
template<>                                                                     \
struct PlaceholdersBindHelper<count>                                           \
{                                                                              \
    template<typename Result, typename Type, typename ...Args>                 \
    static std::function<Result(Args...)>                                      \
    bind(Result(Type::*method)(Args...), Type* object) {                       \
        return std::bind(method, object DPL_PLACEHOLDERS_(count));             \
    }                                                                          \
}

DPL_PLACEHOLDERS_BIND_HELPER_(0);
DPL_PLACEHOLDERS_BIND_HELPER_(1);
DPL_PLACEHOLDERS_BIND_HELPER_(2);
DPL_PLACEHOLDERS_BIND_HELPER_(3);
DPL_PLACEHOLDERS_BIND_HELPER_(4);
DPL_PLACEHOLDERS_BIND_HELPER_(5);
DPL_PLACEHOLDERS_BIND_HELPER_(6);
DPL_PLACEHOLDERS_BIND_HELPER_(7);
DPL_PLACEHOLDERS_BIND_HELPER_(8);
}

template<typename Result, typename Type, typename ...Args>
std::function<Result(Args...)> Bind(Result(Type::*method)(Args...),
                                    Type* object)
{
    return detail::PlaceholdersBindHelper<sizeof...(Args)>::bind(method,
                                                                 object);
}
}

#endif // DPL_BIND_H_
