# This is a sample implementation for UEFI SPDM requester

## Feature

1) A UEFI SPDM requester as DeviceSecurity driver.

2) A UEFI test stub as SPDM responder.

   Both Code can run in UEFI emulation env.

## Build

This repo uses below submodules:

  DeviceSecurityPkg/Library/SpdmLib/openspdm

Build:
  Follow standard EDKII build process for DeviceSecurityPkg.

Run :
  Copy all *.efi in Build\DeviceSecurityPkg\<TARGET>_<TOOLCHAIN>\<ARCH>\*.efi to Build\EmulationPkg\<TARGET>_<TOOLCHAIN>\<ARCH>\.

  Boot to UEFI shell and run below command:

  ```
    DeployCert.efi
    load DeviceSecurityPolicyStub.efi
    load PciIoStub.efi
    load Tcg2Stub.efi
    load SpdmStub.efi
    load SpdmDeviceSecurityDxe.efi
    TestSpdm.efi
  ```

  To test PCI DOE, boot to UEFI shell and run below command:

  ```
    DeployCert.efi
    load DeviceSecurityPolicyStub.efi
    #load PciIoStub.efi
    load PciIoPciDoeStub.efi
    load Tcg2Stub.efi
    #load SpdmStub.efi
    load SpdmPciDoeStub.efi
    load SpdmDeviceSecurityDxe.efi
    TestSpdm.efi
  ```

  In EmulationPkg, the PEI SPDM module can only be launched in second boot, after DeployCert.efi in UEFI shell.

## Known limitation
This package is only the sample code to show the concept.
It does not have a full validation such as robustness functional test and fuzzing test. It does not meet the production quality yet.
Any codes including the API definition, the libary and the drivers are subject to change.

