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
 * @file    widget_dao_read_only.h
 * @author  Yang Jie (jie2.yang@samsung.com)
 * @author  Przemyslaw Dobrowolski (p.dobrowolsk@samsung.com)
 * @author  Pawel Sikorski (p.sikorski@samsung.com)
 * @version 1.0
 * @brief   This file contains the declaration of widget dao
 */

#ifndef _WRT_SRC_CONFIGURATION_WIDGET_DAO_READ_ONLY_H_
#define _WRT_SRC_CONFIGURATION_WIDGET_DAO_READ_ONLY_H_

#include <time.h>
#include <list>
#include <string>
#include <dpl/availability.h>
#include <dpl/string.h>
#include <dpl/exception.h>
#include <dpl/db/orm.h>
#include <dpl/optional_typedefs.h>
#include <dpl/wrt-dao-ro/config_parser_data.h>
#include <dpl/wrt-dao-ro/property_dao_read_only.h>
#include <dpl/wrt-dao-ro/common_dao_types.h>

namespace WrtDB {
enum CertificateSource {
    SIGNATURE_AUTHOR = 0,
    SIGNATURE_DISTRIBUTOR = 1,
    SIGNATURE_DISTRIBUTOR2 = 2,
    SIGNATURE_UNKNOWN = 3
};

struct WidgetLocalizedInfo
{
    DPL::OptionalString name;
    DPL::OptionalString shortName;
    DPL::OptionalString description;
    DPL::OptionalString license;
    DPL::OptionalString licenseHref;
};

/**
 * CertificateData
 * A structure to hold certificate fingerprints.
 */
struct WidgetCertificateData
{
    enum Owner { AUTHOR, DISTRIBUTOR, DISTRIBUTOR2, UNKNOWN };
    enum Type { ROOT, ENDENTITY };

    // type of signature: author/distributor
    Owner owner;
    // indicates whether this is ca certificate
    Type type;

    // chain id number: relative BASE, where BASE is signatureBASE.xml
    int chainId;
    // certificate fingerprint digested by md5
    std::string strMD5Fingerprint;
    // certificate fingerprint digestef by sha1
    std::string strSHA1Fingerprint;
    // Common name field in certificate
    DPL::String strCommonName;

    bool operator== (const WidgetCertificateData& certData) const
    {
        return certData.chainId == chainId &&
               certData.owner == owner &&
               certData.strCommonName == strCommonName &&
               certData.strMD5Fingerprint == strMD5Fingerprint &&
               certData.strSHA1Fingerprint == strSHA1Fingerprint;
    }
};

typedef std::list<WidgetCertificateData> WidgetCertificateDataList;

typedef DPL::String Locale;
typedef std::set<Locale> LocaleSet;
typedef std::list<std::string> ExternalLocationList;

/**
 * WidgetRegisterInfo
 * A structure to hold widget's information needed to be registered.
 * @see WidgetConfigurationInfo
 */
struct WidgetRegisterInfo
{
    struct LocalizedIcon : public ConfigParserData::Icon
    {
        LocalizedIcon(const ConfigParserData::Icon& icon,
                      const LocaleSet& _availableLocales) :
            ConfigParserData::Icon(icon),
            availableLocales(_availableLocales)
        {}

        LocaleSet availableLocales;
    };

    struct StartFileProperties
    {
        DPL::String encoding;
        DPL::String type;
    };

    typedef std::map<Locale,
                     StartFileProperties> StartFilePropertiesForLocalesMap;
    struct LocalizedStartFile
    {
        DPL::String path;
        StartFilePropertiesForLocalesMap propertiesForLocales;
    };

    typedef std::list<LocalizedIcon> LocalizedIconList;
    typedef std::list<LocalizedStartFile> LocalizedStartFileList;
    struct LocalizationData
    {
        LocalizedIconList icons;
        LocalizedStartFileList startFiles;
    };

