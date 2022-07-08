#ifndef GAS_TABLE_H
#define GAS_TABLE_H

#include "../config.h"

#include <inttypes.h>

#define NULL 0

#ifdef MAINNET
#ifdef ARROW_GLACIER
// arrow_glacier base gas table
static const uint64_t arrow_glacier_gt[256] = {
    /* 0x00 */ 0,  // STOP
    /* 0x01 */ 3,  // ADD
    /* 0x02 */ 5,  // MUL
    /* 0x03 */ 3,  // SUB
    /* 0x04 */ 5,  // DIV
    /* 0x05 */ 5,  // SDIV
    /* 0x06 */ 5,  // MOD
    /* 0x07 */ 5,  // SMOD
    /* 0x08 */ 8,  // ADDMOD
    /* 0x09 */ 8,  // MULMOD
    /* 0x0a */ 10, // EXP
    /* 0x0b */ 5,  // SIGNEXTEND
    /* 0x0c */ NULL,
    /* 0x0d */ NULL,
    /* 0x0e */ NULL,
    /* 0x0f */ NULL,
    /* 0x10 */ 3, // LT
    /* 0x11 */ 3, // GT
    /* 0x12 */ 3, // SLT
    /* 0x13 */ 3, // SGT
    /* 0x14 */ 3, // EQ
    /* 0x15 */ 3, // ISZERO
    /* 0x16 */ 3, // AND
    /* 0x17 */ 3, // OR
    /* 0x18 */ 3, // XOR
    /* 0x19 */ 3, // NOT
    /* 0x1a */ 3, // BYTE
    /* 0x1b */ 3, // SHL
    /* 0x1c */ 3, // SHR
    /* 0x1d */ 3, // SAR
    /* 0x1e */ NULL,
    /* 0x1f */ NULL,
    /* 0x20 */ 30, // SHA3
    /* 0x21 */ NULL,
    /* 0x22 */ NULL,
    /* 0x23 */ NULL,
    /* 0x24 */ NULL,
    /* 0x25 */ NULL,
    /* 0x26 */ NULL,
    /* 0x27 */ NULL,
    /* 0x28 */ NULL,
    /* 0x29 */ NULL,
    /* 0x2a */ NULL,
    /* 0x2b */ NULL,
    /* 0x2c */ NULL,
    /* 0x2d */ NULL,
    /* 0x2e */ NULL,
    /* 0x2f */ NULL,
    /* 0x30 */ 2,   // ADDRESS
    /* 0x31 */ 100, // BALANCE
    /* 0x32 */ 2,   // ORIGIN
    /* 0x33 */ 2,   // CALLER
    /* 0x34 */ 2,   // CALLVALUE
    /* 0x35 */ 3,   // CALLDATALOAD
    /* 0x36 */ 2,   // CALLDATASIZE
    /* 0x37 */ 3,   // CALLDATACOPY
    /* 0x38 */ 2,   // CODESIZE
    /* 0x39 */ 3,   // CODECOPY
    /* 0x3a */ 2,   // GASPRICE
    /* 0x3b */ 100, // EXTCODESIZE
    /* 0x3c */ 100, // EXTCODECOPY
    /* 0x3d */ 2,   // RETURNDATASIZE
    /* 0x3e */ 3,   // RETURNDATACOPY
    /* 0x3f */ 100, // EXTCODEHASH
    /* 0x40 */ 20,  // BLOCKHASH
    /* 0x41 */ 2,   // COINBASE
    /* 0x42 */ 2,   // TIMESTAMP
    /* 0x43 */ 2,   // NUMBER
    /* 0x44 */ 2,   // DIFFICULTY
    /* 0x45 */ 2,   // GASLIMIT
    /* 0x46 */ 2,   // CHAINID
    /* 0x47 */ 5,   // SELFBALANCE
    /* 0x48 */ 2,   // BASEFEE
    /* 0x49 */ NULL,
    /* 0x4a */ NULL,
    /* 0x4b */ NULL,
    /* 0x4c */ NULL,
    /* 0x4d */ NULL,
    /* 0x4e */ NULL,
    /* 0x4f */ NULL,
    /* 0x50 */ 2,   // POP
    /* 0x51 */ 3,   // MLOAD
    /* 0x52 */ 3,   // MSTORE
    /* 0x53 */ 3,   // MSTORE8
    /* 0x54 */ 100, // SLOAD
    /* 0x55 */ 100, // SSTORE
    /* 0x56 */ 8,   // JUMP
    /* 0x57 */ 10,  // JUMPI
    /* 0x58 */ 2,   // PC
    /* 0x59 */ 2,   // MSIZE
    /* 0x5a */ 2,   // GAS
    /* 0x5b */ 1,   // JUMPDEST
    /* 0x5c */ NULL,
    /* 0x5d */ NULL,
    /* 0x5e */ NULL,
    /* 0x5f */ NULL,
    /* 0x60 */ 3,    // "PUSH1"
    /* 0x61 */ 3,    // "PUSH2"
    /* 0x62 */ 3,    // "PUSH3"
    /* 0x63 */ 3,    // "PUSH4"
    /* 0x64 */ 3,    // "PUSH5"
    /* 0x65 */ 3,    // "PUSH6"
    /* 0x66 */ 3,    // "PUSH7"
    /* 0x67 */ 3,    // "PUSH8"
    /* 0x68 */ 3,    // "PUSH9"
    /* 0x69 */ 3,    // "PUSH10"
    /* 0x6a */ 3,    // "PUSH11"
    /* 0x6b */ 3,    // "PUSH12"
    /* 0x6c */ 3,    // "PUSH13"
    /* 0x6d */ 3,    // "PUSH14"
    /* 0x6e */ 3,    // "PUSH15"
    /* 0x6f */ 3,    // "PUSH16"
    /* 0x70 */ 3,    // "PUSH17"
    /* 0x71 */ 3,    // "PUSH18"
    /* 0x72 */ 3,    // "PUSH19"
    /* 0x73 */ 3,    // "PUSH20"
    /* 0x74 */ 3,    // "PUSH21"
    /* 0x75 */ 3,    // "PUSH22"
    /* 0x76 */ 3,    // "PUSH23"
    /* 0x77 */ 3,    // "PUSH24"
    /* 0x78 */ 3,    // "PUSH25"
    /* 0x79 */ 3,    // "PUSH26"
    /* 0x7a */ 3,    // "PUSH27"
    /* 0x7b */ 3,    // "PUSH28"
    /* 0x7c */ 3,    // "PUSH29"
    /* 0x7d */ 3,    // "PUSH30"
    /* 0x7e */ 3,    // "PUSH31"
    /* 0x7f */ 3,    // "PUSH32"
    /* 0x80 */ 3,    // "DUP1"
    /* 0x81 */ 3,    // "DUP2"
    /* 0x82 */ 3,    // "DUP3"
    /* 0x83 */ 3,    // "DUP4"
    /* 0x84 */ 3,    // "DUP5"
    /* 0x85 */ 3,    // "DUP6"
    /* 0x86 */ 3,    // "DUP7"
    /* 0x87 */ 3,    // "DUP8"
    /* 0x88 */ 3,    // "DUP9"
    /* 0x89 */ 3,    // "DUP10"
    /* 0x8a */ 3,    // "DUP11"
    /* 0x8b */ 3,    // "DUP12"
    /* 0x8c */ 3,    // "DUP13"
    /* 0x8d */ 3,    // "DUP14"
    /* 0x8e */ 3,    // "DUP15"
    /* 0x8f */ 3,    // "DUP16"
    /* 0x90 */ 3,    // "SWAP1"
    /* 0x91 */ 3,    // "SWAP2"
    /* 0x92 */ 3,    // "SWAP3"
    /* 0x93 */ 3,    // "SWAP4"
    /* 0x94 */ 3,    // "SWAP5"
    /* 0x95 */ 3,    // "SWAP6"
    /* 0x96 */ 3,    // "SWAP7"
    /* 0x97 */ 3,    // "SWAP8"
    /* 0x98 */ 3,    // "SWAP9"
    /* 0x99 */ 3,    // "SWAP10"
    /* 0x9a */ 3,    // "SWAP11"
    /* 0x9b */ 3,    // "SWAP12"
    /* 0x9c */ 3,    // "SWAP13"
    /* 0x9d */ 3,    // "SWAP14"
    /* 0x9e */ 3,    // "SWAP15"
    /* 0x9f */ 3,    // "SWAP16"
    /* 0xa0 */ 375,  // "LOG0"
    /* 0xa1 */ 750,  // "LOG1"
    /* 0xa2 */ 1125, // "LOG2"
    /* 0xa3 */ 1500, // "LOG3"
    /* 0xa4 */ 1875, // "LOG4"
    /* 0xa5 */ NULL,
    /* 0xa6 */ NULL,
    /* 0xa7 */ NULL,
    /* 0xa8 */ NULL,
    /* 0xa9 */ NULL,
    /* 0xaa */ NULL,
    /* 0xab */ NULL,
    /* 0xac */ NULL,
    /* 0xad */ NULL,
    /* 0xae */ NULL,
    /* 0xaf */ NULL,
    /* 0xb0 */ NULL,
    /* 0xb1 */ NULL,
    /* 0xb2 */ NULL,
    /* 0xb3 */ NULL,
    /* 0xb4 */ NULL,
    /* 0xb5 */ NULL,
    /* 0xb6 */ NULL,
    /* 0xb7 */ NULL,
    /* 0xb8 */ NULL,
    /* 0xb9 */ NULL,
    /* 0xba */ NULL,
    /* 0xbb */ NULL,
    /* 0xbc */ NULL,
    /* 0xbd */ NULL,
    /* 0xbe */ NULL,
    /* 0xbf */ NULL,
    /* 0xc0 */ NULL,
    /* 0xc1 */ NULL,
    /* 0xc2 */ NULL,
    /* 0xc3 */ NULL,
    /* 0xc4 */ NULL,
    /* 0xc5 */ NULL,
    /* 0xc6 */ NULL,
    /* 0xc7 */ NULL,
    /* 0xc8 */ NULL,
    /* 0xc9 */ NULL,
    /* 0xca */ NULL,
    /* 0xcb */ NULL,
    /* 0xcc */ NULL,
    /* 0xcd */ NULL,
    /* 0xce */ NULL,
    /* 0xcf */ NULL,
    /* 0xd0 */ NULL,
    /* 0xd1 */ NULL,
    /* 0xd2 */ NULL,
    /* 0xd3 */ NULL,
    /* 0xd4 */ NULL,
    /* 0xd5 */ NULL,
    /* 0xd6 */ NULL,
    /* 0xd7 */ NULL,
    /* 0xd8 */ NULL,
    /* 0xd9 */ NULL,
    /* 0xda */ NULL,
    /* 0xdb */ NULL,
    /* 0xdc */ NULL,
    /* 0xdd */ NULL,
    /* 0xde */ NULL,
    /* 0xdf */ NULL,
    /* 0xe0 */ NULL,
    /* 0xe1 */ NULL,
    /* 0xe2 */ NULL,
    /* 0xe3 */ NULL,
    /* 0xe4 */ NULL,
    /* 0xe5 */ NULL,
    /* 0xe6 */ NULL,
    /* 0xe7 */ NULL,
    /* 0xe8 */ NULL,
    /* 0xe9 */ NULL,
    /* 0xea */ NULL,
    /* 0xeb */ NULL,
    /* 0xec */ NULL,
    /* 0xed */ NULL,
    /* 0xee */ NULL,
    /* 0xef */ NULL,
    /* 0xf0 */ 32000, // "CREATE"
    /* 0xf1 */ 100,   // "CALL"
    /* 0xf2 */ 100,   // "CALLCODE"
    /* 0xf3 */ 0,     // "RETURN"
    /* 0xf4 */ 100,   // "DELEGATECALL"
    /* 0xf5 */ 32000, // "CREATE2"
    /* 0xf6 */ NULL,
    /* 0xf7 */ NULL,
    /* 0xf8 */ NULL,
    /* 0xf9 */ NULL,
    /* 0xfa */ 100, // "STATICCALL"
    /* 0xfb */ 0,   // "REVERT"
    /* 0xfc */ NULL,
    /* 0xfd */ NULL,
    /* 0xfe */ 0,    // "INVALID"
    /* 0xff */ 5000, // "SELFDESTRUCT"
};

#define GAS_TABLE arrow_glacier_gt

#endif
#endif

#endif