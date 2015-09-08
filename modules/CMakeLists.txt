# Copyright (c) 2011 Samsung Electronics Co., Ltd All Rights Reserved
#
#    Licensed under the Apache License, Version 2.0 (the "License");
#    you may not use this file except in compliance with the License.
#    You may obtain a copy of the License at
#
#        http://www.apache.org/licenses/LICENSE-2.0
#
#    Unless required by applicable law or agreed to in writing, software
#    distributed under the License is distributed on an "AS IS" BASIS,
#    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
#    See the License for the specific language governing permissions and
#    limitations under the License.
#
#
# @file        CMakeLists.txt
# @author      Lukasz Marek (l.marek@samsung.com)
# @version     1.0
# @brief
#

INCLUDE(core/config.cmake)
INCLUDE(dbus/config.cmake)
INCLUDE(db/config.cmake)
INCLUDE(event/config.cmake)
INCLUDE(socket/config.cmake)
INCLUDE(rpc/config.cmake)
INCLUDE(test/config.cmake)
INCLUDE(log/config.cmake)
ADD_SUBDIRECTORY(widget_dao)
ADD_SUBDIRECTORY(widget_interface_dao)
ADD_SUBDIRECTORY(security_origin_dao)
ADD_SUBDIRECTORY(custom_handler_dao)
ADD_SUBDIRECTORY(certificate_dao)
ADD_SUBDIRECTORY(i18n)
INCLUDE(utils/config.cmake)
INCLUDE(localization/config.cmake)
INCLUDE(support/config.cmake)
