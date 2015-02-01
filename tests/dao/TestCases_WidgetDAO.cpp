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
 * @file   TestCases_WidgetDAO.cpp
 * @author  Pawel Sikorski (p.sikorski@samsung.com)
 * @version 1.0
 * @brief   This file contains tests for widget dao class.
 */

#include <list>
#include <cstdlib>
#include <cstdio>
#include <string>
#include <algorithm>
#include <dpl/test/test_runner.h>
#include <dpl/foreach.h>
#include <dpl/exception.h>
#include <dpl/wrt-dao-rw/widget_dao.h>
#include <dpl/wrt-dao-ro/wrt_db_types.h>
#include <dpl/string.h>
#include <wrt_plugin_export.h>
#include <LanguageTagsProvider.h>

using namespace WrtDB;

namespace {
class WacSecurityMock : public WrtDB::IWidgetSecurity
{
  public:
    WacSecurityMock() :
        mRecognized(false),
        mDistributorSigned(false)
    {}

    virtual const WidgetCertificateDataList& getCertificateList() const
    {
        return mList;
    }

    virtual bool isRecognized() const
    {
        return mRecognized;
    }
    virtual bool isDistributorSigned() const
    {
        return mDistributorSigned;
    }
    virtual void getCertificateChainList(CertificateChainList& /*lst*/) const {}
    virtual void getCertificateChainList(CertificateChainList& /*lst*/,
                                         CertificateSource /*source*/) const {}

    WrtDB::WidgetCertificateDataList& getCertificateListRef()
    {
        return mList;
    }

    void setRecognized(bool recognized)
    {
        mRecognized = recognized;
    }
    void setDistributorSigned(bool distributorSigned)
    {
        mDistributorSigned = distributorSigned;
    }

  private:
    WrtDB::WidgetCertificateDataList mList;
    // author signature verified
    bool mRecognized;
    // known distribuor
    bool mDistributorSigned;
    // distributor is wac
};

TizenAppId _registerWidget(const WidgetRegisterInfo& regInfo,
                           const IWidgetSecurity& sec,
                           int line)
{
    TizenAppId tizenAppId;
    Try {
        auto previous = WidgetDAO::getTizenAppidList();

        // register widget
        tizenAppId = WidgetDAO::registerWidgetGeneratePkgId(regInfo, sec);

        RUNNER_ASSERT_MSG(!tizenAppId.empty(),
                          "(called from line " << line << ")");

        auto current = WidgetDAO::getTizenAppidList();
        RUNNER_ASSERT_MSG(previous.size() + 1 == current.size(),
                          "(called from line " << line << ")");

        RUNNER_ASSERT_MSG(WidgetDAO::isWidgetInstalled(
                              tizenAppId),
                          "(called from line " << line << " tizenAppId: " <<
                          tizenAppId << ")");
    }
    Catch(WidgetDAO::Exception::AlreadyRegistered) {
        RUNNER_ASSERT_MSG(
            false,
            "Unexpected exception (called from line " << line << ")");
    }
    return tizenAppId;
}

#define REGISTER_WIDGET(regInfo, sec) _registerWidget((regInfo), \
                                                      (sec), \
                                                      __LINE__)

template<typename Exception, typename Function>
bool checkException(Function fun)
{
    Try
    {
        fun();
    }
    Catch(Exception){
        return true;
    }
    return false;
}

} // namespace

// Widgets used <2300,2500), 2000, 2001, 2002, 2003

#define RUNNER_ASSERT_WHAT_EQUALS(in, test)                   \
    { std::string tmp(in);                                     \
      RUNNER_ASSERT_MSG(tmp == test, "Equals: [" + tmp + "]"); }

#define RUNNER_ASSERT_WHAT_EQUALS_OPTIONAL(in, test)          \
    {                                                     \
        if (in.IsNull()) { RUNNER_ASSERT_MSG(false, "NULL"); } \
        else { RUNNER_ASSERT_WHAT_EQUALS(DPL::ToUTF8String(*in), test); } \
    }

#define RUNNER_ASSERT_WHAT_EQUALS_OPTIONALINT(in, test)                   \
    {                                                                 \
        if (in.IsNull()) { RUNNER_ASSERT_MSG(false, "NULL"); }             \
        else { RUNNER_ASSERT_MSG(*in == test, "Equals: [" + *in + "]"); } \
    }

#define RUNNER_ASSERT_EXCEPTION(exceptionType, function)             \
    {                                                                \
        RUNNER_ASSERT(checkException<exceptionType>([](){function})); \
    }

RUNNER_TEST_GROUP_INIT(DAO)

//2300
/*
 * Name: widget_dao_test_register_widget_empty_strings
 * Description: Tests registeration of new widget with empty values
 * Expected: widget should be registered in database
 */
RUNNER_TEST(widget_dao_test_register_widget_empty_strings)
{
    WidgetRegisterInfo regInfo;

    //info
    regInfo.configInfo.widget_id = DPL::FromUTF8String("");
    regInfo.configInfo.version = DPL::FromUTF8String("");
    regInfo.configInfo.width = 10;
    regInfo.configInfo.height = 10;
    regInfo.configInfo.authorName = DPL::FromUTF8String("");
    regInfo.configInfo.authorEmail = DPL::FromUTF8String("");
    regInfo.configInfo.authorHref = DPL::FromUTF8String("");
    regInfo.baseFolder = "";
    //TODO authenticated, etc...
    regInfo.configInfo.flashNeeded = false;
    regInfo.configInfo.minVersionRequired = DPL::FromUTF8String("1.0");
    regInfo.configInfo.backSupported = true;

    //loc info
    ConfigParserData::LocalizedData locData;
    locData.name = DPL::FromUTF8String("");
    locData.shortName = DPL::FromUTF8String("");
    locData.description = DPL::FromUTF8String("");
    locData.license = DPL::FromUTF8String("");
    locData.licenseFile = DPL::FromUTF8String("");
    locData.licenseHref = DPL::FromUTF8String("");
    regInfo.configInfo.localizedDataSet.insert(
        std::make_pair(DPL::FromUTF8String("en"), locData));

    //userAgentLoc

    //icons
    ConfigParserData::Icon icon(DPL::FromUTF8String(""));
    icon.width = 10;
    icon.height = 10;
    LocaleSet locs;
    locs.insert(DPL::FromUTF8String("en"));
    WidgetRegisterInfo::LocalizedIcon locIcon(icon, locs);
    regInfo.localizationData.icons.push_back(locIcon);

    //start file
    WidgetRegisterInfo::StartFileProperties prop;
    prop.encoding = DPL::FromUTF8String("");
    prop.type = DPL::FromUTF8String("");
    WidgetRegisterInfo::LocalizedStartFile file;
    file.path = DPL::FromUTF8String("");
    file.propertiesForLocales.insert(
        std::make_pair(DPL::FromUTF8String("en"), prop));
    regInfo.localizationData.startFiles.push_back(file);

    //widget pref
    ConfigParserData::Preference pref(DPL::FromUTF8String(""), false);
    pref.value = DPL::FromUTF8String("");
    regInfo.configInfo.preferencesList.insert(pref);

    //widget feature
    ConfigParserData::Feature feat(DPL::FromUTF8String(""));
    regInfo.configInfo.featuresList.insert(feat);

    //win modes
    regInfo.configInfo.windowModes.insert(DPL::FromUTF8String(""));

    //WARP info
    ConfigParserData::AccessInfo access(DPL::FromUTF8String(""), true);
    regInfo.configInfo.accessInfoSet.insert(access);

    //certificates
    WidgetCertificateData cert;
    cert.owner = WidgetCertificateData::AUTHOR;
    cert.type = WidgetCertificateData::ROOT;
    cert.chainId = 1;
    cert.strMD5Fingerprint = "";
    cert.strSHA1Fingerprint = "";
    cert.strCommonName = DPL::FromUTF8String("");

    WacSecurityMock security;
    security.getCertificateListRef().push_back(cert);

    REGISTER_WIDGET(regInfo, security);
}

/*
 * Name: widget_dao_test_register_widget_empty_strings
 * Description: Tests possiblity of registering twice same content (different
 * tizenId)
 * Expected: it should be possible
 */
RUNNER_TEST(widget_dao_test_twice_install_same_widget)
{
    WacSecurityMock sec;
    {
        WidgetRegisterInfo regInfo;
        REGISTER_WIDGET(regInfo, sec);
    }
    {
        WidgetRegisterInfo regInfo;
        REGISTER_WIDGET(regInfo, sec);
    }
}

/*
 * Name: widget_dao_test_register_widget_minimum_info
 * Description: Tests simplest registeration of new widget
 * Expected: widget should be registered in database
 */
