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
 * @file    test_suite01.cpp
 * @author  Krzysztof Jackiewicz (k.jackiewicz@samsung.com)
 * @version 1.0
 * @brief
 */

#include <dpl/log/log.h>
#include <dpl/test/test_runner.h>
#include <dpl/static_block.h>
#include <dpl/wrt-dao-ro/widget_dao_read_only.h>
#include <dpl/localization/w3c_file_localization.h>
#include <LanguageTagsProvider.h>

namespace {

STATIC_BLOCK
{
    WrtDB::LanguageTagList tags;
    tags.push_back(L"pl-pl");
    tags.push_back(L"en-en");
    tags.push_back(L"pl-en");
    LanguageTagsProviderSingleton::Instance().setLanguageTags(tags);
}

static const DPL::String widget1Path =
    L"/opt/share/widget/tests/localization/widget1/";
static const DPL::String widget2Path =
    L"/opt/share/widget/tests/localization/widget2/";


const std::string appId1("tizenid201");
const std::string appId2("tizenid202");

} // anonymous namespace

RUNNER_TEST(test01_getFilePathInWidgetPackageFromUrl){
    WrtDB::TizenAppId name = L"tizenid201"; //no difference if it is valid or invalid appId/pkgId, we fill database which has no intergrity constrainst
    WrtDB::WidgetDAOReadOnly dao(name);

    DPL::Optional<DPL::String> result = W3CFileLocalization::getFilePathInWidgetPackageFromUrl(
            name,
            DPL::String(L"widget://one"));

    RUNNER_ASSERT_MSG(!!result, "No result");
    RUNNER_ASSERT(
        *result ==
        L"/opt/share/widget/tests/localization/widget1/res/wgt/locales/pl-en/one");
}

RUNNER_TEST(test02_getFilePathInWidgetPackageFromUrl){
    WrtDB::TizenAppId name = L"tizenid202";
    WrtDB::WidgetDAOReadOnly dao(name);

    DPL::Optional<DPL::String> result = W3CFileLocalization::getFilePathInWidgetPackageFromUrl(
            name,
            DPL::String(L"widget://one"));

    RUNNER_ASSERT_MSG(!!result, "No result");
    RUNNER_ASSERT(
        *result ==
        L"/opt/share/widget/tests/localization/widget2/res/wgt/locales/pl-en/one");
}

RUNNER_TEST(test03_getFilePathInWidgetPackageFromUrl){
    WrtDB::TizenAppId name = L"tizenid202";
    WrtDB::WidgetDAOReadOnly dao(name);

    DPL::Optional<DPL::String> result = W3CFileLocalization::getFilePathInWidgetPackageFromUrl(
            name,
            DPL::String(L"widget://two.html"));

    RUNNER_ASSERT_MSG(!!result, "No result");
    RUNNER_ASSERT(
        *result ==
        L"/opt/share/widget/tests/localization/widget2/res/wgt/locales/en-en/two.html");
}

RUNNER_TEST(test04_getFilePathInWidgetPackageFromUrl)
{
    WrtDB::TizenAppId name = L"tizenid202";

    DPL::Optional<DPL::String> result = W3CFileLocalization::getFilePathInWidgetPackageFromUrl(
            name,
            DPL::String(L"widget://two.html?a=1#b"));

    RUNNER_ASSERT_MSG(!!result, "No result");
    RUNNER_ASSERT(
        *result ==
        L"/opt/share/widget/tests/localization/widget2/res/wgt/locales/en-en/two.html?a=1#b");
}

RUNNER_TEST(test05_getFilePathInWidgetPackageFromUrl)
{
    WrtDB::TizenAppId name = L"tizenid202";

    DPL::Optional<DPL::String> result = W3CFileLocalization::getFilePathInWidgetPackageFromUrl(
            name,
            DPL::String(L"widget://two.html#a?b"));

    RUNNER_ASSERT_MSG(!!result, "No result");
    RUNNER_ASSERT(
        *result ==
        L"/opt/share/widget/tests/localization/widget2/res/wgt/locales/en-en/two.html#a?b");
}

