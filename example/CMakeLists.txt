cmake_minimum_required(VERSION 3.19)
project(ElaWidgetToolsExample LANGUAGES CXX)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets)
find_package(Qt6 REQUIRED COMPONENTS LinguistTools)
find_package(ElaWidgetTools REQUIRED PATHS "${CMAKE_CURRENT_SOURCE_DIR}/../install/lib")


qt_standard_project_setup()

file(GLOB_RECURSE SOURCES
    "main.cpp"
    "mainwindow.cpp"
    "mainwindow.h"
    "ExamplePage/*.cpp"
    "ExamplePage/*.h"
    "ModelView/*.cpp"
    "ModelView/*.h"
)
qt_add_resources(SOURCES
    ElaWidgetToolsExample.qrc)

message(${SOURCES})

qt_add_executable(${PROJECT_NAME}
    WIN32 MACOSX_BUNDLE
    ${SOURCES}
)

# add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/ElaWidgetTools)

target_include_directories(${PROJECT_NAME}
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/ExamplePage
    ${CMAKE_CURRENT_SOURCE_DIR}/ModelView
    ${CMAKE_CURRENT_SOURCE_DIR}/../install/include
)

target_link_directories(${PROJECT_NAME}
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/../install/lib
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
        Qt::Core
        Qt::Widgets
    ElaWidgetTools
)

# include(GNUInstallDirs)

# install(TARGETS untitled
#     BUNDLE  DESTINATION .
#     RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
#     LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
# )

# qt_generate_deploy_app_script(
#     TARGET untitled
#     OUTPUT_SCRIPT deploy_script
#     NO_UNSUPPORTED_PLATFORM_ERROR
# )
# install(SCRIPT ${deploy_script})
