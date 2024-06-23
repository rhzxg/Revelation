cmake_minimum_required(VERSION 3.14)

project(DataPersistence)

find_package(SQLite3 REQUIRED)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(DataPersistence SHARED 
			dllmain.cpp

			DataPersistenceInterface.cpp
			DataPersistenceInterface.h
)

if(MSVC)
set_property(TARGET DataPersistence PROPERTY RUNTIME_OUTPUT_DIRECTORY "$<1:${REVELATION_OUTPUT_DIR}/${CMAKE_BUILD_TYPE}/extensions/${PROJECT_NAME}>") 
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} /Od")
endif()

target_link_libraries(${PROJECT_NAME} PRIVATE SQLite::SQLite3)

target_include_directories(DataPersistence PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../Include)
