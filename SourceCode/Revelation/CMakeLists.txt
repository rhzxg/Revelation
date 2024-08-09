cmake_minimum_required(VERSION 3.14)

project(Revelation)

find_package(Qt6 COMPONENTS Widgets REQUIRED)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(${PROJECT_NAME} 
               main.cpp
               RevelationInterface.cpp
               RevelationInterface.h

               CommonWidgetInterface.cpp
               CommonWidgetInterface.h

               RevelationMainWindow.cpp
               RevelationMainWindow.h
               RevelationSplash.cpp
               RevelationSplash.h
               RevelationSidebar.h
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

if(WIN32)
    target_sources(${PROJECT_NAME} PRIVATE resource/Revelation.rc)
endif()

if(MSVC)
set_property(TARGET ${PROJECT_NAME} PROPERTY FOLDER Revelation)
set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY_DEBUG "$<1:${REVELATION_OUTPUT_DIR}/Debug>"
    RUNTIME_OUTPUT_DIRECTORY_RELEASE "$<1:${REVELATION_OUTPUT_DIR}/Release>"
    RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO "$<1:${REVELATION_OUTPUT_DIR}/RelWithDebInfo>"
    LIBRARY_OUTPUT_DIRECTORY_DEBUG "$<1:${REVELATION_OUTPUT_DIR}/Debug>"
    LIBRARY_OUTPUT_DIRECTORY_RELEASE "$<1:${REVELATION_OUTPUT_DIR}/Release>"
    LIBRARY_OUTPUT_DIRECTORY_RELWITHDEBINFO "$<1:${REVELATION_OUTPUT_DIR}/RelWithDebInfo>"
    ARCHIVE_OUTPUT_DIRECTORY_DEBUG "$<1:${REVELATION_OUTPUT_DIR}/Debug>"
    ARCHIVE_OUTPUT_DIRECTORY_RELEASE "$<1:${REVELATION_OUTPUT_DIR}/Release>"
    ARCHIVE_OUTPUT_DIRECTORY_RELWITHDEBINFO "$<1:${REVELATION_OUTPUT_DIR}/RelWithDebInfo>"
)
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} /Od")
add_definitions(-DNOMINMAX)
endif()

target_link_libraries(${PROJECT_NAME} Qt6::Widgets)
target_link_libraries(${PROJECT_NAME} FluUtils)
target_link_libraries(${PROJECT_NAME} FluControls)

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../Include)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../FluentUI/FluControls)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../FluentUI/FluUtils)

if (WIN32)
set_target_properties(${PROJECT_NAME} PROPERTIES WIN32_EXECUTABLE ON)
endif()