RUNNER_TEST(test06_getFilePathInWidgetPackageFromUrl)
{
    WrtDB::TizenAppId name = L"tizenid202";

    DPL::Optional<DPL::String> result = W3CFileLocalization::getFilePathInWidgetPackageFromUrl(
            name,
            DPL::String(L"file://two.html"));

    RUNNER_ASSERT_MSG(!!result, "No result");
    RUNNER_ASSERT(
        *result ==
        L"/opt/share/widget/tests/localization/widget2/res/wgt/locales/en-en/two.html");
}

RUNNER_TEST(test07_getFilePathInWidgetPackageFromUrl)
{
    WrtDB::TizenAppId name = L"tizenid202";

    DPL::Optional<DPL::String> result = W3CFileLocalization::getFilePathInWidgetPackageFromUrl(
            name,
            DPL::String(L"file:///opt/share/widget/tests/localization/widget2/res/wgt/two.html"));

    RUNNER_ASSERT_MSG(!!result, "No result");
    RUNNER_ASSERT(
        *result ==
        L"/opt/share/widget/tests/localization/widget2/res/wgt/locales/en-en/two.html");
}

RUNNER_TEST(test08_getFilePathInWidgetPackageFromUrl)
{
    WrtDB::TizenAppId name = L"tizenid202";

    DPL::Optional<DPL::String> result = W3CFileLocalization::getFilePathInWidgetPackageFromUrl(
            name,
            DPL::String(L"file:///opt/share/widget/tests/localization/widget2/res/wgt/locales/pl-en/two.html"));

    RUNNER_ASSERT_MSG(!!result, "No result");
    RUNNER_ASSERT(
        *result ==
        L"/opt/share/widget/tests/localization/widget2/res/wgt/locales/en-en/two.html");
}

RUNNER_TEST(test09_getFilePathInWidgetPackageFromUrl)
{
    WrtDB::TizenAppId name = L"tizenid202";

    DPL::Optional<DPL::String> result = W3CFileLocalization::getFilePathInWidgetPackageFromUrl(
            name,
            DPL::String(L"app://two.html"));

    RUNNER_ASSERT(result.IsNull());
}

RUNNER_TEST(test10_getFilePathInWidgetPackageFromUrl)
{
    WrtDB::TizenAppId name = L"tizenid202";

    DPL::Optional<DPL::String> result = W3CFileLocalization::getFilePathInWidgetPackageFromUrl(
            name,
            DPL::String(L"app://tizenid202/two.html"));

    RUNNER_ASSERT_MSG(!!result, "No result");
    RUNNER_ASSERT(
        *result ==
        L"/opt/share/widget/tests/localization/widget2/res/wgt/locales/en-en/two.html");
}

RUNNER_TEST(test11_getFilePathInWidgetPackageFromUrl)
{
    WrtDB::TizenAppId name = L"tizenid202";

    DPL::Optional<DPL::String> result = W3CFileLocalization::getFilePathInWidgetPackageFromUrl(
            name,
            DPL::String(L"dummy"));

    RUNNER_ASSERT(result.IsNull());
}

RUNNER_TEST(test12_getFilePathInWidgetPackageFromUrl)
{
    WrtDB::TizenAppId name = L"tizenid202";

    DPL::Optional<DPL::String> result = W3CFileLocalization::getFilePathInWidgetPackageFromUrl(
            name,
            DPL::String(L"app://tizenid202/notExisingFIle"));

    RUNNER_ASSERT(result.IsNull());
}

RUNNER_TEST(test13_getFilePathInWidgetPackageFromUrl2)
{
    std::string result = W3CFileLocalization::getFilePathInWidgetPackageFromUrl(appId1, "widget://one");

    RUNNER_ASSERT_MSG(!result.empty(), "No result");
    RUNNER_ASSERT(result == "/opt/share/widget/tests/localization/widget1/res/wgt/locales/pl-en/one");
}

RUNNER_TEST(test14_getFilePathInWidgetPackageFromUrl2)
{
    std::string result = W3CFileLocalization::getFilePathInWidgetPackageFromUrl(appId2, "widget://one");

    RUNNER_ASSERT_MSG(!result.empty(), "No result");
    RUNNER_ASSERT(result == "/opt/share/widget/tests/localization/widget2/res/wgt/locales/pl-en/one");
}

