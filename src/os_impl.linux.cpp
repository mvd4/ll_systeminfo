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

#include <platform_utils/linux/shell_utils.h>
#include <base/environment.h>

LL_WARNING_DISABLE_GCC( deprecated-declarations )
#include <boost/tokenizer.hpp>
#include <boost/algorithm/string.hpp>
LL_WARNING_ENABLE_GCC( deprecated-declarations )

#include <array>
#include <map>
#include <memory>

#include <sys/utsname.h>



namespace ll
{
namespace systeminfo
{
namespace os
{
  namespace
  {
    using string_map_t = std::map< std::string, std::string >;    
    using release_info_t = string_map_t;
    using locale_settings_t = string_map_t;
    

    const release_info_t& get_release_info()
    {
      static release_info_t s_releaseInfo;
      
      if( s_releaseInfo.empty() )
      {
        auto s = utils::execute_shell_command( "cat /etc/*-release" );
        boost::char_separator< char > separator{ "=\n" };
        boost::tokenizer< boost::char_separator<char> >  tokenizer{ s, separator };
        for ( auto it = tokenizer.begin(); it != tokenizer.end(); ++it )
        {
          auto key = *it;
          s_releaseInfo[ key ] = boost::replace_all_copy( *(++it), "\"", "" );
        }
      }
      
      return s_releaseInfo;
    }


    const locale_settings_t& get_locale_settings()
    {
      static locale_settings_t s_locale_settings;
      
      if( s_locale_settings.empty() )
      {
        auto s = utils::execute_shell_command( "locale" );
        boost::char_separator< char > separator{ "=\n" };
        boost::tokenizer< boost::char_separator<char> >  tokenizer{ s, separator };
        for ( auto it = tokenizer.begin(); it != tokenizer.end(); ++it )
        {
          auto key = *it;
          s_locale_settings[ key ] = boost::replace_all_copy( *(++it), "\"", "" );
        }
      }
      
      return s_locale_settings;
    }


    string_map_t::const_iterator find_one_of( 
      const string_map_t& map_, 
      std::initializer_list< std::string > keys_ )
    {
      for( const auto& k : keys_ )
      {
        auto it = map_.find( k );
        if( it != map_.end() )
          return it;
      }
      return map_.end(); 
    }
  }


  family get_family()  { return family::linux; }
  
	  
  // ---------------------------------------------------------------------------------------------------------

  std::string get_name()
	{
    const auto& info = get_release_info();
    auto it = find_one_of( info, { "NAME", "DISTRIB_ID" } );
    if( it != info.end() )
      return it->second;
    return "";
	}
  
	  
  // ---------------------------------------------------------------------------------------------------------

  os::architecture get_architecture() 
  { 
    auto s = boost::trim_copy( utils::execute_shell_command( "uname -i" ) );
    if( s == "x86_64" )
      return architecture::x64; 
    else if( ( s.find( "i386" ) != std::string::npos ) ||( s.find( "i686" ) != std::string::npos ) )
      return architecture::x86;
    else
      return architecture::unknown;
  }

  
  // ---------------------------------------------------------------------------------------------------------

  version_t get_version()
  {
    std::string version;
    const auto& info = get_release_info();
    auto it = find_one_of( info, { "VERSION","VERSION_ID", "DISTRIB_RELEASE" } );
    if( it!= info.end() )
      version = it->second;
    else
    {
      return std::make_tuple(
        static_cast< unsigned >( 0 ),
        static_cast< unsigned >( 0 ),
        static_cast< unsigned >( 0 ),
        std::string()
      );
    }

    std::array< unsigned, 3 > parts = { 0 };
    size_t count = 0;
    boost::char_separator< char > separator{ ". " };
    boost::tokenizer< boost::char_separator<char> >  tokenizer{ version, separator };
    for ( auto it = tokenizer.begin(); it != tokenizer.end(); ++it )
    {
      try
      {
        parts[ count++  ] = static_cast< unsigned >( std::stoi( *it ) );
        if( count == 3 ) 
          break;
      }
      catch( std::invalid_argument& )
      {
        break;
      }
    }
   
   
    return std::make_tuple( parts[ 0 ], parts[ 1 ], parts[ 2 ], std::string() );
  }
  
  
  // ---------------------------------------------------------------------------------------------------------

  language get_language()
  {  
    language ret;

    auto localeSettings = get_locale_settings();
    auto it = find_one_of( localeSettings, { "LANG", "LANGUAGE" } );  // LANG is the default 
    if( it != localeSettings.end() && ( it->second.length() >= 5 ) )
    {
      ret.code = ll::to_language_code( it->second.substr( 0, 2 ) );
      ret.country = ll::to_country_code( it->second.substr( 3, 2 ) );
    }

    
    return ret;
  }


  // ---------------------------------------------------------------------------------------------------------

  fs::path get_standard_location( standard_location l_, usermode m_ )
  {
    if( m_ == usermode::shared )
      return fs::path();    // the concept of public folders doesn't exist on linux

    std::string command = "xdg-user-dir ";

    switch( l_ )
    {
      case standard_location::home:
        break;
      case standard_location::temp:
        command += "TEMP";
        break;      
      case standard_location::download:
        command += "DOWNLOAD";
        break;      
      case standard_location::desktop:
        command += "DESKTOP";
        break;
      case standard_location::applications:
      case standard_location::applications32:
        return "/usr/bin";
      case standard_location::app_data:
        return "/usr/share";
      case standard_location::documents:
        command += "DOCUMENTS";
        break;
      case standard_location::music:
        command += "MUSIC";
        break;      
      case standard_location::pictures:
        command += "PICTURES";
        break;      
      case standard_location::videos:
        command += "VIDEOS";
        break;      
      default:
        return fs::path();
    }

    fs::path result = boost::erase_all_copy( utils::execute_shell_command( command ), "\n" );
    return result;
  }


} // namespace os
} // namespace systeminfo
} // namespace ll
