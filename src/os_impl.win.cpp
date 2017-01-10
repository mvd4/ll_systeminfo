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

#include "systeminfo/os.h"

#include <base/environment.h>

#include <array>
#include <map>
#include <vector>

#include <Windows.h>
#include <VersionHelpers.h>
#include <KnownFolders.h>
#include <Shlobj.h>

#undef IN

namespace ll
{
namespace systeminfo
{
namespace os
{
  namespace
  {
    KNOWNFOLDERID to_windows_folder_id( standard_location l_, usermode m_ )
    {
      switch ( l_ )
      {
      case standard_location::home:
        return ( m_ == usermode::current_user ) ? FOLDERID_Profile : FOLDERID_Public;
      case standard_location::temp:
        return ( m_ == usermode::current_user ) ? FOLDERID_LocalAppData : FOLDERID_RoamingAppData;
      case standard_location::download:
        return ( m_ == usermode::current_user ) ? FOLDERID_Downloads : FOLDERID_PublicDownloads;
      case standard_location::desktop:
        return ( m_ == usermode::current_user ) ? FOLDERID_Desktop : FOLDERID_PublicDesktop;
      case standard_location::applications:
        return FOLDERID_ProgramFiles;
      case standard_location::applications32:
        return FOLDERID_ProgramFilesX86;
      case standard_location::app_data:
        return ( m_ == usermode::current_user ) ? FOLDERID_LocalAppData : FOLDERID_RoamingAppData;
      case standard_location::documents:
        return ( m_ == usermode::current_user ) ? FOLDERID_Documents : FOLDERID_PublicDocuments;
      case standard_location::music:
        return ( m_ == usermode::current_user ) ? FOLDERID_Music : FOLDERID_PublicMusic;
      case standard_location::pictures:
        return ( m_ == usermode::current_user ) ? FOLDERID_Pictures : FOLDERID_PublicPictures;
      case standard_location::videos:
        return ( m_ == usermode::current_user ) ? FOLDERID_Videos : FOLDERID_PublicVideos;
      default:
        throw std::invalid_argument( "invalid location and mode" );
      }
    }
  }
  
  
  // -------------------------------------------------------------------------------------------------------

  family get_family() { return family::windows; }
  
    
  // -------------------------------------------------------------------------------------------------------

  architecture get_architecture()
  {
#if LL_ARCHITECTURE == LL_X64
    return architecture::x64;   // 64 bit binary only runs on 64bit windows
#else
  BOOL wow64Process = FALSE;
  ::IsWow64Process( GetCurrentProcess(), &wow64Process );
  return wow64Process ? architecture::x64 : architecture::x86;
#endif
  }

  
  // -------------------------------------------------------------------------------------------------------

  std::string get_name()
  {
    auto v =  get_version();
    auto isServer = static_cast< bool >( ::IsWindowsServer() );

    if( std::get< 0 >( v ) == 10 )
      return "Windows 10";

    switch( std::get< 1 >( v ) )
    {
      case 0:
        return isServer ? "Windows Server 2008" : "Windows Vista";
      case 1:
        return isServer ? "Windows Server 2008 R2" : "Windows 7";
      case 2:
        return isServer ? "Windows Server 2012" :  "Windows 8";
      case 3:
        return isServer ? "Windows Server 2012 R2" : "Windows 8.1";
      case 4:
        return "Windows 10";
      default:
        return "Unknown";
    }
  }

    
  // -------------------------------------------------------------------------------------------------------

  version_t get_version()
  {
    WORD major = 6;  // IsWindowsVersionOrGreater doesn't support anything below Vista anyway
    WORD minor = 0;
    WORD micro = 0;

    while( ::IsWindowsVersionOrGreater( ++major, 0, 0 ) );
    --major;

    while( ::IsWindowsVersionOrGreater( major, ++minor, 0 ) );
    --minor;

    while( ::IsWindowsVersionOrGreater( major, minor, ++micro ) );
    --micro;

    return std::make_tuple(
      static_cast< unsigned >( major ),
      static_cast< unsigned >( minor ),
      static_cast< unsigned >( micro ),
      std::string()
    );
  }


  // -------------------------------------------------------------------------------------------------------

