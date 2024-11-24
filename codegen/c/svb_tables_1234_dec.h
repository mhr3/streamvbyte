#ifndef STREAMVBYTE_SHUFFLE_TABLES_H
#define STREAMVBYTE_SHUFFLE_TABLES_H
#include <stdint.h>

// using 1,2,3,4 bytes per value
static uint8_t lengthTable[256] ={
  4,  5,  6,  7,  5,  6,  7,  8,  6,  7,  8,  9,  7,  8,  9, 10,
  5,  6,  7,  8,  6,  7,  8,  9,  7,  8,  9, 10,  8,  9, 10, 11,
  6,  7,  8,  9,  7,  8,  9, 10,  8,  9, 10, 11,  9, 10, 11, 12,
  7,  8,  9, 10,  8,  9, 10, 11,  9, 10, 11, 12, 10, 11, 12, 13,
  5,  6,  7,  8,  6,  7,  8,  9,  7,  8,  9, 10,  8,  9, 10, 11,
  6,  7,  8,  9,  7,  8,  9, 10,  8,  9, 10, 11,  9, 10, 11, 12,
  7,  8,  9, 10,  8,  9, 10, 11,  9, 10, 11, 12, 10, 11, 12, 13,
  8,  9, 10, 11,  9, 10, 11, 12, 10, 11, 12, 13, 11, 12, 13, 14,
  6,  7,  8,  9,  7,  8,  9, 10,  8,  9, 10, 11,  9, 10, 11, 12,
  7,  8,  9, 10,  8,  9, 10, 11,  9, 10, 11, 12, 10, 11, 12, 13,
  8,  9, 10, 11,  9, 10, 11, 12, 10, 11, 12, 13, 11, 12, 13, 14,
  9, 10, 11, 12, 10, 11, 12, 13, 11, 12, 13, 14, 12, 13, 14, 15,
  7,  8,  9, 10,  8,  9, 10, 11,  9, 10, 11, 12, 10, 11, 12, 13,
  8,  9, 10, 11,  9, 10, 11, 12, 10, 11, 12, 13, 11, 12, 13, 14,
  9, 10, 11, 12, 10, 11, 12, 13, 11, 12, 13, 14, 12, 13, 14, 15,
 10, 11, 12, 13, 11, 12, 13, 14, 12, 13, 14, 15, 13, 14, 15, 16,
 };

