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

#include <boost/algorithm/string.hpp>

#import <Cocoa/Cocoa.h>


namespace ll
{
namespace systeminfo
{
namespace os
{
  namespace
  {
      std::pair< NSSearchPathDirectory, int > to_osx_directory_info( standard_location l_, usermode m_ )
      {
        std::pair< NSSearchPathDirectory, int > result;
          
        result.second = ( m_ == usermode::current_user ) ? NSUserDomainMask : NSLocalDomainMask;
          
        switch ( l_ )
        {
          case standard_location::home:
          case standard_location::temp:
            break;
            
          case standard_location::download:
            result.first = NSDownloadsDirectory;
            break;
            
          case standard_location::desktop:
            result.first = NSDesktopDirectory;
            break;
            
          case standard_location::applications:
          case standard_location::applications32:
            result.first = NSApplicationDirectory;
            break;
            
          case standard_location::app_data:
            result.first = NSApplicationSupportDirectory;
            break;
            
          case standard_location::documents:
            result.first = NSDocumentDirectory;
            break;
            
          case standard_location::music:
            result.first = NSMusicDirectory;
            break;
            
          case standard_location::pictures:
            result.first = NSPicturesDirectory;
            break;
            
          case standard_location::videos:
            result.first = NSMoviesDirectory;
            break;
            
          default:
            throw std::invalid_argument( "unsupported" );
        }
        
        return result;
      }
  }
  
  
  family get_family()  { return family::osx; }
  
  
  // ---------------------------------------------------------------------------------------------------------

  std::string get_name()
	{
		switch( std::get<1>( get_version() ) )
		{
		  case 0:
	  		return "Cheetah";
		  case 1:
  			return "Puma";
		  case 2:
			  return "Jaguar";
		  case 3:
			  return "Panther";
		  case 4:
		  	return "Tiger";
		  case 5:
	  		return "Leopard";
		  case 6:
  			return "Snow Leopard";
		  case 7:
			  return "Lion";
		  case 8:
			  return "Mountain Lion";
		  case 9:
		  	return "Mavericks";
		  case 10:
	  		return "Yosemite";
		  case 11:
  			return "El Capitan";
		  case 12:
			  return "Sierra";
		  default:
			  return "Unknown";
		}
	}
  
	  
  // ---------------------------------------------------------------------------------------------------------

  os::architecture get_architecture() { return architecture::x64; }  // supported osx versions are 64 bit only
  
  
  // ---------------------------------------------------------------------------------------------------------

  version_t get_version()
  {
#if 1 //! \todo for MacOS SDK >= 10.10
    NSOperatingSystemVersion osVersion = [[NSProcessInfo processInfo] operatingSystemVersion ];
    return std::make_tuple(
      static_cast< unsigned >( osVersion.majorVersion ),
      static_cast< unsigned >( osVersion.minorVersion ),
      static_cast< unsigned >( osVersion.patchVersion ),
      std::string()
    );
  
#else 
// ignore deprecation warnings
#pragma GCC diagnostic push 
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"  

    SInt32 major = 0;
    ::Gestalt( gestaltSystemVersionMajor, &major );
    SInt32 minor = 0;
    ::Gestalt(gestaltSystemVersionMinor, &minor);
    SInt32 micro = 0;
    ::Gestalt(gestaltSystemVersionBugFix, &micro);
     
    return std::make_tuple(
      static_cast< unsigned >( major ),
      static_cast< unsigned >( minor ),
      static_cast< unsigned >( micro ),
      std::string()
    );
#pragma GCC diagnostic pop    
#endif
  }
  
  
  // ---------------------------------------------------------------------------------------------------------

  language get_language()
  {
    language ret;

    auto locale = std::string( [ [[NSLocale preferredLanguages] objectAtIndex:0] UTF8String ] );
    if ( locale.length() >= 2 )
        ret.code = ll::to_language_code( locale.substr( 0, 2 ) );
    if ( locale.length() >= 5 )
      ret.country = ll::to_country_code( locale.substr( 3, 2 ) );
    return ret;
  }
    

  // ---------------------------------------------------------------------------------------------------------

  fs::path get_standard_location( standard_location l_, usermode m_ )
  {
    auto info = to_osx_directory_info( l_, m_ );
  
    NSFileManager* fileManager = [NSFileManager defaultManager];
    NSURL* url = [fileManager URLForDirectory:info.first inDomain:info.second appropriateForURL:nil
                  create:NO error:nil];
  
    if( url )
    {
      std::string str( [[url absoluteString] UTF8String ] );
      str = str.substr( 7 );                  // remove file:// from url
      boost::replace_all( str, "%20", " " );  // replace url-encoded spaces \todo use true url-decode
      return fs::path( str );
    }
    else if( l_ == standard_location::home )
    {
      return ( m_ == usermode::current_user ) ? fs::path( ::getenv("HOME") ) : "/Users/Shared";
    }
  
    return fs::path();
  }

} // namespace os
} // namespace systeminfo
} // namespace ll
