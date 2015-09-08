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
 * @file   TestCases_widgetInterfaceDAO.cpp
 * @author  Dominik Duda (d.duda@samsung.com)
 * @version 1.0
 * @brief   This file contains tests for WidgetInterfaceDAO class.
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
#include <wrt-commons/widget-interface-dao/widget_interface_dao.h>

#define WIDGET_ID 2000

using namespace WrtDB;
using namespace WidgetInterfaceDB;

RUNNER_TEST_GROUP_INIT(DAO)

/*
 * Name: widget_interface_dao_test_01_initialization
 * Description: Tests database creation.
 * Expected: The database should be successfully created.
 */
RUNNER_TEST(widget_interface_dao_test_01_initialization)
{
    Try
    {
        WidgetInterfaceDAO dao(WIDGET_ID);
    }
    Catch(WidgetInterfaceDAO::Exception::DatabaseError)
    {
        RUNNER_ASSERT_MSG(false, _rethrown_exception.GetMessage());
    }
}

/*
 * Name: widget_interface_dao_test_01_initialization_Exception
 * Description: Tests if an exception will be thrown when incorrect widgetID
 * is used.
 * Expected: The exception should be thrown.
 */
RUNNER_TEST(widget_interface_dao_test_01_initialization_Exception)
{
    Try
    {
        WidgetInterfaceDAO dao(5555000);
    }
    Catch(WidgetDAOReadOnly::Exception::WidgetNotExist)
    {
        RUNNER_ASSERT_MSG(true, _rethrown_exception.GetMessage());
    }
    Catch(WidgetInterfaceDAO::Exception::DatabaseError)
    {
        RUNNER_ASSERT_MSG(false, _rethrown_exception.GetMessage());
    }
}

/*
 * Name: widget_interface_dao_test_02_readValue
 * Description: Tests getting a value for a given key.
 * Expected: The desired value should be returned.
 */
RUNNER_TEST(widget_interface_dao_test_02_readValue)
{
    DPL::Optional<std::string> value;

    Try
    {
        WidgetInterfaceDAO dao(WIDGET_ID);

        value = dao.getValue("key1_for_2000");

        RUNNER_ASSERT(value == "value_for_key1_2000");
    }
    Catch(WidgetInterfaceDAO::Exception::DatabaseError)
    {
        RUNNER_ASSERT_MSG(false, _rethrown_exception.GetMessage());
    }
}

/*
 * Name: widget_interface_dao_test_02_readValue_badKey
 * Description: Tests if an empty value is returned for an incorrect key.
 * Expected: The empty (NULL) value should be returned.
 */
RUNNER_TEST(widget_interface_dao_test_02_readValue_badKey)
{
    DPL::Optional<std::string> value;

    Try
    {
        WidgetInterfaceDAO dao(WIDGET_ID);

        value = dao.getValue("key1_for_200011111");

        RUNNER_ASSERT(value.IsNull());
    }
    Catch(WidgetInterfaceDAO::Exception::DatabaseError)
    {
        RUNNER_ASSERT_MSG(false, _rethrown_exception.GetMessage());
    }
}

/*
 * Name: widget_interface_dao_test_03_setItem
 * Description: Tests if a new readonly item can be add into a database.
 * Expected: The new item should be successfully added.
 */
RUNNER_TEST(widget_interface_dao_test_03_setItem)
{
    DPL::Optional<std::string> value;

    Try
    {
        WidgetInterfaceDAO dao(WIDGET_ID);

        dao.setItem("key3_for_2000", "value_for_key3_2000", 1);

        value = dao.getValue("key3_for_2000");

        RUNNER_ASSERT(value == "value_for_key3_2000");
    }
    Catch(WidgetInterfaceDAO::Exception::DatabaseError)
    {
        RUNNER_ASSERT_MSG(false, _rethrown_exception.GetMessage());
    }
}

/*
 * Name: widget_interface_dao_test_03_setItem_Exception
 * Description: Tests if a readonly item can be overwritten.
 * Expected: An exception should be thrown.
 */
