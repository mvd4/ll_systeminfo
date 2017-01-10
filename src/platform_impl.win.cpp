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

#include <base/environment.h>
#include <base/debug_helpers.h>

#include <boost/algorithm/string.hpp>

#include <array>
#include <vector>

#include <Windows.h>
#include <PowrProf.h>
#include <Powerbase.h>
#include <dxgi.h>



namespace
{
  // from the ms docu:
  // Note that this structure definition was accidentally omitted from WinNT.h. This error will be corrected 
  // in the future. In the meantime, to compile your application, include the structure definition contained 
  // in this topic in your source code.
  typedef struct _PROCESSOR_POWER_INFORMATION {
    ULONG Number;
    ULONG MaxMhz;
    ULONG CurrentMhz;
    ULONG MhzLimit;
    ULONG MaxIdleState;
    ULONG CurrentIdleState;
  } PROCESSOR_POWER_INFORMATION, *PPROCESSOR_POWER_INFORMATION;


  // ---------------------------------------------------------------------------------------------------------

  std::string query_registry_string( HKEY& key_, const std::string& valueName_ )
  {
    DWORD count = 0;
    DWORD type = REG_SZ;

    if( FAILED( ::RegQueryValueEx( key_, valueName_.c_str(), NULL, &type, NULL, &count ) ) )
      return std::string();

    std::vector< unsigned char > buffer( static_cast< size_t >( count ) );
    if( FAILED( 
      ::RegQueryValueEx( key_, valueName_.c_str(), NULL, &type, static_cast< LPBYTE >( buffer.data() ), &count )
    ) )
      return std::string();
        
    return std::string( reinterpret_cast< char* >( buffer.data() ) ); 
  }

  // ---------------------------------------------------------------------------------------------------------

  std::string query_registry_value( const std::string& keyName_, const std::string& valueName_ )
  {
    std::string value;
    HKEY hKey;
    if(
      SUCCEEDED( ::RegOpenKeyEx(
      HKEY_LOCAL_MACHINE,
      keyName_.c_str(),
      0,
      KEY_READ | KEY_WOW64_64KEY,
      &hKey
      ) ||
      !hKey
      ) )
    {
      value = query_registry_string( hKey, valueName_ );
      ::RegCloseKey( hKey );
    }
    return value;
  }

}


namespace ll
{
namespace systeminfo
{
namespace platform
{
  namespace impl
  {
    namespace
    { 
      unsigned count_set_bits( std::uint64_t mask_ )
      {
        unsigned count = 0;
        size_t numBits = sizeof( mask_ ) * 8;
        for( size_t b = 0; b < numBits; ++b )
        {
          count += mask_ & 0x1u;
          mask_ >>= 1;
        }
        return count;
      }


      // -----------------------------------------------------------------------------------------------------
  
      std::vector< char > get_logical_cpu_info()
      {
        std::vector< char > buffer;

        typedef BOOL( WINAPI* LPFN_GLPI )( PSYSTEM_LOGICAL_PROCESSOR_INFORMATION, PDWORD );

LL_WARNING_DISABLE_MSVC( 4191 )
        LPFN_GLPI pGLPI = reinterpret_cast< LPFN_GLPI >( GetProcAddress( 
          GetModuleHandle( TEXT( "kernel32" ) ),
          "GetLogicalProcessorInformation" 
        ) );
LL_WARNING_ENABLE_MSVC( 4191 )

        if( pGLPI == nullptr )
        {
          LL_ASSERT( false, "GetLogicalProcessorInfo function not available" );
          return buffer;
        }

LL_WARNING_DISABLE_MSVC( 4127 )  
        while( true )
LL_WARNING_ENABLE_MSVC( 4191 )
        {
          DWORD bufferSize = static_cast<DWORD>( buffer.size() );
          DWORD res = pGLPI( reinterpret_cast<PSYSTEM_LOGICAL_PROCESSOR_INFORMATION>( 
            buffer.data() ), 
            &bufferSize 
          );
          if( res != FALSE )
            break;

          // first call will fail, but tell us the required buffer size
          if( GetLastError() != ERROR_INSUFFICIENT_BUFFER )
          {
            LL_ASSERT( false, "Call to GetLogicalProcessorInfo function failed" );
            return buffer;
          }
            
          buffer.resize( bufferSize );
        }
      
        return buffer;
      }

    }
    
      
    // -------------------------------------------------------------------------------------------------------

    std::string get_device_manufacturer()
    {
      return 
        query_registry_value( "SYSTEM\\CurrentControlSet\\Control\\SystemInformation", "SystemManufacturer" )
      ;      
    }

    
    // -------------------------------------------------------------------------------------------------------

    std::string get_device_name()
    {
      return
        query_registry_value( "SYSTEM\\CurrentControlSet\\Control\\SystemInformation", "SystemProductName" )
      ;      
    }


    // -------------------------------------------------------------------------------------------------------

    device_type get_device_type()
    {
      SYSTEM_POWER_CAPABILITIES powerCaps;
      if ( !GetPwrCapabilities( &powerCaps ) )
        return device_type::unknown;
    
      if ( powerCaps.LidPresent || powerCaps.SystemBatteriesPresent )
        return device_type::laptop;
    
      return device_type::desktop;    
    }

    
    // -----------------------------------------------------------------------------------------------------
  