RUNNER_TEST(widget_dao_test_register_widget_minimum_info)
{
    WacSecurityMock sec;
    const std::size_t NUMBER_OF_WIDGETS = 5;

    TizenAppId lastTizenAppId;

    for (std::size_t number = 0; number < NUMBER_OF_WIDGETS; ++number) {
        WidgetRegisterInfo regInfo;
        TizenAppId tizenAppId = REGISTER_WIDGET(regInfo, sec);

        lastTizenAppId = tizenAppId;

        WidgetDAO dao(tizenAppId);
        //TODO check nulls
    }
}

/*
 * Name: widget_dao_test_register_widget_info
 * Description: Tests registeration of many widgets
 * Expected: all widgets should be registered in database
 */
RUNNER_TEST(widget_dao_test_register_widget_info)
{
    WacSecurityMock sec;
    const std::size_t NUMBER_OF_WIDGETS = 5;

    for (std::size_t number = 0; number < NUMBER_OF_WIDGETS; ++number) {
        std::ostringstream str;
        str << "register_info_test_" << number;

        WidgetRegisterInfo regInfo;
        regInfo.configInfo.widget_id = DPL::FromUTF8String(str.str());
        regInfo.configInfo.version = DPL::FromUTF8String(str.str());
        regInfo.configInfo.width = 10;
        regInfo.configInfo.height = 10;
        regInfo.configInfo.authorName = DPL::FromUTF8String(str.str());
        regInfo.configInfo.authorEmail = DPL::FromUTF8String(str.str());
        regInfo.configInfo.authorHref = DPL::FromUTF8String(str.str());
        regInfo.baseFolder = str.str(); //base folder at the end has /
        regInfo.configInfo.flashNeeded = false;
        //TODO authenticated, etc...
        //in wrt-installer: TaskWidgetConfig::fillWidgetConfig:
        //regInfo.minVersion = regInfo.configInfo.minVersionRequired
        regInfo.minVersion = DPL::FromUTF8String("1.0");
        regInfo.configInfo.backSupported = true;

        TizenAppId tizenAppId = REGISTER_WIDGET(regInfo, sec);

        WidgetDAO dao(tizenAppId);
        RUNNER_ASSERT_WHAT_EQUALS_OPTIONAL(dao.getGUID(), str.str());
        RUNNER_ASSERT_WHAT_EQUALS_OPTIONAL(dao.getVersion(), str.str());
        RUNNER_ASSERT_WHAT_EQUALS_OPTIONAL(dao.getAuthorName(), str.str());
        RUNNER_ASSERT_WHAT_EQUALS_OPTIONAL(dao.getAuthorEmail(), str.str());
        RUNNER_ASSERT_WHAT_EQUALS_OPTIONAL(dao.getAuthorHref(), str.str());
        RUNNER_ASSERT_WHAT_EQUALS(dao.getBaseFolder(), str.str() + "/");
        RUNNER_ASSERT(dao.getWebkitPluginsRequired() == false);
        RUNNER_ASSERT_WHAT_EQUALS_OPTIONAL(dao.getMinimumWacVersion(), "1.0");
    }
}

/*
 * Name: widget_dao_test_register_widget_extended_info
 * Description: Tests registeration of widget_extended_info
 * Expected: registeration of extended inforamtion is checked
 * via existence of backgroudn page value
 */
RUNNER_TEST(widget_dao_test_register_widget_extended_info)
{
    WacSecurityMock sec;
    const std::size_t NUMBER_OF_WIDGETS = 5;

    for (std::size_t number = 0; number < NUMBER_OF_WIDGETS; ++number) {
        std::ostringstream str;
        str << "register_ext_info_test_" << number;

        WidgetRegisterInfo regInfo;

        regInfo.configInfo.backgroundPage = L"background.html";

        TizenAppId tizenAppId = REGISTER_WIDGET(regInfo, sec);

        WidgetDAO dao(tizenAppId);

        RUNNER_ASSERT_WHAT_EQUALS_OPTIONAL(dao.getBackgroundPage(),
                                           "background.html");
    }
}

/*
 * Name: widget_dao_test_register_widget_localized_info
 * Description: Tests registeration of localized widgets information
 * Expected: values received by dao should match those which were registered
 */
RUNNER_TEST(widget_dao_test_register_widget_localized_info)
{
    WacSecurityMock sec;
    const std::size_t NUMBER_OF_WIDGETS = 5;

    for (std::size_t number = 0; number < NUMBER_OF_WIDGETS; ++number) {
        WidgetRegisterInfo regInfo;
        std::ostringstream str_en;
        std::ostringstream str_pl;
        str_en << "register_loc_info_test_en_" << number;
        str_pl << "register_loc_info_test_pl_" << number;
        { //EN
            ConfigParserData::LocalizedData locDataEn;
            locDataEn.name = DPL::FromUTF8String(str_en.str());
            locDataEn.shortName = DPL::FromUTF8String(str_en.str());
            locDataEn.description = DPL::FromUTF8String(str_en.str());
            locDataEn.license = DPL::FromUTF8String(str_en.str());
            locDataEn.licenseFile = DPL::FromUTF8String(str_en.str());
            locDataEn.licenseHref = DPL::FromUTF8String(str_en.str());
            regInfo.configInfo.localizedDataSet.insert(
                std::make_pair(DPL::FromUTF8String("en"), locDataEn));
        }

        { //PL
            ConfigParserData::LocalizedData locDataPl;
            locDataPl.name = DPL::FromUTF8String(str_pl.str());
            locDataPl.shortName = DPL::FromUTF8String(str_pl.str());
            locDataPl.description = DPL::FromUTF8String(str_pl.str());
            locDataPl.license = DPL::FromUTF8String(str_pl.str());
            locDataPl.licenseFile = DPL::FromUTF8String(str_pl.str());
            locDataPl.licenseHref = DPL::FromUTF8String(str_pl.str());
            regInfo.configInfo.localizedDataSet.insert(
                std::make_pair(DPL::FromUTF8String("pl"), locDataPl));
        }

        TizenAppId tizenAppId = REGISTER_WIDGET(regInfo, sec);

        WidgetDAO dao(tizenAppId);
        RUNNER_ASSERT_MSG(dao.getLanguageTags().size() == 2,
                          "language tags list invalid");

        { //EN
            WidgetLocalizedInfo locInfo =
                dao.getLocalizedInfo(DPL::FromUTF8String("en"));

            RUNNER_ASSERT_WHAT_EQUALS_OPTIONAL(locInfo.name,
                                               str_en.str());
            RUNNER_ASSERT_WHAT_EQUALS_OPTIONAL(locInfo.shortName,
                                               str_en.str());
            RUNNER_ASSERT_WHAT_EQUALS_OPTIONAL(locInfo.description,
                                               str_en.str());
            RUNNER_ASSERT_WHAT_EQUALS_OPTIONAL(locInfo.license,
                                               str_en.str());
            RUNNER_ASSERT_WHAT_EQUALS_OPTIONAL(locInfo.licenseHref,
                                               str_en.str());
        }

        { //PL
            WidgetLocalizedInfo locInfo =
                dao.getLocalizedInfo(DPL::FromUTF8String("pl"));

            RUNNER_ASSERT_WHAT_EQUALS_OPTIONAL(locInfo.name,
                                               str_pl.str());
            RUNNER_ASSERT_WHAT_EQUALS_OPTIONAL(locInfo.shortName,
                                               str_pl.str());
            RUNNER_ASSERT_WHAT_EQUALS_OPTIONAL(locInfo.description,
                                               str_pl.str());
            RUNNER_ASSERT_WHAT_EQUALS_OPTIONAL(locInfo.license,
                                               str_pl.str());
            RUNNER_ASSERT_WHAT_EQUALS_OPTIONAL(locInfo.licenseHref,
                                               str_pl.str());
        }
    }
}

/*
 * Name: widget_dao_test_register_widget_icons
 * Description: Tests registeration of localized icons information
 * Expected: values received by dao should match those which were registered
 * for icon
 */
RUNNER_TEST(widget_dao_test_register_widget_icons)
{
    WacSecurityMock sec;
    WidgetRegisterInfo regInfo;

    ConfigParserData::Icon icon(L"icon1");
    icon.width = 10;
    icon.height = 10;
    LocaleSet locs;
    locs.insert(DPL::FromUTF8String("en"));
    WidgetRegisterInfo::LocalizedIcon locIcon(icon, locs);
    regInfo.localizationData.icons.push_back(locIcon);

    TizenAppId tizenAppId = REGISTER_WIDGET(regInfo, sec);

    WidgetDAO dao(tizenAppId);
    WidgetDAOReadOnly::WidgetIconList list = dao.getIconList();

    RUNNER_ASSERT(list.size() == regInfo.localizationData.icons.size());
    WidgetDAOReadOnly::WidgetIconList::const_iterator it1 = list.begin();
    WidgetRegisterInfo::LocalizedIconList::const_iterator it2 =
        regInfo.localizationData.icons.begin();
    for (; it1 != list.end() && it2 != regInfo.localizationData.icons.end();
         ++it1, ++it2)
    {
        RUNNER_ASSERT(it2->height == it1->iconHeight);
        RUNNER_ASSERT(it2->width == it1->iconWidth);
        RUNNER_ASSERT(it2->src == it1->iconSrc);
        RUNNER_ASSERT(it2->availableLocales == locs);
    }
}

