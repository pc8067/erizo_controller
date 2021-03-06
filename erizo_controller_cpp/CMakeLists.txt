cmake_minimum_required(VERSION 2.8)

project (ERIZO_CONTROLLER_CPP)

set(ERIZO_CONTROLLER_CPP_VERSION_MAJOR 0)
set(ERIZO_CONTROLLER_CPP_VERSION_MINOR 1)

set(CMAKE_CXX_FLAGS "-g -Wall -Wno-deprecated-declarations -DDEBUG -std=c++11 -pthread ${ERIZO_CONTROLLER_CPP_CMAKE_CXX_FLAGS}")

set(BOOST_LIBS system regex thread)
find_package(Boost COMPONENTS ${BOOST_LIBS} REQUIRED)


include_directories("${ERIZO_CONTROLLER_CPP_SOURCE_DIR}" "${LIBDEPS_INCLUDE}")
link_directories("${LIBDEPS_LIBARAYS}")

file(GLOB_RECURSE ERIZO_CONTROLLER_CPP_SOURCES "${ERIZO_CONTROLLER_CPP_SOURCE_DIR}/*.h" 
                                               "${ERIZO_CONTROLLER_CPP_SOURCE_DIR}/*.hpp"
                                               "${ERIZO_CONTROLLER_CPP_SOURCE_DIR}/*.c" 
                                               "${ERIZO_CONTROLLER_CPP_SOURCE_DIR}/*.cpp" 
                                               "${ERIZO_CONTROLLER_CPP_SOURCE_DIR}/*.cc")

add_executable(erizo_controller_cpp ${ERIZO_CONTROLLER_CPP_SOURCES})

target_link_libraries(erizo_controller_cpp ${Boost_LIBRARIES} pthread log4cxx jsoncpp rabbitmq uWS z ssl crypto acl_cpp protocol acl)

install(TARGETS erizo_controller_cpp RUNTIME DESTINATION bin)

