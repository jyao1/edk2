/** @file
  EDKII DeployCert

  Copyright (c) 2020, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <Uefi.h>
#include <IndustryStandard/UefiTcgPlatform.h>
#include <IndustryStandard/TcgSpdm.h>
#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/BaseCryptLib.h>
#include <Library/TpmMeasurementLib.h>
#include <Guid/DeviceAuthentication.h>
#include <Guid/ImageAuthentication.h>
#include <hal/library/LibspdmStub.h>
#include <industry_standard/spdm.h>
#define SHA256_HASH_SIZE  32

extern UINT8 TestRootCer[];
extern UINTN TestRootCerSize;

extern UINT8 TestRootKey[];
extern UINTN TestRootKeySize;

extern UINT8 EccTestRootCer[];
extern UINTN EccTestRootCerSize;

extern UINT8 EccTestRootKey[];
extern UINTN EccTestRootKeySize;


EFI_STATUS
EFIAPI
MeasureVariable (
  IN      UINT32                    PcrIndex,
  IN      UINT32                    EventType,
  IN      CHAR16                    *VarName,
  IN      EFI_GUID                  *VendorGuid,
  IN      VOID                      *VarData,
  IN      UINTN                     VarSize
  )
{
  EFI_STATUS                        Status;
  UINTN                             VarNameLength;
  UEFI_VARIABLE_DATA                *VarLog;
  UINT32                            VarLogSize;

  ASSERT ((VarSize == 0 && VarData == NULL) || (VarSize != 0 && VarData != NULL));

  VarNameLength      = StrLen (VarName);
  VarLogSize = (UINT32)(sizeof (*VarLog) + VarNameLength * sizeof (*VarName) + VarSize
                        - sizeof (VarLog->UnicodeName) - sizeof (VarLog->VariableData));

  VarLog = (UEFI_VARIABLE_DATA *) AllocateZeroPool (VarLogSize);
  if (VarLog == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  CopyMem (&VarLog->VariableName, VendorGuid, sizeof(VarLog->VariableName));
  VarLog->UnicodeNameLength  = VarNameLength;
  VarLog->VariableDataLength = VarSize;
  CopyMem (
     VarLog->UnicodeName,
     VarName,
     VarNameLength * sizeof (*VarName)
     );
  if (VarSize != 0) {
    CopyMem (
       (CHAR16 *)VarLog->UnicodeName + VarNameLength,
       VarData,
       VarSize
       );
  }

  DEBUG ((EFI_D_INFO, "VariableDxe: MeasureVariable (Pcr - %x, EventType - %x, ", (UINTN)7, (UINTN)EV_EFI_SPDM_DEVICE_POLICY));
  DEBUG ((EFI_D_INFO, "VariableName - %s, VendorGuid - %g)\n", VarName, VendorGuid));

  Status = TpmMeasureAndLogData (
             PcrIndex,
             EventType,
             VarLog,
             VarLogSize,
             VarLog,
             VarLogSize
             );
  FreePool (VarLog);
  return Status;
}

EFI_STATUS
EFIAPI
MainEntryPoint (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
  EFI_STATUS          Status;
  SPDM_CERT_CHAIN     *CertChain;
  UINTN               CertChainSize;
  EFI_SIGNATURE_LIST  *SignatureList;
  EFI_SIGNATURE_DATA  *SignatureData;
  UINTN               SignatureListSize;
  UINTN               SignatureHeaderSize;
  BOOLEAN             Res;
  UINT8               *RootCert;
  UINTN               RootCertLen;
  CertChainSize = sizeof(SPDM_CERT_CHAIN) + SHA256_HASH_SIZE + TestRootCerSize;
  CertChain = AllocateZeroPool (CertChainSize);
  ASSERT (CertChain != NULL);
  CertChain->length = (UINT16)CertChainSize;
  CertChain->reserved = 0;
  Res = X509GetCertFromCertChain(TestRootCer, TestRootCerSize, 0, &RootCert,
                                 &RootCertLen);
  Sha256HashAll (RootCert, RootCertLen, (VOID *)(CertChain + 1));
  CopyMem (
    (UINT8 *)CertChain + sizeof(SPDM_CERT_CHAIN) + SHA256_HASH_SIZE,
    TestRootCer,
    TestRootCerSize
    );

  SignatureHeaderSize = 0;
  SignatureListSize = sizeof(EFI_SIGNATURE_LIST) + SignatureHeaderSize + sizeof(EFI_GUID) + CertChainSize;
  SignatureList = AllocateZeroPool (SignatureListSize);
  ASSERT (SignatureList != NULL);
  CopyGuid (&SignatureList->SignatureType, &gEdkiiCertSpdmCertChainGuid);
  SignatureList->SignatureListSize = (UINT32)SignatureListSize;
  SignatureList->SignatureHeaderSize = (UINT32)SignatureHeaderSize;
  SignatureList->SignatureSize = (UINT32)(sizeof(EFI_GUID) + CertChainSize);
  SignatureData = (VOID *)((UINT8 *)SignatureList + sizeof(EFI_SIGNATURE_LIST));
  CopyGuid (&SignatureData->SignatureOwner, &gEfiCallerIdGuid);
  CopyMem (
    (UINT8 *)SignatureList + sizeof(EFI_SIGNATURE_LIST) + SignatureHeaderSize + sizeof(EFI_GUID),
    CertChain,
    CertChainSize
    );

  Status = gRT->SetVariable (
                  EDKII_DEVICE_SECURITY_DATABASE,
                  &gEdkiiDeviceSignatureDatabaseGuid,
                  EFI_VARIABLE_NON_VOLATILE |
                    EFI_VARIABLE_BOOTSERVICE_ACCESS |
                    EFI_VARIABLE_RUNTIME_ACCESS,
                  SignatureListSize,
                  SignatureList
                  );
  ASSERT_EFI_ERROR(Status);
  FreePool (SignatureList);

  {
    //
    // TBD - we need only include the root-cert, instead of the CertChain
    // BUGBUG: Hardcode here to pass measurement at first
    //
    SignatureHeaderSize = 0;
    SignatureListSize = sizeof(EFI_SIGNATURE_LIST) + SignatureHeaderSize + sizeof(EFI_GUID) + TestRootCerSize;
    SignatureList = AllocateZeroPool (SignatureListSize);
    ASSERT (SignatureList != NULL);
    CopyGuid (&SignatureList->SignatureType, &gEfiCertX509Guid);
    SignatureList->SignatureListSize = (UINT32)SignatureListSize;
    SignatureList->SignatureHeaderSize = (UINT32)SignatureHeaderSize;
    SignatureList->SignatureSize = (UINT32)(sizeof(EFI_GUID) + TestRootCerSize);
    SignatureData = (VOID *)((UINT8 *)SignatureList + sizeof(EFI_SIGNATURE_LIST));
    CopyGuid (&SignatureData->SignatureOwner, &gEfiCallerIdGuid);
    CopyMem (
      (UINT8 *)SignatureList + sizeof(EFI_SIGNATURE_LIST) + SignatureHeaderSize + sizeof(EFI_GUID),
      TestRootCer,
      TestRootCerSize
      );

    MeasureVariable (
      7,
      EV_EFI_SPDM_DEVICE_POLICY,
      EDKII_DEVICE_SECURITY_DATABASE,
      &gEdkiiDeviceSignatureDatabaseGuid,
      SignatureList,
      SignatureListSize
      );
    FreePool (SignatureList);
  }

  Status = gRT->SetVariable (
                  L"PrivDevKey",
                  &gEdkiiDeviceSignatureDatabaseGuid,
                  EFI_VARIABLE_NON_VOLATILE |
                    EFI_VARIABLE_BOOTSERVICE_ACCESS |
                    EFI_VARIABLE_RUNTIME_ACCESS,
                  TestRootKeySize,
                  TestRootKey
                  );
  ASSERT_EFI_ERROR(Status);
  return EFI_SUCCESS;
}
