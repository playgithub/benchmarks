cmake_minimum_required(VERSION 3.26)

project(podofo_vs_poppler LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core)

find_package(Poppler REQUIRED
             COMPONENTS Cpp)

find_package(PkgConfig REQUIRED)

set(ENV{PKG_CONFIG_PATH} /usr/local/lib/pkgconfig)

pkg_check_modules(PODOFO REQUIRED IMPORTED_TARGET libpodofo)

add_executable(${PROJECT_NAME}
  main.cpp
)

target_include_directories(
    ${PROJECT_NAME}
    PRIVATE
        ${Poppler_INCLUDE_DIRS}
)

target_link_libraries(
    ${PROJECT_NAME}
    PRIVATE
        Qt${QT_VERSION_MAJOR}::Core
        PkgConfig::PODOFO
        ${Poppler_LIBRARIES}
)

install(TARGETS ${PROJECT_NAME}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
