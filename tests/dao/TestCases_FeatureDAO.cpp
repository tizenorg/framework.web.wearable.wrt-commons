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
 * @file   TestCases_FeatureDAO.cpp
 * @author  Pawel Sikorski (p.sikorski@samsung.com)
 * @version 1.0
 * @brief   This file contains tests for feature dao class.
 */

#include <dpl/test/test_runner.h>
#include <dpl/foreach.h>
#include <dpl/wrt-dao-rw/feature_dao.h>
#include <dpl/wrt-dao-rw/plugin_dao.h>
#include <dpl/wrt-dao-ro/wrt_db_types.h>

using namespace WrtDB;

#define RUNNER_ASSERT_WHAT_EQUALS(in, test)                   \
    { std::string tmp(in);                                     \
      RUNNER_ASSERT_MSG(tmp == test, "Equals: [" + tmp + "]"); }

RUNNER_TEST_GROUP_INIT(DAO)

/*
 * Name: feature_dao_test_register_features
 * Description: Checks if plugin registeration performs features registration
 *              and if registration is made properly
 * Expected: registrartion should succeed
 */
RUNNER_TEST(feature_dao_test_register_features)
{
    PluginHandle plHandle;
    {
        std::string libraryPath("nfp1 lib_path");
        std::string libraryName("nfp1");

        PluginMetafileData pluginData;
        pluginData.m_libraryName = libraryName;

        plHandle = PluginDAO::registerPlugin(pluginData, libraryPath);
        RUNNER_ASSERT(PluginDAO::isPluginInstalled(libraryName) == true);

        FeatureHandleList old = FeatureDAOReadOnly::GetHandleList();
        PluginMetafileData::Feature f;
        f.m_name = std::string("new_f1");

        FeatureHandle handle = FeatureDAO::RegisterFeature(f, plHandle);
        RUNNER_ASSERT_MSG(handle != -1, "Already registered");
        RUNNER_ASSERT_MSG(old.size() < FeatureDAOReadOnly::GetHandleList().size(),
                "New feature should be saved");

        FeatureDAOReadOnly dao(handle);

        RUNNER_ASSERT_WHAT_EQUALS(dao.GetName(), "new_f1");
        plHandle = dao.GetPluginHandle();
    }

    {
        FeatureHandleList old = FeatureDAOReadOnly::GetHandleList();

        PluginMetafileData::Feature f;
        f.m_name = std::string("new_f2");

        FeatureHandle handle = FeatureDAO::RegisterFeature(f, plHandle);
        RUNNER_ASSERT_MSG(handle != -1, "Already registered");
        RUNNER_ASSERT_MSG(old.size() < FeatureDAOReadOnly::GetHandleList().size(),
                "New feature should be saved");

        FeatureDAOReadOnly dao(handle);

        RUNNER_ASSERT_MSG(plHandle == dao.GetPluginHandle(),
                "New plugin registered (should be old used)");

        FeatureDAO::UnregisterFeature(handle);
        PluginDAO::unregisterPlugin(plHandle);
    }
}

/*
 * Name: feature_dao_test_get_feature_handle
 * Description: Checks GetFeatureHandleListForPlugin
 * Expected: no errors found
 * directly
 */
RUNNER_TEST(feature_dao_test_get_feature_handle)
{
    PluginHandleSetPtr plugins =
        PluginDAO::getPluginHandleByStatus(PluginDAO::INSTALLATION_COMPLETED);

    RUNNER_ASSERT(plugins->size() == 5);

    FOREACH(it, *plugins) {
        FeatureHandleListPtr featureListPtr =
            FeatureDAOReadOnly::GetFeatureHandleListForPlugin(*it);
        int size = featureListPtr->size();

        switch(*it)
        {
            case 1:
                RUNNER_ASSERT(size == 1);
                break;
            case 2:
                RUNNER_ASSERT(size == 0);
                break;
            case 3:
                RUNNER_ASSERT(size == 0);
                break;
            case 4:
                RUNNER_ASSERT(size == 3);
                break;
            case 5:
                RUNNER_ASSERT(size == 0);
                break;
            default:
                RUNNER_ASSERT_MSG(false, "Wrong plugin handle");
                break;
        }
    }
}

