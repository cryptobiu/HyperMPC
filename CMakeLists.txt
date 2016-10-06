cmake_minimum_required(VERSION 3.0)
project(Secret_Sharing)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O3 -std=c++11")

INCLUDE_DIRECTORIES($ENV{HOME}/workspace/paho/src)

set(SOURCE_FILES oldmain.cpp AES_PRG.cpp HIM.cpp TField.cpp TFieldElement.cpp TParty.cpp main.cpp VDM.cpp
        TGate.h ArithmeticCircuit.cpp Communication.cpp Communication.h Protocol.cpp Protocol.h)
add_executable(Secret_Sharing ${SOURCE_FILES})

TARGET_LINK_LIBRARIES(Secret_Sharing ntl.a gmp ssl crypto $ENV{HOME}/workspace/paho/src/libpaho-mqtt3a.so
        $ENV{HOME}/workspace/paho/src/libpaho-mqtt3c.so)