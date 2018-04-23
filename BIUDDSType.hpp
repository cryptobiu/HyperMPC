

/*
WARNING: THIS FILE IS AUTO-GENERATED. DO NOT MODIFY.

This file was generated from BIUDDSType.idl using "rtiddsgen".
The rtiddsgen tool is part of the RTI Connext distribution.
For more information, type 'rtiddsgen -help' at a command shell
or consult the RTI Connext manual.
*/

#ifndef BIUDDSType_712083984_hpp
#define BIUDDSType_712083984_hpp

#include <iosfwd>

#if (defined(RTI_WIN32) || defined (RTI_WINCE)) && defined(NDDS_USER_DLL_EXPORT)
/* If the code is building on Windows, start exporting symbols.
*/
#undef RTIUSERDllExport
#define RTIUSERDllExport __declspec(dllexport)
#endif

#include "dds/domain/DomainParticipant.hpp"
#include "dds/topic/TopicTraits.hpp"
#include "dds/core/SafeEnumeration.hpp"
#include "dds/core/String.hpp"
#include "dds/core/array.hpp"
#include "dds/core/vector.hpp"
#include "dds/core/Optional.hpp"
#include "dds/core/xtypes/DynamicType.hpp"
#include "dds/core/xtypes/StructType.hpp"
#include "dds/core/xtypes/UnionType.hpp"
#include "dds/core/xtypes/EnumType.hpp"
#include "dds/core/xtypes/AliasType.hpp"
#include "rti/core/array.hpp"
#include "rti/core/BoundedSequence.hpp"
#include "rti/util/StreamFlagSaver.hpp"
#include "rti/domain/PluginSupport.hpp"
#include "rti/core/LongDouble.hpp"
#include "dds/core/External.hpp"
#include "rti/core/Pointer.hpp"
#include "rti/topic/TopicTraits.hpp"
#if (defined(RTI_WIN32) || defined (RTI_WINCE)) && defined(NDDS_USER_DLL_EXPORT)
/* If the code is building on Windows, stop exporting symbols.
*/
#undef RTIUSERDllExport
#define RTIUSERDllExport
#endif

#if (defined(RTI_WIN32) || defined (RTI_WINCE)) && defined(NDDS_USER_DLL_EXPORT)
/* If the code is building on Windows, start exporting symbols.
*/
#undef NDDSUSERDllExport
#define NDDSUSERDllExport __declspec(dllexport)
#endif

static const int32_t MAX_SEQUENCE_SIZE = 4000000;

#if (defined(RTI_WIN32) || defined (RTI_WINCE)) && defined(NDDS_USER_DLL_EXPORT)
// On Windows, dll-export template instantiations of standard types used by
// other dll-exported types
template class NDDSUSERDllExport std::allocator<char>;
template class NDDSUSERDllExport std::vector<char >;
#endif
class NDDSUSERDllExport BIUDDSStruct {

  public:
    BIUDDSStruct();
    BIUDDSStruct(
        const std::string& sourceIP,
        uint32_t sourceID,
        int32_t sequenceNumber,
        const std::string& tag,
        const rti::core::bounded_sequence<char, (MAX_SEQUENCE_SIZE)>& payload);

    #ifdef RTI_CXX11_RVALUE_REFERENCES
    #ifndef RTI_CXX11_NO_IMPLICIT_MOVE_OPERATIONS
    BIUDDSStruct (BIUDDSStruct&&) = default;
    BIUDDSStruct& operator=(BIUDDSStruct&&) = default;
    BIUDDSStruct& operator=(const BIUDDSStruct&) = default;
    BIUDDSStruct(const BIUDDSStruct&) = default;
    #else
    BIUDDSStruct(BIUDDSStruct&& other_) OMG_NOEXCEPT;  
    BIUDDSStruct& operator=(BIUDDSStruct&&  other_) OMG_NOEXCEPT;
    #endif
    #endif 

    std::string& sourceIP() OMG_NOEXCEPT; 
    const std::string& sourceIP() const OMG_NOEXCEPT;
    void sourceIP(const std::string& value);

    uint32_t& sourceID() OMG_NOEXCEPT; 
    const uint32_t& sourceID() const OMG_NOEXCEPT;
    void sourceID(uint32_t value);

    int32_t& sequenceNumber() OMG_NOEXCEPT; 
    const int32_t& sequenceNumber() const OMG_NOEXCEPT;
    void sequenceNumber(int32_t value);

    std::string& tag() OMG_NOEXCEPT; 
    const std::string& tag() const OMG_NOEXCEPT;
    void tag(const std::string& value);

    rti::core::bounded_sequence<char, (MAX_SEQUENCE_SIZE)>& payload() OMG_NOEXCEPT; 
    const rti::core::bounded_sequence<char, (MAX_SEQUENCE_SIZE)>& payload() const OMG_NOEXCEPT;
    void payload(const rti::core::bounded_sequence<char, (MAX_SEQUENCE_SIZE)>& value);

    bool operator == (const BIUDDSStruct& other_) const;
    bool operator != (const BIUDDSStruct& other_) const;

    void swap(BIUDDSStruct& other_) OMG_NOEXCEPT ;

  private:

    std::string m_sourceIP_;
    uint32_t m_sourceID_;
    int32_t m_sequenceNumber_;
    std::string m_tag_;
    rti::core::bounded_sequence<char, (MAX_SEQUENCE_SIZE)> m_payload_;

};

inline void swap(BIUDDSStruct& a, BIUDDSStruct& b)  OMG_NOEXCEPT 
{
    a.swap(b);
}

NDDSUSERDllExport std::ostream& operator<<(std::ostream& o, const BIUDDSStruct& sample);

namespace dds { 
    namespace topic {

        template<>
        struct topic_type_name<BIUDDSStruct> {
            NDDSUSERDllExport static std::string value() {
                return "BIUDDSStruct";
            }
        };

        template<>
        struct is_topic_type<BIUDDSStruct> : public dds::core::true_type {};

        template<>
        struct topic_type_support<BIUDDSStruct> {
            NDDSUSERDllExport 
            static void register_type(
                dds::domain::DomainParticipant& participant,
                const std::string & type_name);

            NDDSUSERDllExport 
            static std::vector<char>& to_cdr_buffer(
                std::vector<char>& buffer, const BIUDDSStruct& sample);

            NDDSUSERDllExport 
            static void from_cdr_buffer(BIUDDSStruct& sample, const std::vector<char>& buffer);

            NDDSUSERDllExport 
            static void reset_sample(BIUDDSStruct& sample);

            NDDSUSERDllExport 
            static void allocate_sample(BIUDDSStruct& sample, int, int);

            static const rti::topic::TypePluginKind::type type_plugin_kind = 
            rti::topic::TypePluginKind::STL;
        };

    }
}

namespace rti { 
    namespace topic {

        template<>
        struct dynamic_type<BIUDDSStruct> {
            typedef dds::core::xtypes::StructType type;
            NDDSUSERDllExport static const dds::core::xtypes::StructType& get();
        };

        template <>
        struct extensibility<BIUDDSStruct> {
            static const dds::core::xtypes::ExtensibilityKind::type kind =
            dds::core::xtypes::ExtensibilityKind::EXTENSIBLE;                
        };

    }
}

#if (defined(RTI_WIN32) || defined (RTI_WINCE)) && defined(NDDS_USER_DLL_EXPORT)
/* If the code is building on Windows, stop exporting symbols.
*/
#undef NDDSUSERDllExport
#define NDDSUSERDllExport
#endif

#endif // BIUDDSType_712083984_hpp

