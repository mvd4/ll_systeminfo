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

#include <Cocoa/Cocoa.h>
#include <sys/sysctl.h>
#include <sys/param.h>
#include <sys/mount.h>
#include <mach/mach.h>
#include <mach/vm_statistics.h>

#include <array>
#include <vector>

#include <boost/algorithm/string.hpp>
#include <boost/optional.hpp>

#include <platform_utils/osx/scoped_cf_ref.h>

#include "systeminfo/exception.h"


namespace ll
{
namespace systeminfo
{
namespace platform
{
  namespace
  {
    template<typename property_t>
    inline CFTypeID get_cf_type_id() { return 0; }
  
    template<>
    inline CFTypeID get_cf_type_id< CFStringRef >() { return 7; }
  
    template<>
    inline CFTypeID get_cf_type_id< CFDataRef >() { return 20; }
  
    template<>
    inline CFTypeID get_cf_type_id< CFNumberRef >() { return 22; }
  
  
    template<typename property_t>
    property_t get_property( io_object_t service_, CFStringRef name_ )
    {
      auto cfref = IORegistryEntryCreateCFProperty( service_, name_, kCFAllocatorDefault, 0 );
      if( cfref && ( CFGetTypeID( cfref ) == get_cf_type_id< property_t >() ) )
        return static_cast< property_t >( cfref );
    
      return NULL;
    }
    
    
    
    boost::optional< io_service_t > get_io_service_by_property( const io_iterator_t& it_, CFStringRef property_, CFStringRef name_ )
    {
      while ( auto service = IOIteratorNext( it_ ) )
      {
        utils::scoped_cf_ref< CFStringRef > ioName = static_cast< CFStringRef >(
          IORegistryEntryCreateCFProperty( service, property_, kCFAllocatorDefault, 0 )
        );
        if ( ioName && CFStringCompare(ioName, name_, 0) == kCFCompareEqualTo )
          return service;
      }
      
      return boost::none;
    }
    
    cpu_info::logical_core& get_logical_core_by_index( cpu_info& info_, unsigned index_ )
    {
      assert( index_ < get_logical_core_count( info_ ) );
      
      auto logicalCoresPerPack = get_logical_core_count( info_ ) / info_.processorPacks.size();
      auto pack = index_ / logicalCoresPerPack;
      index_ %= logicalCoresPerPack;
      
      auto logicalCoresPerPhysicalCore = get_logical_core_count( info_ ) / get_physical_core_count( info_ );
      auto core = index_ / logicalCoresPerPhysicalCore;
      index_ %= logicalCoresPerPhysicalCore;
      
      return info_.processorPacks[pack].physicalCores[core].logicalCores[index_];
    }
  }
  
  
  namespace impl
  {
    std::string get_device_manufacturer()
    {
      return "Apple Inc.";
    }

  
    // -------------------------------------------------------------------------------------------------------

    std::string get_device_name()
    {
      std::array< int, 2 > sysctlParams = { {(CTL_HW), (HW_MODEL)} };
      size_t szBuffer = 0;
      if( sysctl( sysctlParams.data(), sysctlParams.size(), nullptr, &szBuffer, nullptr, 0 ) != 0 )
        throw exception( error::internal );
    
      std::vector< char > buffer( szBuffer );
      if( sysctl( sysctlParams.data(), sysctlParams.size(), buffer.data(), &szBuffer, nullptr, 0 ) != 0 )
        throw exception( error::internal );
    
      return std::string( buffer.begin(), buffer.end() );
    }

  
    // -------------------------------------------------------------------------------------------------------

    device_type get_device_type()
    {
      // simple heuristic: if there's a 'Book' in the string, it's some kind of MacBook  => laptop
      auto name = get_device_name();
      return ( name.find( "Book" ) != std::string::npos ) ? device_type::laptop : device_type::desktop;
    }


    // -------------------------------------------------------------------------------------------------------

