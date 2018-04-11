

/*
WARNING: THIS FILE IS AUTO-GENERATED. DO NOT MODIFY.

This file was generated from Test.idl using "rtiddsgen".
The rtiddsgen tool is part of the RTI Connext distribution.
For more information, type 'rtiddsgen -help' at a command shell
or consult the RTI Connext manual.
*/

#include <iosfwd>
#include <iomanip>

#include "rti/topic/cdr/Serialization.hpp"

#include "Test.hpp"
#include "TestPlugin.hpp"

#include <rti/util/ostream_operators.hpp>

// ---- MyStruct: 

MyStruct::MyStruct() :
    m_sequenceNumber_ (0)  {
}   

MyStruct::MyStruct (
    const std::string& ID,
    int32_t sequenceNumber,
    const rti::core::bounded_sequence<char, (MAX_SEQUENCE_SIZE)>& data)
    :
        m_ID_( ID ),
        m_sequenceNumber_( sequenceNumber ),
        m_data_( data ) {
}

#ifdef RTI_CXX11_RVALUE_REFERENCES
#ifdef RTI_CXX11_NO_IMPLICIT_MOVE_OPERATIONS
MyStruct::MyStruct(MyStruct&& other_) OMG_NOEXCEPT  :m_ID_ (std::move(other_.m_ID_))
,
m_sequenceNumber_ (std::move(other_.m_sequenceNumber_))
,
m_data_ (std::move(other_.m_data_))
{
} 

MyStruct& MyStruct::operator=(MyStruct&&  other_) OMG_NOEXCEPT {
    MyStruct tmp(std::move(other_));
    swap(tmp); 
    return *this;
}
#endif
#endif   

void MyStruct::swap(MyStruct& other_)  OMG_NOEXCEPT 
{
    using std::swap;
    swap(m_ID_, other_.m_ID_);
    swap(m_sequenceNumber_, other_.m_sequenceNumber_);
    swap(m_data_, other_.m_data_);
}  

bool MyStruct::operator == (const MyStruct& other_) const {
    if (m_ID_ != other_.m_ID_) {
        return false;
    }
    if (m_sequenceNumber_ != other_.m_sequenceNumber_) {
        return false;
    }
    if (m_data_ != other_.m_data_) {
        return false;
    }
    return true;
}
bool MyStruct::operator != (const MyStruct& other_) const {
    return !this->operator ==(other_);
}

// --- Getters and Setters: -------------------------------------------------
std::string& MyStruct::ID() OMG_NOEXCEPT {
    return m_ID_;
}

const std::string& MyStruct::ID() const OMG_NOEXCEPT {
    return m_ID_;
}

void MyStruct::ID(const std::string& value) {
    m_ID_ = value;
}

int32_t& MyStruct::sequenceNumber() OMG_NOEXCEPT {
    return m_sequenceNumber_;
}

const int32_t& MyStruct::sequenceNumber() const OMG_NOEXCEPT {
    return m_sequenceNumber_;
}

void MyStruct::sequenceNumber(int32_t value) {
    m_sequenceNumber_ = value;
}

rti::core::bounded_sequence<char, (MAX_SEQUENCE_SIZE)>& MyStruct::data() OMG_NOEXCEPT {
    return m_data_;
}

const rti::core::bounded_sequence<char, (MAX_SEQUENCE_SIZE)>& MyStruct::data() const OMG_NOEXCEPT {
    return m_data_;
}

void MyStruct::data(const rti::core::bounded_sequence<char, (MAX_SEQUENCE_SIZE)>& value) {
    m_data_ = value;
}

std::ostream& operator << (std::ostream& o,const MyStruct& sample)
{
    rti::util::StreamFlagSaver flag_saver (o);
    o <<"[";
    o << "ID: " << sample.ID()<<", ";
    o << "sequenceNumber: " << sample.sequenceNumber()<<", ";
    o << "data: " << sample.data() ;
    o <<"]";
    return o;
}

// --- Type traits: -------------------------------------------------

namespace rti { 
    namespace topic {

