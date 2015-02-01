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
/*
 * @file    widget_version.cpp
 * @author  Przemyslaw Dobrowolski (p.dobrowolsk@samsung.com)
 * @version 1.0
 * @brief   Implementation file for test cases for engine internal tests
 */
#include <sstream>
#include <dpl/test/test_runner.h>
#include <dpl/utils/widget_version.h>

RUNNER_TEST_GROUP_INIT(DPL_WIDGET_VERSION)

/*
Name: WidgetVersion_M2_O0
Description: tests correct parsing of version widget in format: [major].[minor]
Expected: major and minor parts matches expected values
*/
RUNNER_TEST(WidgetVersion_M2_O0)
{
    DPL::String raw(L"1.2");
    WidgetVersion version(raw);

    RUNNER_ASSERT(version.IsWac() == true);
    RUNNER_ASSERT(version.Major() == DPL::String(L"1"));
    RUNNER_ASSERT(version.Minor() == DPL::String(L"2"));
    RUNNER_ASSERT(version.Micro() == DPL::Optional<DPL::String>());
    RUNNER_ASSERT(version.Optional() == DPL::Optional<DPL::String>());
    RUNNER_ASSERT(version.Raw() == raw);
}

/*
Name: WidgetVersion_M2_O0_nonwac_1
Description: tests if version is recognized as wac version number
Expected: version should not be recognized as wac compatible
*/
RUNNER_TEST(WidgetVersion_M2_O0_nonwac_1)
{
    DPL::String raw(L"a1.2");
    WidgetVersion version(raw);

    RUNNER_ASSERT(version.IsWac() == false);
    RUNNER_ASSERT(version.Raw() == raw);
}

/*
Name: WidgetVersion_M2_O0_nonwac_2
Description: tests if version is recognized as wac version number
Expected: version should not be recognized as wac compatible
*/
RUNNER_TEST(WidgetVersion_M2_O0_nonwac_2)
{
    DPL::String raw(L"1.2a");
    WidgetVersion version(raw);

    RUNNER_ASSERT(version.IsWac() == false);
    RUNNER_ASSERT(version.Raw() == raw);
}

/*
Name: WidgetVersion_M2_O0_nonwac_3
Description: tests if version is recognized as wac version number
Expected: version should not be recognized as wac compatible
*/
RUNNER_TEST(WidgetVersion_M2_O0_nonwac_3)
{
    DPL::String raw(L"aaa1.2bbb");
    WidgetVersion version(raw);

    RUNNER_ASSERT(version.IsWac() == false);
    RUNNER_ASSERT(version.Raw() == raw);
}

/*
Name: WidgetVersion_M2_O0_nonwac_4
Description: tests if version is recognized as wac version number
Expected: version should not be recognized as wac compatible
*/
RUNNER_TEST(WidgetVersion_M2_O0_nonwac_4)
{
    DPL::String raw(L"1a.a2");
    WidgetVersion version(raw);

    RUNNER_ASSERT(version.IsWac() == false);
    RUNNER_ASSERT(version.Raw() == raw);
}

/*
Name: WidgetVersion_M2_O0_long
Description: tests correct parsing of version widget in format: [major].[minor]
 for huge number
Expected: major and minor parts matches expected values
*/
RUNNER_TEST(WidgetVersion_M2_O0_long)
{
    DPL::String raw(
        L"123456789012345678901234567890.98765432109876543210987654321");
    WidgetVersion version(raw);

    RUNNER_ASSERT(version.IsWac() == true);
    RUNNER_ASSERT(version.Major() ==
                  DPL::String(L"123456789012345678901234567890"));
    RUNNER_ASSERT(version.Minor() ==
                  DPL::String(L"98765432109876543210987654321"));
    RUNNER_ASSERT(version.Micro() == DPL::Optional<DPL::String>());
    RUNNER_ASSERT(version.Optional() == DPL::Optional<DPL::String>());
    RUNNER_ASSERT(version.Raw() == raw);
}