RUNNER_TEST(test15_getFilePathInWidgetPackageFromUrl2)
{
    std::string result = W3CFileLocalization::getFilePathInWidgetPackageFromUrl(appId2, "widget://two.html");

    RUNNER_ASSERT_MSG(!result.empty(), "No result");
    RUNNER_ASSERT(result == "/opt/share/widget/tests/localization/widget2/res/wgt/locales/en-en/two.html");
}

RUNNER_TEST(test16_getFilePathInWidgetPackageFromUrl2)
{
    std::string result = W3CFileLocalization::getFilePathInWidgetPackageFromUrl(appId2, "widget://two.html?a=1#b");

    RUNNER_ASSERT_MSG(!result.empty(), "No result");
    RUNNER_ASSERT(result == "/opt/share/widget/tests/localization/widget2/res/wgt/locales/en-en/two.html?a=1#b");
}

RUNNER_TEST(test17_getFilePathInWidgetPackageFromUrl2)
{
    std::string result = W3CFileLocalization::getFilePathInWidgetPackageFromUrl(appId2, "widget://two.html#a?b");

    RUNNER_ASSERT_MSG(!result.empty(), "No result");
    RUNNER_ASSERT(result == "/opt/share/widget/tests/localization/widget2/res/wgt/locales/en-en/two.html#a?b");
}

RUNNER_TEST(test18_getFilePathInWidgetPackageFromUrl2)
{
    std::string result = W3CFileLocalization::getFilePathInWidgetPackageFromUrl(appId2, "file://two.html");

    RUNNER_ASSERT_MSG(!result.empty(), "No result");
    RUNNER_ASSERT(result == "/opt/share/widget/tests/localization/widget2/res/wgt/locales/en-en/two.html");
}

RUNNER_TEST(test19_getFilePathInWidgetPackageFromUrl2)
{
    std::string result = W3CFileLocalization::getFilePathInWidgetPackageFromUrl(appId2,
            "file:///opt/share/widget/tests/localization/widget2/res/wgt/two.html");

    RUNNER_ASSERT_MSG(!result.empty(), "No result");
    RUNNER_ASSERT(result == "/opt/share/widget/tests/localization/widget2/res/wgt/locales/en-en/two.html");
}

RUNNER_TEST(test20_getFilePathInWidgetPackageFromUrl2)
{
    std::string result = W3CFileLocalization::getFilePathInWidgetPackageFromUrl(appId2,
            "file:///opt/share/widget/tests/localization/widget2/res/wgt/locales/pl-en/two.html");

    RUNNER_ASSERT_MSG(!result.empty(), "No result");
    RUNNER_ASSERT(result == "/opt/share/widget/tests/localization/widget2/res/wgt/locales/en-en/two.html");
}

RUNNER_TEST(test21_getFilePathInWidgetPackageFromUrl2)
{
    std::string result = W3CFileLocalization::getFilePathInWidgetPackageFromUrl(appId2, "app://two.html");

    RUNNER_ASSERT(result.empty());
}

RUNNER_TEST(test22_getFilePathInWidgetPackageFromUrl2)
{
    std::string result = W3CFileLocalization::getFilePathInWidgetPackageFromUrl(appId2, "app://tizenid202/two.html");

    RUNNER_ASSERT_MSG(!result.empty(), "No result");
    RUNNER_ASSERT(result == "/opt/share/widget/tests/localization/widget2/res/wgt/locales/en-en/two.html");
}

RUNNER_TEST(test23_getFilePathInWidgetPackageFromUrl2)
{
    std::string result = W3CFileLocalization::getFilePathInWidgetPackageFromUrl(appId2, "dummy");

    RUNNER_ASSERT(result.empty());
}

RUNNER_TEST(test24_getFilePathInWidgetPackageFromUrl2)
{
    std::string result = W3CFileLocalization::getFilePathInWidgetPackageFromUrl(appId2,
            "app://tizenid202/notExisingFIle");

    RUNNER_ASSERT(result.empty());
}

RUNNER_TEST(test25_getFilePathInWidgetPackage)
{
    WrtDB::TizenAppId name = L"tizenid201";
    WrtDB::WidgetDAOReadOnly dao(name);

    DPL::Optional<DPL::String> result = W3CFileLocalization::getFilePathInWidgetPackage(
            name,
            DPL::String(L"one"));

    RUNNER_ASSERT_MSG(!!result, "No result");
    RUNNER_ASSERT(*result == L"locales/pl-en/one");
}

