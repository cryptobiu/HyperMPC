

/*
WARNING: THIS FILE IS AUTO-GENERATED. DO NOT MODIFY.

This file was generated from Test.idl using "rtiddsgen".
The rtiddsgen tool is part of the RTI Connext distribution.
For more information, type 'rtiddsgen -help' at a command shell
or consult the RTI Connext manual.
*/

#ifndef TestPlugin_1083273499_h
#define TestPlugin_1083273499_h

#include "Test.hpp"

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

#define MyStructPlugin_get_sample PRESTypePluginDefaultEndpointData_getSample 
#define MyStructPlugin_get_buffer PRESTypePluginDefaultEndpointData_getBuffer 
#define MyStructPlugin_return_buffer PRESTypePluginDefaultEndpointData_returnBuffer 

#define MyStructPlugin_create_sample PRESTypePluginDefaultEndpointData_createSample 
#define MyStructPlugin_destroy_sample PRESTypePluginDefaultEndpointData_deleteSample 

/* --------------------------------------------------------------------------------------
Support functions:
* -------------------------------------------------------------------------------------- */

NDDSUSERDllExport extern MyStruct*
MyStructPluginSupport_create_data_w_params(
    const struct DDS_TypeAllocationParams_t * alloc_params);

NDDSUSERDllExport extern MyStruct*
MyStructPluginSupport_create_data_ex(RTIBool allocate_pointers);

NDDSUSERDllExport extern MyStruct*
MyStructPluginSupport_create_data(void);

NDDSUSERDllExport extern RTIBool 
MyStructPluginSupport_copy_data(
    MyStruct *out,
    const MyStruct *in);

NDDSUSERDllExport extern void 
MyStructPluginSupport_destroy_data_w_params(
    MyStruct *sample,
    const struct DDS_TypeDeallocationParams_t * dealloc_params);

NDDSUSERDllExport extern void 
MyStructPluginSupport_destroy_data_ex(
    MyStruct *sample,RTIBool deallocate_pointers);

NDDSUSERDllExport extern void 
MyStructPluginSupport_destroy_data(
    MyStruct *sample);

NDDSUSERDllExport extern void 
MyStructPluginSupport_print_data(
    const MyStruct *sample,
    const char *desc,
    unsigned int indent);

/* ----------------------------------------------------------------------------
Callback functions:
* ---------------------------------------------------------------------------- */

NDDSUSERDllExport extern PRESTypePluginParticipantData 
MyStructPlugin_on_participant_attached(
    void *registration_data, 
    const struct PRESTypePluginParticipantInfo *participant_info,
    RTIBool top_level_registration, 
    void *container_plugin_context,
    RTICdrTypeCode *typeCode);

NDDSUSERDllExport extern void 
MyStructPlugin_on_participant_detached(
    PRESTypePluginParticipantData participant_data);

NDDSUSERDllExport extern PRESTypePluginEndpointData 
MyStructPlugin_on_endpoint_attached(
    PRESTypePluginParticipantData participant_data,
    const struct PRESTypePluginEndpointInfo *endpoint_info,
    RTIBool top_level_registration, 
    void *container_plugin_context);

NDDSUSERDllExport extern void 
MyStructPlugin_on_endpoint_detached(
    PRESTypePluginEndpointData endpoint_data);

NDDSUSERDllExport extern void    
MyStructPlugin_return_sample(
    PRESTypePluginEndpointData endpoint_data,
    MyStruct *sample,
    void *handle);    

NDDSUSERDllExport extern RTIBool 
MyStructPlugin_copy_sample(
    PRESTypePluginEndpointData endpoint_data,
    MyStruct *out,
    const MyStruct *in);

/* ----------------------------------------------------------------------------
(De)Serialize functions:
* ------------------------------------------------------------------------- */

NDDSUSERDllExport extern RTIBool 
MyStructPlugin_serialize(
    PRESTypePluginEndpointData endpoint_data,
    const MyStruct *sample,
    struct RTICdrStream *stream, 
    RTIBool serialize_encapsulation,
    RTIEncapsulationId encapsulation_id,
    RTIBool serialize_sample, 
    void *endpoint_plugin_qos);

NDDSUSERDllExport extern RTIBool 
MyStructPlugin_deserialize_sample(
    PRESTypePluginEndpointData endpoint_data,
    MyStruct *sample, 
    struct RTICdrStream *stream,
    RTIBool deserialize_encapsulation,
    RTIBool deserialize_sample, 
    void *endpoint_plugin_qos);

NDDSUSERDllExport extern RTIBool
MyStructPlugin_serialize_to_cdr_buffer(
    char * buffer,
    unsigned int * length,
    const MyStruct *sample); 

