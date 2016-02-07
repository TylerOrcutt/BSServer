cmake_minimum_required (VERSION 2.6)
project (BSServer)
 SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++0x")
find_package(PkgConfig REQUIRED)




# Search OpenSSL
pkg_search_module(OPENSSL REQUIRED openssl)

if( OPENSSL_FOUND )
    include_directories(${OPENSSL_INCLUDE_DIRS})
    message(STATUS "Using OpenSSL ${OPENSSL_VERSION}")
else()
    # Error; with REQUIRED, pkg_search_module() will throw an error by it's own
endif()

add_executable(BSServer main.cpp DataHandle.cpp )
target_link_libraries(BSServer  -lpthread ${OPENSSL_LIBRARIES})