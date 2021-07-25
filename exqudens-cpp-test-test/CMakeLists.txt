cmake_minimum_required(VERSION 3.20)
project("${CMAKE_PROJECT_NAME}-test" VERSION "${CMAKE_PROJECT_VERSION}")

message("project: '${PROJECT_NAME}-${PROJECT_VERSION}'")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY                "${PROJECT_BINARY_DIR}/bin")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE        "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_MINSIZEREL     "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG          "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY                "${PROJECT_BINARY_DIR}/lib")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE        "${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELWITHDEBINFO "${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_MINSIZEREL     "${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG          "${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}")

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY                "${PROJECT_BINARY_DIR}/lib")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE        "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELWITHDEBINFO "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_MINSIZEREL     "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG          "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}")

add_library("${PROJECT_NAME}-lib" STATIC
    "src/test/cpp/exqudens/other/OtherTests.hpp"
    "src/test/cpp/exqudens/other/OtherTests.cpp"
)
target_include_directories("${PROJECT_NAME}-lib" PUBLIC
    "src/test/cpp"
)
target_link_libraries("${PROJECT_NAME}-lib" PUBLIC
    "${CMAKE_PROJECT_NAME}-lib"
)

add_executable("${PROJECT_NAME}-executable" "src/test/cpp/main.cpp")
target_link_libraries("${PROJECT_NAME}-executable" PUBLIC
    "${PROJECT_NAME}-lib"
)
if("${BUILD_SHARED_LIBS}" AND "${MINGW}")
    get_filename_component(CXX_COMPILER_DIR ${CMAKE_CXX_COMPILER} DIRECTORY)
    foreach(fileName
        "libstdc++-6.dll"
        "libgcc_s_seh-1.dll"
        "libwinpthread-1.dll"
    )
        if(EXISTS ${CXX_COMPILER_DIR}/${fileName})
            add_custom_command(
                TARGET "${PROJECT_NAME}-executable"
                PRE_BUILD
                COMMAND ${CMAKE_COMMAND}
                ARGS -E copy "${CXX_COMPILER_DIR}/${fileName}" "$<TARGET_FILE_DIR:${PROJECT_NAME}-executable>"
                BYPRODUCTS ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${fileName}
                #COMMENT "custom-command-copy: '${CXX_COMPILER_DIR}/${fileName}' to '${CMAKE_RUNTIME_OUTPUT_DIRECTORY}'"
                VERBATIM
            )
        endif()
    endforeach()
endif()

set(PROJECT_PATH_LIST "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")
set(PROJECT_PATH_LIST "${PROJECT_PATH_LIST}" "${CMAKE_BINARY_DIR}/${CMAKE_PROJECT_NAME}-lib/bin")
cmake_path(
    CONVERT "${PROJECT_PATH_LIST}"
    TO_NATIVE_PATH_LIST PROJECT_NATIVE_PATH_LIST
    NORMALIZE
)

add_custom_target("${PROJECT_NAME}-1"
    COMMAND "${CMAKE_COMMAND}"
            -E
            env
            "PATH=${PROJECT_NATIVE_PATH_LIST}"
            "$<TARGET_FILE:${PROJECT_NAME}-executable>"
            --tests
            all
    WORKING_DIRECTORY "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}"
    COMMENT "custom-target: '${PROJECT_NAME}-1'"
    VERBATIM
)

add_custom_target("${PROJECT_NAME}-2"
    COMMAND "${CMAKE_COMMAND}"
            -E
            env
            "PATH=${PROJECT_NATIVE_PATH_LIST}"
            "$<TARGET_FILE:${PROJECT_NAME}-executable>"
            --tests
            exqudens.other.testFunction1
            exqudens.other.testFunction2
            exqudens.other.OtherTests.testMethod1
            exqudens.other.OtherTests.testMethod2
    WORKING_DIRECTORY "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}"
    COMMENT "custom-target: '${PROJECT_NAME}-2'"
    VERBATIM
)

add_custom_target("${PROJECT_NAME}-3"
    COMMAND "${CMAKE_COMMAND}"
            -E
            env
            "PATH=${PROJECT_NATIVE_PATH_LIST}"
            "$<TARGET_FILE:${PROJECT_NAME}-executable>"
            --tests
            0
            1
            2
            3
    WORKING_DIRECTORY "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}"
    COMMENT "custom-target: '${PROJECT_NAME}-3'"
    VERBATIM
)

add_custom_target("${PROJECT_NAME}-4"
    COMMAND "${CMAKE_COMMAND}"
            -E
            env
            "PATH=${PROJECT_NATIVE_PATH_LIST}"
            "$<TARGET_FILE:${PROJECT_NAME}-executable>"
            --tests
            exqudens.other.testFunction2
            exqudens.other.OtherTests.testMethod2
    WORKING_DIRECTORY "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}"
    COMMENT "custom-target: '${PROJECT_NAME}-4'"
    VERBATIM
)

add_custom_target("${PROJECT_NAME}-5"
    COMMAND "${CMAKE_COMMAND}"
            -E
            env
            "PATH=${PROJECT_NATIVE_PATH_LIST}"
            "$<TARGET_FILE:${PROJECT_NAME}-executable>"
            --tests
            1
            3
    WORKING_DIRECTORY "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}"
    COMMENT "custom-target: '${PROJECT_NAME}-5'"
    VERBATIM
)

add_custom_target("${PROJECT_NAME}"
    COMMAND "${CMAKE_COMMAND}"
            -E
            echo
            ALL TESTS PASSED
    COMMENT "custom-target: '${PROJECT_NAME}'"
    VERBATIM
)
add_dependencies("${PROJECT_NAME}"
    "${PROJECT_NAME}-1"
    "${PROJECT_NAME}-2"
    "${PROJECT_NAME}-3"
    "${PROJECT_NAME}-4"
    "${PROJECT_NAME}-5"
)
