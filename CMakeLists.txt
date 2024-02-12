cmake_minimum_required(VERSION 3.27)
project(trading)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /bigobj")
set(OPENSSL_ROOT_DIR "C:/Program Files/OpenSSL/3.1.1/")
set(OPENSSL_INCLUDE_DIR "C:/Program Files/OpenSSL/3.1.1/include/")
set(OPENSSL_LIBRARIES "C:/Program Files/OpenSSL/3.1.1/lib/")
set(OPENSSL_CRYPTO_LIBRARY "C:/Program Files/OpenSSL/3.1.1/lib/libcrypto.lib")
set(OPENSSL_SSL_LIBRARY "C:/Program Files/OpenSSL/3.1.1/lib/libssl.lib")

set(Boost_USE_STATIC_LIBS ON)
find_package(Boost COMPONENTS iostreams zlib container json REQUIRED)
find_package(OpenSSL)

include_directories(${Boost_INCLUDE_DIRS})
include_directories(${OPENSSL_INCLUDE_DIR})

if (WIN32)
    add_definitions(-D_WIN32_WINNT=0x0A00)
endif()

add_executable(trading main.cpp
        WebSocketSession.cpp
        WebSocketSession.hpp
        BoostIncludes.hpp
        Utils.hpp
        Utils.cpp
        BingXWS.hpp
        KLineData.hpp
        KlineDataAggregator.hpp
        Indicator.hpp
)
target_link_libraries(trading ${OPENSSL_SSL_LIBRARY} ${OPENSSL_CRYPTO_LIBRARY} Boost::iostreams Boost::zlib Boost::container Boost::json)