    //Constructor
    WidgetRegisterInfo() :
        webAppType(APP_TYPE_UNKNOWN),
        configInfo(),
        packagingType(PKG_TYPE_UNKNOWN)
    {}

    WidgetType webAppType;
    DPL::OptionalString guid;
    DPL::OptionalString version;
    DPL::OptionalString minVersion;
    std::string baseFolder;
    ConfigParserData configInfo;
    LocalizationData localizationData;

    TizenPkgId tzPkgid;
    TizenAppId tzAppid;
    TizenAppId tzBackupAppid;

    time_t installedTime;
    PackagingType packagingType;
    EncryptedFileList encryptedFiles;
    ExternalLocationList externalLocations;
    DPL::OptionalString widgetInstalledPath;
};

typedef std::list<std::string> CertificateChainList;
class IWidgetSecurity
{
  public:
    virtual ~IWidgetSecurity();

    virtual const WidgetCertificateDataList& getCertificateList() const = 0;

    virtual bool isRecognized() const = 0;

    virtual bool isDistributorSigned() const = 0;

    virtual void getCertificateChainList(CertificateChainList& list,
                                         CertificateSource source) const = 0;
};

/**
 * WidgetAuthorInfo.
 * Structure to hold the information of widget's author.
 */
struct WidgetAuthorInfo
{
    DPL::OptionalString name;
    DPL::OptionalString email;
    DPL::OptionalString href;
};

typedef std::list <std::string> WidgetCertificateCNList;
typedef std::list<DPL::String> LanguageTagList;
typedef std::list<std::string> HostList;
typedef std::list<std::string> FingerPrintList;

class WidgetDAOReadOnly
{
  public:
    /**
     * WidgetDAO Exception classes
     */
    class Exception
    {
      public:
        DECLARE_EXCEPTION_TYPE(DPL::Exception, Base)
        DECLARE_EXCEPTION_TYPE(Base, DatabaseError)
        DECLARE_EXCEPTION_TYPE(Base, ReadOnlyProperty)
        DECLARE_EXCEPTION_TYPE(Base, GUIDisNull)
        DECLARE_EXCEPTION_TYPE(Base, UnexpectedEmptyResult)
        DECLARE_EXCEPTION_TYPE(Base, WidgetNotExist)
        DECLARE_EXCEPTION_TYPE(Base, AlreadyRegistered)
    };

  protected:
    DbWidgetHandle m_widgetHandle;

  public:
    struct WidgetLocalizedIconRow
    {
        int appId;
        int iconId;
        DPL::String widgetLocale;
    };
    typedef std::list<WidgetLocalizedIconRow> WidgetLocalizedIconList;

    struct WidgetIconRow
    {
        int iconId;
        int appId;
        DPL::String iconSrc;
        DPL::OptionalInt iconWidth;
        DPL::OptionalInt iconHeight;
    };
    typedef std::list<WidgetIconRow> WidgetIconList;

    struct WidgetStartFileRow
    {
        int startFileId;
        int appId;
        DPL::String src;
    };
    typedef std::list<WidgetStartFileRow> WidgetStartFileList;

    struct WidgetLocalizedStartFileRow
    {
        int startFileId;
        int appId;
        DPL::String widgetLocale;
        DPL::String type;
        DPL::String encoding;
    };
    typedef std::list<WidgetLocalizedStartFileRow> LocalizedStartFileList;

    /**
     * This is a constructor.
     *
     * @param[in] widgetHandle application id of widget.
     */
    WidgetDAOReadOnly(DbWidgetHandle widgetHandle);
    WidgetDAOReadOnly(DPL::OptionalString widgetGUID);
    WidgetDAOReadOnly(WrtDB::TizenAppId tzAppid);

    /**
     * Destructor
     */
    virtual ~WidgetDAOReadOnly();

