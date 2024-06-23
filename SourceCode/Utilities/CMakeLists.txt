cmake_minimum_required(VERSION 3.14)

project(Utility)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(Utility SHARED 
			dllmain.cpp
)

set_property(TARGET Utility PROPERTY RUNTIME_OUTPUT_DIRECTORY "$<1:${REVELATION_OUTPUT_DIR}/${CMAKE_BUILD_TYPE}/extensions/${PROJECT_NAME}>") 
target_include_directories(Utility PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../Include)
