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

#include "platform_impl.h"

#include "systeminfo/exception.h"

#include <base/environment.h>
#include <platform_utils/linux/shell_utils.h>

LL_WARNING_DISABLE_GCC( deprecated-declarations )
#include <boost/algorithm/string.hpp>
#include <boost/tokenizer.hpp>
LL_WARNING_ENABLE_GCC( deprecated-declarations )


#include <array>
#include <vector>
#include <map>
#include <iostream>


namespace ll
{
namespace systeminfo
{
namespace platform
{
  namespace
  {
    using cpu_info_t = std::map< std::string, std::string >;
    using cpu_speed_t = std::vector< unsigned >;
    using gpu_info_t = std::vector< std::string >;
    using memory_info_t = std::map< std::string, std::string >;
    
    const cpu_info_t& get_static_processor_info()
    {
      static cpu_info_t s_processorInfo;
      
      if( s_processorInfo.empty() )
      {
        auto s = utils::execute_shell_command( "export LANG=C;lscpu" );
        boost::char_separator< char > separator{ ":\n" };
        boost::tokenizer< boost::char_separator<char> >  tokenizer{ s, separator };
        for ( auto it = tokenizer.begin(); it != tokenizer.end(); ++it )
        {
          auto key = *it;
          s_processorInfo[ key ] = boost::replace_all_copy( *(++it), "\"", "" );
        }
      }
      
      return s_processorInfo;
    }

   
    cpu_speed_t get_dynamic_processor_info()
    {
      cpu_speed_t speed;

      auto s = utils::execute_shell_command( "export LANG=C;cat /proc/cpuinfo | grep \"cpu MHz\"" );
      boost::char_separator< char > separator{ ":\n" };
      boost::tokenizer< boost::char_separator<char> >  tokenizer{ s, separator };
      for ( auto it = tokenizer.begin(); it != tokenizer.end(); ++it )
        speed.push_back( std::stoi( *(++it) ) );
      
      return speed;
    }


    const gpu_info_t& get_raw_gpu_info()
    {
      static gpu_info_t s_gpuInfo;
      
      if( s_gpuInfo.empty() )
      {
        auto s = utils::execute_shell_command( "export LANG=C;lspci -d ::0300 -v" );
        boost::char_separator< char > separator{ "\n" };
        boost::tokenizer< boost::char_separator<char> >  tokenizer{ s, separator };
        for ( auto it = tokenizer.begin(); it != tokenizer.end(); ++it )
        {
          s_gpuInfo.push_back( *(it++) );
        }
      }
      
      return s_gpuInfo;
    }


    memory_info_t get_raw_memory_info()
    {
      memory_info_t info;  // not static, because the 'available' information changes
      
      //! \todo this is pretty inefficient when called multiple times

      auto s = utils::execute_shell_command( "cat /proc/meminfo" );
      boost::char_separator< char > separator{ ":\n" };
      boost::tokenizer< boost::char_separator<char> >  tokenizer{ s, separator };
      for ( auto it = tokenizer.begin(); it != tokenizer.end(); ++it )
      {
        auto key = *it;
        info[ key ] = boost::replace_all_copy( *(++it), "\"", "" );
      }
      
      return info;
    }

  }


  namespace impl
  {
    std::string get_device_manufacturer()
    {
      return boost::trim_copy( utils::execute_shell_command( "cat /sys/devices/virtual/dmi/id/sys_vendor" ) ); 
    }

  
    // -------------------------------------------------------------------------------------------------------

    std::string get_device_name()
    {
      return boost::trim_copy( 
        utils::execute_shell_command( "cat /sys/devices/virtual/dmi/id/product_name" ) 
      );
    }

  
    // -------------------------------------------------------------------------------------------------------

    device_type get_device_type()
    {
      auto typeID = boost::trim_copy( 
        utils::execute_shell_command( "cat /sys/devices/virtual/dmi/id/chassis_type" ) 
      );

      switch( std::stoi( typeID ) )
      {
        case 10:
          return device_type::laptop;
        default:
          break; 
      }

      return device_type::unknown;
    }


