/** @file
  EDKII Device Security library for SPDM device.
  It follows the SPDM Specification.

Copyright (c) 2020, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include "SpdmDeviceSecurityDxe.h"

LIST_ENTRY mSpdmDeviceList = INITIALIZE_LIST_HEAD_VARIABLE(mSpdmDeviceList);

EDKII_DEVICE_SECURITY_POLICY_PROTOCOL  *mDeviceSecurityPolicy;

/**
  Compare two device paths to check if they are exactly same.

  @param DevicePath1    A pointer to the first device path data structure.
  @param DevicePath2    A pointer to the second device path data structure.

  @retval TRUE    They are same.
  @retval FALSE   They are not same.

**/
BOOLEAN
CompareDevicePath (
  IN EFI_DEVICE_PATH_PROTOCOL *DevicePath1,
  IN EFI_DEVICE_PATH_PROTOCOL *DevicePath2
  )
{
  UINTN Size1;
  UINTN Size2;

  Size1 = GetDevicePathSize (DevicePath1);
  Size2 = GetDevicePathSize (DevicePath2);

  if (Size1 != Size2) {
    return FALSE;
  }

  if (CompareMem (DevicePath1, DevicePath2, Size1) != 0) {
    return FALSE;
  }

  return TRUE;
}

/**
  Record an SPDM device into device list.
  
  @param[in]  SpdmContext       The SPDM context for the device.
**/
VOID
RecordSpdmDeviceInList (
  IN SPDM_DRIVER_DEVICE_CONTEXT          *SpdmDriverContext
  )
{
  SPDM_DEVICE_INSTANCE  *NewSpdmDevice;
  LIST_ENTRY            *SpdmDeviceList;

  SpdmDeviceList = &mSpdmDeviceList;

  NewSpdmDevice = AllocateZeroPool(sizeof(*NewSpdmDevice));
  ASSERT(NewSpdmDevice != NULL);

  NewSpdmDevice->Signature = SPDM_DEVICE_INSTANCE_SIGNATURE;
  NewSpdmDevice->SpdmDriverContext = SpdmDriverContext;

  InsertTailList(SpdmDeviceList, &NewSpdmDevice->Link);
}

/**
  Return the SPDM device via DeviceId.
  
  @param[in]  DeviceId          The Identifier for the device.

  @return The SPDM device context.
**/
SPDM_DRIVER_DEVICE_CONTEXT *
GetSpdmDriverContextViaDeviceId (
  IN EDKII_DEVICE_IDENTIFIER         *DeviceId
  )
{
  LIST_ENTRY            *Link;
  SPDM_DEVICE_INSTANCE  *CurrentSpdmDevice;
  LIST_ENTRY            *SpdmDeviceList;

  SpdmDeviceList = &mSpdmDeviceList;
  
  Link = GetFirstNode(SpdmDeviceList);
  while (!IsNull(SpdmDeviceList, Link)) {
    CurrentSpdmDevice = SPDM_DEVICE_INSTANCE_FROM_LINK(Link);

    if (CurrentSpdmDevice->SpdmDriverContext->DeviceId.DeviceHandle == DeviceId->DeviceHandle) {
      return CurrentSpdmDevice->SpdmDriverContext;
    }

    Link = GetNextNode(SpdmDeviceList, Link);
  }

  return NULL;
}

/**
  Return the SPDM device via Spdm protocol.
  
  @param[in]  Spdm          The SPDM protocol instance.

  @return The SPDM device context.
**/
SPDM_DRIVER_DEVICE_CONTEXT *
GetSpdmDriverContextViaSpdmProtocol (
  IN SPDM_PROTOCOL *SpdmProtocol
  )
{
  LIST_ENTRY            *Link;
  SPDM_DEVICE_INSTANCE  *CurrentSpdmDevice;
  LIST_ENTRY            *SpdmDeviceList;

  SpdmDeviceList = &mSpdmDeviceList;
  
  Link = GetFirstNode(SpdmDeviceList);
  while (!IsNull(SpdmDeviceList, Link)) {
    CurrentSpdmDevice = SPDM_DEVICE_INSTANCE_FROM_LINK(Link);

    if (CurrentSpdmDevice->SpdmDriverContext->SpdmProtocol == SpdmProtocol) {
      return CurrentSpdmDevice->SpdmDriverContext;
    }

    Link = GetNextNode(SpdmDeviceList, Link);
  }

  return NULL;
}

