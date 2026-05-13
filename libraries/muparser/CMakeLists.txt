# 设置cmake 最低版本
cmake_minimum_required (VERSION 3.10)

project(muparser)


option(ENABLE_OPENMP "Enable OpenMP for multithreading" OFF)
option(ENABLE_WIDE_CHAR "Enable wide character support" OFF)

if(ENABLE_OPENMP)
    find_package(OpenMP REQUIRED)
    set(CMAKE_CXX_FLAGS "${OpenMP_CXX_FLAGS} ${CMAKE_CXX_FLAGS}")
    set(CMAKE_SHARED_LIBRARY_CXX_FLAGS "${OpenMP_CXX_FLAGS} ${CMAKE_SHARED_LIBRARY_CXX_FLAGS}")
endif()

if(MSVC)
    # Force to always compile with W4
    if(CMAKE_CXX_FLAGS MATCHES "/W[0-4]")
      string(REGEX REPLACE "/W[0-4]" "/W4" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
    else()
      set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
    endif()
elseif(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
    # Update if necessary
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wno-long-long -pedantic")
endif()

FILE(GLOB_RECURSE MUPARSER_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp") #all .cpp
FILE(GLOB_RECURSE MUPARSER_HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h") #all .cpp

add_library(${PROJECT_NAME} STATIC ${MUPARSER_SOURCES} ${MUPARSER_HEADERS})

target_compile_definitions(${PROJECT_NAME} PUBLIC MUPARSER_STATIC)
add_definitions( -DMUPARSER_STATIC )

if(ENABLE_OPENMP)
  target_compile_definitions(${PROJECT_NAME} PRIVATE MUP_USE_OPENMP)
endif()

if(ENABLE_WIDE_CHAR)
  target_compile_definitions(${PROJECT_NAME} PUBLIC _UNICODE)
endif()


GroupSourcesByFolder(${PROJECT_NAME})