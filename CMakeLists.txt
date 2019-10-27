cmake_minimum_required(VERSION 2.8)
project(HyperMPC)

if(${CMAKE_HOST_SYSTEM_PROCESSOR} MATCHES aarch64)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O3 -fPIC -march=armv8-a+crypto -flax-vector-conversions")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O3 -std=c++11 -maes -mpclmul -mbmi2")
endif()


INCLUDE_DIRECTORIES($ENV{HOME} $ENV{HOME}/libscapi/install/include/
        $ENV{HOME}/libscapi/install/include/OTExtensionBristol)

set(SOURCE_FILES main.cpp ProtocolParty.cpp TemplateField.cpp GF2_8LookupTable.cpp)

link_directories(/usr/ssl/lib/ $ENV{HOME}/libscapi/install/lib)


add_executable(HyperMPC ${SOURCE_FILES})

TARGET_LINK_LIBRARIES(HyperMPC  $ENV{HOME}/libscapi/install/lib/libntl.a
        $ENV{HOME}/libscapi/libscapi_strip.a gmp
        boost_system boost_thread pthread crypto dl ssl z)