/**
  Return the SPDM device via Spdm Context.
  
  @param[in]  Spdm          The SPDM context instance.

  @return The SPDM device context.
**/
SPDM_DRIVER_DEVICE_CONTEXT *
GetSpdmDriverContextViaSpdmContext (
  IN VOID *SpdmContext
  )
{
  LIST_ENTRY            *Link;
  SPDM_DEVICE_INSTANCE  *CurrentSpdmDevice;
  LIST_ENTRY            *SpdmDeviceList;

  SpdmDeviceList = &mSpdmDeviceList;
  
  Link = GetFirstNode(SpdmDeviceList);
  while (!IsNull(SpdmDeviceList, Link)) {
    CurrentSpdmDevice = SPDM_DEVICE_INSTANCE_FROM_LINK(Link);

    if (CurrentSpdmDevice->SpdmDriverContext->SpdmContext == SpdmContext) {
      return CurrentSpdmDevice->SpdmDriverContext;
    }

    Link = GetNextNode(SpdmDeviceList, Link);
  }

  return NULL;
}

/**
  Record an SPDM device into device list.
  
  @param[in]  SpdmContext       The SPDM context for the device.
**/
VOID
RecordSpdmDeviceInMeasurementList (
  IN SPDM_DRIVER_DEVICE_CONTEXT          *SpdmDriverContext
  )
{
  SpdmDriverContext->IsDeviceMeasured = TRUE;
}

/**
  Check if an SPDM device is recorded in device list.
  
  @param[in]  SpdmContext       The SPDM context for the device.

  @retval TRUE  The SPDM device is in the list.
  @retval FALSE The SPDM device is NOT in the list.
**/
BOOLEAN
IsSpdmDeviceInMeasurementList (
  IN SPDM_DRIVER_DEVICE_CONTEXT          *SpdmDriverContext
  )
{
  return SpdmDriverContext->IsDeviceMeasured;
}

/**
  Record an SPDM device into device list.
  
  @param[in]  SpdmContext       The SPDM context for the device.
**/
VOID
RecordSpdmDeviceInAuthenticationList (
  IN SPDM_DRIVER_DEVICE_CONTEXT          *SpdmDriverContext
  )
{
  SpdmDriverContext->IsDeviceAuthenticated = TRUE;
}

/**
  Check if an SPDM device is recorded in device list.
  
  @param[in]  SpdmContext       The SPDM context for the device.

  @retval TRUE  The SPDM device is in the list.
  @retval FALSE The SPDM device is NOT in the list.
**/
BOOLEAN
IsSpdmDeviceInAuthenticationList (
  IN SPDM_DRIVER_DEVICE_CONTEXT          *SpdmDriverContext
  )
{
  return SpdmDriverContext->IsDeviceAuthenticated;
}

