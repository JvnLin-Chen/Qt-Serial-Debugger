cmake_minimum_required(VERSION 3.16)

set(CMAKE_PREFIX_PATH "D:/QT6.12/6.12.0/mingw_64")

project(sscom_test VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 修改 1：在 COMPONENTS 后面加上 SerialPort
find_package(Qt6 REQUIRED COMPONENTS Widgets SerialPort)

qt_standard_project_setup()

qt_add_executable(sscom_test
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
)

# 修改 2：在链接库里加上 SerialPort
target_link_libraries(sscom_test PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::SerialPort)


set_target_properties(sscom_test PROPERTIES
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS sscom_test
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
