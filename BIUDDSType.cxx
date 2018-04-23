

/*
WARNING: THIS FILE IS AUTO-GENERATED. DO NOT MODIFY.

This file was generated from BIUDDSType.idl using "rtiddsgen".
The rtiddsgen tool is part of the RTI Connext distribution.
For more information, type 'rtiddsgen -help' at a command shell
or consult the RTI Connext manual.
*/

#include <iosfwd>
#include <iomanip>

#include "rti/topic/cdr/Serialization.hpp"

#include "BIUDDSType.hpp"
#include "BIUDDSTypePlugin.hpp"

#include <rti/util/ostream_operators.hpp>

// ---- BIUDDSStruct: 

BIUDDSStruct::BIUDDSStruct() :
    m_sourceID_ (0u) ,
    m_sequenceNumber_ (0)  {
}   

BIUDDSStruct::BIUDDSStruct (
    const std::string& sourceIP,
    uint32_t sourceID,
    int32_t sequenceNumber,
    const std::string& tag,
    const rti::core::bounded_sequence<char, (MAX_SEQUENCE_SIZE)>& payload)
    :
        m_sourceIP_( sourceIP ),
        m_sourceID_( sourceID ),
        m_sequenceNumber_( sequenceNumber ),
        m_tag_( tag ),
        m_payload_( payload ) {
}

#ifdef RTI_CXX11_RVALUE_REFERENCES
#ifdef RTI_CXX11_NO_IMPLICIT_MOVE_OPERATIONS
BIUDDSStruct::BIUDDSStruct(BIUDDSStruct&& other_) OMG_NOEXCEPT  :m_sourceIP_ (std::move(other_.m_sourceIP_))
,
m_sourceID_ (std::move(other_.m_sourceID_))
,
m_sequenceNumber_ (std::move(other_.m_sequenceNumber_))
,
m_tag_ (std::move(other_.m_tag_))
,
m_payload_ (std::move(other_.m_payload_))
{
} 

BIUDDSStruct& BIUDDSStruct::operator=(BIUDDSStruct&&  other_) OMG_NOEXCEPT {
    BIUDDSStruct tmp(std::move(other_));
    swap(tmp); 
    return *this;
}
#endif
#endif   

void BIUDDSStruct::swap(BIUDDSStruct& other_)  OMG_NOEXCEPT 
{
    using std::swap;
    swap(m_sourceIP_, other_.m_sourceIP_);
    swap(m_sourceID_, other_.m_sourceID_);
    swap(m_sequenceNumber_, other_.m_sequenceNumber_);
    swap(m_tag_, other_.m_tag_);
    swap(m_payload_, other_.m_payload_);
}  

bool BIUDDSStruct::operator == (const BIUDDSStruct& other_) const {
    if (m_sourceIP_ != other_.m_sourceIP_) {
        return false;
    }
    if (m_sourceID_ != other_.m_sourceID_) {
        return false;
    }
    if (m_sequenceNumber_ != other_.m_sequenceNumber_) {
        return false;
    }
    if (m_tag_ != other_.m_tag_) {
        return false;
    }
    if (m_payload_ != other_.m_payload_) {
        return false;
    }
    return true;
}
bool BIUDDSStruct::operator != (const BIUDDSStruct& other_) const {
    return !this->operator ==(other_);
}

// --- Getters and Setters: -------------------------------------------------
std::string& BIUDDSStruct::sourceIP() OMG_NOEXCEPT {
    return m_sourceIP_;
}

const std::string& BIUDDSStruct::sourceIP() const OMG_NOEXCEPT {
    return m_sourceIP_;
}

void BIUDDSStruct::sourceIP(const std::string& value) {
    m_sourceIP_ = value;
}

uint32_t& BIUDDSStruct::sourceID() OMG_NOEXCEPT {
    return m_sourceID_;
}

const uint32_t& BIUDDSStruct::sourceID() const OMG_NOEXCEPT {
    return m_sourceID_;
}

void BIUDDSStruct::sourceID(uint32_t value) {
    m_sourceID_ = value;
}

