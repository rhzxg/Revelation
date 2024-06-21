cmake_minimum_required(VERSION 3.14)

project(Revelation)

file(GLOB SUBDIRS RELATIVE ${CMAKE_SOURCE_DIR} *)
foreach(SUBDIR ${SUBDIRS})
    if(IS_DIRECTORY ${CMAKE_SOURCE_DIR}/${SUBDIR})
        if(${SUBDIR} STREQUAL "Include")
            include_directories(${CMAKE_SOURCE_DIR}/${SUBDIR})
        elseif(EXISTS ${CMAKE_SOURCE_DIR}/${SUBDIR}/CMakeLists.txt)
            message("-- Add subdirectory: ${SUBDIR}")
            add_subdirectory(${SUBDIR})
        endif()
    endif()
endforeach()

