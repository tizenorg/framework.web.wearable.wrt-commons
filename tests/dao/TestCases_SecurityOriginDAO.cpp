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
/**
 * @file   TestCases_SecurityOriginDAO.cpp
 * @author  Slawomir Pajak (s.pajak@partner.samsung.com)
 * @version 1.0
 * @brief   This file contains tests for security origin dao class.
 */

#include <dpl/test/test_runner.h>
#include <wrt-commons/security-origin-dao/security_origin_dao.h>


RUNNER_TEST_GROUP_INIT(DAO)

using namespace SecurityOriginDB;

namespace {

class SecurityOriginDAOWrapper {
public:
    static SecurityOriginDAO* getSecurityOriginDAO()
    {
        static SecurityOriginDAOPtr securityOriginDAO(
                new SecurityOriginDB::SecurityOriginDAO(DPL::FromASCIIString("testWidget123")));
        return securityOriginDAO.get();
    }
};
}

/**
 * Name: security_origin_dao_set_remove
 * Description: Test saves SecurityOriginData object, reads it, and removes it at the end.
 * Expected: All operations should succeed.
 */
RUNNER_TEST(security_origin_dao_set_remove)
{
    SecurityOriginDAO* securityOriginDAO = SecurityOriginDAOWrapper::getSecurityOriginDAO();

    SecurityOriginDataList dataList = securityOriginDAO->getSecurityOriginDataList();
    RUNNER_ASSERT_MSG(dataList.empty(), "SecurityOriginDataList should be empty");

    Origin origin(L"scheme", L"host", 123);
    SecurityOriginData securityData(WrtDB::FEATURE_WEB_NOTIFICATION, origin);
    securityOriginDAO->setSecurityOriginData(securityData, RESULT_ALLOW_ONCE, false);

    Origin origin2(L"scheme2", L"host2", 1234);
    SecurityOriginData securityData2(WrtDB::FEATURE_GEOLOCATION, origin2);
    securityOriginDAO->setSecurityOriginData(securityData2, RESULT_DENY_ONCE, true);

    dataList = securityOriginDAO->getSecurityOriginDataList();

    RUNNER_ASSERT_MSG(dataList.size() == 2, "CertificateDataList should not be empty");

    securityOriginDAO->removeSecurityOriginData(securityData);
    securityOriginDAO->removeSecurityOriginData(securityData2);

    dataList = securityOriginDAO->getSecurityOriginDataList();
    RUNNER_ASSERT_MSG(dataList.empty(), "SecurityOriginDataList should be empty");
}

/**
 * Name: security_origin_dao_update
 * Description: Test checks update operation for SecurityOriginData.
 * Expected: All operations should succeed.
 */
RUNNER_TEST(security_origin_dao_update)
{
    SecurityOriginDAO* securityOriginDAO = SecurityOriginDAOWrapper::getSecurityOriginDAO();

    SecurityOriginDataList dataList = securityOriginDAO->getSecurityOriginDataList();
    RUNNER_ASSERT_MSG(dataList.empty(), "SecurityOriginDataList should be empty");

    Origin origin(L"scheme", L"host", 123);
    SecurityOriginData securityData(WrtDB::FEATURE_WEB_NOTIFICATION, origin);
    securityOriginDAO->setSecurityOriginData(securityData, RESULT_ALLOW_ONCE, true);

    dataList = securityOriginDAO->getSecurityOriginDataList();
    RUNNER_ASSERT_MSG(dataList.size() == 1, "CertificateDataList should not be empty");
    RUNNER_ASSERT((*dataList.begin())->feature == WrtDB::FEATURE_WEB_NOTIFICATION);
    RUNNER_ASSERT((*dataList.begin())->origin.host == L"host");
    RUNNER_ASSERT((*dataList.begin())->origin.scheme == L"scheme");
    RUNNER_ASSERT((*dataList.begin())->origin.port == 123);
    RUNNER_ASSERT(securityOriginDAO->getResult(**dataList.begin()) == RESULT_ALLOW_ONCE);
    RUNNER_ASSERT(securityOriginDAO->isReadOnly(**dataList.begin()));

    securityOriginDAO->setSecurityOriginData(securityData, RESULT_DENY_ONCE, false);

    dataList = securityOriginDAO->getSecurityOriginDataList();
    RUNNER_ASSERT_MSG(dataList.size() == 1, "CertificateDataList should not be empty");
    RUNNER_ASSERT((*dataList.begin())->feature == WrtDB::FEATURE_WEB_NOTIFICATION);
    RUNNER_ASSERT((*dataList.begin())->origin.host == L"host");
    RUNNER_ASSERT((*dataList.begin())->origin.scheme == L"scheme");
    RUNNER_ASSERT((*dataList.begin())->origin.port == 123);
    RUNNER_ASSERT(securityOriginDAO->getResult(**dataList.begin()) == RESULT_DENY_ONCE);
    RUNNER_ASSERT(!securityOriginDAO->isReadOnly(**dataList.begin()));

    securityOriginDAO->removeSecurityOriginData(securityData);

    dataList = securityOriginDAO->getSecurityOriginDataList();
    RUNNER_ASSERT_MSG(dataList.empty(), "SecurityOriginDataList should be empty");
}


