

/*
WARNING: THIS FILE IS AUTO-GENERATED. DO NOT MODIFY.

This file was generated from BIUDDSType.idl using "rtiddsgen".
The rtiddsgen tool is part of the RTI Connext distribution.
For more information, type 'rtiddsgen -help' at a command shell
or consult the RTI Connext manual.
*/

#ifndef BIUDDSTypePlugin_712083984_h
#define BIUDDSTypePlugin_712083984_h

#include "BIUDDSType.hpp"

struct RTICdrStream;

#ifndef pres_typePlugin_h
#include "pres/pres_typePlugin.h"
#endif

#if (defined(RTI_WIN32) || defined (RTI_WINCE)) && defined(NDDS_USER_DLL_EXPORT)
/* If the code is building on Windows, start exporting symbols.
*/
#undef NDDSUSERDllExport
#define NDDSUSERDllExport __declspec(dllexport)
#endif

/* The type used to store keys for instances of type struct
* AnotherSimple.
*
* By default, this type is struct BIUDDSStruct
* itself. However, if for some reason this choice is not practical for your
* system (e.g. if sizeof(struct BIUDDSStruct)
* is very large), you may redefine this typedef in terms of another type of
* your choosing. HOWEVER, if you define the KeyHolder type to be something
* other than struct AnotherSimple, the
* following restriction applies: the key of struct
* BIUDDSStruct must consist of a
* single field of your redefined KeyHolder type and that field must be the
* first field in struct BIUDDSStruct.
*/
typedef  class BIUDDSStruct BIUDDSStructKeyHolder;

#define BIUDDSStructPlugin_get_sample PRESTypePluginDefaultEndpointData_getSample 
#define BIUDDSStructPlugin_get_buffer PRESTypePluginDefaultEndpointData_getBuffer 
#define BIUDDSStructPlugin_return_buffer PRESTypePluginDefaultEndpointData_returnBuffer 

#define BIUDDSStructPlugin_get_key PRESTypePluginDefaultEndpointData_getKey 
#define BIUDDSStructPlugin_return_key PRESTypePluginDefaultEndpointData_returnKey

#define BIUDDSStructPlugin_create_sample PRESTypePluginDefaultEndpointData_createSample 
#define BIUDDSStructPlugin_destroy_sample PRESTypePluginDefaultEndpointData_deleteSample 

/* --------------------------------------------------------------------------------------
Support functions:
* -------------------------------------------------------------------------------------- */

NDDSUSERDllExport extern BIUDDSStruct*
BIUDDSStructPluginSupport_create_data_w_params(
    const struct DDS_TypeAllocationParams_t * alloc_params);

NDDSUSERDllExport extern BIUDDSStruct*
BIUDDSStructPluginSupport_create_data_ex(RTIBool allocate_pointers);

NDDSUSERDllExport extern BIUDDSStruct*
BIUDDSStructPluginSupport_create_data(void);

NDDSUSERDllExport extern RTIBool 
BIUDDSStructPluginSupport_copy_data(
    BIUDDSStruct *out,
    const BIUDDSStruct *in);

NDDSUSERDllExport extern void 
BIUDDSStructPluginSupport_destroy_data_w_params(
    BIUDDSStruct *sample,
    const struct DDS_TypeDeallocationParams_t * dealloc_params);

NDDSUSERDllExport extern void 
BIUDDSStructPluginSupport_destroy_data_ex(
    BIUDDSStruct *sample,RTIBool deallocate_pointers);

NDDSUSERDllExport extern void 
BIUDDSStructPluginSupport_destroy_data(
    BIUDDSStruct *sample);

NDDSUSERDllExport extern void 
BIUDDSStructPluginSupport_print_data(
    const BIUDDSStruct *sample,
    const char *desc,
    unsigned int indent);

NDDSUSERDllExport extern BIUDDSStruct*
BIUDDSStructPluginSupport_create_key_ex(RTIBool allocate_pointers);

NDDSUSERDllExport extern BIUDDSStruct*
BIUDDSStructPluginSupport_create_key(void);

NDDSUSERDllExport extern void 
BIUDDSStructPluginSupport_destroy_key_ex(
    BIUDDSStructKeyHolder *key,RTIBool deallocate_pointers);

NDDSUSERDllExport extern void 
BIUDDSStructPluginSupport_destroy_key(
    BIUDDSStructKeyHolder *key);

