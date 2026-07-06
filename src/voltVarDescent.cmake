set(VOLT_VAR_DESCENT_NAME voltVarDescent)

file(GLOB VOLT_VAR_DESCENT_SOURCES  ${CMAKE_CURRENT_LIST_DIR}/src/*.cpp)
file(GLOB_RECURSE VOLT_VAR_DESCENT_INCS ${CMAKE_CURRENT_LIST_DIR}/src/*.h)
file(GLOB VOLT_VAR_DESCENT_INC_TD   ${NATID_SDK_INC}/td/*.h)
file(GLOB VOLT_VAR_DESCENT_INC_GUI  ${NATID_SDK_INC}/gui/*.h)

set(VOLT_VAR_DESCENT_PLIST  ${CMAKE_CURRENT_LIST_DIR}/res/appIcon/AppIcon.plist)
if(WIN32)
	set(VOLT_VAR_DESCENT_APP_ICON ${CMAKE_CURRENT_LIST_DIR}/res/appIcon/winAppIcon.rc)
else()
	set(VOLT_VAR_DESCENT_APP_ICON ${CMAKE_CURRENT_LIST_DIR}/res/appIcon/winAppIcon.cpp)
endif()

# dss_capi paths
if(WIN32)
    set(DSS_CAPI_ROOT ${HOME_ROOT}/dss_capi/win_x64)
    set(DSS_CAPI_LIB_DIR ${DSS_CAPI_ROOT}/lib/win_x64)
    set(DSS_CAPI_LIB ${DSS_CAPI_LIB_DIR}/dss_capi.lib)
    set(DSS_CAPI_DYLIBS
        ${DSS_CAPI_LIB_DIR}/dss_capi.dll
        ${DSS_CAPI_LIB_DIR}/klusolvex.dll)
else()
    set(DSS_CAPI_ROOT ${HOME_ROOT}/dss_capi)
    if(CMAKE_SYSTEM_PROCESSOR MATCHES "arm64")
        set(DSS_CAPI_LIB_DIR ${DSS_CAPI_ROOT}/lib/darwin_arm64)
    else()
        set(DSS_CAPI_LIB_DIR ${DSS_CAPI_ROOT}/lib/darwin_x64)
    endif()
    set(DSS_CAPI_LIB ${DSS_CAPI_LIB_DIR}/libdss_capi.dylib)
    set(DSS_CAPI_DYLIBS
        ${DSS_CAPI_LIB_DIR}/libdss_capi.dylib
        ${DSS_CAPI_LIB_DIR}/libklusolvex.dylib)

    # Fix install names so dylibs use @rpath (run once at configure time)
    execute_process(COMMAND install_name_tool -id @rpath/libdss_capi.dylib
        ${DSS_CAPI_LIB_DIR}/libdss_capi.dylib)
    execute_process(COMMAND install_name_tool
        -change @loader_path/./libklusolvex.dylib @rpath/libklusolvex.dylib
        ${DSS_CAPI_LIB_DIR}/libdss_capi.dylib)
    execute_process(COMMAND install_name_tool -id @rpath/libklusolvex.dylib
        ${DSS_CAPI_LIB_DIR}/libklusolvex.dylib)
    execute_process(COMMAND codesign --force --sign -
        ${DSS_CAPI_LIB_DIR}/libdss_capi.dylib)
    execute_process(COMMAND codesign --force --sign -
        ${DSS_CAPI_LIB_DIR}/libklusolvex.dylib)
endif()

add_executable(${VOLT_VAR_DESCENT_NAME}
	${VOLT_VAR_DESCENT_INCS}
	${VOLT_VAR_DESCENT_SOURCES}
	${VOLT_VAR_DESCENT_INC_TD}
	${VOLT_VAR_DESCENT_INC_GUI}
	${VOLT_VAR_DESCENT_APP_ICON})

source_group("inc"     FILES ${VOLT_VAR_DESCENT_INCS})
source_group("inc\\td" FILES ${VOLT_VAR_DESCENT_INC_TD})
source_group("inc\\gui" FILES ${VOLT_VAR_DESCENT_INC_GUI})
source_group("src"     FILES ${VOLT_VAR_DESCENT_SOURCES})

target_include_directories(${VOLT_VAR_DESCENT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/src
    ${DSS_CAPI_ROOT}/include)

target_link_libraries(${VOLT_VAR_DESCENT_NAME}
	debug ${MU_LIB_DEBUG} optimized ${MU_LIB_RELEASE}
	debug ${NATGUI_LIB_DEBUG} optimized ${NATGUI_LIB_RELEASE}
	debug ${NATPLOT_LIB_DEBUG} optimized ${NATPLOT_LIB_RELEASE}
	debug ${MATRIX_LIB_DEBUG} optimized ${MATRIX_LIB_RELEASE}
	${DSS_CAPI_LIB})

# Copy dss_capi dylibs into the app bundle, fix install names, sign
foreach(DYLIB ${DSS_CAPI_DYLIBS})
    get_filename_component(DYLIB_NAME ${DYLIB} NAME)
    add_custom_command(TARGET ${VOLT_VAR_DESCENT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory
            $<TARGET_BUNDLE_CONTENT_DIR:${VOLT_VAR_DESCENT_NAME}>/Frameworks
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${DYLIB}
            $<TARGET_BUNDLE_CONTENT_DIR:${VOLT_VAR_DESCENT_NAME}>/Frameworks/
        COMMAND install_name_tool -id @rpath/${DYLIB_NAME}
            $<TARGET_BUNDLE_CONTENT_DIR:${VOLT_VAR_DESCENT_NAME}>/Frameworks/${DYLIB_NAME}
        COMMAND codesign --force --sign -
            $<TARGET_BUNDLE_CONTENT_DIR:${VOLT_VAR_DESCENT_NAME}>/Frameworks/${DYLIB_NAME}
    )
endforeach()

# Fix cross-dependency: libdss_capi references libklusolvex
add_custom_command(TARGET ${VOLT_VAR_DESCENT_NAME} POST_BUILD
    COMMAND install_name_tool
        -change @loader_path/./libklusolvex.dylib @rpath/libklusolvex.dylib
        $<TARGET_BUNDLE_CONTENT_DIR:${VOLT_VAR_DESCENT_NAME}>/Frameworks/libdss_capi.dylib
    COMMAND codesign --force --sign -
        $<TARGET_BUNDLE_CONTENT_DIR:${VOLT_VAR_DESCENT_NAME}>/Frameworks/libdss_capi.dylib
)

# Add Frameworks to rpath so the binary finds the dylibs
set_target_properties(${VOLT_VAR_DESCENT_NAME} PROPERTIES
    XCODE_ATTRIBUTE_LD_RUNPATH_SEARCH_PATHS "@executable_path/../Frameworks $(inherited)"
)

setTargetPropertiesForGUIApp(${VOLT_VAR_DESCENT_NAME} ${VOLT_VAR_DESCENT_PLIST})

setAppIcon(${VOLT_VAR_DESCENT_NAME} ${CMAKE_CURRENT_LIST_DIR})

setIDEPropertiesForGUIExecutable(${VOLT_VAR_DESCENT_NAME} ${CMAKE_CURRENT_LIST_DIR})

setPlatformDLLPath(${VOLT_VAR_DESCENT_NAME})