/**
  This function creates the SPDM device contenxt.
  
  @param[in]  DeviceId               The Identifier for the device.

  @return SPDM device context
**/
SPDM_DRIVER_DEVICE_CONTEXT *
CreateSpdmDriverContext (
  IN EDKII_DEVICE_IDENTIFIER      *DeviceId
  )
{
  SPDM_DRIVER_DEVICE_CONTEXT        *SpdmDriverContext;
  VOID                              *SpdmContext;
  EFI_STATUS                        Status;
  EFI_SIGNATURE_LIST                *SignatureList;
  UINTN                             SignatureListSize;
  VOID                              *Data;
  UINTN                             DataSize;
  SPDM_DATA_PARAMETER               Parameter;
  UINT8                             Data8;
  UINT16                            Data16;
  UINT32                            Data32;
  BOOLEAN                           HasRspPubCert;

  SpdmDriverContext = AllocateZeroPool (sizeof(*SpdmDriverContext));
  ASSERT(SpdmDriverContext != NULL);
  SpdmContext = AllocateZeroPool (SpdmGetContextSize());
  ASSERT(SpdmContext != NULL);
  SpdmInitContext (SpdmContext);
  SpdmRegisterDeviceIoFunc (SpdmContext, SpdmDeviceSendMessage, SpdmDeviceReceiveMessage);
//  SpdmRegisterTransportLayerFunc (SpdmContext, SpdmTransportMctpEncodeMessage, SpdmTransportMctpDecodeMessage);
  SpdmRegisterTransportLayerFunc (SpdmContext, SpdmTransportPciDoeEncodeMessage, SpdmTransportPciDoeDecodeMessage);

  SpdmDriverContext->SpdmContext = SpdmContext;

  SpdmDriverContext->Signature = SPDM_DRIVER_DEVICE_CONTEXT_SIGNATURE;
  CopyMem (&SpdmDriverContext->DeviceId, DeviceId, sizeof(*DeviceId));

  Status = gBS->HandleProtocol (
                  DeviceId->DeviceHandle,
                  &gSpdmIoProtocolGuid,
                  (VOID **)&SpdmDriverContext->SpdmIoProtocol
                  );
  if (EFI_ERROR(Status)) {
    DEBUG ((DEBUG_ERROR, "Locate - SpdmIoProtocol - %r\n", Status));
    goto Error;
  }

  Status = gBS->HandleProtocol (
                  DeviceId->DeviceHandle,
                  &gSpdmProtocolGuid,
                  (VOID **)&SpdmDriverContext->SpdmProtocol
                  );
  if (EFI_ERROR(Status)) {
    DEBUG ((DEBUG_ERROR, "Locate - SpdmProtocol - %r\n", Status));
    goto Error;
  }

  Status = gBS->HandleProtocol (
                  DeviceId->DeviceHandle,
                  &gEfiDevicePathProtocolGuid,
                  (VOID **)&SpdmDriverContext->DevicePath
                  );
  if (EFI_ERROR(Status)) {
    DEBUG ((DEBUG_ERROR, "Locate - DevicePath - %r\n", Status));
    goto Error;
  }

  Status = gBS->HandleProtocol (
                  DeviceId->DeviceHandle,
                  &DeviceId->DeviceType,
                  (VOID **)&SpdmDriverContext->DeviceIo
                  );
  if (EFI_ERROR(Status)) {
    DEBUG ((DEBUG_ERROR, "Locate - DeviceIo - %r\n", Status));
    // This is optional, only check known device type later.
  }

  if (CompareGuid (&DeviceId->DeviceType, &gEdkiiDeviceIdentifierTypePciGuid) ||
      CompareGuid (&DeviceId->DeviceType, &gEdkiiDeviceIdentifierTypeUsbGuid)) {
    if (SpdmDriverContext->DeviceIo == NULL) {
      DEBUG ((DEBUG_ERROR, "Locate - PciIo - %r\n", Status));
      goto Error;
    }
  }
  
  //
  // Record list before any transaction
  //
  RecordSpdmDeviceInList (SpdmDriverContext);

  Status = GetVariable2 (
             EDKII_DEVICE_SECURITY_DATABASE,
             &gEdkiiDeviceSignatureDatabaseGuid,
             &SignatureList,
             &SignatureListSize
             );
  if (!EFI_ERROR(Status)) {
    HasRspPubCert = TRUE;
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
    Parameter.Location = SpdmDataLocationLocal;
    SpdmSetData (SpdmContext, SpdmDataPeerPublicCertChains, &Parameter, Data, DataSize);
    // Do not free it.
  } else {
    HasRspPubCert = FALSE;
  }

  Data8 = 0;
  ZeroMem (&Parameter, sizeof(Parameter));
  Parameter.Location = SpdmDataLocationLocal;
  SpdmSetData (SpdmContext, SpdmDataCapabilityCTExponent, &Parameter, &Data8, sizeof(Data8));

  Data32 = 0 |
//           SPDM_GET_CAPABILITIES_REQUEST_FLAGS_CERT_CAP |
//           SPDM_GET_CAPABILITIES_REQUEST_FLAGS_CHAL_CAP |
           SPDM_GET_CAPABILITIES_REQUEST_FLAGS_ENCRYPT_CAP |
           SPDM_GET_CAPABILITIES_REQUEST_FLAGS_MAC_CAP |
//           SPDM_GET_CAPABILITIES_REQUEST_FLAGS_MUT_AUTH_CAP |
           SPDM_GET_CAPABILITIES_REQUEST_FLAGS_KEY_EX_CAP |
           SPDM_GET_CAPABILITIES_REQUEST_FLAGS_PSK_CAP_REQUESTER |
//           SPDM_GET_CAPABILITIES_REQUEST_FLAGS_ENCAP_CAP |
           SPDM_GET_CAPABILITIES_REQUEST_FLAGS_HBEAT_CAP |
           SPDM_GET_CAPABILITIES_REQUEST_FLAGS_KEY_UPD_CAP |
           SPDM_GET_CAPABILITIES_REQUEST_FLAGS_HANDSHAKE_IN_THE_CLEAR_CAP |
//           SPDM_GET_CAPABILITIES_REQUEST_FLAGS_PUB_KEY_ID_CAP |
           0;
  if (!HasRspPubCert) {
    Data32 &= ~SPDM_GET_CAPABILITIES_REQUEST_FLAGS_CHAL_CAP;
  } else {
    Data32 |= SPDM_GET_CAPABILITIES_REQUEST_FLAGS_CHAL_CAP;
  }
  SpdmSetData (SpdmContext, SpdmDataCapabilityFlags, &Parameter, &Data32, sizeof(Data32));

  Data8 = SPDM_MEASUREMENT_BLOCK_HEADER_SPECIFICATION_DMTF;
  SpdmSetData (SpdmContext, SpdmDataMeasurementSpec, &Parameter, &Data8, sizeof(Data8));
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

  Status = SpdmInitConnection (SpdmContext, FALSE);
  if (EFI_ERROR(Status)) {
    DEBUG ((DEBUG_ERROR, "SpdmInitConnection - %r\n", Status));
    goto Error;
  }

  return SpdmDriverContext;
Error:
  FreePool (SpdmDriverContext);
  return NULL;
}

