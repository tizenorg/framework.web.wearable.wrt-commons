#!/bin/sh
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

/bin/rm -rf /opt/share/widget/system/*
uninstall_widgets=1
if [ "$1" == "--old" ]
then
    /bin/echo "Uninstalling turned off"
    uninstall_widgets=0
fi
#Removing of widget desktop icons
WIDGET_EXEC_PATH=/opt/usr/apps/
WIDGET_PRELOAD_EXEC_PATH=/usr/apps/
WIDGET_DESKTOP_PATH=/opt/share/applications/
SMACK_RULES_PATH=/etc/smack/accesses.d/
WRT_DB=/opt/dbspace/.wrt.db
PKGMGR_DB=/opt/dbspace/.pkgmgr_parser.db
PLUGINS_INSTALLATION_REQUIRED_PATH=/opt/share/widget/
PLUGINS_INSTALLATION_REQUIRED=plugin-installation-required

if [ -f ${WRT_DB} ]
then
    PKG_NAME_SET=$(/usr/bin/sqlite3 $WRT_DB 'select tizen_appid from WidgetInfo;')
    for appid in $PKG_NAME_SET
    do
        if [ $uninstall_widgets -eq 1 ]
        then
            wrt-installer -un $appid 1>/dev/null 2>&1
        fi
        pkgId=`echo "$appid" | cut -f1 -d"."`
        /usr/bin/sqlite3 $PKGMGR_DB "delete from package_info where package=\"$pkgId\""
        /usr/bin/sqlite3 $PKGMGR_DB "delete from package_app_info where app_id=\"$appid\""
        /bin/rm -rf ${WIDGET_EXEC_PATH}${pkgId}
        /bin/rm -rf ${WIDGET_PRELOAD_EXEC_PATH}${pkgId}
        widget_desktop_file="${WIDGET_DESKTOP_PATH}${appid}.desktop";
        if [ -f ${widget_desktop_file} ]; then
            /bin/rm -f $widget_desktop_file;
        fi
        widget_smack_rule="${SMACK_RULES_PATH}${pkgId}"
        if [ -f ${widget_smack_rule} ]; then
            /bin/rm -f $widget_smack_rule;
        fi
    done
else
    echo "${WRT_DB} doesn't exist"
fi

/usr/bin/wrt_commons_create_clean_db.sh

#TODO: remove this when switched to wrt-plugins-installer completely
if [ -e ${PLUGINS_INSTALLATION_REQUIRED_PATH} ] && [ -d ${PLUGINS_INSTALLATION_REQUIRED_PATH} ]
then
    /bin/touch ${PLUGINS_INSTALLATION_REQUIRED_PATH}${PLUGINS_INSTALLATION_REQUIRED}
fi

#update plugins
if [ -x /usr/bin/wrt-installer ]
then
    /usr/bin/wrt-installer -p
fi