    /**
     * This method returns widget handle(m_widgetHandle).
     *
     * @return widget handle(m_widgetHandle).
     * @exception WRT_CONF_ERR_EMDB_FAILURE - Fail to query DB table.
     * @exception WRT_CONF_ERR_EMDB_NO_RECORD - Can not find matching records in
     * DB table.
     */
    DbWidgetHandle getHandle() const;
    static DbWidgetHandle getHandle(const WidgetGUID GUID);
    static DbWidgetHandle getHandle(const DPL::String tzAppId);
    static DbWidgetHandle getHandleByPkgId(const DPL::String pkgId);

    /**
     * This method Returns tizen application id for the specified web application
     * @return TizenAppId
     */
    TizenAppId getTzAppId() const DPL_DEPRECATED_WITH_MESSAGE("Use getTizenAppId");
    static TizenAppId getTzAppId(const WidgetGUID GUID) DPL_DEPRECATED_WITH_MESSAGE("Use getTizenAppId");
    static TizenAppId getTzAppId(const DbWidgetHandle handle) DPL_DEPRECATED_WITH_MESSAGE("Use getTizenAppId");
    static TizenAppId getTzAppId(const TizenPkgId tizenPkgId) DPL_DEPRECATED_WITH_MESSAGE("Use getTizenAppId");
    TizenAppId getTizenAppId() const;
    static TizenAppId getTizenAppId(const WidgetGUID GUID);
    static TizenAppId getTizenAppId(const DbWidgetHandle handle);
    static TizenAppId getTizenAppId(const TizenPkgId tizenPkgId);

    /**
     * This method returns list of installed tizen application id
     * @return TizenAppIdList
     */
    static TizenAppIdList getTizenAppidList() DPL_DEPRECATED_WITH_MESSAGE("Use getTizenAppIdList");
    static TizenAppIdList getTizenAppIdList();
    static std::list<TizenAppId> getTzAppIdList(const TizenPkgId tzPkgid);

    /**
     * Returns TizenPkgId for the specified widget
     *
     * @return TizenPkgId;
     * @exception WRT_CONF_ERR_EMDB_FAILURE - Fail to query DB table.
     * @exception WRT_CONF_ERR_EMDB_NO_RECORD - Can not find matching records in
     * DB table.
     */

    TizenPkgId getTzPkgId() const DPL_DEPRECATED_WITH_MESSAGE("Use getTizenPkgId");
    static TizenPkgId getTzPkgId(const DbWidgetHandle handle) DPL_DEPRECATED_WITH_MESSAGE("Use getTizenPkgId");
    static TizenPkgId getTzPkgId(const TizenAppId tzAppid) DPL_DEPRECATED_WITH_MESSAGE("Use getTizenPkgId");
    TizenPkgId getTizenPkgId() const;
    static TizenPkgId getTizenPkgId(const DbWidgetHandle handle);
    static TizenPkgId getTizenPkgId(const TizenAppId tzAppid);

    /**
     * This method returns list of tizen package list of installed packages
     * @return list of TizenPkgIdList of installed packages
     */
    static TizenPkgIdList getTizenPkgidList() DPL_DEPRECATED_WITH_MESSAGE("Use getTizenPkgIdList");
    static TizenPkgIdList getTizenPkgIdList();

    /**
     * This method returns the root directory of widget resource.
     *
     * @return path name of root directory.
     * @exception WRT_CONF_ERR_EMDB_FAILURE - Fail to query DB table.
     * @exception WRT_CONF_ERR_EMDB_NO_RECORD - Can not find matching records in
     * DB table.
     */
    virtual DPL::String getPath() const;

    DPL::String getFullPath() const;

    /**
     * This method returns the preferred size of the widget,
     * including width and height.
     *
     * @see DbWidgetSize
     * @return An structure type variable to hold widget's size.
     * @exception WRT_CONF_ERR_EMDB_FAILURE - Fail to query DB table.
     * @exception WRT_CONF_ERR_EMDB_NO_RECORD - Can not find matching
     *                                          DB table.
     */
    DbWidgetSize getPreferredSize() const;

