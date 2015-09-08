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
 * @file        application.cpp
 * @author      Przemyslaw Dobrowolski (p.dobrowolsk@samsung.com)
 * @version     1.0
 * @brief       This file is the implementation file of MVC application support
 */
#include <stddef.h>
#include <dpl/application.h>
#include <dpl/log/log.h>

namespace // anonymous
{
static DPL::Application *g_application = NULL;
} // namespace anonymous

namespace DPL {
bool Application::app_create(void *data)
{
    Application *This = static_cast<Application *>(data);
    This->OnCreate();
    return true;
}

void Application::app_terminate(void *data)
{
    Application *This = static_cast<Application *>(data);
    This->OnTerminate();
}

void Application::app_pause(void *data)
{
    Application *This = static_cast<Application *>(data);
    This->OnPause();
}

void Application::app_resume(void *data)
{
    Application *This = static_cast<Application *>(data);
    This->OnResume();
}

void Application::app_control(app_control_h app_control, void *data)
{
    Application *This = static_cast<Application *>(data);

    // convert app_control to bundle
    bundle *b;
    app_control_to_bundle(app_control, &b);

    This->OnReset(b);
}

Application::Application(int argc, char** argv,
                         const std::string& applicationName,
                         bool showMainWindow) :
    m_argc(argc),
    m_argv(argv),
    m_applicationName(applicationName),
    m_mainWindowVisible(showMainWindow)
{
    if (g_application != NULL) {
        ThrowMsg(Exception::TooManyInstances,
                 "Only single instance of Application allowed at one time!");
    }

    g_application = this;
}

Application::~Application()
{
    g_application = NULL;
}

int Application::Exec()
{
    LogPedantic("Starting application framework...");

    ui_app_lifecycle_callback_s callback;
    callback.create = app_create;
    callback.terminate = app_terminate;
    callback.pause = app_pause;
    callback.resume = app_resume;
    callback.app_control = app_control;

    int result = ui_app_main(m_argc, m_argv, &callback, this);

    LogPedantic("Exited application framework");

    return result;
}

void Application::OnCreate()
{
    LogPedantic("On application create");
}

void Application::OnStart()
{
    LogPedantic("On application start");
}

void Application::OnStop()
{
    LogPedantic("On application stop");
}

void Application::OnResume()
{
    LogPedantic("On application resume");
}

void Application::OnPause()
{
    LogPedantic("On application pause");
}

void Application::OnRelaunch()
{
    LogPedantic("On application relaunch");
}

void Application::OnReset(bundle *b)
{
    (void)b;
    LogPedantic("On application reset");
}

void Application::OnTerminate()
{
    LogPedantic("On application terminate");
}

void Application::OnLowMemory()
{
    LogPedantic("On application low memory");
}

void Application::OnLowBattery()
{
    LogPedantic("On application low battery");
}

void Application::OnLanguageChanged()
{
    LogPedantic("On application language changed");
}

void Application::Quit()
{
    elm_exit();
}

DPL::Atomic ApplicationExt::m_useCount(0);

ApplicationExt::ApplicationExt(int argc,
                               char** argv,
                               const std::string& applicationName,
                               bool showMainWindow) :
    Application(argc, argv, applicationName, showMainWindow)
{}

ApplicationExt::~ApplicationExt()
{}

int ApplicationExt::Exec()
{
    if (0 == m_useCount.CompareAndExchange(0, 1)) {
        return Application::Exec();
    } else {
        elm_run();
    }
    return 0;
}

void ApplicationExt::Quit()
{
    elm_exit();
}
} // namespace DPL
