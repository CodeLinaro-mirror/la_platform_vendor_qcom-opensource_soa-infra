/************************************************************
* Copyright (c) 2024 Qualcomm Innovation Center, Inc. All rights reserved.
* SPDX-License-Identifier: BSD-3-Clause-Clear
*************************************************************/
#include <vsomeip_daemon_main.hpp>

int main()
{
      while (true)
      {
         std::this_thread::sleep_for(std::chrono::milliseconds(1000));
      }
      return 0;
}
