cmake_minimum_required(VERSION 3.0)
project(Secret_Sharing)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -std=c++11" )

INCLUDE_DIRECTORIES($ENV{HOME}/workspace/paho/src /home/meital/ntl-9.10.0/include)

set(SOURCE_FILES AES_PRG.cpp HIM.cpp main.cpp VDM.cpp
        TGate.h ArithmeticCircuit.cpp Communication.cpp Communication.h Protocol.cpp Protocol.h TemplateField.h TemplateField.cpp ProtocolTimer.cpp ProtocolTimer.h)
add_executable(Secret_Sharing ${SOURCE_FILES})

TARGET_LINK_LIBRARIES(Secret_Sharing $ENV{HOME}/ntl-9.10.0/src/ntl.a gmp crypto $ENV{HOME}/workspace/paho/src/libpaho-mqtt3a.so
        $ENV{HOME}/workspace/paho/src/libpaho-mqtt3c.so)
