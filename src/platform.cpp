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

#include "systeminfo/platform.h"
#include "platform_impl.h"

#include <base/environment.h>


namespace ll
{
namespace systeminfo
{
namespace platform
{
  namespace state
  {
    static std::string s_deviceManufacturer;
    static std::string s_deviceName;
    static device_type s_deviceType = device_type::unknown;
    
    static bool s_cpuInfoRetrieved = false;
    static cpu_info s_cpuInfo;
  
    static bool s_gpuInfoRetrieved = false;
    static gpu_info s_gpuInfo;
  }


  // ---------------------------------------------------------------------------------------------------------

  std::string get_device_manufacturer()
  {
    if( state::s_deviceManufacturer.empty() )
      state::s_deviceManufacturer = impl::get_device_manufacturer();

    return state::s_deviceManufacturer;
  }


  // ---------------------------------------------------------------------------------------------------------

  std::string get_device_name()
  {
    if( state::s_deviceName.empty() )
      state::s_deviceName = impl::get_device_name();

    return state::s_deviceName;
  }


  // ---------------------------------------------------------------------------------------------------------

  device_type get_device_type()
  {
    if( state::s_deviceType == device_type::unknown )
      state::s_deviceType = impl::get_device_type();

    return state::s_deviceType;
  }


  // ---------------------------------------------------------------------------------------------------------

  cpu_info get_cpu_info()
  {
    if( !state::s_cpuInfoRetrieved )
    {
      state::s_cpuInfo = impl::get_static_cpu_info();

      for( auto& p : state::s_cpuInfo.processorPacks )
        p.name = impl::get_processor_name();
        
      state::s_cpuInfoRetrieved = true;      
    }

    impl::get_dynamic_cpu_info( state::s_cpuInfo );
    return state::s_cpuInfo;
  }
  

  // ---------------------------------------------------------------------------------------------------------

  gpu_info get_gpu_info()
  {
    if( !state::s_gpuInfoRetrieved )
    {
      state::s_gpuInfo = impl::get_gpu_info();
      state::s_gpuInfoRetrieved = true;      
    }

    return state::s_gpuInfo;
  }


  // ---------------------------------------------------------------------------------------------------------

  size_t get_physical_core_count( const platform::cpu_info& cpu_ )
  {
    return std::accumulate(
      cpu_.processorPacks.begin(),
      cpu_.processorPacks.end(),
      size_t( 0 ),
      []( size_t count_, const platform::cpu_info::processor_pack p_ )
    {
      return count_ + p_.physicalCores.size();
    }
    );
  }


  // ---------------------------------------------------------------------------------------------------------

  size_t get_logical_core_count( const platform::cpu_info& cpu_ )
  {
    return std::accumulate(
      cpu_.processorPacks.begin(),
      cpu_.processorPacks.end(),
      size_t( 0 ),
      []( size_t count_, const platform::cpu_info::processor_pack p_ )
    {
      for ( const auto& c : p_.physicalCores )
        count_ += c.logicalCores.size();
      return count_;
    }
    );
  }

} // namespace platform
} // namespace systeminfo
} // namespace ll



namespace ll
{
  using namespace systeminfo;

  std::string to_string( platform::device_type t_ )
  {
    switch ( t_ )
    {
    case platform::device_type::desktop:
      return "Desktop";
    case platform::device_type::laptop:
      return "Laptop";
    case platform::device_type::mobile:
      return "Mobile";
    default:
      return "Unknown";
    }
  }

} // namespace ll