/* ----------------------------------------------------------------------------
Callback functions:
* ---------------------------------------------------------------------------- */

NDDSUSERDllExport extern PRESTypePluginParticipantData 
BIUDDSStructPlugin_on_participant_attached(
    void *registration_data, 
    const struct PRESTypePluginParticipantInfo *participant_info,
    RTIBool top_level_registration, 
    void *container_plugin_context,
    RTICdrTypeCode *typeCode);

NDDSUSERDllExport extern void 
BIUDDSStructPlugin_on_participant_detached(
    PRESTypePluginParticipantData participant_data);

NDDSUSERDllExport extern PRESTypePluginEndpointData 
BIUDDSStructPlugin_on_endpoint_attached(
    PRESTypePluginParticipantData participant_data,
    const struct PRESTypePluginEndpointInfo *endpoint_info,
    RTIBool top_level_registration, 
    void *container_plugin_context);

NDDSUSERDllExport extern void 
BIUDDSStructPlugin_on_endpoint_detached(
    PRESTypePluginEndpointData endpoint_data);

NDDSUSERDllExport extern void    
BIUDDSStructPlugin_return_sample(
    PRESTypePluginEndpointData endpoint_data,
    BIUDDSStruct *sample,
    void *handle);    

NDDSUSERDllExport extern RTIBool 
BIUDDSStructPlugin_copy_sample(
    PRESTypePluginEndpointData endpoint_data,
    BIUDDSStruct *out,
    const BIUDDSStruct *in);

/* ----------------------------------------------------------------------------
(De)Serialize functions:
* ------------------------------------------------------------------------- */

NDDSUSERDllExport extern RTIBool 
BIUDDSStructPlugin_serialize(
    PRESTypePluginEndpointData endpoint_data,
    const BIUDDSStruct *sample,
    struct RTICdrStream *stream, 
    RTIBool serialize_encapsulation,
    RTIEncapsulationId encapsulation_id,
    RTIBool serialize_sample, 
    void *endpoint_plugin_qos);

NDDSUSERDllExport extern RTIBool 
BIUDDSStructPlugin_deserialize_sample(
    PRESTypePluginEndpointData endpoint_data,
    BIUDDSStruct *sample, 
    struct RTICdrStream *stream,
    RTIBool deserialize_encapsulation,
    RTIBool deserialize_sample, 
    void *endpoint_plugin_qos);

NDDSUSERDllExport extern RTIBool
BIUDDSStructPlugin_serialize_to_cdr_buffer(
    char * buffer,
    unsigned int * length,
    const BIUDDSStruct *sample); 

NDDSUSERDllExport extern RTIBool 
BIUDDSStructPlugin_deserialize(
    PRESTypePluginEndpointData endpoint_data,
    BIUDDSStruct **sample, 
    RTIBool * drop_sample,
    struct RTICdrStream *stream,
    RTIBool deserialize_encapsulation,
    RTIBool deserialize_sample, 
    void *endpoint_plugin_qos);

NDDSUSERDllExport extern RTIBool
BIUDDSStructPlugin_deserialize_from_cdr_buffer(
    BIUDDSStruct *sample,
    const char * buffer,
    unsigned int length);    

NDDSUSERDllExport extern RTIBool
BIUDDSStructPlugin_skip(
    PRESTypePluginEndpointData endpoint_data,
    struct RTICdrStream *stream, 
    RTIBool skip_encapsulation,  
    RTIBool skip_sample, 
    void *endpoint_plugin_qos);

NDDSUSERDllExport extern unsigned int 
BIUDDSStructPlugin_get_serialized_sample_max_size_ex(
    PRESTypePluginEndpointData endpoint_data,
    RTIBool * overflow,
    RTIBool include_encapsulation,
    RTIEncapsulationId encapsulation_id,
    unsigned int current_alignment);    

NDDSUSERDllExport extern unsigned int 
BIUDDSStructPlugin_get_serialized_sample_max_size(
    PRESTypePluginEndpointData endpoint_data,
    RTIBool include_encapsulation,
    RTIEncapsulationId encapsulation_id,
    unsigned int current_alignment);

NDDSUSERDllExport extern unsigned int 
BIUDDSStructPlugin_get_serialized_sample_min_size(
    PRESTypePluginEndpointData endpoint_data,
    RTIBool include_encapsulation,
    RTIEncapsulationId encapsulation_id,
    unsigned int current_alignment);

