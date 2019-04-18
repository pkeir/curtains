cmake_minimum_required(VERSION 3.10)

set(soln curtains)
set(proj curtains-tests)
project(${soln})

file(GLOB curtains-files ${CMAKE_SOURCE_DIR}/curtains/*.hpp)
file(GLOB  prelude-files ${CMAKE_SOURCE_DIR}/prelude/*.hpp)

add_executable(${proj} ${proj}.cpp ${proj}.hpp ${curtains-files} ${prelude-files})

source_group("Header Files\\curtains" FILES ${curtains-files})
source_group("Header Files\\prelude"  FILES ${prelude-files})

set_property(TARGET ${proj} PROPERTY CXX_STANDARD 17)
set_property(TARGET ${proj} PROPERTY CXX_STANDARD_REQUIRED ON)
set_property(DIRECTORY PROPERTY VS_STARTUP_PROJECT ${proj})


