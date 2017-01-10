cmake_minimum_required(VERSION 2.8)

set( LL_MODULE "systeminfo" )
project( "ll_${LL_MODULE}" )

# -------------------------------------------------------------------------------------------------
# CMake setup
# -------------------------------------------------------------------------------------------------

set( CMAKE_MODULE_PATH ${LL_PROJECT_ROOT}/support/cmake;${CMAKE_MODULE_PATH} )
set( CMAKE_MODULE_PATH ${LL_PROJECT_ROOT}/cfg;${CMAKE_MODULE_PATH} )

include( config )
include( build_settings )
include( utilities )


# -------------------------------------------------------------------------------------------------
# Library
# -------------------------------------------------------------------------------------------------

# include paths
include_directories( "${CMAKE_CURRENT_LIST_DIR}/include" )
include_directories( "${CMAKE_CURRENT_LIST_DIR}/../base/include" )
include_directories( "${CMAKE_CURRENT_LIST_DIR}/../platform_utils/include" )

#sources

SOURCE_GROUP( include  "include/" )
SOURCE_GROUP( source  "src/" )


set( SRC_FILE_LIST "" )

add_ll_source( ${LL_MODULE} SRC_FILE_LIST "include/systeminfo/types.h" )

add_ll_source( ${LL_MODULE} SRC_FILE_LIST "src/platform_impl.h" )

add_ll_source( ${LL_MODULE} SRC_FILE_LIST "src/exception.cpp" HAS_PUBLIC_HEADER )
add_ll_source( ${LL_MODULE} SRC_FILE_LIST "src/os.cpp" HAS_PUBLIC_HEADER )
add_ll_source( ${LL_MODULE} SRC_FILE_LIST "src/platform.cpp" HAS_PUBLIC_HEADER )
add_ll_source( ${LL_MODULE} SRC_FILE_LIST "src/storage.cpp" HAS_PUBLIC_HEADER )


if( WIN32 )
  add_ll_source( ${LL_MODULE} SRC_FILE_LIST "src/os_impl.win.cpp" )
  add_ll_source( ${LL_MODULE} SRC_FILE_LIST "src/platform_impl.win.cpp" )
  add_ll_source( ${LL_MODULE} SRC_FILE_LIST "src/storage_impl.win.cpp" )  
elseif( APPLE )
  add_ll_source( ${LL_MODULE} SRC_FILE_LIST "src/os_impl.osx.mm" )
  add_ll_source( ${LL_MODULE} SRC_FILE_LIST "src/platform_impl.osx.mm" )
  add_ll_source( ${LL_MODULE} SRC_FILE_LIST "src/storage_impl.osx.mm" )
else()
  add_ll_source( ${LL_MODULE} SRC_FILE_LIST "src/os_impl.linux.cpp" )
  add_ll_source( ${LL_MODULE} SRC_FILE_LIST "src/platform_impl.linux.cpp" )
  add_ll_source( ${LL_MODULE} SRC_FILE_LIST "src/storage_impl.linux.cpp" )
endif()

add_library( "ll_${LL_MODULE}${LL_ARCHITECTURE_POSTFIX}" STATIC ${SRC_FILE_LIST} )


# -------------------------------------------------------------------------------------------------
# Tests
# -------------------------------------------------------------------------------------------------

set( TEST_EXE_NAME "ll_${LL_MODULE}_test${LL_ARCHITECTURE_POSTFIX}" )
set( TESTCASE_DIR "tests/testcases" )
set( HELPERS_DIR "tests/helpers" )

# paths
include_directories( "${BOOST_INCLUDE_DIR}" )
include_directories( "${CATCH_INCLUDE_DIR}" )

link_directories( "${BOOST_LIBRARY_DIR}" )


# sources
SOURCE_GROUP( src\\helpers  "tests/helpers/" )
SOURCE_GROUP( src\\testcases  "tests/testcases/" )
SOURCE_GROUP( src "tests/" )
SOURCE_GROUP( helpers "helpers/" )

set( TEST_SRC_LIST "" )

#list( APPEND TEST_SRC_LIST "${TESTCASE_DIR}/hash.test.cpp" )
#list( APPEND TEST_SRC_LIST "${TESTCASE_DIR}/password.test.cpp" )


list( APPEND TEST_SRC_LIST "tests/main.cpp" )


add_executable( ${TEST_EXE_NAME} ${TEST_SRC_LIST} )


# link to the module(s)
add_ll_module( ${TEST_EXE_NAME} ${LL_MODULE} )


# link to externals
if( WIN32 )
  
 
else()
  
  if( APPLE )

    find_library(COCOA_FRAMEWORK Cocoa)
    if (NOT COCOA_FRAMEWORK)
      message( FATAL_ERROR "Cocoa Framework not found" )
    endif()

    target_link_libraries( ${TEST_EXE_NAME} ${COCOA_FRAMEWORK} )

  else()

  endif()

  target_link_libraries( ${TEST_EXE_NAME} pthread )
  target_link_libraries( ${TEST_EXE_NAME} boost_filesystem boost_system )  

  
endif()


# post-build step (copy test data)
#add_custom_command( TARGET ${TEST_EXE_NAME} POST_BUILD
#    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_LIST_DIR}/tests/data 
#    $<TARGET_FILE_DIR:${TEST_EXE_NAME}>/data
#)


# -------------------------------------------------------------------------------------------------
# Demo
# -------------------------------------------------------------------------------------------------

set( DEMO_EXE_NAME "ll_${LL_MODULE}_demo${LL_ARCHITECTURE_POSTFIX}" )

# paths
include_directories( "${BOOST_INCLUDE_DIR}" )

link_directories( "${BOOST_LIBRARY_DIR}" )
link_directories( "${LL_LIBRARY_PATH_RELEASE}" )


# sources

set( DEMO_SRC_LIST "examples/demo/main.cpp" )
list( APPEND DEMO_SRC_LIST "examples/demo/systeminfo_demo.manifest" )


add_executable( ${DEMO_EXE_NAME} ${DEMO_SRC_LIST} )


# link to the module(s)
add_ll_module( ${DEMO_EXE_NAME} ${LL_MODULE} )


# link to externals
if( WIN32 )

  target_link_libraries( ${DEMO_EXE_NAME}  powrprof dxgi )  

else()

  if( APPLE )

    find_library(COCOA_FRAMEWORK Cocoa)
    if (NOT COCOA_FRAMEWORK)
      message( FATAL_ERROR "Cocoa Framework not found" )
    endif()

    find_library(IO_KIT IOKit)
    if (NOT IO_KIT)
      message( FATAL_ERROR "IOKit not found" )
    endif()

    target_link_libraries( ${DEMO_EXE_NAME} ${COCOA_FRAMEWORK} ${IO_KIT} )

  else()

    target_link_libraries( ${DEMO_EXE_NAME} ll_platform_utils )

  endif()

  target_link_libraries( ${DEMO_EXE_NAME} boost_filesystem boost_system )  

endif()
