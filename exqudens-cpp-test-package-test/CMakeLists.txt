cmake_minimum_required(VERSION 3.20)
project("${CMAKE_PROJECT_NAME}-package-test" VERSION "${CMAKE_PROJECT_VERSION}")

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

set(CMAKE_MODULE_PATH "")
set(CMAKE_PREFIX_PATH "")
set(CONAN_INSTALL_PREFIX "${PROJECT_BINARY_DIR}/conan-install")

if(DEFINED "CONAN_COMMAND")
    set(CONAN_PACKAGE_NAMES
        "${CMAKE_PROJECT_NAME}-lib"
    )

    set_conan_settings(CONAN_SETTINGS
        "${CMAKE_SYSTEM_NAME}"
        "${CMAKE_SYSTEM_PROCESSOR}"
        "${CMAKE_CXX_COMPILER_ID}"
        "${CMAKE_CXX_COMPILER_VERSION}"
        "${CMAKE_MSVC_RUNTIME_LIBRARY}"
        "${CMAKE_CXX_STANDARD}"
        "${CMAKE_BUILD_TYPE}"
    )

    set_conan_options(CONAN_OPTIONS
        "${BUILD_SHARED_LIBS}"
        ${CONAN_PACKAGE_NAMES}
    )

    add_custom_target_conan_install("${PROJECT_NAME}-conan-install"
        "${CONAN_COMMAND}"
        "${PROJECT_SOURCE_DIR}/conanfile.py"
        "${CONAN_INSTALL_PREFIX}"
        "${CONAN_SETTINGS}"
        "${CONAN_OPTIONS}"
    )

    if(EXISTS "${CONAN_INSTALL_PREFIX}/conan_paths.cmake")
        set(CMAKE_MODULE_PATH "")
        set(CMAKE_PREFIX_PATH "")
        include("${CONAN_INSTALL_PREFIX}/conan_paths.cmake")
        set(CMAKE_MODULE_PATH "")
        set(CMAKE_PREFIX_PATH "")
        foreach(packageName ${CONAN_PACKAGE_NAMES})
            string(TOUPPER "${packageName}" packageNameUpper)
            if(DEFINED "CONAN_${packageNameUpper}_ROOT" AND IS_DIRECTORY "${CONAN_${packageNameUpper}_ROOT}")
                if("" STREQUAL "${CMAKE_PREFIX_PATH}")
                    set(CMAKE_PREFIX_PATH "${CONAN_${packageNameUpper}_ROOT}")
                else()
                    set(CMAKE_PREFIX_PATH "${CMAKE_PREFIX_PATH}" "${CONAN_${packageNameUpper}_ROOT}")
                endif()
            endif()
        endforeach()
    endif()
endif()

if(NOT "" STREQUAL "${CMAKE_PREFIX_PATH}")
    find_package("${CMAKE_PROJECT_NAME}-lib" "${CMAKE_PROJECT_VERSION}" QUIET CONFIG)
endif()

set_not_found_package_names(NOT_FOUND_PACKAGE_NAMES
    ${CONAN_PACKAGE_NAMES}
)

if(NOT "" STREQUAL "${NOT_FOUND_PACKAGE_NAMES}")
    foreach(package ${NOT_FOUND_PACKAGE_NAMES})
        message(WARNING "${package}_FOUND: '${${package}_FOUND}'")
    endforeach()
    return()
endif()

file(REMOVE_RECURSE "${PROJECT_BINARY_DIR}/generated")
get_target_property(TEST_LIB_SOURCES "${CMAKE_PROJECT_NAME}-test-lib" SOURCES)
foreach(testLibSource ${TEST_LIB_SOURCES})
    get_filename_component(testLibSourceDir "${testLibSource}" DIRECTORY)
    file(COPY "${CMAKE_SOURCE_DIR}/${CMAKE_PROJECT_NAME}-test/${testLibSource}" DESTINATION "${PROJECT_BINARY_DIR}/generated/${testLibSourceDir}")
    if("" STREQUAL "${PACKAGE_TEST_LIB_SOURCES}")
        set(PACKAGE_TEST_LIB_SOURCES "${PROJECT_BINARY_DIR}/generated/${testLibSource}")
    else()
        set(PACKAGE_TEST_LIB_SOURCES "${PACKAGE_TEST_LIB_SOURCES}" "${PROJECT_BINARY_DIR}/generated/${testLibSource}")
    endif()
endforeach()
get_target_property(TEST_EXECUTABLE_SOURCES "${CMAKE_PROJECT_NAME}-test-executable" SOURCES)
get_filename_component(testExecutableSourceDir "${TEST_EXECUTABLE_SOURCES}" DIRECTORY)
file(COPY "${CMAKE_SOURCE_DIR}/${CMAKE_PROJECT_NAME}-test/${TEST_EXECUTABLE_SOURCES}" DESTINATION "${PROJECT_BINARY_DIR}/generated/${testExecutableSourceDir}")
set(PACKAGE_TEST_EXECUTABLE_SOURCES "${PROJECT_BINARY_DIR}/generated/${TEST_EXECUTABLE_SOURCES}")

add_library("${PROJECT_NAME}-lib" STATIC
    ${PACKAGE_TEST_LIB_SOURCES}
)
target_include_directories("${PROJECT_NAME}-lib" PUBLIC
    "${PROJECT_BINARY_DIR}/generated/src/test/cpp"
)
target_link_libraries("${PROJECT_NAME}-lib" PUBLIC
    "${CMAKE_PROJECT_NAME}-lib::${CMAKE_PROJECT_NAME}-lib"
)

add_executable("${PROJECT_NAME}-executable" ${PACKAGE_TEST_EXECUTABLE_SOURCES})
target_link_libraries("${PROJECT_NAME}-executable" PRIVATE
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

set(PROJECT_PATH_LIST "${CONAN_INSTALL_PREFIX}/bin")
cmake_path(
    CONVERT "${PROJECT_PATH_LIST}"
    TO_NATIVE_PATH_LIST PROJECT_NATIVE_PATH_LIST
    NORMALIZE
)
add_custom_target("${PROJECT_NAME}"
    COMMAND "${CMAKE_COMMAND}"
            -E
            env
            "PATH=${PROJECT_NATIVE_PATH_LIST}"
            "$<TARGET_FILE:${PROJECT_NAME}-executable>"
            --tests
            all
    WORKING_DIRECTORY "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}"
    COMMENT "custom-target: '${PROJECT_NAME}'"
    VERBATIM
)
