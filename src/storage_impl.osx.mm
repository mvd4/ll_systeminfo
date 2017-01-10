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

#import <Cocoa/Cocoa.h>

#include <sys/mount.h>

#include <string>

#include <boost/filesystem/convenience.hpp>
#include <boost/optional.hpp>

#include <platform_utils/osx/scoped_cf_ref.h>
#include <base/environment.h>


namespace ll
{
namespace systeminfo
{
namespace storage
{
  storage_info get_storage_info( scan_network_storage )
  {
    storage_info info;
    
    struct statfs* buffer = nullptr;
    auto volumeCount = ::getmntinfo( &buffer, MNT_NOWAIT );
    for( auto v = 0; v < volumeCount; ++v )
    {
      const auto& stat = buffer[v];

      if( stat.f_type == 19 )   // devfs i.e. device file
        continue;               // => not a storage
    
      auto volume = storage_info::volume();
      volume.path = stat.f_mntonname;
      
      // path is the path of a folder
      NSString* path = [NSString stringWithCString:volume.path.c_str() encoding:[NSString defaultCStringEncoding]];
      NSURL* url = [NSURL fileURLWithPath:path];
      
      NSError* err = nil;
      NSString* volumeName = nil;
      NSNumber* isRemovable = nil;
      NSNumber* totalCapacity = nil;
      NSNumber* availableCapacity = nil;
    
      [url getResourceValue:&volumeName forKey:NSURLVolumeNameKey error:&err];
      [url getResourceValue:&isRemovable forKey:NSURLVolumeIsRemovableKey error:&err];
      [url getResourceValue:&totalCapacity forKey:NSURLVolumeTotalCapacityKey error:&err];
      [url getResourceValue:&availableCapacity forKey:NSURLVolumeAvailableCapacityKey error:&err];
      
      
      if( volumeName )
        volume.name = std::string([ volumeName UTF8String ] );
      if( totalCapacity )
        volume.totalSizeInBytes = [ totalCapacity unsignedLongLongValue ];
      if( availableCapacity )
        volume.availableSizeInBytes = [ availableCapacity unsignedLongLongValue ];
      
      switch ( stat.f_type )
      {
      case 17:
        volume.fileSystem = filesystem::hfs_plus;
        break;
      case 20:
        volume.fileSystem = filesystem::autofs;
        break;
      case 21:
        volume.fileSystem = filesystem::smb;
        break;
      case 23:
        volume.fileSystem = filesystem::ntfs;
        break;
      default:
        volume.fileSystem = filesystem::unknown;
      }
    
      switch( volume.fileSystem )
      {
      case filesystem::unknown:
        break;  // leave at type::unknown
      case filesystem::smb:
      case filesystem::afp:
      case filesystem::nfs:
      case filesystem::autofs:
        volume.type = storage_type::network;
        break;
      default:
        if ([isRemovable boolValue])
          volume.type = storage_type::removable;
        else
          volume.type = storage_type::harddisk;
        break;
      }
    
      //volume.totalSizeInBytes = static_cast< std::uint64_t >( stat.f_blocks ) * stat.f_bsize;
      //volume.availableSizeInBytes = static_cast< std::uint64_t >( stat.f_bavail ) * stat.f_bsize;
      info.volumes.push_back( volume );
    }
    
    
    /* auto keys = [NSArray arrayWithObjects:
      @[NSURLVolumeNameKey],
      @[NSURLVolumeLocalizedFormatDescriptionKey],
      @[NSURLVolumeIsRemovableKey],
      @[NSURLVolumeAvailableCapacityKey],
      @[NSURLVolumeTotalCapacityKey],
      @[NSURLVolumeUUIDStringKey],
      nil
    ];
    
    auto* urls = [[NSFileManager defaultManager] mountedVolumeURLsIncludingResourceValuesForKeys:keys options:0];
    for ( NSURL* url in urls )
    {
     
    */
    
    
    /*  FOR DISK INFO:
     
     io_iterator_t it;
     CFMutableDictionaryRef dict = IOServiceMatching( "IOAHCIBlockStorageDevice" );
     if( IOServiceGetMatchingServices(kIOMasterPortDefault, dict, &it) != 0 )
     return info;
     
     while ( auto nextMedia = IOIteratorNext(it) )
     {
     CFMutableDictionaryRef properties;
     auto kernResult = IORegistryEntryCreateCFProperties(nextMedia,
     &properties,
     kCFAllocatorDefault, 0);
     
     if (kernResult != KERN_SUCCESS)
     continue;
     
     => "device characteristics" entry contains information about physical harddisks
     
     IOObjectRelease(nextMedia);
     CFRelease(properties);
     }
     IOObjectRelease(it);
     */
    
    
    return info;
  }

} // namespace storage
} // namespace systeminfo
} // namespace ll
