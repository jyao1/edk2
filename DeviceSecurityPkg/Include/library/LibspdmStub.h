#ifndef __LIBSPDM_STUB_H__
#define __LIBSPDM_STUB_H__

/*Interface of base.h*/
typedef UINT64  uint64;
typedef INT64   int64;
typedef UINT32  uint32;
typedef INT32   int32;
typedef UINT16  uint16;
typedef INT16   int16;
typedef BOOLEAN boolean;
typedef UINT8   uint8;
typedef CHAR8   char8;
typedef INT8    int8;
typedef UINTN   uintn;
typedef INTN    intn;
typedef uintn return_status;

/*Interface of spdm.h*/
#define		SPDM_MESSAGE_HEADER		spdm_message_header_t 
#define		SPDM_GET_VERSION_REQUEST		spdm_get_version_request_t 
#define		SPDM_VERSION_NUMBER		spdm_version_number_t 
#define		SPDM_NEGOTIATE_ALGORITHMS_REQUEST		spdm_negotiate_algorithms_request_t 
#define		SPDM_NEGOTIATE_ALGORITHMS_STRUCT_TABLE		spdm_negotiate_algorithms_struct_table_t 
#define		SPDM_NEGOTIATE_ALGORITHMS_STRUCT_TABLE_ALG_COUNT		spdm_negotiate_algorithms_struct_table_alg_count_t 
#define		SPDM_NEGOTIATE_ALGORITHMS_COMMON_STRUCT_TABLE		spdm_negotiate_algorithms_common_struct_table_t 
#define		SPDM_ALGORITHMS_RESPONSE		spdm_algorithms_response_t 
#define		SPDM_EXTENDED_ALGORITHM		spdm_extended_algorithm_t 
#define		SPDM_GET_DIGEST_REQUEST		spdm_get_digest_request_t 
#define		SPDM_DIGEST_RESPONSE		spdm_digest_response_t 
#define		SPDM_GET_CERTIFICATE_REQUEST		spdm_get_certificate_request_t 
#define		SPDM_CERTIFICATE_RESPONSE		spdm_certificate_response_t 
#define		SPDM_CERT_CHAIN		spdm_cert_chain_t 
#define		SPDM_CHALLENGE_REQUEST		spdm_challenge_request_t 
#define		SPDM_CHALLENGE_AUTH_RESPONSE		spdm_challenge_auth_response_t 
#define		SPDM_CHALLENGE_AUTH_RESPONSE_ATTRIBUTE		spdm_challenge_auth_response_attribute_t 
#define		SPDM_GET_MEASUREMENTS_REQUEST		spdm_get_measurements_request_t 
#define		SPDM_GET_MEASUREMENTS_REQUEST_SLOT_ID_PARAMETER		spdm_get_measurements_request_slot_id_parameter_t 
#define		SPDM_MEASUREMENT_BLOCK_COMMON_HEADER		spdm_measurement_block_common_header_t 
#define		SPDM_MEASUREMENT_BLOCK_DMTF_HEADER		spdm_measurement_block_dmtf_header_t 
#define		SPDM_MEASUREMENT_BLOCK_DMTF		spdm_measurement_block_dmtf_t 
#define		SPDM_MEASUREMENTS_RESPONSE		spdm_measurements_response_t 
#define		SPDM_ERROR_RESPONSE		spdm_error_response_t 
#define		SPDM_ERROR_DATA_RESPONSE_NOT_READY		spdm_error_data_response_not_ready_t 
#define		SPDM_ERROR_RESPONSE_DATA_RESPONSE_NOT_READY		spdm_error_response_data_response_not_ready_t 
#define		SPDM_RESPONSE_IF_READY_REQUEST		spdm_response_if_ready_request_t 
#define		SPDM_VENDOR_DEFINED_REQUEST_MSG		spdm_vendor_defined_request_msg_t 
#define		SPDM_VENDOR_DEFINED_RESPONSE_MSG		spdm_vendor_defined_response_msg_t 
#define		SPDM_KEY_EXCHANGE_REQUEST		spdm_key_exchange_request_t 
#define		SPDM_KEY_EXCHANGE_RESPONSE		spdm_key_exchange_response_t 
#define		SPDM_FINISH_REQUEST		spdm_finish_request_t 
#define		SPDM_FINISH_RESPONSE		spdm_finish_response_t 
#define		SPDM_PSK_EXCHANGE_REQUEST		spdm_psk_exchange_request_t 
#define		SPDM_PSK_EXCHANGE_RESPONSE		spdm_psk_exchange_response_t 
#define		SPDM_PSK_FINISH_REQUEST		spdm_psk_finish_request_t 
#define		SPDM_PSK_FINISH_RESPONSE		spdm_psk_finish_response_t 
#define		SPDM_HEARTBEAT_REQUEST		spdm_heartbeat_request_t 
#define		SPDM_HEARTBEAT_RESPONSE		spdm_heartbeat_response_t 
#define		SPDM_KEY_UPDATE_REQUEST		spdm_key_update_request_t 
#define		SPDM_KEY_UPDATE_RESPONSE		spdm_key_update_response_t 
#define		SPDM_GET_ENCAPSULATED_REQUEST_REQUEST		spdm_get_encapsulated_request_request_t 
#define		SPDM_ENCAPSULATED_REQUEST_RESPONSE		spdm_encapsulated_request_response_t 
#define		SPDM_DELIVER_ENCAPSULATED_RESPONSE_REQUEST		spdm_deliver_encapsulated_response_request_t 
#define		SPDM_ENCAPSULATED_RESPONSE_ACK_RESPONSE		spdm_encapsulated_response_ack_response_t 
#define		SPDM_END_SESSION_REQUEST		spdm_end_session_request_t 
#define		SPDM_END_SESSION_RESPONSE		spdm_end_session_response_t 
/*Interface of spdm_common_lib.h*/
#define		SPDM_DATA_PARAMETER		spdm_data_parameter_t 


