/*++

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


Module Name:

  PlatformBoards.h

Abstract:

  Board config definitions for each of the boards supported by this platform
  package.

--*/
#include "Platform.h"

#ifndef __PLATFORM_BOARDS_H__
#define __PLATFORM_BOARDS_H__

//
// Constant definition
//

//
// Basic Configs for GPIO table definitions.
//
#define NULL_LEGACY_GPIO_INITIALIZER                    {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}
#define ALL_INPUT_LEGACY_GPIO_INITIALIZER               {0x03,0x03,0x00,0x00,0x00,0x00,0x00,0x03,0x00,0x3f,0x3f,0x00,0x00,0x00,0x00,0x00,0x3f,0x00}
#define QUARK_EMULATION_LEGACY_GPIO_INITIALIZER         ALL_INPUT_LEGACY_GPIO_INITIALIZER
#define CLANTON_PEAK_SVP_LEGACY_GPIO_INITIALIZER        {0x03,0x03,0x00,0x00,0x00,0x00,0x00,0x03,0x00,0x3f,0x3f,0x00,0x00,0x3f,0x3f,0x00,0x3f,0x00}
#define KIPS_BAY_LEGACY_GPIO_INITIALIZER                {0x03,0x03,0x00,0x00,0x00,0x00,0x00,0x03,0x00,0x3f,0x25,0x10,0x00,0x00,0x00,0x00,0x3f,0x00}
#define CROSS_HILL_LEGACY_GPIO_INITIALIZER              {0x03,0x03,0x00,0x00,0x00,0x00,0x00,0x03,0x00,0x3f,0x03,0x10,0x00,0x03,0x03,0x00,0x3f,0x00}
#define CLANTON_HILL_LEGACY_GPIO_INITIALIZER            {0x03,0x03,0x00,0x00,0x00,0x00,0x00,0x03,0x00,0x3f,0x06,0x10,0x00,0x04,0x04,0x00,0x3f,0x00}
#define GALILEO_LEGACY_GPIO_INITIALIZER                 {0x03,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0x00,0x3f,0x21,0x14,0x00,0x00,0x00,0x00,0x3f,0x00}

#define NULL_GPIO_CONTROLLER_INITIALIZER                {0,0,0,0,0,0,0,0}
#define ALL_INPUT_GPIO_CONTROLLER_INITIALIZER           NULL_GPIO_CONTROLLER_INITIALIZER
#define QUARK_EMULATION_GPIO_CONTROLLER_INITIALIZER     NULL_GPIO_CONTROLLER_INITIALIZER
#define CLANTON_PEAK_SVP_GPIO_CONTROLLER_INITIALIZER    NULL_GPIO_CONTROLLER_INITIALIZER
#define KIPS_BAY_GPIO_CONTROLLER_INITIALIZER            {0x05,0x05,0,0,0,0,0,0}
#define CROSS_HILL_GPIO_CONTROLLER_INITIALIZER          {0x0D,0x2D,0,0,0,0,0,0}
#define CLANTON_HILL_GPIO_CONTROLLER_INITIALIZER        {0x01,0x39,0,0,0,0,0,0}
#define GALILEO_GPIO_CONTROLLER_INITIALIZER             {0x05,0x15,0,0,0,0,0,0}

//
// Legacy GPIO config struct for each element in PLATFORM_LEGACY_GPIO_TABLE_DEFINITION.
//
typedef struct {
  UINT32  CoreWellEnable;               ///< Value for QNC NC Reg R_QNC_GPIO_CGEN_CORE_WELL.
  UINT32  CoreWellIoSelect;             ///< Value for QNC NC Reg R_QNC_GPIO_CGIO_CORE_WELL.
  UINT32  CoreWellLvlForInputOrOutput;  ///< Value for QNC NC Reg R_QNC_GPIO_CGLVL_CORE_WELL.
  UINT32  CoreWellTriggerPositiveEdge;  ///< Value for QNC NC Reg R_QNC_GPIO_CGTPE_CORE_WELL.
  UINT32  CoreWellTriggerNegativeEdge;  ///< Value for QNC NC Reg R_QNC_GPIO_CGTNE_CORE_WELL.
  UINT32  CoreWellGPEEnable;            ///< Value for QNC NC Reg R_QNC_GPIO_CGGPE_CORE_WELL.
  UINT32  CoreWellSMIEnable;            ///< Value for QNC NC Reg R_QNC_GPIO_CGSMI_CORE_WELL.
  UINT32  CoreWellTriggerStatus;        ///< Value for QNC NC Reg R_QNC_GPIO_CGTS_CORE_WELL.
  UINT32  CoreWellNMIEnable;            ///< Value for QNC NC Reg R_QNC_GPIO_CGNMIEN_CORE_WELL.
  UINT32  ResumeWellEnable;             ///< Value for QNC NC Reg R_QNC_GPIO_RGEN_RESUME_WELL.
  UINT32  ResumeWellIoSelect;           ///< Value for QNC NC Reg R_QNC_GPIO_RGIO_RESUME_WELL.
  UINT32  ResumeWellLvlForInputOrOutput;///< Value for QNC NC Reg R_QNC_GPIO_RGLVL_RESUME_WELL.
  UINT32  ResumeWellTriggerPositiveEdge;///< Value for QNC NC Reg R_QNC_GPIO_RGTPE_RESUME_WELL.
  UINT32  ResumeWellTriggerNegativeEdge;///< Value for QNC NC Reg R_QNC_GPIO_RGTNE_RESUME_WELL.
  UINT32  ResumeWellGPEEnable;          ///< Value for QNC NC Reg R_QNC_GPIO_RGGPE_RESUME_WELL.
  UINT32  ResumeWellSMIEnable;          ///< Value for QNC NC Reg R_QNC_GPIO_RGSMI_RESUME_WELL.
  UINT32  ResumeWellTriggerStatus;      ///< Value for QNC NC Reg R_QNC_GPIO_RGTS_RESUME_WELL.
  UINT32  ResumeWellNMIEnable;          ///< Value for QNC NC Reg R_QNC_GPIO_RGNMIEN_RESUME_WELL.
} BOARD_LEGACY_GPIO_CONFIG;