/*
 * Name: widget_dao_test_register_widget_start_files
 * Description: Tests registeration of localized start files
 * Expected: no expectations as it should be removed
 */
RUNNER_TEST(widget_dao_test_register_widget_start_files)
{
    WacSecurityMock sec;

    WidgetRegisterInfo::LocalizedStartFileList files;
    WidgetRegisterInfo::StartFilePropertiesForLocalesMap map1;
    WidgetRegisterInfo::StartFileProperties prop1;
    prop1.encoding = DPL::FromUTF8String("enc1");
    prop1.type = DPL::FromUTF8String("type1");

    map1.insert(std::make_pair(DPL::FromUTF8String("en"), prop1));
    map1.insert(std::make_pair(DPL::FromUTF8String("pl"), prop1));

    WidgetRegisterInfo::LocalizedStartFile file1;
    WidgetRegisterInfo::LocalizedStartFile file2;
    file1.path = DPL::FromUTF8String("path1");
    file1.propertiesForLocales = map1;
    file2.path = DPL::FromUTF8String("path2");
    file2.propertiesForLocales = map1;

    files.push_back(file1);
    files.push_back(file1);

    WidgetRegisterInfo regInfo;
    regInfo.localizationData.startFiles = files;

    REGISTER_WIDGET(regInfo, sec);

    //TODO no getter in WidgetDAO (getter in LocalizedWidgetDAO,
    // but it will be removed
}

/*
 * Name: widget_dao_test_register_widget_features
 * Description: Tests registeration of features of widget
 * Expected: number of features should match (for given widget reginfo)
 */
RUNNER_TEST(widget_dao_test_register_widget_features)
{
    WacSecurityMock sec;
    ConfigParserData::FeaturesList features;
    features.insert(ConfigParserData::Feature(DPL::FromUTF8String("f1")));
    features.insert(ConfigParserData::Feature(DPL::FromUTF8String("f2")));
    features.insert(ConfigParserData::Feature(DPL::FromUTF8String("f3")));

    WidgetRegisterInfo regInfo;
    FOREACH(it, features)
    regInfo.configInfo.featuresList.insert(*it);

    TizenAppId tizenAppId = REGISTER_WIDGET(regInfo, sec);

    WidgetDAO dao(tizenAppId);
    WidgetFeatureSet out = dao.getFeaturesList();
    RUNNER_ASSERT_MSG(out.size() == features.size(),
                      "wrong number of features");
    //    FOREACH(it, out)
    //        RUNNER_ASSERT(features.find(*it) != features.end());
}

/*
 * Name: widget_dao_test_register_widget_win_modes
 * Description: Tests registeration of window modes
 * Expected: all modes should be returned from dao
 */
RUNNER_TEST(widget_dao_test_register_widget_win_modes)
{
    WacSecurityMock sec;
    std::set<DPL::String> modes;
    modes.insert(DPL::FromUTF8String("full"));
    modes.insert(DPL::FromUTF8String("window"));

    WidgetRegisterInfo regInfo;

    FOREACH(it, modes)
    regInfo.configInfo.windowModes.insert(*it);

    TizenAppId tizenAppId = REGISTER_WIDGET(regInfo, sec);

    WidgetDAO dao(tizenAppId);
    std::list<DPL::String> wins = dao.getWindowModes();
    RUNNER_ASSERT_MSG(modes.size() == wins.size(),
                      "wrong number of window modes");
    FOREACH(it, wins)
    RUNNER_ASSERT(modes.find(*it) != modes.end());
}

/*
 * Name: widget_dao_test_register_widget_warp_info
 * Description: Tests registeration of access info iris
 * Expected: all access info iris should be returned from dao
 */
RUNNER_TEST(widget_dao_test_register_widget_warp_info)
{
    WacSecurityMock sec;
    ConfigParserData::AccessInfoSet orig;
    orig.insert(ConfigParserData::AccessInfo(DPL::FromUTF8String("iri1"),
                                             true));
    orig.insert(ConfigParserData::AccessInfo(DPL::FromUTF8String("iri2"),
                                             false));
    orig.insert(ConfigParserData::AccessInfo(DPL::FromUTF8String("iri3"),
                                             true));

    WidgetRegisterInfo regInfo;
    FOREACH(it, orig)
    regInfo.configInfo.accessInfoSet.insert(*it);

    TizenAppId tizenAppId = REGISTER_WIDGET(regInfo, sec);

    WidgetDAO dao(tizenAppId);

    WidgetAccessInfoList out;
    dao.getWidgetAccessInfo(out);
    RUNNER_ASSERT_MSG(out.size() == orig.size(),
                      "wrong number of access info elem");
    FOREACH(it, out){
        ConfigParserData::AccessInfo tmp(it->strIRI, it->bSubDomains);
        RUNNER_ASSERT(orig.find(tmp) != orig.end());
    }
}

/*
 * Name: widget_dao_test_register_widget_certificates
 * Description: Tests registeration of widget certificates
 * Expected: all certificates should be returned from dao
 * and should contain inserted data
 */
RUNNER_TEST(widget_dao_test_register_widget_certificates)
{
    WacSecurityMock sec;
    WidgetRegisterInfo regInfo;

    WidgetCertificateData cert;
    cert.owner = WidgetCertificateData::AUTHOR;
    cert.type = WidgetCertificateData::ROOT;
    cert.chainId = 1;
    cert.strMD5Fingerprint = "md5";
    cert.strSHA1Fingerprint = "sha1";
    cert.strCommonName = DPL::FromUTF8String("common name");

    WidgetCertificateDataList& certListRef = sec.getCertificateListRef();
    certListRef.push_back(cert);

    // register widget
    TizenAppId tizenAppId = REGISTER_WIDGET(regInfo, sec);

    WidgetDAO dao(tizenAppId);

    // certificates
    WidgetCertificateDataList recList = dao.getCertificateDataList();
    RUNNER_ASSERT(recList.size() == certListRef.size());

    auto recListIt = recList.begin();
    auto certListIt = certListRef.begin();
    for (; recListIt != recList.end() && certListIt != certListRef.end();
         ++recListIt, ++certListIt)
    {
        RUNNER_ASSERT(recListIt->chainId == certListIt->chainId);
        RUNNER_ASSERT(recListIt->owner == certListIt->owner);
        RUNNER_ASSERT(recListIt->strCommonName == certListIt->strCommonName);
        RUNNER_ASSERT(recListIt->strMD5Fingerprint ==
                      certListIt->strMD5Fingerprint);
        RUNNER_ASSERT(recListIt->strSHA1Fingerprint ==
                      certListIt->strSHA1Fingerprint);
        RUNNER_ASSERT(recListIt->type == certListIt->type);
    }

    // fingerprints
    RUNNER_ASSERT(dao.getKeyFingerprints(WidgetCertificateData::DISTRIBUTOR,
                                         WidgetCertificateData::ENDENTITY).
                      empty());
    RUNNER_ASSERT(dao.getKeyFingerprints(WidgetCertificateData::AUTHOR,
                                         WidgetCertificateData::ENDENTITY).
                      empty());
    RUNNER_ASSERT(dao.getKeyFingerprints(WidgetCertificateData::DISTRIBUTOR,
                                         WidgetCertificateData::ROOT).empty());

    FingerPrintList fingerprints = dao.getKeyFingerprints(
            WidgetCertificateData::AUTHOR,
            WidgetCertificateData::ROOT);

    RUNNER_ASSERT(fingerprints.size() == certListRef.size() * 2);
    FOREACH(it, certListRef)
    {
        auto md5 = std::find(fingerprints.begin(),
                             fingerprints.end(),
                             it->strMD5Fingerprint);
        RUNNER_ASSERT(md5 != fingerprints.end());

        auto sha = std::find(fingerprints.begin(),
                             fingerprints.end(),
                             it->strSHA1Fingerprint);
        RUNNER_ASSERT(sha != fingerprints.end());
    }

    // common names
    RUNNER_ASSERT(dao.getKeyCommonNameList(WidgetCertificateData::DISTRIBUTOR,
                                           WidgetCertificateData::ENDENTITY).
                      empty());
    RUNNER_ASSERT(dao.getKeyCommonNameList(WidgetCertificateData::AUTHOR,
                                           WidgetCertificateData::ENDENTITY).
                      empty());
    RUNNER_ASSERT(dao.getKeyCommonNameList(WidgetCertificateData::DISTRIBUTOR,
                                           WidgetCertificateData::ROOT).empty());

    FingerPrintList commonNames = dao.getKeyCommonNameList(
            WidgetCertificateData::AUTHOR,
            WidgetCertificateData::ROOT);

    RUNNER_ASSERT(commonNames.size() == certListRef.size());
    FOREACH(it, certListRef)
    {
        auto cn = std::find(commonNames.begin(),
                            commonNames.end(),
                            DPL::ToUTF8String(it->strCommonName));
        RUNNER_ASSERT(cn != commonNames.end());
    }
}

