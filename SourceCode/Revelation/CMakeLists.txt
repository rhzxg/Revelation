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
               RevelationSidebar.h
               RevelationLeftSidebar.cpp
               RevelationLeftSidebar.h
               RevelationRightSidebar.cpp
               RevelationRightSidebar.h
               RevelationBottomBar.cpp
               RevelationBottomBar.h
               Revelation.cpp
               Revelation.h
               RevelationListModel.cpp
               RevelationListModel.h
               RevelationListView.cpp
               RevelationListView.h
               RevelationListDelegate.cpp
               RevelationListDelegate.h
               RevelationListItem.cpp
               RevelationListItem.h
)

if(MSVC)
set_property(TARGET Revelation PROPERTY RUNTIME_OUTPUT_DIRECTORY "$<1:${REVELATION_OUTPUT_DIR}/${CMAKE_BUILD_TYPE}>") 
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} /Od")
add_definitions(-DNOMINMAX)
endif()

target_link_libraries(${PROJECT_NAME} Qt6::Widgets)

target_include_directories(Revelation PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../Include)
