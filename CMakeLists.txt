# 设置cmake 最低版本
cmake_minimum_required (VERSION 3.10)

# append the qt folder
if(WIN32)

elseif(UNIX)
     list(APPEND CMAKE_PREFIX_PATH "/opt/Qt5.14.2/5.14.2/gcc_64")
elseif(APPLE)

endif()

# 设置项目名称
project (YiCAD)

set(PACKAGE_VERSION_MAJOR "0")
set(PACKAGE_VERSION_MINOR "20")
set(PACKAGE_VERSION_PATCH "0")
set(PACKAGE_VERSION_SUFFIX "dev") # either "dev" for development snapshot or "" (empty string)
set(YICAD_VERSION_PATCH "0") # number of patch release (e.g. "4" for the 0.18.4 release)
set(YICAD_VERSION "${PACKAGE_VERSION_MAJOR}.${PACKAGE_VERSION_MINOR}")

set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set_property(GLOBAL PROPERTY PREDEFINED_TARGETS_FOLDER "CMakePredefinedTargets")

# Add Path for searching cmake files
# 下面这2行代码针对有些qt版本无法找到的情况
#set(ENV{Qt_DIR} "D:/Qt/5.15.2/msvc2019_64")
#set(CMAKE_PREFIX_PATH $ENV{Qt5_DIR}/lib/cmake)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/cmakeutil.cmake)
download_thirdparty()
if(UNIX)
     # 先确保linux下有已经编好的qt路径（TODO ： 待修改）
     #list(APPEND CMAKE_PREFIX_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../3rd_patry/Qt/qt-5.12.11-x64-lnx)
elseif(WIN32)
     list(APPEND CMAKE_PREFIX_PATH ${CMAKE_CURRENT_SOURCE_DIR}/ThirdParty/qt/msvc2019_64)
endif()
include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/deployqt.cmake)

# Where is the binary should be placed
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Define compiler warnings
if (NOT MSVC) # Too much warnings on MSVC
     add_definitions("-Wall")
endif()

# 设置 std 和 c++版本为
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 23)

set(Boost_USE_MULTITHREADED ON)

# settings path
set(SETTINGS_PATH "${PROJECT_BINARY_DIR}/bin")

#disable warnings
#4819 non Unicode format warning
if(MSVC)
     add_compile_options(/MP /W3 /wd4819 /Zc:inline)
endif()

OPTION(UPDATE_TRANSLATIONS "Update translations on compile" OFF)
MESSAGE(STATUS "UPDATE TRANSLATIONS: ${UPDATE_TRANSLATIONS}")
option(USE_VISUAL_LEAK_DETECTOR "Use visual leak detector to detect memory leakage" OFF)

# 设置子项目
add_subdirectory("libraries")
add_subdirectory("YiCAD")
add_subdirectory("plugins")