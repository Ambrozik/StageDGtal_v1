project(ATC_Computation)
cmake_minimum_required(VERSION 2.8)

cmake_policy(SET CMP0057 NEW)

#Required in DGtal
FIND_PACKAGE(DGtal REQUIRED)
INCLUDE_DIRECTORIES(${DGTAL_INCLUDE_DIRS})
LINK_DIRECTORIES(${DGTAL_LIBRARY_DIRS})

FIND_PACKAGE(Boost 1.46.0 REQUIRED COMPONENTS program_options)
if ( Boost_FOUND )
    message(STATUS "Boost and boost_program_options found.")
    include_directories( ${Boost_INCLUDE_DIRS} )
    SET(DGtalToolsLibDependencies ${DGtalToolsLibDependencies}
        ${Boost_LIBRAIRIES}
        ${Boost_PROGRAM_OPTIONS_LIBRARY})
    SET(DGtalLibInc ${Boost_INCLUDE_DIRS})
endif( Boost_FOUND)

add_definitions(-g)

aux_source_directory(. SRC_LIST)
add_executable(${PROJECT_NAME} ${SRC_LIST})
TARGET_LINK_LIBRARIES(ATC_Computation ${DGTAL_LIBRARIES} ${DGtalToolsLibDependencies})

