# The following five lines of boilerplate have to be in your project's
# CMakeLists in this exact order for cmake to work correctly
cmake_minimum_required(VERSION 3.5)

include($ENV{IDF_PATH}/tools/cmake/project.cmake)
idf_build_component("components/driver_mqtt")
idf_build_component("components/driver_wifi")
idf_build_component("components/driver_ade")
idf_build_component("components/Tasks")
project(ziggy)