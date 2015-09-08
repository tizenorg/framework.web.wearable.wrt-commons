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
 * @file        test_sql_connection.cpp
 * @author      Przemyslaw Dobrowolski (p.dobrowolsk@samsung.com)
 * @version     1.0
 * @brief       This file is the implementation file of sql connection tests
 */
#include <dpl/test/test_runner.h>
#include <dpl/db/sql_connection.h>
#include <dpl/db/naive_synchronization_object.h>
#include <memory>
#include <dpl/log/log.h>
#include <sstream>
#include <string>
#include <cstdlib>
#include <ctime>

extern const char* PATH_DB;

RUNNER_TEST_GROUP_INIT(DPL)

class AbstractSynchronizationObjectGenerator
{
  public:
    virtual ~AbstractSynchronizationObjectGenerator() {}

    virtual DPL::DB::SqlConnection::SynchronizationObject *Create() = 0;
};

class NaiveSynchronizationObjectGenerator :
    public AbstractSynchronizationObjectGenerator
{
  public:
    virtual DPL::DB::SqlConnection::SynchronizationObject *Create()
    {
        return new DPL::DB::NaiveSynchronizationObject();
    }
};

void MassiveReadWriteTest(AbstractSynchronizationObjectGenerator *generator);

class StressGenerator :
    public DPL::Thread
{
  private:
    size_t m_prefix;
    std::string m_dbFileName;
    AbstractSynchronizationObjectGenerator *m_generator;

  protected:
    virtual int ThreadEntry()
    {
        DPL::DB::SqlConnection connection(
            m_dbFileName,
            DPL::DB::SqlConnection::Flag::None,
            DPL::DB::SqlConnection::Flag::RW,
            m_generator->Create());

        DPL::DB::SqlConnection::DataCommandAutoPtr countCommand =
            connection.PrepareDataCommand(
                "SELECT COUNT(*) FROM test WHERE value=?");

        for (size_t i = 0; i < 10; ++i) {
            std::ostringstream valueStream;

            valueStream << "value_";
            valueStream << static_cast<unsigned long>(m_prefix);
            valueStream << "_";
            valueStream << static_cast<unsigned long>(i);

            std::string value = valueStream.str();

            connection.ExecCommand(
                "INSERT INTO test VALUES ('%s');",
                value.c_str());

            countCommand->BindString(1, value.c_str());

            RUNNER_ASSERT(countCommand->Step());

            RUNNER_ASSERT(countCommand->GetColumnString(0) == "1");

            countCommand->Reset();
        }

        countCommand.reset();

        return 0;
    }

  public:
    StressGenerator(size_t prefix,
                    const std::string &dbFileName,
                    AbstractSynchronizationObjectGenerator *generator) :
        m_prefix(prefix),
        m_dbFileName(dbFileName),
        m_generator(generator)
    {}
};

typedef std::shared_ptr<DPL::Thread> ThreadPtr;

void MassiveReadWriteTest(AbstractSynchronizationObjectGenerator *generator)
{
    DPL::DB::SqlConnection connection(PATH_DB,
                                      DPL::DB::SqlConnection::Flag::UseLucene,
                                      DPL::DB::SqlConnection::Flag::RW);

    connection.ExecCommand("CREATE TABLE test(value TEXT);");

    const size_t STRESS_GENERATOR_COUNT = 5;
    ThreadPtr stressGenerators[STRESS_GENERATOR_COUNT];

    for (size_t i = 0; i < STRESS_GENERATOR_COUNT; ++i) {
        stressGenerators[i].reset(
            new StressGenerator(i, PATH_DB, generator));

        stressGenerators[i]->Run();
    }

    for (size_t i = 0; i < STRESS_GENERATOR_COUNT; ++i) {
        stressGenerators[i]->Quit();
    }

    connection.ExecCommand("DROP TABLE test;");
}

/*
Name: SqlConnection_MassiveReadWrite_NaiveSynchronization
Description: tests massive multiple quiries from many threads
Expected: no ORM/db failures
*/
RUNNER_TEST(SqlConnection_MassiveReadWrite_NaiveSynchronization)
{
    srand(time(NULL));

    NaiveSynchronizationObjectGenerator m_generator;
    MassiveReadWriteTest(&m_generator);
}