/*
Name: WidgetVersion_M3_O0
Description: tests correct wac version number
Expected: major and minor and micro parts matches expected values.
 Version should be recognized as wac compatible
*/
RUNNER_TEST(WidgetVersion_M3_O0)
{
    DPL::String raw(L"1.2.3");
    WidgetVersion version(raw);

    RUNNER_ASSERT(version.IsWac() == true);
    RUNNER_ASSERT(version.Major() == DPL::String(L"1"));
    RUNNER_ASSERT(version.Minor() == DPL::String(L"2"));
    RUNNER_ASSERT(version.Micro() == DPL::Optional<DPL::String>(L"3"));
    RUNNER_ASSERT(version.Optional() == DPL::Optional<DPL::String>());
    RUNNER_ASSERT(version.Raw() == raw);
}

/*
Name: WidgetVersion_M3_O0_nonwac_1
Description: tests if version is recognized as wac version number
Expected: version should not be recognized as wac compatible
*/
RUNNER_TEST(WidgetVersion_M3_O0_nonwac_1)
{
    DPL::String raw(L"a1a.2.3");
    WidgetVersion version(raw);

    RUNNER_ASSERT(version.IsWac() == false);
    RUNNER_ASSERT(version.Raw() == raw);
}

/*
Name: WidgetVersion_M3_O0_nonwac_2
Description: tests if version is recognized as wac version number
Expected: version should not be recognized as wac compatible
*/
RUNNER_TEST(WidgetVersion_M3_O0_nonwac_2)
{
    DPL::String raw(L"1.b2.3");
    WidgetVersion version(raw);

    RUNNER_ASSERT(version.IsWac() == false);
    RUNNER_ASSERT(version.Raw() == raw);
}

/*
Name: WidgetVersion_M3_O0_nonwac_3
Description: tests if version is recognized as wac version number
Expected: version should not be recognized as wac compatible
*/
RUNNER_TEST(WidgetVersion_M3_O0_nonwac_3)
{
    DPL::String raw(L"1.2.3c");
    WidgetVersion version(raw);

    RUNNER_ASSERT(version.IsWac() == false);
    RUNNER_ASSERT(version.Raw() == raw);
}

/*
Name: WidgetVersion_M3_O1_1
Description: tests correct wac version number with optional part
Expected: major and minor and micro and optional parts matches expected values.
 Version should be recognized as wac compatible
*/
RUNNER_TEST(WidgetVersion_M3_O1_1)
{
    DPL::String raw(L"1.2.3 test111");
    WidgetVersion version(raw);

    RUNNER_ASSERT(version.IsWac() == true);
    RUNNER_ASSERT(version.Major() == DPL::String(L"1"));
    RUNNER_ASSERT(version.Minor() == DPL::String(L"2"));
    RUNNER_ASSERT(version.Micro() == DPL::Optional<DPL::String>(L"3"));
    RUNNER_ASSERT(version.Optional() == DPL::Optional<DPL::String>(L"test111"));
    RUNNER_ASSERT(version.Raw() == raw);
}

/*
Name: WidgetVersion_M3_O1_1
Description: tests correct wac version number with numeric optional part
Expected: major and minor and micro and optional parts matches expected values.
 Version should be recognized as wac compatible
*/
RUNNER_TEST(WidgetVersion_M3_O1_2)
{
    DPL::String raw(L"1.2.3 111");
    WidgetVersion version(raw);

    RUNNER_ASSERT(version.IsWac() == true);
    RUNNER_ASSERT(version.Major() == DPL::String(L"1"));
    RUNNER_ASSERT(version.Minor() == DPL::String(L"2"));
    RUNNER_ASSERT(version.Micro() == DPL::Optional<DPL::String>(L"3"));
    RUNNER_ASSERT(version.Optional() == DPL::Optional<DPL::String>(L"111"));
    RUNNER_ASSERT(version.Raw() == raw);
}

/*
Name: WidgetVersion_M3_O1_3
Description: tests if version is recognized as wac version number
 when trailer spaces exists
Expected: version should not be recognized as wac compatible
*/
RUNNER_TEST(WidgetVersion_M3_O1_3)
{
    DPL::String raw(L"1.2.3 ");
    WidgetVersion version(raw);

    RUNNER_ASSERT(version.IsWac() == false);
    RUNNER_ASSERT(version.Raw() == raw);
}

