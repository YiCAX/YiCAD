# 设置cmake 最低版本
cmake_minimum_required (VERSION 3.10)

PROJECT (YiCAD)

# 加载Qt模块
set(YICAD_USE_QT_MODULES Widgets Gui  Core  PrintSupport Designer Svg Network)

foreach(qtmod ${YICAD_USE_QT_MODULES})
    find_package(Qt5${qtmod} REQUIRED)
endforeach(qtmod)

set(CMAKE_AUTOMOC ON)
SET(QT_USE_QTDESIGNER ON)

# 设置qt包含目录
include_directories(${Qt5Core_INCLUDE_DIRS}
					${Qt5Gui_INCLUDE_DIRS}
					${Qt5Widgets_INCLUDE_DIRS}
					${Qt5PrintSupport_INCLUDE_DIRS}
					${Qt5Svg_INCLUDE_DIRS})

# 添加Boost库头文件（header-only: signals2, math, uuid, ublas, algorithm）
set(Boost_DIR ${CMAKE_SOURCE_DIR}/ThirdParty/boost/boost_1_90_0)
include_directories(${Boost_DIR})

# 添加gmp库（CGAL依赖于此）
if(WIN32)
    set(GMP_DIR ${CMAKE_SOURCE_DIR}/ThirdParty/gmp/include)
    set(GMP_LIB_DIR ${CMAKE_SOURCE_DIR}/ThirdParty/gmp/lib)
    include_directories(${GMP_DIR})
endif()

# 添加CGAL几何库
set(CGAL_DIR ${CMAKE_SOURCE_DIR}/ThirdParty/CGAL/CGAL-6.1/include)
include_directories(${CGAL_DIR})

# 添加freetype库 (todo：考虑到liunx macos后续需改成FindFreetype.cmke)
set(freetype_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/ThirdParty/freetype/freetypre-2.12.1/include)
set (freetype_LIB_DIR ${CMAKE_SOURCE_DIR}/ThirdParty/freetype/freetypre-2.12.1/lib/win/x64)
include_directories(${freetype_INCLUDE_DIR})

# 添加glm库 (该库只有hpp文件所以无需链接)
set(glm_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/ThirdParty/glm)
include_directories(${glm_INCLUDE_DIR})

# 添加glew库 (todo：考虑到liunx macos后续需改成FindGlew.cmke)
if(WIN32)
    set(glew_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/ThirdParty/glew/include)
    set (glew_LIB_DIR ${CMAKE_SOURCE_DIR}/ThirdParty/glew/lib/Release/x64)
    include_directories(${glew_INCLUDE_DIR})
endif(WIN32)

# 添加zlib库
if(WIN32)
    set(zlib_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/ThirdParty/zlib/include)
    set(zlib_LIB_DIR ${CMAKE_SOURCE_DIR}/ThirdParty/zlib/lib)
else()
    set(zlib_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/ThirdParty/zlib/linux/include)
    set(zlib_LIB_DIR ${CMAKE_SOURCE_DIR}/ThirdParty/zlib/linux/lib)
endif()
include_directories(${zlib_INCLUDE_DIR})

# 添加xerces库
if(WIN32)
    set(xercesc_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/ThirdParty/xercesc/include)
    set (xercesc_LIB_DIR ${CMAKE_SOURCE_DIR}/ThirdParty/xercesc/lib)
else()
    set(xercesc_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/ThirdParty/xercesc/linux/include)
    set (xercesc_LIB_DIR ${CMAKE_SOURCE_DIR}/ThirdParty/xercesc/linux/lib)
endif()
include_directories(${xercesc_INCLUDE_DIR})

# 添加mimalloc内存托管
if(WIN32)
    set (mimalloc_LIB_DIR ${CMAKE_SOURCE_DIR}/ThirdParty/mimalloc/lib)
else()
    set (mimalloc_LIB_DIR ${CMAKE_SOURCE_DIR}/ThirdParty/mimalloc/linux/lib)
