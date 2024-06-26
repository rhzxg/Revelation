cmake_minimum_required(VERSION 3.14)

project(Revelation)

find_package(Qt6 COMPONENTS Widgets REQUIRED)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(Revelation 
               main.cpp
               RevelationInterface.cpp
               RevelationInterface.h

               RevelationMainWindow.cpp
               RevelationMainWindow.h
               RevelationSplash.cpp
               RevelationSplash.h
               RevelationSplash.ui
               RevelationSidebar.cpp
               RevelationSidebar.h
               RevelationSidebar.ui
               RevelationBottomBar.cpp
               RevelationBottomBar.h
               RevelationBottomBar.ui
               Revelation.cpp
               Revelation.h
               Revelation.ui
               RevelationListView.cpp
               RevelationListView.h
)

if(MSVC)
set_property(TARGET Revelation PROPERTY RUNTIME_OUTPUT_DIRECTORY "$<1:${REVELATION_OUTPUT_DIR}/${CMAKE_BUILD_TYPE}>") 
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} /Od")
endif()

target_link_libraries(${PROJECT_NAME} Qt6::Widgets)

target_include_directories(Revelation PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../Include)