typedef enum {
  //
  // SPDM parameter
  //
  SpdmDataSpdmVersion,
  SpdmDataSecuredMessageVersion,
  //
  // SPDM capability
  //
  SpdmDataCapabilityFlags,
  SpdmDataCapabilityCTExponent,
  //
  // SPDM Algorithm setting
  //
  SpdmDataMeasurementSpec,
  SpdmDataMeasurementHashAlgo,
  SpdmDataBaseAsymAlgo,
  SpdmDataBaseHashAlgo,
  SpdmDataDHENamedGroup,
  SpdmDataAEADCipherSuite,
  SpdmDataReqBaseAsymAlg,
  SpdmDataKeySchedule,
  //
  // Connection State
  //
  SpdmDataConnectionState,
  //
  // ResponseState
  //
  SpdmDataResponseState,
  //
  // Certificate info
  //
  SpdmDataLocalPublicCertChain,
  SpdmDataLocalSlotCount,
  SpdmDataPeerPublicRootCertHash,
  SpdmDataPeerPublicCertChains,
  SpdmDataBasicMutAuthRequested,
  SpdmDataMutAuthRequested,
  //
  // Negotiated result
  //
  SpdmDataLocalUsedCertChainBuffer,
  SpdmDataPeerUsedCertChainBuffer,
  //
  // Pre-shared Key Hint
  // If PSK is present, then PSK_EXCHANGE is used.
  // Otherwise, the KEY_EXCHANGE is used.
  //
  SpdmDataPskHint,
  //
  // OpaqueData
  //
  SpdmDataOpaqueChallengeAuthRsp,
  SpdmDataOpaqueMeasurementRsp,
  SpdmDataOpaqueKeyExchangeReq,
  SpdmDataOpaqueKeyExchangeRsp,
  SpdmDataOpaquePskExchangeReq,
  SpdmDataOpaquePskExchangeRsp,
  //
  // SessionData
  //
  SpdmDataSessionUsePsk,
  SpdmDataSessionMutAuthRequested,
  SpdmDataSessionEndSessionAttributes,

  //
  // MAX
  //
  SpdmDataMax,
} SPDM_DATA_TYPE;

typedef enum {
  SpdmDataLocationLocal,
  SpdmDataLocationConnection,
  SpdmDataLocationSession,
  SpdmDataLocationMax,
} SPDM_DATA_LOCATION;