/*
 * Name: feature_dao_test_get_device_capability
 * Description: Checks GetDeviceCapability
 * Expected: no errors found
 * directly
 */
RUNNER_TEST(feature_dao_test_get_device_capability)
{
    RUNNER_ASSERT(FeatureDAOReadOnly::GetDeviceCapability(L"feature1").size() == 1);
    RUNNER_ASSERT(FeatureDAOReadOnly::GetDeviceCapability(L"feature2").size() == 2);
    RUNNER_ASSERT(FeatureDAOReadOnly::GetDeviceCapability(L"feature3").size() == 1);
    RUNNER_ASSERT(FeatureDAOReadOnly::GetDeviceCapability(L"feature4").size() == 0);
}

/*
 * Name: feature_dao_test_unregister
 * Description: Checks UnregisterFeature
 * Expected: no errors found
 */
RUNNER_TEST(feature_dao_test_unregister)
{
    RUNNER_ASSERT(FeatureDAOReadOnly::isFeatureInstalled("feature5") == false);
    unsigned int size = FeatureDAOReadOnly::GetHandleList().size();

    PluginHandle plHandle;
    PluginMetafileData pluginData;
    pluginData.m_libraryName = "nfp1 lib_path";
    plHandle = PluginDAO::registerPlugin(pluginData, "lib_path");

    PluginMetafileData::Feature f0;
    f0.m_name = std::string("feature5");
    f0.m_deviceCapabilities.insert("1devicecap");
    f0.m_deviceCapabilities.insert("2devicecap");
    f0.m_deviceCapabilities.insert("3devicecap");

    FeatureHandle handle0 = FeatureDAO::RegisterFeature(f0, plHandle);
    RUNNER_ASSERT_MSG(handle0 != -1, "Already registered");

    RUNNER_ASSERT(FeatureDAOReadOnly::isFeatureInstalled(handle0) == true);
    RUNNER_ASSERT(FeatureDAOReadOnly::isFeatureInstalled("feature5") == true);
    RUNNER_ASSERT(FeatureDAOReadOnly::GetHandleList().size() == size + 1);

    FeatureDAO::UnregisterFeature(handle0);

    RUNNER_ASSERT(FeatureDAOReadOnly::isFeatureInstalled(handle0) == false);
    RUNNER_ASSERT(FeatureDAOReadOnly::isFeatureInstalled("feature5") == false);
    RUNNER_ASSERT(FeatureDAOReadOnly::GetHandleList().size() == size);

    PluginMetafileData::Feature f1;
    f1.m_name = std::string("feature5");

    FeatureHandle handle1 = FeatureDAO::RegisterFeature(f1, plHandle);
    RUNNER_ASSERT_MSG(handle1 != -1, "Already registered");

    RUNNER_ASSERT(FeatureDAOReadOnly::isFeatureInstalled(handle1) == true);
    RUNNER_ASSERT(FeatureDAOReadOnly::isFeatureInstalled("feature5") == true);
    RUNNER_ASSERT(FeatureDAOReadOnly::GetHandleList().size() == size + 1);

    FeatureDAO::UnregisterFeature(handle1);

    RUNNER_ASSERT(FeatureDAOReadOnly::isFeatureInstalled(handle1) == false);
    RUNNER_ASSERT(FeatureDAOReadOnly::isFeatureInstalled("feature5") == false);
    RUNNER_ASSERT(FeatureDAOReadOnly::GetHandleList().size() == size);

    PluginDAO::unregisterPlugin(plHandle);
}

/*
 * Name: feature_dao_test_get_device_capabilities
 * Description: Checks GetDeviceCapabilities
 * Expected: no errors found
 */