/*
 * Name: widget_dao_test_register_widget_privileges
 * Description: Tests registration of widget privileges
 */
RUNNER_TEST(widget_dao_test_register_widget_privileges)
{

    WacSecurityMock sec;
    WidgetRegisterInfo regInfo;

    ConfigParserData::PrivilegeList& privilegeList =
            regInfo.configInfo.privilegeList;
    privilegeList.insert(DPL::FromUTF8String("name"));
    privilegeList.insert(DPL::FromUTF8String("name2"));

    TizenAppId tizenAppId = REGISTER_WIDGET(regInfo, sec);
    WidgetDAO dao(tizenAppId);

    WrtDB::PrivilegeList privListFromDB;
    privListFromDB = dao.getWidgetPrivilege();

    RUNNER_ASSERT(privilegeList.size() == privListFromDB.size());

    auto privListIt = privilegeList.begin();
    auto privDBIt = privListFromDB.begin();
    for(; privListIt != privilegeList.end() && privDBIt != privListFromDB.end();
            ++privListIt, ++privDBIt)
    {
        RUNNER_ASSERT(*privDBIt == privListIt->name);
    }
}

/*
 * Name: widget_dao_test_register_app_control
 * Description: Tests app control
 */
RUNNER_TEST(widget_dao_test_register_app_control)
{
    WacSecurityMock sec;
    WidgetRegisterInfo regInfo;

    ConfigParserData::AppControlInfo appControl(DPL::FromUTF8String("operation"));
    appControl.m_disposition
            = ConfigParserData::AppControlInfo::Disposition::WINDOW;
    appControl.m_mimeList.insert(DPL::FromUTF8String("mime"));
    appControl.m_src = DPL::FromUTF8String("src");
    appControl.m_uriList.insert(DPL::FromUTF8String("uri"));

    ConfigParserData::AppControlInfoList& appControlListRef
            = regInfo.configInfo.appControlList;
    appControlListRef.push_back(appControl);

    TizenAppId tizenAppId = REGISTER_WIDGET(regInfo, sec);

    WidgetDAO dao(tizenAppId);

    WrtDB::WidgetAppControlList appControlInfoListDB;
    dao.getAppControlList(appControlInfoListDB);
    RUNNER_ASSERT(appControlInfoListDB.size() == appControlListRef.size());
    auto appDBIt = appControlInfoListDB.begin();
    auto appRefIt = appControlListRef.begin();

    for (;appDBIt != appControlInfoListDB.end()
            && appRefIt != appControlListRef.end();
            ++appDBIt, ++appRefIt)
    {
        RUNNER_ASSERT((WidgetAppControl::Disposition)
                appRefIt->m_disposition == appDBIt->disposition);
        RUNNER_ASSERT(appRefIt->m_index == appDBIt->index);
        RUNNER_ASSERT(appRefIt->m_operation == appDBIt->operation);
        RUNNER_ASSERT(appRefIt->m_src == appDBIt->src);
        for(auto it = appRefIt->m_mimeList.begin();
                it != appRefIt->m_mimeList.end();
                ++it)
        {
            RUNNER_ASSERT((*it) == appDBIt->mime);
        }
        for(auto it = appRefIt->m_uriList.begin();
                it != appRefIt->m_uriList.end();
                ++it)
        {
            RUNNER_ASSERT((*it) == appDBIt->uri);
        }
    }
}

/*
 * Name: widget_dao_test_is_widget_installed
 * Description: Tests checking if widgets are installed
 * Expected: installed widgets should be stated as installed
 */
RUNNER_TEST(widget_dao_test_is_widget_installed)
{
    RUNNER_ASSERT(WidgetDAO::isWidgetInstalled(L"tizenid201"));

    WacSecurityMock sec;
    WidgetRegisterInfo regInfo;

    TizenAppId tizenAppId = REGISTER_WIDGET(regInfo, sec);

    RUNNER_ASSERT(WidgetDAO::isWidgetInstalled(tizenAppId));
}

/*
 * Name: widget_dao_test_unregister_widget
 * Description: Tests unregistering widgets
 * Expected: widget register informations should be successfully removed
 */
RUNNER_TEST(widget_dao_test_unregister_widget)
{
    WacSecurityMock sec;
    TizenAppIdList ids = WidgetDAO::getTizenAppidList();

    WidgetRegisterInfo regInfo;

    TizenAppId tizenAppId = REGISTER_WIDGET(regInfo, sec);

    WidgetDAO::unregisterWidget(tizenAppId);

    RUNNER_ASSERT_MSG(ids.size() == WidgetDAO::getTizenAppidList().size(),
                      "Widget unregister failed");
}

/*
 * Name: widget_dao_test_register_or_update_widget
 * Description: Tests reregistering widgets
 * Expected: widget should be successfully replaced
 */
RUNNER_TEST(widget_dao_test_register_or_update_widget)
{
    WacSecurityMock sec;

    WidgetRegisterInfo regInfo;
    regInfo.configInfo.version = L"1.0";
    regInfo.configInfo.authorName = L"AAA";

    WidgetRegisterInfo regInfo2;
    regInfo2.configInfo.version = L"1.1";
    regInfo2.configInfo.authorName = L"BBB";

    WrtDB::TizenAppId tizenAppId(L"abcdefghij");

    //first installation
    WidgetDAO::registerWidget(tizenAppId, regInfo, sec);
    RUNNER_ASSERT_MSG(WidgetDAO::isWidgetInstalled(
                          tizenAppId), "Widget is not registered");

    //success full update
    WidgetDAO::updateTizenAppId(tizenAppId, L"backup");
    WidgetDAO::registerWidget(tizenAppId, regInfo2, sec);
    WidgetDAO::unregisterWidget(L"backup");
    RUNNER_ASSERT_MSG(WidgetDAO::isWidgetInstalled(
                          tizenAppId), "Widget is not reregistered");
    WidgetDAO dao(tizenAppId);
    RUNNER_ASSERT_MSG(
        *dao.getVersion() == L"1.1", "Data widget was not updated");
    RUNNER_ASSERT_MSG(
        *dao.getAuthorName() == L"BBB", "Data widget was not updated");


    WidgetDAO::unregisterWidget(tizenAppId);
}

/*
 * Name: widget_dao_test_get_widget_tizenAppId_list
 * Description: Tests getTizenAppidList API for backendlib
 * Expected: For all position in database should be returned one item in list
 */
RUNNER_TEST(widget_dao_test_get_widget_tizenAppId_list)
{
    TizenAppIdList tizenAppIds = WidgetDAO::getTizenAppidList();
    RUNNER_ASSERT(tizenAppIds.size() >= 3);
}

/*
 * Name: widget_dao_test_get_widget_list
 * Description: Tests getTizenAppidList API for backendlib
 * Expected: For all position in database should be returned one item in list
 * Those item should contain valid tizenAppId
 */
RUNNER_TEST(widget_dao_test_get_widget_list)
{
    WidgetDAOReadOnlyList list = WidgetDAOReadOnly::getWidgetList();
    RUNNER_ASSERT(list.size() >= 3);
    RUNNER_ASSERT_MSG(!!list.front(), "widget dao exists");
    WidgetDAOReadOnlyPtr dao = list.front();
}

/*
 * Name: widget_dao_test_get_widget_attributes
 * Description: Tests returning basic widget attributes by dao
 * Expected: Attributes should match values inserted into database
 */
RUNNER_TEST(widget_dao_test_get_widget_attributes)
{
    {
        TizenAppId tizenAppId = L"tizenid201";
        WidgetDAO dao(tizenAppId);

        RUNNER_ASSERT_WHAT_EQUALS_OPTIONAL(dao.getGUID(), "w_id_2000");
        RUNNER_ASSERT_WHAT_EQUALS_OPTIONAL(dao.getVersion(), "1.0.0");
        RUNNER_ASSERT_WHAT_EQUALS_OPTIONAL(dao.getAuthorName(), "a_name_2000");
        RUNNER_ASSERT_WHAT_EQUALS_OPTIONAL(dao.getAuthorEmail(),
                                           "a_email_2000");
        RUNNER_ASSERT_WHAT_EQUALS_OPTIONAL(dao.getAuthorHref(), "a_href_2000");
        RUNNER_ASSERT_WHAT_EQUALS(dao.getBaseFolder(), "basef_2000/");
        RUNNER_ASSERT(dao.getWebkitPluginsRequired() == true);
        RUNNER_ASSERT_WHAT_EQUALS_OPTIONAL(dao.getMinimumWacVersion(), "1.0");
    }
}