/**
  The device driver uses this service to measure and/or verify a device.

  The flow in device driver is:
  1) Device driver discovers a new device.
  2) Device driver creates an EFI_DEVICE_PATH_PROTOCOL.
  3) Device driver creates a device access protocol. e.g.
     EFI_PCI_IO_PROTOCOL for PCI device.
     EFI_USB_IO_PROTOCOL for USB device.
     EFI_EXT_SCSI_PASS_THRU_PROTOCOL for SCSI device.
     EFI_ATA_PASS_THRU_PROTOCOL for ATA device.
     EFI_NVM_EXPRESS_PASS_THRU_PROTOCOL for NVMe device.
     EFI_SD_MMC_PASS_THRU_PROTOCOL for SD/MMC device.
     The device driver also creates the SPDM_IO_PROTOCOL.
  4) Device driver installs the EFI_DEVICE_PATH_PROTOCOL with EFI_DEVICE_PATH_PROTOCOL_GUID,
     the SPDM_IO_PROTOCOL with SPDM_PROTOCOL_IO_GUID,
     and the device access protocol with EDKII_DEVICE_IDENTIFIER_TYPE_xxx_GUID.
     Once it is done, a DeviceHandle is returned.
  5) Device driver creates EDKII_DEVICE_IDENTIFIER with EDKII_DEVICE_IDENTIFIER_TYPE_xxx_GUID
     and the DeviceHandle.
  6) Device driver calls DeviceAuthenticate().
  7) If DeviceAuthenticate() returns EFI_SECURITY_VIOLATION, the device driver uninstalls
     all protocols on this handle.
  8) If DeviceAuthenticate() returns EFI_SUCCESS, the device driver installs the device access
     protocol with a real protocol GUID. e.g.
     EFI_PCI_IO_PROTOCOL with EFI_PCI_IO_PROTOCOL_GUID.
     EFI_USB_IO_PROTOCOL with EFI_USB_IO_PROTOCOL_GUID.

  @param[in]  This              The protocol instance pointer.
  @param[in]  DeviceId          The Identifier for the device.

  @retval EFI_SUCCESS              The device specified by the DeviceId passed the measurement
                                   and/or authentication based upon the platform policy.
                                   If TCG measurement is required, the measurement is extended to TPM PCR.
  @retval EFI_SECURITY_VIOLATION   The device fails to return the measurement data.
  @retval EFI_SECURITY_VIOLATION   The device fails to response the authentication request.
  @retval EFI_SECURITY_VIOLATION   The system fails to verify the device based upon the authentication response.
  @retval EFI_SECURITY_VIOLATION   The system fails to extend the measurement to TPM PCR.
**/
EFI_STATUS
EFIAPI
DeviceAuthentication (
  IN EDKII_DEVICE_SECURITY_PROTOCOL  *This,
  IN EDKII_DEVICE_IDENTIFIER         *DeviceId
  )
{
  EDKII_DEVICE_SECURITY_POLICY           DeviceSecurityPolicy;
  EDKII_DEVICE_SECURITY_STATE            DeviceSecurityState;
  SPDM_DRIVER_DEVICE_CONTEXT             *SpdmDriverContext;
  EFI_STATUS                             Status;

  if (mDeviceSecurityPolicy == NULL) {
    return EFI_SUCCESS;
  }

  SpdmDriverContext = GetSpdmDriverContextViaDeviceId (DeviceId);
  if (SpdmDriverContext == NULL) {
    SpdmDriverContext = CreateSpdmDriverContext (DeviceId);
  }
  if (SpdmDriverContext == NULL) {
    return EFI_UNSUPPORTED;
  }

  DeviceSecurityState.Revision = EDKII_DEVICE_SECURITY_STATE_REVISION;
  DeviceSecurityState.MeasurementState = 0x0;
  DeviceSecurityState.AuthenticationState = 0x0;

  Status = mDeviceSecurityPolicy->GetDevicePolicy (mDeviceSecurityPolicy, DeviceId, &DeviceSecurityPolicy);
  if (EFI_ERROR(Status)) {
    DEBUG((DEBUG_ERROR, "mDeviceSecurityPolicy->GetDevicePolicy - %r\n", Status));
    DeviceSecurityState.MeasurementState = EDKII_DEVICE_SECURITY_STATE_ERROR_UEFI_GET_POLICY_PROTOCOL;
    DeviceSecurityState.AuthenticationState = EDKII_DEVICE_SECURITY_STATE_ERROR_UEFI_GET_POLICY_PROTOCOL;
  } else {
    if ((DeviceSecurityPolicy.AuthenticationPolicy & EDKII_DEVICE_AUTHENTICATION_REQUIRED) != 0) {
      DoDeviceAuthentication (SpdmDriverContext, &DeviceSecurityState);
      DEBUG((DEBUG_ERROR, "AuthenticationState - 0x%08x\n", DeviceSecurityState.AuthenticationState));
    }
    if ((DeviceSecurityPolicy.MeasurementPolicy & EDKII_DEVICE_MEASUREMENT_REQUIRED) != 0) {
      DoDeviceMeasurement (SpdmDriverContext, &DeviceSecurityState);
      DEBUG((DEBUG_ERROR, "MeasurementState - 0x%08x\n", DeviceSecurityState.MeasurementState));
    }
  }

  Status = mDeviceSecurityPolicy->NotifyDeviceState (mDeviceSecurityPolicy, DeviceId, &DeviceSecurityState);
  if (EFI_ERROR(Status)) {
    DEBUG((DEBUG_ERROR, "mDeviceSecurityPolicy->NotifyDeviceState - %r\n", Status));
  }

  if ((DeviceSecurityState.MeasurementState == 0) &&
      (DeviceSecurityState.AuthenticationState == 0)) {
    return EFI_SUCCESS;
  } else {
    return EFI_SECURITY_VIOLATION;
  }
}

