cmake_minimum_required(VERSION 3.20)
file(READ "${CMAKE_CURRENT_SOURCE_DIR}/../version.txt" RAW_VERSION)
string(STRIP "${RAW_VERSION}" VERSION)
project("exqudens-cpp-test-lib" VERSION "${VERSION}")

message("project: '${PROJECT_NAME}-${PROJECT_VERSION}'")

if(${CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT})
    set(CMAKE_INSTALL_PREFIX "${CMAKE_CURRENT_BINARY_DIR}/cmake-install" CACHE PATH "..." FORCE)
endif()

set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS FALSE)
#set(CMAKE_CXX_VISIBILITY_PRESET "hidden")
#set(CMAKE_VISIBILITY_INLINES_HIDDEN "YES")
if("${BUILD_SHARED_LIBS}")
    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS TRUE)
    #set(CMAKE_CXX_VISIBILITY_PRESET "default")
    #set(CMAKE_VISIBILITY_INLINES_HIDDEN "NO")
endif()

set(CMAKE_OBJECT_PATH_MAX 1000)
set(CMAKE_SHARED_LIBRARY_PREFIX "")
set(CMAKE_SHARED_MODULE_PREFIX "")
set(CMAKE_STATIC_LIBRARY_PREFIX "")
set(CMAKE_IMPORT_LIBRARY_PREFIX "")
set(CMAKE_STAGING_PREFIX "")
set(CMAKE_FIND_LIBRARY_PREFIXES "")
set(CMAKE_CXX_STANDARD 20)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY                "${CMAKE_CURRENT_BINARY_DIR}/bin")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE        "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_MINSIZEREL     "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG          "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY                "${CMAKE_CURRENT_BINARY_DIR}/lib")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE        "${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELWITHDEBINFO "${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_MINSIZEREL     "${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG          "${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}")

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY                "${CMAKE_CURRENT_BINARY_DIR}/lib")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE        "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELWITHDEBINFO "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_MINSIZEREL     "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG          "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}")

if(DEFINED CONAN_PROGRAM)
    if("${CMAKE_SYSTEM_NAME}" STREQUAL "Windows")
        set(CONAN_SETTINGS --settings "os=${CMAKE_SYSTEM_NAME}")
    elseif("${CMAKE_SYSTEM_NAME}" STREQUAL "Linux")
        set(CONAN_SETTINGS --settings "os=${CMAKE_SYSTEM_NAME}")
    elseif("${CMAKE_SYSTEM_NAME}" STREQUAL "Darwin")
        set(CONAN_SETTINGS --settings "os=Macos")
    else()
        message(FATAL_ERROR "Unsupported CMAKE_SYSTEM_NAME: '${CMAKE_SYSTEM_NAME}'")
    endif()

    if("${CMAKE_SYSTEM_PROCESSOR}" STREQUAL "AMD64")
        set(CONAN_SETTINGS ${CONAN_SETTINGS} --settings "arch=x86_64")
    else()
        message(FATAL_ERROR "Unsupported CMAKE_SYSTEM_PROCESSOR: '${CMAKE_SYSTEM_PROCESSOR}'")
    endif()

    if(
        "${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC"
        AND "${CMAKE_CXX_COMPILER_VERSION}" VERSION_GREATER_EQUAL "19"
    )
        set(CONAN_SETTINGS ${CONAN_SETTINGS} --settings "compiler=Visual Studio")
        set(CONAN_SETTINGS ${CONAN_SETTINGS} --settings "compiler.version=16")
    elseif(
        "${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU"
        AND "${CMAKE_CXX_COMPILER_VERSION}" VERSION_GREATER_EQUAL "10"
    )
        set(CONAN_SETTINGS ${CONAN_SETTINGS} --settings "compiler=gcc")
        set(CONAN_SETTINGS ${CONAN_SETTINGS} --settings "compiler.version=10")
        if("${CMAKE_CXX_STANDARD}" GREATER_EQUAL "11")
            set(CONAN_SETTINGS ${CONAN_SETTINGS} --settings "compiler.libcxx=libstdc++11")
        else()
            set(CONAN_SETTINGS ${CONAN_SETTINGS} --settings "compiler.libcxx=libstdc++")
        endif()
    else()
        message(FATAL_ERROR "Unsupported CMAKE_CXX_COMPILER_ID: '${CMAKE_CXX_COMPILER_ID}' and CMAKE_CONAN_SETTINGS_COMPILER_VERSION: '${CMAKE_CONAN_SETTINGS_COMPILER_VERSION}'")
    endif()

    if("${CMAKE_BUILD_TYPE}" STREQUAL "Release")
        set(CONAN_SETTINGS ${CONAN_SETTINGS} --settings "build_type=Release")
    else()
        message(FATAL_ERROR "Unsupported CMAKE_BUILD_TYPE: '${CMAKE_BUILD_TYPE}'")
    endif()

    if("${BUILD_SHARED_LIBS}")
        set(CONAN_OPTIONS_SHARED "True")
    else()
        set(CONAN_OPTIONS_SHARED "False")
    endif()

    set(CONAN_OPTIONS
        --options "shared=${CONAN_OPTIONS_SHARED}"
    )

    add_custom_target("${PROJECT_NAME}-conan-export-testing"
        COMMAND "${CONAN_PROGRAM}"
                export-pkg
                -f
                "${CMAKE_CURRENT_SOURCE_DIR}"
                test-user/test-channel
                --package-folder
                "${CMAKE_INSTALL_PREFIX}"
                ${CONAN_SETTINGS}
                ${CONAN_OPTIONS}
        COMMENT "execute: '${PROJECT_NAME}-conan-export-testing'"
        VERBATIM
    )

    add_custom_target("${PROJECT_NAME}-conan-export"
        COMMAND "${CONAN_PROGRAM}"
                export-pkg
                "${CMAKE_CURRENT_SOURCE_DIR}"
                --package-folder
                "${CMAKE_INSTALL_PREFIX}"
                ${CONAN_SETTINGS}
                ${CONAN_OPTIONS}
        COMMENT "execute: '${PROJECT_NAME}-conan-export'"
        VERBATIM
    )
