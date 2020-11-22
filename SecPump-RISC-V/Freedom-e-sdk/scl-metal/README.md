# SCL - SiFive Cryptographic Library
scl-metal provides a bridge between a high level API for the cryptographic functions and basic crypto operations, that could be either a software or a hardware implementation of cryptographic functions 

The current version implements the basic functions operations connected to the HCA (Hardware Cryptographic Accelerator). It supports the AES through the HCA-supported modes of operation, the SHA-2 and the TRNG.

For AES it supports:

- key size: 128, 192 and 256 bits
- mode ECB, CBC, CFB, OFB, CTR, CCM and GCM

For SHA-2, it supports:

- SHA224, SHA256, SHA384 and SHA512

Supports the HCA version 0.5.x
