Limelight System-Info
=====================

Welcome to the Limelight System Info module!

This library provides functionality to retrieve system information such as memory sizes, processor capabilities and operating-system information in a portable way. 

Features
--------

* operating-system information, such as
    * name
    * version
    * language setting
    * standard file locations
* hardware information, such as
    * processor information (package count, physical cores, logical cores, speed)
    * memory sizes
    * device type (laptop, desktop)
* information on available storage devices
    * volume names
    * volume sizes
    * filesystem
* modern C++11 code
    
    
Usage example
-------------

The following code ...

```c++
#include <systeminfo/os.h>
#include <iostream>

int main()
{
  using namespace ll::systeminfo;

  std::cout << "Operating System:\n-----------------\n\n";

  std::cout << "OS Family Name: " << ll::to_string( os::get_family() ) << "\n";
  std::cout << "OS Name: " << os::get_name() << "\n";
  std::cout << "OS Version: " << ll::to_string( os::get_version() ) << "\n";
  std::cout << "OS Architecture: " << ll::to_string( os::get_architecture() ) << "\n";
  std::cout << "OS Language: " << ll::to_string( os::get_language() ) << ", " 
            << ll::to_ietf_code( os::get_language() ) << "\n";

  return 0;
}
```

... will output something like the following on a windows machine (*have a look at the notes below for some windows specifics regarding the operating system version*):

```
Operating System:
-----------------

OS Family Name: Windows
OS Name: Windows 10
OS Version: 10.0.0
OS Architecture: x64
OS Language: German (Germany), de-DE
```

License
-------

The systeminfo module is licensed under the Apache License, Version 2.0 ( http://www.apache.org/licenses/LICENSE-2.0 )


Supported platforms
-------------------
* Windows >= 7
* OSX >= 10.8
* any Linux with OpenSSL

Dependencies
------------
The systeminfo module is currently dependent on the following 3rd party libraries:

* boost (filesystem, system) >= 1.56

Additionally you might require the following additional software

* CMake >= 2.8 (for creating the build-projects)
* Catch >= 1.56 (for building the tests) 
* python3 (for running the convenience build scripts)

*Older versions of these libraries might work but were not tested against*


Getting started
---------------

*This is the quick run-through. Please refer to the build_support manual for a detailed documentation on the convenience build-scripts, or on how to configure the systeminfo module manually*

Limelight uses a CMake based build system, enhanced by a set of custom convenience build-scripts.
The quickest way to get the systeminfo module up and running is to use these scripts. You'll need a working python3 installation with pip for that.

### 1. clone the repository
```bash
git clone --recursive https://mvd4@bitbucket.org/limelightframework/systeminfo.git
cd systeminfo
```

### 2. create and activate a virtual environment
```bash
python3 -m venv env
source env/bin/activate  # or the corresponding env/Scripts/activate.bat on Windows
```

### 3. run the build scripts

```bash
python build_ll.py init       # this will download and install the required python packages
python build_ll.py bootstrap  # note that this will download all dependencies by default, pls check the available options to control the bootstrap behaviour
python build_ll.py configure
python build_ll.py build -w 
``` 
You now should have a subfolder 'build' in your checkout directory and within that the project- / makefiles (depending on your platform) to build the systeminfo module.

To use systeminfo in your project, you have to add the path *< systeminfo checkout directory >/include* to the list of include paths in your project, and to link against the library module which will be in a subfolder of *< systeminfo checkout directory >/lib* after building the module.

Notes
-----

On Windows, in order to retrieve the correct operating system version, you need to add a manifest
to your application identifying it as 'windows 8' or 'windows 10-enabled'. Have a look at the 
manifest file in the systeminfo_demo example that is included in the repository.