RUNNER_TEST(widget_interface_dao_test_03_setItem_Exception)
{
    DPL::Optional<std::string> value;

    Try
    {
        WidgetInterfaceDAO dao(WIDGET_ID);

        dao.setItem("key3_for_2000", "value_for_key3_2000", 1);

        RUNNER_ASSERT_MSG(false, "Readonly value should not be overwritten");
    }
    Catch(WidgetInterfaceDAO::Exception::LocalStorageValueNoModifableException)
    {
        RUNNER_ASSERT(true);
    }
    Catch(WidgetInterfaceDAO::Exception::DatabaseError)
    {
        RUNNER_ASSERT_MSG(false, _rethrown_exception.GetMessage());
    }
}

/*
 * Name: widget_interface_dao_test_04_setItem
 * Description: Tests inserting a new item with non readonly and fromConfigXML
 * properties set.
 * Expected: The new item should be added.
 */
RUNNER_TEST(widget_interface_dao_test_04_setItem)
{
    DPL::Optional<std::string> value;

    Try
    {
        WidgetInterfaceDAO dao(WIDGET_ID);

        dao.setItem("key4_for_2000", "value_for_key4_2000", 0, 1);

        value = dao.getValue("key4_for_2000");

        RUNNER_ASSERT(value == "value_for_key4_2000");
    }
    Catch(WidgetInterfaceDAO::Exception::DatabaseError)
    {
        RUNNER_ASSERT_MSG(false, _rethrown_exception.GetMessage());
    }
}

/*
 * Name: widget_interface_dao_test_05_getKeyByIndex
 * Description: Tests getting a key by a given index.
 * Expected: The key should be successfully read.
 */
RUNNER_TEST(widget_interface_dao_test_05_getKeyByIndex)
{
    std::string key;

    Try
    {
        WidgetInterfaceDAO dao(WIDGET_ID);

        key = dao.getKeyByIndex(1);

        RUNNER_ASSERT(key == "key2_for_2000");
    }
    Catch(WidgetInterfaceDAO::Exception::DatabaseError)
    {
        RUNNER_ASSERT_MSG(false, _rethrown_exception.GetMessage());
    }
}

/*
 * Name: widget_interface_dao_test_05_getKeyByIndex_badIndex_01
 * Description: Tests if an exception will be thrown when getting a key
 * by a negative index.
 * Expected: An exception should be thrown.
 */
RUNNER_TEST(widget_interface_dao_test_05_getKeyByIndex_badIndex_01)
{
    std::string key;

    Try
    {
        WidgetInterfaceDAO dao(WIDGET_ID);

        key = dao.getKeyByIndex(-111);

        RUNNER_ASSERT_MSG(false, "A key should not be returned for "
                "a negative index!");
    }
    Catch(WidgetInterfaceDAO::Exception::InvalidArgumentException)
    {
        RUNNER_ASSERT_MSG(true, _rethrown_exception.GetMessage());
    }
    Catch(WidgetInterfaceDAO::Exception::DatabaseError)
    {
        RUNNER_ASSERT_MSG(false, _rethrown_exception.GetMessage());
    }
}

/*
 * Name: widget_interface_dao_test_05_getKeyByIndex_badIndex_02
 * Description: Tests if an exception will be thrown when getting a key
 * by a non existing index.
 * Expected: An exception should be thrown.
 */
RUNNER_TEST(widget_interface_dao_test_05_getKeyByIndex_badIndex_02)
{
    std::string key;

    Try
    {
        WidgetInterfaceDAO dao(WIDGET_ID);

        key = dao.getKeyByIndex(1111);

        RUNNER_ASSERT_MSG(false, "A key should not be returned for "
                "a non existing index!");
    }
    Catch(WidgetInterfaceDAO::Exception::InvalidArgumentException)
    {
        RUNNER_ASSERT(true);
    }
    Catch(WidgetInterfaceDAO::Exception::DatabaseError)
    {
        RUNNER_ASSERT_MSG(false, _rethrown_exception.GetMessage());
    }
}

/*
 * Name: widget_interface_dao_test_06_removeItem
 * Description: Tests removing an item for a given key.
 * Expected: An item should be successfully removed. The table size should be
 * smaller after removing.
 */
