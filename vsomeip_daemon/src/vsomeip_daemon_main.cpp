/************************************************************
* Copyright (c) 2024 Qualcomm Innovation Center, Inc. All rights reserved.
* SPDX-License-Identifier: BSD-3-Clause-Clear
*************************************************************/
#include <vsomeip_daemon_main.hpp>

int vsomeip_daemon_start(std::string app_name)
{ 
      static std::shared_ptr<vsomeip::application> daemon_app;
      std::shared_ptr<vsomeip::runtime> daemon_runtime= vsomeip::runtime::get();

      if (!daemon_runtime) {
        std::cerr <<  " [vsomeip_daemon] " << app_name << " daemon_runtime error " << std::endl;  
        return -1;
      }
      daemon_app = daemon_runtime->create_application(app_name);
      if (daemon_app->init()) 
      { 
        if (daemon_app->is_routing()) {
            std::cerr <<  " [vsomeip_daemon] " << app_name << " start " << std::endl;  
            daemon_app->start();
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
      //ToDo
      //Add necessary input Validation
      //Add logging framework
      //Add signal handling 
      //Add state handlng
      //Add Multi routing manager 
      
      std::string int_name = "vsomeip_daemon_vlan1500";
      
      vsomeip_daemon_start(int_name);

      std::cerr <<  " [vsomeip_daemon] Exit " << std::endl;
      
      return 0;
}
