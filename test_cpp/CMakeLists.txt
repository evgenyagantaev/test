message("")
message(">>> Hello, Master! <<<")
message(">>> CMake is the most powerful buildsystem! <<<")
message("")

cmake_minimum_required(VERSION 3.10)
project(Test_cpp VERSION 0.0.0.1 LANGUAGES C CXX)

set(TARGET_NAME "msg_dispatcher_cpp")
set(TEST_TARGET_NAME "test_runner")

if (CMAKE_BUILD_TYPE STREQUAL "")
   message(STATUS "CMAKE_BUILD_TYPE not defined, 'Debug' will be used")
#    set(CMAKE_BUILD_TYPE "Release")
   set(CMAKE_BUILD_TYPE "Debug")
endif()

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
#set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build)
#set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}
#    -Wall -Werror -std=c++14 -fstrict-aliasing -pedantic-errors
#    -pedantic -Wno-deprecated-declarations -Wno-unused-variable")

set(CMAKE_CXX_FLAGS "-Wall -Wextra -fPIC -fopenmp  -DBOOST_LOG_DYN_LINK")

include(FetchContent)
FetchContent_Declare(
  googletest
  # Specify the commit you depend on and update it regularly.
  URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
)
FetchContent_MakeAvailable(googletest)


find_package(OpenCV  REQUIRED)
include_directories (${OpenCV_INCLUDE_DIRS})


file (GLOB MAIN_SRC
        ${CMAKE_CURRENT_SOURCE_DIR}/*.c*
        ${CMAKE_CURRENT_SOURCE_DIR}/Src/*.c*
         #${CMAKE_CURRENT_SOURCE_DIR}/*.qrc
        #${QM_FILES}
        )

file (GLOB MAIN_HEADERS
     ${CMAKE_CURRENT_SOURCE_DIR}/*.h*
     ${CMAKE_CURRENT_SOURCE_DIR}/Inc/*.h*
     ${CMAKE_CURRENT_SOURCE_DIR}/tests/*.h*
     )

file (GLOB TEST_SRC
     ${CMAKE_CURRENT_SOURCE_DIR}/tests/*.c*
     ${MAIN_SRC}
     )
list(FILTER TEST_SRC EXCLUDE REGEX ".*main\.cpp.*")     

file (GLOB TEST_HEADERS
     ${CMAKE_CURRENT_SOURCE_DIR}/tests/*.h*
     )

source_group("Src" FILES ${MAIN_SRC})
source_group("Inc" FILES ${MAIN_HEADERS})

include_directories(
        /usr/include
        ${CMAKE_CURRENT_SOURCE_DIR}/Inc
        )

add_executable(${TARGET_NAME} ${MAIN_SRC} ${MAIN_HEADERS})

target_link_libraries(${TARGET_NAME}  PRIVATE ${OpenCV_LIBRARIES} )

enable_testing()

add_executable(${TEST_TARGET_NAME} ${TEST_SRC} ${TEST_HEADERS} ${MAIN_HEADERS} ) 
#target_sources(${TEST_TARGET_NAME} PUBLIC 
        #${CMAKE_CURRENT_SOURCE_DIR}/Src/sample1.cpp
#        ${CMAKE_CURRENT_SOURCE_DIR}/Src/utilities.cpp
#        )

target_link_libraries(${TEST_TARGET_NAME} PRIVATE ${OpenCV_LIBRARIES} gtest_main)

include(GoogleTest)
gtest_discover_tests(${TEST_TARGET_NAME})