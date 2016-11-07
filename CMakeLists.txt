cmake_minimum_required(VERSION 3.0)
project(Secret_Sharing)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -std=c++11")

INCLUDE_DIRECTORIES($ENV{HOME}/workspace/paho/src /home/meital/ntl-9.10.0/include)

set(SOURCE_FILES AES_PRG.cpp HIM.cpp TField.cpp TFieldElement.cpp TParty.cpp main.cpp VDM.cpp
        TGate.h ArithmeticCircuit.cpp Communication.cpp Communication.h Protocol.cpp Protocol.h TFieldElementZp.cpp TFieldElementZp.h TFieldZp.cpp TFieldZp.h TFieldGF2E.cpp TFieldGF2E.h TFieldElementGF2E.cpp TFieldElementGF2E.h Def.h)
add_executable(Secret_Sharing ${SOURCE_FILES})

TARGET_LINK_LIBRARIES(Secret_Sharing /home/meital/ClionProjects/Secret-Sharing/ntl.a gmp ssl crypto $ENV{HOME}/workspace/paho/src/libpaho-mqtt3a.so
        $ENV{HOME}/workspace/paho/src/libpaho-mqtt3c.so)