NDDSUSERDllExport extern RTIBool 
MyStructPlugin_deserialize(
    PRESTypePluginEndpointData endpoint_data,
    MyStruct **sample, 
    RTIBool * drop_sample,
    struct RTICdrStream *stream,
    RTIBool deserialize_encapsulation,
    RTIBool deserialize_sample, 
    void *endpoint_plugin_qos);

NDDSUSERDllExport extern RTIBool
MyStructPlugin_deserialize_from_cdr_buffer(
    MyStruct *sample,
    const char * buffer,
    unsigned int length);    

NDDSUSERDllExport extern RTIBool
MyStructPlugin_skip(
    PRESTypePluginEndpointData endpoint_data,
    struct RTICdrStream *stream, 
    RTIBool skip_encapsulation,  
    RTIBool skip_sample, 
    void *endpoint_plugin_qos);

NDDSUSERDllExport extern unsigned int 
MyStructPlugin_get_serialized_sample_max_size_ex(
    PRESTypePluginEndpointData endpoint_data,
    RTIBool * overflow,
    RTIBool include_encapsulation,
    RTIEncapsulationId encapsulation_id,
    unsigned int current_alignment);    

NDDSUSERDllExport extern unsigned int 
MyStructPlugin_get_serialized_sample_max_size(
    PRESTypePluginEndpointData endpoint_data,
    RTIBool include_encapsulation,
    RTIEncapsulationId encapsulation_id,
    unsigned int current_alignment);

NDDSUSERDllExport extern unsigned int 
MyStructPlugin_get_serialized_sample_min_size(
    PRESTypePluginEndpointData endpoint_data,
    RTIBool include_encapsulation,
    RTIEncapsulationId encapsulation_id,
    unsigned int current_alignment);

NDDSUSERDllExport extern unsigned int
MyStructPlugin_get_serialized_sample_size(
    PRESTypePluginEndpointData endpoint_data,
    RTIBool include_encapsulation,
    RTIEncapsulationId encapsulation_id,
    unsigned int current_alignment,
    const MyStruct * sample);

/* --------------------------------------------------------------------------------------
Key Management functions:
* -------------------------------------------------------------------------------------- */
NDDSUSERDllExport extern PRESTypePluginKeyKind 
MyStructPlugin_get_key_kind(void);

NDDSUSERDllExport extern unsigned int 
MyStructPlugin_get_serialized_key_max_size_ex(
    PRESTypePluginEndpointData endpoint_data,
    RTIBool * overflow,
    RTIBool include_encapsulation,
    RTIEncapsulationId encapsulation_id,
    unsigned int current_alignment);

NDDSUSERDllExport extern unsigned int 
MyStructPlugin_get_serialized_key_max_size(
    PRESTypePluginEndpointData endpoint_data,
    RTIBool include_encapsulation,
    RTIEncapsulationId encapsulation_id,
    unsigned int current_alignment);

NDDSUSERDllExport extern RTIBool 
MyStructPlugin_serialize_key(
    PRESTypePluginEndpointData endpoint_data,
    const MyStruct *sample,
    struct RTICdrStream *stream,
    RTIBool serialize_encapsulation,
    RTIEncapsulationId encapsulation_id,
    RTIBool serialize_key,
    void *endpoint_plugin_qos);

NDDSUSERDllExport extern RTIBool 
MyStructPlugin_deserialize_key_sample(
    PRESTypePluginEndpointData endpoint_data,
    MyStruct * sample,
    struct RTICdrStream *stream,
    RTIBool deserialize_encapsulation,
    RTIBool deserialize_key,
    void *endpoint_plugin_qos);

NDDSUSERDllExport extern RTIBool 
MyStructPlugin_deserialize_key(
    PRESTypePluginEndpointData endpoint_data,
    MyStruct ** sample,
    RTIBool * drop_sample,
    struct RTICdrStream *stream,
    RTIBool deserialize_encapsulation,
    RTIBool deserialize_key,
    void *endpoint_plugin_qos);

NDDSUSERDllExport extern RTIBool
MyStructPlugin_serialized_sample_to_key(
    PRESTypePluginEndpointData endpoint_data,
    MyStruct *sample,
    struct RTICdrStream *stream, 
    RTIBool deserialize_encapsulation,  
    RTIBool deserialize_key, 
    void *endpoint_plugin_qos);

/* Plugin Functions */
NDDSUSERDllExport extern struct PRESTypePlugin*
MyStructPlugin_new(void);

NDDSUSERDllExport extern void
MyStructPlugin_delete(struct PRESTypePlugin *);

#if (defined(RTI_WIN32) || defined (RTI_WINCE)) && defined(NDDS_USER_DLL_EXPORT)
/* If the code is building on Windows, stop exporting symbols.
*/
#undef NDDSUSERDllExport
#define NDDSUSERDllExport
#endif

#endif /* TestPlugin_1083273499_h */

