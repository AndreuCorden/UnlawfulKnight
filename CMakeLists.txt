cmake_minimum_required(VERSION 3.16)
project(UnlawfulKnight LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build." FORCE)
endif()

set(CMAKE_PREFIX_PATH "C:/Qt/6.11.1/mingw_64") 

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets)

set(SOURCES
    resources.qrc
    src/main.cpp
    src/MainWindow.h
    src/MainWindow.cpp
    src/Intro-Screen/IntroScreen.h
    src/Intro-Screen/IntroScreen.cpp
    src/Intro-Screen/IntroScreenController.h
    src/Intro-Screen/IntroScreenController.cpp
    src/Intro-Screen/NameEntryWidget.h
    src/Intro-Screen/NameEntryWidget.cpp
)

# Standard console executable
add_executable(UnlawfulKnight ${SOURCES})

# Prevents WinMain/qMain linking issues on MinGW while keeping console logging available
target_link_options(UnlawfulKnight PRIVATE -mconsole)

target_link_libraries(UnlawfulKnight PRIVATE Qt6::Core Qt6::Gui Qt6::Widgets)
target_include_directories(UnlawfulKnight PRIVATE .)