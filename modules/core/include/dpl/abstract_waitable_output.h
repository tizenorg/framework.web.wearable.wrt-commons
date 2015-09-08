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
 * @file        abstract_waitable_output.h
 * @author      Przemyslaw Dobrowolski (p.dobrowolsk@samsung.com)
 * @version     1.0
 * @brief       This file is the header file of abstract waitable output
 */
#ifndef DPL_ABSTRACT_WAITABLE_OUTPUT_H
#define DPL_ABSTRACT_WAITABLE_OUTPUT_H

#include <dpl/abstract_output.h>
#include <dpl/waitable_handle.h>

namespace DPL {
class AbstractWaitableOutput :
    public AbstractOutput
{
  public:
    virtual ~AbstractWaitableOutput() {}

    virtual WaitableHandle WaitableWriteHandle() const = 0;
};
} // namespace DPL

#endif // DPL_ABSTRACT_WAITABLE_OUTPUT_H
