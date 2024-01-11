cmake_minimum_required(VERSION 3.5)

include("${PROJECT_SOURCE_DIR}/extra/lib/cmake/auxiliary.cmake")

find_package("Boost" REQUIRED COMPONENTS "headers")

cmake_language(CALL "_auxiliary::get_target_namespace" "_target")
mark_as_advanced("_target")

add_library("${_target}" STATIC)
add_library("${PROJECT_NAME}" ALIAS "${_target}")

file(
    GLOB_RECURSE "_sources"
    LIST_DIRECTORIES FALSE
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cxx"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/*.hxx"
)
mark_as_advanced("_sources")
target_sources("${_target}" PRIVATE ${_sources})

set_target_properties("${_target}" PROPERTIES
    CXX_STANDARD "20"
    CXX_STANDARD_REQUIRED TRUE
    POSITION_INDEPENDENT_CODE TRUE
)

target_compile_features("${_target}" PUBLIC "cxx_std_20")
target_include_directories("${_target}" PUBLIC "include")

target_link_libraries("${_target}" PRIVATE "Boost::boost")

include("GNUInstallDirs")
install(TARGETS "${_target}")

if("${CMAKE_SOURCE_DIR}" STREQUAL "${PROJECT_SOURCE_DIR}")
    add_subdirectory("tests")
endif("${CMAKE_SOURCE_DIR}" STREQUAL "${PROJECT_SOURCE_DIR}")