  language get_language()
  {
    static std::array< 
      std::pair< language::language_code, std::vector< language::country_code > >, 
      0x093u      
    > windows_to_iso = { {
      { language::language_code::unknown, { language::country_code::unknown } },   // 0x00
      { language::language_code::ar, { language::country_code::SA, 
                                       language::country_code::IQ, 
                                       language::country_code::EG, 
                                       language::country_code::LY, 
                                       language::country_code::DZ, 
                                       language::country_code::MA, 
                                       language::country_code::TN, 
                                       language::country_code::OM, 
                                       language::country_code::YE,
                                       language::country_code::SY,
                                       language::country_code::JO,
                                       language::country_code::LB, 
                                       language::country_code::KW,
                                       language::country_code::AE,
                                       language::country_code::BH,
                                       language::country_code::QA } },
      { language::language_code::bg, { language::country_code::BG } },
      { language::language_code::ca, { language::country_code::CA } },
      { language::language_code::zh, { language::country_code::unknown } },  // traditional, i.e. would have to be hant
      { language::language_code::cs, { language::country_code::CZ } },
      { language::language_code::da, { language::country_code::DK } },
      { language::language_code::de, { language::country_code::DE,
                                       language::country_code::CH,
                                       language::country_code::AT,
                                       language::country_code::LU,      
                                       language::country_code::LI } },
      { language::language_code::el, { language::country_code::GR } },
      { language::language_code::en, { language::country_code::US,
                                       language::country_code::GB,
                                       language::country_code::AU,
                                       language::country_code::CA,
                                       language::country_code::NZ,
                                       language::country_code::IE,
                                       language::country_code::ZA, 
                                       language::country_code::JM,
                                       language::country_code::unknown,
                                       language::country_code::BZ,
                                       language::country_code::TT,
                                       language::country_code::ZW,
                                       language::country_code::PH,
                                       language::country_code::unknown,
                                       language::country_code::unknown,
                                       language::country_code::IN,
                                       language::country_code::MY,
                                       language::country_code::SG } },
      { language::language_code::es, { language::country_code::ES,
                                       language::country_code::MX,
                                       language::country_code::ES,
                                       language::country_code::GT,
                                       language::country_code::CR,
                                       language::country_code::PA,
                                       language::country_code::DO,
                                       language::country_code::VE,
                                       language::country_code::CO,
                                       language::country_code::PE,
                                       language::country_code::AR,
                                       language::country_code::EC,
                                       language::country_code::CL,
                                       language::country_code::UY,
                                       language::country_code::PY,
                                       language::country_code::BO,
                                       language::country_code::SV,
                                       language::country_code::HN,
                                       language::country_code::NI,
                                       language::country_code::PR } },
      { language::language_code::fi, { language::country_code::FI } },   
      { language::language_code::fr, { language::country_code::FR,
                                       language::country_code::BE,
                                       language::country_code::CA,
                                       language::country_code::CH,
                                       language::country_code::LU,
                                       language::country_code::MC } },
      { language::language_code::he, { language::country_code::IL } },
      { language::language_code::hu, { language::country_code::HU } },
      { language::language_code::is, { language::country_code::IS } },
      { language::language_code::it, { language::country_code::IT } },        // 0x10
      { language::language_code::ja, { language::country_code::JP } },
      { language::language_code::ko, { language::country_code::KR } },
      { language::language_code::nl, { language::country_code::NL } },
      { language::language_code::no, { language::country_code::NO } },
      { language::language_code::pl, { language::country_code::PL } },
      { language::language_code::pt, { language::country_code::PT } },
      { language::language_code::unknown, { language::country_code::unknown } },
      { language::language_code::ro, { language::country_code::RO } },
      { language::language_code::ru, { language::country_code::RU } },
      { language::language_code::hr, { language::country_code::HR } },
      { language::language_code::sk, { language::country_code::SK } },
      { language::language_code::sq, { language::country_code::AL } },
      { language::language_code::sv, { language::country_code::SV } },
      { language::language_code::unknown, { language::country_code::unknown } },
      { language::language_code::tr, { language::country_code::TR } },
      { language::language_code::ur, { language::country_code::PK } },   // 0x20
      { language::language_code::id, { language::country_code::ID } },
      { language::language_code::uk, { language::country_code::UA } },
      { language::language_code::be, { language::country_code::BE } },
      { language::language_code::sl, { language::country_code::SL } },
      { language::language_code::et, { language::country_code::ET } },
      { language::language_code::lv, { language::country_code::LV } },
      { language::language_code::lt, { language::country_code::LT } },
      { language::language_code::tg, { language::country_code::TG } },
      { language::language_code::fa, { language::country_code::IR } },
      { language::language_code::vi, { language::country_code::VI } },
      { language::language_code::hy, { language::country_code::AM } },
      { language::language_code::az, { language::country_code::AZ, language::country_code::AZ } },
      { language::language_code::unknown, { language::country_code::unknown } },
      { language::language_code::unknown, { language::country_code::unknown } },
      { language::language_code::mk, { language::country_code::MK } },
      { language::language_code::unknown, { language::country_code::unknown } },   // 0x30
      { language::language_code::unknown, { language::country_code::unknown } },
      { language::language_code::tn, { language::country_code::TN } },
      { language::language_code::unknown, { language::country_code::unknown } },
      { language::language_code::xh, { language::country_code::ZA } },
      { language::language_code::zu, { language::country_code::ZA } },
      { language::language_code::af, { language::country_code::AF } },
      { language::language_code::ka, { language::country_code::GE } },
      { language::language_code::fo, { language::country_code::FO } },
      { language::language_code::hi, { language::country_code::IN } },
      { language::language_code::mt, { language::country_code::MT } },
      { language::language_code::unknown, { language::country_code::unknown } },
      { language::language_code::ga, { language::country_code::GA } },
      { language::language_code::unknown, { language::country_code::unknown } },
      { language::language_code::ms, { language::country_code::MS } },
      { language::language_code::kk, { language::country_code::KZ } },
      { language::language_code::ky, { language::country_code::KY } },   // 0x40
      { language::language_code::sw, { language::country_code::KE } },
      { language::language_code::tk, { language::country_code::TK } },
      { language::language_code::uz, { language::country_code::UZ } },
      { language::language_code::tt, { language::country_code::TT } },
      { language::language_code::bn, { language::country_code::IN, 
                                       language::country_code::BD } },
      { language::language_code::pa, { language::country_code::PA } },
      { language::language_code::gu, { language::country_code::GU } },
      { language::language_code::or_, { language::country_code::IN } },
      { language::language_code::ta, { language::country_code::IN } },
      { language::language_code::te, { language::country_code::IN } },
      { language::language_code::kn, { language::country_code::KN } },
      { language::language_code::ml, { language::country_code::ML } },
      { language::language_code::as, { language::country_code::AS } },
      { language::language_code::mr, { language::country_code::MR } },
      { language::language_code::sa, { language::country_code::SA } },
      { language::language_code::mn, { language::country_code::MN } },   // 0x50
      { language::language_code::bo, { language::country_code::BO } },
      { language::language_code::cy, { language::country_code::CY } },
      { language::language_code::km, { language::country_code::KM } },
      { language::language_code::lo, { language::country_code::LA } },
      { language::language_code::unknown, { language::country_code::unknown } },
      { language::language_code::gl, { language::country_code::GL } },
      { language::language_code::unknown, { language::country_code::unknown } },
      { language::language_code::unknown, { language::country_code::unknown } },
      { language::language_code::unknown, { language::country_code::unknown } },
      { language::language_code::unknown, { language::country_code::unknown } },
      { language::language_code::si, { language::country_code::SI } },
      { language::language_code::unknown, { language::country_code::unknown } },
      { language::language_code::iu, { language::country_code::CA } },
      { language::language_code::am, { language::country_code::AM } },
      { language::language_code::unknown, { language::country_code::unknown } },
      { language::language_code::unknown, { language::country_code::unknown } },   // 0x60
      { language::language_code::ne, { language::country_code::NE } },
      { language::language_code::fy, { language::country_code::NL } },
      { language::language_code::ps, { language::country_code::PS } },
      { language::language_code::unknown, { language::country_code::unknown } },
      { language::language_code::dv, { language::country_code::MV } },
      { language::language_code::unknown, { language::country_code::unknown } },
      { language::language_code::ff, { language::country_code::SN } },
      { language::language_code::ha, { language::country_code::NG } },
      { language::language_code::unknown, { language::country_code::unknown } },
      { language::language_code::yo, { language::country_code::NG } },
      { language::language_code::qu, { language::country_code::BO } },
      { language::language_code::unknown, { language::country_code::unknown } },
      { language::language_code::ba, { language::country_code::BA } },
      { language::language_code::lb, { language::country_code::LB } },
      { language::language_code::kl, { language::country_code::GL } },
      { language::language_code::ig, { language::country_code::NG } },   // 0x70
      { language::language_code::unknown, { language::country_code::unknown } },
      { language::language_code::unknown, { language::country_code::unknown } },
      { language::language_code::ti, { language::country_code::ET } },
      { language::language_code::unknown, { language::country_code::unknown } },
      { language::language_code::unknown, { language::country_code::unknown } },
      { language::language_code::unknown, { language::country_code::unknown } },
      { language::language_code::unknown, { language::country_code::unknown } },
      { language::language_code::ii, { language::country_code::CN } },
      { language::language_code::unknown, { language::country_code::unknown } },
      { language::language_code::unknown, { language::country_code::unknown } },
      { language::language_code::unknown, { language::country_code::unknown } },
      { language::language_code::unknown, { language::country_code::unknown } },
      { language::language_code::unknown, { language::country_code::unknown } },
      { language::language_code::br, { language::country_code::BR } },
      { language::language_code::unknown, { language::country_code::unknown } },
      { language::language_code::ug, { language::country_code::UG } },   // 0x80
      { language::language_code::mi, { language::country_code::NZ } },
      { language::language_code::oc, { language::country_code::FR } },
      { language::language_code::co, { language::country_code::CO } },
      { language::language_code::unknown, { language::country_code::unknown } },
      { language::language_code::unknown, { language::country_code::unknown } },
      { language::language_code::unknown, { language::country_code::unknown } },
      { language::language_code::rw, { language::country_code::RW } },
      { language::language_code::wo, { language::country_code::SN } },
      { language::language_code::unknown, { language::country_code::unknown } },
      { language::language_code::unknown, { language::country_code::unknown } },
      { language::language_code::unknown, { language::country_code::unknown } },
      { language::language_code::unknown, { language::country_code::unknown } },
      { language::language_code::unknown, { language::country_code::unknown } },
      { language::language_code::unknown, { language::country_code::unknown } },
      { language::language_code::unknown, { language::country_code::unknown } },
      { language::language_code::unknown, { language::country_code::unknown } },   // 0x90
      { language::language_code::unknown, { language::country_code::unknown } },
      { language::language_code::ku, { language::country_code::IQ } }
    } };

    
    language l;

    auto id = ::GetUserDefaultUILanguage();
    if( id == 0 )
      return l;

    auto language = id & 0x1ff;
    if( language <= static_cast<size_t>( os::language::language_code::other ) )
    {
      l.code = windows_to_iso[language].first;

      auto sub = ( id & 0xfc00 ) >> 10;
      if ( sub > 0 && sub <= windows_to_iso[language].second.size() )
        l.country = windows_to_iso[language].second[sub - 1];
    }

    return l;
  }


  // ---------------------------------------------------------------------------------------------------------

  fs::path get_standard_location( standard_location l_, usermode m_ )
  {
    PWSTR path = nullptr;
    fs::path p;

#if LL_ARCHITECTURE == LL_X86
    // getting the x64 program files dir is not supported for 32bit apps
    if ( l_ == standard_location::applications )
    {
      size_t sz = 0;
      ::_wdupenv_s( &path, &sz, L"ProgramW6432" );
      if ( !path )
        throw std::runtime_error( "Internal error" );

      p = path;
      free( path );
    }
    else
#endif

    {
      HRESULT result = SHGetKnownFolderPath( to_windows_folder_id( l_, m_ ), 0, 0, &path );
      if ( FAILED( result ) || !path )
        throw std::runtime_error( "Internal error" );
      p = path;
      ::CoTaskMemFree( static_cast<void*>( path ) );  // ok when assuming that we only allocated on success
    }

    if ( l_ == standard_location::temp )
      p /= "Temp";

    return p;
  }

} // namespace os
} // namespace systeminfo
} // namespace ll
