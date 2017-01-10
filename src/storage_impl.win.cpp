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

#include "systeminfo/storage.h"

#include "systeminfo/exception.h"

#include <base/environment.h>

#include <string>
#include <array>
#include <algorithm>

#include <Shlobj.h>


namespace ll
{
namespace systeminfo
{
namespace storage
{
  // ---------------------------------------------------------------------------------------------------------

  storage_info get_storage_info( scan_network_storage scan_ )
  {
    auto buffersize = ::GetLogicalDriveStringsW( 0, nullptr );
    if( buffersize == 0 )
      throw exception( error::internal );

    std::vector< wchar_t > buffer( static_cast< size_t >( buffersize ) );
    if( !SUCCEEDED( ::GetLogicalDriveStringsW( buffersize, buffer.data() ) ) )
      throw exception( error::internal );

    storage_info info;
    
    auto it = buffer.begin();
    auto last = buffer.begin();
    while( ( *it != '\0' ) && ( last != buffer.end() ) )
    {
      last = std::find( it, buffer.end(), '\0' );
      
      info.volumes.push_back( storage_info::volume() );
      info.volumes.back().path = std::wstring( it, last );
      it = ++last;      
    }

    for( auto& v : info.volumes )
    {
      switch( ::GetDriveTypeW( v.path.c_str() ) )
      {
        case DRIVE_RAMDISK:
        case DRIVE_FIXED:
          v.type = storage_type::harddisk;
          break;
        case DRIVE_CDROM:
        case DRIVE_REMOVABLE:
          v.type = storage_type::removable;
          break;
        case DRIVE_REMOTE:
          v.type = storage_type::network;
          if( scan_ == scan_network_storage::exclude )
            continue;
          break;
        default:
          v.type = storage_type::unknown;
          continue;
      }
            
      std::array< wchar_t, 10 > fsBuffer = {};
      std::array< wchar_t, 128 > labelBuffer = {};
      if( SUCCEEDED( ::GetVolumeInformationW( 
        v.path.wstring().c_str(), 
        labelBuffer.data(), 
        sizeof( labelBuffer ) * sizeof( wchar_t ),
        nullptr, 
        nullptr, 
        nullptr, 
        fsBuffer.data(), 
        sizeof( fsBuffer ) * sizeof( wchar_t )
      ) ) )
      {
        std::wstring temp = labelBuffer.data();     //! \todo we need a proper utf8 conversion here
        v.name.assign( temp.begin(), temp.end() );

        std::wstring fsname = fsBuffer.data();
        if( fsname == L"NTFS" )
          v.fileSystem = filesystem::ntfs;
        else if( fsname == L"FAT32" )
          v.fileSystem = filesystem::fat32;
        else
          continue;
      }

      ULARGE_INTEGER bytesTotal = ULARGE_INTEGER();
      ULARGE_INTEGER bytesAvailable = ULARGE_INTEGER();
      if ( SUCCEEDED( 
        ::GetDiskFreeSpaceEx( v.path.string().c_str(), &bytesAvailable, &bytesTotal, nullptr ) 
      ) )
      {
        v.totalSizeInBytes = static_cast< std::uint64_t >( bytesTotal.QuadPart );  
        v.availableSizeInBytes = static_cast< std::uint64_t >( bytesAvailable.QuadPart );  
      }
    }
    return info;
  }


  

} // namespace storage
} // namespace systeminfo
} // namespace ll