RUNNER_TEST(feature_dao_test_get_device_capabilities)
{
    FeatureDAOReadOnly f1("feature1");
    RUNNER_ASSERT(f1.GetDeviceCapabilities().size() == 1);

    FeatureDAOReadOnly f2("feature2");
    RUNNER_ASSERT(f2.GetDeviceCapabilities().size() == 2);

    FeatureDAOReadOnly f3("feature3");
    RUNNER_ASSERT(f3.GetDeviceCapabilities().size() == 1);

    FeatureDAOReadOnly f4("feature4");
    RUNNER_ASSERT(f4.GetDeviceCapabilities().size() == 0);
}

/*
 * Name: feature_dao_test_get_names
 * Description: Checks GetNames
 * Expected: no errors found
 * directly
 */
RUNNER_TEST(feature_dao_test_get_names)
{
    FeatureDAOReadOnly::NameMap names = FeatureDAOReadOnly::GetNames();

    RUNNER_ASSERT(names.size() == 4);

    int count = 0;
    FOREACH(n, names){
        if(n->second == "feature1" || n->second == "feature2" ||
                n->second == "feature3" || n->second == "feature4")
            count++;
        else RUNNER_ASSERT_MSG(false, "Wrong feature name");
    }

    RUNNER_ASSERT(count == 4);
}

/*
 * Name: feature_dao_test_dev_cap_with_feature_handle
 * Description: Checks GetDevCapWithFeatureHandle
 * Expected: no errors found
 */
RUNNER_TEST(feature_dao_test_dev_cap_with_feature_handle)
{
    FeatureDAOReadOnly::DeviceCapabilitiesMap map = FeatureDAOReadOnly::GetDevCapWithFeatureHandle();
    RUNNER_ASSERT(map.size() == 4);

    int count = 0;
    FOREACH(n, map){
        if(n->second == "devicecap1" || n->second == "devicecap2" ||
                n->second == "devicecap3" || n->second == "devicecap4")
            count++;
        else RUNNER_ASSERT_MSG(false, "Wrong device capability");
    }

    RUNNER_ASSERT(count == 4);
}


/*
 * Name: feature_dao_test_get_features
 * Description: Checks GetFeatures
 * Expected: no errors
 */
RUNNER_TEST(feature_dao_test_get_features)
{
//tests commented because internal error occured due to many GetFeatures() calls
//    std::list<std::string> fs0(4);
//    RUNNER_ASSERT(FeatureDAOReadOnly::GetFeatures(fs0).size() == 0);
//
//    std::list<std::string> fs1(4);
//    fs1.push_front("feature");
//    fs1.push_front("feature2");
//    RUNNER_ASSERT(FeatureDAOReadOnly::GetFeatures(fs1).size() == 2);
//    std::list<std::string> fs2(4);
//    fs2.push_front("feature");
//    fs2.push_front("feature2");
//    RUNNER_ASSERT(FeatureDAOReadOnly::GetFeatures(fs2).size() == 1);

    std::list<std::string> fs3(4);
    fs3.push_front("feature1");
    fs3.push_front("feature2");
    fs3.push_front("feature3");
    fs3.push_front("feature4");
    RUNNER_ASSERT(FeatureDAOReadOnly::GetFeatures(fs3).size() == 4);
}

/*
 * Name: feature_dao_test_get_feature_properties
 * Description: Checks properties of inserted features
 * Expected: properties of features should match values inserted to database
 * directly
 */
