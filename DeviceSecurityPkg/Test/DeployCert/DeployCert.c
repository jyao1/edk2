/** @file
  EDKII DeployCert

  Copyright (c) 2020, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <Uefi.h>
#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/BaseCryptLib.h>
#include <Guid/DeviceAuthentication.h>
#include <Guid/ImageAuthentication.h>
#include <library/LibspdmStub.h>
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

  CertChainSize = sizeof(SPDM_CERT_CHAIN) + SHA256_HASH_SIZE + TestRootCerSize;
  CertChain = AllocateZeroPool (CertChainSize);
  ASSERT (CertChain != NULL);
  CertChain->length = (UINT16)CertChainSize;
  CertChain->reserved = 0;
  Sha256HashAll (TestRootCer, TestRootCerSize, (VOID *)(CertChain + 1));
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
