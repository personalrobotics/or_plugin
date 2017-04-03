cmake_minimum_required(VERSION 2.8.3)
project(or_plugin)
set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")
add_definitions("-std=c++11")

# Detect and include the YAML-CPP library.
set(YAMLCPP_STATIC_LIBRARY OFF CACHE BOOL
    "If true, try to find static yaml-cpp library first instead of dynamic.")
find_package(YamlCpp REQUIRED)
include_directories(SYSTEM ${YAMLCPP_INCLUDE_DIRS})
add_definitions(${YAMLCPP_DEFINITIONS})

# Detect and include the OpenRAVE library.
find_package(OpenRAVE REQUIRED)
include_directories(SYSTEM ${OpenRAVE_INCLUDE_DIRS})
add_definitions(${OpenRAVE_DEFINITIONS})

# Find and include Catkin dependencies.
find_package(catkin REQUIRED COMPONENTS
    openrave_catkin
)
include_directories(SYSTEM ${catkin_INCLUDE_DIRS})

catkin_package(
    INCLUDE_DIRS include
    DEPENDS yaml-cpp
    CFG_EXTRAS or_plugin_extras.cmake
)
catkin_python_setup()
include_directories("include/")

if (CATKIN_ENABLE_TESTING)
    # TODO: Re-enable unit tests once they are working.
    #catkin_add_nosetests(tests)
endif()
