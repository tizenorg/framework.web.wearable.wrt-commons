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
 * @file        platform.h
 * @author      Jihoon Chung (jihoon.chung@samsung.com)
 * @version     1.0
 */
#ifndef DPL_PLATFORM_H
#define DPL_PLATFORM_H

// Use Features definition
//   Use a particular optional platform service or third-party library
//
// Description : <text>
//               <text>
// Author : <text>(<email>) - <date>
// #define USE_<DEPENDENT_MODULE_NAME>_<FEATURE_NAME> <value>(0 or 1)
#define USE(FEATURE) (defined USE_##FEATURE && USE_##FEATURE)

// Description : Application side patch to use manual rotation feature in webkit
// Author : Jihoon Chung(jihoon.chung@samsung.com) - 10.26.2013
#define USE_WEBKIT_MANUAL_ROTATION 1

// Description : Temporary patch about enable/disable webapp specific settings, especially for web-provider.
// Author : Jihoon Chung(jihoon.chung@samsung.com) - 11.09.2013
#define USE_WEB_PROVIDER_EXCEPTION_IN_EWK_CONTEXT 1

// Description : Enhanced "progress bar" user experience.
//               Show "progress bar" in "load,started" webkit callback to show earlier.
//               Confirmed by webkit loader team.
// Author : Jihoon Chung(jihoon.chung@samsung.com) - 11.21.2013
#define USE_WEBKIT_SHOW_PROGRESS_BAR_EARLIER 1

// Description : Webkit version-up
// Author : Jihoon Chung(jihoon.chung@samsung.com) - 01.14.2013
#define USE_WEBKIT_UPVERSION 0

// Enable Features definition
//   Turn on a specific feature of WRT
//
// Description : <text>
//               <text>
// Author : <text>(<email>) - <date>
// #define ENABLE_<FEATURE_NAME> <value>(0 or 1)
#define ENABLE(FEATURE) (defined ENABLE_##FEATURE && ENABLE_##FEATURE)

// Description : Support onbeforeunload event
// Author : Jihoon Chung(jihoon.chung@samsung.com) - 11.15.2013
#define ENABLE_JAVASCRIPT_ONBEFOREUNLOAD_EVENT 0

// Description : Support app scheme(app://)
//               App scheme(app://) supports special scheme to packaged resources.
//               Main reason of requirement is that enhance cross origin security and deprecate file scheme(file://).
// Author : Tomasz Iwanek(t.iwanek@samsung.com) - 11.16.2013
#define ENABLE_APP_SCHEME 0

// Description : Custom user agent support by tizen widget setting
// Author : Tae-Jeong Lee (taejeong.lee@samsung.com) - 11.26.2013
#define ENABLE_CUSTOM_USER_AGENT_SUPPORT 0

// Description : Support CORS(Cross-origin resource sharing) whitelisting
//               Allow to use resource by trust origin. Basically, trust domain includes own app scheme(app://).
// Author : Tomasz Iwanek(t.iwanek@samsung.com) - 01.02.2014
#define ENABLE_CORS_WHITELISTING 0

// Description : Support CSP(Content security policy)
// Author : Tomasz Iwanek(t.iwanek@samsung.com) - 01.03.2014
#define ENABLE_CONTENT_SECURITY_POLICY 1

// Description : Enabling background decryption for encrypted resources
// Author : Tomasz Iwanek(t.iwanek@samsung.com) - 16.12.2013
#define ENABLE_BACKGROUND_THREAD_DECRYPTION 0

// Description : Support allow-navigation
//               Origin based navigation control of main resource.
// Author : Jihoon Chung (jihoon.chung@samsung.com) - 01.08.2014
#define ENABLE_ALLOW_NAVIGATION 1

#endif // DPL_PLATFORM_H