int32_t& BIUDDSStruct::sequenceNumber() OMG_NOEXCEPT {
    return m_sequenceNumber_;
}

const int32_t& BIUDDSStruct::sequenceNumber() const OMG_NOEXCEPT {
    return m_sequenceNumber_;
}

void BIUDDSStruct::sequenceNumber(int32_t value) {
    m_sequenceNumber_ = value;
}

std::string& BIUDDSStruct::tag() OMG_NOEXCEPT {
    return m_tag_;
}

const std::string& BIUDDSStruct::tag() const OMG_NOEXCEPT {
    return m_tag_;
}

void BIUDDSStruct::tag(const std::string& value) {
    m_tag_ = value;
}

rti::core::bounded_sequence<char, (MAX_SEQUENCE_SIZE)>& BIUDDSStruct::payload() OMG_NOEXCEPT {
    return m_payload_;
}

const rti::core::bounded_sequence<char, (MAX_SEQUENCE_SIZE)>& BIUDDSStruct::payload() const OMG_NOEXCEPT {
    return m_payload_;
}

void BIUDDSStruct::payload(const rti::core::bounded_sequence<char, (MAX_SEQUENCE_SIZE)>& value) {
    m_payload_ = value;
}

std::ostream& operator << (std::ostream& o,const BIUDDSStruct& sample)
{
    rti::util::StreamFlagSaver flag_saver (o);
    o <<"[";
    o << "sourceIP: " << sample.sourceIP()<<", ";
    o << "sourceID: " << sample.sourceID()<<", ";
    o << "sequenceNumber: " << sample.sequenceNumber()<<", ";
    o << "tag: " << sample.tag()<<", ";
    o << "payload: " << sample.payload() ;
    o <<"]";
    return o;
}

// --- Type traits: -------------------------------------------------

namespace rti { 
    namespace topic {

