cmake_minimum_required(VERSION 3.16)

project(AverraWidgets VERSION 1.0.1 LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(AVERRA_BUILD_EXAMPLES "Build Averra examples." ON)
option(AVERRA_BUILD_TESTS "Build Averra tests." ON)
option(AVERRA_ENABLE_CPACK "Enable CPack package generation." ON)

find_package(Qt5 5.14.2 REQUIRED COMPONENTS Widgets Test)

add_subdirectory(src)

if (AVERRA_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif ()

if (AVERRA_BUILD_TESTS)
    include(CTest)
    if (BUILD_TESTING)
        add_subdirectory(tests)
    endif ()
endif ()

if (AVERRA_ENABLE_CPACK)
    set(CPACK_PACKAGE_NAME "AverraWidgets")
    set(CPACK_PACKAGE_VENDOR "Averra")
    set(CPACK_PACKAGE_CONTACT "Averra Team")
    set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Qt Widgets desktop component library and business workflow toolkit.")
    set(CPACK_PACKAGE_VERSION ${PROJECT_VERSION})
    set(CPACK_PACKAGE_DIRECTORY ${CMAKE_BINARY_DIR}/packages)
    set(CPACK_PACKAGE_FILE_NAME "AverraWidgets-${PROJECT_VERSION}-${CMAKE_SYSTEM_NAME}-${CMAKE_SYSTEM_PROCESSOR}")
    set(CPACK_SOURCE_PACKAGE_FILE_NAME "AverraWidgets-${PROJECT_VERSION}-src")
    set(CPACK_RESOURCE_FILE_README "${CMAKE_CURRENT_SOURCE_DIR}/README.md")
    set(CPACK_SOURCE_IGNORE_FILES
        "/build/"
        "/build-.*/"
        "/dist/"
        "/\\\\.git/"
        "/\\\\.qtcreator/"
        "/\\\\.qmake\\\\.stash"
        ".*\\\\.pro\\\\.user.*"
        ".*~$"
    )
    set(CPACK_GENERATOR "TGZ;ZIP")
    set(CPACK_SOURCE_GENERATOR "TGZ;ZIP")
    set(CPACK_INCLUDE_TOPLEVEL_DIRECTORY ON)
    include(CPack)
endif ()
