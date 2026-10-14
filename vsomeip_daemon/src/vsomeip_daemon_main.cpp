/************************************************************
* Copyright (c) 2024-2025 Qualcomm Innovation Center, Inc. All rights reserved.
* SPDX-License-Identifier: BSD-3-Clause-Clear
*************************************************************/
#include <vsomeip_daemon_main.hpp>
#include <csignal>

namespace {
std::shared_ptr<vsomeip::application> g_daemon_app;

// Runs on a dedicated thread with SIGTERM/SIGINT blocked everywhere else, so
// sigwait() delivers the signal here synchronously instead of via an
// async-signal-unsafe handler. Calling stop() lets application::start()
// return normally so vsomeip's own graceful teardown (withdraw offers, close
// routing endpoints) runs instead of the process dying on default signal
// disposition with no notice to connected clients.
void wait_for_shutdown_signal()
{
    sigset_t signal_set;
    sigemptyset(&signal_set);
    sigaddset(&signal_set, SIGTERM);
    sigaddset(&signal_set, SIGINT);

    int received_signal = 0;
    sigwait(&signal_set, &received_signal);

    std::cerr << " [vsomeip_daemon] received signal " << received_signal
              << ", stopping gracefully" << std::endl;
    if (g_daemon_app) {
        g_daemon_app->stop();
    }
}
}

int vsomeip_daemon_start(std::string app_name)
{
      std::shared_ptr<vsomeip::runtime> daemon_runtime= vsomeip::runtime::get();

      if (!daemon_runtime) {
        std::cerr <<  " [vsomeip_daemon] " << app_name << " daemon_runtime error " << std::endl;
        return -1;
      }
      g_daemon_app = daemon_runtime->create_application(app_name);
      if (g_daemon_app->init())
      {
        if (g_daemon_app->is_routing()) {
            std::cerr <<  " [vsomeip_daemon] " << app_name << " start " << std::endl;
            std::thread signal_thread(wait_for_shutdown_signal);
            g_daemon_app->start();
            signal_thread.join();
        }
        else {
            std::cerr <<  " [vsomeip_daemon] " << app_name << " is_routing error " << std::endl;
        }
      }
      else {
            std::cerr <<  " [vsomeip_daemon] " << app_name << " init error " << std::endl;
      }
      return 0;
}

int main(int argc, char* argv[])
{
      std::cerr <<  " [vsomeip_daemon] Entry  arg count " << argc << std::endl;

      // Block before creating the app so every thread vsomeip spawns
      // inherits the mask; only wait_for_shutdown_signal's sigwait() will
      // ever observe SIGTERM/SIGINT.
      sigset_t signal_set;
      sigemptyset(&signal_set);
      sigaddset(&signal_set, SIGTERM);
      sigaddset(&signal_set, SIGINT);
      pthread_sigmask(SIG_BLOCK, &signal_set, nullptr);

      std::string nw_name("default");
      //ToDo
      //Add necessary input Validation
      //Add logging framework
      //Add state handlng
      //Add Multi routing manager
      int c;
      while ((c = getopt(argc, argv, "n:")) != -1) {
        switch (c) {
            case 'n':
                nw_name = strdup(optarg);;
                break;
            case '?':
                nw_name = "default";
                break;
            default:
                nw_name = "default";
                break;
        }
    }
    std::string int_name = "vsomeip_daemon_"+nw_name;
    vsomeip_daemon_start(int_name);
    std::cerr <<  " [vsomeip_daemon] Exit " << std::endl;
    return 0;
}
