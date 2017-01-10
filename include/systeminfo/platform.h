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

#include <string>
#include <cstdint>
#include <vector>
#include <numeric>


namespace ll
{
namespace systeminfo
{
namespace platform
{ 
  // ---------------------------------------------------------------------------------------------------------
  // Types
  // ---------------------------------------------------------------------------------------------------------
  
  enum class device_type
  {
    unknown,
    desktop,
    laptop,
    mobile
  };

  
  struct cpu_info
  {
    struct logical_core
    {
      unsigned currentSpeedInMHz = 0;
      unsigned maximumSpeedInMHz = 0;
    };

    struct physical_core
    {
      std::vector< logical_core > logicalCores;      
    };

    struct processor_pack
    {
      std::string name;

      std::vector< physical_core > physicalCores;
    };

    std::vector< processor_pack > processorPacks;
  };



  struct memory_info
  {
    std::uint64_t totalPhysicalMemoryInBytes = 0;
    std::uint64_t availablePhysicalMemoryInBytes = 0;
    std::uint64_t totalVirtualMemoryInBytes = 0;
    std::uint64_t availableVirtualMemoryInBytes = 0;
  };



  struct gpu_info
  {
    struct gpu
    {
      std::string    name;
      std::uint64_t  videoMemoryInBytes = 0;
    };

    std::vector< gpu > gpus;
  };



  // ---------------------------------------------------------------------------------------------------------
  // Functions
  // ---------------------------------------------------------------------------------------------------------
  
  std::string get_device_manufacturer();
  
  std::string get_device_name();
  
  device_type get_device_type();

  cpu_info    get_cpu_info();

  memory_info get_memory_info();  //! \todo this is also static and dynamic information mixed
  
  gpu_info    get_gpu_info();

  
  size_t get_physical_core_count( const platform::cpu_info& cpu_ );

  size_t get_logical_core_count( const platform::cpu_info& cpu_ );

}  // namespace platform
}  // namespace systeminfo
}  // namespace ll



// -----------------------------------------------------------------------------------------------------------
// Utilities
// -----------------------------------------------------------------------------------------------------------

namespace ll
{

  std::string to_string( ll::systeminfo::platform::device_type t_ );

}  // namespace ll