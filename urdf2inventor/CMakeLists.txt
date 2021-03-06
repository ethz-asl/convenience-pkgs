cmake_minimum_required(VERSION 2.8.3)
project(urdf2inventor)

include(cmake/architecture_binding.cmake.in)

# set this variable to true if you would like that cmake
# downloads and builds the ivcon package
# (https://github.com/JenniferBuehler/ivcon).
# If this is set to false, it will search for the package
# ivcon in your catkin workspace instead.
# IMPORTANT: If you set this to false and are compiling
# with catkin, uncomment the <*_depend> tags relating to
# graspit in package.xml!
if (NOT DEFINED BUILD_IVCON)
    set (BUILD_IVCON true)
endif (NOT DEFINED BUILD_IVCON)

set (CATKIN_PKGS
  roscpp
  urdf
  eigen_conversions
  architecture_binding
  roslint
)

if (NOT BUILD_IVCON)
    set (CATKIN_PKGS ${CATKIN_PKGS} ivcon)
endif (NOT BUILD_IVCON)

## Find catkin macros and libraries
## if COMPONENTS list like find_package(catkin REQUIRED COMPONENTS xyz)
## is used, also find other catkin packages
find_package(catkin REQUIRED COMPONENTS ${CATKIN_PKGS})

## System dependencies are found with CMake's conventions
set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/CMakeMacros/")
find_package(SoQt4 REQUIRED)
find_package(Coin REQUIRED)
find_package(Qt4 REQUIRED)

# if the ivcon fork should be build with this package,
# do this now.
if (BUILD_IVCON)
    set (GLOBAL_OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR}/ivcon/install)
    include(CMakeLists-external-ivcon.txt)
endif (BUILD_IVCON)


## System dependencies are found with CMake's conventions
# find_package(Boost REQUIRED COMPONENTS system)

## Uncomment this if the package has a setup.py. This macro ensures
## modules and global scripts declared therein get installed
## See http://ros.org/doc/api/catkin/html/user_guide/setup_dot_py.html
# catkin_python_setup()

###################################
## catkin specific configuration ##
###################################

set (CATKIN_DEPEND_PKGS roscpp urdf eigen_conversions architecture_binding)
if (NOT BUILD_IVCON)
    set (CATKIN_DEPEND_PKGS ${CATKIN_DEPEND_PKGS} ivcon)
endif (NOT BUILD_IVCON)

set (CATKIN_DEPENDS_LIBS libsoqt4-dev libqt4-dev libcoin80-dev git)

## The catkin_package macro generates cmake config files for your package
## Declare things to be passed to dependent projects
## INCLUDE_DIRS: uncomment this if you package contains header files
## LIBRARIES: libraries you create in this project that dependent projects also need
## CATKIN_DEPENDS: catkin_packages dependent projects also need
## DEPENDS: system dependencies of this project that dependent projects also need
catkin_package(
  INCLUDE_DIRS include
  LIBRARIES urdf2inventor
  CATKIN_DEPENDS ${CATKIN_DEPEND_PKGS} 
  DEPENDS ${CATKIN_DEPENDS_LIBS}
)


###########
## Build ##
###########

## Specify additional locations of header files
## Your package locations should be listed before other locations
include_directories(
  include
  ${catkin_INCLUDE_DIRS}
  ${SOQt_INCLUDE_DIRS}
  ${QT_INCLUDE_DIR}
  ${QT_INCLUDE_DIR}/Qt
  ${IVCON_INCLUDE_DIRS}
)

## Declare a C++ library
add_library(urdf2inventor
  src/Urdf2Inventor.cpp
  src/Helpers.cpp
  src/IVHelpers.cpp
  src/InventorViewer.cpp
)

## Add cmake target dependencies of the library
## as an example, code may need to be generated before libraries
## either from message generation or dynamic reconfigure
add_dependencies(urdf2inventor ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})

## Declare a C++ executable
add_executable(urdf2inventor_node src/urdf2inventor_node.cpp)
add_executable(urdf_viewer src/urdf_viewer_node.cpp)

## Add cmake target dependencies of the executable
## same as for the library above
add_dependencies(urdf2inventor_node ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})
add_dependencies(urdf_viewer ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})

if (BUILD_IVCON)
    add_dependencies(urdf2inventor ivcon_external)
endif (BUILD_IVCON)


set (DEPEND_LIBRARIES
   ${catkin_LIBRARIES}
   ${SOQT_LIBRARY}
   ${QT_LIBRARIES}
)

if (BUILD_IVCON)
    set (DEPEND_LIBRARIES
       ${DEPEND_LIBRARIES}
       ${IVCON_LIBRARY}
    )
endif (BUILD_IVCON)

## Specify libraries to link a library or executable target against
target_link_libraries(urdf2inventor ${DEPEND_LIBRARIES})
target_link_libraries(urdf2inventor_node urdf2inventor ${DEPEND_LIBRARIES})
target_link_libraries(urdf_viewer urdf2inventor ${DEPEND_LIBRARIES})

#############
## Install ##
#############

# all install targets should use catkin DESTINATION variables
# See http://ros.org/doc/api/catkin/html/adv_user_guide/variables.html

## Mark executable scripts (Python etc.) for installation
## in contrast to setup.py, you can choose the destination
# install(PROGRAMS
#   scripts/my_python_script
#   DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
# )

## Mark executables and/or libraries for installation
# install(TARGETS urdf2inventor urdf2inventor_node
#   ARCHIVE DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
#   LIBRARY DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
#   RUNTIME DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
# )

## Mark cpp header files for installation
# install(DIRECTORY include/${PROJECT_NAME}/
#   DESTINATION ${CATKIN_PACKAGE_INCLUDE_DESTINATION}
#   FILES_MATCHING PATTERN "*.h"
#   PATTERN ".svn" EXCLUDE
# )

## Mark other files for installation (e.g. launch and bag files, etc.)
# install(FILES
#   # myfile1
#   # myfile2
#   DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION}
# )

#############
## Testing ##
#############

## Add gtest based cpp test target and link libraries
# catkin_add_gtest(${PROJECT_NAME}-test test/test_urdf2inventor.cpp)
# if(TARGET ${PROJECT_NAME}-test)
#   target_link_libraries(${PROJECT_NAME}-test ${PROJECT_NAME})
# endif()

## Add folders to be run by python nosetests
# catkin_add_nosetests(test)