EDKII_DEVICE_SECURITY_PROTOCOL mDeviceSecurity = {
  EDKII_DEVICE_SECURITY_PROTOCOL_REVISION,
  DeviceAuthentication
};

/**
  Entrypoint of the device security driver.

  @param[in]  ImageHandle  ImageHandle of the loaded driver
  @param[in]  SystemTable  Pointer to the System Table

  @retval  EFI_SUCCESS           The Protocol is installed.
  @retval  EFI_OUT_OF_RESOURCES  Not enough resources available to initialize driver.
  @retval  EFI_DEVICE_ERROR      A device error occurred attempting to initialize the driver.

**/
EFI_STATUS
EFIAPI
MainEntryPoint (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
  EFI_HANDLE  Handle;
  EFI_STATUS  Status;

  Status = gBS->LocateProtocol (&gEdkiiDeviceSecurityPolicyProtocolGuid, NULL, (VOID **)&mDeviceSecurityPolicy);
  ASSERT_EFI_ERROR(Status);

  Handle = NULL;
  Status = gBS->InstallProtocolInterface (
                  &Handle,
                  &gEdkiiDeviceSecurityProtocolGuid,
                  EFI_NATIVE_INTERFACE,
                  (VOID **)&mDeviceSecurity
                  );
  ASSERT_EFI_ERROR(Status);

  InitializeSpdmCommunication ();

  return Status;
}