//
// GPIO controller config struct for each element in PLATFORM_GPIO_CONTROLLER_CONFIG_DEFINITION.
//
typedef struct {
  UINT32  PortADR;                      ///< Value for IOH REG GPIO_SWPORTA_DR.
  UINT32  PortADir;                     ///< Value for IOH REG GPIO_SWPORTA_DDR.
  UINT32  IntEn;                        ///< Value for IOH REG GPIO_INTEN.
  UINT32  IntMask;                      ///< Value for IOH REG GPIO_INTMASK.
  UINT32  IntType;                      ///< Value for IOH REG GPIO_INTTYPE_LEVEL.
  UINT32  IntPolarity;                  ///< Value for IOH REG GPIO_INT_POLARITY.
  UINT32  Debounce;                     ///< Value for IOH REG GPIO_DEBOUNCE.
  UINT32  LsSync;                       ///< Value for IOH REG GPIO_LS_SYNC.
} BOARD_GPIO_CONTROLLER_CONFIG;

///
/// Table of BOARD_LEGACY_GPIO_CONFIG structures for each board supported
/// by this platform package.
/// Table indexed with EFI_PLATFORM_TYPE enum value.
///
#define PLATFORM_LEGACY_GPIO_TABLE_DEFINITION \
  /* EFI_PLATFORM_TYPE - TypeUnknown*/\
  NULL_LEGACY_GPIO_INITIALIZER,\
  /* EFI_PLATFORM_TYPE - QuarkEmulation*/\
  QUARK_EMULATION_LEGACY_GPIO_INITIALIZER,\
  /* EFI_PLATFORM_TYPE - ClantonPeakSVP*/\
  CLANTON_PEAK_SVP_LEGACY_GPIO_INITIALIZER,\
  /* EFI_PLATFORM_TYPE - KipsBay*/\
  KIPS_BAY_LEGACY_GPIO_INITIALIZER,\
  /* EFI_PLATFORM_TYPE - CrossHill*/\
  CROSS_HILL_LEGACY_GPIO_INITIALIZER,\
  /* EFI_PLATFORM_TYPE - ClantonHill*/\
  CLANTON_HILL_LEGACY_GPIO_INITIALIZER,\
  /* EFI_PLATFORM_TYPE - Galileo*/\
  GALILEO_LEGACY_GPIO_INITIALIZER,\

///
/// Table of BOARD_GPIO_CONTROLLER_CONFIG structures for each board
/// supported by this platform package.
/// Table indexed with EFI_PLATFORM_TYPE enum value.
///
#define PLATFORM_GPIO_CONTROLLER_CONFIG_DEFINITION \
  /* EFI_PLATFORM_TYPE - TypeUnknown*/\
  NULL_GPIO_CONTROLLER_INITIALIZER,\
  /* EFI_PLATFORM_TYPE - QuarkEmulation*/\
  QUARK_EMULATION_GPIO_CONTROLLER_INITIALIZER,\
  /* EFI_PLATFORM_TYPE - ClantonPeakSVP*/\
  CLANTON_PEAK_SVP_GPIO_CONTROLLER_INITIALIZER,\
  /* EFI_PLATFORM_TYPE - KipsBay*/\
  KIPS_BAY_GPIO_CONTROLLER_INITIALIZER,\
  /* EFI_PLATFORM_TYPE - CrossHill*/\
  CROSS_HILL_GPIO_CONTROLLER_INITIALIZER,\
  /* EFI_PLATFORM_TYPE - ClantonHill*/\
  CLANTON_HILL_GPIO_CONTROLLER_INITIALIZER,\
  /* EFI_PLATFORM_TYPE - Galileo*/\
  GALILEO_GPIO_CONTROLLER_INITIALIZER,\

#endif
