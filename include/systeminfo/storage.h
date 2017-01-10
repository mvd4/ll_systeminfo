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

#include <base/environment.h>

LL_WARNING_DISABLE_GCC( deprecated-declarations )
#include <boost/filesystem/path.hpp>
LL_WARNING_ENABLE_GCC( deprecated-declarations )

#include <cstdint>



namespace ll
{
namespace systeminfo
{
namespace storage
{  
  // ---------------------------------------------------------------------------------------------------------
  // Types
  // ---------------------------------------------------------------------------------------------------------

  namespace fs = boost::filesystem;
 

  enum class filesystem
  {
    unknown,
    ntfs,
    fat32,
    hfs_plus,
    autofs,
    smb,
    afp,
    nfs,
    ext4
  };

  enum class storage_type
  {
    unknown,
    harddisk,
    removable,
    network,      
  };

  enum class scan_network_storage
  {
    exclude,
    include
  };
      

  struct storage_info
  {
    struct volume
    {
      std::string name;
      fs::path path;
      storage_type type = storage_type::unknown;
      filesystem fileSystem = filesystem::unknown;

      std::uint64_t totalSizeInBytes = 0;
      std::uint64_t availableSizeInBytes = 0;

      
      //! \todo drive serial
    };

    std::vector< volume > volumes; 
  };


  // ---------------------------------------------------------------------------------------------------------
  // Functions
  // ---------------------------------------------------------------------------------------------------------

  storage_info get_storage_info( scan_network_storage scan_ = scan_network_storage::include );

  
}  // namespace storage
}  // namespace systeminfo
}  // namespace ll


// -----------------------------------------------------------------------------------------------------------
// Utilities
// -----------------------------------------------------------------------------------------------------------

namespace ll
{

  std::string to_string( ll::systeminfo::storage::storage_type t_ );
 
}  // namespace ll