RUNNER_TEST(feature_dao_test_get_feature_properties)
{
    {
        FeatureDAOReadOnly dao("feature1");
        RUNNER_ASSERT_WHAT_EQUALS(dao.GetName(), "feature1");
        RUNNER_ASSERT_WHAT_EQUALS(dao.GetLibraryName(), "plugin1");
        RUNNER_ASSERT_WHAT_EQUALS(dao.GetLibraryPath(), "");
    }

    {
        FeatureDAOReadOnly dao("feature2");
        RUNNER_ASSERT_WHAT_EQUALS(dao.GetName(), "feature2");
        RUNNER_ASSERT_WHAT_EQUALS(dao.GetLibraryName(), "p4");
        RUNNER_ASSERT_WHAT_EQUALS(dao.GetLibraryPath(), "path_to_p4");
    }

    {
        FeatureDAOReadOnly dao("feature3");
        RUNNER_ASSERT_WHAT_EQUALS(dao.GetName(), "feature3");
        RUNNER_ASSERT_WHAT_EQUALS(dao.GetLibraryName(), "p4");
        RUNNER_ASSERT_WHAT_EQUALS(dao.GetLibraryPath(), "path_to_p4");
    }
}

/*
 * Name: feature_dao_test_feature_constructor_name
 * Description: -
 * Expected: -
 *
 * TODO: test
 */
RUNNER_TEST(feature_dao_test_feature_constructor_name)
{
    std::list<const char *> preinstalled;
    preinstalled.push_back("feature1");
    preinstalled.push_back("feature2");
    preinstalled.push_back("feature3");
    preinstalled.push_back("feature4");

    FOREACH(it, preinstalled)
    {
        FeatureDAOReadOnly dao(*it);
        RUNNER_ASSERT_WHAT_EQUALS(dao.GetName(), *it);
    }

    //TODO check exception that may occur (feature does not exist)
}

/*
 * Name: feature_dao_test_feature_handle_list
 * Description: Checks if list of installed features is returend correctly
 * Expected: list size should be at last equal number of preinserted features
 */
RUNNER_TEST(feature_dao_test_feature_handle_list)
{
    RUNNER_ASSERT(FeatureDAOReadOnly::GetHandleList().size() == 4);
}

/*
 * Name: feature_dao_test_is_feature_installed
 * Description: Checks if installed features are showed correctly.
 * Expected: correct installed features should be present
 */
RUNNER_TEST(feature_dao_test_is_feature_installed)
{
    //installed
    {
        std::list<const char *> preinstalled;
        preinstalled.push_back("feature1");
        preinstalled.push_back("feature2");
        preinstalled.push_back("feature3");
        preinstalled.push_back("feature4");

        FOREACH(it, preinstalled)
        RUNNER_ASSERT(FeatureDAOReadOnly::isFeatureInstalled(*it));
    }

    //not installed
    {
        RUNNER_ASSERT(FeatureDAOReadOnly::isFeatureInstalled(
                          "not_installed1") == false);
        RUNNER_ASSERT(FeatureDAOReadOnly::isFeatureInstalled(
                          "plugin1") == false);
        RUNNER_ASSERT(FeatureDAOReadOnly::isFeatureInstalled("") == false);
        RUNNER_ASSERT(FeatureDAOReadOnly::isFeatureInstalled("ff") == false);
    }
}

/*
 * Name: feature_dao_test_is_device_capab_installed
 * Description: Checks if FeatureDAOReadOnly::isDeviceCapabilityInstalled works
 * correctly.
 * Expected: correct capabilities should be present
 */
RUNNER_TEST(feature_dao_test_is_device_capab_installed)
{
    //installed
    std::list<const char *> preinstalled;
    preinstalled.push_back("devicecap1");
    preinstalled.push_back("devicecap2");
    preinstalled.push_back("devicecap3");
    preinstalled.push_back("devicecap4");

    FOREACH(it, preinstalled)
    RUNNER_ASSERT(FeatureDAOReadOnly::isDeviceCapabilityInstalled(*it));

    //not installed
    std::list<const char *> notinstalled;
    notinstalled.push_back("notinstalled1");
    notinstalled.push_back("plugin1");
    notinstalled.push_back("");
    notinstalled.push_back("ff");

    FOREACH(it, notinstalled)
    RUNNER_ASSERT(!FeatureDAOReadOnly::isDeviceCapabilityInstalled(*it));
}

#undef RUNNER_ASSERT_WHAT_EQUALS
