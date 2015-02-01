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
/**
 * @file    wrt_global_settings.h
 * @version    0.6
 * @author    Pawel Sikorski(p.sikorski@samsung.com)
 * @brief     Header file for global predefined wrt setting
 */

#ifndef WRT_COMMON_GLOBAL_SETTINGS_H_
#define WRT_COMMON_GLOBAL_SETTINGS_H_

namespace GlobalSettings {
// Methods for getting test mode environment flag
bool TestModeEnabled();
bool PopupsTestModeEnabled();
bool WarpTestModeEnabled();
bool RoamingTestModeEnabled();
bool OCSPTestModeEnabled();
bool CrlTestModeEnabled();
bool MakeScreenTestModeEnabled();
bool IsEmulator();
}

#endif /* WRT_COMMON_GLOBAL_SETTINGS_H_ */