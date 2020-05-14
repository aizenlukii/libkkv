cmake_minimum_required(VERSION 3.15)
project(kkv)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(NOT DEFINED KKV_BUILD_EXAMPLE_TEST)
    option(KKV_BUILD_EXAMPLE_TEST "Build test examples" OFF)
endif()

set(kkv_INCLUDE_DIR ${CMAKE_CURRENT_LIST_DIR}/include)

set(kkv_HEADERS
    include/kkv/api.h
    include/kkv/db.h
    include/kkv/logger.h
    include/kkv/options.h
    include/kkv/slice.h
    include/kkv/status.h
    )

set(kkv_SOURCES
    src/options.cc
    )

set(kkv_common_HEADERS
    src/common/types.h
    )

set(kkv_core_HEADERS
    src/core/configuration.h
    src/core/core.h
    src/core/streamer.h
    )

set(kkv_core_SOURCES
    src/core/configuration.cc
    src/core/core.cc
    src/core/streamer.cc
    )

set(kkv_utility_HEADERS
    src/utility/filesystem.h
    src/utility/utils.h
    )

set(kkv_utility_SOURCES
    src/utility/filesystem.cc
    )

set(kkv_all_HEADERS
    ${kkv_HEADERS}
    ${kkv_common_HEADERS}
    ${kkv_core_HEADERS}
    ${kkv_utility_HEADERS}
    )

set(kkv_all_SOURCES
    ${kkv_SOURCES}
    ${kkv_core_SOURCES}
    ${kkv_utility_SOURCES}
    )

source_group(kkv                    FILES ${kkv_HEADERS})
source_group(kkv\\common            FILES ${kkv_common_HEADERS})
source_group(kkv\\core              FILES ${kkv_core_HEADERS})
source_group(kkv\\core              FILES ${kkv_core_SOURCES})
source_group(kkv\\utility           FILES ${kkv_utility_HEADERS})
source_group(kkv\\utility           FILES ${kkv_utility_SOURCES})

set(kkv_library_TARGET "kkv")
add_library(${kkv_library_TARGET} STATIC ${kkv_all_HEADERS} ${kkv_all_SOURCES})
target_include_directories(${kkv_library_TARGET} PUBLIC ${kkv_INCLUDE_DIR})
target_include_directories(${kkv_library_TARGET} PRIVATE "${CMAKE_CURRENT_LIST_DIR}/src")

set(spdlog_DIR "${CMAKE_CURRENT_SOURCE_DIR}/third_party/spdlog")
add_subdirectory(${spdlog_DIR})
target_link_libraries(${kkv_library_TARGET} spdlog)

if ((CMAKE_CXX_COMPILER_ID MATCHES "Clang") OR (CMAKE_CXX_COMPILER_ID STREQUAL "GNU"))
    target_compile_options(${kkv_library_TARGET} PRIVATE "-Wunused-parameter")
endif()

if(KKV_BUILD_EXAMPLE_TEST)
    set(kkv_example_test_HEADERS
        example/test/test.h
        )

    set(kkv_example_test_SOURCES
        example/test/test.cc

        example/test/test_db.cc
        example/test/test_slice.cc
        )

    set(kkv_example_test_TARGET "test")

    source_group(kkv\\example\\test FILES ${kkv_example_test_SOURCES})
    source_group(kkv\\example\\test FILES ${kkv_example_test_HEADERS})

    add_executable(${kkv_example_test_TARGET} ${kkv_example_test_SOURCES} ${kkv_example_test_HEADERS})

    set_target_properties(${kkv_example_test_TARGET}
        PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/example/test"
        )

    target_include_directories(${kkv_example_test_TARGET} PRIVATE ${kkv_INCLUDE_DIR})
    target_link_libraries(${kkv_example_test_TARGET} PRIVATE ${kkv_library_TARGET})
endif()
