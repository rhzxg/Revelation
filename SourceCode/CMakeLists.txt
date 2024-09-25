cmake_minimum_required(VERSION 3.14)

project(Revelation)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

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

add_custom_target(ResourceCopy
    COMMAND ${CMAKE_COMMAND} -E copy_directory_if_different
    ${CMAKE_CURRENT_SOURCE_DIR}/../resources
    ${PROJECT_BINARY_DIR}/Revelation/resources

    COMMAND ${CMAKE_COMMAND} -E copy_directory_if_different
    ${CMAKE_CURRENT_SOURCE_DIR}/../resources
    ${REVELATION_OUTPUT_DIR}/${CMAKE_BUILD_TYPE}/resources
)