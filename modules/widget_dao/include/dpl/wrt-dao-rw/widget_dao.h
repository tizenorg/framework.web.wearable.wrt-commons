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
 * This file contains the declaration of widget dao class.
 *
 * @file    widget_dao.h
 * @author  Yang Jie (jie2.yang@samsung.com)
 * @author  Przemyslaw Dobrowolski (p.dobrowolsk@samsung.com)
 * @author  Pawel Sikorski (p.sikorski@samsung.com)
 * @version 1.0
 * @brief   This file contains the declaration of widget dao
 */
#ifndef WIDGET_DAO_H
#define WIDGET_DAO_H

#include <dpl/wrt-dao-ro/widget_dao_read_only.h>
#include <list>
#include <string>
#include <sys/time.h>
#include <ctime>
#include <cstdlib>
#include <dpl/availability.h>
#include <dpl/exception.h>
#include <dpl/db/orm.h>
#include <dpl/wrt-dao-ro/config_parser_data.h>
#include <dpl/wrt-dao-rw/property_dao.h>

namespace WrtDB {
class WidgetDAO : public WidgetDAOReadOnly
{
  public:
    typedef std::list<DPL::String> LanguageTagsList;

    WidgetDAO(DPL::OptionalString widgetGUID);
    WidgetDAO(DPL::String tzAppId);

    /**
     * Destructor
     */
    virtual ~WidgetDAO();

    /**
     * This method registers the widget information in the DB when it is
     * installed.
     *
     * @see WidgetRegisterInfo
     * @see UnRegisterWidget()
     * @param[in] TizenAppId Widget app id that will be registered.
     * @param[in] pWidgetRegisterInfo    Specified the widget's information
     * needed to be registered.
     * @param[in] widgetSecurity   Widget's security certificates.
     */
    static void registerWidget(
        const TizenAppId& tzAppId,
        const WidgetRegisterInfo &widgetRegInfo,
        const IWidgetSecurity &widgetSecurity);

    static void registerService(
        const ConfigParserData::ServiceAppInfo &serviceAppInfo,
        const WidgetRegisterInfo &widgetRegInfo,
        const IWidgetSecurity &widgetSecurity);

    /**
     * @brief registerWidgetGenerateTizenId Registers widget with auto-generated
     * tizen id
     *
     * This function is disadviced and should be used only in tests.
     * Function is not thread-safe.
     *
     * @param pWidgetRegisterInfo registeration information
     * @param widgetSecurity Widget's security certificates.
     * @return tzAppId generated
     */
    static TizenAppId registerWidgetGeneratePkgId(
        const WidgetRegisterInfo &pWidgetRegisterInfo,
        const IWidgetSecurity &widgetSecurity);

    static void updateTizenAppId(const TizenAppId & fromAppId,
                                 const TizenAppId & toAppId);
    /**
     * This method removes a widget's information from EmDB.
     *
     * @see RegisterWidget()
     * @param[in] tzAppId widgets name to be unregistered
     */
    static void unregisterWidget(const TizenAppId & tzAppId);

    /* This method removes widget property
     */
    void removeProperty(const PropertyDAOReadOnly::WidgetPropertyKey &key);

    /**
     * @brief registerExternalLocations Removes rows from
     * WidgetExternalLocations
     */
    void unregisterAllExternalLocations();

    /* This method sets widget property
     */
    void setProperty(const PropertyDAOReadOnly::WidgetPropertyKey &key,
                     const PropertyDAOReadOnly::WidgetPropertyValue &value,
                     bool readOnly = false);

    /* set tzAppId
     */
    void setTizenAppId(const DPL::OptionalString& tzAppId);

    /* This function will update of api-feature status.
     * If status is true (feature rejected) plugin connected with this
     * api feature mustn't be loaded durign widget launch.
     */
    void updateFeatureRejectStatus(const DbWidgetFeature &widgetFeature);

  private:
    //Methods used during widget registering
    static DbWidgetHandle registerWidgetInfo(
        const TizenAppId & tzAppId,
        const TizenPkgId & tzPkgId,
        const std::string & baseFolder,
        AppType appType,
        PkgType pkgType,
        const ConfigParserData &widgetConfigurationInfo,
        const IWidgetSecurity &widgetSecurity,
        const DPL::Optional<DbWidgetHandle> handle =
            DPL::Optional<DbWidgetHandle>());
    static void registerWidgetExtendedInfo(DbWidgetHandle widgetHandle,
        time_t installedTime,
        const DPL::OptionalString & splashImgSrc, const DPL::OptionalString & backgroundPage,
        const DPL::OptionalString & widgetInstalledPath);
    static void registerWidgetLocalizedInfo(
        DbWidgetHandle widgetHandle,
        const ConfigParserData::LocalizedDataSet &localizedDataSet);
    static void registerWidgetIcons(
        DbWidgetHandle widgetHandle,
        const WidgetRegisterInfo::LocalizedIconList &icons);
    static void registerWidgetStartFile(
        DbWidgetHandle widgetHandle,
        const WidgetRegisterInfo::LocalizedStartFileList &startFiles);
    static void registerWidgetFeatures(
        DbWidgetHandle widgetHandle,
        const ConfigParserData::FeaturesList &featuresList);
    static void registerWidgetPrivilege(
        DbWidgetHandle widgetHandle,
        const ConfigParserData::PrivilegeList &privilegeList);
    static void registerWidgetWindowModes(
        DbWidgetHandle widgetHandle,
        const ConfigParserData::StringsList &windowModes);
    static void registerWidgetWarpInfo(
        DbWidgetHandle widgetHandle,
        const ConfigParserData::AccessInfoSet &accessInfoSet);
    static void registerWidgetAllowNavigationInfo(
        DbWidgetHandle widgetHandle,
        const ConfigParserData::AllowNavigationInfoList &allowNavigationInfoList);
    static void registerWidgetCertificates(
        DbWidgetHandle widgetHandle,
        const IWidgetSecurity &widgetSecurity);
    static void registerCertificatesChains(
        DbWidgetHandle widgetHandle,
        CertificateSource certificateSource,
        const CertificateChainList &list);
    static void registerWidgetSettings(
        DbWidgetHandle widgetHandle,
         const  ConfigParserData::SettingsList &settingsList);
    static void registerAppControl(
        DbWidgetHandle widgetHandle,
        const ConfigParserData::AppControlInfoList &appControlList);
    static void registerEncryptedResouceInfo(
        DbWidgetHandle widgetHandle,
        const EncryptedFileList &encryptedFiles);

    /**
     * @brief registerExternalLocations Inserts new rows to
     * WidgetExternalLocations
     * @param externals list of files
     */
    static void registerExternalLocations(
        DbWidgetHandle widgetHandle,
        const ExternalLocationList &
        externals);

    static void registerServiceInternal(const ConfigParserData::ServiceAppInfo &serviceAppInfo,
    const WidgetRegisterInfo &widgetRegInfo, const IWidgetSecurity &widgetSecurity);

    static void registerWidgetInternal(
        const TizenAppId & tzAppId,
        const WidgetRegisterInfo &widgetRegInfo,
        const IWidgetSecurity &widgetSecurity,
        const DPL::Optional<DbWidgetHandle> handle =
            DPL::Optional<DbWidgetHandle>());
    static void unregisterWidgetInternal(const TizenAppId & tzAppId);
    static void insertAppControlInfo(DbWidgetHandle handle,
                                     DPL::String src,
                                     DPL::String operation,
                                     DPL::String uri,
                                     DPL::String mime,
                                     unsigned index,
                                     unsigned disposition);
};
} // namespace WrtDB

#endif // WIDGET_DAO_H