/*
Name: SqlConnection_Not_Connected_Lucene
Description: tests connection to not existing database with Lucene option
Expected: exception throw
*/
RUNNER_TEST(SqlConnection_Not_Connected_Lucene)
{
    Try {
        DPL::DB::SqlConnection connection(
            "/notexisitingdirectiory/foo",
            DPL::DB::SqlConnection::Flag::
                UseLucene,
            DPL::DB::SqlConnection::Flag::RW);
        RUNNER_ASSERT_MSG(false,
                          "connection should throw on accessing "
                          "nonexistent file as a database");
    }
    Catch(DPL::DB::SqlConnection::Exception::ConnectionBroken)
    {
        RUNNER_ASSERT(true);
    } catch (DPL::Exception) {
        RUNNER_ASSERT_MSG(false, "Wrong exception found");
    }
}

/*
Name: SqlConnection_Not_Connected
Description: tests connection to not existing database without Lucene option
Expected: exception throw
*/
RUNNER_TEST(SqlConnection_Not_Connected)
{
    Try {
        DPL::DB::SqlConnection connection("/notexisitingdirectiory/foo",
                                          DPL::DB::SqlConnection::Flag::None,
                                          DPL::DB::SqlConnection::Flag::RW);
        RUNNER_ASSERT_MSG(false,
                          "connection should throw on accessing "
                          "nonexistent file as a database");
    }
    Catch(DPL::DB::SqlConnection::Exception::ConnectionBroken)
    {
        RUNNER_ASSERT(true);
    } catch (DPL::Exception) {
        RUNNER_ASSERT_MSG(false, "Wrong exception found");
    }
}

/*
Name: SqlConnection_Null_Query
Description: tests resistance to passing NULL as query in ExecCommand
Expected: exception throw
*/
RUNNER_TEST(SqlConnection_Null_Query)
{
    DPL::DB::SqlConnection connection(PATH_DB,
                                      DPL::DB::SqlConnection::Flag::UseLucene,
                                      DPL::DB::SqlConnection::Flag::RW);
    Try
    {
        connection.ExecCommand(NULL);
        RUNNER_ASSERT_MSG(false,
                          "Null pointer should not be accepted");
    }
    Catch(DPL::DB::SqlConnection::Exception::SyntaxError)
    {
        RUNNER_ASSERT(true);
    } catch (DPL::Exception) {
        RUNNER_ASSERT_MSG(false, "Wrong exception found");
    }
}

/*
Name: SqlConnection_Bad_Query
Description: tests resistance to passing trash as query in ExecCommand
Expected: exception throw
*/
RUNNER_TEST(SqlConnection_Bad_Query)
{
    DPL::DB::SqlConnection connection(PATH_DB,
                                      DPL::DB::SqlConnection::Flag::UseLucene,
                                      DPL::DB::SqlConnection::Flag::RW);
    Try
    {
        connection.ExecCommand("Some stupid string");
        RUNNER_ASSERT_MSG(false, "This string should not be accepted");
    }
    Catch(DPL::DB::SqlConnection::Exception::SyntaxError)
    {
        RUNNER_ASSERT(true);
    } catch (DPL::Exception) {
        RUNNER_ASSERT_MSG(false, "Wrong exception found");
    }
}

/*
Name: SqlConnection_IsNull
Description: tests IsColumnNull function
Expected: Function returns correct values
*/
RUNNER_TEST(SqlConnection_IsNull)
{
    DPL::DB::SqlConnection connection(PATH_DB, DPL::DB::SqlConnection::Flag::UseLucene,
            DPL::DB::SqlConnection::Flag::RW);

    connection.ExecCommand("CREATE TABLE testNull(value INT8);");

    connection.ExecCommand("INSERT INTO testNull VALUES (NULL);");
    connection.ExecCommand("INSERT INTO testNull VALUES (0);");

    DPL::DB::SqlConnection::DataCommandAutoPtr selectCommand = connection.PrepareDataCommand(
            "SELECT value FROM testNull");

    RUNNER_ASSERT(selectCommand->Step());
    RUNNER_ASSERT(selectCommand->IsColumnNull(0));
    RUNNER_ASSERT(selectCommand->Step());
    RUNNER_ASSERT(!selectCommand->IsColumnNull(0));
    selectCommand->Reset();

    connection.ExecCommand("DROP TABLE testNull;");
}