    /**
     * This method returns the type of the widget.
     *
     * @return WidgetType
     * @exception WRT_CONF_ERR_EMDB_FAILURE - Fail to query DB table.
     * @exception WRT_CONF_ERR_EMDB_NO_RECORD - Can not find matching
     *                                          records in DB table.
     */
    WidgetType getWidgetType() const;

    /**
     * This method returns the id of the widget.
     *
     * @return widget id
     * @exception WRT_CONF_ERR_EMDB_FAILURE - Fail to query DB table.
     * @exception WRT_CONF_ERR_EMDB_NO_RECORD - Can not find matching records in
     *  DB table.
     */
    WidgetGUID getGUID() const;

    /**
     * This method returns the defaultlocale for the widget.
     *
     * @return defaultlocale
     * @exception WRT_CONF_ERR_EMDB_FAILURE - Fail to query DB table.
     * @exception WRT_CONF_ERR_EMDB_NO_RECORD - Can not find matching records in
     *  DB table.
     */
    DPL::OptionalString getDefaultlocale() const;

    /**
     * This method returns list of localized icons files;
     *
     * @exception WRT_CONF_ERR_EMDB_FAILURE - Fail to query DB table.
     * @exception WRT_CONF_ERR_EMDB_NO_RECORD - Can not find matching records in
     *  DB table.
     */
    WidgetLocalizedIconList getLocalizedIconList() const;

    /**
     * This method returns list of icons files;
     *
     * @exception WRT_CONF_ERR_EMDB_FAILURE - Fail to query DB table.
     * @exception WRT_CONF_ERR_EMDB_NO_RECORD - Can not find matching records in
     *  DB table.
     */
    WidgetIconList getIconList() const;

    /**
     * This method returns list of localized start files;
     *
     * @exception WRT_CONF_ERR_EMDB_FAILURE - Fail to query DB table.
     * @exception WRT_CONF_ERR_EMDB_NO_RECORD - Can not find matching records in
     *  DB table.
     */
    LocalizedStartFileList getLocalizedStartFileList() const;

    /**
     * This method returns list of start files;
     *
     * @exception WRT_CONF_ERR_EMDB_FAILURE - Fail to query DB table.
     * @exception WRT_CONF_ERR_EMDB_NO_RECORD - Can not find matching records in
     *  DB table.
     */
    WidgetStartFileList getStartFileList() const;

    /**
     * @param[out] outAccessInfoList list filled with access info structures
     */
    void getWidgetAccessInfo(WidgetAccessInfoList& outAccessInfoList) const;
    void getWidgetAllowNavigationInfo(
        WidgetAllowNavigationInfoList& allowNavigationInfoList) const;

    /**
     * This method returns window mode of widget.
     *
     * @return window modes of widget
     * @exception WRT_CONF_ERR_EMDB_FAILURE - Fail to query DB table.
     * @exception WRT_CONF_ERR_EMDB_NO_RECORD - Can not find matching records in
     *  DB table.
     */
    WindowModeList getWindowModes() const;

    /**
     * This method returns the version of the widget.
     *
     * @return version of widget
     * @exception WRT_CONF_ERR_EMDB_FAILURE - Fail to query DB table.
     * @exception WRT_CONF_ERR_EMDB_NO_RECORD - Can not find matching records in
     *  DB table.
     */
    DPL::OptionalString getVersion() const;

    /**
     * This method is used as a getter for csp policy of widget. It should be
     * provided in configuration file.
     * @return global csp policy for widget
     */
    DPL::OptionalString getCspPolicy() const;

    /**
     * This method is used as a getter for report only csp policy of widget.
     * It may be provided in configuration file.
     * @return global csp report only policy for widget
     */
    DPL::OptionalString getCspPolicyReportOnly() const;