    // -------------------------------------------------------------------------------------------------------

    std::string get_processor_name()
    {
      auto it = get_static_processor_info().find( "Model name" );
      return ( it == get_static_processor_info().end() ) ? "" : it->second;
    }

    
    // -------------------------------------------------------------------------------------------------------

    cpu_info get_static_cpu_info()
    {
      cpu_info info;
    
      auto cpuInfo = get_static_processor_info();  //! \todo this is a copy

      auto packageCount = std::stoi( boost::trim_copy( cpuInfo[ "Socket(s)" ] ) );      
      auto logicalCoreCount = std::stoi( boost::trim_copy( cpuInfo[ "CPU(s)" ] ) );
      auto coresPerSocket = std::stoi( boost::trim_copy( cpuInfo[ "Core(s) per socket" ] ) );
      auto cpuMaxMHz = std::stoi( boost::trim_copy( cpuInfo[ "CPU max MHz" ] ) );

      info.processorPacks.resize( static_cast< size_t >( packageCount ) );
      for( auto& p : info.processorPacks )
      {
        p.physicalCores.resize( static_cast< size_t >( coresPerSocket ) );
        for( auto& c : p.physicalCores )
        {
          c.logicalCores.resize( logicalCoreCount / packageCount / coresPerSocket );
          for( auto& l : c.logicalCores )
          {
            l.maximumSpeedInMHz = cpuMaxMHz;
          }
        }
      }
      return info;
    }

    
    // -------------------------------------------------------------------------------------------------------

    void get_dynamic_cpu_info( cpu_info& info_ )
    {
      auto speed = get_dynamic_processor_info();
      
      auto totalLogicalCoreCount = get_logical_core_count( info_ );
      auto coresPerPack = totalLogicalCoreCount / info_.processorPacks.size();
      auto logicalPerCore = totalLogicalCoreCount / get_physical_core_count( info_ );
      size_t index = 0;
      for( const auto& s : speed )
      {
        auto p = index / coresPerPack;
        auto c = ( index % coresPerPack ) / 2;
        auto l = index % logicalPerCore;
        info_.processorPacks[ p ].physicalCores[ c ].logicalCores[l].currentSpeedInMHz = s;
        ++index;
      }
    }
    

    // -------------------------------------------------------------------------------------------------------

    gpu_info get_gpu_info()
    {
      gpu_info info;

      auto rawInfo = get_raw_gpu_info();
      for( auto l : rawInfo )
      {
        if( l.find( "VGA compatible controller" ) == std::string::npos )
          continue;

        info.gpus.push_back( gpu_info::gpu() );
        info.gpus.back().name = l.substr( l.find( "controller:" ) + 12 );
      }
      return info;
    }


  }  // namespace impl
  
  
  memory_info get_memory_info()
  {
    // see http://stackoverflow.com/questions/63166/how-to-determine-cpu-and-memory-consumption-from-inside-a-process
    // for details
  
    memory_info info;
    auto rawInfo = get_raw_memory_info();

    info.totalPhysicalMemoryInBytes = std::stoll( boost::trim_copy( rawInfo[ "MemTotal" ] ) ) * 1024;
    info.availablePhysicalMemoryInBytes = std::stoll( boost::trim_copy( rawInfo[ "MemFree" ] ) ) * 1024;
    
    info.totalVirtualMemoryInBytes = std::stoll( boost::trim_copy( rawInfo[ "SwapTotal" ] ) ) * 1024;   
    info.totalVirtualMemoryInBytes += info.totalPhysicalMemoryInBytes;

    info.availableVirtualMemoryInBytes = std::stoll( boost::trim_copy( rawInfo[ "SwapFree" ] ) ) * 1024; 
    info.availableVirtualMemoryInBytes += info.availablePhysicalMemoryInBytes;

    return info;
  }
  
} // namespace platform
} // namespace systeminfo
} // namespace ll
