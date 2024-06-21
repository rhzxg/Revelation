cmake_minimum_required(VERSION 3.14)

project(DataPersistence)

add_library(DataPersistence SHARED 
			dllmain.cpp

			DataPersistenceInterface.cpp
			DataPersistenceInterface.h
)

set_property(TARGET DataPersistence PROPERTY RUNTIME_OUTPUT_DIRECTORY "$<1:${REVELATION_OUTPUT_DIR}/${CMAKE_BUILD_TYPE}/extensions>") 
target_include_directories(DataPersistence PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../Include)