    /**
     * This method returns list filed with Common Name entries from certificate.
     *
     * @return Common Name of Distribuotor End Entity certificate.
     * @exception WRT_CONF_ERR_EMDB_FAILURE - Fail to query DB table.
     * @exception WRT_CONF_ERR_EMDB_NO_RECORD - Can not find matching records in
     *  DB table.
     */
    WidgetCertificateCNList getKeyCommonNameList(
        WidgetCertificateData::Owner owner,
        WidgetCertificateData::Type type) const;

    /**
     * given a certificate owner (author / distributor) and type of certificate
     * (end entity / ca)
     * function returns list of matching fingerprints
     */
    FingerPrintList getKeyFingerprints(
        WidgetCertificateData::Owner owner,
        WidgetCertificateData::Type type) const;

    /*
     *  This method gets certificate data list for a widget from database.
     */
    WidgetCertificateDataList getCertificateDataList() const;

    /**
     * This method returns a list of widget features.
     *
     * @see WidgetFeature
     * @see FreeFeatureList()
     * @return list of widget features, type of list element is structure
     *  WidgetFeature
     * @exception WRT_CONF_ERR_EMDB_FAILURE - Fail to query DB table.
     * @exception WRT_CONF_ERR_EMDB_NO_RECORD - Can not find matching records in
     *  DB table.
     */
    DbWidgetFeatureSet getFeaturesList() const;

    /**
     * This method checks whether widget has specified feature.
     *
     * @return true if has, false if has not
     * @exception WRT_CONF_ERR_EMDB_FAILURE - Fail to query DB table.
     */
    bool hasFeature(const std::string& featureName) const;

    /**
     * This method gets if widget needs webkit plugins enabled
     *
     * @return true: widget needs webkit plugins enabled
     * @exception WRT_CONF_ERR_EMDB_FAILURE - Fail to query DB table.
     * @exception WRT_CONF_ERR_EMDB_NO_RECORD - Can not find matching records in
     *  DB table.
     */
    bool getWebkitPluginsRequired() const;

    /**
     * This method returns a list of all the installed widgets.
     *
     * @return list of installed widgets.
     * @exception WRT_CONF_ERR_EMDB_FAILURE - Fail to query DB table.
     * @exception WRT_CONF_ERR_EMDB_NO_RECORD - Can not find matching records in
     *  DB table.
     */
    static DbWidgetDAOReadOnlyList getWidgetList();

    /**
     * This method gets author's infomation of a widget which is parsed from
     *  configiration document.
     *
     * @see WidgetAuthorInfo
     * @param[out] pAuthorInfo
     * @return true if succeed, false if fail.
     */
    WidgetAuthorInfo getAuthorInfo() const;

    /**
     * This method gets author's name of a widget which is parsed from
     *  configiration document.
     *
     * @param[out] pAuthorInfo
     * @return author's name.
     * @exception WRT_CONF_ERR_EMDB_FAILURE - Fail to query DB table.
     * @exception WRT_CONF_ERR_EMDB_NO_RECORD - Can not find matching records in
     *  DB table.
     */
    DPL::OptionalString getAuthorName() const;

    /**
     * This method gets author's email of a widget which is parsed from
     *  configiration document.
     *
     * @param[out] pAuthorInfo
     * @return author's email.
     * @exception WRT_CONF_ERR_EMDB_FAILURE - Fail to query DB table.
     * @exception WRT_CONF_ERR_EMDB_NO_RECORD - Can not find matching records in
     *  DB table.
     */
    DPL::OptionalString getAuthorEmail() const;

    /**
     * This method gets author's email of a widget which is parsed from
     *  configiration document.
     *
     * @param[out] pAuthorInfo
     * @return author's email.
     * @exception WRT_CONF_ERR_EMDB_FAILURE - Fail to query DB table.
     * @exception WRT_CONF_ERR_EMDB_NO_RECORD - Can not find matching records in
     *  DB table.
     */
    DPL::OptionalString getAuthorHref() const;