/*
 * Name: widget_dao_test_localization
 * Description: Tests inserting and returning localization info
 * Expected: Values inserted into database should match values received from
 * database
 */
RUNNER_TEST(widget_dao_test_localization)
{
    WacSecurityMock sec;

    // icon
    WidgetRegisterInfo regInfo;
    ConfigParserData::Icon icon(L"icon1");
    icon.width = 10;
    icon.height = 10;
    LocaleSet locs;
    locs.insert(DPL::FromUTF8String("en"));
    locs.insert(DPL::FromUTF8String("pl"));
    WidgetRegisterInfo::LocalizedIcon locIcon(icon, locs);
    regInfo.localizationData.icons.push_back(locIcon);

    //start file
    WidgetRegisterInfo::StartFileProperties prop_en;
    prop_en.encoding = DPL::FromUTF8String("encoding_en");
    prop_en.type = DPL::FromUTF8String("type_en");

    WidgetRegisterInfo::StartFileProperties prop_pl;
    prop_pl.encoding = DPL::FromUTF8String("encoding_pl");
    prop_pl.type = DPL::FromUTF8String("type_pl");

    WidgetRegisterInfo::LocalizedStartFile file;
    file.path = DPL::FromUTF8String("path");
    file.propertiesForLocales.insert(
        std::make_pair(DPL::FromUTF8String("en"), prop_en));
    file.propertiesForLocales.insert(
        std::make_pair(DPL::FromUTF8String("pl"), prop_pl));
    regInfo.localizationData.startFiles.push_back(file);

    // register widget
    TizenAppId tizenAppId = REGISTER_WIDGET(regInfo, sec);

    WidgetDAO dao(tizenAppId);

    // check localized icons
    WidgetDAO::WidgetLocalizedIconList locList = dao.getLocalizedIconList();
    RUNNER_ASSERT(locList.size() == locs.size());

    // non-localized icon
    WidgetDAO::WidgetIconList list = dao.getIconList();
    int iconId = list.front().iconId;

    // compare every icon with the origin
    auto locsIt = locs.begin();
    auto iconIt = locList.begin();
    for (; locsIt != locs.end() && iconIt != locList.end(); ++locsIt,
         ++iconIt)
    {
        RUNNER_ASSERT(iconIt->appId == dao.getHandle());
        RUNNER_ASSERT(iconIt->iconId == iconId);
        RUNNER_ASSERT(iconIt->widgetLocale == *locsIt);
    }

    // localized start file list
    WidgetDAO::LocalizedStartFileList fList = dao.getLocalizedStartFileList();
    RUNNER_ASSERT(fList.size() == file.propertiesForLocales.size());

    int startFileId = dao.getStartFileList().front().startFileId;

    FOREACH(it, fList)
    {
        RUNNER_ASSERT(it->appId == dao.getHandle());
        auto propIt = file.propertiesForLocales.find(it->widgetLocale);
        RUNNER_ASSERT(propIt != file.propertiesForLocales.end());
        RUNNER_ASSERT(it->encoding == propIt->second.encoding);
        RUNNER_ASSERT(it->type == propIt->second.type);
        RUNNER_ASSERT(it->startFileId == startFileId);
    }
}

/*
 * Name: widget_dao_test_register_scp
 * Description: Tests inserting and returning scp policy information
 * Expected: Value inserted into database should match values received from
 * database
 */
RUNNER_TEST(widget_dao_test_register_scp)
{
    WacSecurityMock sec;
    WidgetRegisterInfo regInfo;
    DPL::OptionalString policy = DPL::FromUTF8String("Some awesome csp policy");
    regInfo.configInfo.cspPolicy = policy;
    {
        // register widget
        TizenAppId tizenAppId = REGISTER_WIDGET(regInfo, sec);
        WidgetDAO dao(tizenAppId);

        RUNNER_ASSERT_WHAT_EQUALS_OPTIONAL(
            dao.getCspPolicy(), DPL::ToUTF8String(*policy));
    }
}

/*
 * Name: widget_dao_test_register_csp_empty
 * Description: Tests inserting and returning empty csp policy
 * Expected: Value inserted into database should match values received from
 * database
 */
RUNNER_TEST(widget_dao_test_register_csp_empty)
{
    WacSecurityMock sec;
    WidgetRegisterInfo regInfo;
    {
        // register widget
        TizenAppId tizenAppId = REGISTER_WIDGET(regInfo, sec);
        WidgetDAO dao(tizenAppId);

        RUNNER_ASSERT_MSG(dao.getCspPolicy().IsNull(), "Policy is not null");
    }
}

/*
 * Name: widget_dao_test_widget_privileges_basics
 * Description: Tests basic operators of privileges
 * Expected: operators should behave properly
 */
RUNNER_TEST(widget_dao_test_widget_privileges_basics)
{
    ConfigParserData::Privilege f1(DPL::FromUTF8String("a"));
    ConfigParserData::Privilege f2(DPL::FromUTF8String("f2"));
    ConfigParserData::Privilege f3(DPL::FromUTF8String("f3"));
    ConfigParserData::Privilege f1prim(DPL::FromUTF8String("a"));

    RUNNER_ASSERT_MSG(f1 != f2, "features not equal");
    RUNNER_ASSERT_MSG(f2 != f3, "features not equal");
    RUNNER_ASSERT_MSG(f1 != f3, "features not equal");
    RUNNER_ASSERT_MSG(f1 == f1prim, "features equal");

    RUNNER_ASSERT(f1 >= f1prim);
    RUNNER_ASSERT(f1 <= f1prim);
    RUNNER_ASSERT(f1 < f2);
    RUNNER_ASSERT(f1 <= f2);
    RUNNER_ASSERT(f3 > f2);
    RUNNER_ASSERT(f3 >= f2);
}

/*
 * Name: widget_dao_test_widget_preferences_basics
 * Description: Tests basic operators of preferences
 * Expected: operators should behave properly
 */
RUNNER_TEST(widget_dao_test_widget_preferences_basics)
{
    ConfigParserData::Preference f1(DPL::FromUTF8String("a"));
    ConfigParserData::Preference f2(DPL::FromUTF8String("f2"));
    ConfigParserData::Preference f3(DPL::FromUTF8String("f3"));
    ConfigParserData::Preference f1prim(DPL::FromUTF8String("a"));

    RUNNER_ASSERT_MSG(f1 != f2, "preferences not equal");
    RUNNER_ASSERT_MSG(f2 != f3, "preferences not equal");
    RUNNER_ASSERT_MSG(f1 != f3, "preferences not equal");
    RUNNER_ASSERT_MSG(f1 == f1prim, "preferences equal");

    RUNNER_ASSERT(f1 >= f1prim);
    RUNNER_ASSERT(f1 <= f1prim);
    RUNNER_ASSERT(f1 < f2);
    RUNNER_ASSERT(f1 <= f2);
    RUNNER_ASSERT(f3 > f2);
    RUNNER_ASSERT(f3 >= f2);
}

/*
 * Name: widget_dao_test_widget_features_basics
 * Description: Tests basic operators of features
 * Expected: operators should behave properly
 */
RUNNER_TEST(widget_dao_test_widget_features_basics)
{
    ConfigParserData::Feature f1(DPL::FromUTF8String("a"));
    ConfigParserData::Feature f2(DPL::FromUTF8String("f2"));
    ConfigParserData::Feature f3(DPL::FromUTF8String("f3"));
    ConfigParserData::Feature f1prim(DPL::FromUTF8String("a"));

    RUNNER_ASSERT_MSG(f1 != f2, "features not equal");
    RUNNER_ASSERT_MSG(f2 != f3, "features not equal");
    RUNNER_ASSERT_MSG(f1 != f3, "features not equal");
    RUNNER_ASSERT_MSG(f1 == f1prim, "features equal");

    RUNNER_ASSERT(f1 >= f1prim);
    RUNNER_ASSERT(f1 <= f1prim);
    RUNNER_ASSERT(f1 < f2);
    RUNNER_ASSERT(f1 <= f2);
    RUNNER_ASSERT(f3 > f2);
    RUNNER_ASSERT(f3 >= f2);
}