/*
Name: SqlConnection_Int8
Description: tests bind and getColumn functions for Int8
Expected: Functions returns correct values
*/
RUNNER_TEST(SqlConnection_Int8)
{
    DPL::DB::SqlConnection connection(PATH_DB, DPL::DB::SqlConnection::Flag::UseLucene,
            DPL::DB::SqlConnection::Flag::RW);

    connection.ExecCommand("CREATE TABLE testInt8(value INT8);");

    DPL::DB::SqlConnection::DataCommandAutoPtr insertCommand = connection.PrepareDataCommand(
            "INSERT INTO testInt8 VALUES (?)");

    DPL::DB::SqlConnection::DataCommandAutoPtr selectCommand = connection.PrepareDataCommand(
            "SELECT value FROM testInt8");

    insertCommand->BindInt8(1, 127);
    RUNNER_ASSERT(!insertCommand->Step());
    insertCommand->Reset();

    insertCommand->BindInt8(1, DPL::Optional<int8_t>(-127));
    RUNNER_ASSERT(!insertCommand->Step());
    insertCommand->Reset();

    RUNNER_ASSERT(selectCommand->Step());
    RUNNER_ASSERT(selectCommand->GetColumnInt8(0) == 127);
    RUNNER_ASSERT(selectCommand->Step());
    RUNNER_ASSERT(selectCommand->GetColumnOptionalInt8(0) == -127);
    selectCommand->Reset();

    connection.ExecCommand("DROP TABLE testInt8;");
}

/*
Name: SqlConnection_Int16
Description: tests bind and getColumn functions for Int16
Expected: Functions returns correct values
*/
RUNNER_TEST(SqlConnection_Int16)
{
    DPL::DB::SqlConnection connection(PATH_DB, DPL::DB::SqlConnection::Flag::UseLucene,
            DPL::DB::SqlConnection::Flag::RW);

    connection.ExecCommand("CREATE TABLE testInt16(value INT16);");

    DPL::DB::SqlConnection::DataCommandAutoPtr insertCommand = connection.PrepareDataCommand(
            "INSERT INTO testInt16 VALUES (?)");

    DPL::DB::SqlConnection::DataCommandAutoPtr selectCommand = connection.PrepareDataCommand(
            "SELECT value FROM testInt16");

    insertCommand->BindInt16(1, (int16_t)0xFFFF);
    RUNNER_ASSERT(!insertCommand->Step());
    insertCommand->Reset();

    insertCommand->BindInt16(1, DPL::Optional<int16_t>((int16_t)0x8000));
    RUNNER_ASSERT(!insertCommand->Step());
    insertCommand->Reset();

    RUNNER_ASSERT(selectCommand->Step());
    RUNNER_ASSERT(selectCommand->GetColumnInt16(0) == (int16_t)0xFFFF);
    RUNNER_ASSERT(selectCommand->Step());
    RUNNER_ASSERT(selectCommand->GetColumnOptionalInt16(0) == (int16_t)0x8000);
    selectCommand->Reset();

    connection.ExecCommand("DROP TABLE testInt16;");
}

/*
Name: SqlConnection_Int32
Description: tests bind and getColumn functions for Int32
Expected: Functions returns correct values
*/
RUNNER_TEST(SqlConnection_Int32)
{
    DPL::DB::SqlConnection connection(PATH_DB, DPL::DB::SqlConnection::Flag::UseLucene,
            DPL::DB::SqlConnection::Flag::RW);

    connection.ExecCommand("CREATE TABLE testInt32(value INT32);");

    DPL::DB::SqlConnection::DataCommandAutoPtr insertCommand = connection.PrepareDataCommand(
            "INSERT INTO testInt32 VALUES (?)");

    DPL::DB::SqlConnection::DataCommandAutoPtr selectCommand = connection.PrepareDataCommand(
            "SELECT value FROM testInt32");

    insertCommand->BindInt32(1, 0xFFFFFFFF);
    RUNNER_ASSERT(!insertCommand->Step());
    insertCommand->Reset();

    insertCommand->BindInt32(1, DPL::Optional<int32_t>(0x80000000));
    RUNNER_ASSERT(!insertCommand->Step());
    insertCommand->Reset();

    RUNNER_ASSERT(selectCommand->Step());
    RUNNER_ASSERT(selectCommand->GetColumnInt32(0) == (int32_t)0xFFFFFFFF);
    RUNNER_ASSERT(selectCommand->Step());
    RUNNER_ASSERT(selectCommand->GetColumnOptionalInt32(0) == 0x80000000);
    selectCommand->Reset();

    connection.ExecCommand("DROP TABLE testInt32;");
}