/*
Name: WidgetVersion_M2_O1_1
Description: tests if version is recognized as wac version number
 when optional part
Expected: version should be recognized as wac compatible
*/
RUNNER_TEST(WidgetVersion_M2_O1_1)
{
    DPL::String raw(L"1.2 t");
    WidgetVersion version(raw);

    RUNNER_ASSERT(version.IsWac() == true);
    RUNNER_ASSERT(version.Major() == DPL::String(L"1"));
    RUNNER_ASSERT(version.Minor() == DPL::String(L"2"));
    RUNNER_ASSERT(version.Micro() == DPL::Optional<DPL::String>());
    RUNNER_ASSERT(version.Optional() == DPL::Optional<DPL::String>(L"t"));
    RUNNER_ASSERT(version.Raw() == raw);
}

/*
Name: WidgetVersion_M2_O1_2
Description: tests if version is recognized as wac version number
 with numeric optional part.
Expected: major and minor and optional parts matches expected values.
 Version should be recognized as wac compatible.
*/
RUNNER_TEST(WidgetVersion_M2_O1_2)
{
    DPL::String raw(L"1.2 1234");
    WidgetVersion version(raw);

    RUNNER_ASSERT(version.IsWac() == true);
    RUNNER_ASSERT(version.Major() == DPL::String(L"1"));
    RUNNER_ASSERT(version.Minor() == DPL::String(L"2"));
    RUNNER_ASSERT(version.Micro() == DPL::Optional<DPL::String>());
    RUNNER_ASSERT(version.Optional() == DPL::Optional<DPL::String>(L"1234"));
    RUNNER_ASSERT(version.Raw() == raw);
}

/*
Name: WidgetVersion_M4_O0_1
Description: Tests if version is recognized as wac version number.
Expected: major and minor and micro and optional parts matches expected values.
 Version should be recognized as wac compatible.
*/
RUNNER_TEST(WidgetVersion_M4_O0_1)
{
    DPL::String raw(L"1.1");
    DPL::String majorV(L"1");
    DPL::String minorV(L"1");
    DPL::Optional<DPL::String> microV = DPL::Optional<DPL::String>();
    DPL::Optional<DPL::String> optionalV = DPL::Optional<DPL::String>();
    WidgetVersion version(majorV, minorV, microV, optionalV);

    RUNNER_ASSERT(version.IsWac() == true);
    RUNNER_ASSERT(version.Major() == majorV);
    RUNNER_ASSERT(version.Minor() == minorV);
    RUNNER_ASSERT(version.Micro() == microV);
    RUNNER_ASSERT(version.Optional() == optionalV);
    RUNNER_ASSERT(version.Raw() == raw);
}

/*
Name: WidgetVersion_M4_O0_nonwac_1
Description: Tests if version is recognized as wac version number.
Expected: Version should be recognized as non wac compatible.
*/
RUNNER_TEST(WidgetVersion_M4_O0_nonwac_1)
{
    DPL::String raw(L"a1.1");
    WidgetVersion version(L"a1", L"1", DPL::Optional<DPL::String>(), DPL::Optional<DPL::String>());

    RUNNER_ASSERT(version.IsWac() == false);
    RUNNER_ASSERT(version.Raw() == raw);
}

/*
Name: WidgetVersion_M4_O0_nonwac_2
Description: Tests if version is recognized as wac version number.
Expected: Version should not be recognized as wac compatible.
*/
RUNNER_TEST(WidgetVersion_M4_O0_nonwac_2)
{
    DPL::String raw(L"1.1a");
    WidgetVersion version(L"1", L"1a", DPL::Optional<DPL::String>(), DPL::Optional<DPL::String>());

    RUNNER_ASSERT(version.IsWac() == false);
    RUNNER_ASSERT(version.Raw() == raw);
}

/*
Name: WidgetVersion_M4_O1_1
Description: Tests if version is recognized as wac version number.
Expected: major, minor, micro and optional parts matches expected values.
 Version should be recognized as wac compatible.
*/
RUNNER_TEST(WidgetVersion_M4_O1_1)
{
    DPL::String raw(L"1.1.1");
    DPL::String majorV(L"1");
    DPL::String minorV(L"1");
    DPL::Optional<DPL::String> microV = DPL::Optional<DPL::String>(L"1");
    DPL::Optional<DPL::String> optionalV = DPL::Optional<DPL::String>();
    WidgetVersion version(majorV, minorV, microV, optionalV);

    RUNNER_ASSERT(version.IsWac() == true);
    RUNNER_ASSERT(version.Major() == majorV);
    RUNNER_ASSERT(version.Minor() == minorV);
    RUNNER_ASSERT(version.Micro() == microV);
    RUNNER_ASSERT(version.Optional() == optionalV);
    RUNNER_ASSERT(version.Raw() == raw);
}