/*
 * Name: widget_dao_test_widget_icons_basics
 * Description: Tests basic operators for icons
 * Expected: operators should behave properly
 */
RUNNER_TEST(widget_dao_test_widget_icons_basics)
{
    ConfigParserData::Icon f1(DPL::FromUTF8String("a"));
    ConfigParserData::Icon f2(DPL::FromUTF8String("f2"));
    ConfigParserData::Icon f3(DPL::FromUTF8String("f3"));
    ConfigParserData::Icon f1prim(DPL::FromUTF8String("a"));

    RUNNER_ASSERT_MSG(f1 != f2, "icons not equal");
    RUNNER_ASSERT_MSG(f2 != f3, "icons not equal");
    RUNNER_ASSERT_MSG(f1 != f3, "icons not equal");
    RUNNER_ASSERT_MSG(f1 == f1prim, "icons equal");

    RUNNER_ASSERT(f1 >= f1prim);
    RUNNER_ASSERT(f1 <= f1prim);
    RUNNER_ASSERT(f1 < f2);
    RUNNER_ASSERT(f1 <= f2);
    RUNNER_ASSERT(f3 > f2);
    RUNNER_ASSERT(f3 >= f2);
}

/*
 * Name: widget_dao_test_widget_settings_basics
 * Description: Tests basic operators for settings of widget
 * Expected: operators should behave properly
 */
RUNNER_TEST(widget_dao_test_widget_settings_basics)
{
    ConfigParserData::Setting f1(DPL::FromUTF8String("a"), DPL::FromUTF8String("1"));
    ConfigParserData::Setting f2(DPL::FromUTF8String("a"), DPL::FromUTF8String("2"));
    ConfigParserData::Setting f3(DPL::FromUTF8String("b"), DPL::FromUTF8String("2"));
    ConfigParserData::Setting f1prim(DPL::FromUTF8String("a"), DPL::FromUTF8String("1"));

    RUNNER_ASSERT_MSG(f1 != f2, "settings not equal");
    RUNNER_ASSERT_MSG(f2 != f3, "settings not equal");
    RUNNER_ASSERT_MSG(f1 != f3, "settings not equal");
    RUNNER_ASSERT_MSG(f1 == f1prim, "settings equal");

    RUNNER_ASSERT(f1 >= f1prim);
    RUNNER_ASSERT(f1 <= f1prim);
    RUNNER_ASSERT(f1 <= f2);
    RUNNER_ASSERT(f3 > f2);
    RUNNER_ASSERT(f2 < f3);
    RUNNER_ASSERT(f3 >= f2);
}

/*
 * Name: widget_dao_test_widget_access_basics
 * Description: Tests basic operators for access of widget
 * Expected: operators should behave properly
 */
RUNNER_TEST(widget_dao_test_widget_access_basics)
{
    ConfigParserData::AccessInfo a(DPL::FromUTF8String("a"), true);
    ConfigParserData::AccessInfo b(DPL::FromUTF8String("b"), true);
    ConfigParserData::AccessInfo a1(DPL::FromUTF8String("a"), true);
    ConfigParserData::AccessInfo a2(DPL::FromUTF8String("a"), false);

    RUNNER_ASSERT_MSG(a != b, "access info not equal");
    RUNNER_ASSERT_MSG(a == a1, "access info equal");
    RUNNER_ASSERT_MSG(b == b, "access info equal");
    RUNNER_ASSERT_MSG(a1 != b, "access info are not equal");
    RUNNER_ASSERT_MSG(a1 != a2, "access info are not equal");
    RUNNER_ASSERT(a2 < a1);
    RUNNER_ASSERT(a1 < b);
}

/*
 * Name: widget_dao_test_widget_metadata_basics
 * Description: Tests basic operators for metadata
 * Expected: operators should behave properly
 */
RUNNER_TEST(widget_dao_test_widget_metadata_basics)
{
    ConfigParserData::Metadata a(DPL::FromUTF8String("a"), DPL::FromUTF8String("1"));
    ConfigParserData::Metadata b(DPL::FromUTF8String("b"), DPL::FromUTF8String("1"));
    ConfigParserData::Metadata a1(DPL::FromUTF8String("a"), DPL::FromUTF8String("1"));
    ConfigParserData::Metadata a2(DPL::FromUTF8String("a"), DPL::FromUTF8String("2"));

    RUNNER_ASSERT_MSG(a != b, "metadata not equal");
    RUNNER_ASSERT_MSG(a == a1, "metadata equal");
    RUNNER_ASSERT_MSG(b == b, "metadata equal");
    RUNNER_ASSERT_MSG(a1 != b, "metadata not equal");
    RUNNER_ASSERT_MSG(a1 != a2, "metadata not equal");
}

/*
 * Name: widget_dao_test_widget_appcontrolinfo_basics
 * Description: Tests basic operators for app control info
 * Expected: operators should behave properly
 */
RUNNER_TEST(widget_dao_test_widget_appcontrolinfo_basics)
{
    ConfigParserData::AppControlInfo a(DPL::FromUTF8String("operation"));
    a.m_disposition = ConfigParserData::AppControlInfo::Disposition::WINDOW;
    a.m_mimeList.insert(DPL::FromUTF8String("mime"));
    a.m_src = DPL::FromUTF8String("src");
    a.m_uriList.insert(DPL::FromUTF8String("uri"));

    ConfigParserData::AppControlInfo a0(DPL::FromUTF8String("operation1"));
    a0.m_disposition = ConfigParserData::AppControlInfo::Disposition::WINDOW;
    a0.m_mimeList.insert(DPL::FromUTF8String("mime"));
    a0.m_src = DPL::FromUTF8String("src");
    a0.m_uriList.insert(DPL::FromUTF8String("uri"));

    ConfigParserData::AppControlInfo a1(DPL::FromUTF8String("operation"));
    a1.m_disposition = ConfigParserData::AppControlInfo::Disposition::UNDEFINE;
    a1.m_mimeList.insert(DPL::FromUTF8String("mime"));
    a1.m_src = DPL::FromUTF8String("src");
    a1.m_uriList.insert(DPL::FromUTF8String("uri"));

    ConfigParserData::AppControlInfo a2(DPL::FromUTF8String("operation"));
    a2.m_disposition = ConfigParserData::AppControlInfo::Disposition::WINDOW;
    a2.m_mimeList.insert(DPL::FromUTF8String("mime1"));
    a2.m_src = DPL::FromUTF8String("src");
    a2.m_uriList.insert(DPL::FromUTF8String("uri"));

    ConfigParserData::AppControlInfo a3(DPL::FromUTF8String("operation"));
    a3.m_disposition = ConfigParserData::AppControlInfo::Disposition::WINDOW;
    a3.m_mimeList.insert(DPL::FromUTF8String("mime"));
    a3.m_src = DPL::FromUTF8String("src1");
    a3.m_uriList.insert(DPL::FromUTF8String("uri"));

    ConfigParserData::AppControlInfo a4(DPL::FromUTF8String("operation"));
    a4.m_disposition = ConfigParserData::AppControlInfo::Disposition::WINDOW;
    a4.m_mimeList.insert(DPL::FromUTF8String("mime"));
    a4.m_src = DPL::FromUTF8String("src");
    a4.m_uriList.insert(DPL::FromUTF8String("uri1"));

    ConfigParserData::AppControlInfo a5(DPL::FromUTF8String("operation"));
    a5.m_disposition = ConfigParserData::AppControlInfo::Disposition::WINDOW;
    a5.m_mimeList.insert(DPL::FromUTF8String("mime"));
    a5.m_src = DPL::FromUTF8String("src");
    a5.m_uriList.insert(DPL::FromUTF8String("uri"));

    RUNNER_ASSERT_MSG(a != a0, "app control info not equal");
    RUNNER_ASSERT_MSG(a != a1, "app control info not equal");
    RUNNER_ASSERT_MSG(a != a2, "app control info not equal");
    RUNNER_ASSERT_MSG(a != a3, "app control info not equal");
    RUNNER_ASSERT_MSG(a != a4, "app control info not equal");
    RUNNER_ASSERT_MSG(a == a5, "app control info equal");
    RUNNER_ASSERT_MSG(a == a, "app control info equal");
}

/*
 * Name: widget_dao_test_widget_metadata_basics
 * Description: Tests basic operators for livebox info
 * Expected: operators should behave properly
 */