    std::string get_processor_name()
    {
      static const unsigned maxFunctionID = 0x80000000;       // id of the highest available standard function
      static const unsigned requiredFunctionID = 0x80000004;  // id of the required extended function

      std::array< int, 4 > cpui;

      // make sure the required extended functions are available
      __cpuid( cpui.data(), maxFunctionID );
      if( cpui[0] < requiredFunctionID )
        return std::string();

      std::vector< char > buffer( 3 * sizeof( cpui ) );
      char* p = buffer.data();
      for( unsigned fn = maxFunctionID + 2; fn <= requiredFunctionID; ++fn )
      {
        __cpuidex( cpui.data(), fn, 0 );
        memcpy( p, cpui.data(), sizeof( cpui ) );
        p += sizeof( cpui );
      }

      return std::string( buffer.data() );
    }
    
    
    // -------------------------------------------------------------------------------------------------------

    cpu_info get_static_cpu_info()
    {
      cpu_info info;

      std::vector< char > buffer = get_logical_cpu_info();
      if( buffer.empty() )
        return info;
    

      unsigned logicalCoresPerPhysicalCore = 0;
      size_t byteOffset = 0;
      while( byteOffset < buffer.size() )
      {
        PSYSTEM_LOGICAL_PROCESSOR_INFORMATION pData = reinterpret_cast<PSYSTEM_LOGICAL_PROCESSOR_INFORMATION>( &buffer[byteOffset] );
        switch( pData->Relationship )
        {
          case RelationProcessorPackage:
            info.processorPacks.push_back( cpu_info::processor_pack() );
            info.processorPacks.back().physicalCores.resize( count_set_bits( pData->ProcessorMask ) ); 
            break;
          
          case RelationProcessorCore:
            logicalCoresPerPhysicalCore = count_set_bits( pData->ProcessorMask );
            break;
        
          default:
            break;
        }

        byteOffset += sizeof( SYSTEM_LOGICAL_PROCESSOR_INFORMATION );
      }

      if( logicalCoresPerPhysicalCore > 1 )
      {
        for( auto & p : info.processorPacks )
        {
          p.physicalCores.resize( p.physicalCores.size() / logicalCoresPerPhysicalCore );
          for( auto& c : p.physicalCores )
            c.logicalCores.resize( logicalCoresPerPhysicalCore );
        }
      }

      return info;
    }


    // -------------------------------------------------------------------------------------------------------

    void get_dynamic_cpu_info( cpu_info& info_ )
    {
      std::vector< PROCESSOR_POWER_INFORMATION > powerInfo;
      auto logicalCoreCount = get_logical_core_count( info_ );
      powerInfo.resize( logicalCoreCount );

      DWORD res = ::CallNtPowerInformation(
        ProcessorInformation,
        NULL,
        0,
        powerInfo.data(),
        static_cast< ULONG >( powerInfo.size() * sizeof( PROCESSOR_POWER_INFORMATION ) )
      );

      if( res != 0 )  // 0 == STATUS_SUCCESS
        return;

      auto coresPerPack = logicalCoreCount / info_.processorPacks.size();
      auto logicalCoresPerPhysicalCore = logicalCoreCount / get_physical_core_count( info_ );

      size_t index = 0;
      for( const auto& i : powerInfo )
      {
        auto p = index / coresPerPack;
        auto c = ( index % coresPerPack ) / logicalCoresPerPhysicalCore;
        for( auto& l : info_.processorPacks[ p ].physicalCores[ c ].logicalCores )
        {
          l.maximumSpeedInMHz = static_cast< unsigned >( i.MaxMhz );
          l.currentSpeedInMHz = static_cast< unsigned >( i.CurrentMhz );
        }
        ++index;
      }
    }


    // -------------------------------------------------------------------------------------------------------

    gpu_info get_gpu_info()
    {
      gpu_info info;

      IDXGIFactory * pFactory;
      if ( FAILED( CreateDXGIFactory( __uuidof( IDXGIFactory ), (void**)( &pFactory ) ) ) )
        return info;

      IDXGIAdapter* pAdapter = nullptr;
      for ( UINT i = 0; pFactory->EnumAdapters( i, &pAdapter ) != DXGI_ERROR_NOT_FOUND; ++i )
      {
        gpu_info::gpu gpu;

        DXGI_ADAPTER_DESC desc;
        pAdapter->GetDesc( &desc );

        std::wstring name( desc.Description );


        gpu.name.assign( name.begin(), name.end() );
        gpu.videoMemoryInBytes = desc.DedicatedVideoMemory;
        info.gpus.push_back( std::move( gpu ) );

        pAdapter->Release();
      }


      return info;
    }


  } // namespace impl

  
  // ---------------------------------------------------------------------------------------------------------

  memory_info get_memory_info()
  {
    memory_info info;

    {
      ULONGLONG sz = 0;
      if( ::GetPhysicallyInstalledSystemMemory( &sz ) == TRUE )                 // this returns the size in kb
        info.totalPhysicalMemoryInBytes = static_cast< std::uint64_t >( sz ) << 10;  
    }

    {
      MEMORYSTATUSEX memstatus;
      memstatus.dwLength = sizeof( memstatus );

      if( ::GlobalMemoryStatusEx ( &memstatus ) )
      {
        info.availablePhysicalMemoryInBytes = memstatus.ullAvailPhys;
        info.totalVirtualMemoryInBytes = memstatus.ullTotalPageFile;
        info.availableVirtualMemoryInBytes = memstatus.ullAvailPageFile;
      }
    }

    return info;
  }

} // namespace platform
} // namespace systeminfo
} // namespace ll
