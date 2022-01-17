/*

Copyright (C) 2015-2018 Night Dive Studios, LLC.

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

*/
#include "fix.h"
#include "trigtab.h"

// First, the sine table.
// The sine table is indexed by the top 8 bits of a fixang.
// Its units are fix >> 2 (so -1 = 0xc000, 0 = 0x0000, 1 = 0x4000)
// This means that the high bit is almost useless, but otherwise
//    we get results like sin(PI/2) = 0.fffe rather than 1.0000.
//
// cos[x] = sin[x + 64].

uint16_t sintab[256+64+1] = { // indexed by fixang
    000000, 0x0192, 0x0324, 0x04b5, 0x0646, 0x07d6, 0x0964, 0x0af1,
    0x0c7c, 0x0e06, 0x0f8d, 0x1112, 0x1294, 0x1413, 0x1590, 0x1709,
    0x187e, 0x19ef, 0x1b5d, 0x1cc6, 0x1e2b, 0x1f8c, 0x20e7, 0x223d,
    0x238e, 0x24da, 0x2620, 0x2760, 0x289a, 0x29ce, 0x2afb, 0x2c21,
    0x2d41, 0x2e5a, 0x2f6c, 0x3076, 0x3179, 0x3274, 0x3368, 0x3453,
    0x3537, 0x3612, 0x36e5, 0x37b0, 0x3871, 0x392b, 0x39db, 0x3a82,
    0x3b21, 0x3bb6, 0x3c42, 0x3cc5, 0x3d3f, 0x3daf, 0x3e15, 0x3e72,
    0x3ec5, 0x3f0f, 0x3f4f, 0x3f85, 0x3fb1, 0x3fd4, 0x3fec, 0x3ffb,
    0x4000, 0x3ffb, 0x3fec, 0x3fd4, 0x3fb1, 0x3f85, 0x3f4f, 0x3f0f,
    0x3ec5, 0x3e72, 0x3e15, 0x3daf, 0x3d3f, 0x3cc5, 0x3c42, 0x3bb6,
    0x3b21, 0x3a82, 0x39db, 0x392b, 0x3871, 0x37b0, 0x36e5, 0x3612,
    0x3537, 0x3453, 0x3368, 0x3274, 0x3179, 0x3076, 0x2f6c, 0x2e5a,
    0x2d41, 0x2c21, 0x2afb, 0x29ce, 0x289a, 0x2760, 0x2620, 0x24da,
    0x238e, 0x223d, 0x20e7, 0x1f8c, 0x1e2b, 0x1cc6, 0x1b5d, 0x19ef,
    0x187e, 0x1709, 0x1590, 0x1413, 0x1294, 0x1112, 0x0f8d, 0x0e06,
    0x0c7c, 0x0af1, 0x0964, 0x07d6, 0x0646, 0x04b5, 0x0324, 0x0192,
    000000, 0xfe6f, 0xfcdd, 0xfb4c, 0xf9bb, 0xf82b, 0xf69d, 0xf510,
    0xf385, 0xf1fb, 0xf074, 0xeeef, 0xed6d, 0xebee, 0xea71, 0xe8f8,
    0xe783, 0xe612, 0xe4a4, 0xe33b, 0xe1d6, 0xe075, 0xdf1a, 0xddc4,
    0xdc73, 0xdb27, 0xd9e1, 0xd8a1, 0xd767, 0xd633, 0xd506, 0xd3e0,
    0xd2c0, 0xd1a7, 0xd095, 0xcf8b, 0xce88, 0xcd8d, 0xcc99, 0xcbae,
    0xcaca, 0xc9ef, 0xc91c, 0xc851, 0xc790, 0xc6d6, 0xc626, 0xc57f,
    0xc4e0, 0xc44b, 0xc3bf, 0xc33c, 0xc2c2, 0xc252, 0xc1ec, 0xc18f,
    0xc13c, 0xc0f2, 0xc0b2, 0xc07c, 0xc050, 0xc02d, 0xc015, 0xc006,
    0xc001, 0xc006, 0xc015, 0xc02d, 0xc050, 0xc07c, 0xc0b2, 0xc0f2,
    0xc13c, 0xc18f, 0xc1ec, 0xc252, 0xc2c2, 0xc33c, 0xc3bf, 0xc44b,
    0xc4e0, 0xc57f, 0xc626, 0xc6d6, 0xc790, 0xc851, 0xc91c, 0xc9ef,
    0xcaca, 0xcbae, 0xcc99, 0xcd8d, 0xce88, 0xcf8b, 0xd095, 0xd1a7,
    0xd2c0, 0xd3e0, 0xd506, 0xd633, 0xd767, 0xd8a1, 0xd9e1, 0xdb27,
    0xdc73, 0xddc4, 0xdf1a, 0xe075, 0xe1d6, 0xe33b, 0xe4a4, 0xe612,
    0xe783, 0xe8f8, 0xea71, 0xebee, 0xed6d, 0xeeef, 0xf074, 0xf1fb,
    0xf385, 0xf510, 0xf69d, 0xf82b, 0xf9bb, 0xfb4c, 0xfcdd, 0xfe6f,
    000000, 0x0192, 0x0324, 0x04b5, 0x0646, 0x07d6, 0x0964, 0x0af1,
    0x0c7c, 0x0e06, 0x0f8d, 0x1112, 0x1294, 0x1413, 0x1590, 0x1709,
    0x187e, 0x19ef, 0x1b5d, 0x1cc6, 0x1e2b, 0x1f8c, 0x20e7, 0x223d,
    0x238e, 0x24da, 0x2620, 0x2760, 0x289a, 0x29ce, 0x2afb, 0x2c21,
    0x2d41, 0x2e5a, 0x2f6c, 0x3076, 0x3179, 0x3274, 0x3368, 0x3453,
    0x3537, 0x3612, 0x36e5, 0x37b0, 0x3871, 0x392b, 0x39db, 0x3a82,
    0x3b21, 0x3bb6, 0x3c42, 0x3cc5, 0x3d3f, 0x3daf, 0x3e15, 0x3e72,
    0x3ec5, 0x3f0f, 0x3f4f, 0x3f85, 0x3fb1, 0x3fd4, 0x3fec, 0x3ffb,
    0x4000
};

