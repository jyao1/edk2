/** @file
  EDKII Device Security library for SPDM device.
  It follows the SPDM Specification.

Copyright (c) 2020, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include "SpdmDeviceSecurityDxe.h"

extern SPDM_IO_PROTOCOL                   *mSpdmIoProtocol;

RETURN_STATUS
EFIAPI
SpdmDeviceSendMessage (
  IN     VOID                                   *SpdmContext,
  IN     UINTN                                  MessageSize,
  IN     CONST VOID                             *Message,
  IN     UINT64                                 Timeout
  )
{
  if (mSpdmIoProtocol == NULL) {
    return RETURN_NOT_FOUND;
  }
  return mSpdmIoProtocol->SendMessage (mSpdmIoProtocol, MessageSize, Message, Timeout);
}

RETURN_STATUS
EFIAPI
SpdmDeviceReceiveMessage (
  IN     VOID                                   *SpdmContext,
  IN OUT UINTN                                  *MessageSize,
  IN OUT VOID                                   **Message,
  IN     UINT64                                 Timeout
  )
{
  if (mSpdmIoProtocol == NULL) {
    return RETURN_NOT_FOUND;
  }
  return mSpdmIoProtocol->ReceiveMessage (mSpdmIoProtocol, MessageSize, Message, Timeout);
}