RUNNER_TEST(widget_interface_dao_test_06_removeItem)
{
    std::string key;
    size_t sizeBefore, sizeAfter;
    DPL::Optional<std::string> value;

    Try
    {
        WidgetInterfaceDAO dao(WIDGET_ID);

        sizeBefore = dao.getStorageSize();

        if (sizeBefore == 0)
        {
            RUNNER_ASSERT_MSG(false, "Database is empty!");
            return;
        }

        dao.removeItem("key4_for_2000");

        sizeAfter = dao.getStorageSize();
        value = dao.getValue("key4_for_2000");

        RUNNER_ASSERT(sizeAfter == sizeBefore - 1);
        RUNNER_ASSERT(value.IsNull());
    }
    Catch(WidgetInterfaceDAO::Exception::DatabaseError)
    {
        RUNNER_ASSERT_MSG(false, _rethrown_exception.GetMessage());
    }
}

/*
 * Name: widget_interface_dao_test_06_removeItem_Exception
 * Description: Tests if an exception will be thrown when a readonly item
 * is removed.
 * Expected: An exception should be thrown.
 */
RUNNER_TEST(widget_interface_dao_test_06_removeItem_Exception)
{
    WidgetInterfaceDAO* dao = NULL;
    DPL::Optional<std::string> value;
    size_t sizeBefore, sizeAfter;

    Try
    {
        dao = new WidgetInterfaceDAO(WIDGET_ID);

        sizeBefore = dao->getStorageSize();

        if (sizeBefore == 0)
        {
            RUNNER_ASSERT_MSG(false, "Database is empty!");
            return;
        }

        dao->removeItem("key3_for_2000");

        RUNNER_ASSERT_MSG(false, "A readonly item should not be removed!");
    }
    Catch(WidgetInterfaceDAO::Exception::LocalStorageValueNoModifableException)
    {
        sizeAfter = dao->getStorageSize();
        value = dao->getValue("key3_for_2000");

        RUNNER_ASSERT(sizeAfter == sizeBefore);
        RUNNER_ASSERT(value == "value_for_key3_2000");
    }
    Catch(WidgetInterfaceDAO::Exception::DatabaseError)
    {
        RUNNER_ASSERT_MSG(false, _rethrown_exception.GetMessage());
    }

    if (dao != NULL)
        delete dao;
}

/*
 * Name: widget_interface_dao_test_07_clear
 * Description: Tests removing all non readonly rows from the table.
 * Expected: The size of the table should be smaller than before removing rows.
 */
RUNNER_TEST(widget_interface_dao_test_07_clear)
{
    size_t sizeBefore, sizeAfter;
    DPL::Optional<std::string> value;

    Try
    {
        WidgetInterfaceDAO dao(WIDGET_ID);

        sizeBefore = dao.getStorageSize();
        dao.clear(false);

        sizeAfter = dao.getStorageSize();
        value = dao.getValue("key3_for_2000");

        RUNNER_ASSERT(sizeAfter == 1);
        RUNNER_ASSERT(sizeAfter < sizeBefore);
        RUNNER_ASSERT(value == "value_for_key3_2000");
    }
    Catch(WidgetInterfaceDAO::Exception::DatabaseError)
    {
        RUNNER_ASSERT_MSG(false, _rethrown_exception.GetMessage());
    }
}

/*
 * Name: widget_interface_dao_test_07_clearAll
 * Description: Tests removing all rows from the table.
 * Expected: The size of the table should be 0.
 */
RUNNER_TEST(widget_interface_dao_test_07_clearAll)
{
    size_t sizeAfter;

    Try
    {
        WidgetInterfaceDAO dao(WIDGET_ID);

        dao.clear(true);
        sizeAfter = dao.getStorageSize();

        RUNNER_ASSERT(sizeAfter == 0);
    }
    Catch(WidgetInterfaceDAO::Exception::DatabaseError)
    {
        RUNNER_ASSERT_MSG(false, _rethrown_exception.GetMessage());
    }
}