// decoding:
static int8_t shuffleTable[256][16] = {
 {  0, -1, -1, -1,  1, -1, -1, -1,  2, -1, -1, -1,  3, -1, -1, -1 },    // 0000
 {  0,  1, -1, -1,  2, -1, -1, -1,  3, -1, -1, -1,  4, -1, -1, -1 },    // 1000
 {  0,  1,  2, -1,  3, -1, -1, -1,  4, -1, -1, -1,  5, -1, -1, -1 },    // 2000
 {  0,  1,  2,  3,  4, -1, -1, -1,  5, -1, -1, -1,  6, -1, -1, -1 },    // 3000
 {  0, -1, -1, -1,  1,  2, -1, -1,  3, -1, -1, -1,  4, -1, -1, -1 },    // 0100
 {  0,  1, -1, -1,  2,  3, -1, -1,  4, -1, -1, -1,  5, -1, -1, -1 },    // 1100
 {  0,  1,  2, -1,  3,  4, -1, -1,  5, -1, -1, -1,  6, -1, -1, -1 },    // 2100
 {  0,  1,  2,  3,  4,  5, -1, -1,  6, -1, -1, -1,  7, -1, -1, -1 },    // 3100
 {  0, -1, -1, -1,  1,  2,  3, -1,  4, -1, -1, -1,  5, -1, -1, -1 },    // 0200
 {  0,  1, -1, -1,  2,  3,  4, -1,  5, -1, -1, -1,  6, -1, -1, -1 },    // 1200
 {  0,  1,  2, -1,  3,  4,  5, -1,  6, -1, -1, -1,  7, -1, -1, -1 },    // 2200
 {  0,  1,  2,  3,  4,  5,  6, -1,  7, -1, -1, -1,  8, -1, -1, -1 },    // 3200
 {  0, -1, -1, -1,  1,  2,  3,  4,  5, -1, -1, -1,  6, -1, -1, -1 },    // 0300
 {  0,  1, -1, -1,  2,  3,  4,  5,  6, -1, -1, -1,  7, -1, -1, -1 },    // 1300
 {  0,  1,  2, -1,  3,  4,  5,  6,  7, -1, -1, -1,  8, -1, -1, -1 },    // 2300
 {  0,  1,  2,  3,  4,  5,  6,  7,  8, -1, -1, -1,  9, -1, -1, -1 },    // 3300
 {  0, -1, -1, -1,  1, -1, -1, -1,  2,  3, -1, -1,  4, -1, -1, -1 },    // 0010
 {  0,  1, -1, -1,  2, -1, -1, -1,  3,  4, -1, -1,  5, -1, -1, -1 },    // 1010
 {  0,  1,  2, -1,  3, -1, -1, -1,  4,  5, -1, -1,  6, -1, -1, -1 },    // 2010
 {  0,  1,  2,  3,  4, -1, -1, -1,  5,  6, -1, -1,  7, -1, -1, -1 },    // 3010
 {  0, -1, -1, -1,  1,  2, -1, -1,  3,  4, -1, -1,  5, -1, -1, -1 },    // 0110
 {  0,  1, -1, -1,  2,  3, -1, -1,  4,  5, -1, -1,  6, -1, -1, -1 },    // 1110
 {  0,  1,  2, -1,  3,  4, -1, -1,  5,  6, -1, -1,  7, -1, -1, -1 },    // 2110
 {  0,  1,  2,  3,  4,  5, -1, -1,  6,  7, -1, -1,  8, -1, -1, -1 },    // 3110
 {  0, -1, -1, -1,  1,  2,  3, -1,  4,  5, -1, -1,  6, -1, -1, -1 },    // 0210
 {  0,  1, -1, -1,  2,  3,  4, -1,  5,  6, -1, -1,  7, -1, -1, -1 },    // 1210
 {  0,  1,  2, -1,  3,  4,  5, -1,  6,  7, -1, -1,  8, -1, -1, -1 },    // 2210
 {  0,  1,  2,  3,  4,  5,  6, -1,  7,  8, -1, -1,  9, -1, -1, -1 },    // 3210
 {  0, -1, -1, -1,  1,  2,  3,  4,  5,  6, -1, -1,  7, -1, -1, -1 },    // 0310
 {  0,  1, -1, -1,  2,  3,  4,  5,  6,  7, -1, -1,  8, -1, -1, -1 },    // 1310
 {  0,  1,  2, -1,  3,  4,  5,  6,  7,  8, -1, -1,  9, -1, -1, -1 },    // 2310
 {  0,  1,  2,  3,  4,  5,  6,  7,  8,  9, -1, -1, 10, -1, -1, -1 },    // 3310
 {  0, -1, -1, -1,  1, -1, -1, -1,  2,  3,  4, -1,  5, -1, -1, -1 },    // 0020
 {  0,  1, -1, -1,  2, -1, -1, -1,  3,  4,  5, -1,  6, -1, -1, -1 },    // 1020
 {  0,  1,  2, -1,  3, -1, -1, -1,  4,  5,  6, -1,  7, -1, -1, -1 },    // 2020
 {  0,  1,  2,  3,  4, -1, -1, -1,  5,  6,  7, -1,  8, -1, -1, -1 },    // 3020
 {  0, -1, -1, -1,  1,  2, -1, -1,  3,  4,  5, -1,  6, -1, -1, -1 },    // 0120
 {  0,  1, -1, -1,  2,  3, -1, -1,  4,  5,  6, -1,  7, -1, -1, -1 },    // 1120
 {  0,  1,  2, -1,  3,  4, -1, -1,  5,  6,  7, -1,  8, -1, -1, -1 },    // 2120
 {  0,  1,  2,  3,  4,  5, -1, -1,  6,  7,  8, -1,  9, -1, -1, -1 },    // 3120
 {  0, -1, -1, -1,  1,  2,  3, -1,  4,  5,  6, -1,  7, -1, -1, -1 },    // 0220
 {  0,  1, -1, -1,  2,  3,  4, -1,  5,  6,  7, -1,  8, -1, -1, -1 },    // 1220
 {  0,  1,  2, -1,  3,  4,  5, -1,  6,  7,  8, -1,  9, -1, -1, -1 },    // 2220
 {  0,  1,  2,  3,  4,  5,  6, -1,  7,  8,  9, -1, 10, -1, -1, -1 },    // 3220
 {  0, -1, -1, -1,  1,  2,  3,  4,  5,  6,  7, -1,  8, -1, -1, -1 },    // 0320
 {  0,  1, -1, -1,  2,  3,  4,  5,  6,  7,  8, -1,  9, -1, -1, -1 },    // 1320
 {  0,  1,  2, -1,  3,  4,  5,  6,  7,  8,  9, -1, 10, -1, -1, -1 },    // 2320
 {  0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, -1, 11, -1, -1, -1 },    // 3320
 {  0, -1, -1, -1,  1, -1, -1, -1,  2,  3,  4,  5,  6, -1, -1, -1 },    // 0030
 {  0,  1, -1, -1,  2, -1, -1, -1,  3,  4,  5,  6,  7, -1, -1, -1 },    // 1030
 {  0,  1,  2, -1,  3, -1, -1, -1,  4,  5,  6,  7,  8, -1, -1, -1 },    // 2030
 {  0,  1,  2,  3,  4, -1, -1, -1,  5,  6,  7,  8,  9, -1, -1, -1 },    // 3030
 {  0, -1, -1, -1,  1,  2, -1, -1,  3,  4,  5,  6,  7, -1, -1, -1 },    // 0130
 {  0,  1, -1, -1,  2,  3, -1, -1,  4,  5,  6,  7,  8, -1, -1, -1 },    // 1130
 {  0,  1,  2, -1,  3,  4, -1, -1,  5,  6,  7,  8,  9, -1, -1, -1 },    // 2130
 {  0,  1,  2,  3,  4,  5, -1, -1,  6,  7,  8,  9, 10, -1, -1, -1 },    // 3130
 {  0, -1, -1, -1,  1,  2,  3, -1,  4,  5,  6,  7,  8, -1, -1, -1 },    // 0230
 {  0,  1, -1, -1,  2,  3,  4, -1,  5,  6,  7,  8,  9, -1, -1, -1 },    // 1230
 {  0,  1,  2, -1,  3,  4,  5, -1,  6,  7,  8,  9, 10, -1, -1, -1 },    // 2230
 {  0,  1,  2,  3,  4,  5,  6, -1,  7,  8,  9, 10, 11, -1, -1, -1 },    // 3230
 {  0, -1, -1, -1,  1,  2,  3,  4,  5,  6,  7,  8,  9, -1, -1, -1 },    // 0330
 {  0,  1, -1, -1,  2,  3,  4,  5,  6,  7,  8,  9, 10, -1, -1, -1 },    // 1330
 {  0,  1,  2, -1,  3,  4,  5,  6,  7,  8,  9, 10, 11, -1, -1, -1 },    // 2330
 {  0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, -1, -1, -1 },    // 3330
 {  0, -1, -1, -1,  1, -1, -1, -1,  2, -1, -1, -1,  3,  4, -1, -1 },    // 0001
 {  0,  1, -1, -1,  2, -1, -1, -1,  3, -1, -1, -1,  4,  5, -1, -1 },    // 1001
 {  0,  1,  2, -1,  3, -1, -1, -1,  4, -1, -1, -1,  5,  6, -1, -1 },    // 2001
 {  0,  1,  2,  3,  4, -1, -1, -1,  5, -1, -1, -1,  6,  7, -1, -1 },    // 3001
 {  0, -1, -1, -1,  1,  2, -1, -1,  3, -1, -1, -1,  4,  5, -1, -1 },    // 0101
 {  0,  1, -1, -1,  2,  3, -1, -1,  4, -1, -1, -1,  5,  6, -1, -1 },    // 1101
 {  0,  1,  2, -1,  3,  4, -1, -1,  5, -1, -1, -1,  6,  7, -1, -1 },    // 2101
 {  0,  1,  2,  3,  4,  5, -1, -1,  6, -1, -1, -1,  7,  8, -1, -1 },    // 3101
 {  0, -1, -1, -1,  1,  2,  3, -1,  4, -1, -1, -1,  5,  6, -1, -1 },    // 0201
 {  0,  1, -1, -1,  2,  3,  4, -1,  5, -1, -1, -1,  6,  7, -1, -1 },    // 1201
 {  0,  1,  2, -1,  3,  4,  5, -1,  6, -1, -1, -1,  7,  8, -1, -1 },    // 2201
 {  0,  1,  2,  3,  4,  5,  6, -1,  7, -1, -1, -1,  8,  9, -1, -1 },    // 3201
 {  0, -1, -1, -1,  1,  2,  3,  4,  5, -1, -1, -1,  6,  7, -1, -1 },    // 0301
 {  0,  1, -1, -1,  2,  3,  4,  5,  6, -1, -1, -1,  7,  8, -1, -1 },    // 1301
 {  0,  1,  2, -1,  3,  4,  5,  6,  7, -1, -1, -1,  8,  9, -1, -1 },    // 2301
 {  0,  1,  2,  3,  4,  5,  6,  7,  8, -1, -1, -1,  9, 10, -1, -1 },    // 3301
 {  0, -1, -1, -1,  1, -1, -1, -1,  2,  3, -1, -1,  4,  5, -1, -1 },    // 0011
 {  0,  1, -1, -1,  2, -1, -1, -1,  3,  4, -1, -1,  5,  6, -1, -1 },    // 1011
 {  0,  1,  2, -1,  3, -1, -1, -1,  4,  5, -1, -1,  6,  7, -1, -1 },    // 2011
 {  0,  1,  2,  3,  4, -1, -1, -1,  5,  6, -1, -1,  7,  8, -1, -1 },    // 3011
 {  0, -1, -1, -1,  1,  2, -1, -1,  3,  4, -1, -1,  5,  6, -1, -1 },    // 0111
 {  0,  1, -1, -1,  2,  3, -1, -1,  4,  5, -1, -1,  6,  7, -1, -1 },    // 1111
 {  0,  1,  2, -1,  3,  4, -1, -1,  5,  6, -1, -1,  7,  8, -1, -1 },    // 2111
 {  0,  1,  2,  3,  4,  5, -1, -1,  6,  7, -1, -1,  8,  9, -1, -1 },    // 3111
 {  0, -1, -1, -1,  1,  2,  3, -1,  4,  5, -1, -1,  6,  7, -1, -1 },    // 0211
 {  0,  1, -1, -1,  2,  3,  4, -1,  5,  6, -1, -1,  7,  8, -1, -1 },    // 1211
 {  0,  1,  2, -1,  3,  4,  5, -1,  6,  7, -1, -1,  8,  9, -1, -1 },    // 2211
 {  0,  1,  2,  3,  4,  5,  6, -1,  7,  8, -1, -1,  9, 10, -1, -1 },    // 3211
 {  0, -1, -1, -1,  1,  2,  3,  4,  5,  6, -1, -1,  7,  8, -1, -1 },    // 0311
 {  0,  1, -1, -1,  2,  3,  4,  5,  6,  7, -1, -1,  8,  9, -1, -1 },    // 1311
 {  0,  1,  2, -1,  3,  4,  5,  6,  7,  8, -1, -1,  9, 10, -1, -1 },    // 2311
 {  0,  1,  2,  3,  4,  5,  6,  7,  8,  9, -1, -1, 10, 11, -1, -1 },    // 3311
 {  0, -1, -1, -1,  1, -1, -1, -1,  2,  3,  4, -1,  5,  6, -1, -1 },    // 0021
 {  0,  1, -1, -1,  2, -1, -1, -1,  3,  4,  5, -1,  6,  7, -1, -1 },    // 1021
 {  0,  1,  2, -1,  3, -1, -1, -1,  4,  5,  6, -1,  7,  8, -1, -1 },    // 2021
 {  0,  1,  2,  3,  4, -1, -1, -1,  5,  6,  7, -1,  8,  9, -1, -1 },    // 3021
 {  0, -1, -1, -1,  1,  2, -1, -1,  3,  4,  5, -1,  6,  7, -1, -1 },    // 0121
 {  0,  1, -1, -1,  2,  3, -1, -1,  4,  5,  6, -1,  7,  8, -1, -1 },    // 1121
 {  0,  1,  2, -1,  3,  4, -1, -1,  5,  6,  7, -1,  8,  9, -1, -1 },    // 2121
 {  0,  1,  2,  3,  4,  5, -1, -1,  6,  7,  8, -1,  9, 10, -1, -1 },    // 3121
 {  0, -1, -1, -1,  1,  2,  3, -1,  4,  5,  6, -1,  7,  8, -1, -1 },    // 0221
 {  0,  1, -1, -1,  2,  3,  4, -1,  5,  6,  7, -1,  8,  9, -1, -1 },    // 1221
 {  0,  1,  2, -1,  3,  4,  5, -1,  6,  7,  8, -1,  9, 10, -1, -1 },    // 2221
 {  0,  1,  2,  3,  4,  5,  6, -1,  7,  8,  9, -1, 10, 11, -1, -1 },    // 3221
 {  0, -1, -1, -1,  1,  2,  3,  4,  5,  6,  7, -1,  8,  9, -1, -1 },    // 0321
 {  0,  1, -1, -1,  2,  3,  4,  5,  6,  7,  8, -1,  9, 10, -1, -1 },    // 1321
 {  0,  1,  2, -1,  3,  4,  5,  6,  7,  8,  9, -1, 10, 11, -1, -1 },    // 2321
 {  0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, -1, 11, 12, -1, -1 },    // 3321
 {  0, -1, -1, -1,  1, -1, -1, -1,  2,  3,  4,  5,  6,  7, -1, -1 },    // 0031
 {  0,  1, -1, -1,  2, -1, -1, -1,  3,  4,  5,  6,  7,  8, -1, -1 },    // 1031
 {  0,  1,  2, -1,  3, -1, -1, -1,  4,  5,  6,  7,  8,  9, -1, -1 },    // 2031
 {  0,  1,  2,  3,  4, -1, -1, -1,  5,  6,  7,  8,  9, 10, -1, -1 },    // 3031
 {  0, -1, -1, -1,  1,  2, -1, -1,  3,  4,  5,  6,  7,  8, -1, -1 },    // 0131
 {  0,  1, -1, -1,  2,  3, -1, -1,  4,  5,  6,  7,  8,  9, -1, -1 },    // 1131
 {  0,  1,  2, -1,  3,  4, -1, -1,  5,  6,  7,  8,  9, 10, -1, -1 },    // 2131
 {  0,  1,  2,  3,  4,  5, -1, -1,  6,  7,  8,  9, 10, 11, -1, -1 },    // 3131
 {  0, -1, -1, -1,  1,  2,  3, -1,  4,  5,  6,  7,  8,  9, -1, -1 },    // 0231
 {  0,  1, -1, -1,  2,  3,  4, -1,  5,  6,  7,  8,  9, 10, -1, -1 },    // 1231
 {  0,  1,  2, -1,  3,  4,  5, -1,  6,  7,  8,  9, 10, 11, -1, -1 },    // 2231
 {  0,  1,  2,  3,  4,  5,  6, -1,  7,  8,  9, 10, 11, 12, -1, -1 },    // 3231
 {  0, -1, -1, -1,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, -1, -1 },    // 0331
 {  0,  1, -1, -1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, -1, -1 },    // 1331
 {  0,  1,  2, -1,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, -1, -1 },    // 2331
 {  0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, -1, -1 },    // 3331
 {  0, -1, -1, -1,  1, -1, -1, -1,  2, -1, -1, -1,  3,  4,  5, -1 },    // 0002
 {  0,  1, -1, -1,  2, -1, -1, -1,  3, -1, -1, -1,  4,  5,  6, -1 },    // 1002
 {  0,  1,  2, -1,  3, -1, -1, -1,  4, -1, -1, -1,  5,  6,  7, -1 },    // 2002
 {  0,  1,  2,  3,  4, -1, -1, -1,  5, -1, -1, -1,  6,  7,  8, -1 },    // 3002
 {  0, -1, -1, -1,  1,  2, -1, -1,  3, -1, -1, -1,  4,  5,  6, -1 },    // 0102
 {  0,  1, -1, -1,  2,  3, -1, -1,  4, -1, -1, -1,  5,  6,  7, -1 },    // 1102
 {  0,  1,  2, -1,  3,  4, -1, -1,  5, -1, -1, -1,  6,  7,  8, -1 },    // 2102
 {  0,  1,  2,  3,  4,  5, -1, -1,  6, -1, -1, -1,  7,  8,  9, -1 },    // 3102
 {  0, -1, -1, -1,  1,  2,  3, -1,  4, -1, -1, -1,  5,  6,  7, -1 },    // 0202
 {  0,  1, -1, -1,  2,  3,  4, -1,  5, -1, -1, -1,  6,  7,  8, -1 },    // 1202
 {  0,  1,  2, -1,  3,  4,  5, -1,  6, -1, -1, -1,  7,  8,  9, -1 },    // 2202
 {  0,  1,  2,  3,  4,  5,  6, -1,  7, -1, -1, -1,  8,  9, 10, -1 },    // 3202
 {  0, -1, -1, -1,  1,  2,  3,  4,  5, -1, -1, -1,  6,  7,  8, -1 },    // 0302
 {  0,  1, -1, -1,  2,  3,  4,  5,  6, -1, -1, -1,  7,  8,  9, -1 },    // 1302
 {  0,  1,  2, -1,  3,  4,  5,  6,  7, -1, -1, -1,  8,  9, 10, -1 },    // 2302
 {  0,  1,  2,  3,  4,  5,  6,  7,  8, -1, -1, -1,  9, 10, 11, -1 },    // 3302
 {  0, -1, -1, -1,  1, -1, -1, -1,  2,  3, -1, -1,  4,  5,  6, -1 },    // 0012
 {  0,  1, -1, -1,  2, -1, -1, -1,  3,  4, -1, -1,  5,  6,  7, -1 },    // 1012
 {  0,  1,  2, -1,  3, -1, -1, -1,  4,  5, -1, -1,  6,  7,  8, -1 },    // 2012
 {  0,  1,  2,  3,  4, -1, -1, -1,  5,  6, -1, -1,  7,  8,  9, -1 },    // 3012
 {  0, -1, -1, -1,  1,  2, -1, -1,  3,  4, -1, -1,  5,  6,  7, -1 },    // 0112
 {  0,  1, -1, -1,  2,  3, -1, -1,  4,  5, -1, -1,  6,  7,  8, -1 },    // 1112
 {  0,  1,  2, -1,  3,  4, -1, -1,  5,  6, -1, -1,  7,  8,  9, -1 },    // 2112
 {  0,  1,  2,  3,  4,  5, -1, -1,  6,  7, -1, -1,  8,  9, 10, -1 },    // 3112
 {  0, -1, -1, -1,  1,  2,  3, -1,  4,  5, -1, -1,  6,  7,  8, -1 },    // 0212
 {  0,  1, -1, -1,  2,  3,  4, -1,  5,  6, -1, -1,  7,  8,  9, -1 },    // 1212
 {  0,  1,  2, -1,  3,  4,  5, -1,  6,  7, -1, -1,  8,  9, 10, -1 },    // 2212
 {  0,  1,  2,  3,  4,  5,  6, -1,  7,  8, -1, -1,  9, 10, 11, -1 },    // 3212
 {  0, -1, -1, -1,  1,  2,  3,  4,  5,  6, -1, -1,  7,  8,  9, -1 },    // 0312
 {  0,  1, -1, -1,  2,  3,  4,  5,  6,  7, -1, -1,  8,  9, 10, -1 },    // 1312
 {  0,  1,  2, -1,  3,  4,  5,  6,  7,  8, -1, -1,  9, 10, 11, -1 },    // 2312
 {  0,  1,  2,  3,  4,  5,  6,  7,  8,  9, -1, -1, 10, 11, 12, -1 },    // 3312
 {  0, -1, -1, -1,  1, -1, -1, -1,  2,  3,  4, -1,  5,  6,  7, -1 },    // 0022
 {  0,  1, -1, -1,  2, -1, -1, -1,  3,  4,  5, -1,  6,  7,  8, -1 },    // 1022
 {  0,  1,  2, -1,  3, -1, -1, -1,  4,  5,  6, -1,  7,  8,  9, -1 },    // 2022
 {  0,  1,  2,  3,  4, -1, -1, -1,  5,  6,  7, -1,  8,  9, 10, -1 },    // 3022
 {  0, -1, -1, -1,  1,  2, -1, -1,  3,  4,  5, -1,  6,  7,  8, -1 },    // 0122
 {  0,  1, -1, -1,  2,  3, -1, -1,  4,  5,  6, -1,  7,  8,  9, -1 },    // 1122
 {  0,  1,  2, -1,  3,  4, -1, -1,  5,  6,  7, -1,  8,  9, 10, -1 },    // 2122
 {  0,  1,  2,  3,  4,  5, -1, -1,  6,  7,  8, -1,  9, 10, 11, -1 },    // 3122
 {  0, -1, -1, -1,  1,  2,  3, -1,  4,  5,  6, -1,  7,  8,  9, -1 },    // 0222
 {  0,  1, -1, -1,  2,  3,  4, -1,  5,  6,  7, -1,  8,  9, 10, -1 },    // 1222
 {  0,  1,  2, -1,  3,  4,  5, -1,  6,  7,  8, -1,  9, 10, 11, -1 },    // 2222
 {  0,  1,  2,  3,  4,  5,  6, -1,  7,  8,  9, -1, 10, 11, 12, -1 },    // 3222
 {  0, -1, -1, -1,  1,  2,  3,  4,  5,  6,  7, -1,  8,  9, 10, -1 },    // 0322
 {  0,  1, -1, -1,  2,  3,  4,  5,  6,  7,  8, -1,  9, 10, 11, -1 },    // 1322
 {  0,  1,  2, -1,  3,  4,  5,  6,  7,  8,  9, -1, 10, 11, 12, -1 },    // 2322
 {  0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, -1, 11, 12, 13, -1 },    // 3322
 {  0, -1, -1, -1,  1, -1, -1, -1,  2,  3,  4,  5,  6,  7,  8, -1 },    // 0032
 {  0,  1, -1, -1,  2, -1, -1, -1,  3,  4,  5,  6,  7,  8,  9, -1 },    // 1032
 {  0,  1,  2, -1,  3, -1, -1, -1,  4,  5,  6,  7,  8,  9, 10, -1 },    // 2032
 {  0,  1,  2,  3,  4, -1, -1, -1,  5,  6,  7,  8,  9, 10, 11, -1 },    // 3032
 {  0, -1, -1, -1,  1,  2, -1, -1,  3,  4,  5,  6,  7,  8,  9, -1 },    // 0132
 {  0,  1, -1, -1,  2,  3, -1, -1,  4,  5,  6,  7,  8,  9, 10, -1 },    // 1132
 {  0,  1,  2, -1,  3,  4, -1, -1,  5,  6,  7,  8,  9, 10, 11, -1 },    // 2132
 {  0,  1,  2,  3,  4,  5, -1, -1,  6,  7,  8,  9, 10, 11, 12, -1 },    // 3132
 {  0, -1, -1, -1,  1,  2,  3, -1,  4,  5,  6,  7,  8,  9, 10, -1 },    // 0232
 {  0,  1, -1, -1,  2,  3,  4, -1,  5,  6,  7,  8,  9, 10, 11, -1 },    // 1232
 {  0,  1,  2, -1,  3,  4,  5, -1,  6,  7,  8,  9, 10, 11, 12, -1 },    // 2232
 {  0,  1,  2,  3,  4,  5,  6, -1,  7,  8,  9, 10, 11, 12, 13, -1 },    // 3232
 {  0, -1, -1, -1,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, -1 },    // 0332
 {  0,  1, -1, -1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, -1 },    // 1332
 {  0,  1,  2, -1,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, -1 },    // 2332
 {  0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, -1 },    // 3332
 {  0, -1, -1, -1,  1, -1, -1, -1,  2, -1, -1, -1,  3,  4,  5,  6 },    // 0003
 {  0,  1, -1, -1,  2, -1, -1, -1,  3, -1, -1, -1,  4,  5,  6,  7 },    // 1003
 {  0,  1,  2, -1,  3, -1, -1, -1,  4, -1, -1, -1,  5,  6,  7,  8 },    // 2003
 {  0,  1,  2,  3,  4, -1, -1, -1,  5, -1, -1, -1,  6,  7,  8,  9 },    // 3003
 {  0, -1, -1, -1,  1,  2, -1, -1,  3, -1, -1, -1,  4,  5,  6,  7 },    // 0103
 {  0,  1, -1, -1,  2,  3, -1, -1,  4, -1, -1, -1,  5,  6,  7,  8 },    // 1103
 {  0,  1,  2, -1,  3,  4, -1, -1,  5, -1, -1, -1,  6,  7,  8,  9 },    // 2103
 {  0,  1,  2,  3,  4,  5, -1, -1,  6, -1, -1, -1,  7,  8,  9, 10 },    // 3103
 {  0, -1, -1, -1,  1,  2,  3, -1,  4, -1, -1, -1,  5,  6,  7,  8 },    // 0203
 {  0,  1, -1, -1,  2,  3,  4, -1,  5, -1, -1, -1,  6,  7,  8,  9 },    // 1203
 {  0,  1,  2, -1,  3,  4,  5, -1,  6, -1, -1, -1,  7,  8,  9, 10 },    // 2203
 {  0,  1,  2,  3,  4,  5,  6, -1,  7, -1, -1, -1,  8,  9, 10, 11 },    // 3203
 {  0, -1, -1, -1,  1,  2,  3,  4,  5, -1, -1, -1,  6,  7,  8,  9 },    // 0303
 {  0,  1, -1, -1,  2,  3,  4,  5,  6, -1, -1, -1,  7,  8,  9, 10 },    // 1303
 {  0,  1,  2, -1,  3,  4,  5,  6,  7, -1, -1, -1,  8,  9, 10, 11 },    // 2303
 {  0,  1,  2,  3,  4,  5,  6,  7,  8, -1, -1, -1,  9, 10, 11, 12 },    // 3303
 {  0, -1, -1, -1,  1, -1, -1, -1,  2,  3, -1, -1,  4,  5,  6,  7 },    // 0013
 {  0,  1, -1, -1,  2, -1, -1, -1,  3,  4, -1, -1,  5,  6,  7,  8 },    // 1013
 {  0,  1,  2, -1,  3, -1, -1, -1,  4,  5, -1, -1,  6,  7,  8,  9 },    // 2013
 {  0,  1,  2,  3,  4, -1, -1, -1,  5,  6, -1, -1,  7,  8,  9, 10 },    // 3013
 {  0, -1, -1, -1,  1,  2, -1, -1,  3,  4, -1, -1,  5,  6,  7,  8 },    // 0113
 {  0,  1, -1, -1,  2,  3, -1, -1,  4,  5, -1, -1,  6,  7,  8,  9 },    // 1113
 {  0,  1,  2, -1,  3,  4, -1, -1,  5,  6, -1, -1,  7,  8,  9, 10 },    // 2113
 {  0,  1,  2,  3,  4,  5, -1, -1,  6,  7, -1, -1,  8,  9, 10, 11 },    // 3113
 {  0, -1, -1, -1,  1,  2,  3, -1,  4,  5, -1, -1,  6,  7,  8,  9 },    // 0213
 {  0,  1, -1, -1,  2,  3,  4, -1,  5,  6, -1, -1,  7,  8,  9, 10 },    // 1213
 {  0,  1,  2, -1,  3,  4,  5, -1,  6,  7, -1, -1,  8,  9, 10, 11 },    // 2213
 {  0,  1,  2,  3,  4,  5,  6, -1,  7,  8, -1, -1,  9, 10, 11, 12 },    // 3213
 {  0, -1, -1, -1,  1,  2,  3,  4,  5,  6, -1, -1,  7,  8,  9, 10 },    // 0313
 {  0,  1, -1, -1,  2,  3,  4,  5,  6,  7, -1, -1,  8,  9, 10, 11 },    // 1313
 {  0,  1,  2, -1,  3,  4,  5,  6,  7,  8, -1, -1,  9, 10, 11, 12 },    // 2313
 {  0,  1,  2,  3,  4,  5,  6,  7,  8,  9, -1, -1, 10, 11, 12, 13 },    // 3313
 {  0, -1, -1, -1,  1, -1, -1, -1,  2,  3,  4, -1,  5,  6,  7,  8 },    // 0023
 {  0,  1, -1, -1,  2, -1, -1, -1,  3,  4,  5, -1,  6,  7,  8,  9 },    // 1023
 {  0,  1,  2, -1,  3, -1, -1, -1,  4,  5,  6, -1,  7,  8,  9, 10 },    // 2023
 {  0,  1,  2,  3,  4, -1, -1, -1,  5,  6,  7, -1,  8,  9, 10, 11 },    // 3023
 {  0, -1, -1, -1,  1,  2, -1, -1,  3,  4,  5, -1,  6,  7,  8,  9 },    // 0123
 {  0,  1, -1, -1,  2,  3, -1, -1,  4,  5,  6, -1,  7,  8,  9, 10 },    // 1123
 {  0,  1,  2, -1,  3,  4, -1, -1,  5,  6,  7, -1,  8,  9, 10, 11 },    // 2123
 {  0,  1,  2,  3,  4,  5, -1, -1,  6,  7,  8, -1,  9, 10, 11, 12 },    // 3123
 {  0, -1, -1, -1,  1,  2,  3, -1,  4,  5,  6, -1,  7,  8,  9, 10 },    // 0223
 {  0,  1, -1, -1,  2,  3,  4, -1,  5,  6,  7, -1,  8,  9, 10, 11 },    // 1223
 {  0,  1,  2, -1,  3,  4,  5, -1,  6,  7,  8, -1,  9, 10, 11, 12 },    // 2223
 {  0,  1,  2,  3,  4,  5,  6, -1,  7,  8,  9, -1, 10, 11, 12, 13 },    // 3223
 {  0, -1, -1, -1,  1,  2,  3,  4,  5,  6,  7, -1,  8,  9, 10, 11 },    // 0323
 {  0,  1, -1, -1,  2,  3,  4,  5,  6,  7,  8, -1,  9, 10, 11, 12 },    // 1323
 {  0,  1,  2, -1,  3,  4,  5,  6,  7,  8,  9, -1, 10, 11, 12, 13 },    // 2323
 {  0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, -1, 11, 12, 13, 14 },    // 3323
 {  0, -1, -1, -1,  1, -1, -1, -1,  2,  3,  4,  5,  6,  7,  8,  9 },    // 0033
 {  0,  1, -1, -1,  2, -1, -1, -1,  3,  4,  5,  6,  7,  8,  9, 10 },    // 1033
 {  0,  1,  2, -1,  3, -1, -1, -1,  4,  5,  6,  7,  8,  9, 10, 11 },    // 2033
 {  0,  1,  2,  3,  4, -1, -1, -1,  5,  6,  7,  8,  9, 10, 11, 12 },    // 3033
 {  0, -1, -1, -1,  1,  2, -1, -1,  3,  4,  5,  6,  7,  8,  9, 10 },    // 0133
 {  0,  1, -1, -1,  2,  3, -1, -1,  4,  5,  6,  7,  8,  9, 10, 11 },    // 1133
 {  0,  1,  2, -1,  3,  4, -1, -1,  5,  6,  7,  8,  9, 10, 11, 12 },    // 2133
 {  0,  1,  2,  3,  4,  5, -1, -1,  6,  7,  8,  9, 10, 11, 12, 13 },    // 3133
 {  0, -1, -1, -1,  1,  2,  3, -1,  4,  5,  6,  7,  8,  9, 10, 11 },    // 0233
 {  0,  1, -1, -1,  2,  3,  4, -1,  5,  6,  7,  8,  9, 10, 11, 12 },    // 1233
 {  0,  1,  2, -1,  3,  4,  5, -1,  6,  7,  8,  9, 10, 11, 12, 13 },    // 2233
 {  0,  1,  2,  3,  4,  5,  6, -1,  7,  8,  9, 10, 11, 12, 13, 14 },    // 3233
 {  0, -1, -1, -1,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12 },    // 0333
 {  0,  1, -1, -1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13 },    // 1333
 {  0,  1,  2, -1,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14 },    // 2333
 {  0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15 },    // 3333
};
#endif
