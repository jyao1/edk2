/** @file
  Null instance of Sec Platform Hook Lib.

  Copyright(c) 2013 Intel Corporation. All rights reserved.

  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions
  are met:

  * Redistributions of source code must retain the above copyright
  notice, this list of conditions and the following disclaimer.
  * Redistributions in binary form must reproduce the above copyright
  notice, this list of conditions and the following disclaimer in
  the documentation and/or other materials provided with the
  distribution.
  * Neither the name of Intel Corporation nor the names of its
  contributors may be used to endorse or promote products derived
  from this software without specific prior written permission.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.


**/

//
// The package level header files this module uses
//
#include "QuarkSecLib.h"

VOID
CopyRomFv (
  IN OUT   VOID        *SecCoreData
  )
{
  EFI_FIRMWARE_VOLUME_HEADER       *Fv;
  HAND_OFF_INFO                    *Handoff;

  Handoff = SecCoreData;
  DEBUG_WRITE_STRING ("Update BFV...");
  Handoff->BootFirmwareVolumeBase = (VOID *)(UINTN)(FixedPcdGet32 (PcdEsramStage1Base) + FixedPcdGet32(PcdPlatformFlashFvBootRomSize));
  DEBUG_WRITE_UINT32 ((UINT32)(UINTN)Handoff->BootFirmwareVolumeBase);
  DEBUG_WRITE_STRING ("\n");
  Fv = (EFI_FIRMWARE_VOLUME_HEADER *)Handoff->BootFirmwareVolumeBase;
  CopyMem ((VOID *)(UINTN)FixedPcdGet32(PcdPlatformBootFvMemBase), Handoff->BootFirmwareVolumeBase, (UINTN)Fv->FvLength);
  Handoff->BootFirmwareVolumeBase = (VOID *)(UINTN)FixedPcdGet32(PcdPlatformBootFvMemBase);
  DEBUG_WRITE_STRING ("Update BFV done.\n");
}
