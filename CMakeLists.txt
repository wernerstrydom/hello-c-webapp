cmake_minimum_required(VERSION 3.24)

IF (DEFINED ENV{BUILD_NUMBER})
    set(HELLO_BUILD_NUMBER $ENV{BUILD_NUMBER})
ELSE ()
    set(HELLO_BUILD_NUMBER 1)
ENDIF ()

project(hello
        VERSION "0.1.${HELLO_BUILD_NUMBER}"
        DESCRIPTION "Prints Hello World"
        HOMEPAGE_URL "https://wernerstrydom.com"
        LANGUAGES C)

set(CMAKE_C_STANDARD 99)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../opt/hello/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../opt/hello/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../opt/hello/bin)

add_executable(hello main.c mongoose.c mongoose.h)


install(TARGETS hello DESTINATION bin COMPONENT core)

if (WINDOWS)
    if (CMAKE_CL_64)
        set (CPACK_SYSTEM_NAME "win64")
    else ()
        set (CPACK_SYSTEM_NAME "win32")
    endif ()
    set (CPACK_PACKAGE_ARCHITECTURE)
elseif (APPLE)
    set (CPACK_SYSTEM_NAME darwin)
    set (CPACK_PACKAGE_ARCHITECTURE amd64)
else ()
    string (TOLOWER "${CMAKE_SYSTEM_NAME}" CPACK_SYSTEM_NAME)
    if (CMAKE_CXX_FLAGS MATCHES "-m32")
        set (CPACK_PACKAGE_ARCHITECTURE i386)
    else ()
        execute_process (
                COMMAND         dpkg --print-architecture
                RESULT_VARIABLE RV
                OUTPUT_VARIABLE CPACK_PACKAGE_ARCHITECTURE
        )
        if (RV EQUAL 0)
            string (STRIP "${CPACK_PACKAGE_ARCHITECTURE}" CPACK_PACKAGE_ARCHITECTURE)
        else ()
            execute_process (COMMAND uname -m OUTPUT_VARIABLE CPACK_PACKAGE_ARCHITECTURE)
            if (CPACK_PACKAGE_ARCHITECTURE MATCHES "x86_64")
                set (CPACK_PACKAGE_ARCHITECTURE amd64)
            else ()
                set (CPACK_PACKAGE_ARCHITECTURE i386)
            endif ()
        endif ()
    endif ()
endif ()

# CPack
set(
        CPACK_INSTALL_DEFAULT_DIRECTORY_PERMISSIONS
        OWNER_READ OWNER_WRITE OWNER_EXECUTE
        GROUP_READ GROUP_EXECUTE
        WORLD_READ WORLD_EXECUTE
)

# Package
set (CPACK_VERBATIM_VARIABLES           YES)
set (CPACK_COMPONENTS_GROUPING          ALL_COMPONENTS_IN_ONE)
set (CPACK_STRIP_FILES                  YES)
set (CPACK_PACKAGE_VENDOR               "Werner Strydom")
set (CPACK_PACKAGE_CONTACT              "hello@wernerstrydom.com")
string (TOLOWER "${CMAKE_PROJECT_NAME}" CPACK_PACKAGE_NAME)
string (TOLOWER "${CMAKE_SYSTEM_NAME}"  CPACK_SYSTEM_NAME)
set (CPACK_PACKAGE_VERSION             "${PROJECT_VERSION}")
set (CPACK_PACKAGE_VERSION_MAJOR       "${PROJECT_VERSION_MAJOR}")
set (CPACK_PACKAGE_VERSION_MINOR       "${PROJECT_VERSION_MINOR}")
set (CPACK_PACKAGE_VERSION_PATCH       "${PROJECT_VERSION_PATCH}")
set (CPACK_PACKAGE_DESCRIPTION_SUMMARY "${CMAKE_PROJECT_DESCRIPTION}")
set (CPACK_RESOURCE_FILE_WELCOME       "${CMAKE_CURRENT_LIST_DIR}/setup/README.txt")
set (CPACK_RESOURCE_FILE_LICENSE       "${CMAKE_CURRENT_LIST_DIR}/setup/LICENSE.txt")
set (CPACK_PACKAGE_DESCRIPTION_FILE    "${CMAKE_CURRENT_LIST_DIR}/setup/README.txt")
set (CPACK_PACKAGE_INSTALL_DIRECTORY    ${CPACK_PACKAGE_NAME})
set (CPACK_INSTALL_PREFIX               "/opt/hello")
set (CPACK_OUTPUT_FILE_PREFIX           packages)
set (CPACK_PACKAGE_RELOCATABLE          TRUE)
set (CPACK_MONOLITHIC_INSTALL           TRUE)
set (CPACK_OUTPUT_FILE_PREFIX           "${CMAKE_SOURCE_DIR}/dist/packages")

set (CPACK_PACKAGE_FILE_NAME          "${CPACK_PACKAGE_NAME}-${CPACK_PACKAGE_VERSION}-${CPACK_SYSTEM_NAME}")
if (CPACK_PACKAGE_ARCHITECTURE)
    set (CPACK_PACKAGE_FILE_NAME      "${CPACK_PACKAGE_FILE_NAME}-${CPACK_PACKAGE_ARCHITECTURE}")
endif ()

configure_file (
        "${CMAKE_CURRENT_LIST_DIR}/cmake/package.cmake.in"
        "${PROJECT_BINARY_DIR}/${PACKAGE_NAME}-package.cmake" @ONLY
)
set (CPACK_PROJECT_CONFIG_FILE "${PROJECT_BINARY_DIR}/${PACKAGE_NAME}-package.cmake")

INCLUDE(CPack)

