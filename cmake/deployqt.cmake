find_package(Qt5Core REQUIRED)

# get absolute path to qmake, then use it to find windeployqt executable

get_target_property(_qmake_executable Qt5::qmake IMPORTED_LOCATION)
get_filename_component(_qt_bin_dir "${_qmake_executable}" DIRECTORY)

function(windeployqt target)

    # POST_BUILD step
    # - after build, we have a bin/lib for analyzing qt dependencies
    # - we run windeployqt on target and deploy Qt libs

    string(TOLOWER "${CMAKE_BUILD_TYPE}" CMAKE_BUILD_TYPE_LOWER)
    add_custom_command(TARGET ${target} POST_BUILD
        COMMAND "${_qt_bin_dir}/windeployqt.exe"         
                --verbose 1
                --${CMAKE_BUILD_TYPE_LOWER}
                \"$<TARGET_FILE:${target}>\"
        COMMENT "Deploying Qt libraries using windeployqt for compilation target '${target}' ..."
    )
endfunction()