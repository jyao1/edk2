/** @file
  EDKII TestSpdm

  Copyright (c) 2020, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <Library/PeimEntryPoint.h>
#include <Library/PeiServicesLib.h>
#include <Library/PeiServicesTablePointerLib.h>
#include <Uefi.h>
#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiLib.h>
#include <Library/SpdmRequesterLib.h>
#include <Protocol/PciIo.h>
#include <Ppi/SpdmIoPpi.h>
#include <Ppi/SpdmPpi.h>
#include <Ppi/SpdmTestPpi.h>
#include <Protocol/DeviceSecurity.h>

#define USE_PSK 0

VOID
InternalDumpData (
  IN UINT8  *Data,
  IN UINTN  Size
  )
{
  UINTN  Index;
  for (Index = 0; Index < Size; Index++) {
    DEBUG ((DEBUG_INFO, "%02x ", (UINTN)Data[Index]));
  }
}

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
    DEBUG ((DEBUG_INFO, "%04x: ", Index * COLUME_SIZE));
    InternalDumpData (Data + Index * COLUME_SIZE, COLUME_SIZE);
    DEBUG ((DEBUG_INFO, "\n"));
  }

  if (Left != 0) {
    DEBUG ((DEBUG_INFO, "%04x: ", Index * COLUME_SIZE));
    InternalDumpData (Data + Index * COLUME_SIZE, Left);
    DEBUG ((DEBUG_INFO, "\n"));
  }
}

VOID
TestPci (
  VOID
  )
{
  EFI_STATUS                 Status;
  EDKII_DEVICE_SECURITY_PPI  *DeviceSecurityPpi;
  EDKII_DEVICE_IDENTIFIER    DeviceId;

  Status = PeiServicesLocatePpi(
             &gEdkiiDeviceSecurityPpiGuid,
             0,
             NULL,
             (VOID**)&DeviceSecurityPpi
             );
  if (EFI_ERROR(Status)) {
    DEBUG((DEBUG_ERROR, "LocatePpi gEdkiiDeviceSecurityPpiGuid - %r\n", Status));
    return;
  }

  DeviceId.Version = EDKII_DEVICE_IDENTIFIER_REVISION;
  CopyGuid (&DeviceId.DeviceType, &gEdkiiDeviceIdentifierTypePciGuid);

  Status = DeviceSecurityPpi->DeviceAuthenticate (DeviceSecurityPpi, &DeviceId);
  if (EFI_ERROR(Status)) {
    DEBUG((DEBUG_ERROR, " DeviceSecurityPpi->DeviceAuthenticate - %r\n", Status));
    return;
  }
#if 0
  EFI_STATUS                      Status;
  SPDM_IO_PROTOCOL                *SpdmIo;
  EFI_HANDLE                      Handle;
  UINTN                           BufferSize;
  EDKII_DEVICE_SECURITY_PROTOCOL  *DeviceSecurity;
  EDKII_DEVICE_IDENTIFIER         DeviceId;

  Status = gBS->LocateProtocol (&gSpdmIoProtocolGuid, NULL, &SpdmIo);
  ASSERT_EFI_ERROR(Status);

  BufferSize = sizeof(Handle);
  Status = gBS->LocateHandle (
                  ByProtocol,
                  &gEdkiiDeviceIdentifierTypePciGuid,
                  NULL,
                  &BufferSize,
                  &Handle
                  );
  ASSERT_EFI_ERROR(Status);

  Status = gBS->InstallProtocolInterface (
                  &Handle,
                  &gSpdmIoProtocolGuid,
                  EFI_NATIVE_INTERFACE,
                  SpdmIo
                  );
  ASSERT_EFI_ERROR(Status);
  
  Status = gBS->LocateProtocol (&gEdkiiDeviceSecurityProtocolGuid, NULL, &DeviceSecurity);
  ASSERT_EFI_ERROR(Status);

  DeviceId.Version = EDKII_DEVICE_IDENTIFIER_REVISION;
  CopyGuid (&DeviceId.DeviceType, &gEdkiiDeviceIdentifierTypePciGuid);
  DeviceId.DeviceHandle = Handle;
  Status = DeviceSecurity->DeviceAuthenticate (DeviceSecurity, &DeviceId);
#endif
}

typedef struct {
  SPDM_DATA_TYPE  DataType;
  CHAR8                 *String;
} DATA_TYPE_STRING;

DATA_TYPE_STRING  mDataTypeString[] = {
  {SpdmDataDheSecret,        "DheSecret"},
  {SpdmDataHandshakeSecret,  "HandshakeSecret"},
  {SpdmDataMasterSecret,     "MasterSecret"},
  {SpdmDataRequestHandshakeSecret,   "RequestHandshakeSecret"},
  {SpdmDataResponseHandshakeSecret,  "ResponseHandshakeSecret"},
  {SpdmDataRequestDataSecret,        "RequestDataSecret"},
  {SpdmDataResponseDataSecret,       "ResponseDataSecret"},
  {SpdmDataRequestFinishedKey,       "RequestFinishedKey"},
  {SpdmDataResponseFinishedKey,      "ResponseFinishedKey"},
  {SpdmDataRequestHandshakeEncryptionKey,  "RequestHandshakeEncryptionKey"},
  {SpdmDataRequestHandshakeSalt,           "RequestHandshakeSalt"},
  {SpdmDataResponseHandshakeEncryptionKey, "ResponseHandshakeEncryptionKey"},
  {SpdmDataResponseHandshakeSalt,          "ResponseHandshakeSalt"},
  {SpdmDataRequestDataEncryptionKey,       "RequestDataEncryptionKey"},
  {SpdmDataRequestDataSalt,                "RequestDataSalt"},
  {SpdmDataResponseDataEncryptionKey,      "ResponseDataEncryptionKey"},
  {SpdmDataResponseDataSalt,               "ResponseDataSalt"},
};

#pragma pack(1)
#define TEST_PAYLOAD_CLIENT "Hello Client!"
#define TEST_PAYLOAD_SERVER "Hello Server!"
#define TEST_PAYLOAD_LEN (sizeof("Hello XXXXXX!"))
///
/// SPDM VENDOR_DEFINED request
///
typedef struct {
  SPDM_MESSAGE_HEADER  Header;
  // Param1 == RSVD
  // Param2 == RSVD
  UINT16               StandardID;
  UINT8                Len;
  UINT16               VendorID;
  UINT16               PayloadLength;
  UINT8                VendorDefinedPayload[TEST_PAYLOAD_LEN];
} SPDM_VENDOR_DEFINED_REQUEST_MINE;

///
/// SPDM VENDOR_DEFINED response
///
typedef struct {
  SPDM_MESSAGE_HEADER  Header;
  // Param1 == RSVD
  // Param2 == RSVD
  UINT16               StandardID;
  UINT8                Len;
  UINT16               VendorID;
  UINT16               PayloadLength;
  UINT8                VendorDefinedPayload[TEST_PAYLOAD_LEN];
} SPDM_VENDOR_DEFINED_RESPONSE_MINE;

#pragma pack()

SPDM_VENDOR_DEFINED_REQUEST_MINE  mVendorDefinedRequest = {
  {
    SPDM_MESSAGE_VERSION_10,
    SPDM_VENDOR_DEFINED_REQUEST,
    0, // Param1
    0, // Param2
  },
  SPDM_EXTENDED_ALGORITHM_REGISTRY_ID_PCISIG, // StandardID
  2, // Len
  0x8086, // VendorID
  TEST_PAYLOAD_LEN, // PayloadLength
  {TEST_PAYLOAD_CLIENT}
};

SPDM_VENDOR_DEFINED_REQUEST_MINE  mVendorDefinedResponse = {
  {
    SPDM_MESSAGE_VERSION_10,
    SPDM_VENDOR_DEFINED_RESPONSE,
    0, // Param1
    0, // Param2
  },
  SPDM_EXTENDED_ALGORITHM_REGISTRY_ID_PCISIG, // StandardID
  2, // Len
  0x8086, // VendorID
  TEST_PAYLOAD_LEN, // PayloadLength
  {TEST_PAYLOAD_SERVER}
};

/**
  Process a packet in the current SPDM session.

  @param  This                         Indicates a pointer to the calling context.
  @param  SessionId                    ID of the session.
  @param  Request                      A pointer to the request data.
  @param  RequestSize                  Size of the request data.
  @param  Response                     A pointer to the response data.
  @param  ResponseSize                 Size of the response data. On input, it means the size of Data
                                       buffer. On output, it means the size of copied Data buffer if
                                       EFI_SUCCESS, and means the size of desired Data buffer if
                                       EFI_BUFFER_TOO_SMALL.

  @retval EFI_SUCCESS                  The SPDM request is set successfully.
  @retval EFI_INVALID_PARAMETER        The DataSize is NULL or the Data is NULL and *DataSize is not zero.
  @retval EFI_UNSUPPORTED              The DataType is unsupported.
  @retval EFI_NOT_FOUND                The DataType cannot be found.
  @retval EFI_NOT_READY                The DataType is not ready to return.
  @retval EFI_BUFFER_TOO_SMALL         The buffer is too small to hold the data.
  @retval EFI_TIMEOUT                  A timeout occurred while waiting for the SPDM request
                                       to execute.
**/
EFI_STATUS
EFIAPI
TestSpdmProcessPacketCallback (
  IN     UINT32                       SessionId,
  IN     VOID                         *Request,
  IN     UINTN                        RequestSize,
     OUT VOID                         *Response,
  IN OUT UINTN                        *ResponseSize
  )
{
  SPDM_VENDOR_DEFINED_REQUEST_MINE   *SpmdRequest;
  SpmdRequest = Request;
  ASSERT (RequestSize == sizeof(SPDM_VENDOR_DEFINED_REQUEST_MINE));
  ASSERT (SpmdRequest->Header.RequestResponseCode == SPDM_VENDOR_DEFINED_REQUEST);
  ASSERT (SpmdRequest->StandardID == SPDM_EXTENDED_ALGORITHM_REGISTRY_ID_PCISIG);
  ASSERT (SpmdRequest->VendorID == 0x8086);
  ASSERT (SpmdRequest->PayloadLength == TEST_PAYLOAD_LEN);
  ASSERT (CompareMem (SpmdRequest->VendorDefinedPayload, TEST_PAYLOAD_CLIENT, TEST_PAYLOAD_LEN) == 0);

  CopyMem (Response, &mVendorDefinedResponse, sizeof(mVendorDefinedResponse));
  *ResponseSize = sizeof(mVendorDefinedResponse);
  return EFI_SUCCESS;
}

