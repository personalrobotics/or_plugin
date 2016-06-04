cmake_minimum_required(VERSION 2.8.3)
project(or_plugin)
set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")

# Detect and include the YAML-CPP library.
set(YAMLCPP_STATIC_LIBRARY OFF CACHE BOOL
    "If true, try to find static yaml-cpp library first instead of dynamic.")
find_package(YamlCpp REQUIRED)
include_directories(${YAMLCPP_INCLUDE_DIRS})
add_definitions(${YAMLCPP_DEFINITIONS})

# Detect and include the OpenRAVE library.
find_package(OpenRAVE REQUIRED)
include_directories(${OpenRAVE_INCLUDE_DIRS})
add_definitions(${OpenRAVE_DEFINITIONS})

# Find and include Catkin dependencies.
find_package(catkin REQUIRED COMPONENTS
    openrave_catkin
)
catkin_package(
    INCLUDE_DIRS include
    LIBRARIES "${PROJECT_NAME}"
    DEPENDS yaml-cpp
)
include_directories(
    ${catkin_INCLUDE_DIRS}
    "include/"
)

add_library("${PROJECT_NAME}" SHARED
    src/module.cpp
)
target_link_libraries("${PROJECT_NAME}"
    ${catkin_LIBRARIES}
    ${YAMLCPP_LIBRARIES}
    ${OpenRAVE_LIBRARIES}
)
