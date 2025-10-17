cmake_minimum_required(VERSION 3.14)

project(DataPersistence)

find_package(SQLite3 REQUIRED)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SOURCES
    dllmain.cpp
    DataPersistenceInterface.cpp
    DataPersistenceInterface.h

    TaskSerializer/TaskSerializer.cpp
    TaskSerializer/TaskSerializer.h

    RoutineSerializer/RoutineSerializer.cpp
    RoutineSerializer/RoutineSerializer.h

    SQLite/SQLiteDatabase.h
    SQLite/SQLiteDatabase.cpp
    SQLite/SQLiteLink.h
    SQLite/SQLiteLink.cpp
)

foreach(source IN LISTS SOURCES)
    get_filename_component(source_path "${source}" PATH)
    string(REPLACE "/" "\\" source_path_group "${source_path}")
    source_group("${source_path_group}" FILES "${source}")
endforeach()

add_library(${PROJECT_NAME} SHARED ${SOURCES})

if(MSVC)
set_property(TARGET ${PROJECT_NAME} PROPERTY FOLDER Revelation)
set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY_DEBUG "$<1:${REVELATION_OUTPUT_DIR}/Debug/extensions/${PROJECT_NAME}>"
    RUNTIME_OUTPUT_DIRECTORY_RELEASE "$<1:${REVELATION_OUTPUT_DIR}/Release/extensions/${PROJECT_NAME}>"
    RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO "$<1:${REVELATION_OUTPUT_DIR}/RelWithDebInfo/extensions/${PROJECT_NAME}>"
    LIBRARY_OUTPUT_DIRECTORY_DEBUG "$<1:${REVELATION_OUTPUT_DIR}/Debug/extensions/${PROJECT_NAME}>"
    LIBRARY_OUTPUT_DIRECTORY_RELEASE "$<1:${REVELATION_OUTPUT_DIR}/Release/extensions/${PROJECT_NAME}>"
    LIBRARY_OUTPUT_DIRECTORY_RELWITHDEBINFO "$<1:${REVELATION_OUTPUT_DIR}/RelWithDebInfo/extensions/${PROJECT_NAME}>"
    ARCHIVE_OUTPUT_DIRECTORY_DEBUG "$<1:${REVELATION_OUTPUT_DIR}/Debug/extensions/${PROJECT_NAME}>"
    ARCHIVE_OUTPUT_DIRECTORY_RELEASE "$<1:${REVELATION_OUTPUT_DIR}/Release/extensions/${PROJECT_NAME}>"
    ARCHIVE_OUTPUT_DIRECTORY_RELWITHDEBINFO "$<1:${REVELATION_OUTPUT_DIR}/RelWithDebInfo/extensions/${PROJECT_NAME}>"
)
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} /Od")
endif()

target_link_libraries(${PROJECT_NAME} PRIVATE SQLite::SQLite3)

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../Include)
