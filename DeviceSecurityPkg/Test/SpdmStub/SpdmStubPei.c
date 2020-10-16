/** @file
  EDKII SpdmIo Stub

  Copyright (c) 2020, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <Library/PeimEntryPoint.h>
#include <Library/PeiServicesLib.h>
#include <Library/PeiServicesTablePointerLib.h>
#include <Ppi/ReadOnlyVariable2.h>
#include "SpdmStubPei.h"

#define SHA256_HASH_SIZE  32

#define BLOCK_NUMBER   4
#define SLOT_NUMBER    2

#pragma pack(1)

typedef struct {
  SPDM_MEASUREMENT_BLOCK_COMMON_HEADER  MeasurementBlockCommonHeader;
  SPDM_MEASUREMENT_BLOCK_DMTF_HEADER    MeasurementBlockDmtfHeader;
  UINT8                                 HashValue[SHA256_HASH_SIZE];
} MY_SPDM_MEASUREMENT_BLOCK;

#pragma pack()

SPDM_MESSAGE_HEADER  *mSpdmIoLastSpdmRequest;
UINTN                mSpdmIoLastSpdmRequestSize;
UINT32               mSpdmIoLastSpdmSessionId;
UINT32               *mSpdmIoLastSpdmSessionIdPtr;

BOOLEAN
EFIAPI
SpdmDataSignFunc (
  IN      VOID         *SpdmContext,
  IN      BOOLEAN      IsResponder,
  IN      UINT32       AsymAlgo,
  IN      CONST UINT8  *MessageHash,
  IN      UINTN        HashSize,
  OUT     UINT8        *Signature,
  IN OUT  UINTN        *SigSize
  );
extern VOID  *mResponderPrivateCertData;
extern UINTN mResponderPrivateCertDataSize;


BOOLEAN
RegisterMeasurement (
  OUT VOID                            **DeviceMeasurement,
  OUT UINTN                           *DeviceMeasurementSize,
  OUT UINT8                           *DeviceMeasurementCount
  )
{
  MY_SPDM_MEASUREMENT_BLOCK    *MeasurementBlock;
  UINT16                       HashSize;
  UINT8                        Index;

  *DeviceMeasurementCount = BLOCK_NUMBER;
  *DeviceMeasurement = AllocateZeroPool (BLOCK_NUMBER * sizeof(MY_SPDM_MEASUREMENT_BLOCK));
  if (*DeviceMeasurement == NULL) {
    return FALSE;
  }

  MeasurementBlock = *DeviceMeasurement;
  HashSize = SHA256_HASH_SIZE;

  for (Index = 0; Index < BLOCK_NUMBER; Index++) {
    MeasurementBlock[Index].MeasurementBlockCommonHeader.Index = Index + 1;
    MeasurementBlock[Index].MeasurementBlockCommonHeader.MeasurementSpecification = SPDM_MEASUREMENT_BLOCK_HEADER_SPECIFICATION_DMTF;
    MeasurementBlock[Index].MeasurementBlockCommonHeader.MeasurementSize = (UINT16)(sizeof(SPDM_MEASUREMENT_BLOCK_DMTF_HEADER) + HashSize);
    switch (Index) {
    case 0:
      MeasurementBlock[Index].MeasurementBlockDmtfHeader.DMTFSpecMeasurementValueType = SPDM_MEASUREMENT_BLOCK_MEASUREMENT_TYPE_IMMUTABLE_ROM;
      break;
    case 1:
      MeasurementBlock[Index].MeasurementBlockDmtfHeader.DMTFSpecMeasurementValueType = SPDM_MEASUREMENT_BLOCK_MEASUREMENT_TYPE_MUTABLE_FIRMWARE;
      break;
    case 2:
      MeasurementBlock[Index].MeasurementBlockDmtfHeader.DMTFSpecMeasurementValueType = SPDM_MEASUREMENT_BLOCK_MEASUREMENT_TYPE_HARDWARE_CONFIGURATION;
      break;
    case 3:
      MeasurementBlock[Index].MeasurementBlockDmtfHeader.DMTFSpecMeasurementValueType = SPDM_MEASUREMENT_BLOCK_MEASUREMENT_TYPE_FIRMWARE_CONFIGURATION;
      break;
    default:
      ASSERT(FALSE);
      break;
    }
    MeasurementBlock[Index].MeasurementBlockDmtfHeader.DMTFSpecMeasurementValueSize = HashSize;
    SetMem (
      MeasurementBlock[Index].HashValue,
      HashSize,
      (UINT8)(Index + 1)
      );
  }

  return TRUE;
}

EFI_STATUS
EFIAPI
SpdmIoSendMessage (
  IN     SPDM_IO_PPI                            *This,
  IN     UINT32                                 *SessionId,
  IN     UINTN                                  MessageSize,
  IN     VOID                                   *Message,
  IN     UINT64                                 Timeout
  )
{
  SPDM_TEST_DEVICE_CONTEXT  *SpdmTestContext;
  VOID                      *SpdmContext;

  SpdmTestContext = SPDM_TEST_DEVICE_CONTEXT_FROM_SPDM_IO_PROTOCOL(This);
  SpdmContext = SpdmTestContext->SpdmContext;

  if (Message == NULL) {
    return EFI_INVALID_PARAMETER;
  }
  if (MessageSize == 0) {
    return EFI_INVALID_PARAMETER;
  }
  if (mSpdmIoLastSpdmRequest != NULL) {
    FreePool (mSpdmIoLastSpdmRequest);
    mSpdmIoLastSpdmRequest = NULL;
  }

  if (SessionId == NULL) {
    mSpdmIoLastSpdmSessionIdPtr = NULL;
  } else {
    mSpdmIoLastSpdmSessionId = *SessionId;
    mSpdmIoLastSpdmSessionIdPtr = &mSpdmIoLastSpdmSessionId;
  }
  mSpdmIoLastSpdmRequestSize = MessageSize;
  mSpdmIoLastSpdmRequest = AllocateCopyPool (MessageSize, Message);

  return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
SpdmIoReceiveMessage (
  IN     SPDM_IO_PPI                            *This,
     OUT UINT32                                 **SessionId,
  IN OUT UINTN                                  *MessageSize,
  IN OUT VOID                                   *Message,
  IN     UINT64                                 Timeout
  )
{
  SPDM_TEST_DEVICE_CONTEXT  *SpdmTestContext;
  VOID                      *SpdmContext;

  SpdmTestContext = SPDM_TEST_DEVICE_CONTEXT_FROM_SPDM_IO_PROTOCOL(This);
  SpdmContext = SpdmTestContext->SpdmContext;

  if (mSpdmIoLastSpdmSessionIdPtr == NULL) {
    *SessionId = NULL;
    return SpdmReceiveSendData (SpdmContext, mSpdmIoLastSpdmRequest, mSpdmIoLastSpdmRequestSize, Message, MessageSize);
  } else {
    *SessionId = &mSpdmIoLastSpdmSessionId;
    return SpdmReceiveSendSessionData (SpdmContext, mSpdmIoLastSpdmSessionId, mSpdmIoLastSpdmRequest, mSpdmIoLastSpdmRequestSize, Message, MessageSize);
  }
}

SPDM_TEST_DEVICE_CONTEXT  mSpdmTestDeviceContext = {
  SPDM_TEST_DEVICE_CONTEXT_SIGNATURE,
  NULL,
  {
    SpdmIoSendMessage,
    SpdmIoReceiveMessage,
  },
};

EFI_PEI_PPI_DESCRIPTOR  mSpdmIoPpiList = {
  EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST,
  &gSpdmIoPpiGuid,
  &mSpdmTestDeviceContext.SpdmIoPpi
};

EFI_STATUS
EFIAPI
MainEntryPoint (
  IN       EFI_PEI_FILE_HANDLE  FileHandle,
  IN CONST EFI_PEI_SERVICES     **PeiServices
  )
{
  EFI_STATUS                        Status;
  UINT8                             Index;
  VOID                              *CertChain;
  UINTN                             CertChainSize;
  EFI_SIGNATURE_LIST                *SignatureList;
  UINTN                             SignatureListSize;
  VOID                              *SpdmContext;
  BOOLEAN                           Res;
  VOID                              *Data;
  UINTN                             DataSize = 2048;
  SPDM_DATA_PARAMETER               Parameter;
  UINT8                             Data8;
  UINT16                            Data16;
  UINT32                            Data32;
  BOOLEAN                           HasRspPubCert;
  BOOLEAN                           HasRspPrivKey;
  EFI_PEI_READ_ONLY_VARIABLE2_PPI   *VariablePpi;

  SpdmContext = AllocateZeroPool (SpdmGetContextSize());
  ASSERT(SpdmContext != NULL);
  SpdmInitContext (SpdmContext);
  mSpdmTestDeviceContext.SpdmContext = SpdmContext;
  SpdmRegisterDeviceIoFunc (SpdmContext, SpdmDeviceSendMessage, SpdmDeviceReceiveMessage);

  Status = PeiServicesLocatePpi (&gEfiPeiReadOnlyVariable2PpiGuid, 0, NULL, (VOID **) &VariablePpi);
  ASSERT_EFI_ERROR (Status);

  //SignatureListSize = sizeof (EFI_SIGNATURE_LIST);
  SignatureListSize = 1024;
  SignatureList = AllocateZeroPool (SignatureListSize);
  Status = VariablePpi->GetVariable (
                          VariablePpi,
                          EDKII_DEVICE_SECURITY_DATABASE,
                          &gEdkiiDeviceSignatureDatabaseGuid,
                          NULL,
                          &SignatureListSize,
                          SignatureList
                          );
  if (!EFI_ERROR(Status)) {
    HasRspPubCert = TRUE;
    // BUGBUG: Assume only 1 SPDM cert.
    ASSERT (CompareGuid (&SignatureList->SignatureType, &gEdkiiCertSpdmCertChainGuid));
    ASSERT (SignatureList->SignatureListSize == SignatureList->SignatureListSize);
    ASSERT (SignatureList->SignatureHeaderSize == 0);
    ASSERT (SignatureList->SignatureSize == SignatureList->SignatureListSize - (sizeof(EFI_SIGNATURE_LIST) + SignatureList->SignatureHeaderSize));
    CertChain = (VOID *)((UINT8 *)SignatureList +
                         sizeof(EFI_SIGNATURE_LIST) +
                         SignatureList->SignatureHeaderSize +
                         sizeof(EFI_GUID));
    CertChainSize = SignatureList->SignatureSize - sizeof(EFI_GUID);

    ZeroMem (&Parameter, sizeof(Parameter));
    Parameter.Location = SpdmDataLocationLocal;
    Data8 = SLOT_NUMBER;
    SpdmSetData (SpdmContext, SpdmDataSlotCount, &Parameter, &Data8, sizeof(Data8));

    for (Index = 0; Index < SLOT_NUMBER; Index++) {
      Parameter.AdditionalData[0] = Index;
      SpdmSetData (SpdmContext, SpdmDataPublicCertChains, &Parameter, CertChain, CertChainSize);
    }
    // do not free it
  } else {
    HasRspPubCert = FALSE;
  }

  Data = AllocateZeroPool (DataSize);
  Status = VariablePpi->GetVariable (
                          VariablePpi,
                          L"PrivDevKey",
                          &gEdkiiDeviceSignatureDatabaseGuid,
                          NULL,
                          &DataSize,
                          (VOID *)Data
                          );
  if (!EFI_ERROR(Status)) {
    HasRspPrivKey = TRUE;
    mResponderPrivateCertData = Data;
    mResponderPrivateCertDataSize = DataSize;
    SpdmRegisterDataSignFunc (SpdmContext, SpdmDataSignFunc);
  } else{
    HasRspPrivKey = FALSE;
  }

  Data32 = SPDM_GET_CAPABILITIES_RESPONSE_FLAGS_CERT_CAP |
           SPDM_GET_CAPABILITIES_RESPONSE_FLAGS_CHAL_CAP |
//           SPDM_GET_CAPABILITIES_RESPONSE_FLAGS_MEAS_CAP_NO_SIG |
           SPDM_GET_CAPABILITIES_RESPONSE_FLAGS_MEAS_CAP_SIG |
           SPDM_GET_CAPABILITIES_RESPONSE_FLAGS_ENCRYPT_CAP |
           SPDM_GET_CAPABILITIES_RESPONSE_FLAGS_MAC_CAP |
//           SPDM_GET_CAPABILITIES_RESPONSE_FLAGS_MUT_AUTH_CAP |
           SPDM_GET_CAPABILITIES_RESPONSE_FLAGS_KEY_EX_CAP |
//           SPDM_GET_CAPABILITIES_RESPONSE_FLAGS_PSK_CAP_RESPONDER |
           SPDM_GET_CAPABILITIES_RESPONSE_FLAGS_PSK_CAP_RESPONDER_WITH_CONTEXT |
           SPDM_GET_CAPABILITIES_RESPONSE_FLAGS_ENCAP_CAP |
           SPDM_GET_CAPABILITIES_RESPONSE_FLAGS_HBEAT_CAP |
           SPDM_GET_CAPABILITIES_RESPONSE_FLAGS_KEY_UPD_CAP |
           SPDM_GET_CAPABILITIES_RESPONSE_FLAGS_HANDSHAKE_IN_THE_CLEAR_CAP |
           SPDM_GET_CAPABILITIES_RESPONSE_FLAGS_PUB_KEY_ID_CAP;
  if (!HasRspPubCert) {
    Data32 &= ~SPDM_GET_CAPABILITIES_RESPONSE_FLAGS_CERT_CAP;
  } else {
    Data32 |= SPDM_GET_CAPABILITIES_RESPONSE_FLAGS_CERT_CAP;
  }
  if (!HasRspPrivKey) {
    Data32 &= ~SPDM_GET_CAPABILITIES_RESPONSE_FLAGS_CHAL_CAP;
    Data32 &= ~SPDM_GET_CAPABILITIES_RESPONSE_FLAGS_MEAS_CAP_SIG;
    Data32 |= SPDM_GET_CAPABILITIES_RESPONSE_FLAGS_MEAS_CAP_NO_SIG;
  } else {
    Data32 |= SPDM_GET_CAPABILITIES_RESPONSE_FLAGS_CHAL_CAP;
    Data32 |= SPDM_GET_CAPABILITIES_RESPONSE_FLAGS_MEAS_CAP_SIG;
    Data32 &= ~SPDM_GET_CAPABILITIES_RESPONSE_FLAGS_MEAS_CAP_NO_SIG;
  }
  SpdmSetData (SpdmContext, SpdmDataCapabilityFlags, &Parameter, &Data32, sizeof(Data32));

  Data32 = SPDM_ALGORITHMS_MEASUREMENT_HASH_ALGO_TPM_ALG_SHA_256;
  SpdmSetData (SpdmContext, SpdmDataMeasurementHashAlgo, &Parameter, &Data32, sizeof(Data32));
  Data32 = SPDM_ALGORITHMS_BASE_ASYM_ALGO_TPM_ALG_RSASSA_2048;
  SpdmSetData (SpdmContext, SpdmDataBaseAsymAlgo, &Parameter, &Data32, sizeof(Data32));
  Data32 = SPDM_ALGORITHMS_BASE_HASH_ALGO_TPM_ALG_SHA_256;
  SpdmSetData (SpdmContext, SpdmDataBaseHashAlgo, &Parameter, &Data32, sizeof(Data32));
  Data16 = SPDM_ALGORITHMS_DHE_NAMED_GROUP_FFDHE_2048;
  SpdmSetData (SpdmContext, SpdmDataDHENamedGroup, &Parameter, &Data16, sizeof(Data16));
  Data16 = SPDM_ALGORITHMS_KEY_SCHEDULE_HMAC_HASH;
  SpdmSetData (SpdmContext, SpdmDataAEADCipherSuite, &Parameter, &Data16, sizeof(Data16));
  Data16 = SPDM_ALGORITHMS_KEY_SCHEDULE_HMAC_HASH;
  SpdmSetData (SpdmContext, SpdmDataKeySchedule, &Parameter, &Data16, sizeof(Data16));

  Res = RegisterMeasurement (&Data, &DataSize, &Data8);
  if (Res) {
    ZeroMem (&Parameter, sizeof(Parameter));
    Parameter.Location = SpdmDataLocationLocal;
    Parameter.AdditionalData[0] = Data8;
    SpdmSetData (SpdmContext, SpdmDataMeasurementRecord, &Parameter, Data, DataSize);
    // do not free it
  }

  Status = PeiServicesInstallPpi (&mSpdmIoPpiList);
  ASSERT_EFI_ERROR (Status);

  InitializeSpdmTest (&mSpdmTestDeviceContext);

  return EFI_SUCCESS;
}
