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

#define CATCH_CONFIG_RUNNER
#include <catch.hpp>

#include <systeminfo/version.h>

#include <iostream>


int main( int argc, char* argv[] )
{
  std::cout << "Limelight Framework - Crypto Utils v" << ll::systeminfo::libraryVersionMajor << "."
    << ll::systeminfo::libraryVersionMinor << "." << ll::systeminfo::libraryVersionMicro << " Test Suite\n\n";

  int result = Catch::Session().run( argc, argv );

  return result;
}