RUNNER_TEST(widget_dao_test_widget_livebox_basics)
{
    ConfigParserData::LiveboxInfo a;
    a.m_icon = DPL::FromUTF8String("icon");
    a.m_liveboxId = DPL::FromUTF8String("id");
    a.m_autoLaunch = DPL::FromUTF8String("auto");
    a.m_updatePeriod = DPL::FromUTF8String("period");
    a.m_primary = DPL::FromUTF8String("primary");

    ConfigParserData::LiveboxInfo a0;
    a0.m_icon = DPL::FromUTF8String("icon0");
    a0.m_liveboxId = DPL::FromUTF8String("id");
    a0.m_autoLaunch = DPL::FromUTF8String("auto");
    a0.m_updatePeriod = DPL::FromUTF8String("period");
    a0.m_primary = DPL::FromUTF8String("primary");

    ConfigParserData::LiveboxInfo a1;
    a1.m_icon = DPL::FromUTF8String("icon");
    a1.m_liveboxId = DPL::FromUTF8String("id0");
    a1.m_autoLaunch = DPL::FromUTF8String("auto");
    a1.m_updatePeriod = DPL::FromUTF8String("period");
    a1.m_primary = DPL::FromUTF8String("primary");

    ConfigParserData::LiveboxInfo a2;
    a2.m_icon = DPL::FromUTF8String("icon");
    a2.m_liveboxId = DPL::FromUTF8String("id");
    a2.m_autoLaunch = DPL::FromUTF8String("auto0");
    a2.m_updatePeriod = DPL::FromUTF8String("period");
    a2.m_primary = DPL::FromUTF8String("primary");

    ConfigParserData::LiveboxInfo a3;
    a3.m_icon = DPL::FromUTF8String("icon");
    a3.m_liveboxId = DPL::FromUTF8String("id");
    a3.m_autoLaunch = DPL::FromUTF8String("auto");
    a3.m_updatePeriod = DPL::FromUTF8String("period0");
    a3.m_primary = DPL::FromUTF8String("primary");

    ConfigParserData::LiveboxInfo a4;
    a4.m_icon = DPL::FromUTF8String("icon");
    a4.m_liveboxId = DPL::FromUTF8String("id");
    a4.m_autoLaunch = DPL::FromUTF8String("auto");
    a4.m_updatePeriod = DPL::FromUTF8String("period");
    a4.m_primary = DPL::FromUTF8String("primary0");

    ConfigParserData::LiveboxInfo a5;
    a5.m_icon = DPL::FromUTF8String("icon");
    a5.m_liveboxId = DPL::FromUTF8String("id");
    a5.m_autoLaunch = DPL::FromUTF8String("auto");
    a5.m_updatePeriod = DPL::FromUTF8String("period");
    a5.m_primary = DPL::FromUTF8String("primary");

    const DPL::String s1 = DPL::FromUTF8String("1");
    const DPL::String s2 =  DPL::FromUTF8String("2");

    ConfigParserData::LiveboxInfo a10;
    a10.m_icon = DPL::FromUTF8String("icon");
    a10.m_liveboxId = DPL::FromUTF8String("id");
    a10.m_autoLaunch = DPL::FromUTF8String("auto");
    a10.m_updatePeriod = DPL::FromUTF8String("period");
    a10.m_primary = DPL::FromUTF8String("primary");
    a10.m_label.push_back(std::pair<DPL::String,DPL::String>(s1, s1));

    ConfigParserData::LiveboxInfo a11;
    a11.m_icon = DPL::FromUTF8String("icon");
    a11.m_liveboxId = DPL::FromUTF8String("id");
    a11.m_autoLaunch = DPL::FromUTF8String("auto");
    a11.m_updatePeriod = DPL::FromUTF8String("period");
    a11.m_primary = DPL::FromUTF8String("primary");
    a11.m_label.push_back(std::pair<DPL::String,DPL::String>(s1, s2));
    a11.m_label.push_back(std::pair<DPL::String,DPL::String>(s1, s2));

    ConfigParserData::LiveboxInfo a12;
    a12.m_icon = DPL::FromUTF8String("icon");
    a12.m_liveboxId = DPL::FromUTF8String("id");
    a12.m_autoLaunch = DPL::FromUTF8String("auto");
    a12.m_updatePeriod = DPL::FromUTF8String("period");
    a12.m_primary = DPL::FromUTF8String("primary");
    a12.m_label.push_back(std::pair<DPL::String,DPL::String>(s1, s1));

    RUNNER_ASSERT_MSG(a != a0, "livebox info not equal");
    RUNNER_ASSERT_MSG(a != a1, "livebox info not equal");
    RUNNER_ASSERT_MSG(a != a2, "livebox info not equal");
    RUNNER_ASSERT_MSG(a != a3, "livebox info not equal");
    RUNNER_ASSERT_MSG(a != a4, "livebox info not equal");
    RUNNER_ASSERT_MSG(a == a5, "livebox info equal");
    RUNNER_ASSERT_MSG(a == a, "livebox info equal");
    RUNNER_ASSERT_MSG(a10 != a11, "livebox info not equal");
    RUNNER_ASSERT_MSG(a10 == a12, "livebox info equal");
}

/*
 * Name: widget_dao_test_get_widget_by_guid
 * Description: Tests creating WidgetDAO using GUID
 * Expected: Correct WidgetDAO should be created
 */
RUNNER_TEST(widget_dao_test_get_widget_by_guid)
{
    WidgetDAO dao(DPL::OptionalString(L"w_id_2000"));
    RUNNER_ASSERT(dao.getTizenAppId() == L"tizenid201");
}

/*
 * Name: widget_dao_test_set_tizen_app_id
 * Description: Tests changing TizenAppId
 * Expected: Change should be possible
 */
RUNNER_TEST(widget_dao_test_set_tizen_app_id)
{
    WacSecurityMock sec;
    WidgetRegisterInfo regInfo;
    TizenAppId originaltizenAppId = REGISTER_WIDGET(regInfo, sec);
    TizenAppId changedTizenAppId = L"changedTizenAppId";

    TizenAppIdList ids = WidgetDAO::getTizenAppidList();
    RUNNER_ASSERT(std::count(ids.begin(),ids.end(),originaltizenAppId) == 1);

    //Change tizenAppId
    WidgetDAO dao(originaltizenAppId);
    dao.setTizenAppId(changedTizenAppId);

    ids = WidgetDAO::getTizenAppidList();
    RUNNER_ASSERT(std::count(ids.begin(), ids.end(), originaltizenAppId) == 0);
    RUNNER_ASSERT(std::count(ids.begin(), ids.end(), changedTizenAppId) == 1);

    bool exceptionCaught = checkException < WidgetDAOReadOnly::Exception::WidgetNotExist > ([&]() {
            WidgetDAO dao2(originaltizenAppId);
            //Changing should fail because original tizenAppId doesn't exist any more.
            dao2.setTizenAppId(changedTizenAppId);
        });
    RUNNER_ASSERT(exceptionCaught);

    WidgetDAO::unregisterWidget(changedTizenAppId);
    ids = WidgetDAO::getTizenAppidList();
    RUNNER_ASSERT(std::count(ids.begin(),ids.end(),originaltizenAppId) == 0);
    RUNNER_ASSERT(std::count(ids.begin(),ids.end(),changedTizenAppId) == 0);

}

/*
 * Name: widget_dao_test_update_feature_reject_status
 * Description: Tests updating feature reject status
 * Expected: Reject status should be properly updated
 */
RUNNER_TEST(widget_dao_test_update_feature_reject_status)
{
    WidgetDAO dao(L"tizenid202");
    DbWidgetFeatureSet featureSet = dao.getFeaturesList();

    RUNNER_ASSERT(featureSet.size() == 1);
    DbWidgetFeature feature = *featureSet.begin();
    RUNNER_ASSERT(!feature.rejected);

    feature.rejected = true;
    dao.updateFeatureRejectStatus(feature);

    featureSet = dao.getFeaturesList();
    RUNNER_ASSERT(featureSet.size() == 1);
    DbWidgetFeature feature2 = *featureSet.begin();
    RUNNER_ASSERT(feature.rejected);

    feature2.rejected = false;
    dao.updateFeatureRejectStatus(feature2);
}

/*
 * Name: widget_dao_test_register_widget_allow_navigation
 * Description: Tests AllowNavigationInfo registration
 * Expected: AllowNavigationInfo registration should be possible
 */
RUNNER_TEST(widget_dao_test_register_widget_allow_navigation)
{
    WacSecurityMock sec;
    WidgetRegisterInfo regInfo;
    ConfigParserData::AllowNavigationInfo navigationInfo1(L"scheme1", L"host1");
    ConfigParserData::AllowNavigationInfo navigationInfo2(L"scheme2", L"host2");
    regInfo.configInfo.allowNavigationInfoList.push_back(navigationInfo1);
    regInfo.configInfo.allowNavigationInfoList.push_back(navigationInfo2);

    TizenAppId tizenAppId = REGISTER_WIDGET(regInfo, sec);

    WidgetDAOReadOnly dao(tizenAppId);
    WidgetAllowNavigationInfoList navigationList;
    dao.getWidgetAllowNavigationInfo(navigationList);

    RUNNER_ASSERT(navigationList.size() == 2);
    RUNNER_ASSERT(
            std::count_if(navigationList.begin(), navigationList.end(), [] (const WidgetAllowNavigationInfo& navInfo)
                    { return navInfo.host == L"host1" && navInfo.scheme == L"scheme1"; }) == 1);
    RUNNER_ASSERT(
            std::count_if(navigationList.begin(), navigationList.end(), [] (const WidgetAllowNavigationInfo& navInfo)
                    { return navInfo.host == L"host2" && navInfo.scheme == L"scheme2"; }) == 1);

}