/*
Name: WidgetVersion_M4_O1_nonwac_1
Description: Tests if version is recognized as wac version number.
Expected: Version should not be recognized as wac compatible.
*/
RUNNER_TEST(WidgetVersion_M4_O1_nonwac_1)
{
    DPL::String majorV(L"1");
    DPL::String minorV(L"1");
    WidgetVersion version(L"1", L"1", DPL::Optional<DPL::String>(L"1a"), DPL::Optional<DPL::String>());

    RUNNER_ASSERT(version.IsWac() == false);
    RUNNER_ASSERT(version.Raw() == L"1.1.1a");
}

/*
Name: WidgetVersion_M4_O1_nonwac_1
Description: Tests if version is recognized as wac version number.
Expected: Version should not be recognized as wac compatible.
*/
RUNNER_TEST(WidgetVersion_M4_O1_nonwac_2)
{
    WidgetVersion version(L"1", L"1", DPL::Optional<DPL::String>(L"a1"), DPL::Optional<DPL::String>());

    RUNNER_ASSERT(version.IsWac() == false);
    RUNNER_ASSERT(version.Raw() == L"1.1.a1");
}

/*
Name: WidgetVersion_M4_O2_1
Description: Tests if version is recognized as wac version number.
Expected: major, minor, micro and optional parts matches expected values.
 Version should be recognized as wac compatible.
*/
RUNNER_TEST(WidgetVersion_M4_O2_1)
{
    DPL::String raw(L"1.1.1 a1");
    DPL::String majorV(L"1");
    DPL::String minorV(L"1");
    DPL::Optional<DPL::String> microV = DPL::Optional<DPL::String>(L"1");
    DPL::Optional<DPL::String> optionalV = DPL::Optional<DPL::String>(L"a1");
    WidgetVersion version(majorV, minorV, microV, optionalV);

    RUNNER_ASSERT(version.IsWac() == true);
    RUNNER_ASSERT(version.Major() == majorV);
    RUNNER_ASSERT(version.Minor() == minorV);
    RUNNER_ASSERT(version.Micro() == microV);
    RUNNER_ASSERT(version.Optional() == optionalV);
    RUNNER_ASSERT(version.Raw() == raw);
}

/*
Name: WidgetVersion_M4_O2_1
Description: Tests if version is recognized as wac version number.
Expected: major, minor, micro and optional parts matches expected values.
 Version should be recognized as wac compatible.
*/
RUNNER_TEST(WidgetVersion_M4_O2_nonwac_1)
{
    WidgetVersion version(L"1", L"1", DPL::Optional<DPL::String>(L"a1"), DPL::Optional<DPL::String>(L"b1"));

    RUNNER_ASSERT(version.IsWac() == false);
    RUNNER_ASSERT(version.Raw() == L"1.1.a1 b1");
}

/*
Name: WidgetVersion_Strange_0
Description: tests if version is recognized as wac version number
Expected: version should not be recognized as wac compatible
*/
RUNNER_TEST(WidgetVersion_Strange_0)
{
    DPL::String raw(L"1");
    WidgetVersion version(raw);

    RUNNER_ASSERT(version.IsWac() == false);
    RUNNER_ASSERT(version.Raw() == raw);
}

/*
Name: WidgetVersion_Strange_1
Description: tests if version is recognized as wac version number
Expected: version should not be recognized as wac compatible
*/
RUNNER_TEST(WidgetVersion_Strange_1)
{
    DPL::String raw(L".1");
    WidgetVersion version(raw);

    RUNNER_ASSERT(version.IsWac() == false);
    RUNNER_ASSERT(version.Raw() == raw);
}