RUNNER_TEST(test26_getFilePathInWidgetPackage)
{
    WrtDB::TizenAppId name = L"tizenid202";
    WrtDB::WidgetDAOReadOnly dao(name);

    DPL::Optional<DPL::String> result = W3CFileLocalization::getFilePathInWidgetPackage(
            name,
            DPL::String(L"two.html"));

    RUNNER_ASSERT_MSG(!!result, "No result");
    RUNNER_ASSERT(*result == L"locales/en-en/two.html");
}

RUNNER_TEST(test27_getFilePathInWidgetPackage)
{
    WrtDB::TizenAppId name = L"tizenid202";

    DPL::Optional<DPL::String> result = W3CFileLocalization::getFilePathInWidgetPackage(name, L"");
    RUNNER_ASSERT(result.IsNull());

    result = W3CFileLocalization::getFilePathInWidgetPackage(name, L"/");
    RUNNER_ASSERT(result.IsNull());

    result = W3CFileLocalization::getFilePathInWidgetPackage(name, L"//");
    RUNNER_ASSERT(result.IsNull());

    result = W3CFileLocalization::getFilePathInWidgetPackage(name, L"dummy");
    RUNNER_ASSERT(result.IsNull());

    result = W3CFileLocalization::getFilePathInWidgetPackage(name, L"/two.html/");
    RUNNER_ASSERT_MSG(!!result, "No result");
    RUNNER_ASSERT(*result == L"locales/en-en/two.html");
}

RUNNER_TEST(test28_getValidIconsList)
{
    WrtDB::TizenAppId name = L"not existing";

    bool exceptionCaught = false;

    try {
        W3CFileLocalization::WidgetIconList result = W3CFileLocalization::getValidIconsList(name);
    } catch (WrtDB::WidgetDAOReadOnly::Exception::WidgetNotExist&) {
        exceptionCaught = true;
    }
    RUNNER_ASSERT(exceptionCaught);
}

RUNNER_TEST(test29_getValidIconsList)
{
    WrtDB::TizenAppId name = L"tizenid202";

    W3CFileLocalization::WidgetIconList result = W3CFileLocalization::getValidIconsList(name);
    RUNNER_ASSERT(result.empty());
}


RUNNER_TEST(test30_getValidIconsList)
{
    WrtDB::TizenAppId name = L"tizenid201";

    W3CFileLocalization::WidgetIconList result = W3CFileLocalization::getValidIconsList(name);
    RUNNER_ASSERT(result.size() == 2);
    W3CFileLocalization::WidgetIconList::iterator iter = result.begin();
    RUNNER_ASSERT(iter->src == L"icon");
    RUNNER_ASSERT(iter->height == 250);
    RUNNER_ASSERT(iter->width == 251);
    iter++;
    RUNNER_ASSERT(iter->src == L"icon2");
    RUNNER_ASSERT(iter->height == 252);
    RUNNER_ASSERT(iter->width == 253);
}

RUNNER_TEST(test31_getStartFileInfo)
{
    WrtDB::TizenAppId name = L"not existing";

    bool exceptionCaught = false;

    try {
        OptionalWidgetStartFileInfo result = W3CFileLocalization::getStartFileInfo(name);
    } catch (WrtDB::WidgetDAOReadOnly::Exception::WidgetNotExist&) {
        exceptionCaught = true;
    }
    RUNNER_ASSERT(exceptionCaught);
}

RUNNER_TEST(test32_getStartFileInfo)
{
    WrtDB::TizenAppId name = L"tizenid202";

    OptionalWidgetStartFileInfo result = W3CFileLocalization::getStartFileInfo(name);
    RUNNER_ASSERT(result.IsNull());
}


RUNNER_TEST(test33_getStartFileInfo)
{
    WrtDB::TizenAppId name = L"tizenid201";

    OptionalWidgetStartFileInfo result = W3CFileLocalization::getStartFileInfo(name);
    RUNNER_ASSERT(!result.IsNull());
    RUNNER_ASSERT(result->file == L"start_file");
    RUNNER_ASSERT(result->localizedPath == L"locales/en-en/start_file");
}