/*
Name: SqlConnection_Int64
Description: tests bind and getColumn functions for Int64
Expected: Functions returns correct values
*/
RUNNER_TEST(SqlConnection_Int64)
{
    DPL::DB::SqlConnection connection(PATH_DB, DPL::DB::SqlConnection::Flag::UseLucene,
            DPL::DB::SqlConnection::Flag::RW);

    connection.ExecCommand("CREATE TABLE testInt64(value INT64);");

    DPL::DB::SqlConnection::DataCommandAutoPtr insertCommand = connection.PrepareDataCommand(
            "INSERT INTO testInt64 VALUES (?)");

    DPL::DB::SqlConnection::DataCommandAutoPtr selectCommand = connection.PrepareDataCommand(
            "SELECT value FROM testInt64");

    insertCommand->BindInt64(1, 0xFFFFFFFFFFFFFFFF);
    RUNNER_ASSERT(!insertCommand->Step());
    insertCommand->Reset();

    insertCommand->BindInt64(1, DPL::Optional<int64_t>(0x8000000000000000));
    RUNNER_ASSERT(!insertCommand->Step());
    insertCommand->Reset();


    RUNNER_ASSERT(selectCommand->Step());
    RUNNER_ASSERT(selectCommand->GetColumnInt64(0) == (int64_t)0xFFFFFFFFFFFFFFFF);
    RUNNER_ASSERT(selectCommand->Step());
    RUNNER_ASSERT(selectCommand->GetColumnOptionalInt64(0) == 0x8000000000000000);
    selectCommand->Reset();

    connection.ExecCommand("DROP TABLE testInt64;");
}

/*
Name: SqlConnection_Float
Description: tests bind and getColumn functions for float
Expected: Functions returns correct values
*/
RUNNER_TEST(SqlConnection_Float)
{
    DPL::DB::SqlConnection connection(PATH_DB, DPL::DB::SqlConnection::Flag::UseLucene,
            DPL::DB::SqlConnection::Flag::RW);

    connection.ExecCommand("CREATE TABLE testFloat(value FLOAT);");

    DPL::DB::SqlConnection::DataCommandAutoPtr insertCommand = connection.PrepareDataCommand(
            "INSERT INTO testFloat VALUES (?)");

    DPL::DB::SqlConnection::DataCommandAutoPtr selectCommand = connection.PrepareDataCommand(
            "SELECT value FROM testFloat");

    insertCommand->BindFloat(1, 10.2545f);
    RUNNER_ASSERT(!insertCommand->Step());
    insertCommand->Reset();

    insertCommand->BindFloat(1, DPL::Optional<float>(-90.6788f));
    RUNNER_ASSERT(!insertCommand->Step());
    insertCommand->Reset();

    RUNNER_ASSERT(selectCommand->Step());
    float value = selectCommand->GetColumnFloat(0);
    RUNNER_ASSERT(value > 10.2544 && value < 10.2546);
    RUNNER_ASSERT(selectCommand->Step());
    value = *selectCommand->GetColumnOptionalFloat(0);
    RUNNER_ASSERT(value > -90.6789 && value < -90.6787);
    selectCommand->Reset();

    connection.ExecCommand("DROP TABLE testFloat;");
}

/*
Name: SqlConnection_Double
Description: tests bind and getColumn functions for double
Expected: Functions returns correct values
*/
RUNNER_TEST(SqlConnection_Double)
{
    DPL::DB::SqlConnection connection(PATH_DB, DPL::DB::SqlConnection::Flag::UseLucene,
            DPL::DB::SqlConnection::Flag::RW);

    connection.ExecCommand("CREATE TABLE testDouble(value DOUBLE);");

    DPL::DB::SqlConnection::DataCommandAutoPtr insertCommand = connection.PrepareDataCommand(
            "INSERT INTO testDouble VALUES (?)");

    DPL::DB::SqlConnection::DataCommandAutoPtr selectCommand = connection.PrepareDataCommand(
            "SELECT value FROM testDouble");

    insertCommand->BindDouble(1, 10.2545);
    RUNNER_ASSERT(!insertCommand->Step());
    insertCommand->Reset();

    insertCommand->BindDouble(1, DPL::Optional<double>(-90.6788));
    RUNNER_ASSERT(!insertCommand->Step());
    insertCommand->Reset();

    RUNNER_ASSERT(selectCommand->Step());
    double value = selectCommand->GetColumnDouble(0);
    RUNNER_ASSERT(value > 10.2544 && value < 10.2546);
    RUNNER_ASSERT(selectCommand->Step());
    value = *selectCommand->GetColumnOptionalDouble(0);
    RUNNER_ASSERT(value > -90.6789 && value < -90.6787);
    selectCommand->Reset();

    connection.ExecCommand("DROP TABLE testDouble;");
}