/*
Name: WidgetVersion_Strange_2
Description: tests if version is recognized as wac version number
Expected: version should not be recognized as wac compatible
*/
RUNNER_TEST(WidgetVersion_Strange_2)
{
    DPL::String raw(L"..1");
    WidgetVersion version(raw);

    RUNNER_ASSERT(version.IsWac() == false);
    RUNNER_ASSERT(version.Raw() == raw);
}

/*
Name: WidgetVersion_Strange_3
Description: tests if version is recognized as wac version number
Expected: version should not be recognized as wac compatible
*/
RUNNER_TEST(WidgetVersion_Strange_3)
{
    DPL::String raw(L"...1");
    WidgetVersion version(raw);

    RUNNER_ASSERT(version.IsWac() == false);
    RUNNER_ASSERT(version.Raw() == raw);
}

/*
Name: WidgetVersion_Strange_4
Description: tests if version is recognized as wac version number
Expected: version should not be recognized as wac compatible
*/
RUNNER_TEST(WidgetVersion_Strange_4)
{
    DPL::String raw(L"qwerty");
    WidgetVersion version(raw);

    RUNNER_ASSERT(version.IsWac() == false);
    RUNNER_ASSERT(version.Raw() == raw);
}

/*
Name: WidgetVersion_Strange_5
Description: tests if version is recognized as wac version number
Expected: version should not be recognized as wac compatible
*/
RUNNER_TEST(WidgetVersion_Strange_5)
{
    DPL::String raw(L"!@#$%^&*()_+   ^&%^*&%$^*&%*()&   JHKJLHKJLH   685685687");
    WidgetVersion version(raw);

    RUNNER_ASSERT(version.IsWac() == false);
    RUNNER_ASSERT(version.Raw() == raw);
}

/*
Name: WidgetVersion_Strange_6
Description: Tests if version is recognized as wac version number.
Expected: Version should not be recognized as wac compatible.
*/
RUNNER_TEST(WidgetVersion_Strange_6)
{
    WidgetVersion version(L"1", L"", DPL::Optional<DPL::String>(), DPL::Optional<DPL::String>());

    RUNNER_ASSERT(version.IsWac() == false);
    RUNNER_ASSERT(version.Raw() == L"1.");
}

/*
Name: WidgetVersion_Strange_7
Description: Tests if version is recognized as wac version number.
Expected: Version should not be recognized as wac compatible.
*/
RUNNER_TEST(WidgetVersion_Strange_7)
{
    WidgetVersion version(L"a", L"b", DPL::Optional<DPL::String>(), DPL::Optional<DPL::String>());

    RUNNER_ASSERT(version.IsWac() == false);
    RUNNER_ASSERT(version.Raw() == L"a.b");
}

/*
Name: WidgetVersion_Strange_8
Description: Tests if version is recognized as wac version number.
Expected: Version should not be recognized as wac compatible.
*/
RUNNER_TEST(WidgetVersion_Strange_8)
{
    WidgetVersion version(L"", L"", DPL::Optional<DPL::String>(L"12"), DPL::Optional<DPL::String>(L"abcd123"));

    RUNNER_ASSERT(version.IsWac() == false);
    RUNNER_ASSERT(version.Raw() == L"..12 abcd123");
}

/*
Name: WidgetVersion_Compare_0
Description: Tests versions comparision by less than operator.
Expected: Compare should work as expected.
*/
RUNNER_TEST(WidgetVersion_Compare_0)
{
    RUNNER_ASSERT(WidgetVersion(L"1.1") < WidgetVersion(L"1.2"));
    RUNNER_ASSERT(WidgetVersion(L"01.001") < WidgetVersion(L"0001.002"));
    RUNNER_ASSERT(WidgetVersion(L"01.001") < WidgetVersion(L"0001.0010"));
    RUNNER_ASSERT(WidgetVersion(L"01.001.01") < WidgetVersion(L"0001.001.012 abc"));
    RUNNER_ASSERT(WidgetVersion(L"087654321.0123456789") < WidgetVersion(L"0987654321.0123456789"));
}

