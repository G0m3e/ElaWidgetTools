cmake_minimum_required(VERSION 3.5)

project(ElaWidgetTools VERSION 2.0.0 LANGUAGES CXX)

set(EXPORT_NAME "ElaWidgetTools")
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

qt_standard_project_setup()
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")

SET(CMAKE_INSTALL_PREFIX ${CMAKE_SOURCE_DIR}/install CACHE PATH "Installation path" FORCE)
option(ELAWIDGETTOOLS_BUILD_STATIC_LIB "Build static library." ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

FILE(GLOB ORIGIN *.h *.cpp)
FILE(GLOB INCLUDE include/*.h)
FILE(GLOB PRIVATE private/*.h private/*.cpp)
FILE(GLOB DEVELOPER DeveloperComponents/*.h DeveloperComponents/*.cpp)

source_group(include FILES ${INCLUDE})
source_group(private FILES ${PRIVATE})
source_group(DeveloperComponents FILES ${DEVELOPER})

set(PROJECT_SOURCES
    ${ORIGIN}
    ${INCLUDE}
    ${PRIVATE}
    ${DEVELOPER}
)

message("${CMAKE_CURRENT_SOURCE_DIR}/include/ElaWidgetTools.qrc")
qt_add_resources(PROJECT_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/include/ElaWidgetTools.qrc)
message(${PROJECT_SOURCES})

if (ELAWIDGETTOOLS_BUILD_STATIC_LIB)
    set(LIB_TYPE "STATIC")
else ()
    add_definitions(-DELAWIDGETTOOLS_LIBRARY)
    set(LIB_TYPE "SHARED"
        DeveloperComponents/ElaKeyBinderContainer.cpp
        DeveloperComponents/ElaKeyBinderContainer.h)
endif ()

if (${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_library(${EXPORT_NAME} ${LIB_TYPE}
        ${PROJECT_SOURCES}
    )
else ()
    add_library(${EXPORT_NAME} ${LIB_TYPE}
        ${PROJECT_SOURCES}
    )
endif ()

FILE(GLOB EXPORT_HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/include/*.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/*.qrc
)
target_include_directories(${EXPORT_NAME} PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/private>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/DeveloperComponents>
    $<INSTALL_INTERFACE:include>
)

# if (${QT_VERSION_MAJOR} GREATER_EQUAL 6)
#     #遍历所有资源文件
#     file(GLOB_RECURSE RES_PATHS *.png *.jpg *.svg *.ico *.ttf *.webp *.js)
#     foreach (filepath ${RES_PATHS})
#         string(REPLACE "${CMAKE_CURRENT_SOURCE_DIR}/" "" filename ${filepath})
#         list(APPEND resource_files ${filename})
#     endforeach (filepath)
#     qt_add_resources(${EXPORT_NAME} "ElaWidgetTools"
#         RESOURCES PREFIX "/"
#         FILES
#         ${resource_files}
#     )
# endif ()

if (MINGW)
    set_target_properties(${EXPORT_NAME} PROPERTIES PREFIX "")
endif ()
if (MSVC)
    set_target_properties(${EXPORT_NAME} PROPERTIES DEBUG_POSTFIX "d")
endif ()

if (WIN32)
    target_link_libraries(${EXPORT_NAME} PUBLIC
        Qt${QT_VERSION_MAJOR}::Widgets
        D3D11
        DXGI
    )
else ()
    target_link_libraries(${EXPORT_NAME} PUBLIC
        Qt${QT_VERSION_MAJOR}::Widgets
    )
endif ()


# file(GLOB_RECURSE INCLUDE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/*.h")
install(FILES ${EXPORT_HEADERS} DESTINATION include)

install(TARGETS ${PROJECT_NAME}
  EXPORT ${PROJECT_NAME}Targets
  LIBRARY DESTINATION "lib"
  ARCHIVE DESTINATION "lib"
)

install(EXPORT ${PROJECT_NAME}Targets
  FILE ${PROJECT_NAME}Targets.cmake
  NAMESPACE ${PROJECT_NAME}::
  DESTINATION "cmake")

configure_file("${CMAKE_CURRENT_SOURCE_DIR}/../cmake/config.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
    @ONLY)

install(FILES "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake" DESTINATION "cmake")
