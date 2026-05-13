# 设置子项目

set(YICAD_PLUGINS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../YiCAD/src/plugins)

FILE(GLOB_RECURSE TS_PLG_FILES  ${CMAKE_CURRENT_SOURCE_DIR}/ts/*.ts)

# 将.ui转成.h
QT5_WRAP_UI(ui_FILES ${UI_UIS_HEADERS})

# 将.qrc转成.cpp
qt5_add_resources(qrc_FILES ${qrc_DIRS})

# 生成moc文件
qt5_wrap_cpp(moc_FILES ${UI_HEADERS})

# 将生成的uic文件夹添加到包含目录
include_directories(${CMAKE_CURRENT_BINARY_DIR})

# 附加库路径(lib路径)
FIND_PACKAGE(Qt5LinguistTools)
QT5_ADD_TRANSLATION(QM_PLUG_FILES ${TS_PLG_FILES})
SET(QM_PLUG_FILES ${QM_PLUG_FILES} PARENT_SCOPE)
ADD_CUSTOM_TARGET(TS_Plugins ALL DEPENDS ${QM_PLUG_FILES})
set_target_properties(TS_Plugins PROPERTIES FOLDER "CMakePredefinedTargets")

install(FILES ${QM_PLUG_FILES} DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/$<CONFIGURATION>/resources/qm)