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
 * @file        bool_operator.h
 * @author      Przemyslaw Dobrowolski (p.dobrowolsk@samsung.com)
 * @version     1.0
 * @brief       This file is the implementation file of bool operator
 *
 * This module is deprecated, please use standard C++11 feature: explicit conversion operator
 *
 * example:
 *  explicit operator bool() {};
 */
#ifndef DPL_BOOL_OPERATOR_H
#define DPL_BOOL_OPERATOR_H

#include <dpl/availability.h>

#define DPL_IMPLEMENT_BOOL_OPERATOR(Type, ThisType, CheckPtr, ClassPtr)                        \
    typedef Type *ThisType::*UnknownBoolType;                                                  \
                                                                                               \
    operator UnknownBoolType() const                                                           \
    {                                                                                          \
        return (CheckPtr == NULL) ? NULL : &ThisType::ClassPtr;                                \
    } DPL_DEPRECATED_WITH_MESSAGE("use standard C++11 feature: explicit conversion operator"); \
                                                                                               \
    bool operator !() const                                                                    \
    {                                                                                          \
        return CheckPtr == NULL;                                                               \
    } DPL_DEPRECATED_WITH_MESSAGE("use standard C++11 feature: explicit conversion operator"); \

#endif // DPL_BOOL_OPERATOR_H