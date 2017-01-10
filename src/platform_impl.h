/*************************************************************************************************************

 Limelight Framework - SystemInfo Utils


 Copyright 2016 mvd

 Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except in
 compliance with the License. You may obtain a copy of the License at

  http://www.apache.org/licenses/LICENSE-2.0

 Unless required by applicable law or agreed to in writing, software distributed under the License is
 distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 See the License for the specific language governing permissions and limitations under the License.

*************************************************************************************************************/

#pragma once

#include "systeminfo/platform.h"

#include <string>
#include <tuple>


namespace ll
{
namespace systeminfo
{
namespace platform
{  
  
  namespace impl
  {
    std::string get_device_manufacturer();
    
    std::string get_device_name();

    device_type get_device_type();

    std::string get_processor_name();

    cpu_info get_static_cpu_info();

    void get_dynamic_cpu_info( cpu_info& info_ );
  
    gpu_info get_gpu_info();

  }  // namespace impl

}  // namespace platform
}  // namespace systeminfo
}  // namespace ll
