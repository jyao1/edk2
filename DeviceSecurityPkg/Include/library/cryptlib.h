/**
    Copyright Notice:
    Copyright 2021 DMTF. All rights reserved.
    License: BSD 3-Clause License. For full text see link: https://github.com/DMTF/libspdm/blob/main/LICENSE.md
**/

/** @file
  Defines base cryptographic library APIs.
  The Base Cryptographic Library provides implementations of basic cryptography
  primitives (hash Serials, HMAC, AES, RSA, Diffie-Hellman, Elliptic Curve, etc) for UEFI security
  functionality enabling.
**/

#ifndef __SPDM_CRYPT_LIB_H__
#define __SPDM_CRYPT_LIB_H__

#include <Library/BaseCryptLib.h>

#define		sha256_get_context_size		Sha256GetContextSize
#define		sha256_init		Sha256Init
#define		sha256_duplicate		Sha256Duplicate
#define		sha256_update		Sha256Update
#define		sha256_final		Sha256Final
#define		sha256_hash_all		Sha256HashAll
#define		sha384_get_context_size		Sha384GetContextSize
#define		sha384_init		Sha384Init
#define		sha384_duplicate		Sha384Duplicate
#define		sha384_update		Sha384Update
#define		sha384_final		Sha384Final
#define		sha384_hash_all		Sha384HashAll
#define		sha512_get_context_size		Sha512GetContextSize
#define		sha512_init		Sha512Init
#define		sha512_duplicate		Sha512Duplicate
#define		sha512_update		Sha512Update
#define		sha512_final		Sha512Final
#define		sha512_hash_all		Sha512HashAll
#define		sha3_256_get_context_size		Sha3_256GetContextSize
#define		sha3_256_init		Sha3_256Init
#define		sha3_256_duplicate		Sha3_256Duplicate
#define		sha3_256_update		Sha3_256Update
#define		sha3_256_final		Sha3_256Final
#define		sha3_256_hash_all		Sha3_256HashAll
#define		sha3_384_get_context_size		Sha3_384GetContextSize
#define		sha3_384_init		Sha3_384Init
#define		sha3_384_duplicate		Sha3_384Duplicate
#define		sha3_384_update		Sha3_384Update
#define		sha3_384_final		Sha3_384Final
#define		sha3_384_hash_all		Sha3_384HashAll
#define		sha3_512_get_context_size		Sha3_512GetContextSize
#define		sha3_512_init		Sha3_512Init
#define		sha3_512_duplicate		Sha3_512Duplicate
#define		sha3_512_update		Sha3_512Update
#define		sha3_512_final		Sha3_512Final
#define		sha3_512_hash_all		Sha3_512HashAll
#define		shake256_get_context_size		Shake256GetContextSize
#define		shake256_init		Shake256Init
#define		shake256_duplicate		Shake256Duplicate
#define		shake256_update		Shake256Update
#define		shake256_final		Shake256Final
#define		shake256_hash_all		Shake256HashAll
#define		sm3_256_get_context_size		Sm3GetContextSize
#define		sm3_256_init		Sm3Init
#define		sm3_256_duplicate		Sm3Duplicate
#define		sm3_256_update		Sm3Update
#define		sm3_256_final		Sm3Final
#define		sm3_256_hash_all		Sm3HashAll
#define		hmac_sha256_new		HmacSha256New
#define		hmac_sha256_free		HmacSha256Free
#define		hmac_sha256_set_key		HmacSha256SetKey
#define		hmac_sha256_duplicate		HmacSha256Duplicate
#define		hmac_sha256_update		HmacSha256Update
#define		hmac_sha256_final		HmacSha256Final
#define		hmac_sha256_all		HmacSha256All
#define		hmac_sha384_new		HmacSha384New
#define		hmac_sha384_free		HmacSha384Free
#define		hmac_sha384_set_key		HmacSha384SetKey
#define		hmac_sha384_duplicate		HmacSha384Duplicate
#define		hmac_sha384_update		HmacSha384Update
#define		hmac_sha384_final		HmacSha384Final
#define		hmac_sha384_all		HmacSha384All
#define		hmac_sha512_new		HmacSha512New
#define		hmac_sha512_free		HmacSha512Free
#define		hmac_sha512_set_key		HmacSha512SetKey
#define		hmac_sha512_duplicate		HmacSha512Duplicate
#define		hmac_sha512_update		HmacSha512Update
#define		hmac_sha512_final		HmacSha512Final
#define		hmac_sha512_all		HmacSha512All


