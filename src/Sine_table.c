// Volle Sinuswelle als Offset-Binary

#include <stdint.h>

// Achtung: wird Datentyp [uint16_t] ge�ndert, muss dies auch bei init von DAC/DMA ge�ndert werden!
const uint16_t sine_table[738] =
{		/* 0x0 */	 0x7ff,
		/* 0x1 */	 0x80f,
		/* 0x2 */	 0x81f,
		/* 0x3 */	 0x830,
		/* 0x4 */	 0x840,
		/* 0x5 */	 0x850,
		/* 0x6 */	 0x861,
		/* 0x7 */	 0x871,
		/* 0x8 */	 0x881,
		/* 0x9 */	 0x891,
		/* 0xa */	 0x8a2,
		/* 0xb */	 0x8b2,
		/* 0xc */	 0x8c2,
		/* 0xd */	 0x8d3,
		/* 0xe */	 0x8e3,
		/* 0xf */	 0x8f3,
		/* 0x10 */	 0x903,
		/* 0x11 */	 0x913,
		/* 0x12 */	 0x924,
		/* 0x13 */	 0x934,
		/* 0x14 */	 0x944,
		/* 0x15 */	 0x954,
		/* 0x16 */	 0x964,
		/* 0x17 */	 0x974,
		/* 0x18 */	 0x984,
		/* 0x19 */	 0x994,
		/* 0x1a */	 0x9a4,
		/* 0x1b */	 0x9b4,
		/* 0x1c */	 0x9c4,
		/* 0x1d */	 0x9d4,
		/* 0x1e */	 0x9e4,
		/* 0x1f */	 0x9f3,
		/* 0x20 */	 0xa03,
		/* 0x21 */	 0xa13,
		/* 0x22 */	 0xa23,
		/* 0x23 */	 0xa32,
		/* 0x24 */	 0xa42,
		/* 0x25 */	 0xa51,
		/* 0x26 */	 0xa61,
		/* 0x27 */	 0xa70,
		/* 0x28 */	 0xa80,
		/* 0x29 */	 0xa8f,
		/* 0x2a */	 0xa9f,
		/* 0x2b */	 0xaae,
		/* 0x2c */	 0xabd,
		/* 0x2d */	 0xacc,
		/* 0x2e */	 0xadb,
		/* 0x2f */	 0xaea,
		/* 0x30 */	 0xaf9,
		/* 0x31 */	 0xb08,
		/* 0x32 */	 0xb17,
		/* 0x33 */	 0xb26,
		/* 0x34 */	 0xb35,
		/* 0x35 */	 0xb44,
		/* 0x36 */	 0xb52,
		/* 0x37 */	 0xb61,
		/* 0x38 */	 0xb70,
		/* 0x39 */	 0xb7e,
		/* 0x3a */	 0xb8d,
		/* 0x3b */	 0xb9b,
		/* 0x3c */	 0xba9,
		/* 0x3d */	 0xbb7,
		/* 0x3e */	 0xbc6,
		/* 0x3f */	 0xbd4,
		/* 0x40 */	 0xbe2,
		/* 0x41 */	 0xbf0,
		/* 0x42 */	 0xbfd,
		/* 0x43 */	 0xc0b,
		/* 0x44 */	 0xc19,
		/* 0x45 */	 0xc27,
		/* 0x46 */	 0xc34,
		/* 0x47 */	 0xc42,
		/* 0x48 */	 0xc4f,
		/* 0x49 */	 0xc5c,
		/* 0x4a */	 0xc6a,
		/* 0x4b */	 0xc77,
		/* 0x4c */	 0xc84,
		/* 0x4d */	 0xc91,
		/* 0x4e */	 0xc9e,
		/* 0x4f */	 0xcab,
		/* 0x50 */	 0xcb7,
		/* 0x51 */	 0xcc4,
		/* 0x52 */	 0xcd1,
		/* 0x53 */	 0xcdd,
		/* 0x54 */	 0xcea,
		/* 0x55 */	 0xcf6,
		/* 0x56 */	 0xd02,
		/* 0x57 */	 0xd0e,
		/* 0x58 */	 0xd1a,
		/* 0x59 */	 0xd26,
		/* 0x5a */	 0xd32,
		/* 0x5b */	 0xd3e,
		/* 0x5c */	 0xd49,
		/* 0x5d */	 0xd55,
		/* 0x5e */	 0xd60,
		/* 0x5f */	 0xd6c,
		/* 0x60 */	 0xd77,
		/* 0x61 */	 0xd82,
		/* 0x62 */	 0xd8d,
		/* 0x63 */	 0xd98,
		/* 0x64 */	 0xda3,
		/* 0x65 */	 0xdad,
		/* 0x66 */	 0xdb8,
		/* 0x67 */	 0xdc3,
		/* 0x68 */	 0xdcd,
		/* 0x69 */	 0xdd7,
		/* 0x6a */	 0xde1,
		/* 0x6b */	 0xdeb,
		/* 0x6c */	 0xdf5,
		/* 0x6d */	 0xdff,
		/* 0x6e */	 0xe09,
		/* 0x6f */	 0xe13,
		/* 0x70 */	 0xe1c,
		/* 0x71 */	 0xe26,
		/* 0x72 */	 0xe2f,
		/* 0x73 */	 0xe38,
		/* 0x74 */	 0xe41,
		/* 0x75 */	 0xe4a,
		/* 0x76 */	 0xe53,
		/* 0x77 */	 0xe5c,
		/* 0x78 */	 0xe64,
		/* 0x79 */	 0xe6d,
		/* 0x7a */	 0xe75,
		/* 0x7b */	 0xe7d,
		/* 0x7c */	 0xe85,
		/* 0x7d */	 0xe8d,
		/* 0x7e */	 0xe95,
		/* 0x7f */	 0xe9d,
		/* 0x80 */	 0xea5,
		/* 0x81 */	 0xeac,
		/* 0x82 */	 0xeb3,
		/* 0x83 */	 0xebb,
		/* 0x84 */	 0xec2,
		/* 0x85 */	 0xec9,
		/* 0x86 */	 0xed0,
		/* 0x87 */	 0xed7,
		/* 0x88 */	 0xedd,
		/* 0x89 */	 0xee4,
		/* 0x8a */	 0xeea,
		/* 0x8b */	 0xef0,
		/* 0x8c */	 0xef6,
		/* 0x8d */	 0xefc,
		/* 0x8e */	 0xf02,
		/* 0x8f */	 0xf08,
		/* 0x90 */	 0xf0d,
		/* 0x91 */	 0xf13,
		/* 0x92 */	 0xf18,
		/* 0x93 */	 0xf1d,
		/* 0x94 */	 0xf23,
		/* 0x95 */	 0xf27,
		/* 0x96 */	 0xf2c,
		/* 0x97 */	 0xf31,
		/* 0x98 */	 0xf35,
		/* 0x99 */	 0xf3a,
		/* 0x9a */	 0xf3e,
		/* 0x9b */	 0xf42,
		/* 0x9c */	 0xf46,
		/* 0x9d */	 0xf4a,
		/* 0x9e */	 0xf4e,
		/* 0x9f */	 0xf51,
		/* 0xa0 */	 0xf55,
		/* 0xa1 */	 0xf58,
		/* 0xa2 */	 0xf5b,
		/* 0xa3 */	 0xf5e,
		/* 0xa4 */	 0xf61,
		/* 0xa5 */	 0xf64,
		/* 0xa6 */	 0xf67,
		/* 0xa7 */	 0xf69,
		/* 0xa8 */	 0xf6c,
		/* 0xa9 */	 0xf6e,
		/* 0xaa */	 0xf70,
		/* 0xab */	 0xf72,
		/* 0xac */	 0xf74,
		/* 0xad */	 0xf75,
		/* 0xae */	 0xf77,
		/* 0xaf */	 0xf78,
		/* 0xb0 */	 0xf79,
		/* 0xb1 */	 0xf7b,
		/* 0xb2 */	 0xf7c,
		/* 0xb3 */	 0xf7c,
		/* 0xb4 */	 0xf7d,
		/* 0xb5 */	 0xf7e,
		/* 0xb6 */	 0xf7e,
		/* 0xb7 */	 0xf7e,
		/* 0xb8 */	 0xf7e,
		/* 0xb9 */	 0xf7e,
		/* 0xba */	 0xf7e,
		/* 0xbb */	 0xf7e,
		/* 0xbc */	 0xf7e,
		/* 0xbd */	 0xf7d,
		/* 0xbe */	 0xf7c,
		/* 0xbf */	 0xf7c,
		/* 0xc0 */	 0xf7b,
		/* 0xc1 */	 0xf79,
		/* 0xc2 */	 0xf78,
		/* 0xc3 */	 0xf77,
		/* 0xc4 */	 0xf75,
		/* 0xc5 */	 0xf74,
		/* 0xc6 */	 0xf72,
		/* 0xc7 */	 0xf70,
		/* 0xc8 */	 0xf6e,
		/* 0xc9 */	 0xf6c,
		/* 0xca */	 0xf69,
		/* 0xcb */	 0xf67,
		/* 0xcc */	 0xf64,
		/* 0xcd */	 0xf61,
		/* 0xce */	 0xf5e,
		/* 0xcf */	 0xf5b,
		/* 0xd0 */	 0xf58,
		/* 0xd1 */	 0xf55,
		/* 0xd2 */	 0xf51,
		/* 0xd3 */	 0xf4e,
		/* 0xd4 */	 0xf4a,
		/* 0xd5 */	 0xf46,
		/* 0xd6 */	 0xf42,
		/* 0xd7 */	 0xf3e,
		/* 0xd8 */	 0xf3a,
		/* 0xd9 */	 0xf35,
		/* 0xda */	 0xf31,
		/* 0xdb */	 0xf2c,
		/* 0xdc */	 0xf27,
		/* 0xdd */	 0xf23,
		/* 0xde */	 0xf1d,
		/* 0xdf */	 0xf18,
		/* 0xe0 */	 0xf13,
		/* 0xe1 */	 0xf0d,
		/* 0xe2 */	 0xf08,
		/* 0xe3 */	 0xf02,
		/* 0xe4 */	 0xefc,
		/* 0xe5 */	 0xef6,
		/* 0xe6 */	 0xef0,
		/* 0xe7 */	 0xeea,
		/* 0xe8 */	 0xee4,
		/* 0xe9 */	 0xedd,
		/* 0xea */	 0xed7,
		/* 0xeb */	 0xed0,
		/* 0xec */	 0xec9,
		/* 0xed */	 0xec2,
		/* 0xee */	 0xebb,
		/* 0xef */	 0xeb3,
		/* 0xf0 */	 0xeac,
		/* 0xf1 */	 0xea5,
		/* 0xf2 */	 0xe9d,
		/* 0xf3 */	 0xe95,
		/* 0xf4 */	 0xe8d,
		/* 0xf5 */	 0xe85,
		/* 0xf6 */	 0xe7d,
		/* 0xf7 */	 0xe75,
		/* 0xf8 */	 0xe6d,
		/* 0xf9 */	 0xe64,
		/* 0xfa */	 0xe5c,
		/* 0xfb */	 0xe53,
		/* 0xfc */	 0xe4a,
		/* 0xfd */	 0xe41,
		/* 0xfe */	 0xe38,
		/* 0xff */	 0xe2f,
		/* 0x100 */	 0xe26,
		/* 0x101 */	 0xe1c,
		/* 0x102 */	 0xe13,
		/* 0x103 */	 0xe09,
		/* 0x104 */	 0xdff,
		/* 0x105 */	 0xdf5,
		/* 0x106 */	 0xdeb,
		/* 0x107 */	 0xde1,
		/* 0x108 */	 0xdd7,
		/* 0x109 */	 0xdcd,
		/* 0x10a */	 0xdc3,
		/* 0x10b */	 0xdb8,
		/* 0x10c */	 0xdad,
		/* 0x10d */	 0xda3,
		/* 0x10e */	 0xd98,
		/* 0x10f */	 0xd8d,
		/* 0x110 */	 0xd82,
		/* 0x111 */	 0xd77,
		/* 0x112 */	 0xd6c,
		/* 0x113 */	 0xd60,
		/* 0x114 */	 0xd55,
		/* 0x115 */	 0xd49,
		/* 0x116 */	 0xd3e,
		/* 0x117 */	 0xd32,
		/* 0x118 */	 0xd26,
		/* 0x119 */	 0xd1a,
		/* 0x11a */	 0xd0e,
		/* 0x11b */	 0xd02,
		/* 0x11c */	 0xcf6,
		/* 0x11d */	 0xcea,
		/* 0x11e */	 0xcdd,
		/* 0x11f */	 0xcd1,
		/* 0x120 */	 0xcc4,
		/* 0x121 */	 0xcb7,
		/* 0x122 */	 0xcab,
		/* 0x123 */	 0xc9e,
		/* 0x124 */	 0xc91,
		/* 0x125 */	 0xc84,
		/* 0x126 */	 0xc77,
		/* 0x127 */	 0xc6a,
		/* 0x128 */	 0xc5c,
		/* 0x129 */	 0xc4f,
		/* 0x12a */	 0xc42,
		/* 0x12b */	 0xc34,
		/* 0x12c */	 0xc27,
		/* 0x12d */	 0xc19,
		/* 0x12e */	 0xc0b,
		/* 0x12f */	 0xbfd,
		/* 0x130 */	 0xbf0,
		/* 0x131 */	 0xbe2,
		/* 0x132 */	 0xbd4,
		/* 0x133 */	 0xbc6,
		/* 0x134 */	 0xbb7,
		/* 0x135 */	 0xba9,
		/* 0x136 */	 0xb9b,
		/* 0x137 */	 0xb8d,
		/* 0x138 */	 0xb7e,
		/* 0x139 */	 0xb70,
		/* 0x13a */	 0xb61,
		/* 0x13b */	 0xb52,
		/* 0x13c */	 0xb44,
		/* 0x13d */	 0xb35,
		/* 0x13e */	 0xb26,
		/* 0x13f */	 0xb17,
		/* 0x140 */	 0xb08,
		/* 0x141 */	 0xaf9,
		/* 0x142 */	 0xaea,
		/* 0x143 */	 0xadb,
		/* 0x144 */	 0xacc,
		/* 0x145 */	 0xabd,
		/* 0x146 */	 0xaae,
		/* 0x147 */	 0xa9f,
		/* 0x148 */	 0xa8f,
		/* 0x149 */	 0xa80,
		/* 0x14a */	 0xa70,
		/* 0x14b */	 0xa61,
		/* 0x14c */	 0xa51,
		/* 0x14d */	 0xa42,
		/* 0x14e */	 0xa32,
		/* 0x14f */	 0xa23,
		/* 0x150 */	 0xa13,
		/* 0x151 */	 0xa03,
		/* 0x152 */	 0x9f3,
		/* 0x153 */	 0x9e4,
		/* 0x154 */	 0x9d4,
		/* 0x155 */	 0x9c4,
		/* 0x156 */	 0x9b4,
		/* 0x157 */	 0x9a4,
		/* 0x158 */	 0x994,
		/* 0x159 */	 0x984,
		/* 0x15a */	 0x974,
		/* 0x15b */	 0x964,
		/* 0x15c */	 0x954,
		/* 0x15d */	 0x944,
		/* 0x15e */	 0x934,
		/* 0x15f */	 0x924,
		/* 0x160 */	 0x913,
		/* 0x161 */	 0x903,
		/* 0x162 */	 0x8f3,
		/* 0x163 */	 0x8e3,
		/* 0x164 */	 0x8d3,
		/* 0x165 */	 0x8c2,
		/* 0x166 */	 0x8b2,
		/* 0x167 */	 0x8a2,
		/* 0x168 */	 0x891,
		/* 0x169 */	 0x881,
		/* 0x16a */	 0x871,
		/* 0x16b */	 0x861,
		/* 0x16c */	 0x850,
		/* 0x16d */	 0x840,
		/* 0x16e */	 0x830,
		/* 0x16f */	 0x81f,
		/* 0x170 */	 0x80f,
		/* 0x171 */	 0x7ff,
		/* 0x172 */	 0x7ee,
		/* 0x173 */	 0x7de,
		/* 0x174 */	 0x7cd,
		/* 0x175 */	 0x7bd,
		/* 0x176 */	 0x7ad,
		/* 0x177 */	 0x79c,
		/* 0x178 */	 0x78c,
		/* 0x179 */	 0x77c,
		/* 0x17a */	 0x76c,
		/* 0x17b */	 0x75b,
		/* 0x17c */	 0x74b,
		/* 0x17d */	 0x73b,
		/* 0x17e */	 0x72a,
		/* 0x17f */	 0x71a,
		/* 0x180 */	 0x70a,
		/* 0x181 */	 0x6fa,
		/* 0x182 */	 0x6ea,
		/* 0x183 */	 0x6d9,
		/* 0x184 */	 0x6c9,
		/* 0x185 */	 0x6b9,
		/* 0x186 */	 0x6a9,
		/* 0x187 */	 0x699,
		/* 0x188 */	 0x689,
		/* 0x189 */	 0x679,
		/* 0x18a */	 0x669,
		/* 0x18b */	 0x659,
		/* 0x18c */	 0x649,
		/* 0x18d */	 0x639,
		/* 0x18e */	 0x629,
		/* 0x18f */	 0x619,
		/* 0x190 */	 0x60a,
		/* 0x191 */	 0x5fa,
		/* 0x192 */	 0x5ea,
		/* 0x193 */	 0x5da,
		/* 0x194 */	 0x5cb,
		/* 0x195 */	 0x5bb,
		/* 0x196 */	 0x5ac,
		/* 0x197 */	 0x59c,
		/* 0x198 */	 0x58d,
		/* 0x199 */	 0x57d,
		/* 0x19a */	 0x56e,
		/* 0x19b */	 0x55e,
		/* 0x19c */	 0x54f,
		/* 0x19d */	 0x540,
		/* 0x19e */	 0x531,
		/* 0x19f */	 0x522,
		/* 0x1a0 */	 0x513,
		/* 0x1a1 */	 0x504,
		/* 0x1a2 */	 0x4f5,
		/* 0x1a3 */	 0x4e6,
		/* 0x1a4 */	 0x4d7,
		/* 0x1a5 */	 0x4c8,
		/* 0x1a6 */	 0x4b9,
		/* 0x1a7 */	 0x4ab,
		/* 0x1a8 */	 0x49c,
		/* 0x1a9 */	 0x48d,
		/* 0x1aa */	 0x47f,
		/* 0x1ab */	 0x470,
		/* 0x1ac */	 0x462,
		/* 0x1ad */	 0x454,
		/* 0x1ae */	 0x446,
		/* 0x1af */	 0x437,
		/* 0x1b0 */	 0x429,
		/* 0x1b1 */	 0x41b,
		/* 0x1b2 */	 0x40d,
		/* 0x1b3 */	 0x400,
		/* 0x1b4 */	 0x3f2,
		/* 0x1b5 */	 0x3e4,
		/* 0x1b6 */	 0x3d6,
		/* 0x1b7 */	 0x3c9,
		/* 0x1b8 */	 0x3bb,
		/* 0x1b9 */	 0x3ae,
		/* 0x1ba */	 0x3a1,
		/* 0x1bb */	 0x393,
		/* 0x1bc */	 0x386,
		/* 0x1bd */	 0x379,
		/* 0x1be */	 0x36c,
		/* 0x1bf */	 0x35f,
		/* 0x1c0 */	 0x352,
		/* 0x1c1 */	 0x346,
		/* 0x1c2 */	 0x339,
		/* 0x1c3 */	 0x32c,
		/* 0x1c4 */	 0x320,
		/* 0x1c5 */	 0x313,
		/* 0x1c6 */	 0x307,
		/* 0x1c7 */	 0x2fb,
		/* 0x1c8 */	 0x2ef,
		/* 0x1c9 */	 0x2e3,
		/* 0x1ca */	 0x2d7,
		/* 0x1cb */	 0x2cb,
		/* 0x1cc */	 0x2bf,
		/* 0x1cd */	 0x2b4,
		/* 0x1ce */	 0x2a8,
		/* 0x1cf */	 0x29d,
		/* 0x1d0 */	 0x291,
		/* 0x1d1 */	 0x286,
		/* 0x1d2 */	 0x27b,
		/* 0x1d3 */	 0x270,
		/* 0x1d4 */	 0x265,
		/* 0x1d5 */	 0x25a,
		/* 0x1d6 */	 0x250,
		/* 0x1d7 */	 0x245,
		/* 0x1d8 */	 0x23a,
		/* 0x1d9 */	 0x230,
		/* 0x1da */	 0x226,
		/* 0x1db */	 0x21c,
		/* 0x1dc */	 0x212,
		/* 0x1dd */	 0x208,
		/* 0x1de */	 0x1fe,
		/* 0x1df */	 0x1f4,
		/* 0x1e0 */	 0x1ea,
		/* 0x1e1 */	 0x1e1,
		/* 0x1e2 */	 0x1d7,
		/* 0x1e3 */	 0x1ce,
		/* 0x1e4 */	 0x1c5,
		/* 0x1e5 */	 0x1bc,
		/* 0x1e6 */	 0x1b3,
		/* 0x1e7 */	 0x1aa,
		/* 0x1e8 */	 0x1a1,
		/* 0x1e9 */	 0x199,
		/* 0x1ea */	 0x190,
		/* 0x1eb */	 0x188,
		/* 0x1ec */	 0x180,
		/* 0x1ed */	 0x178,
		/* 0x1ee */	 0x170,
		/* 0x1ef */	 0x168,
		/* 0x1f0 */	 0x160,
		/* 0x1f1 */	 0x158,
		/* 0x1f2 */	 0x151,
		/* 0x1f3 */	 0x14a,
		/* 0x1f4 */	 0x142,
		/* 0x1f5 */	 0x13b,
		/* 0x1f6 */	 0x134,
		/* 0x1f7 */	 0x12d,
		/* 0x1f8 */	 0x126,
		/* 0x1f9 */	 0x120,
		/* 0x1fa */	 0x119,
		/* 0x1fb */	 0x113,
		/* 0x1fc */	 0x10d,
		/* 0x1fd */	 0x107,
		/* 0x1fe */	 0x101,
		/* 0x1ff */	 0xfb,
		/* 0x200 */	 0xf5,
		/* 0x201 */	 0xf0,
		/* 0x202 */	 0xea,
		/* 0x203 */	 0xe5,
		/* 0x204 */	 0xe0,
		/* 0x205 */	 0xda,
		/* 0x206 */	 0xd6,
		/* 0x207 */	 0xd1,
		/* 0x208 */	 0xcc,
		/* 0x209 */	 0xc8,
		/* 0x20a */	 0xc3,
		/* 0x20b */	 0xbf,
		/* 0x20c */	 0xbb,
		/* 0x20d */	 0xb7,
		/* 0x20e */	 0xb3,
		/* 0x20f */	 0xaf,
		/* 0x210 */	 0xac,
		/* 0x211 */	 0xa8,
		/* 0x212 */	 0xa5,
		/* 0x213 */	 0xa2,
		/* 0x214 */	 0x9f,
		/* 0x215 */	 0x9c,
		/* 0x216 */	 0x99,
		/* 0x217 */	 0x96,
		/* 0x218 */	 0x94,
		/* 0x219 */	 0x91,
		/* 0x21a */	 0x8f,
		/* 0x21b */	 0x8d,
		/* 0x21c */	 0x8b,
		/* 0x21d */	 0x89,
		/* 0x21e */	 0x88,
		/* 0x21f */	 0x86,
		/* 0x220 */	 0x85,
		/* 0x221 */	 0x84,
		/* 0x222 */	 0x82,
		/* 0x223 */	 0x81,
		/* 0x224 */	 0x81,
		/* 0x225 */	 0x80,
		/* 0x226 */	 0x7f,
		/* 0x227 */	 0x7f,
		/* 0x228 */	 0x7f,
		/* 0x229 */	 0x7f,
		/* 0x22a */	 0x7f,
		/* 0x22b */	 0x7f,
		/* 0x22c */	 0x7f,
		/* 0x22d */	 0x7f,
		/* 0x22e */	 0x80,
		/* 0x22f */	 0x81,
		/* 0x230 */	 0x81,
		/* 0x231 */	 0x82,
		/* 0x232 */	 0x84,
		/* 0x233 */	 0x85,
		/* 0x234 */	 0x86,
		/* 0x235 */	 0x88,
		/* 0x236 */	 0x89,
		/* 0x237 */	 0x8b,
		/* 0x238 */	 0x8d,
		/* 0x239 */	 0x8f,
		/* 0x23a */	 0x91,
		/* 0x23b */	 0x94,
		/* 0x23c */	 0x96,
		/* 0x23d */	 0x99,
		/* 0x23e */	 0x9c,
		/* 0x23f */	 0x9f,
		/* 0x240 */	 0xa2,
		/* 0x241 */	 0xa5,
		/* 0x242 */	 0xa8,
		/* 0x243 */	 0xac,
		/* 0x244 */	 0xaf,
		/* 0x245 */	 0xb3,
		/* 0x246 */	 0xb7,
		/* 0x247 */	 0xbb,
		/* 0x248 */	 0xbf,
		/* 0x249 */	 0xc3,
		/* 0x24a */	 0xc8,
		/* 0x24b */	 0xcc,
		/* 0x24c */	 0xd1,
		/* 0x24d */	 0xd6,
		/* 0x24e */	 0xda,
		/* 0x24f */	 0xe0,
		/* 0x250 */	 0xe5,
		/* 0x251 */	 0xea,
		/* 0x252 */	 0xf0,
		/* 0x253 */	 0xf5,
		/* 0x254 */	 0xfb,
		/* 0x255 */	 0x101,
		/* 0x256 */	 0x107,
		/* 0x257 */	 0x10d,
		/* 0x258 */	 0x113,
		/* 0x259 */	 0x119,
		/* 0x25a */	 0x120,
		/* 0x25b */	 0x126,
		/* 0x25c */	 0x12d,
		/* 0x25d */	 0x134,
		/* 0x25e */	 0x13b,
		/* 0x25f */	 0x142,
		/* 0x260 */	 0x14a,
		/* 0x261 */	 0x151,
		/* 0x262 */	 0x158,
		/* 0x263 */	 0x160,
		/* 0x264 */	 0x168,
		/* 0x265 */	 0x170,
		/* 0x266 */	 0x178,
		/* 0x267 */	 0x180,
		/* 0x268 */	 0x188,
		/* 0x269 */	 0x190,
		/* 0x26a */	 0x199,
		/* 0x26b */	 0x1a1,
		/* 0x26c */	 0x1aa,
		/* 0x26d */	 0x1b3,
		/* 0x26e */	 0x1bc,
		/* 0x26f */	 0x1c5,
		/* 0x270 */	 0x1ce,
		/* 0x271 */	 0x1d7,
		/* 0x272 */	 0x1e1,
		/* 0x273 */	 0x1ea,
		/* 0x274 */	 0x1f4,
		/* 0x275 */	 0x1fe,
		/* 0x276 */	 0x208,
		/* 0x277 */	 0x212,
		/* 0x278 */	 0x21c,
		/* 0x279 */	 0x226,
		/* 0x27a */	 0x230,
		/* 0x27b */	 0x23a,
		/* 0x27c */	 0x245,
		/* 0x27d */	 0x250,
		/* 0x27e */	 0x25a,
		/* 0x27f */	 0x265,
		/* 0x280 */	 0x270,
		/* 0x281 */	 0x27b,
		/* 0x282 */	 0x286,
		/* 0x283 */	 0x291,
		/* 0x284 */	 0x29d,
		/* 0x285 */	 0x2a8,
		/* 0x286 */	 0x2b4,
		/* 0x287 */	 0x2bf,
		/* 0x288 */	 0x2cb,
		/* 0x289 */	 0x2d7,
		/* 0x28a */	 0x2e3,
		/* 0x28b */	 0x2ef,
		/* 0x28c */	 0x2fb,
		/* 0x28d */	 0x307,
		/* 0x28e */	 0x313,
		/* 0x28f */	 0x320,
		/* 0x290 */	 0x32c,
		/* 0x291 */	 0x339,
		/* 0x292 */	 0x346,
		/* 0x293 */	 0x352,
		/* 0x294 */	 0x35f,
		/* 0x295 */	 0x36c,
		/* 0x296 */	 0x379,
		/* 0x297 */	 0x386,
		/* 0x298 */	 0x393,
		/* 0x299 */	 0x3a1,
		/* 0x29a */	 0x3ae,
		/* 0x29b */	 0x3bb,
		/* 0x29c */	 0x3c9,
		/* 0x29d */	 0x3d6,
		/* 0x29e */	 0x3e4,
		/* 0x29f */	 0x3f2,
		/* 0x2a0 */	 0x400,
		/* 0x2a1 */	 0x40d,
		/* 0x2a2 */	 0x41b,
		/* 0x2a3 */	 0x429,
		/* 0x2a4 */	 0x437,
		/* 0x2a5 */	 0x446,
		/* 0x2a6 */	 0x454,
		/* 0x2a7 */	 0x462,
		/* 0x2a8 */	 0x470,
		/* 0x2a9 */	 0x47f,
		/* 0x2aa */	 0x48d,
		/* 0x2ab */	 0x49c,
		/* 0x2ac */	 0x4ab,
		/* 0x2ad */	 0x4b9,
		/* 0x2ae */	 0x4c8,
		/* 0x2af */	 0x4d7,
		/* 0x2b0 */	 0x4e6,
		/* 0x2b1 */	 0x4f5,
		/* 0x2b2 */	 0x504,
		/* 0x2b3 */	 0x513,
		/* 0x2b4 */	 0x522,
		/* 0x2b5 */	 0x531,
		/* 0x2b6 */	 0x540,
		/* 0x2b7 */	 0x54f,
		/* 0x2b8 */	 0x55e,
		/* 0x2b9 */	 0x56e,
		/* 0x2ba */	 0x57d,
		/* 0x2bb */	 0x58d,
		/* 0x2bc */	 0x59c,
		/* 0x2bd */	 0x5ac,
		/* 0x2be */	 0x5bb,
		/* 0x2bf */	 0x5cb,
		/* 0x2c0 */	 0x5da,
		/* 0x2c1 */	 0x5ea,
		/* 0x2c2 */	 0x5fa,
		/* 0x2c3 */	 0x60a,
		/* 0x2c4 */	 0x619,
		/* 0x2c5 */	 0x629,
		/* 0x2c6 */	 0x639,
		/* 0x2c7 */	 0x649,
		/* 0x2c8 */	 0x659,
		/* 0x2c9 */	 0x669,
		/* 0x2ca */	 0x679,
		/* 0x2cb */	 0x689,
		/* 0x2cc */	 0x699,
		/* 0x2cd */	 0x6a9,
		/* 0x2ce */	 0x6b9,
		/* 0x2cf */	 0x6c9,
		/* 0x2d0 */	 0x6d9,
		/* 0x2d1 */	 0x6ea,
		/* 0x2d2 */	 0x6fa,
		/* 0x2d3 */	 0x70a,
		/* 0x2d4 */	 0x71a,
		/* 0x2d5 */	 0x72a,
		/* 0x2d6 */	 0x73b,
		/* 0x2d7 */	 0x74b,
		/* 0x2d8 */	 0x75b,
		/* 0x2d9 */	 0x76c,
		/* 0x2da */	 0x77c,
		/* 0x2db */	 0x78c,
		/* 0x2dc */	 0x79c,
		/* 0x2dd */	 0x7ad,
		/* 0x2de */	 0x7bd,
		/* 0x2df */	 0x7cd,
		/* 0x2e0 */	 0x7de,
		/* 0x2e1 */	 0x7ee
};