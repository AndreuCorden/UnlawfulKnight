cmake_minimum_required(VERSION 3.16)
project(PeasantToKnight LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# Find Qt (Change Qt6 to Qt5 if you are on an older Qt installation)
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

add_executable(PeasantToKnight
    main.cpp
    GameWidget.h
    GameWidget.cpp
)

target_link_libraries(PeasantToKnight PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)