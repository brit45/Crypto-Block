cmake_minimum_required(VERSION 3.18)

project("cryptos")

find_package(PkgConfig REQUIRED)
pkg_check_modules(JSONCPP jsoncpp)
find_package(jsoncpp REQUIRED)
pkg_check_modules(CRYPTOPP libcrypto)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -Werror")
set(CMAKE_EXE_LINKER_FLAGS "-lstdc++ -lpthread")

set(CMAKE_THREAD_PREFER_PTHREAD TRUE) # these are to use the prefered -pthread flag
set(THREADS_PREFER_PTHREAD_FLAG TRUE) # over the -lpthread option
find_package(Threads REQUIRED)

file(GLOB sources ${CMAKE_SOURCE_DIR} *.cpp)
file(GLOB source_H ${CMAKE_SOURCE_DIR} *.hpp *.h)

add_executable("cryptos" ${sources} ${source_H})
set_property(TARGET "cryptos" PROPERTY CXX_STANDARD 17)
target_compile_options("cryptos" PRIVATE -w)
target_link_libraries("cryptos" jsoncpp ${CRYPTOPP})
