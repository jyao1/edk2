/** @file
  EDKII Tcg2 Stub

  Copyright (c) 2020, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <Uefi.h>
#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/DevicePathLib.h>
#include <Protocol/Tcg2Protocol.h>

/**

  This function dump raw data.

  @param  Data  raw data
  @param  Size  raw data size

**/
VOID
InternalDumpData (
  IN UINT8  *Data,
  IN UINTN  Size
  )
{
  UINTN  Index;
  for (Index = 0; Index < Size; Index++) {
    DEBUG ((EFI_D_INFO, "%02x ", (UINTN)Data[Index]));
  }
}

/**

  This function dump raw data with colume format.

  @param  Data  raw data
  @param  Size  raw data size

**/
VOID
InternalDumpHex (
  IN UINT8  *Data,
  IN UINTN  Size
  )
{
  UINTN   Index;
  UINTN   Count;
  UINTN   Left;

#define COLUME_SIZE  (16 * 2)

  Count = Size / COLUME_SIZE;
  Left  = Size % COLUME_SIZE;
  for (Index = 0; Index < Count; Index++) {
    DEBUG ((EFI_D_INFO, "%04x: ", Index * COLUME_SIZE));
    InternalDumpData (Data + Index * COLUME_SIZE, COLUME_SIZE);
    DEBUG ((EFI_D_INFO, "\n"));
  }

  if (Left != 0) {
    DEBUG ((EFI_D_INFO, "%04x: ", Index * COLUME_SIZE));
    InternalDumpData (Data + Index * COLUME_SIZE, Left);
    DEBUG ((EFI_D_INFO, "\n"));
  }
}

EFI_STATUS
EFIAPI
GetCapability (
  IN EFI_TCG2_PROTOCOL                    *This,
  IN OUT EFI_TCG2_BOOT_SERVICE_CAPABILITY *ProtocolCapability
  )
{
  ASSERT(FALSE);
  return EFI_UNSUPPORTED;
}

EFI_STATUS
EFIAPI
GetEventLog (
  IN EFI_TCG2_PROTOCOL         *This,
  IN EFI_TCG2_EVENT_LOG_FORMAT EventLogFormat,
  OUT EFI_PHYSICAL_ADDRESS     *EventLogLocation,
  OUT EFI_PHYSICAL_ADDRESS     *EventLogLastEntry,
  OUT BOOLEAN                  *EventLogTruncated
  )
{
  ASSERT(FALSE);
  return EFI_UNSUPPORTED;
}

EFI_STATUS
EFIAPI
HashLogExtendEvent (
  IN EFI_TCG2_PROTOCOL    *This,
  IN UINT64               Flags,
  IN EFI_PHYSICAL_ADDRESS DataToHash,
  IN UINT64               DataToHashLen,
  IN EFI_TCG2_EVENT       *EfiTcgEvent
  )
{
  UINTN   LogLen;

  DEBUG ((DEBUG_INFO, "HashLogExtendEvent:\n", DataToHashLen));

  DEBUG ((DEBUG_INFO, "  EfiTcgEvent\n"));
  DEBUG ((DEBUG_INFO, "    Size - 0x%08x\n", EfiTcgEvent->Size));
  DEBUG ((DEBUG_INFO, "    HeaderSize    - 0x%08x\n", EfiTcgEvent->Header.HeaderSize));
  DEBUG ((DEBUG_INFO, "    HeaderVersion - 0x%04x\n", EfiTcgEvent->Header.HeaderVersion));
  DEBUG ((DEBUG_INFO, "    PCRIndex      - 0x%08x\n", EfiTcgEvent->Header.PCRIndex));
  DEBUG ((DEBUG_INFO, "    EventType     - 0x%08x\n", EfiTcgEvent->Header.EventType));
  LogLen = EfiTcgEvent->Size - (sizeof (EfiTcgEvent->Size) + sizeof(EfiTcgEvent->Header));
  DEBUG ((DEBUG_INFO, "  EventLog (0x%x):\n", LogLen));
  InternalDumpHex (EfiTcgEvent->Event, LogLen);

  DEBUG ((DEBUG_INFO, "  DataToHash (0x%x):\n", DataToHashLen));
  InternalDumpHex ((VOID *)(UINTN)DataToHash, (UINTN)DataToHashLen);

  return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
SubmitCommand (
  IN EFI_TCG2_PROTOCOL *This,
  IN UINT32            InputParameterBlockSize,
  IN UINT8             *InputParameterBlock,
  IN UINT32            OutputParameterBlockSize,
  IN UINT8             *OutputParameterBlock
  )
{
  ASSERT(FALSE);
  return EFI_UNSUPPORTED;
}

EFI_STATUS
EFIAPI
GetActivePcrBanks (
  IN  EFI_TCG2_PROTOCOL *This,
  OUT UINT32            *ActivePcrBanks
  )
{
  ASSERT(FALSE);
  return EFI_UNSUPPORTED;
}

EFI_STATUS
EFIAPI
SetActivePcrBanks (
  IN EFI_TCG2_PROTOCOL *This,
  IN UINT32            ActivePcrBanks
  )
{
  ASSERT(FALSE);
  return EFI_UNSUPPORTED;
}

EFI_STATUS
EFIAPI
GetResultOfSetActivePcrBanks (
  IN  EFI_TCG2_PROTOCOL  *This,
  OUT UINT32             *OperationPresent,
  OUT UINT32             *Response
  )
{
  ASSERT(FALSE);
  return EFI_UNSUPPORTED;
}

EFI_TCG2_PROTOCOL  mTcg2Protocol = {
  GetCapability,
  GetEventLog,
  HashLogExtendEvent,
  SubmitCommand,
  GetActivePcrBanks,
  SetActivePcrBanks,
  GetResultOfSetActivePcrBanks
};

EFI_HANDLE  mTcg2Handle;

EFI_STATUS
EFIAPI
MainEntryPoint (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
  EFI_STATUS  Status;

  Status = gBS->InstallProtocolInterface (
                  &mTcg2Handle,
                  &gEfiTcg2ProtocolGuid,
                  EFI_NATIVE_INTERFACE,
                  &mTcg2Protocol
                  );

  return Status;
}
