cmake_minimum_required(VERSION 3.5)

include("${PROJECT_SOURCE_DIR}/extra/lib/cmake/auxiliary.cmake")

find_package("Boost" REQUIRED COMPONENTS "headers" "unit_test_framework")

cmake_language(CALL "_auxiliary::get_target_namespace" "_namespaces.this")
mark_as_advanced("_namespaces.this")

cmake_language(CALL "_auxiliary::get_target_namespace" "_namespaces.related" DIRECTORY "..")
mark_as_advanced("_namespaces.related")

add_executable("${_namespaces.this}-utils.log" "src/utils.log.cxx")

target_compile_features("${_namespaces.this}-utils.log" PRIVATE "cxx_std_20")

set_target_properties("${_namespaces.this}-utils.log" PROPERTIES
    OUTPUT_NAME "utils.log"
    CXX_STANDARD "20"
    CXX_STANDARD_REQUIRED TRUE
    POSITION_INDEPENDENT_CODE TRUE
)

target_link_libraries(
    "${_namespaces.this}-utils.log" PRIVATE
    "Boost::boost" "Boost::unit_test_framework"
    "${_namespaces.related}"
)

target_compile_definitions(
    "${_namespaces.this}-utils.log" PRIVATE
    "P5_LAMBDA__BASE_PATH=\"${PROJECT_SOURCE_DIR}\""
)

add_test(NAME "${_namespaces.this}-utils.log" COMMAND "utils.log")
