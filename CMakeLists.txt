cmake_minimum_required(VERSION 3.16)
project(InvestmentsModeling LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# find Qt6
find_package(Qt6 REQUIRED COMPONENTS Widgets Charts)

# Recommended: add nlohmann_json only if you want to use it later
# find_package(nlohmann_json CONFIG REQUIRED)

add_subdirectory(src)
# add_subdirectory(tests OPTIONAL) # if you add tests later