#define		aead_aes_gcm_encrypt		AeadAesGcmEncrypt
#define		aead_aes_gcm_decrypt		AeadAesGcmDecrypt
#define		aead_chacha20_poly1305_encrypt		AeadChaCha20Poly1305Encrypt
#define		aead_chacha20_poly1305_decrypt		AeadChaCha20Poly1305Decrypt
#define		aead_sm4_gcm_encrypt		AeadSm4GcmEncrypt
#define		aead_sm4_gcm_decrypt		AeadSm4GcmDecrypt
#define		rsa_new		RsaNew
#define		rsa_free		RsaFree
#define		rsa_set_key		RsaSetKey
#define		rsa_get_key		RsaGetKey
#define		rsa_generate_key		RsaGenerateKey
#define		rsa_check_key		RsaCheckKey
#define		rsa_pkcs1_sign		RsaPkcs1Sign
#define		rsa_pkcs1_verify		RsaPkcs1Verify
#define		rsa_pkcs1_sign_with_nid		RsaPkcs1SignWithNid
#define		rsa_pkcs1_verify_with_nid		RsaPkcs1VerifyWithNid
#define		rsa_pss_sign		RsaPssSign
#define		rsa_pss_verify		RsaPssVerify
#define		rsa_get_private_key_from_pem		RsaGetPrivateKeyFromPem
#define		rsa_get_public_key_from_x509		RsaGetPublicKeyFromX509
#define		ec_get_private_key_from_pem		EcGetPrivateKeyFromPem
#define		ec_get_public_key_from_x509		EcGetPublicKeyFromX509
#define		ed_get_private_key_from_pem		EdGetPrivateKeyFromPem
#define		ed_get_public_key_from_x509		EdGetPublicKeyFromX509
#define		sm2_get_private_key_from_pem		Sm2GetPrivateKeyFromPem
#define		sm2_get_public_key_from_x509		Sm2GetPublicKeyFromX509
#define		asn1_get_tag		Asn1GetTag
#define		x509_get_subject_name		X509GetSubjectName
#define		x509_get_common_name		X509GetCommonName
#define		x509_get_organization_name		X509GetOrganizationName
#define		x509_get_version		X509GetVersion
#define		x509_get_serial_number		X509GetSerialNumber
#define		x509_get_issuer_name		X509GetIssuerName
#define		x509_get_issuer_common_name		X509GetIssuerCommonName
#define		x509_get_issuer_organization_name		X509GetIssuerOrganizationName
#define		x509_get_signature_algorithm		X509GetSignatureAlgorithm
#define		x509_get_extension_data		X509GetExtensionData
#define		x509_get_validity		X509GetValidity
#define		x509_set_date_time		X509SetDateTime
#define		x509_compare_date_time		X509CompareDateTime
#define		x509_get_key_usage		X509GetKeyUsage
#define		x509_get_extended_key_usage		X509GetExtendedKeyUsage
#define		x509_verify_cert		X509VerifyCert
#define		x509_verify_cert_chain		X509VerifyCertChain
#define		x509_get_cert_from_cert_chain		X509GetCertFromCertChain
#define		x509_construct_certificate		X509ConstructCertificate
#define		x509_construct_certificate_stack_v		X509ConstructCertificateStackV
#define		x509_construct_certificate_stack		X509ConstructCertificateStack
#define		x509_free		X509Free
#define		x509_stack_free		X509StackFree
#define		x509_get_t_b_s_cert		X509GetTBSCert
#define		pkcs5_hash_password		Pkcs5HashPassword
#define		pkcs1v2_encrypt		Pkcs1v2Encrypt
#define		pkcs7_get_signers		Pkcs7GetSigners
#define		pkcs7_free_signers		Pkcs7FreeSigners
#define		pkcs7_get_certificates_list		Pkcs7GetCertificatesList
#define		pkcs7_sign		Pkcs7Sign
#define		pkcs7_verify		Pkcs7Verify
#define		verify_e_k_us_in_pkcs7_signature		VerifyEKUsInPkcs7Signature
#define		pkcs7_get_attached_content		Pkcs7GetAttachedContent
#define		authenticode_verify		AuthenticodeVerify
#define		image_timestamp_verify		ImageTimestampVerify
#define		dh_new		DhNew
#define		dh_new_by_nid		DhNewByNid
#define		dh_free		DhFree
#define		dh_generate_parameter		DhGenerateParameter
#define		dh_set_parameter		DhSetParameter
#define		dh_generate_key		DhGenerateKey
#define		dh_compute_key		DhComputeKey
#define		ec_new_by_nid		EcNewByNid
#define		ec_free		EcFree
#define		ec_set_pub_key		EcSetPubKey
#define		ec_get_pub_key		EcGetPubKey
#define		ec_check_key		EcCheckKey
#define		ec_generate_key		EcGenerateKey
#define		ec_compute_key		EcComputeKey
#define		ecdsa_sign		EcDsaSign
#define		ecdsa_verify		EcDsaVerify
#define		ed_new_by_nid		EdNewByNid
#define		ed_free		EdFree
#define		ed_set_pub_key		EdSetPubKey
#define		ed_get_pub_key		EdGetPubKey
#define		ed_check_key		EdCheckKey
#define		ed_generate_key		EdGenerateKey
#define		ed_dsa_sign		EdDsaSign
#define		ed_dsa_verify		EdDsaVerify
#define		sm2_new		Sm2New
#define		sm2_free		Sm2Free
#define		sm2_set_pub_key		Sm2SetPubKey
#define		sm2_get_pub_key		Sm2GetPubKey
#define		sm2_check_key		Sm2CheckKey
#define		sm2_generate_key		Sm2GenerateKey
#define		sm2_compute_key		Sm2ComputeKey
#define		sm2_sign		Sm2Sign
#define		sm2_verify		Sm2Verify
#define		random_seed		RandomSeed
#define		random_bytes		RandomBytes
#define		hkdf_sha256_extract_and_expand		HkdfSha256ExtractAndExpand
#define		hkdf_sha256_extract		HkdfSha256Extract
#define		hkdf_sha256_expand		HkdfSha256Expand
#define		hkdf_sha384_extract_and_expand		HkdfSha384ExtractAndExpand
#define		hkdf_sha384_extract		HkdfSha384Extract
#define		hkdf_sha384_expand		HkdfSha384Expand
#define		hkdf_sha512_extract_and_expand		HkdfSha512ExtractAndExpand
#define		hkdf_sha512_extract		HkdfSha512Extract
#define		hkdf_sha512_expand		HkdfSha512Expand


#endif // __SPDM_CRYPT_LIB_H__