    std::string get_processor_name()
    {
      size_t szBuffer = 0;
      if( sysctlbyname( "machdep.cpu.brand_string", nullptr, &szBuffer, nullptr, 0 ) != 0 )
        throw exception( error::internal );
    
      std::vector< char > buffer( szBuffer );
      if( sysctlbyname( "machdep.cpu.brand_string", buffer.data(), &szBuffer, nullptr, 0 ) != 0 )
        throw exception( error::internal );
    
      auto name = std::string( buffer.begin(), buffer.end() );
      return boost::trim_copy( name );
    }

    
    // -------------------------------------------------------------------------------------------------------

    cpu_info get_static_cpu_info()
    {
      cpu_info info;
    
      int packageCount = 0;
      int physicalCoreCount = 0;
      int logicalCoreCount = 0;
      std::uint64_t currentCPUFreq = 0;
      
      size_t sz = sizeof( packageCount );
      sysctlbyname( "hw.packages", &packageCount, &sz, nullptr, 0 );
      sz = sizeof( physicalCoreCount );
      sysctlbyname( "hw.physicalcpu", &physicalCoreCount, &sz, nullptr, 0 );
      sz = sizeof( logicalCoreCount );
      sysctlbyname( "hw.logicalcpu", &logicalCoreCount, &sz, nullptr, 0 );
      sz = sizeof( currentCPUFreq );
      sysctlbyname( "hw.cpufrequency", &currentCPUFreq, &sz, nullptr, 0 );
      currentCPUFreq /= 1000000;  // transform Hz to MHz
      
      info.processorPacks.resize( static_cast< size_t >( packageCount ) );
    
      auto logicalCoresPerPhysicalCore = logicalCoreCount / physicalCoreCount;
      for( auto& p : info.processorPacks )
      {
        p.physicalCores.resize( static_cast< size_t >(  physicalCoreCount / info.processorPacks.size() ) );
        for( auto& c : p.physicalCores )
        {
          c.logicalCores.resize( logicalCoresPerPhysicalCore );
          for( auto& l : c.logicalCores )
            l.maximumSpeedInMHz = currentCPUFreq;
        }
      }
    
      return info;
    }

    
    // -------------------------------------------------------------------------------------------------------

    void get_dynamic_cpu_info( cpu_info& info_ )
    {
      // Get a list of all platform devices
      io_iterator_t it;
      CFMutableDictionaryRef dict = IOServiceMatching( "IOPlatformDevice" );  // reference is consumed by next call
      if( IOServiceGetMatchingServices( kIOMasterPortDefault, dict, &it) != KERN_SUCCESS )
        return;

      while ( auto service = IOIteratorNext( it ) )
      {
        utils::scoped_cf_ref< CFDataRef > data = get_property< CFDataRef >( service, CFSTR( "device_type" ) );
        if( !data )
          continue;
        const UInt8* rawdata = CFDataGetBytePtr(data);
        auto deviceType = std::string( reinterpret_cast< const char* >( rawdata ) );
        if( ( deviceType != "processor" ) && ( deviceType != "cpu" ) )
          continue;
        
        utils::scoped_cf_ref< CFNumberRef > processorNumber = get_property< CFNumberRef >( service, CFSTR( "processor-number" ) );
        if( !processorNumber )
          continue;
        unsigned id;
        CFNumberGetValue( processorNumber, kCFNumberSInt32Type, &id );
        
        utils::scoped_cf_ref< CFDataRef > freq = get_property< CFDataRef >( service, CFSTR("clock-frequency") );
        if( !freq )
          continue;
        
        auto length = CFDataGetLength( freq );
        auto buf = std::vector< unsigned char >( static_cast< size_t >(length ) );
        CFDataGetBytes( freq, CFRangeMake(0, length), buf.data() );
        auto frequency = *( reinterpret_cast< std::int64_t* >( buf.data() ) );
        
        get_logical_core_by_index( info_, id ).currentSpeedInMHz = frequency / 1000 / 1000;
      }
    }
  
  
    // -------------------------------------------------------------------------------------------------------

