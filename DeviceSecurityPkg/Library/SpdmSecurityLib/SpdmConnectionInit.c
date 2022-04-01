/** @file
  EDKII Device Security library for SPDM device.
  It follows the SPDM Specification.

Copyright (c) 2020, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include "SpdmSecurityLibInterna.h"

SPDM_DEVICE_CONTEXT *
CreateSpdmDeviceContext (
  IN EDKII_SPDM_DEVICE_INFO         *SpdmDeviceInfo
  )
{
  SPDM_DEVICE_CONTEXT               *SpdmDeviceContext;
  VOID                              *SpdmContext;
  UINTN                             SpdmContextSize;
  VOID                              *ScratchBuffer;
  UINTN                             ScratchBufferSize;
  EFI_STATUS                        Status;
  EFI_SIGNATURE_LIST                *SignatureList;
  UINTN                             SignatureListSize;
  VOID                              *Data;
  UINTN                             DataSize;
  SPDM_DATA_PARAMETER               Parameter;
  UINT8                             Data8;
  UINT16                            Data16;
  UINT32                            Data32;

  SpdmDeviceContext = AllocateZeroPool (sizeof(*SpdmDeviceContext));
  ASSERT(SpdmDeviceContext != NULL);
  SpdmDeviceContext->Signature = SPDM_DEVICE_CONTEXT_SIGNATURE;
  CopyMem (&SpdmDeviceContext->DeviceId, SpdmDeviceInfo->DeviceId, sizeof(EDKII_DEVICE_IDENTIFIER));
  SpdmDeviceContext->IsEmbeddedDevice = SpdmDeviceInfo->IsEmbeddedDevice;

  SpdmContextSize = SpdmGetContextSize();
  SpdmContext = AllocateZeroPool (SpdmContextSize);
  ASSERT(SpdmContext != NULL);

  ScratchBufferSize = SpdmGetSizeofRequiredScratchBuffer(SpdmContext);
  ScratchBuffer = AllocateZeroPool(ScratchBufferSize);
  ASSERT(ScratchBuffer != NULL);

  SpdmInitContext (SpdmContext);
  SpdmRegisterDeviceIoFunc (SpdmContext,
    SpdmDeviceInfo->SendMessage,
    SpdmDeviceInfo->ReceiveMessage);
  SpdmRegisterTransportLayerFunc (SpdmContext,
    SpdmDeviceInfo->TransportEncodeMessage,
    SpdmDeviceInfo->TransportDecodeMessage,
    SpdmDeviceInfo->TransportGetHeaderSize);
  SpdmRegisterDeviceBufferFunc (SpdmContext,
    SpdmDeviceInfo->AcquireSenderBuffer,
    SpdmDeviceInfo->ReleaseSenderBuffer,
    SpdmDeviceInfo->AcquireReceiverBuffer,
    SpdmDeviceInfo->ReleaseReceiverBuffer);
  SpdmSetScratchBuffer (SpdmContext, ScratchBuffer, ScratchBufferSize);

  SpdmDeviceContext->SpdmContextSize = SpdmContextSize;
  SpdmDeviceContext->SpdmContext = SpdmContext;
  SpdmDeviceContext->ScratchBufferSize = ScratchBufferSize;
  SpdmDeviceContext->ScratchBuffer = ScratchBuffer;

  Status = gBS->HandleProtocol (
                  SpdmDeviceContext->DeviceId.DeviceHandle,
                  &gEfiDevicePathProtocolGuid,
                  (VOID **)&SpdmDeviceContext->DevicePath
                  );
  if (EFI_ERROR(Status)) {
    DEBUG ((DEBUG_ERROR, "Locate - DevicePath - %r\n", Status));
    goto Error;
  }

  Status = gBS->HandleProtocol (
                  SpdmDeviceContext->DeviceId.DeviceHandle,
                  &SpdmDeviceContext->DeviceId.DeviceType,
                  (VOID **)&SpdmDeviceContext->DeviceIo
                  );
  if (EFI_ERROR(Status)) {
    DEBUG ((DEBUG_ERROR, "Locate - DeviceIo - %r\n", Status));
    // This is optional, only check known device type later.
  }

  if (CompareGuid (&SpdmDeviceContext->DeviceId.DeviceType, &gEdkiiDeviceIdentifierTypePciGuid)) {
    if (SpdmDeviceContext->DeviceIo == NULL) {
      DEBUG ((DEBUG_ERROR, "Locate - PciIo - %r\n", Status));
      goto Error;
    }
  }

#define SPDM_UID  1 // TBD - hardcoded
  SpdmDeviceContext->DeviceUID = SPDM_UID;

  Status = GetVariable2 (
             EDKII_DEVICE_SECURITY_DATABASE,
             &gEdkiiDeviceSignatureDatabaseGuid,
             &SignatureList,
             &SignatureListSize
             );
  if (!EFI_ERROR(Status)) {
    // BUGBUG: Assume only 1 SPDM cert.
    ASSERT (CompareGuid (&SignatureList->SignatureType, &gEdkiiCertSpdmCertChainGuid));
    ASSERT (SignatureList->SignatureListSize == SignatureList->SignatureListSize);
    ASSERT (SignatureList->SignatureHeaderSize == 0);
    ASSERT (SignatureList->SignatureSize == SignatureList->SignatureListSize - (sizeof(EFI_SIGNATURE_LIST) + SignatureList->SignatureHeaderSize));

    Data = (VOID *)((UINT8 *)SignatureList +
                             sizeof(EFI_SIGNATURE_LIST) +
                             SignatureList->SignatureHeaderSize +
                             sizeof(EFI_GUID));
    DataSize = SignatureList->SignatureSize - sizeof(EFI_GUID);

    ZeroMem (&Parameter, sizeof(Parameter));
    Parameter.location = SpdmDataLocationLocal;
    SpdmSetData (SpdmContext, SpdmDataPeerPublicCertChains, &Parameter, Data, DataSize);
    // Do not free it.
  }

  Data8 = 0;
  ZeroMem (&Parameter, sizeof(Parameter));
  Parameter.location = SpdmDataLocationLocal;
  SpdmSetData (SpdmContext, SpdmDataCapabilityCTExponent, &Parameter, &Data8, sizeof(Data8));

  Data32 = 0;
  SpdmSetData (SpdmContext, SpdmDataCapabilityFlags, &Parameter, &Data32, sizeof(Data32));

  Data8 = SPDM_MEASUREMENT_BLOCK_HEADER_SPECIFICATION_DMTF;
  SpdmSetData (SpdmContext, SpdmDataMeasurementSpec, &Parameter, &Data8, sizeof(Data8));
  if (SpdmDeviceInfo->BaseAsymAlgo != 0) {
    Data32 = SpdmDeviceInfo->BaseAsymAlgo;
  } else {
    Data32 = SPDM_ALGORITHMS_BASE_ASYM_ALGO_TPM_ALG_RSASSA_2048;
  }
  SpdmSetData (SpdmContext, SpdmDataBaseAsymAlgo, &Parameter, &Data32, sizeof(Data32));
  if (SpdmDeviceInfo->BaseHashAlgo != 0) {
    Data32 = SpdmDeviceInfo->BaseHashAlgo;
  } else {
    Data32 = SPDM_ALGORITHMS_BASE_HASH_ALGO_TPM_ALG_SHA_256;
  }
  SpdmSetData (SpdmContext, SpdmDataBaseHashAlgo, &Parameter, &Data32, sizeof(Data32));

  Status = SpdmInitConnection (SpdmContext, FALSE);
  if (EFI_ERROR(Status)) {
    DEBUG ((DEBUG_ERROR, "SpdmInitConnection - %r\n", Status));
    goto Error;
  }

  ZeroMem (&Parameter, sizeof(Parameter));
  Parameter.location = SpdmDataLocationConnection;
  DataSize = sizeof(Data16);
  SpdmGetData (SpdmContext, SpdmDataSpdmVersion, &Parameter, &Data16, &DataSize);
  SpdmDeviceContext->SpdmVersion = (Data16 >> 8);

  return SpdmDeviceContext;
Error:
  FreePool (SpdmDeviceContext);
  return NULL;
}

VOID
DestroySpdmDeviceContext (
  IN SPDM_DEVICE_CONTEXT         *SpdmDeviceContext
  )
{
  // need zero memory in case of secret in memory.
  ZeroMem (SpdmDeviceContext->SpdmContext, SpdmDeviceContext->SpdmContextSize);
  FreePool (SpdmDeviceContext->SpdmContext);

  ZeroMem (SpdmDeviceContext->ScratchBuffer, SpdmDeviceContext->ScratchBufferSize);
  FreePool (SpdmDeviceContext->ScratchBuffer);

  FreePool (SpdmDeviceContext);
}