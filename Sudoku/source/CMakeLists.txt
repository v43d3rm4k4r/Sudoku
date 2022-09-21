cmake_minimum_required(VERSION 3.5)

project(Sudoku VERSION 1.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


if (NOT DEFINED SUDOKU_APP_VERSION)
    add_definitions(-DSUDOKU_APP_VERSION=${CMAKE_PROJECT_VERSION})
endif ()

option(DEBUG "Enable debug output" ON)
if(DEBUG)
    add_definitions(-DDEBUG)
endif()

find_package(QT NAMES Qt5 REQUIRED COMPONENTS Widgets LinguistTools)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets LinguistTools)

include_directories(${CMAKE_SOURCE_DIR}/includes)

set(TS_FILES languages/Sudoku_ru_RU.ts)

set(PROJECT_SOURCES
    main.cpp
    MainWindow.cpp
    FieldModel.cpp
    CellEventFilter.cpp
    ${TS_FILES}
)

set(PROJECT_HEADERS
    includes/MainWindow.h
    includes/MainWindow.ui
    includes/FieldModel.h
    includes/CellEventFilter.h
    includes/Types.h
)

set(find_result "")
find_path(find_result NAMES "resources.qrc" PATHS resources/resources)
message(STATUS "result=${find_result}")

set(RESOURCES
    resources/resources.qrc
    )

#qt5_add_resources(RESOURCES ${RESOURCES})


if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(${PROJECT_NAME}
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        ${PROJECT_HEADERS}
        ${RESOURCES}
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET Sudoku APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation

    qt_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
else()
    if(ANDROID)
        add_library(${PROJECT_NAME} SHARED
            ${PROJECT_SOURCES}
            ${PROJECT_HEADERS}
            ${RESOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(${PROJECT_NAME}
            ${PROJECT_SOURCES}
            ${PROJECT_HEADERS}
            ${RESOURCES}
        )
    endif()

    qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR}/languages ${TS_FILES}) # added "/languages"

endif()

target_link_libraries(${PROJECT_NAME} PRIVATE # todo: TRY TO LINK QT`S LIBS STATIC
    Qt5::Core
    Qt${QT_VERSION_MAJOR}::Widgets
)

set_target_properties(${PROJECT_NAME} PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(${PROJECT_NAME})
endif()
