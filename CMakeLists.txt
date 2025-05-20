cmake_minimum_required(VERSION 3.15)
project(USDA-FoodCentral-ETL)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_CXX_FLAGS_RELEASE "-O3")
set(CMAKE_INTERPROCEDURAL_OPTIMIZATION_RELEASE TRUE)

set(INPUT_LOCATIONS_FILE "${CMAKE_CURRENT_SOURCE_DIR}/input_locations.txt")

if(NOT EXISTS "${INPUT_LOCATIONS_FILE}")
    message(FATAL_ERROR "Input locations file not found: ${INPUT_LOCATIONS_FILE}"
    )
endif()

file(GLOB_RECURSE SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")

add_executable(${PROJECT_NAME} ${SOURCES})

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/input_locations.txt
    ${CMAKE_CURRENT_BINARY_DIR}/input_locations.txt
    COPYONLY
)