        template<>
        struct native_type_code<BIUDDSStruct> {
            static DDS_TypeCode * get()
            {
                static RTIBool is_initialized = RTI_FALSE;

                static DDS_TypeCode BIUDDSStruct_g_tc_sourceIP_string = DDS_INITIALIZE_STRING_TYPECODE((255));
                static DDS_TypeCode BIUDDSStruct_g_tc_tag_string = DDS_INITIALIZE_STRING_TYPECODE((255));
                static DDS_TypeCode BIUDDSStruct_g_tc_payload_sequence = DDS_INITIALIZE_SEQUENCE_TYPECODE(((MAX_SEQUENCE_SIZE)),NULL);
                static DDS_TypeCode_Member BIUDDSStruct_g_tc_members[5]=
                {

                    {
                        (char *)"sourceIP",/* Member name */
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
                        (char *)"sourceID",/* Member name */
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
                        RTI_CDR_KEY_MEMBER , /* Is a key? */
                        DDS_PUBLIC_MEMBER,/* Member visibility */
                        1,
                        NULL/* Ignored */
                    }, 
                    {
                        (char *)"sequenceNumber",/* Member name */
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
                    }, 
                    {
                        (char *)"tag",/* Member name */
                        {
                            3,/* Representation ID */          
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
                        (char *)"payload",/* Member name */
                        {
                            4,/* Representation ID */          
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

                static DDS_TypeCode BIUDDSStruct_g_tc =
                {{
                        DDS_TK_STRUCT,/* Kind */
                        DDS_BOOLEAN_FALSE, /* Ignored */
                        -1, /*Ignored*/
                        (char *)"BIUDDSStruct", /* Name */
                        NULL, /* Ignored */      
                        0, /* Ignored */
                        0, /* Ignored */
                        NULL, /* Ignored */
                        5, /* Number of members */
                        BIUDDSStruct_g_tc_members, /* Members */
                        DDS_VM_NONE  /* Ignored */         
                    }}; /* Type code for BIUDDSStruct*/

                if (is_initialized) {
                    return &BIUDDSStruct_g_tc;
                }

                BIUDDSStruct_g_tc_payload_sequence._data._typeCode = (RTICdrTypeCode *)&DDS_g_tc_char;

                BIUDDSStruct_g_tc_members[0]._representation._typeCode = (RTICdrTypeCode *)&BIUDDSStruct_g_tc_sourceIP_string;

                BIUDDSStruct_g_tc_members[1]._representation._typeCode = (RTICdrTypeCode *)&DDS_g_tc_ulong;

                BIUDDSStruct_g_tc_members[2]._representation._typeCode = (RTICdrTypeCode *)&DDS_g_tc_long;

                BIUDDSStruct_g_tc_members[3]._representation._typeCode = (RTICdrTypeCode *)&BIUDDSStruct_g_tc_tag_string;

                BIUDDSStruct_g_tc_members[4]._representation._typeCode = (RTICdrTypeCode *)& BIUDDSStruct_g_tc_payload_sequence;

                is_initialized = RTI_TRUE;

                return &BIUDDSStruct_g_tc;
            }
        }; // native_type_code

        const dds::core::xtypes::StructType& dynamic_type<BIUDDSStruct>::get()
        {
            return static_cast<const dds::core::xtypes::StructType&>(
                rti::core::native_conversions::cast_from_native<dds::core::xtypes::DynamicType>(
                    *(native_type_code<BIUDDSStruct>::get())));
        }

    }
}  

namespace dds { 
    namespace topic {
        void topic_type_support<BIUDDSStruct>:: register_type(
            dds::domain::DomainParticipant& participant,
            const std::string& type_name) 
        {

            rti::domain::register_type_plugin(
                participant,
                type_name,
                BIUDDSStructPlugin_new,
                BIUDDSStructPlugin_delete);
        }

        std::vector<char>& topic_type_support<BIUDDSStruct>::to_cdr_buffer(
            std::vector<char>& buffer, const BIUDDSStruct& sample)
        {
            // First get the length of the buffer
            unsigned int length = 0;
            RTIBool ok = BIUDDSStructPlugin_serialize_to_cdr_buffer(
                NULL, 
                &length,
                &sample);
            rti::core::check_return_code(
                ok ? DDS_RETCODE_OK : DDS_RETCODE_ERROR,
                "Failed to calculate cdr buffer size");

            // Create a vector with that size and copy the cdr buffer into it
            buffer.resize(length);
            ok = BIUDDSStructPlugin_serialize_to_cdr_buffer(
                &buffer[0], 
                &length, 
                &sample);
            rti::core::check_return_code(
                ok ? DDS_RETCODE_OK : DDS_RETCODE_ERROR,
                "Failed to copy cdr buffer");

            return buffer;
        }

        void topic_type_support<BIUDDSStruct>::from_cdr_buffer(BIUDDSStruct& sample, 
        const std::vector<char>& buffer)
        {

            RTIBool ok  = BIUDDSStructPlugin_deserialize_from_cdr_buffer(
                &sample, 
                &buffer[0], 
                static_cast<unsigned int>(buffer.size()));
            rti::core::check_return_code(ok ? DDS_RETCODE_OK : DDS_RETCODE_ERROR,
            "Failed to create BIUDDSStruct from cdr buffer");
        }

        void topic_type_support<BIUDDSStruct>::reset_sample(BIUDDSStruct& sample) 
        {
            rti::topic::reset_sample(sample.sourceIP());
            rti::topic::reset_sample(sample.sourceID());
            rti::topic::reset_sample(sample.sequenceNumber());
            rti::topic::reset_sample(sample.tag());
            rti::topic::reset_sample(sample.payload());
        }

        void topic_type_support<BIUDDSStruct>::allocate_sample(BIUDDSStruct& sample, int, int) 
        {
            rti::topic::allocate_sample(sample.sourceIP(),  -1, 255);
            rti::topic::allocate_sample(sample.tag(),  -1, 255);
            rti::topic::allocate_sample(sample.payload(),  (MAX_SEQUENCE_SIZE), -1);
        }

    }
}  

