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
 * @file    union_cast.h
 * @author  Przemyslaw Dobrowolski (p.dobrowolsk@samsung.com)
 * @version 1.0
 * @brief   Header file for union cast
 */
#ifndef DPL_UNION_CAST_H
#define DPL_UNION_CAST_H

#include <cstring>

namespace DPL {
template<typename TargetType, typename SourceType>
TargetType union_cast(const SourceType &data)
{
    union
    {
        SourceType source;
        TargetType target;
    } cast;

    std::memset(&cast, 0, sizeof(cast));

    cast.source = data;
    return cast.target;
}
} // namespace DPL

#endif // DPL_UNION_CAST_H