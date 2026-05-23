function(GroupSourcesByFolder target)
  set(SOURCE_GROUP_DELIMITER "/")
  set(last_dir "")
  set(files "")

  get_target_property(sources ${target} SOURCES)

  foreach(file ${sources})
    file(RELATIVE_PATH relative_file "${PROJECT_SOURCE_DIR}" ${file})
    get_filename_component(dir "${relative_file}" PATH)
    if(NOT "${dir}" STREQUAL "${last_dir}")
      if(files)
        source_group("${last_dir}" FILES ${files})
      endif()
      set(files "")
    endif()
    set(files ${files} ${file})
    set(last_dir "${dir}")
  endforeach()

  if(files)
    source_group("${last_dir}" FILES ${files})
  endif()
endfunction()


macro(add_copy_fullfolder_command _folder)
    get_filename_component(_folder_base "${_folder}/."   ABSOLUTE)
    get_filename_component(_folder_name "${_folder}"      NAME)
    
	set(_destination $<TARGET_FILE_DIR:${PROJECT_NAME}>/)
    file(GLOB_RECURSE allfiles RELATIVE "${_folder_base}" "${_folder}/*")
    set(_old_fn_folder "")
    foreach(_fn ${allfiles})
        get_filename_component(_fn_folder "${_destination}/${_fn}" DIRECTORY)
        #if (NOT "${_fn_folder}" STREQUAL "${_old_fn_folder}")
         #   add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD COMMAND ${ARGN} ${CMAKE_COMMAND} -E make_directory ${_fn_folder})
          #  set(_old_fn_folder ${_fn_folder})
        #endif()
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD COMMAND ${ARGN} ${CMAKE_COMMAND} -E copy_if_different ${_folder_base}/${_fn}  $<TARGET_FILE_DIR:${PROJECT_NAME}>/${_fn})
    endforeach()
endmacro()

