cmake_minimum_required(VERSION 3.14)

project(Revelation)

find_package(Qt6 COMPONENTS Widgets REQUIRED)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)

add_executable(Revelation 
               main.cpp
               RevelationInterface.cpp
               RevelationInterface.h

               FramelessWindow.cpp
               FramelessWindow.h
               RevelationSplash.cpp
               RevelationSplash.h
               RevelationSplash.ui
               Revelation.cpp
               Revelation.h
               Revelation.ui
)

set_property(TARGET Revelation PROPERTY RUNTIME_OUTPUT_DIRECTORY "$<1:${REVELATION_OUTPUT_DIR}/${CMAKE_BUILD_TYPE}>") 
target_include_directories(Revelation PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../Include)

target_link_libraries(${PROJECT_NAME} Qt6::Widgets)