/*
Name: WidgetVersion_Compare_1
Description: Tests versions comparision by more than operator.
Expected: Compare should work as expected.
*/
RUNNER_TEST(WidgetVersion_Compare_1)
{
    RUNNER_ASSERT(WidgetVersion(L"1.2") > WidgetVersion(L"1.1"));
    RUNNER_ASSERT(WidgetVersion(L"1.020") > WidgetVersion(L"0001.00002"));
    RUNNER_ASSERT(WidgetVersion(L"01.0020") > WidgetVersion(L"0001.001"));
    RUNNER_ASSERT(WidgetVersion(L"0001.0020.01") > WidgetVersion(L"01.0002.0 abcd"));
    RUNNER_ASSERT(WidgetVersion(
                      L"19647963733338932479072098437089778943732432.00000000000000004324324324324321")
                      > WidgetVersion(L"4324324324324324324321.000432"));
}

/*
Name: WidgetVersion_Compare_2
Description: Tests versions comparision by less than or equal operator.
Expected: Compare should work as expected.
*/
RUNNER_TEST(WidgetVersion_Compare_2)
{
    RUNNER_ASSERT(WidgetVersion(L"1.1") <= WidgetVersion(L"01.2"));
    RUNNER_ASSERT(WidgetVersion(L"0001.02") <= WidgetVersion(L"1.02"));
    RUNNER_ASSERT(WidgetVersion(L"001.002") <= WidgetVersion(L"0002.002"));
    RUNNER_ASSERT(WidgetVersion(L"2.00000000000000") <=
                  WidgetVersion(L"2.0 test"));
}

/*
Name: WidgetVersion_Compare_3
Description: Tests versions comparision more than or equal operator.
Expected: Compare should work as expected.
*/
RUNNER_TEST(WidgetVersion_Compare_3)
{
    RUNNER_ASSERT(WidgetVersion(L"1.2") >= WidgetVersion(L"1.1"));
    RUNNER_ASSERT(WidgetVersion(L"1.20") >= WidgetVersion(L"01.2"));
    RUNNER_ASSERT(WidgetVersion(L"001.20") >= WidgetVersion(L"01.0020"));
    RUNNER_ASSERT(WidgetVersion(L"001.20.11") >= WidgetVersion(L"01.0020.10 abc"));
    RUNNER_ASSERT(WidgetVersion(L"1.00000000000000") >=
                  WidgetVersion(L"1.0 test"));
}

/*
Name: WidgetVersion_Compare_4
Description: Tests versions comparsion by equality operator.
Expected: Versions should be equal.
*/
RUNNER_TEST(WidgetVersion_Compare_4)
{
    RUNNER_ASSERT(WidgetVersion(L"0.1") == WidgetVersion(L"00.1"));
    RUNNER_ASSERT(WidgetVersion(L"0002.0001") == WidgetVersion(L"02.01"));
    RUNNER_ASSERT(WidgetVersion(L"0001.02") == WidgetVersion(L"1.02"));
    RUNNER_ASSERT(WidgetVersion(L"2.0001.12") == WidgetVersion(L"002.01.12 abcd"));
    RUNNER_ASSERT(WidgetVersion(L"12345.1") == WidgetVersion(L"12345.1"));
    RUNNER_ASSERT(WidgetVersion(L"000123000.0 notatest") ==
                  WidgetVersion(L"00123000.0 testtesttest"));
}

/*
Name: WidgetVersion_Compare_5
Description: Tests versions comparsion by inequality operator.
Expected: Versions should not be equal.
*/
RUNNER_TEST(WidgetVersion_Compare_5)
{
    RUNNER_ASSERT(WidgetVersion(L"1.1") != WidgetVersion(L"1.11"));
    RUNNER_ASSERT(WidgetVersion(L"2.1.1") != WidgetVersion(L"2.1.11"));
    RUNNER_ASSERT(WidgetVersion(L"003.10.1") != WidgetVersion(L"3.10.11"));
    RUNNER_ASSERT(WidgetVersion(L"4.2.1 abc") != WidgetVersion(L"4.2.11 abc"));
}

/*
Name: WidgetVersion_Compare_6
Description: Tests insertin WidgetVersion object into a stream (operator<<)
Expected: Version should be successfully inserted into a stream.
*/
RUNNER_TEST(WidgetVersion_Compare_6)
{
    std::stringbuf buf;
    std::ostream stream(&buf);
    DPL::String raw(L"1.1");

    stream << WidgetVersion(raw);

    RUNNER_ASSERT(DPL::StringCompare(raw, DPL::FromASCIIString(buf.str())) == 0);
}
