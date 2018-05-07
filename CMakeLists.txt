cmake_minimum_required(VERSION 2.8)
project(Secret_Sharing)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -std=c++11 -maes -mpclmul -mbmi2 -m64 -DRTI_UNIX -DRTI_LINUX -DRTI_64BIT")
set(NDDSHOME $ENV{HOME}/rti_connext_dds-5.3.0)


INCLUDE_DIRECTORIES($ENV{HOME}/ntl-9.10.0/include $ENV{HOME} $ENV{HOME}/libscapi/install/include/
        $ENV{HOME}/libscapi/install/include/OTExtensionBristol
        $ENV{HOME} ${NDDSHOME}/include ${NDDSHOME}/include/ndds
        ${NDDSHOME}/include/ndds/hpp)

set(SOURCE_FILES main.cpp ProtocolParty.cpp ProtocolParty.h TemplateField.h TemplateField.cpp ProtocolTimer.cpp
        ProtocolTimer.h ZpGMPElement.cpp ZpGMPElement.h  GF2_8LookupTable.cpp GF2_8LookupTable.h
        DDSCommunicator.cxx DDSChannelWriter.cxx DDSSample.cxx BIUDDSType.cxx BIUDDSTypePlugin.cxx)

link_directories(/usr/ssl/lib/ $ENV{HOME}/libscapi/install/lib ${NDDSHOME}/lib/x64Linux3gcc5.4.0)


add_executable(Secret_Sharing ${SOURCE_FILES})

TARGET_LINK_LIBRARIES(Secret_Sharing $ENV{HOME}/ntl-9.10.0/src/ntl.a
        $ENV{HOME}/libscapi/libscapi.a gmp
        nddscpp2.so nddsc.so nddscore.so dl nsl m pthread rt
        libboost_system.a libboost_thread.a crypto ssl dl)
