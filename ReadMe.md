# OpenSSL 1.1 replacement

This branch will be used to develop the OpenSSL 1.1 replacement for EDKII CryptoPkg.

## Problem Statement
According to https://www.OpenSSL.org/policies/releasestrat.html, OpenSSL Version 1.1.1 will be supported until 2023-09-11 (LTS).

We need find replacement in EDKII CryptoPkg.

Bugzilla:
1. OpenSSL3.0: https://bugzilla.tianocore.org/show_bug.cgi?id=3466
2. MbedTls: https://bugzilla.tianocore.org/show_bug.cgi?id=4177

## Investigation Result

1. [OpenSSL 3.0](https://www.OpenSSL.org/) - [github](https://github.com/OpenSSL/OpenSSL/) (V)

License: OpenSSL

POC: https://github.com/kraxel/edk2/tree/archive/openssl3-v1

PROs: The natural successor.

CONs: It brings size issue according to the POC evaluation - https://edk2.groups.io/g/devel/topic/87479913.
The concern is that: The size of flash is fixed. If we naive replace it directly, the existing platforms may be broken immediately.

2. [Mbedtls](https://www.trustedfirmware.org/projects/mbed-tls/) 2.x or 3.x - [github](https://github.com/Mbed-TLS/mbedtls) (V)

License: Apache

POC: https://github.com/jyao1/edk2/tree/DeviceSecurity/CryptoMbedTlsPkg

PROs: Small size

CONs: Feature gap (e.g. SHA-3). See bugzilla for more detail.

3. [Intel-IPP](https://software.intel.com/en-us/intel-ipp) - [github](https://github.com/intel/ipp-crypto) (X)

License: Apache

POC: https://github.com/slimbootloader/slimbootloader/tree/master/BootloaderCommonPkg/Library/IppCryptoLib

CONs: Only crypto primitive, no certificate, no TLS

4. [Libsodium](https://doc.libsodium.org/) - [github](https://github.com/jedisct1/libsodium) (X)

License: ISC

CONs: Only crypto primitive, no certificate, no TLS

5. BoringSSL - [github](https://github.com/google/boringssl) (X)

License: ISC

CONs: Google only project. It says: "We don't recommend that third parties depend upon it."

6. [WolfSSL](https://www.wolfssl.com/) - [github](https://github.com/wolfSSL/wolfssl) (X)

License: GPL

CONs: GPL License issue.

7. [BearSSL](https://bearssl.org/) (X)

License: MIT

CONs: Current version is 0.6. It is now considered beta-quality software.

## Proposal
1. OpenSSL 3.0 - research on how to reduce the size.

It is possible that we may need add MACRO to OpenSSL 3.0 to reduce the size. We can do POC and submit to OpenSSL community.

2. MbedTls - research on how to add missing features.

3. Final Choice:
   * If 1 can success, we can replace OpenSSL 1.1 with OpenSSL 3.0.
   * Elseif 2 can success, we can replace OpenSSL 1.1 with MbedTls.
   * Else, we may use *dual-crypto module*. For example: MbedTls for PEI and OpenSSL3.0 for DXE. The source code size will become larger, more time to download the tree.

## EDK-II Check-in Criteria

We need to control the quality of OpenSSL 1.1 replacement to avoid regression.

1. Binary Size should be equivalent

  new crypto module size <= edk2-stable202211 crypto module size * (1 + 10%)

2. There should be no API change.

3. All existing Crypto usages in EDKII should be covered, including:
   * UEFI Secure Boot - [DxeImageVerificationLib](https://github.com/tianocore/edk2/tree/master/SecurityPkg/Library/DxeImageVerificationLib), [AuthVariableLib](https://github.com/tianocore/edk2/tree/master/SecurityPkg/Library/AuthVariableLib) (PKCS7)
   * TCG Trusted Boot - [Tcg2Dxe](https://github.com/tianocore/edk2/tree/master/SecurityPkg/Tcg/Tcg2Dxe), [Tcg2Pei](https://github.com/tianocore/edk2/tree/master/SecurityPkg/Tcg/Tcg2Pei) (SHA2)
   * UEFI/PI FMP Capsule - [FmpAuthenticationLibPkcs7](https://github.com/tianocore/edk2/tree/master/SecurityPkg/Library/FmpAuthenticationLibPkcs7) (PKCS7), [FmpAuthenticationLibRsa2048Sha256](https://github.com/tianocore/edk2/tree/master/SecurityPkg/Library/FmpAuthenticationLibRsa2048Sha256) (PKCS1-v1.5-RSA)
   * UEFI Crypto API - [Hash2Dxe](https://github.com/tianocore/edk2/tree/master/SecurityPkg/Hash2DxeCrypto) (SHA2), [Pkcs7Verify](https://github.com/tianocore/edk2/tree/master/SecurityPkg/Pkcs7Verify) (PKCS7)
   * Authentication - [HddPassword](https://github.com/tianocore/edk2/tree/master/SecurityPkg/HddPassword) (SHA2), [UserAuthenticationDxeSmm](https://github.com/tianocore/edk2-platforms/tree/master/Features/Intel/UserInterface/UserAuthFeaturePkg/UserAuthenticationDxeSmm) (PKCS5-PBKDF)
   * UEFI Network - [TlsDxe](https://github.com/tianocore/edk2/tree/master/NetworkPkg/TlsDxe) (TLS 1.2, TLS 1.3), [IScsiDxe](https://github.com/tianocore/edk2/tree/master/NetworkPkg/IScsiDxe) (SHA2)

4. All other Crypto APIs in EDKII besides above use case should be covered, including:
   * SHA3-ParallelHash
   * HMAC/HKDF
   * ECC

5. All Crypto [Test](https://github.com/tianocore/edk2/tree/master/CryptoPkg/Test) should pass.