// Now the arcsin table.
// The arcsin table is indexed by (((fix >> 2) + 0x4000) & 0xffff).
// That means -1 = 0xc000 + 0x4000 = 0x0000,
//                 0 = 0x0000 + 0x4000 = 0x4000,
//                 1 = 0x4000 + 0x4000 = 0x8000.
// So the high bit is almost useless, but otherwise we have problems
//    trying to differentiate between 1 and -1.
// Its units are fixangs.
//
// acos(x) = PI/2 - asin(x).    (PI/2 is fixang 0x4000)

// indexed by (high 8 bits of (fix >> 2 + 0x4000)
fixang asintab[128+1] = {
    0xc001, 0xc737, 0xca37, 0xcc87, 0xce7c, 0xd037, 0xd1ca, 0xd33d,
    0xd498, 0xd5e0, 0xd716, 0xd840, 0xd95d, 0xda6f, 0xdb78, 0xdc7a,
    0xdd73, 0xde67, 0xdf54, 0xe03c, 0xe11e, 0xe1fd, 0xe2d7, 0xe3ad,
    0xe47f, 0xe54e, 0xe61a, 0xe6e3, 0xe7aa, 0xe86e, 0xe92f, 0xe9ee,
    0xeaac, 0xeb67, 0xec20, 0xecd8, 0xed8e, 0xee42, 0xeef5, 0xefa7,
    0xf058, 0xf107, 0xf1b5, 0xf262, 0xf30e, 0xf3b9, 0xf463, 0xf50d,
    0xf5b5, 0xf65d, 0xf705, 0xf7ab, 0xf852, 0xf8f7, 0xf99d, 0xfa41,
    0xfae6, 0xfb8a, 0xfc2e, 0xfcd1, 0xfd75, 0xfe18, 0xfebb, 0xff5e,
    000000, 0x00a3, 0x0146, 0x01e9, 0x028c, 0x0330, 0x03d3, 0x0477,
    0x051b, 0x05c0, 0x0664, 0x070a, 0x07af, 0x0856, 0x08fc, 0x09a4,
    0x0a4c, 0x0af4, 0x0b9e, 0x0c48, 0x0cf3, 0x0d9f, 0x0e4c, 0x0efa,
    0x0fa9, 0x105a, 0x110c, 0x11bf, 0x1273, 0x1329, 0x13e1, 0x149a,
    0x1555, 0x1613, 0x16d2, 0x1793, 0x1857, 0x191e, 0x19e7, 0x1ab3,
    0x1b82, 0x1c54, 0x1d2a, 0x1e04, 0x1ee3, 0x1fc5, 0x20ad, 0x219a,
    0x228e, 0x2387, 0x2489, 0x2592, 0x26a4, 0x27c1, 0x28eb, 0x2a21,
    0x2b69, 0x2cc4, 0x2e37, 0x2fca, 0x3185, 0x337a, 0x35ca, 0x38ca,
    0x4000
};

// There are two exp tables.  The first is for integer exponents.
// The table only goes from -11 to 11 because that's all that will
// fit in a 16:16 fixed point number.  Add INTEGER_EXP_OFFSET to
// your exponent before looking it up in the table.

#define INTEGER_EXP_OFFSET 11

uint32_t expinttab[INTEGER_EXP_OFFSET*2+1] = {
    0x00000001, 0x00000003, 0x00000008, 0x00000016,
    0x0000003c, 0x000000a2, 0x000001ba, 0x000004b0,
    0x00000cbf, 0x000022a5, 0x00005e2d, 0x00010000,
    0x0002b7e1, 0x00076399, 0x001415e6, 0x00369920,
    0x009469c5, 0x01936dc5, 0x0448a217, 0x0ba4f53f,
    0x1fa7157c, 0x560a773e, 0xe9e22447,
};

// Now for the fractional table, which currently has 16+1 values,
// which should be interpolated between.  We can crank up the
// accuracy later if we need it.  So this input to this table goes
// from 0 to 1 by sixteenths.

uint32_t expfractab[16+1] = {
    0x00010000, 0x00011083, 0x00012216, 0x000134cc,
    0x000148b6, 0x00015de9, 0x0001747a, 0x00018c80,
    0x0001a613, 0x0001c14b, 0x0001de45, 0x0001fd1e,
    0x00021df4, 0x000240e8, 0x0002661d, 0x00028db8,
    0x0002b7e1,
};