/**
 * Name: security_origin_dao_remove_by_result
 * Description: Test remove by result operation for SecurityOriginData.
 * Expected: All operations should succeed.
 */
RUNNER_TEST(security_origin_dao_remove_by_result)
{
    SecurityOriginDAO* securityOriginDAO = SecurityOriginDAOWrapper::getSecurityOriginDAO();

    SecurityOriginDataList dataList = securityOriginDAO->getSecurityOriginDataList();
    RUNNER_ASSERT_MSG(dataList.empty(), "SecurityOriginDataList should be empty");

    Origin origin(L"scheme", L"host", 123);
    SecurityOriginData securityData(WrtDB::FEATURE_WEB_NOTIFICATION, origin);
    securityOriginDAO->setSecurityOriginData(securityData, RESULT_ALLOW_ONCE, false);

    Origin origin2(L"scheme2", L"host2", 1234);
    SecurityOriginData securityData2(WrtDB::FEATURE_GEOLOCATION, origin2);
    securityOriginDAO->setSecurityOriginData(securityData2, RESULT_DENY_ONCE, true);

    Origin origin3(L"scheme3", L"host3", 2546);
    SecurityOriginData securityData3(WrtDB::FEATURE_FULLSCREEN_MODE, origin3);
    securityOriginDAO->setSecurityOriginData(securityData3, RESULT_DENY_ONCE, false);

    dataList = securityOriginDAO->getSecurityOriginDataList();
    RUNNER_ASSERT_MSG(dataList.size() ==  3, "CertificateDataList should not be empty");

    securityOriginDAO->removeSecurityOriginData(RESULT_DENY_ONCE);

    dataList = securityOriginDAO->getSecurityOriginDataList();
    RUNNER_ASSERT_MSG(dataList.size() ==  1, "CertificateDataList should not be empty");

    securityOriginDAO->removeSecurityOriginData(RESULT_ALLOW_ONCE);

    dataList = securityOriginDAO->getSecurityOriginDataList();
    RUNNER_ASSERT_MSG(dataList.empty(), "SecurityOriginDataList should be empty");
}

/**
 * Name: security_origin_dao_origin_operators
 * Description: Test comparison operators for Origin class.
 * Expected: All operations should succeed.
 */
RUNNER_TEST(security_origin_dao_origin_operators)
{
    Origin origin1(L"scheme", L"host", 123);
    Origin origin2(L"scheme", L"host", 123);
    Origin origin3(L"scheme1", L"host", 123);
    Origin origin4(L"scheme", L"host2", 123);
    Origin origin5(L"scheme", L"host", 122);

    RUNNER_ASSERT(origin1 == origin2);
    RUNNER_ASSERT(origin2 == origin1);
    RUNNER_ASSERT(origin1 != origin3);
    RUNNER_ASSERT(origin1 != origin4);
    RUNNER_ASSERT(origin1 != origin5);
    RUNNER_ASSERT(origin3 != origin4);
    RUNNER_ASSERT(origin4 != origin5);
}

/**
 * Name: security_origin_dao_data_operators
 * Description: Test comparison operators for SecurityOriginData class.
 * Expected: All operations should succeed.
 */
RUNNER_TEST(security_origin_dao_data_operators)
{
    SecurityOriginData data1(WrtDB::FEATURE_WEB_NOTIFICATION, Origin(L"scheme", L"host", 123));
    SecurityOriginData data2(WrtDB::FEATURE_WEB_NOTIFICATION, Origin(L"scheme", L"host", 123));
    SecurityOriginData data3(WrtDB::FEATURE_END, Origin(L"scheme", L"host", 123));
    SecurityOriginData data4(WrtDB::FEATURE_WEB_NOTIFICATION, Origin(L"scheme1", L"host", 123));
    SecurityOriginData data5(WrtDB::FEATURE_WEB_NOTIFICATION, Origin(L"scheme", L"host1", 123));
    SecurityOriginData data6(WrtDB::FEATURE_WEB_NOTIFICATION, Origin(L"scheme", L"host", 124));

    RUNNER_ASSERT(data1 == data2);
    RUNNER_ASSERT(data2 == data1);
    RUNNER_ASSERT(data1 != data3);
    RUNNER_ASSERT(data1 != data4);
    RUNNER_ASSERT(data1 != data5);
    RUNNER_ASSERT(data1 != data6);
    RUNNER_ASSERT(data3 != data4);
    RUNNER_ASSERT(data3 != data5);
    RUNNER_ASSERT(data3 != data6);
    RUNNER_ASSERT(data4 != data5);
    RUNNER_ASSERT(data4 != data6);
    RUNNER_ASSERT(data5 != data6);
}
