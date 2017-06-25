cmake_minimum_required(VERSION 2.8)
project(Secret_Sharing)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -std=c++11 -maes -mpclmul -mbmi2" )

SET (BOOST_ROOT $ENV{HOME}/boost_1_60_0/)
SET (BOOST_INCLUDEDIR $ENV{HOME}/boost_1_60_0/)
SET (BOOST_LIBRARYDIR $ENV{HOME}/boost_1_60_0/stage/lib)
find_package(Boost COMPONENTS system thread REQUIRED)

INCLUDE_DIRECTORIES($ENV{HOME}/ntl-9.10.0/include $ENV{HOME} ${BOOST_INCLUDEDIR}
        $ENV{HOME}/libscapi/lib/OTExtensionBristol $ENV{HOME}/libscapi/lib/)

set(SOURCE_FILES AES_PRG.cpp HIM.cpp main.cpp VDM.cpp MPCCommunication.cpp MPCCommunication.h
        TGate.h ArithmeticCircuit.cpp Protocol.cpp Protocol.h TemplateField.h TemplateField.cpp ProtocolTimer.cpp ProtocolTimer.h ZpGMPElement.cpp ZpGMPElement.h ZpMersenneIntElement.cpp ZpMersenneIntElement.h ZpMersenneLongElement.cpp ZpMersenneLongElement.h GF2_8LookupTable.cpp GF2_8LookupTable.h)


link_directories(/usr/ssl/lib/ $ENV{HOME}/libscapi/install/lib ${BOOST_LIBRARYDIR})


add_executable(Secret_Sharing ${SOURCE_FILES})

TARGET_LINK_LIBRARIES(Secret_Sharing $ENV{HOME}/ntl-9.10.0/src/ntl.a
        $ENV{HOME}/libscapi/scapi.a gmp
        OTExtensionBristol
        $ENV{HOME}/libscapi/install/lib/libsimpleot.a
        boost_system boost_thread pthread crypto dl ssl z)
