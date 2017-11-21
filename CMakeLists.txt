cmake_minimum_required(VERSION 2.8)
project(Secret_Sharing)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -std=c++11 -maes -mpclmul -mbmi2")


INCLUDE_DIRECTORIES($ENV{HOME}/ntl-9.10.0/include $ENV{HOME} $ENV{HOME}/libscapi/install/include/
        $ENV{HOME}/libscapi/install/include/OTExtensionBristol )

set(SOURCE_FILES main.cpp ProtocolParty.cpp ProtocolParty.h TemplateField.h TemplateField.cpp ProtocolTimer.cpp
        ProtocolTimer.h ZpGMPElement.cpp ZpGMPElement.h  GF2_8LookupTable.cpp GF2_8LookupTable.h)

link_directories(/usr/ssl/lib/ $ENV{HOME}/libscapi/install/lib)


add_executable(Secret_Sharing ${SOURCE_FILES})

TARGET_LINK_LIBRARIES(Secret_Sharing $ENV{HOME}/ntl-9.10.0/src/ntl.a
        $ENV{HOME}/libscapi/libscapi.a gmp
        OTExtensionBristol libsimpleot.a libjsoncpp.a
        libboost_system.a libboost_thread.a pthread crypto dl ssl z)
