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
#ifndef DPL_OPTIONAL_TYPEDEFS_H
#define DPL_OPTIONAL_TYPEDEFS_H

#include <string>
#include <dpl/string.h>
#include <dpl/optional.h>

namespace DPL {
typedef Optional<String> OptionalString;
typedef Optional<int> OptionalInt;
typedef Optional<unsigned int> OptionalUInt;
typedef Optional<bool> OptionalBool;
typedef Optional<float> OptionalFloat;
typedef Optional<std::string> OptionalStdString;
} //namespace DPL

#endif /* DPL_OPTIONAL_TYPEDEFS_H */
