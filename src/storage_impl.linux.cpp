/*************************************************************************************************************

 Limelight Framework - SystemInfo Utils


 Copyright 2016 mvd

 Licensed under the Apache License: Version 2.0 (the "License"); you may not use this file except in
 compliance with the License. You may obtain a copy of the License at

  http://www.apache.org/licenses/LICENSE-2.0

 Unless required by applicable law or agreed to in writing, software distributed under the License is
 distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 See the License for the specific language governing permissions and limitations under the License.

*************************************************************************************************************/

#include "systeminfo/storage.h"

#include <platform_utils/linux/shell_utils.h>
#include <base/environment.h>

LL_WARNING_DISABLE_GCC( deprecated-declarations )
#include <boost/filesystem/convenience.hpp>
#include <boost/algorithm/string.hpp>
LL_WARNING_ENABLE_GCC( deprecated-declarations )

#include <string>
#include <iostream>

#include <sys/statvfs.h>
#include <mntent.h>



namespace ll
{
namespace systeminfo
{
namespace storage
{
  namespace
  {
    std::string mnt_dir_to_volume_name( const char* dir_ )
    {
      if( !dir_ ) 
        return std::string();
      
      std::string dir( dir_ );
      auto index = dir.rfind( "/" );
      if( index != std::string::npos )
        return dir.substr( ++index );

      return std::string();
    }


    storage_type fsname_to_volume_type( const char* fsname_ )
    {
     if( !fsname_ ) 
        return storage_type::unknown;
      
      std::string fsname( fsname_ );
      if( ( fsname.find( "sr" ) == 5 ) || ( fsname.find( "scd" ) == 5 ) )
        return storage_type::removable;

      if( fsname.find( "sd" ) == 5 )
        return storage_type::harddisk;

      return storage_type::unknown;
    }


    filesystem mnt_type_to_filesystem( const char* type_ )
    {
      if( !type_ ) 
        return filesystem::unknown;
      
      std::string type( type_ );
      if( type == "ext4" )
        return filesystem::ext4;

      return filesystem::unknown;
    }
  }


  storage_info get_storage_info( scan_network_storage scan_ )
  {
    storage_info info;

    FILE* mtab = setmntent( "/etc/mtab", "r" );
    if( !mtab )
      return info;

    struct mntent mnt;
    char strings[4096];
    while( getmntent_r( mtab, &mnt, strings, sizeof(strings ) ) )
    {
      if( mnt.mnt_dir == NULL ) 
        continue;
 
      std::string fsname = mnt.mnt_fsname;
      if( fsname.find( "/dev/" ) != 0 )
        continue;

      struct statvfs64 fs;
      if( statvfs64( mnt.mnt_dir, &fs ) != 0 )
        continue;

      storage_info::volume volume;
      volume.path = mnt.mnt_dir;
      volume.name = mnt_dir_to_volume_name( mnt.mnt_dir );
      volume.type = fsname_to_volume_type( mnt.mnt_fsname );
      volume.fileSystem = mnt_type_to_filesystem( mnt.mnt_type );

      volume.totalSizeInBytes = fs.f_blocks * fs.f_frsize;
      volume.availableSizeInBytes = fs.f_bavail * fs.f_frsize;
      
      info.volumes.push_back( volume );
    }
    return info;
  }
  
  
} // namespace storage
} // namespace systeminfo
} // namespace ll
