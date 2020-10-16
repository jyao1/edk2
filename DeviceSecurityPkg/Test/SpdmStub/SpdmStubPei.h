/** @file
  EDKII SpdmIo Stub

  Copyright (c) 2020, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#ifndef _SPDM_STUB_H_
#define _SPDM_STUB_H_

#include <Uefi.h>
#include <IndustryStandard/Spdm.h>
#include <IndustryStandard/SpdmSecureMessage.h>
#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/DevicePathLib.h>
#include <Library/UefiLib.h>
#include <Library/RngLib.h>
#include <Library/BaseCryptLib.h>
#include <Library/SpdmResponderLib.h>
#include <Guid/DeviceAuthentication.h>
#include <Guid/ImageAuthentication.h>
#include <Ppi/SpdmIoPpi.h>
#include <Ppi/SpdmPpi.h>
#include <Ppi/SpdmTestPpi.h>


typedef struct {
  UINTN                                           Signature;
  EFI_HANDLE                                      SpdmHandle;
  SPDM_IO_PPI                                     SpdmIoPpi;
  SPDM_TEST_PPI                                   SpdmTestPpi;
  SPDM_TEST_PROCESS_PACKET_CALLBACK               ProcessPacketCallback;
  VOID                                            *SpdmContext;
} SPDM_TEST_DEVICE_CONTEXT;

#define SPDM_TEST_DEVICE_CONTEXT_SIGNATURE  SIGNATURE_32 ('S', 'T', 'D', 'C')
#define SPDM_TEST_DEVICE_CONTEXT_FROM_SPDM_TEST_PROTOCOL(a)  CR (a, SPDM_TEST_DEVICE_CONTEXT, SpdmTestPpi, SPDM_TEST_DEVICE_CONTEXT_SIGNATURE)
#define SPDM_TEST_DEVICE_CONTEXT_FROM_SPDM_IO_PROTOCOL(a)  CR (a, SPDM_TEST_DEVICE_CONTEXT, SpdmIoPpi, SPDM_TEST_DEVICE_CONTEXT_SIGNATURE)

VOID
InitializeSpdmTest (
  IN OUT SPDM_TEST_DEVICE_CONTEXT  *SpdmTestDeviceContext
  );

RETURN_STATUS
EFIAPI
SpdmDeviceSendMessage (
  IN     VOID                                   *SpdmContext,
  IN     UINT32                                 *SessionId,
  IN     UINTN                                  MessageSize,
  IN     VOID                                   *Message,
  IN     UINT64                                 Timeout
  );

RETURN_STATUS
EFIAPI
SpdmDeviceReceiveMessage (
  IN     VOID                                   *SpdmContext,
     OUT UINT32                                 **SessionId,
  IN OUT UINTN                                  *MessageSize,
  IN OUT VOID                                   *Message,
  IN     UINT64                                 Timeout
  );

extern EFI_HANDLE  mSpdmHandle;

#endif

