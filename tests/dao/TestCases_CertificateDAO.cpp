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
 * @file   TestCases_CertificateDAO.cpp
 * @author  Slawomir Pajak (s.pajak@partner.samsung.com)
 * @version 1.0
 * @brief   This file contains tests for certificate dao class.
 */

#include <dpl/test/test_runner.h>
#include <wrt-commons/certificate-dao/certificate_dao.h>

RUNNER_TEST_GROUP_INIT(DAO)

using namespace CertificateDB;

namespace {

class CertificateDAOWrapper {
public:
    static CertificateDAO* getCertificateDAO()
    {
        static CertificateDAOPtr certificateDAO(
                new CertificateDB::CertificateDAO(DPL::FromASCIIString("testWidget123")));
        return certificateDAO.get();
    }
};
}

/**
 * Name: certificate_dao_save_remove_by_certdata
 * Description: Tests saves certificateDao object, reads it, and removes it at the end
 *              using certificate data value.
 * Expected: All operations should succeed.
 */
RUNNER_TEST(certificate_dao_save_remove_by_certdata)
{
    CertificateDAO* certificateDAO = CertificateDAOWrapper::getCertificateDAO();

    CertificateDataList dataList = certificateDAO->getCertificateDataList();
    RUNNER_ASSERT_MSG(dataList.empty(), "CertificateDataList should be empty");

    CertificateData certData(DPL::FromASCIIString("sampleCetificate"));
    certificateDAO->setCertificateData(certData, RESULT_ALLOW_ONCE);

    dataList = certificateDAO->getCertificateDataList();

    RUNNER_ASSERT_MSG(dataList.size() == 1, "CertificateDataList should not be empty");
    RUNNER_ASSERT_MSG((*dataList.begin())->certificate == DPL::FromASCIIString("sampleCetificate"),
            "Wrong element in table");

    RUNNER_ASSERT_MSG(certificateDAO->getResult(**dataList.begin()) == RESULT_ALLOW_ONCE, "Wrong result");

    certificateDAO->removeCertificateData(certData);

    dataList = certificateDAO->getCertificateDataList();
    RUNNER_ASSERT_MSG(dataList.empty(), "CertificateDataList should be empty");

}

/**
 * Name: certificate_dao_save_remove_by_result
 * Description: Tests saves certificateDao object, reads it, and removes it at the end
 *              using result value.
 * Expected: All operations should succeed.
 */
RUNNER_TEST(certificate_dao_save_remove_by_result)
{
    CertificateDAO* certificateDAO = CertificateDAOWrapper::getCertificateDAO();

    CertificateDataList dataList = certificateDAO->getCertificateDataList();
    RUNNER_ASSERT_MSG(dataList.empty(), "CertificateDataList should be empty");

    CertificateData certData(DPL::FromASCIIString("sampleCetificate1"));
    certificateDAO->setCertificateData(certData, RESULT_ALLOW_ALWAYS);

    CertificateData certData2(DPL::FromASCIIString("sampleCetificate2"));
    certificateDAO->setCertificateData(certData2, RESULT_ALLOW_ALWAYS);

    dataList = certificateDAO->getCertificateDataList();
    RUNNER_ASSERT_MSG(dataList.size() == 2, "CertificateDataList should not be empty");

    certificateDAO->removeCertificateData(RESULT_ALLOW_ALWAYS);
    dataList = certificateDAO->getCertificateDataList();
    RUNNER_ASSERT_MSG(dataList.empty(), "CertificateDataList should be empty");
}

/**
 * Name: certificate_dao_update
 * Description: Tests update of certificateDao object.
 * Expected: All operations should succeed.
 */
RUNNER_TEST(certificate_dao_update)
{
    CertificateDAO* certificateDAO = CertificateDAOWrapper::getCertificateDAO();

    CertificateDataList dataList = certificateDAO->getCertificateDataList();
    RUNNER_ASSERT_MSG(dataList.empty(), "CertificateDataList should be empty");

    CertificateData certData(DPL::FromASCIIString("sampleCetificate3"));
    certificateDAO->setCertificateData(certData, RESULT_ALLOW_ONCE);

    dataList = certificateDAO->getCertificateDataList();
    RUNNER_ASSERT_MSG(dataList.size() == 1, "CertificateDataList should not be empty");
    RUNNER_ASSERT_MSG((*dataList.begin())->certificate == DPL::FromASCIIString("sampleCetificate3"),
            "Wrong element in table");
    RUNNER_ASSERT_MSG(certificateDAO->getResult(**dataList.begin()) == RESULT_ALLOW_ONCE, "Wrong result");

    certificateDAO->setCertificateData(certData, RESULT_DENY_ALWAYS);
    dataList = certificateDAO->getCertificateDataList();
    RUNNER_ASSERT_MSG(dataList.size() == 1, "CertificateDataList should not be empty");
    RUNNER_ASSERT_MSG((*dataList.begin())->certificate == DPL::FromASCIIString("sampleCetificate3"),
            "Wrong element in table");
    RUNNER_ASSERT_MSG(certificateDAO->getResult(**dataList.begin()) == RESULT_DENY_ALWAYS, "Wrong result");

    certificateDAO->removeCertificateData(certData);
    dataList = certificateDAO->getCertificateDataList();
    RUNNER_ASSERT_MSG(dataList.empty(), "CertificateDataList should be empty");
}

/**
 * Name: certificate_dao_equality_operator
 * Description: Tests CertificateData equality operators.
 * Expected: All operations should succeed.
 */
RUNNER_TEST(certificate_dao_equality_operator)
{
    CertificateData certData1(DPL::FromASCIIString("sampleCetificate1"));
    CertificateData certData2(DPL::FromASCIIString("sampleCetificate1"));
    CertificateData certData3(DPL::FromASCIIString("sampleCetificate2"));

    RUNNER_ASSERT_MSG(certData1 == certData2, "Object should be equal");
    RUNNER_ASSERT_MSG(certData1 != certData3, "Object should not be equal");
    RUNNER_ASSERT_MSG(certData2 != certData3, "Object should not be equal");
}

