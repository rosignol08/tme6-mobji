# -*- explicit-buffer-name: "CMakeLists.txt<M1-MOBJ/4-5>" -*-
#
# To display executed commands, use:
#     > cmake -DCMAKE_VERBOSE_MAKEFILE:STRING=YES ../src
find_package(LibXml2)
 cmake_minimum_required(VERSION 2.8.0)
 project(SCHEMATIC)
 
set (CMAKE_CXX_FLAGS "-Wall -g" CACHE STRING "C++ Compiler Release options." FORCE)
## Require C++11
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set (CMAKE_INSTALL_PREFIX       "../install" )

include_directories( ${SCHEMATIC_SOURCE_DIR} ${LIBXML2_INCLUDE_DIR})
set( includes Indentation.h Term.h Net.h Instance.h Point.h Node.h Cell.h)

set( cpps Indentation.cpp Term.cpp Net.cpp Instance.cpp Point.cpp Node.cpp Cell.cpp Main.cpp XmlUtil.cpp XmlUtil.h)

add_executable ( tme7 ${cpps} ) 
install ( TARGETS tme7 DESTINATION bin ) 
install ( FILES ${includes} DESTINATION include ) 
target_link_libraries ( tme7 ${LIBXML2_LIBRARIES} )