        template<>
        struct native_type_code<MyStruct> {
            static DDS_TypeCode * get()
            {
                static RTIBool is_initialized = RTI_FALSE;

                static DDS_TypeCode MyStruct_g_tc_ID_string = DDS_INITIALIZE_STRING_TYPECODE((255));
                static DDS_TypeCode MyStruct_g_tc_data_sequence = DDS_INITIALIZE_SEQUENCE_TYPECODE(((MAX_SEQUENCE_SIZE)),NULL);
                static DDS_TypeCode_Member MyStruct_g_tc_members[3]=
                {

                    {
                        (char *)"ID",/* Member name */
                        {
                            0,/* Representation ID */          
                            DDS_BOOLEAN_FALSE,/* Is a pointer? */
                            -1, /* Bitfield bits */
                            NULL/* Member type code is assigned later */
                        },
                        0, /* Ignored */
                        0, /* Ignored */
                        0, /* Ignored */
                        NULL, /* Ignored */
                        RTI_CDR_REQUIRED_MEMBER, /* Is a key? */
                        DDS_PUBLIC_MEMBER,/* Member visibility */
                        1,
                        NULL/* Ignored */
                    }, 
                    {
                        (char *)"sequenceNumber",/* Member name */
                        {
                            1,/* Representation ID */          
                            DDS_BOOLEAN_FALSE,/* Is a pointer? */
                            -1, /* Bitfield bits */
                            NULL/* Member type code is assigned later */
                        },
                        0, /* Ignored */
                        0, /* Ignored */
                        0, /* Ignored */
                        NULL, /* Ignored */
                        RTI_CDR_REQUIRED_MEMBER, /* Is a key? */
                        DDS_PUBLIC_MEMBER,/* Member visibility */
                        1,
                        NULL/* Ignored */
                    }, 
                    {
                        (char *)"data",/* Member name */
                        {
                            2,/* Representation ID */          
                            DDS_BOOLEAN_FALSE,/* Is a pointer? */
                            -1, /* Bitfield bits */
                            NULL/* Member type code is assigned later */
                        },
                        0, /* Ignored */
                        0, /* Ignored */
                        0, /* Ignored */
                        NULL, /* Ignored */
                        RTI_CDR_REQUIRED_MEMBER, /* Is a key? */
                        DDS_PUBLIC_MEMBER,/* Member visibility */
                        1,
                        NULL/* Ignored */
                    }
                };

                static DDS_TypeCode MyStruct_g_tc =
                {{
                        DDS_TK_STRUCT,/* Kind */
                        DDS_BOOLEAN_FALSE, /* Ignored */
                        -1, /*Ignored*/
                        (char *)"MyStruct", /* Name */
                        NULL, /* Ignored */      
                        0, /* Ignored */
                        0, /* Ignored */
                        NULL, /* Ignored */
                        3, /* Number of members */
                        MyStruct_g_tc_members, /* Members */
                        DDS_VM_NONE  /* Ignored */         
                    }}; /* Type code for MyStruct*/

                if (is_initialized) {
                    return &MyStruct_g_tc;
                }

                MyStruct_g_tc_data_sequence._data._typeCode = (RTICdrTypeCode *)&DDS_g_tc_char;

                MyStruct_g_tc_members[0]._representation._typeCode = (RTICdrTypeCode *)&MyStruct_g_tc_ID_string;

                MyStruct_g_tc_members[1]._representation._typeCode = (RTICdrTypeCode *)&DDS_g_tc_long;

                MyStruct_g_tc_members[2]._representation._typeCode = (RTICdrTypeCode *)& MyStruct_g_tc_data_sequence;

                is_initialized = RTI_TRUE;

                return &MyStruct_g_tc;
            }
        }; // native_type_code

        const dds::core::xtypes::StructType& dynamic_type<MyStruct>::get()
        {
            return static_cast<const dds::core::xtypes::StructType&>(
                rti::core::native_conversions::cast_from_native<dds::core::xtypes::DynamicType>(
                    *(native_type_code<MyStruct>::get())));
        }

    }
}  

namespace dds { 
    namespace topic {
        void topic_type_support<MyStruct>:: register_type(
            dds::domain::DomainParticipant& participant,
            const std::string& type_name) 
        {

            rti::domain::register_type_plugin(
                participant,
                type_name,
                MyStructPlugin_new,
                MyStructPlugin_delete);
        }

        std::vector<char>& topic_type_support<MyStruct>::to_cdr_buffer(
            std::vector<char>& buffer, const MyStruct& sample)
        {
            // First get the length of the buffer
            unsigned int length = 0;
            RTIBool ok = MyStructPlugin_serialize_to_cdr_buffer(
                NULL, 
                &length,
                &sample);
            rti::core::check_return_code(
                ok ? DDS_RETCODE_OK : DDS_RETCODE_ERROR,
                "Failed to calculate cdr buffer size");

            // Create a vector with that size and copy the cdr buffer into it
            buffer.resize(length);
            ok = MyStructPlugin_serialize_to_cdr_buffer(
                &buffer[0], 
                &length, 
                &sample);
            rti::core::check_return_code(
                ok ? DDS_RETCODE_OK : DDS_RETCODE_ERROR,
                "Failed to copy cdr buffer");

            return buffer;
        }

        void topic_type_support<MyStruct>::from_cdr_buffer(MyStruct& sample, 
        const std::vector<char>& buffer)
        {

            RTIBool ok  = MyStructPlugin_deserialize_from_cdr_buffer(
                &sample, 
                &buffer[0], 
                static_cast<unsigned int>(buffer.size()));
            rti::core::check_return_code(ok ? DDS_RETCODE_OK : DDS_RETCODE_ERROR,
            "Failed to create MyStruct from cdr buffer");
        }

        void topic_type_support<MyStruct>::reset_sample(MyStruct& sample) 
        {
            rti::topic::reset_sample(sample.ID());
            rti::topic::reset_sample(sample.sequenceNumber());
            rti::topic::reset_sample(sample.data());
        }

        void topic_type_support<MyStruct>::allocate_sample(MyStruct& sample, int, int) 
        {
            rti::topic::allocate_sample(sample.ID(),  -1, 255);
            rti::topic::allocate_sample(sample.data(),  (MAX_SEQUENCE_SIZE), -1);
        }

    }
}  

