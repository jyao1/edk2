/** @file
  TCG SPDM extension

  Copyright (c) 2020, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#ifndef _TCG_SPDM_H_
#define _TCG_SPDM_H_

#include <IndustryStandard/UefiTcgPlatform.h>

#define EV_EFI_SPDM_DEVICE_BLOB        EV_EFI_SPDM_FIRMWARE_BLOB
#define EV_EFI_SPDM_DEVICE_CONFIG      EV_EFI_SPDM_FIRMWARE_CONFIG

// The SPDM policy database for SPDM verification.
// It goes to PCR7
#define EV_EFI_SPDM_DEVICE_POLICY      (EV_EFI_EVENT_BASE + 0xE3)

// The SPDM policy authority for SPDM verification for the signature of GET_MEASUREMENT or CHALLENGE_AUTH.
// It goes to PCR7.
#define EV_EFI_SPDM_DEVICE_AUTHORITY   (EV_EFI_EVENT_BASE + 0xE4)

/*
  ======================================================================================================================
   Event Type                    PCR  Event Log                                   Usage
  ======================================================================================================================
   EV_EFI_SPDM_DEVICE_BLOB       2    SPDM_MEASUREMENT_BLOCK (subtype)            MEASUREMENT from device
   EV_EFI_SPDM_DEVICE_CONFIG     3    SPDM_MEASUREMENT_BLOCK (subtype)            MEASUREMENT from device
   EV_EFI_SPDM_DEVICE_BLOB       2    SPDM_MEASUREMENT_SUMMARY_HASH (subtype)     SUMMARY_HASH from device
   EV_EFI_SPDM_DEVICE_BLOB       2    SECURE_VERSION_NUMBER (subtype)             SVN from device
   EV_EFI_SPDM_DEVICE_AUTHORITY  7    SPDM_CERT_CHAIN (subtype)                   MEASUREMENTS signature verification

   EV_EFI_SPDM_DEVICE_POLICY     7    UEFI_VARIABLE_DATA with EFI_SIGNATURE_LIST  Provisioned device public cert.
   EV_EFI_SPDM_DEVICE_AUTHORITY  7    UEFI_VARIABLE_DATA with EFI_SIGNATURE_DATA  CHALLENGE_AUTH signature verification
  ======================================================================================================================
*/


#pragma pack(1)

#define TCG_DEVICE_SECURITY_EVENT_DATA_VERSION2   1

typedef struct {
  UINT8                          Signature[16];
  UINT16                         Version;
  UINT16                         Reserved;
  UINT32                         Length; // Length in bytes for all following structures.
  UINT32                         SpdmHashAlgo;
  UINT32                         DeviceType;
  UINT32                         SubHeaderType;
  UINT32                         SubHeaderLength; // Length in bytes of the sub header followed by.
  UINT64                         SubHeaderUID;    // Universal identifier assigned by the event log creator. It can be used to bind two sub header structure together.
  MEASUREMENT_BLOCK         	   MeasurementBlock;
  UINT64					               DevicePathLength;
  UINT8					                 DevicePath[DevicePathLength];
} DEVICE_SECURITY_EVENT_DATA_HEADER2;

#define TCG_DEVICE_SECURITY_EVENT_DATA_DEVICE_SUB_HEADER_TYPE_SPDM_MEASUREMENT_BLOCK          0
#define TCG_DEVICE_SECURITY_EVENT_DATA_DEVICE_SUB_HEADER_TYPE_SPDM_MEASUREMENT_SUMMARY_HASH   1
#define TCG_DEVICE_SECURITY_EVENT_DATA_DEVICE_SUB_HEADER_TYPE_SPDM_CERT_CHAIN                 2
#define TCG_DEVICE_SECURITY_EVENT_DATA_DEVICE_SUB_HEADER_TYPE_SECURE_VERSION_NUMBER           3

typedef struct {
  UINT16                         SpdmVersion;
  UINT32                         SpdmMeasurementHashAlgo;
//SPDM_MEASUREMENT_BLOCK         SpdmMeasurementBlock;
} DEVICE_SECURITY_EVENT_DATA_SUB_HEADER_SPDM_MEASUREMENT_BLOCK;

typedef struct {
  UINT16                         SpdmVersion;
  UINT32                         SpdmMeasurementHashAlgo;
  UINT8                          SpdmMeasurementSummaryHashType;
//UINT8                          SpdmMeasurementSummaryHash[HashSize];
} DEVICE_SECURITY_EVENT_DATA_SUB_HEADER_SPDM_MEASUREMENT_SUMMARY_HASH;

typedef struct {
  UINT16                         SpdmVersion;
  UINT32                         SpdmHashAlgo;
//SPDM_CERT_CHAIN 	             SpdmCertChain;
} DEVICE_SECURITY_EVENT_DATA_SUB_HEADER_SPDM_CERT_CHAIN;

typedef struct {
  UINT32                         SecureVersionNumberFormat;
//UINT8                          Data[FormatBasedSize];
} DEVICE_SECURITY_EVENT_DATA_SUB_HEADER_SECURE_VERSION_NUMBER;

#define TCG_DEVICE_SECURITY_EVENT_DATA_SECURE_VERSION_NUMBER_FORMAT_UINT64    1

typedef union {
	DEVICE_SECURITY_EVENT_DATA_SUB_HEADER_SPDM_MEASUREMENT_BLOCK          SpdmMeasurementBlock;
	DEVICE_SECURITY_EVENT_DATA_SUB_HEADER_SPDM_MEASUREMENT_SUMMARY_HASH   SpdmMeasurementSummaryHash;
	DEVICE_SECURITY_EVENT_DATA_SUB_HEADER_SPDM_CERT_CHAIN                 SpdmCertChain;
	DEVICE_SECURITY_EVENT_DATA_SUB_HEADER_SECURE_VERSION_NUMBER           Svn;
} DEVICE_SECURITY_EVENT_DATA_SUB_HEADER;

typedef union {
	TCG_DEVICE_SECURITY_EVENT_DATA_PCI_CONTEXT          Pci;
	TCG_DEVICE_SECURITY_EVENT_DATA_USB_CONTEXT          Usb;
} DEVICE_SECURITY_EVENT_DATA_DEVICE_CONTEXT;

typedef struct {
  DEVICE_SECURITY_EVENT_DATA_HEADER2        EventDataHeader;
  DEVICE_SECURITY_EVENT_DATA_SUB_HEADER     EventDataSubHeader;
  DEVICE_SECURITY_EVENT_DATA_DEVICE_CONTEXT DeviceContext;
} DEVICE_SECURITY_EVENT_DATA2;

#pragma pack()

#endif