endif()

add_library("${PROJECT_NAME}"
    "src/main/cpp/exqudens/test/Application.hpp"
    "src/main/cpp/exqudens/test/Application.cpp"
)
target_include_directories("${PROJECT_NAME}" PRIVATE
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src/main/cpp>"
    "$<INSTALL_INTERFACE:include>"
)
set_property(TARGET "${PROJECT_NAME}" PROPERTY "VERSION" "${PROJECT_VERSION}")
set_property(TARGET "${PROJECT_NAME}" PROPERTY "SOVERSION" "${PROJECT_VERSION_MAJOR}")
set_property(TARGET "${PROJECT_NAME}" PROPERTY "INTERFACE_${PROJECT_NAME}_MAJOR_VERSION" "${PROJECT_VERSION_MAJOR}")
set_property(TARGET "${PROJECT_NAME}" APPEND PROPERTY "COMPATIBLE_INTERFACE_STRING" "${PROJECT_NAME}_MAJOR_VERSION")

install(
    TARGETS "${PROJECT_NAME}"
    EXPORT "${PROJECT_NAME}-targets"
    INCLUDES DESTINATION "include"
    RUNTIME DESTINATION "bin"
    LIBRARY DESTINATION "lib"
    ARCHIVE DESTINATION "lib"
)

install(
    DIRECTORY "src/main/cpp/"
    DESTINATION "include"
    FILES_MATCHING PATTERN "*.hpp"
)

install(
    EXPORT "${PROJECT_NAME}-targets"
    DESTINATION "cmake"
    NAMESPACE "${PROJECT_NAME}::"
    FILE "${PROJECT_NAME}-targets.cmake"
)

include(CMakePackageConfigHelpers)

configure_package_config_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/config.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}-config.cmake"
    INSTALL_DESTINATION "cmake"
)

write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}-config-version.cmake"
    VERSION "${PROJECT_VERSION}"
    COMPATIBILITY "ExactVersion"
)

install(
    FILES "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}-config.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}-config-version.cmake"
    DESTINATION "cmake"
)

add_custom_target("${PROJECT_NAME}-cmake-install"
    COMMAND "${CMAKE_COMMAND}" --install "${CMAKE_CURRENT_BINARY_DIR}" --prefix "${CMAKE_INSTALL_PREFIX}"
    COMMENT "execute: '${PROJECT_NAME}-cmake-install'"
    VERBATIM
)

add_custom_target("${PROJECT_NAME}-cmake-install-clean"
    COMMAND "${CMAKE_COMMAND}" -E rm -rf "${CMAKE_INSTALL_PREFIX}"
    COMMENT "execute: '${PROJECT_NAME}-cmake-install-clean'"
    VERBATIM
)

add_custom_target("${PROJECT_NAME}-cmake-clean"
    COMMAND "${CMAKE_COMMAND}" --build "${CMAKE_CURRENT_BINARY_DIR}" --target clean
    COMMENT "execute: '${PROJECT_NAME}-cmake-clean'"
    VERBATIM
)

add_dependencies("${PROJECT_NAME}-cmake-clean"   "${PROJECT_NAME}-cmake-install-clean")
add_dependencies("${PROJECT_NAME}-cmake-install" "${PROJECT_NAME}")
