cmake_minimum_required(VERSION 2.8)
project(Secret_Sharing)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -std=c++11 -maes -mpclmul" )

INCLUDE_DIRECTORIES($ENV{HOME}/paho.mqtt.c/src $ENV{HOME}/ntl-9.10.0/include)

set(SOURCE_FILES AES_PRG.cpp HIM.cpp main.cpp VDM.cpp
        TGate.h ArithmeticCircuit.cpp Communication.cpp Communication.h Protocol.cpp Protocol.h TemplateField.h TemplateField.cpp ProtocolTimer.cpp ProtocolTimer.h)
add_executable(Secret_Sharing ${SOURCE_FILES})

TARGET_LINK_LIBRARIES(Secret_Sharing $ENV{HOME}/ntl-9.10.0/src/ntl.a gmp crypto
        $ENV{HOME}/paho.mqtt.c/src/libpaho-mqtt3c.so)
