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
/*
 * @file        test_waitable_handle_watch.cpp
 * @author      Slawomir Pajak (s.pajak@partner.samsung.com)
 * @version     1.0
 * @brief       This file is the implementation file of WaitableHandleWatchSupport tests
 */
#include <dpl/test/test_runner.h>

RUNNER_TEST_GROUP_INIT(DPL)

using namespace DPL;

class TestListener: public WaitableHandleWatchSupport::WaitableHandleListener {
public:
    int listenerHitCount;
    TestListener() :
            listenerHitCount(0)
    {
    }
    virtual ~TestListener()
    {
    }
    virtual void OnWaitableHandleEvent(WaitableHandle /*waitableHandle*/, WaitMode::Type /*mode*/)
    {
        listenerHitCount++;
    }
};

class TestWaitableHandleWatch: public WaitableHandleWatchSupport {
public:
    virtual ~TestWaitableHandleWatch()
    {
    }

    virtual Thread *GetInvokerThread()
    {
        return Thread::GetCurrentThread();
    }

    // Invoke direct invoker
    virtual void HandleDirectInvoker()
    {

    }

    using WaitableHandleWatchSupport::HandleWatcher;
};

/*
 Name: WaitableHandleWatchSupport_SingleListener
 Description: tests WaitableHandleWatchSupport simple case
 Expected: listener is notified
 */
RUNNER_TEST(WaitableHandleWatchSupport_SingleListener)
{
    TestWaitableHandleWatch handleWatch;
    WaitableHandle handle = 1;
    TestListener listener;
    handleWatch.AddWaitableHandleWatch(&listener, handle, WaitMode::Read);

    handleWatch.HandleWatcher(handle, WaitMode::Read);
    RUNNER_ASSERT(1 == listener.listenerHitCount);
}

/*
 Name: WaitableHandleWatchSupport_MultipleListener
 Description: tests WaitableHandleWatchSupport multiple listeners
 Expected: listeners are notified correctly
 */
RUNNER_TEST(WaitableHandleWatchSupport_MultipleListener)
{
    TestWaitableHandleWatch handleWatch;
    WaitableHandle handle = 1;
    TestListener listenerR1;
    TestListener listenerR2;
    TestListener listenerW1;
    TestListener listenerW2;
    handleWatch.AddWaitableHandleWatch(&listenerR1, handle, WaitMode::Read);
    handleWatch.AddWaitableHandleWatch(&listenerR2, handle, WaitMode::Read);
    handleWatch.AddWaitableHandleWatch(&listenerW1, handle, WaitMode::Write);
    handleWatch.AddWaitableHandleWatch(&listenerW2, handle, WaitMode::Write);

    handleWatch.HandleWatcher(handle, WaitMode::Read);
    RUNNER_ASSERT(1 == listenerR1.listenerHitCount);
    RUNNER_ASSERT(1 == listenerR2.listenerHitCount);
    RUNNER_ASSERT(0 == listenerW1.listenerHitCount);
    RUNNER_ASSERT(0 == listenerW2.listenerHitCount);

    handleWatch.HandleWatcher(handle, WaitMode::Write);
    RUNNER_ASSERT(1 == listenerR1.listenerHitCount);
    RUNNER_ASSERT(1 == listenerR2.listenerHitCount);
    RUNNER_ASSERT(1 == listenerW1.listenerHitCount);
    RUNNER_ASSERT(1 == listenerW2.listenerHitCount);
}

/*
 Name: WaitableHandleWatchSupport_MultipleHandle
 Description: tests WaitableHandleWatchSupport multiple handles
 Expected: listeners are notified correctly
 */
RUNNER_TEST(WaitableHandleWatchSupport_MultipleHandle)
{
    TestWaitableHandleWatch handleWatch;
    TestListener listener;
    WaitableHandle handle1 = 1;
    WaitableHandle handle2 = 2;
    handleWatch.AddWaitableHandleWatch(&listener, handle1, WaitMode::Read);
    handleWatch.AddWaitableHandleWatch(&listener, handle2, WaitMode::Read);
    handleWatch.AddWaitableHandleWatch(&listener, handle1, WaitMode::Write);
    handleWatch.AddWaitableHandleWatch(&listener, handle2, WaitMode::Write);

    handleWatch.HandleWatcher(handle1, WaitMode::Read);
    RUNNER_ASSERT(1 == listener.listenerHitCount);

    handleWatch.HandleWatcher(handle1, WaitMode::Write);
    RUNNER_ASSERT(2 == listener.listenerHitCount);

    handleWatch.HandleWatcher(handle2, WaitMode::Read);
    RUNNER_ASSERT(3 == listener.listenerHitCount);

    handleWatch.HandleWatcher(handle2, WaitMode::Write);
    RUNNER_ASSERT(4 == listener.listenerHitCount);
}

/*
 Name: WaitableHandleWatchSupport_AddRemoveListener
 Description: tests WaitableHandleWatchSupport removing listeners function
 Expected: remaining listeners are notified correctly, removed are not notified
 */
RUNNER_TEST(WaitableHandleWatchSupport_AddRemoveListener)
{
    TestWaitableHandleWatch handleWatch;
    TestListener listener;
    WaitableHandle handle1 = 1;
    WaitableHandle handle2 = 2;
    handleWatch.AddWaitableHandleWatch(&listener, handle1, WaitMode::Read);
    handleWatch.AddWaitableHandleWatch(&listener, handle2, WaitMode::Read);
    handleWatch.AddWaitableHandleWatch(&listener, handle1, WaitMode::Write);
    handleWatch.AddWaitableHandleWatch(&listener, handle2, WaitMode::Write);

    handleWatch.HandleWatcher(handle1, WaitMode::Read);
    RUNNER_ASSERT(1 == listener.listenerHitCount);

    handleWatch.RemoveWaitableHandleWatch(&listener, handle1, WaitMode::Write);
    handleWatch.HandleWatcher(handle1, WaitMode::Write);
    RUNNER_ASSERT(1 == listener.listenerHitCount);

    handleWatch.RemoveWaitableHandleWatch(&listener, handle2, WaitMode::Read);
    handleWatch.HandleWatcher(handle2, WaitMode::Read);
    RUNNER_ASSERT(1 == listener.listenerHitCount);

    handleWatch.HandleWatcher(handle2, WaitMode::Write);
    RUNNER_ASSERT(2 == listener.listenerHitCount);
}