endif()
set(mimalloc_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/ThirdParty/mimalloc/include)
include_directories(${mimalloc_INCLUDE_DIR})

# 设置.qrc目录
set(qrc_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/res/ribbon/ribbon_icon.qrc)

# #####################################################
# ====================actions==========================
FILE(GLOB ACTIONS_HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/src/actions/*.h)
file(GLOB ACTIONS_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/actions/*.cpp)

# #####################################################
# ======================cmd============================
file(GLOB CMD_HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/src/cmd/*.h)
file(GLOB CMD_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/cmd/*.cpp)

# #####################################################
# ======================lib============================
file(GLOB LIB_HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/src/kernel/actions/*.h
${CMAKE_CURRENT_SOURCE_DIR}/src/kernel/data_model/*.h
${CMAKE_CURRENT_SOURCE_DIR}/src/kernel/data_model/dimension/*.h
${CMAKE_CURRENT_SOURCE_DIR}/src/kernel/data_model/text/*.h
${CMAKE_CURRENT_SOURCE_DIR}/src/kernel/builder_model/*.h
${CMAKE_CURRENT_SOURCE_DIR}/src/kernel/builder_model/array/*.h
${CMAKE_CURRENT_SOURCE_DIR}/src/kernel/builder_model/dimension/*.h
${CMAKE_CURRENT_SOURCE_DIR}/src/kernel/builder_model/text/*.h
${CMAKE_CURRENT_SOURCE_DIR}/src/kernel/debug/*.h
${CMAKE_CURRENT_SOURCE_DIR}/src/kernel/fileio/*.h
${CMAKE_CURRENT_SOURCE_DIR}/src/kernel/filters/*.h
${CMAKE_CURRENT_SOURCE_DIR}/src/kernel/generators/*.h
${CMAKE_CURRENT_SOURCE_DIR}/src/kernel/gui/*.h
${CMAKE_CURRENT_SOURCE_DIR}/src/kernel/history/*.h
${CMAKE_CURRENT_SOURCE_DIR}/src/kernel/information/*.h
${CMAKE_CURRENT_SOURCE_DIR}/src/kernel/math/*.h
${CMAKE_CURRENT_SOURCE_DIR}/src/kernel/modification/*.h
${CMAKE_CURRENT_SOURCE_DIR}/src/kernel/printing/*.h
${CMAKE_CURRENT_SOURCE_DIR}/src/kernel/solver/*.h
${CMAKE_CURRENT_SOURCE_DIR}/src/kernel/painters/*.h
${CMAKE_CURRENT_SOURCE_DIR}/src/kernel/painters/opengl/*.h
${CMAKE_CURRENT_SOURCE_DIR}/src/kernel/temp/entity_builders/*.h
${CMAKE_CURRENT_SOURCE_DIR}/src/kernel/temp/entity_data/*.h
${CMAKE_CURRENT_SOURCE_DIR}/src/kernel/temp/geometry/*.h
${CMAKE_CURRENT_SOURCE_DIR}/src/kernel/utility/*.h
${CMAKE_CURRENT_SOURCE_DIR}/src/kernel/persistence/*.h
${CMAKE_CURRENT_SOURCE_DIR}/src/kernel/persistence/zipios++/*.h
${CMAKE_CURRENT_SOURCE_DIR}/src/kernel/persistence/Meta/*.h
)

file(GLOB LIB_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/kernel/actions/*.cpp
${CMAKE_CURRENT_SOURCE_DIR}/src/kernel/data_model/*.cpp
${CMAKE_CURRENT_SOURCE_DIR}/src/kernel/data_model/dimension/*.cpp
${CMAKE_CURRENT_SOURCE_DIR}/src/kernel/data_model/text/*.cpp
${CMAKE_CURRENT_SOURCE_DIR}/src/kernel/builder_model/*.cpp
${CMAKE_CURRENT_SOURCE_DIR}/src/kernel/builder_model/array/*.cpp
${CMAKE_CURRENT_SOURCE_DIR}/src/kernel/builder_model/dimension/*.cpp
${CMAKE_CURRENT_SOURCE_DIR}/src/kernel/builder_model/text/*.cpp
${CMAKE_CURRENT_SOURCE_DIR}/src/kernel/debug/*.cpp
${CMAKE_CURRENT_SOURCE_DIR}/src/kernel/fileio/*.cpp
${CMAKE_CURRENT_SOURCE_DIR}/src/kernel/filters/*.cpp
${CMAKE_CURRENT_SOURCE_DIR}/src/kernel/generators/*.cpp
${CMAKE_CURRENT_SOURCE_DIR}/src/kernel/gui/*.cpp
${CMAKE_CURRENT_SOURCE_DIR}/src/kernel/history/*.cpp
${CMAKE_CURRENT_SOURCE_DIR}/src/kernel/information/*.cpp
${CMAKE_CURRENT_SOURCE_DIR}/src/kernel/math/*.cpp
${CMAKE_CURRENT_SOURCE_DIR}/src/kernel/modification/*.cpp
${CMAKE_CURRENT_SOURCE_DIR}/src/kernel/printing/*.cpp
${CMAKE_CURRENT_SOURCE_DIR}/src/kernel/solver/*.cpp
${CMAKE_CURRENT_SOURCE_DIR}/src/kernel/painters/*.cpp
${CMAKE_CURRENT_SOURCE_DIR}/src/kernel/painters/opengl/*.cpp
${CMAKE_CURRENT_SOURCE_DIR}/src/kernel/temp/entity_builders/*.cpp
${CMAKE_CURRENT_SOURCE_DIR}/src/kernel/temp/entity_data/*.cpp
${CMAKE_CURRENT_SOURCE_DIR}/src/kernel/temp/geometry/*.cpp
${CMAKE_CURRENT_SOURCE_DIR}/src/kernel/utility/*.cpp
${CMAKE_CURRENT_SOURCE_DIR}/src/kernel/persistence/*.cpp
${CMAKE_CURRENT_SOURCE_DIR}/src/kernel/persistence/zipios++/*.cpp
${CMAKE_CURRENT_SOURCE_DIR}/src/kernel/persistence/Meta/*.cpp
)

# #####################################################
# ======================main===========================
file(GLOB MAIN_HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/src/main/*.h)

file(GLOB MAIN_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/main/*.cpp)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/src/main/YiCAD.rc.cmake ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/YiCAD.rc)
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/src/main/icon.ico DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
file(GLOB MAIN_RES ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/YiCAD.rc)

# #####################################################
# =====================plugins=========================
file(GLOB PLUGINS_HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/src/plugins/*.h
${CMAKE_CURRENT_SOURCE_DIR}/src/plugins/intern/*.h
${CMAKE_CURRENT_SOURCE_DIR}/src/plugins/filter/*.h)

file(GLOB PLUGINS_SOURCES  ${CMAKE_CURRENT_SOURCE_DIR}/src/plugins/intern/*.cpp
${CMAKE_CURRENT_SOURCE_DIR}/src/plugins/*.cpp
${CMAKE_CURRENT_SOURCE_DIR}/src/plugins/filter/*.cpp)

# #####################################################
# =======================ai=============================
file(GLOB AI_HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/src/ai/*.h)
file(GLOB AI_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/ai/*.cpp)

# #####################################################
# =======================UI============================
file(GLOB UI_HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/src/ui/*.h
${CMAKE_CURRENT_SOURCE_DIR}/src/ui/forms/*.h)

file(GLOB UI_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/ui/*.cpp
${CMAKE_CURRENT_SOURCE_DIR}/src/ui/forms/*.cpp)

# 设置.ui目录
file(GLOB UI_UIS_HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/src/ui/*.ui
${CMAKE_CURRENT_SOURCE_DIR}/src/ui/forms/*.ui)

# #####################################################
file(GLOB TS_FILES "${CMAKE_CURRENT_SOURCE_DIR}/ts/*.ts")
file(GLOB TS_PLUGIN_FILES "plugins/ts/*.ts")
   
# 将.ui转成.h
QT5_WRAP_UI(ui_FILES ${UI_UIS_HEADERS})

# 将.qrc转成.cpp
qt5_add_resources(qrc_FILES ${qrc_DIRS})

# 生成moc文件
qt5_wrap_cpp(moc_FILES ${UI_HEADERS})

# 将生成的uic文件夹添加到包含目录
include_directories(${CMAKE_CURRENT_BINARY_DIR})

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/$<CONFIGURATION>)

# 附加库路径(lib路径)
FIND_PACKAGE(Qt5LinguistTools)
set (ALL_RSC_FILES ${ACTIONS_SOURCES}
                    ${ACTIONS_HEADERS}
                    ${CMD_SOURCES} 
                    ${CMD_HEADERS}
                    ${MAIN_SOURCES}
                    ${MAIN_HEADERS}
                    ${PLUGINS_SOURCES} 
                    ${PLUGINS_HEADERS}
                    ${AI_SOURCES}
                    ${AI_HEADERS}
                    ${UI_SOURCES}
                    ${LIB_SOURCES} 
                    ${LIB_HEADERS}
		    ${MAIN_RES}
                    ${ZIPIOS_HEADERS}
                    ${ZIPIOS_SRCS}
                     )

if(UPDATE_TRANSLATIONS)
    QT5_CREATE_TRANSLATION_Exclude_SubFolder(QM_FILES ${ALL_RSC_FILES} ${TS_FILES} ${TS_PLUGIN_FILES})
else()
    QT5_ADD_TRANSLATION(QM_FILES ${TS_FILES} ${TS_PLUGIN_FILES})
endif(UPDATE_TRANSLATIONS )

SET(QM_FILES ${QM_FILES} PARENT_SCOPE)
ADD_CUSTOM_TARGET (TS_YiCAD ALL DEPENDS ${QM_FILES})
set_target_properties(TS_YiCAD PROPERTIES FOLDER "CMakePredefinedTargets")

add_executable(${PROJECT_NAME} ${ALL_RSC_FILES}                                 
                                ${UI_HEADERS}                               
                                ${ui_FILES}
                                ${moc_FILES}
                                 ${qrc_DIRS}
                                 ${qrc_FILES}
                                 ${QM_FILES})

# 支持linux相对路径库加载，参考：https://stackoverflow.com/questions/43330165/how-to-link-a-shared-library-with-cmake-with-relative-path/69707790#69707790
# https://gitlab.kitware.com/cmake/community/-/wikis/doc/cmake/RPATH-handling
if(NOT WIN32)
set_target_properties(${PROJECT_NAME} PROPERTIES
    LINK_FLAGS "-Wl,-rpath,$ORIGIN")
endif()

include_directories(${CMAKE_SOURCE_DIR}/ThirdParty
                    ${CMAKE_SOURCE_DIR}/libraries/muparser/src
                    ${CMAKE_SOURCE_DIR}/libraries/SARibbonBar)

include_directories(src/actions
src/cmd
src/kernel/data_model
src/kernel/data_model/dimension
src/kernel/data_model/text
src/kernel/builder_model
src/kernel/builder_model/array
src/kernel/builder_model/dimension
src/kernel/builder_model/text
src/kernel/actions
src/kernel/debug
src/kernel/engine
src/kernel/fileio
src/kernel/filters
src/kernel/generators
src/kernel/gui
src/kernel/history
src/kernel/information
src/kernel/math
src/kernel/modification
src/kernel/printing
src/kernel/scripting
src/kernel/solver
src/kernel/painters
src/kernel/painters/opengl
src/kernel/temp/entity_builders
src/kernel/temp/entity_data
src/kernel/utility
src/kernel/persistence
src/kernel/persistence/zipios++
src/kernel/persistence/Meta
src/main
src/ai
src/plugins
src/plugins/intern
src/ui
src/ui/forms)


# 添加预处理器定义值
Add_Definitions(-D_USE_MATH_DEFINES -D_ENABLE_EXTENDED_ALIGNED_STORAGE -DAPPDIR="YiCAD" -DVERSION="0.5.0.0-alpha")
remove_definitions(-D_HAS_STD_BYTE)

# MSVC: 强制源文件和执行字符集为 UTF-8，避免中文乱码
if(MSVC)
    add_compile_options(/utf-8)
endif()
#add_compile_definitions(/FI${mimalloc_INCLUDE_DIR}/mimalloc-override.h)
add_link_options(/INCLUDE:mi_version)

# 根据选项
use_visual_leak_detector(${PROJECT_NAME})

if(WIN32)
    #mimalloc
    target_link_libraries(${PROJECT_NAME} optimized ${mimalloc_LIB_DIR}/release/mimalloc.lib)
    target_link_libraries(${PROJECT_NAME} debug ${mimalloc_LIB_DIR}/debug/mimalloc.lib)

    #xerces
    target_link_libraries(${PROJECT_NAME} optimized ${xercesc_LIB_DIR}/xerces-c_3.lib)
    target_link_libraries(${PROJECT_NAME} debug ${xercesc_LIB_DIR}/xerces-c_3D.lib)

    #cgal
    target_link_libraries(${PROJECT_NAME} ${GMP_LIB_DIR}/libgmp-10.lib ${GMP_LIB_DIR}/libmpfr-4.lib)

    target_link_libraries(${PROJECT_NAME} ${freetype_LIB_DIR}/freetype.lib)
    target_link_libraries(${PROJECT_NAME} opengl32.lib glu32.lib)
    target_link_libraries(${PROJECT_NAME} ${glew_LIB_DIR}/glew32.lib)

    #zlib
    target_link_libraries(${PROJECT_NAME} optimized ${zlib_LIB_DIR}/zlib.lib)
    target_link_libraries(${PROJECT_NAME} debug ${zlib_LIB_DIR}/zlibd.lib)
    
    # windows系统
    target_link_libraries(${PROJECT_NAME} Dwrite.lib)
else()
    target_link_libraries(${PROJECT_NAME} ${mimalloc_LIB_DIR}/libmimalloc.so)
    target_link_libraries(${PROJECT_NAME} ${zlib_LIB_DIR}/libz.so)
    target_link_libraries(${PROJECT_NAME} ${xercesc_LIB_DIR}/libxerces-c.so)
    target_link_libraries(${PROJECT_NAME} libGL.so libGLEW.so libfreetype.so libgmp.so libgmpxx.so libmpfr.so )
    #message(STATUS "LINK_DIRECTORIES : ${LINK_DIRECTORIES}")

    # 解决linux编译时因链接顺序报DSO的问题
    set(CMAKE_EXE_LINKER_FLAGS "-Wl,--copy-dt-needed-entries")

endif()

# 添加依赖项(lib项)
set(EXTRA_LIBS muparser SARibbonBar)
target_link_libraries(${PROJECT_NAME} ${EXTRA_LIBS})

if(WIN32)
    set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS_DEBUG "/SUBSYSTEM:CONSOLE")
    set_target_properties(${PROJECT_NAME} PROPERTIES COMPILE_DEFINITIONS_DEBUG "_CONSOLE")
    set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS_RELWITHDEBINFO "/SUBSYSTEM:WINDOWS")
    set_target_properties(${PROJECT_NAME} PROPERTIES COMPILE_DEFINITIONS_RELWITHDEBINFO "/SUBSYSTEM:WINDOWS")
    set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS_RELEASE "/SUBSYSTEM:WINDOWS")
    set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS_MINSIZEREL "/SUBSYSTEM:WINDOWS")
endif()

#add QT one by one
foreach(qtmod ${YICAD_USE_QT_MODULES})
    target_link_libraries(${PROJECT_NAME} Qt5::${qtmod})
endforeach(qtmod)

#copy all third party files to target folder
if(WIN32)

    #copy all qt related files
    windeployqt(${PROJECT_NAME})

    INSTALL(DIRECTORY ${CMAKE_SOURCE_DIR}/ThirdParty/glew/bin/Release/x64/ DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/ CONFIGURATIONS  Debug FILES_MATCHING PATTERN "*.*")
    INSTALL(DIRECTORY ${CMAKE_SOURCE_DIR}/ThirdParty/glew/bin/Release/x64/ DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/ CONFIGURATIONS  Release OR  RelWithDebInfo FILES_MATCHING PATTERN "*.*")
    INSTALL(DIRECTORY ${CMAKE_SOURCE_DIR}/ThirdParty/CGAL/bin/ DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/ CONFIGURATIONS  Debug FILES_MATCHING PATTERN "*.*")
    INSTALL(DIRECTORY ${CMAKE_SOURCE_DIR}/ThirdParty/CGAL/bin/ DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/ CONFIGURATIONS  Release OR  RelWithDebInfo FILES_MATCHING PATTERN "*.*")
	INSTALL(DIRECTORY ${CMAKE_SOURCE_DIR}/ThirdParty/zlib/bin/ DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/ CONFIGURATIONS  Debug FILES_MATCHING PATTERN "*.*")
	INSTALL(DIRECTORY ${CMAKE_SOURCE_DIR}/ThirdParty/zlib/bin/ DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/ CONFIGURATIONS  Release OR  RelWithDebInfo FILES_MATCHING PATTERN "*.*")
	INSTALL(DIRECTORY ${CMAKE_SOURCE_DIR}/ThirdParty/xercesc/bin/ DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/ CONFIGURATIONS  Debug FILES_MATCHING PATTERN "*.*")
	INSTALL(DIRECTORY ${CMAKE_SOURCE_DIR}/ThirdParty/xercesc/bin/ DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/ CONFIGURATIONS  Release OR  RelWithDebInfo FILES_MATCHING PATTERN "*.*")
    INSTALL(DIRECTORY ${CMAKE_SOURCE_DIR}/ThirdParty/CGAL/bin/ DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/ CONFIGURATIONS  Release OR  RelWithDebInfo  FILES_MATCHING PATTERN "*.*")
    INSTALL(DIRECTORY ${CMAKE_SOURCE_DIR}/ThirdParty/gmp/bin/ DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/ CONFIGURATIONS  Debug OR Release OR  RelWithDebInfo FILES_MATCHING PATTERN "*.*")
    INSTALL(DIRECTORY ${CMAKE_SOURCE_DIR}/ThirdParty/mimalloc/bin/debug/ DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/ CONFIGURATIONS  Debug FILES_MATCHING PATTERN "*.*")
    INSTALL(DIRECTORY ${CMAKE_SOURCE_DIR}/ThirdParty/mimalloc/bin/release/ DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/ CONFIGURATIONS  Release OR RelWithDebInfo FILES_MATCHING PATTERN "*.*")
endif()

#install all resources
INSTALL(FILES ${QM_FILES} DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/resources/qm)
INSTALL(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/support/patterns/ DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/resources/patterns FILES_MATCHING PATTERN "*.*")
INSTALL(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/support/linetypes/ DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/resources/linetypes FILES_MATCHING PATTERN "*.*")
INSTALL(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/support/fonts/ DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/resources/fonts FILES_MATCHING PATTERN "*.shx")
INSTALL(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/res/shaders/ DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/resources/shaders FILES_MATCHING PATTERN "*.*")

#install config file
INSTALL(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/support/config/ DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/ FILES_MATCHING PATTERN "*.*")

#structure files
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}/src/ui" PREFIX "UIs" FILES ${UI_UIS_HEADERS})
GroupSourcesByFolder(${PROJECT_NAME})