typedef enum {
  //
  // Before GET_VERSION/VERSION
  //
  SpdmConnectionStateNotStarted,
  //
  // After GET_VERSION/VERSION
  //
  SpdmConnectionStateAfterVersion,
  //
  // After GET_CAPABILITIES/CAPABILITIES
  //
  SpdmConnectionStateAfterCapabilities,
  //
  // After NEGOTIATE_ALGORITHMS/ALGORITHMS
  //
  SpdmConnectionStateNegotiated,
  //
  // After GET_DIGESTS/DIGESTS
  //
  SpdmConnectionStateAfterDigests,
  //
  // After GET_CERTIFICATE/CERTIFICATE
  //
  SpdmConnectionStateAfterCertificate,
  //
  // After CHALLENGE/CHALLENGE_AUTH, and ENCAP CALLENGE/CHALLENG_AUTH if MUT_AUTH is enabled.
  //
  SpdmConnectionStateAuthenticated,
  //
  // MAX
  //
  SpdmConnectionStateMax,
} SPDM_CONNECTION_STATE;

typedef enum {
  //
  // Normal response.
  //
  SpdmResponseStateNormal,
  //
  // Other component is busy.
  //
  SpdmResponseStateBusy,
  //
  // Hardware is not ready.
  //
  SpdmResponseStateNotReady,
  //
  // Firmware Update is done. Need resync.
  //
  SpdmResponseStateNeedResync,
  //
  // Processing Encapsulated message.
  //
  SpdmResponseStateProcessingEncap,
  //
  // MAX
  //
  SpdmResponseStateMax,
} SPDM_RESPONSE_STATE;

/*Interface of spdm_crypt_lib.h*/
/*Interface of spdm_device_secret_lib.h*/
/*Interface of spdm_lib_config.h*/
/*Interface of spdm_requester_lib.h*/
/*Interface of spdm_responder_lib.h*/
/*Interface of spdm_secured_message.h*/
#define		SPDM_SECURED_MESSAGE_A_DATA_HEADER1		spdm_secured_message_a_data_header1_t 
#define		SPDM_SECURED_MESSAGE_A_DATA_HEADER2		spdm_secured_message_a_data_header2_t 
#define		SPDM_SECURED_MESSAGE_CIPHER_HEADER		spdm_secured_message_cipher_header_t 
#define		SECURED_MESSAGE_GENERAL_OPAQUE_DATA_TABLE_HEADER		secured_message_general_opaque_data_table_header_t 
#define		OPAQUE_ELEMENT_TABLE_HEADER		opaque_element_table_header_t 
#define		SECURED_MESSAGE_OPAQUE_ELEMENT_TABLE_HEADER		secured_message_opaque_element_table_header_t 
#define		SECURED_MESSAGE_OPAQUE_ELEMENT_HEADER		secured_message_opaque_element_header_t 
#define		SECURED_MESSAGE_OPAQUE_ELEMENT_VERSION_SELECTION		secured_message_opaque_element_version_selection_t 
#define		SECURED_MESSAGE_OPAQUE_ELEMENT_SUPPORTED_VERSION		secured_message_opaque_element_supported_version_t 
/*Interface of spdm_secured_message_lib.h*/
#define		SPDM_SESSION_TYPE		spdm_session_type_t 
#define		SPDM_SESSION_STATE		spdm_session_state_t 
#define		SPDM_SECURE_SESSION_KEYS_STRUCT		spdm_secure_session_keys_struct_t 
#define		SPDM_KEY_UPDATE_ACTION		spdm_key_update_action_t 
#define		SPDM_SECURED_MESSAGE_CALLBACKS		spdm_secured_message_callbacks_t 
#define		SPDM_ERROR_STRUCT		spdm_error_struct_t 
/*Interface of spdm_transport_mctp_lib.h*/
/*Interface of spdm_transport_pcidoe_lib.h*/
/*Interface of pldm.h*/
#define     PLDM_MESSAGE_HEADER     pldm_message_header_t
#define     PLDM_MESSAGE_RESPONSE_HEADER     pldm_message_response_header_t
/*Interface of pcidoe.h*/
#define     PCI_DOE_DATA_OBJECT_HEADER    pci_doe_data_object_header_t
#define     PCI_DOE_DISCOVERY_REQUEST     pci_doe_discovery_request_t
#define     PCI_DOE_DISCOVERY_RESPONSE     pci_doe_discovery_response_t
/*Interface of mctp.h*/
#define     MCTP_HEADER       mctp_header_t
#define     MCTP_MESSAGE_HEADER       mctp_message_header_t
/*Interface of pci_idekm.h*/
#define		PCI_PROTOCOL_HEADER		pci_protocol_header_t;
#define		PCI_IDE_KM_HEADER		pci_ide_km_header_t;
#define		PCI_IDE_KM_QUERY		pci_ide_km_query_t;
#define		PCI_IDE_KM_QUERY_RESP		pci_ide_km_query_resp_t;
#define		PCI_IDE_KM_KEY_PROG		pci_ide_km_key_prog_t;
#define		PCI_IDE_KM_KP_ACK		pci_ide_km_kp_ack_t;
#define		PCI_IDE_KM_K_SET_GO		pci_ide_km_k_set_go_t;
#define		PCI_IDE_KM_K_SET_STOP		pci_ide_km_k_set_stop_t;
#define		PCI_IDE_KM_K_GOSTOP_ACK		pci_ide_km_k_gostop_ack_t;






