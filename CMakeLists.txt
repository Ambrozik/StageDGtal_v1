
project(StageDGtal)

cmake_minimum_required(VERSION 2.8.11)
aux_source_directory(. SRC_LIST)

cmake_policy(SET CMP0057 NEW)

set(CMAKE_INCLUDE_PATH /home/ambrozik/Documents/DGtal/build/)
#Required in DGtal

FIND_PACKAGE(DGtal REQUIRED)
INCLUDE_DIRECTORIES(${DGTAL_INCLUDE_DIRS})
LINK_DIRECTORIES(${DGTAL_LIBRARY_DIRS})


find_package(Boost QUIET REQUIRED COMPONENTS program_options)

if(NOT Boost_FOUND)
    message(FATAL_ERROR "Boost Not found")
endif()

add_definitions(-g)

add_executable(${PROJECT_NAME} ${SRC_LIST} AdaptiveTangentCover/functions.cpp AdaptiveTangentCover/ATC_Computation.cpp)

target_link_libraries(${PROJECT_NAME} ${DGTAL_LIBRARIES})
target_link_libraries(${PROJECT_NAME} PUBLIC Boost::program_options)