    gpu_info get_gpu_info()
    {
      gpu_info info;
    
      // Get a list of all PCI/PCIe devices
      io_iterator_t it;
      CFMutableDictionaryRef dict = IOServiceMatching( "IOPCIDevice" );  // reference is consumed by next call
      if( IOServiceGetMatchingServices(kIOMasterPortDefault, dict, &it) != 0 )
        return info;
    
      // Find GPUs in that list, store their names
      while ( auto service = get_io_service_by_property( it, CFSTR( "IOName" ), CFSTR("display") ) )
      {
        utils::scoped_cf_ref< CFDataRef > model = get_property< CFDataRef >( service.value(), CFSTR("model") );
        if ( !model )
          continue;
      
        auto length = CFDataGetLength( model );
      
        gpu_info::gpu gpu;
        gpu.name.resize( static_cast< size_t >( length + 1 ) );
      
        auto data = const_cast< char* >( gpu.name.data() );
        CFDataGetBytes( model, CFRangeMake( 0, length ), reinterpret_cast< unsigned char* >( data ) );
        data[ static_cast< size_t >( length ) ] = 0;
      
        // storage location of memory size depends on gpu
        {
          utils::scoped_cf_ref< CFNumberRef > memsize = get_property< CFNumberRef >( service.value(), CFSTR( "ATY,memsize" ) );
          if( memsize )
            CFNumberGetValue( memsize, kCFNumberSInt64Type, &gpu.videoMemoryInBytes );
        }
      
        if( gpu.videoMemoryInBytes == 0 )
        {
          utils::scoped_cf_ref< CFDataRef > memsize = get_property< CFDataRef >( service.value(), CFSTR("VRAM,totalsize") );
          if( memsize )
          {
            auto length = CFDataGetLength( memsize );
            auto buf = std::vector< unsigned char >( static_cast< size_t >(length ) );
            CFDataGetBytes( memsize, CFRangeMake(0, length), buf.data() );
            gpu.videoMemoryInBytes = *( reinterpret_cast< std::int64_t* >( buf.data() ) );
          }
        }      
      
        info.gpus.push_back( gpu );
      }

      return info;
    }
  
  }  // namespace impl
  
  
  memory_info get_memory_info()
  {
    // see http://stackoverflow.com/questions/63166/how-to-determine-cpu-and-memory-consumption-from-inside-a-process
    // for details
  
    //! \todo error handling for sysctlbyname => do we want to throw exceptions here or fail silently
  
    memory_info info;

    size_t sz = sizeof( info.totalPhysicalMemoryInBytes );
    sysctlbyname( "hw.memsize", &info.totalPhysicalMemoryInBytes, &sz, nullptr, 0 );

    int pagesize = 0;
    sz = sizeof( pagesize );
    sysctlbyname( "hw.pagesize", &pagesize, &sz, nullptr, 0 );

    xsw_usage swapusage = {0};
    sz = sizeof( swapusage );
    sysctlbyname( "vm.swapusage", &swapusage, &sz, nullptr, 0 );
  
  
    mach_msg_type_number_t count = HOST_VM_INFO_COUNT;
    vm_statistics_data_t vmstat;
    if(KERN_SUCCESS != host_statistics64(mach_host_self(), HOST_VM_INFO, (host_info64_t)&vmstat, &count))
      throw exception( error::internal );
  
    /*std::uint64_t wired = vmstat.wire_count;
    //std::uint64_t speculative = vmstat.speculative_count;  // from the vm_statistics_data header: this is part of the
                                                           // free count
    std::uint64_t active = vmstat.active_count;
    std::uint64_t inactive = vmstat.inactive_count;
    std::uint64_t free = vmstat.free_count;
    std::uint64_t purgeable = vmstat.purgeable_count;
  */
    info.availablePhysicalMemoryInBytes = static_cast< std::uint64_t >( vmstat.free_count ) * pagesize;
  
    // get the free space on the root partition => that's the maximum available swap space
    struct statfs stats;
    if ( 0 == statfs("/", &stats ) )
    {
      auto freeSpaceOnRoot = static_cast< uint64_t >( stats.f_bsize ) * stats.f_bfree;
      info.totalVirtualMemoryInBytes = freeSpaceOnRoot + info.totalPhysicalMemoryInBytes;
      info.availableVirtualMemoryInBytes = info.availablePhysicalMemoryInBytes + freeSpaceOnRoot - swapusage.xsu_used;
    }
  
    return info;
  }
  
} // namespace platform
} // namespace systeminfo
} // namespace ll