NDDSUSERDllExport extern unsigned int
BIUDDSStructPlugin_get_serialized_sample_size(
    PRESTypePluginEndpointData endpoint_data,
    RTIBool include_encapsulation,
    RTIEncapsulationId encapsulation_id,
    unsigned int current_alignment,
    const BIUDDSStruct * sample);

/* --------------------------------------------------------------------------------------
Key Management functions:
* -------------------------------------------------------------------------------------- */
NDDSUSERDllExport extern PRESTypePluginKeyKind 
BIUDDSStructPlugin_get_key_kind(void);

NDDSUSERDllExport extern unsigned int 
BIUDDSStructPlugin_get_serialized_key_max_size_ex(
    PRESTypePluginEndpointData endpoint_data,
    RTIBool * overflow,
    RTIBool include_encapsulation,
    RTIEncapsulationId encapsulation_id,
    unsigned int current_alignment);

NDDSUSERDllExport extern unsigned int 
BIUDDSStructPlugin_get_serialized_key_max_size(
    PRESTypePluginEndpointData endpoint_data,
    RTIBool include_encapsulation,
    RTIEncapsulationId encapsulation_id,
    unsigned int current_alignment);

NDDSUSERDllExport extern RTIBool 
BIUDDSStructPlugin_serialize_key(
    PRESTypePluginEndpointData endpoint_data,
    const BIUDDSStruct *sample,
    struct RTICdrStream *stream,
    RTIBool serialize_encapsulation,
    RTIEncapsulationId encapsulation_id,
    RTIBool serialize_key,
    void *endpoint_plugin_qos);

NDDSUSERDllExport extern RTIBool 
BIUDDSStructPlugin_deserialize_key_sample(
    PRESTypePluginEndpointData endpoint_data,
    BIUDDSStruct * sample,
    struct RTICdrStream *stream,
    RTIBool deserialize_encapsulation,
    RTIBool deserialize_key,
    void *endpoint_plugin_qos);

NDDSUSERDllExport extern RTIBool 
BIUDDSStructPlugin_deserialize_key(
    PRESTypePluginEndpointData endpoint_data,
    BIUDDSStruct ** sample,
    RTIBool * drop_sample,
    struct RTICdrStream *stream,
    RTIBool deserialize_encapsulation,
    RTIBool deserialize_key,
    void *endpoint_plugin_qos);

NDDSUSERDllExport extern RTIBool
BIUDDSStructPlugin_serialized_sample_to_key(
    PRESTypePluginEndpointData endpoint_data,
    BIUDDSStruct *sample,
    struct RTICdrStream *stream, 
    RTIBool deserialize_encapsulation,  
    RTIBool deserialize_key, 
    void *endpoint_plugin_qos);

NDDSUSERDllExport extern RTIBool 
BIUDDSStructPlugin_instance_to_key(
    PRESTypePluginEndpointData endpoint_data,
    BIUDDSStructKeyHolder *key, 
    const BIUDDSStruct *instance);

NDDSUSERDllExport extern RTIBool 
BIUDDSStructPlugin_key_to_instance(
    PRESTypePluginEndpointData endpoint_data,
    BIUDDSStruct *instance, 
    const BIUDDSStructKeyHolder *key);

NDDSUSERDllExport extern RTIBool 
BIUDDSStructPlugin_instance_to_keyhash(
    PRESTypePluginEndpointData endpoint_data,
    DDS_KeyHash_t *keyhash,
    const BIUDDSStruct *instance);

NDDSUSERDllExport extern RTIBool 
BIUDDSStructPlugin_serialized_sample_to_keyhash(
    PRESTypePluginEndpointData endpoint_data,
    struct RTICdrStream *stream, 
    DDS_KeyHash_t *keyhash,
    RTIBool deserialize_encapsulation,
    void *endpoint_plugin_qos); 

/* Plugin Functions */
NDDSUSERDllExport extern struct PRESTypePlugin*
BIUDDSStructPlugin_new(void);

NDDSUSERDllExport extern void
BIUDDSStructPlugin_delete(struct PRESTypePlugin *);

#if (defined(RTI_WIN32) || defined (RTI_WINCE)) && defined(NDDS_USER_DLL_EXPORT)
/* If the code is building on Windows, stop exporting symbols.
*/
#undef NDDSUSERDllExport
#define NDDSUSERDllExport
#endif

#endif /* BIUDDSTypePlugin_712083984_h */

