cmake_minimum_required(VERSION 3.16)

project(GeoVizVisualizer3 VERSION 0.1 LANGUAGES CXX)

set(QT6_TOOLCHAIN_PATH "C:/Qt/6.8.1/msvc2022_64")
set(CMAKE_PREFIX_PATH "${QT6_TOOLCHAIN_PATH}/lib/cmake")

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR}
    REQUIRED COMPONENTS
    Widgets
    OpenGLWidgets
    OpenGL)

set(PROJECT_SOURCES
        main.cpp
        resources.qrc
        MainWindow.cpp
        MainWindow.h
        MainWindow.ui
        Scene.cpp
        Scene.h
        Settings.cpp
        Settings.h
        materials/Material.cpp
        materials/Material.h
        materials/Phong.cpp
        materials/Phong.h
        cameras/Camera.cpp
        cameras/Camera.h
        cameras/PerspectiveCamera.cpp
        cameras/PerspectiveCamera.h
        models/Plane.cpp
        models/Plane.h
        models/Model.cpp
        models/Model.h
        cameras/OrthographicCamera.cpp
        cameras/OrthographicCamera.h
        lights/Light.cpp
        lights/Light.h
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(GeoVizVisualizer3
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        Viewport.h Viewport.cpp
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET GeoVizVisualizer3 APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(GeoVizVisualizer3 SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(GeoVizVisualizer3
            ${PROJECT_SOURCES}
        )
    endif()
endif()

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(GeoVizVisualizer3
    PRIVATE
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::OpenGLWidgets
    Qt${QT_VERSION_MAJOR}::OpenGL
)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.GeoVizVisualizer3)
endif()
set_target_properties(GeoVizVisualizer3 PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS GeoVizVisualizer3
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(GeoVizVisualizer3)
endif()

if (WIN32)
    add_custom_command(TARGET GeoVizVisualizer3 POST_BUILD
            COMMAND ${QT6_TOOLCHAIN_PATH}/bin/windeployqt6.exe --plugindir plugins ${CMAKE_CURRENT_BINARY_DIR}
    )
endif()