VOID
TestSpdmApplication (
  IN SPDM_PPI      *SpdmPpi,
  IN SPDM_TEST_PPI *SpdmTestPpi,
  IN UINT32        SessionId
  )
{
  EFI_STATUS                         Status;
  SPDM_VENDOR_DEFINED_REQUEST_MINE   Request;
  UINTN                              RequestSize;
  SPDM_VENDOR_DEFINED_RESPONSE_MINE  Response;
  UINTN                              ResponseSize;

  Status = SpdmTestPpi->RegisterProcessPacketCallback (SpdmTestPpi, TestSpdmProcessPacketCallback);

  CopyMem (&Request, &mVendorDefinedRequest, sizeof(Request));

  RequestSize = sizeof(Request);
  ResponseSize = sizeof(Response);
  Status = SpdmPpi->SendReceiveSessionData (SpdmPpi, SessionId, &Request, RequestSize, &Response, &ResponseSize);
  ASSERT_EFI_ERROR(Status);

  ASSERT (ResponseSize == sizeof(SPDM_VENDOR_DEFINED_RESPONSE_MINE));
  ASSERT (Response.Header.RequestResponseCode == SPDM_VENDOR_DEFINED_RESPONSE);
  ASSERT (Response.StandardID == SPDM_EXTENDED_ALGORITHM_REGISTRY_ID_PCISIG);
  ASSERT (Response.VendorID == 0x8086);
  ASSERT (Response.PayloadLength == TEST_PAYLOAD_LEN);
  ASSERT (CompareMem (Response.VendorDefinedPayload, TEST_PAYLOAD_SERVER, TEST_PAYLOAD_LEN) == 0);
}