function(QT5_CREATE_TRANSLATION_Exclude_SubFolder _qm_files)
    set(options)
    set(oneValueArgs)
    set(multiValueArgs OPTIONS)

    cmake_parse_arguments(_LUPDATE "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})
    set(_lupdate_files ${_LUPDATE_UNPARSED_ARGUMENTS})
    set(_lupdate_options ${_LUPDATE_OPTIONS})

    set(_my_sources)
    set(_my_tsfiles)
    foreach(_file ${_lupdate_files})
        get_filename_component(_ext ${_file} EXT)
        get_filename_component(_abs_FILE ${_file} ABSOLUTE)
        if(_ext MATCHES "ts")
            list(APPEND _my_tsfiles ${_abs_FILE})
        else()
            list(APPEND _my_sources ${_abs_FILE})
        endif()
    endforeach()
    foreach(_ts_file ${_my_tsfiles})
        if(_my_sources)
          # make a list file to call lupdate on, so we don't make our commands too
          # long for some systems
          get_filename_component(_ts_name ${_ts_file} NAME)
          set(_ts_lst_file "${CMAKE_CURRENT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/${_ts_name}_lst_file")
          set(_lst_file_srcs)
          foreach(_lst_file_src ${_my_sources})
              set(_lst_file_srcs "${_lst_file_src}\n${_lst_file_srcs}")
          endforeach()

         # get_directory_property(_inc_DIRS INCLUDE_DIRECTORIES)
         # foreach(_pro_include ${_inc_DIRS})
         #     get_filename_component(_abs_include "${_pro_include}" ABSOLUTE)
         #     set(_lst_file_srcs "-I${_pro_include}\n${_lst_file_srcs}")
         # endforeach()

          file(WRITE ${_ts_lst_file} "${_lst_file_srcs}")
        endif()
        add_custom_command(OUTPUT ${_ts_file}
            COMMAND ${Qt5_LUPDATE_EXECUTABLE}
            ARGS ${_lupdate_options} "@${_ts_lst_file}" -ts ${_ts_file}
            DEPENDS ${_my_sources}
            BYPRODUCTS ${_ts_lst_file} VERBATIM)
    endforeach()
    qt5_add_translation(${_qm_files} ${_my_tsfiles})
    set(${_qm_files} ${${_qm_files}} PARENT_SCOPE)
endfunction()

if(NOT QT_NO_CREATE_VERSIONLESS_FUNCTIONS)
    function(qt_create_translation _qm_files)
        if(QT_DEFAULT_MAJOR_VERSION EQUAL 5)
            qt5_create_translation("${_qm_files}" ${ARGN})
        elseif(QT_DEFAULT_MAJOR_VERSION EQUAL 6)
            qt6_create_translation("${_qm_files}" ${ARGN})
        endif()
        set("${_qm_files}" "${${_qm_files}}" PARENT_SCOPE)
    endfunction()
endif()

# 使用 visual leak detector(VLD)来探测内存泄漏
# 需要指定USE_VISUAL_LEAK_DETECTOR为ON， 并且环境变量PATH中存在Visual Leak Detector安装路径。VLD的使用方法为在main()所在cpp #include "vld.h"，编译运行Debug，然[...]
# 如果需要将记录输出到文件，在VLD安装路径编辑vld.ini，编辑"ReportTo = both"，默认输出文件为memory_leak_report.txt。
function(use_visual_leak_detector target)
    if(NOT USE_VISUAL_LEAK_DETECTOR OR NOT CMAKE_BUILD_TYPE STREQUAL "Debug")
        return()
    endif ()
    if(DEFINED ENV{PATH})
        set(path_list )
        if(WIN32)
            string(REPLACE ";" ";" path_list "$ENV{PATH}")
        endif ()
        set(VLD_PATH "")
        set(VLD_FOUND FALSE)
        foreach(path IN LISTS path_list)
            # 检查路径是否包含 "Visual Leak Detector" 且以 "Win64" 结尾
            if(path MATCHES "Visual Leak Detector" AND path MATCHES "Win64$")
                set(VLD_PATH "${path}")
                set(VLD_FOUND TRUE)
                message(STATUS "Found Visual Leak Detector Win64 path: ${VLD_PATH}")
                break()
            endif()
        endforeach()

        if(VLD_FOUND)
            target_include_directories(${target} PRIVATE ${VLD_PATH}/../../include)
            target_link_libraries(${target} debug ${VLD_PATH}/../../lib/Win64/vld.lib)
        else ()
            message(ERROR "Not found Visual Leak Detector Win64 path but USE_VISUAL_LEAK_DETECTOR specified!")
        endif ()
    endif ()
endfunction()

# 检查 ThirdParty 文件夹，不存在则自动下载并解压
function(download_thirdparty)
    set(THIRDPARTY_DIR "${CMAKE_CURRENT_SOURCE_DIR}/ThirdParty")

    if(EXISTS "${THIRDPARTY_DIR}")
        message(STATUS "✓ ThirdParty 目录已存在，跳过下载")
        return()
    endif()

    set(THIRDPARTY_URL "https://github.com/YiCAX/YiCAD/releases/download/thirdparty/2026-05-12/ThirdParty.zip")
    set(THIRDPARTY_ZIP "${CMAKE_CURRENT_SOURCE_DIR}/ThirdParty.zip")

    message(STATUS "========================================")
    message(STATUS "正在下载 ThirdParty.zip")
    message(STATUS "URL: ${THIRDPARTY_URL}")
    message(STATUS "目标路径: ${THIRDPARTY_ZIP}")
    message(STATUS "CMake 版本: ${CMAKE_VERSION}")
    message(STATUS "系统: ${CMAKE_SYSTEM_NAME} ${CMAKE_SYSTEM_VERSION}")
    message(STATUS "========================================")
    
    # 重试机制：最多尝试 3 次
    set(MAX_RETRIES 3)
    set(RETRY_COUNT 0)
    set(DOWNLOAD_SUCCESS FALSE)
    
    while(RETRY_COUNT LESS MAX_RETRIES AND NOT DOWNLOAD_SUCCESS)
        if(RETRY_COUNT GREATER 0)
            message(STATUS "")
            message(STATUS "第 ${RETRY_COUNT}/${MAX_RETRIES} 次重试...")
            file(REMOVE "${THIRDPARTY_ZIP}")
        endif()
        
        # 增加超时时间到 1800 秒（30分钟），并启用详细日志
        message(STATUS "[${RETRY_COUNT}] 开始下载... (超时: 1800秒)")
        file(DOWNLOAD "${THIRDPARTY_URL}" "${THIRDPARTY_ZIP}"
             STATUS DOWNLOAD_STATUS
             TIMEOUT 1800
             SHOW_PROGRESS
             LOG DOWNLOAD_LOG)

        list(GET DOWNLOAD_STATUS 0 STATUS_CODE)
        list(GET DOWNLOAD_STATUS 1 STATUS_MESSAGE)

        if(STATUS_CODE EQUAL 0)
            # 检查文件大小
            if(EXISTS "${THIRDPARTY_ZIP}")
                file(SIZE "${THIRDPARTY_ZIP}" ZIP_SIZE)
                message(STATUS "✓ 下载完成 (文件大小: ${ZIP_SIZE} bytes)")
                set(DOWNLOAD_SUCCESS TRUE)
            else()
                message(STATUS "✗ 文件不存在，下载失败")
                math(EXPR RETRY_COUNT "${RETRY_COUNT} + 1")
            endif()
        else()
            message(STATUS "✗ 下载失败 (错误代码: ${STATUS_CODE})")
            message(STATUS "错误信息: ${STATUS_MESSAGE}")
            
            # 打印网络诊断信息
            if(WIN32)
                message(STATUS "提示: 在 Windows CI 环境中，可能存在网络连接问题")
            endif()
            
            math(EXPR RETRY_COUNT "${RETRY_COUNT} + 1")
            if(RETRY_COUNT LESS MAX_RETRIES)
                message(STATUS "将在 5 秒后进行第 ${RETRY_COUNT} 次重试...")
            endif()
        endif()
    endwhile()

    if(NOT DOWNLOAD_SUCCESS)
        file(REMOVE "${THIRDPARTY_ZIP}")
        message(FATAL_ERROR "")
        message(FATAL_ERROR "========== 下载失败 ==========")
        message(FATAL_ERROR "已尝试 ${MAX_RETRIES} 次，仍无法下载 ThirdParty.zip")
        message(FATAL_ERROR "")
        message(FATAL_ERROR "可能原因:")
        message(FATAL_ERROR "  1. 网络连接不稳定（CI 环境网络问题）")
        message(FATAL_ERROR "  2. GitHub Release 资源暂时不可用")
        message(FATAL_ERROR "  3. 防火墙/代理阻止下载")
        message(FATAL_ERROR "")
        message(FATAL_ERROR "解决方案:")
        message(FATAL_ERROR "  A. 手动下载并放置:")
        message(FATAL_ERROR "     URL: ${THIRDPARTY_URL}")
        message(FATAL_ERROR "     解压到: ${THIRDPARTY_DIR}")
        message(FATAL_ERROR "  B. 检查网络连接")
        message(FATAL_ERROR "  C. 使用 curl 或 wget 下载:")
        message(FATAL_ERROR "     curl -L -o ThirdParty.zip ${THIRDPARTY_URL}")
        message(FATAL_ERROR "=============================")
    endif()

    message(STATUS "正在解压 ThirdParty.zip...")
    file(ARCHIVE_EXTRACT INPUT "${THIRDPARTY_ZIP}"
         DESTINATION "${CMAKE_CURRENT_SOURCE_DIR}")

    if(EXISTS "${THIRDPARTY_DIR}")
        message(STATUS "✓ ThirdParty 解压完成")
        file(REMOVE "${THIRDPARTY_ZIP}")
    else()
        message(FATAL_ERROR "✗ 解压失败: ${THIRDPARTY_DIR} 不存在")
    endif()
endfunction()
