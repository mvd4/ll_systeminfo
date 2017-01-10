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

#include "systeminfo/version.h"
#include "systeminfo/os.h"
#include "systeminfo/platform.h"
#include "systeminfo/storage.h"

#include <vector>
#include <map>
#include <iostream>
#include <numeric>


using namespace ll::systeminfo;

// -----------------------------------------------------------------------------------------------------------

void output_platform_info()
{
  using namespace ll::systeminfo::platform;

  std::cout << "Platform:\n---------\n\n";

  std::cout << "Device Manufacturer: " << get_device_manufacturer() << "\n";
  std::cout << "Device Name: " << get_device_name() << "\n";
  std::cout << "Device Type: " << ll::to_string( get_device_type() ) << "\n";
  std::cout << "\n";
  
  auto cpus = get_cpu_info();
  std::cout << "Number of processor packs: " << cpus.processorPacks.size() << "\n";
  std::cout << "Total number of physical cores: " << get_physical_core_count( cpus ) << "\n";
  std::cout << "Total number of logical cores: " << get_logical_core_count( cpus ) << "\n";
  for( const auto& p : cpus.processorPacks )
  {
    auto logicalCoreCount = std::accumulate( 
      p.physicalCores.begin(), 
      p.physicalCores.end(), 
      size_t{ 0 },
      []( size_t val_, const cpu_info::physical_core& c_ ) -> size_t 
      {  
        return val_ + c_.logicalCores.size();
      }
    );
    std::cout << "Package: \n";
    std::cout << "  Name: " << p.name << "\n";
    std::cout << "  Number of physical cores: " << p.physicalCores.size() << "\n";
    std::cout << "  Number of logical cores: " << logicalCoreCount << "\n";
    
    std::vector< cpu_info::logical_core > cpuSpeed;
    for( const auto& c : p.physicalCores )
    {
      for( const auto& l : c.logicalCores )
        cpuSpeed.push_back( l );
    }
    
    std::cout << "  Maximum core speed: ";
    for( const auto& s : cpuSpeed )
      std::cout << s.maximumSpeedInMHz << "MHz\t";
    std::cout << "\n";

    std::cout << "  Current core speed: ";
    for( const auto& s : cpuSpeed )
      std::cout << s.currentSpeedInMHz << "MHz\t";
    std::cout << "\n\n";
  }

  auto memory = get_memory_info();
  std::cout << "Total physical memory in bytes: " << memory.totalPhysicalMemoryInBytes << "\n";
  std::cout << "Available physical memory in bytes: " << memory.availablePhysicalMemoryInBytes << "\n";
  std::cout << "Total virtual memory in bytes: " << memory.totalVirtualMemoryInBytes << "\n";
  std::cout << "Available virtual memory in bytes: " << memory.availableVirtualMemoryInBytes << "\n";
  std::cout << "\n";
  
  
  auto gpus = get_gpu_info();
  std::cout << "Number of GPUs: " << gpus.gpus.size() << "\n";
  for ( const auto& g : gpus.gpus )
  {
    std::cout << "GPU: \n";
    std::cout << "  Name: " << g.name << "\n";
    std::cout << "  Video memory in bytes: " << g.videoMemoryInBytes << "\n";
  }
  
  std::cout << "\n\n";
}


// -----------------------------------------------------------------------------------------------------------

void output_os_info()
{
  using namespace ll::systeminfo::os;

  std::cout << "Operating System:\n-----------------\n\n";

  std::cout << "OS Family Name: " << ll::to_string( get_family() ) << "\n";
  std::cout << "OS Name: " << get_name() << "\n";
  std::cout << "OS Version: " << ll::to_string( get_version() ) << "\n";
  std::cout << "OS Architecture: " << ll::to_string( get_architecture() ) << "\n";
  std::cout << "OS Language: " << ll::to_string( get_language() ) << ", " 
            << ll::to_ietf_code( get_language() ) << "\n";

  std::cout << "\n";

  static std::vector< std::pair< std::string, standard_location > > locations = {
    { "Home", standard_location::home },
    { "Temp", standard_location::temp },
    { "Download", standard_location::download },
    { "Desktop", standard_location::desktop },
    { "Applications", standard_location::applications },
    { "Applications (32bit)", standard_location::applications32 },
    { "AppData", standard_location::app_data },
    { "Documents", standard_location::documents },
    { "Music", standard_location::music },
    { "Pictures", standard_location::pictures },
    { "Videos", standard_location::videos },
  };

  for ( const auto l : locations )
  {
    auto p = get_standard_location( l.second, usermode::current_user );
    std::cout << l.first << " directory (User):  \t" << p.string() << "\n";
    p = get_standard_location( l.second, usermode::shared );
    std::cout << l.first << " directory (Shared):\t" << p.string() << "\n";
  }

  std::cout << "\n\n";
}


// -----------------------------------------------------------------------------------------------------------

void output_storage_info()
{
  using namespace ll::systeminfo::storage;

  std::cout << "Storage:\n--------\n\n";

  static std::map< filesystem, std::string > filesystems = {
    { filesystem::unknown, "Unknown" },
    { filesystem::ntfs, "NTFS" },
    { filesystem::fat32, "FAT32" },
    { filesystem::hfs_plus, "MacOS Extended (HFS Plus)" },
    { filesystem::autofs, "Unknown (AutoFS)" },
    { filesystem::smb, "Samba" },
    { filesystem::ext4, "ext4" },
  };

  std::cout << "Volumes:\n";
  auto info = get_storage_info();
  for( const auto&  v : info.volumes )
  {
    std::cout << "  Volume name: " << v.name << "\n";
    std::cout << "  Volume root path: " << v.path.string() << "\n";
    std::cout << "  Volume type: " << ll::to_string( v.type ) << "\n";
    std::cout << "  Filesystem: " << filesystems[ v.fileSystem ] << "\n";
    std::cout << "  Total Size: " << v.totalSizeInBytes << " Bytes\n";
    std::cout << "  Available Size: " << v.availableSizeInBytes << " Bytes\n";
    std::cout << "\n";
  }

  std::cout << "\n\n";
}


// -----------------------------------------------------------------------------------------------------------

int main()
{
  std::cout << "Limelight Framework - SystemInfo v" << ll::systeminfo::libraryVersionMajor << "."
            << ll::systeminfo::libraryVersionMinor << "." << ll::systeminfo::libraryVersionMicro << " Demo\n\n";
    
  output_platform_info();
  output_os_info();
  output_storage_info();

  return 0;
}