VOID
TestSpdm (
  VOID
  )
{  
  EFI_STATUS                       Status;
  SPDM_PPI                         *SpdmPpi;
  SPDM_TEST_PPI                    *SpdmTestPpi;
  UINTN                            DataSize;
  UINT8                            Data[512];
  UINTN                            TestDataSize;
  UINT8                            TestData[512];
  UINTN                            Index;
  UINT32                           SessionId;
  SPDM_DATA_PARAMETER              Parameter;
  UINT8                            HeartbeatPeriod;
  UINT8                            MeasurementHash[64];

  Status = PeiServicesLocatePpi(
             &gSpdmPpiGuid,
             0,
             NULL,
             (VOID**)&SpdmPpi
             );
  ASSERT_EFI_ERROR(Status);

  Status = PeiServicesLocatePpi(
             &gSpdmTestPpiGuid,
             0,
             NULL,
             (VOID**)&SpdmTestPpi
             );
  ASSERT_EFI_ERROR(Status);

#if USE_PSK
  Status = SpdmPpi->SetData (SpdmPpi, SpdmDataPsk, NULL, "TestPskData", sizeof("TestPskData"));
  ASSERT_EFI_ERROR(Status);

  Status = SpdmTestPpi->SetData (SpdmTestPpi, SpdmDataPsk, NULL, "TestPskData", sizeof("TestPskData"));
  ASSERT_EFI_ERROR(Status);
#endif

  HeartbeatPeriod = 0;
  ZeroMem(MeasurementHash, sizeof(MeasurementHash));
  Status = SpdmPpi->StartSession (
                           SpdmPpi,
                           USE_PSK,
                           SPDM_CHALLENGE_REQUEST_TCB_COMPONENT_MEASUREMENT_HASH,
                           0,
                           &HeartbeatPeriod,
                           &SessionId,
                           MeasurementHash
                           );
  //ASSERT_EFI_ERROR(Status);
  if (EFI_ERROR(Status)) {
    DEBUG((DEBUG_ERROR, "TestSpdm(): SpdmPpi->StartSession - %r \n", Status));
    return;
  }

  ZeroMem (&Parameter, sizeof(Parameter));
  Parameter.Location = SpdmDataLocationSession;
  *(UINT32 *)Parameter.AdditionalData = SessionId;
  for (Index = 0; Index < ARRAY_SIZE(mDataTypeString); Index++) {
    DataSize = sizeof(Data);
    ZeroMem (Data, sizeof(Data));
    Status = SpdmPpi->GetData (SpdmPpi, mDataTypeString[Index].DataType, &Parameter, Data, &DataSize);
    if (!EFI_ERROR(Status)) {
      DEBUG ((DEBUG_INFO, "%a (%d) - ", mDataTypeString[Index].String, DataSize));
      InternalDumpData (Data, DataSize);
      DEBUG ((DEBUG_INFO, "\n"));
      
      TestDataSize = sizeof(TestData);
      ZeroMem (TestData, sizeof(TestData));
      Status = SpdmTestPpi->GetData (SpdmTestPpi, mDataTypeString[Index].DataType, &Parameter, TestData, &TestDataSize);
      ASSERT_EFI_ERROR(Status);
      if (DataSize != TestDataSize) {
        DEBUG ((DEBUG_INFO, "-- Verify DataSize FAIL, "));
      } else {
        DEBUG ((DEBUG_INFO, "-- Verify DataSize PASS, "));
      }
      if (CompareMem(Data, TestData, DataSize) != 0) {
        DEBUG ((DEBUG_INFO, "Verify Data FAIL\n"));
      } else {
        DEBUG ((DEBUG_INFO, "Verify Data PASSL\n"));
      }

    } else {
      DEBUG ((DEBUG_INFO, "%a - %r\n", mDataTypeString[Index].String, Status));
    }
  }

  TestSpdmApplication (SpdmPpi, SpdmTestPpi, SessionId);
  
  Status = SpdmPpi->StopSession (SpdmPpi, SessionId, 0);
  ASSERT_EFI_ERROR(Status);
}

EFI_STATUS
EFIAPI
MainEntryPoint (
  IN       EFI_PEI_FILE_HANDLE  FileHandle,
  IN CONST EFI_PEI_SERVICES     **PeiServices
  )
{
  //CpuBreakpoint();
  TestPci ();

  TestSpdm ();
  return EFI_SUCCESS;
}