    /**
     * This method returns minimum version of WAC that WRT has to be compliant
     *  to to run this widget
     *
     * @return Minimum version
     * @exception WRT_CONF_ERR_EMDB_FAILURE - Fail to query DB table.
     * @exception WRT_CONF_ERR_EMDB_NO_RECORD - Can not find matching records in
     *  DB table.
     */
    DPL::OptionalString getMinimumWacVersion() const;

    /**
     * This method get widget installed time
     *
     * @return time_t : return widget's install time
     */
    time_t getInstallTime() const;

    /**
     * This method gets widget base folder.
     *
     * @return widget base folder.
     * @exception WRT_CONF_ERR_EMDB_FAILURE - Fail to query DB table.
     * @exception WRT_CONF_ERR_EMDB_NO_RECORD - Can not find matching records in
     *  DB table.
     */
    std::string getBaseFolder() const;

    /* This method checks read only flag for given property
     */
    DPL::OptionalInt checkPropertyReadFlag(
        const PropertyDAOReadOnly::WidgetPropertyKey &key) const;

    /* This method gets widget property key list
     */
    PropertyDAOReadOnly::WidgetPropertyKeyList getPropertyKeyList() const;

    /* This method gets widget property list
     */
    PropertyDAOReadOnly::WidgetPreferenceList getPropertyList() const;

    /* This method get widget property value
     */
    PropertyDAOReadOnly::WidgetPropertyValue getPropertyValue(
        const PropertyDAOReadOnly::WidgetPropertyKey &key) const;

    LanguageTagList getLanguageTags() const;
    LanguageTagList getIconLanguageTags() const;

    WidgetLocalizedInfo getLocalizedInfo(const DPL::String& languageTag) const;

    bool getBackSupported() const;

    static bool isWidgetInstalled(DbWidgetHandle handle);
    static bool isWidgetInstalled(const TizenAppId & tzAppId);

    /* This method get path of the splash image.
     *
     * @return path of the widget's splash image
     */
    DPL::OptionalString getSplashImgSrc() const;

    ExternalLocationList getWidgetExternalLocations() const;

    /*
     * Default value is required to keep compatibility with
     * wrt-installer and wrt.
     */
    CertificateChainList getWidgetCertificate(
        CertificateSource source = SIGNATURE_DISTRIBUTOR) const;

    void getWidgetSettings(WidgetSettings& outWidgetSettings) const;

    /**
     * This method gets application control list that define AUL value
     *
     * @return See above comment
     * @exception WRT_CONF_ERR_EMDB_FAILURE - Fail to query DB table.
     */
    void getAppControlList(
        WidgetAppControlList& outAppControlList) const;

    /**
     * This method returns the type of the package.
     *
     * @return PackagingType
     * @exception WRT_CONF_ERR_EMDB_FAILURE - Fail to query DB table.
     * @exception WRT_CONF_ERR_EMDB_NO_RECORD - Can not find matching
     *                                          records in DB table.
     */
    PackagingType getPackagingType() const;

    void getEncryptedFileList(EncryptedFileList& filesList) const;

    /**
     * This method returns widget's background page filename.
     *
     * @return Name of file containing background page
     */
    DPL::OptionalString getBackgroundPage() const;

    /**
     * @brief generateTizenId generates new package id
     *
     * If widget do not supplies it's own tizen package id, this method can be
     * used,
     * although it should be removed in future.
     *
     * @return new tizen package id
     */
    static TizenPkgId generatePkgId();
    static TizenPkgId generateTizenId()
    {
        return generatePkgId();
    }

    /**
     * This method returns widget's installed path
     *
     * @return path of widget installed
     */
    DPL::OptionalString getWidgetInstalledPath() const;
    PrivilegeList getWidgetPrivilege() const;
    WidgetSecurityModelVersion getSecurityModelVersion() const;

};
} // namespace WrtDB

#endif // _WRT_SRC_CONFIGURATION_WIDGET_DAO_READ_ONLY_H_