/* FUNCTION */
/*Interface of BaseCryptLib.h*/

/*Interface of spdm.h*/
/*Interface of spdm_common_lib.h*/
#define		SpdmSetData		spdm_set_data
#define		SpdmGetData		spdm_get_data
#define		SpdmGetLastError		spdm_get_last_error
#define		SpdmGetLastSpdmErrorStruct		spdm_get_last_spdm_error_struct
#define		SpdmSetLastSpdmErrorStruct		spdm_set_last_spdm_error_struct
#define		SpdmInitContext		spdm_init_context
#define		SpdmGetContextSize		spdm_get_context_size
#define		SpdmRegisterDeviceIoFunc		spdm_register_device_io_func
#define		SpdmRegisterTransportLayerFunc		spdm_register_transport_layer_func
#define		SpdmResetMessageA		spdm_reset_message_a
#define		SpdmResetMessageB		spdm_reset_message_b
#define		SpdmResetMessageC		spdm_reset_message_c
#define		SpdmResetMessageMutB		spdm_reset_message_mut_b
#define		SpdmResetMessageMutC		spdm_reset_message_mut_c
#define		SpdmResetMessageM		spdm_reset_message_m
#define		SpdmAppendMessageA		spdm_append_message_a
#define		SpdmAppendMessageB		spdm_append_message_b
#define		SpdmAppendMessageC		spdm_append_message_c
#define		SpdmAppendMessageMutB		spdm_append_message_mut_b
#define		SpdmAppendMessageMutC		spdm_append_message_mut_c
#define		SpdmAppendMessageM		spdm_append_message_m
#define		SpdmAppendMessageK		spdm_append_message_k
#define		SpdmAppendMessageF		spdm_append_message_f
#define		SpdmGetSessionInfoViaSessionId		spdm_get_session_info_via_session_id
#define		SpdmGetSecuredMessageContextViaSessionId		spdm_get_secured_message_context_via_session_id
#define		SpdmGetSecuredMessageContextViaSessionInfo		spdm_get_secured_message_context_via_session_info
#define		SpdmAssignSessionId		spdm_assign_session_id
#define		SpdmFreeSessionId		spdm_free_session_id
#define		SpdmCalculateThForExchange		spdm_calculate_th_for_exchange
#define		SpdmCalculateThForFinish		spdm_calculate_th_for_finish
#define		SpdmCalculateTh1Hash		spdm_calculate_th1_hash
#define		SpdmCalculateTh2Hash		spdm_calculate_th2_hash
#define		SpdmGetPeerCertChainBuffer		spdm_get_peer_cert_chain_buffer
#define		SpdmGetPeerCertChainData		spdm_get_peer_cert_chain_data
#define		SpdmGetLocalCertChainBuffer		spdm_get_local_cert_chain_buffer
#define		SpdmGetLocalCertChainData		spdm_get_local_cert_chain_data
#define		SpdmReadUint24		spdm_read_uint24
#define		SpdmWriteUint24		spdm_write_uint24
/*Interface of spdm_common_lib_internal.h*/
#define		InternalDumpHexStr		internal_dump_hex_str
#define		InternalDumpData		internal_dump_data
#define		InternalDumpHex		internal_dump_hex
#define		AppendManagedBuffer		append_managed_buffer
#define		ShrinkManagedBuffer		shrink_managed_buffer
#define		ResetManagedBuffer		reset_managed_buffer
#define		GetManagedBufferSize		get_managed_buffer_size
#define		getManagedBuffer		get_managed_buffer
#define		InitManagedBuffer		init_managed_buffer
#define		SpdmSessionInfoInit		spdm_session_info_init
#define		SpdmAllocateReqSessionId		spdm_allocate_req_session_id
#define		SpdmAllocateRspSessionId		spdm_allocate_rsp_session_id
#define		SpdmIsVersionSupported		spdm_is_version_supported
#define		SpdmIsCapabilitiesFlagSupported		spdm_is_capabilities_flag_supported
#define		SpdmCalculateM1m2		spdm_calculate_m1m2
#define		SpdmCalculateL1l2		spdm_calculate_l1l2
#define		SpdmGenerateCertChainHash		spdm_generate_cert_chain_hash
#define		SpdmVerifyPeerDigests		spdm_verify_peer_digests
#define		SpdmVerifyPeerCertChainBuffer		spdm_verify_peer_cert_chain_buffer
#define		SpdmGenerateChallengeAuthSignature		spdm_generate_challenge_auth_signature
#define		SpdmVerifyCertificateChainHash		spdm_verify_certificate_chain_hash
#define		SpdmVerifyChallengeAuthSignature		spdm_verify_challenge_auth_signature
#define		SpdmGetMeasurementSummaryHashSize		spdm_get_measurement_summary_hash_size
#define		SpdmGenerateMeasurementSummaryHash		spdm_generate_measurement_summary_hash
#define		SpdmGenerateMeasurementSignature		spdm_generate_measurement_signature
#define		SpdmVerifyMeasurementSignature		spdm_verify_measurement_signature
#define		SpdmGenerateKeyExchangeRspSignature		spdm_generate_key_exchange_rsp_signature
#define		SpdmGenerateKeyExchangeRspHmac		spdm_generate_key_exchange_rsp_hmac
#define		SpdmVerifyKeyExchangeRspSignature		spdm_verify_key_exchange_rsp_signature
#define		SpdmVerifyKeyExchangeRspHmac		spdm_verify_key_exchange_rsp_hmac
#define		SpdmGenerateFinishReqSignature		spdm_generate_finish_req_signature
#define		SpdmGenerateFinishReqHmac		spdm_generate_finish_req_hmac
#define		SpdmVerifyFinishReqSignature		spdm_verify_finish_req_signature
#define		SpdmVerifyFinishReqHmac		spdm_verify_finish_req_hmac
#define		SpdmGenerateFinishRspHmac		spdm_generate_finish_rsp_hmac
#define		SpdmVerifyFinishRspHmac		spdm_verify_finish_rsp_hmac
#define		SpdmGeneratePskExchangeRspHmac		spdm_generate_psk_exchange_rsp_hmac
#define		SpdmVerifyPskExchangeRspHmac		spdm_verify_psk_exchange_rsp_hmac
#define		SpdmGeneratePskExchangeReqHmac		spdm_generate_psk_exchange_req_hmac
#define		SpdmVerifyPskFinishReqHmac		spdm_verify_psk_finish_req_hmac
#define		SpdmGetOpaqueDataSupportedVersionDataSize		spdm_get_opaque_data_supported_version_data_size
#define		SpdmBuildOpaqueDataSupportedVersionData		spdm_build_opaque_data_supported_version_data
#define		SpdmProcessOpaqueDataVersionSelectionData		spdm_process_opaque_data_version_selection_data
#define		SpdmGetOpaqueDataVersionSelectionDataSize		spdm_get_opaque_data_version_selection_data_size
#define		SpdmBuildOpaqueDataVersionSelectionData		spdm_build_opaque_data_version_selection_data
#define		SpdmProcessOpaqueDataSupportedVersionData		spdm_process_opaque_data_supported_version_data
/*Interface of spdm_crypt_lib.h*/
#define		GetSpdmHashSize		spdm_get_hash_size
#define		SpdmHashAll		spdm_hash_all
#define		GetSpdmMeasurementHashSize		spdm_get_measurement_hash_size
#define		SpdmMeasurementHashAll		spdm_measurement_hash_all
#define		SpdmHmacAll		spdm_hmac_all
#define		SpdmHkdfExpand		spdm_hkdf_expand
#define		SpdmGetAsymSignatureSize		spdm_get_asym_signature_size
#define		SpdmAsymGetPublicKeyFromX509		spdm_asym_get_public_key_from_x509
#define		SpdmAsymFree		spdm_asym_free
#define		SpdmAsymVerify		spdm_asym_verify
#define		SpdmAsymGetPrivateKeyFromPem		spdm_asym_get_private_key_from_pem
#define		SpdmAsymSign		spdm_asym_sign
#define		SpdmGetReqAsymSignatureSize		spdm_get_req_asym_signature_size
#define		SpdmReqAsymGetPublicKeyFromX509		spdm_req_asym_get_public_key_from_x509
#define		SpdmReqAsymFree		spdm_req_asym_free
#define		SpdmReqAsymVerify		spdm_req_asym_verify
#define		SpdmReqAsymGetPrivateKeyFromPem		spdm_req_asym_get_private_key_from_pem
#define		SpdmReqAsymSign		spdm_req_asym_sign
#define		SpdmGetDhePubKeySize		spdm_get_dhe_pub_key_size
#define		SpdmDheNew		spdm_dhe_new
#define		SpdmDheFree		spdm_dhe_free
#define		SpdmDheGenerateKey		spdm_dhe_generate_key
#define		SpdmDheComputeKey		spdm_dhe_compute_key
#define		SpdmGetAeadKeySize		spdm_get_aead_key_size
#define		SpdmGetAeadIvSize		spdm_get_aead_iv_size
#define		SpdmGetAeadTagSize		spdm_get_aead_tag_size
#define		SpdmGetAeadBlockSize		spdm_get_aead_block_size
#define		SpdmAeadEncryption		spdm_aead_encryption
#define		SpdmAeadDecryption		spdm_aead_decryption
#define		SpdmGetRandomNumber		spdm_get_random_number
#define		SpdmX509CertificateCheck		spdm_x509_certificate_check
#define		SpdmGetDmtfSubjectAltNameFromBytes		spdm_get_dmtf_subject_alt_name_from_bytes
#define		SpdmGetDmtfSubjectAltName		spdm_get_dmtf_subject_alt_name
#define		SpdmVerifyCertChainData		spdm_verify_cert_chain_data
#define		SpdmVerifyCertificateChainBuffer		spdm_verify_certificate_chain_buffer
/*Interface of spdm_device_secret_lib.h*/
// #define		SpdmMeasurementCollection		spdm_measurement_collection
// #define		SpdmRequesterDataSign		spdm_requester_data_sign
// #define		SpdmResponderDataSign		spdm_responder_data_sign
// #define		SpdmPskHandshakeSecretHkdfExpand		spdm_psk_handshake_secret_hkdf_expand
// #define		SpdmPskMasterSecretHkdfExpand		spdm_psk_master_secret_hkdf_expand
#define		spdm_measurement_collection             SpdmMeasurementCollectionFunc		
#define		spdm_requester_data_sign                SpdmRequesterDataSignFunc		
#define		spdm_responder_data_sign                SpdmResponderDataSignFunc		
#define		spdm_psk_handshake_secret_hkdf_expand      SpdmPskHandshakeSecretHkdfExpandFunc		
#define		spdm_psk_master_secret_hkdf_expand          SpdmPskMasterSecretHkdfExpandFunc		
/*Interface of spdm_lib_config.h*/
/*Interface of spdm_requester_lib.h*/
#define		SpdmSendRequest		spdm_send_request
#define		SpdmReceiveResponse		spdm_receive_response
#define		SpdmInitConnection		spdm_init_connection
#define		SpdmGetDigest		spdm_get_digest
#define		SpdmGetCertificate		spdm_get_certificate
#define		SpdmGetCertificateChooseLength		spdm_get_certificate_choose_length
#define		SpdmChallenge		spdm_challenge
#define		SpdmGetMeasurement		spdm_get_measurement
#define		SpdmStartSession		spdm_start_session
#define		SpdmStopSession		spdm_stop_session
#define		SpdmSendReceiveData		spdm_send_receive_data
#define		SpdmHeartbeat		spdm_heartbeat
#define		SpdmKeyUpdate		spdm_key_update
#define		SpdmSendReceiveEncapRequest		spdm_send_receive_encap_request
#define		SpdmRegisterGetEncapResponseFunc		spdm_register_get_encap_response_func
#define		SpdmGenerateEncapErrorResponse		spdm_generate_encap_error_response
#define		SpdmGenerateEncapExtendedErrorResponse		spdm_generate_encap_extended_error_response
/*Interface of spdm_responder_lib.h*/
#define		SpdmRegisterGetResponseFunc		spdm_register_get_response_func
#define		SpdmProcessRequest		spdm_process_request
#define		SpdmBuildResponse		spdm_build_response
#define		SpdmProcessMessage		spdm_process_message
#define		SpdmResponderDispatchMessage		spdm_responder_dispatch_message
#define		SpdmGenerateErrorResponse		spdm_generate_error_response
#define		SpdmGenerateExtendedErrorResponse		spdm_generate_extended_error_response
#define		SpdmRegisterSessionStateCallbackFunc		spdm_register_session_state_callback_func
#define		SpdmRegisterConnectionStateCallbackFunc		spdm_register_connection_state_callback_func
#define		SpdmInitKeyUpdateEncapState		spdm_init_key_update_encap_state
/*Interface of spdm_secured_message.h*/
/*Interface of spdm_secured_message_lib.h*/
#define		SpdmSecuredMessageGetContextSize		spdm_secured_message_get_context_size
#define		SpdmSecuredMessageInitContext		spdm_secured_message_init_context
#define		SpdmSecuredMessageSetUsePsk		spdm_secured_message_set_use_psk
#define		SpdmSecuredMessageSetSessionState		spdm_secured_message_set_session_state
#define		SpdmSecuredMessageGetSessionState		spdm_secured_message_get_session_state
#define		SpdmSecuredMessageSetSessionType		spdm_secured_message_set_session_type
#define		SpdmSecuredMessageSetAlgorithms		spdm_secured_message_set_algorithms
#define		SpdmSecuredMessageSetPskHint		spdm_secured_message_set_psk_hint
#define		SpdmSecuredMessageImportDheSecret		spdm_secured_message_import_dhe_secret
#define		SpdmSecuredMessageExportMasterSecret		spdm_secured_message_export_master_secret
#define		SpdmSecuredMessageExportSessionKeys		spdm_secured_message_export_session_keys
#define		SpdmSecuredMessageImportSessionKeys		spdm_secured_message_import_session_keys
#define		SpdmSecuredMessageDheNew		spdm_secured_message_dhe_new
#define		SpdmSecuredMessageDheFree		spdm_secured_message_dhe_free
#define		SpdmSecuredMessageDheGenerateKey		spdm_secured_message_dhe_generate_key
#define		SpdmSecuredMessageDheComputeKey		spdm_secured_message_dhe_compute_key
#define		SpdmHmacAllWithRequestFinishedKey		spdm_hmac_all_with_request_finished_key
#define		SpdmHmacAllWithResponseFinishedKey		spdm_hmac_all_with_response_finished_key
#define		SpdmBinConcat		spdm_bin_concat
#define		SpdmGenerateSessionHandshakeKey		spdm_generate_session_handshake_key
#define		SpdmGenerateSessionDataKey		spdm_generate_session_data_key
#define		SpdmCreateUpdateSessionDataKey		spdm_create_update_session_data_key
#define		SpdmActivateUpdateSessionDataKey		spdm_activate_update_session_data_key
#define		SpdmEncodeSecuredMessage		spdm_encode_secured_message
#define		SpdmDecodeSecuredMessage		spdm_decode_secured_message
#define		SpdmSecuredMessageGetLastSpdmErrorStruct		spdm_secured_message_get_last_spdm_error_struct
#define		SpdmSecuredMessageSetLastSpdmErrorStruct		spdm_secured_message_set_last_spdm_error_struct
/*Interface of spdm_transport_mctp_lib.h*/
#define		SpdmTransportMctpEncodeMessage		spdm_transport_mctp_encode_message
#define		SpdmTransportMctpDecodeMessage		spdm_transport_mctp_decode_message
#define		SpdmMctpGetSequenceNumber		spdm_mctp_get_sequence_number
#define		SpdmMctpGetMaxRandomNumberCount		spdm_mctp_get_max_random_number_count
/*Interface of spdm_transport_pcidoe_lib.h*/
#define		SpdmTransportPciDoeEncodeMessage		spdm_transport_pci_doe_encode_message
#define		SpdmTransportPciDoeDecodeMessage		spdm_transport_pci_doe_decode_message
#define		SpdmPciDoeGetSequenceNumber		spdm_pci_doe_get_sequence_number
#define		SpdmPciDoeGetMaxRandomNumberCount		spdm_pci_doe_get_max_random_number_count


#endif