/*
 * Name: widget_dao_test_register_external_locations
 * Description: Tests ExternalLocation registration
 * Expected: ExternalLocation registration should be possible
 */
RUNNER_TEST(widget_dao_test_register_external_locations)
{
    WacSecurityMock sec;
    WidgetRegisterInfo regInfo;
    regInfo.externalLocations.push_back("location1");
    regInfo.externalLocations.push_back("location2");

    TizenAppId tizenAppId = REGISTER_WIDGET(regInfo, sec);

    WidgetDAOReadOnly dao(tizenAppId);
    ExternalLocationList locationList = dao.getWidgetExternalLocations();

    RUNNER_ASSERT(locationList.size() == 2);
    RUNNER_ASSERT(std::count(locationList.begin(), locationList.end(), "location1") == 1);
    RUNNER_ASSERT(std::count(locationList.begin(), locationList.end(), "location2") == 1);
}

 /*
  * Name: widget_dao_test_get_tz_app_id
  * Description: Tests getTizenAppId functions
  * Expected: TizenAppId is correctly fetched for correct widgets, exception for not existing.
  */
RUNNER_TEST(widget_dao_test_get_tz_app_id)
{
    //No such widget
    RUNNER_ASSERT_EXCEPTION(WidgetDAOReadOnly::Exception::WidgetNotExist, WidgetDAOReadOnly::getTizenAppId(1234); );

    TizenAppId appId = WidgetDAOReadOnly::getTizenAppId(2001);
    RUNNER_ASSERT(L"tizenid202" == appId);

    RUNNER_ASSERT_EXCEPTION(WidgetDAOReadOnly::Exception::WidgetNotExist, WidgetDAOReadOnly::getTizenAppId(L"no_such_widget"); );

    appId = WidgetDAOReadOnly::getTizenAppId(L"pkgid202");
    RUNNER_ASSERT(L"tizenid202" == appId);
}

/*
 * Name: widget_dao_test_get_tz_pkg_id
 * Description: Tests getTizenPkgId functions
 * Expected: TizenPkgId is correctly fetched for correct widgets, exception for not existing.
 */
RUNNER_TEST(widget_dao_test_get_tz_pkg_id)
{
    //No such widget
    RUNNER_ASSERT_EXCEPTION(WidgetDAOReadOnly::Exception::WidgetNotExist, WidgetDAOReadOnly::getTizenPkgId(1234); );


    TizenPkgId pkgId = WidgetDAOReadOnly::getTizenPkgId(2001);
    RUNNER_ASSERT(L"pkgid202" == pkgId);

    RUNNER_ASSERT_EXCEPTION(WidgetDAOReadOnly::Exception::WidgetNotExist, WidgetDAOReadOnly::getTizenPkgId(L"no_such_widget"););

    pkgId = WidgetDAOReadOnly::getTizenPkgId(L"tizenid202");
    RUNNER_ASSERT(L"pkgid202" == pkgId);
}

/*
 * Name: widget_dao_test_get_property_key
 * Description: Tests getPropertyKeyList function
 * Expected: Property keys are fetched correctly.
 */
RUNNER_TEST(widget_dao_test_get_property_key)
{
    WidgetDAOReadOnly dao(L"tizenid201");
    PropertyDAOReadOnly::WidgetPropertyKeyList propertyKeys = dao.getPropertyKeyList();

    RUNNER_ASSERT(propertyKeys.size() == 2);
    RUNNER_ASSERT(std::count(propertyKeys.begin(), propertyKeys.end(), L"key1_for_2000") == 1);
    RUNNER_ASSERT(std::count(propertyKeys.begin(), propertyKeys.end(), L"key2_for_2000") == 1);
}

/*
 * Name: widget_dao_test_get_property_value
 * Description: Tests getPropertyValue function
 * Expected: Property key values are fetched correctly.
 */
RUNNER_TEST(widget_dao_test_get_property_value)
{
    WidgetDAOReadOnly dao(L"tizenid201");

    RUNNER_ASSERT(dao.getPropertyValue(L"key1_for_2000") == L"value_for_key1_2000");
    RUNNER_ASSERT(dao.getPropertyValue(L"key2_for_2000") == L"value_for_key2_2000");
    RUNNER_ASSERT(dao.getPropertyValue(L"not_existing").IsNull());
    RUNNER_ASSERT(dao.getPropertyValue(L"").IsNull());
}

/*
 * Name: widget_dao_test_get_property_read_flag
 * Description: Tests checkPropertyReadFlag function
 * Expected: Property read flags values are fetched correctly.
 */
RUNNER_TEST(widget_dao_test_get_property_read_flag)
{
    WidgetDAOReadOnly dao(L"tizenid201");

    RUNNER_ASSERT(dao.checkPropertyReadFlag(L"key1_for_2000") == 0);
    RUNNER_ASSERT(dao.checkPropertyReadFlag(L"key2_for_2000") == 0);
    RUNNER_ASSERT(dao.checkPropertyReadFlag(L"not_existing").IsNull());
    RUNNER_ASSERT(dao.checkPropertyReadFlag(L"").IsNull());
}

/*
 * Name: widget_dao_test_get_pkg_id_list
 * Description: Tests getTizenPkgIdList function
 * Expected: All pkg ids are returned.
 */
RUNNER_TEST(widget_dao_test_get_pkg_id_list)
{
    TizenPkgIdList pkgIds = WidgetDAOReadOnly::getTizenPkgIdList();

    RUNNER_ASSERT(pkgIds.size() == 4);
    RUNNER_ASSERT(std::count(pkgIds.begin(), pkgIds.end(), L"pkgid201") == 1);
    RUNNER_ASSERT(std::count(pkgIds.begin(), pkgIds.end(), L"pkgid202") == 1);
    RUNNER_ASSERT(std::count(pkgIds.begin(), pkgIds.end(), L"") == 2);
}

/*
 * Name: widget_dao_test_get_back_supported
 * Description: Tests getBackSupported function
 * Expected: Back supported information is fetched properly.
 */
RUNNER_TEST(widget_dao_test_get_back_supported)
{
    WidgetDAOReadOnly dao1(L"tizenid201");
    RUNNER_ASSERT(dao1.getBackSupported());

    WidgetDAOReadOnly dao2(L"tizenid202");
    RUNNER_ASSERT(!dao2.getBackSupported());
}

/*
 * Name: widget_dao_test_get_csp_policy_report
 * Description: Tests getCspPolicyReportOnly function
 * Expected: CSP policy information is fetched properly.
 */
RUNNER_TEST(widget_dao_test_get_csp_policy_report)
{
    WidgetDAOReadOnly dao1(L"tizenid201");
    RUNNER_ASSERT(DPL::OptionalString(L"policy_report201") == dao1.getCspPolicyReportOnly());

    WidgetDAOReadOnly dao2(L"tizenid202");
    RUNNER_ASSERT(DPL::OptionalString(L"policy_report202") == dao2.getCspPolicyReportOnly());
}

/*
 * Name: widget_dao_test_get_install_time
 * Description: Tests getInstallTime function
 * Expected: Installation time is fetched properly.
 */
RUNNER_TEST(widget_dao_test_get_install_time)
{
    WidgetDAOReadOnly dao1(L"tizenid201");
    RUNNER_ASSERT(1256 == dao1.getInstallTime());

    WidgetDAOReadOnly dao2(L"tizenid202");
    RUNNER_ASSERT(5687 == dao2.getInstallTime());
}

/*
 * Name: widget_dao_test_get_icon_language_tags
 * Description: Tests getIconLanguageTags function
 * Expected: Information about icon localization is fetched properly.
 */
RUNNER_TEST(widget_dao_test_get_icon_language_tags)
{
    WidgetDAOReadOnly dao1(L"tizenid201");
    LanguageTags languageTags = dao1.getIconLanguageTags();

    RUNNER_ASSERT(2 == languageTags.size());
    RUNNER_ASSERT(std::count(languageTags.begin(), languageTags.end(), L"en") == 1);
    RUNNER_ASSERT(std::count(languageTags.begin(), languageTags.end(), L"pl") == 1);
}

#undef RUNNER_ASSERT_WHAT_EQUALS
