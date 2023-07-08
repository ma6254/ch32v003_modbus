#ifndef MBITS_H
#define MBITS_H

#define SetBit(x) (1 << x)
#define RSetBit(x) (~(1 << x))

#define Set8Bit(x, b) ((x) |= ((uint8_t)1 << (b)))
#define Clr8Bit(x, b) ((x) &= ~((uint8_t)1 << (b)))
#define Op8Bit(x, b, y) (y) ? Set8Bit(x, b) : Clr8Bit(x, b)
#define Read8Bit(x, b) ((x) & ((uint8_t)1 << (b)))

#define Set16Bit(x, b) ((x) |= ((uint16_t)1 << (b)))
#define Clr16Bit(x, b) ((x) &= ~((uint16_t)1 << (b)))
#define Op16Bit(x, b, y) (y) ? Set16Bit(x, b) : Clr16Bit(x, b)
#define Read16Bit(x, b) ((x) & ((uint16_t)1 << (b)))

#define Set32Bit(x, b) ((x) |= ((uint32_t)1 << (b)))
#define Clr32Bit(x, b) ((x) &= ~((uint32_t)1 << (b)))
#define Op32Bit(x, b, y) (y) ? Set32Bit(x, b) : Clr32Bit(x, b)
#define Read32Bit(x, b) ((x) & ((uint32_t)1 << (b)))

#define Set32Bit_Le(x, b) ((x) |= ((uint32_t)1 << (b)))

#define S8(x) (1 << (x))
#define S16(x) (((uint16_t)1) << (x))
#define S32(x) (((uint32_t)1) << (x))

#define Set8Bit0(x) Set8Bit(x, 0)
#define Set8Bit1(x) Set8Bit(x, 1)
#define Set8Bit2(x) Set8Bit(x, 2)
#define Set8Bit3(x) Set8Bit(x, 3)
#define Set8Bit4(x) Set8Bit(x, 4)
#define Set8Bit5(x) Set8Bit(x, 5)
#define Set8Bit6(x) Set8Bit(x, 6)
#define Set8Bit7(x) Set8Bit(x, 7)

#define Clr8Bit0(x) Clr8Bit(x, 0)
#define Clr8Bit1(x) Clr8Bit(x, 1)
#define Clr8Bit2(x) Clr8Bit(x, 2)
#define Clr8Bit3(x) Clr8Bit(x, 3)
#define Clr8Bit4(x) Clr8Bit(x, 4)
#define Clr8Bit5(x) Clr8Bit(x, 5)
#define Clr8Bit6(x) Clr8Bit(x, 6)
#define Clr8Bit7(x) Clr8Bit(x, 7)

#define Read8Bit0(x) Read8Bit(x, 0)
#define Read8Bit1(x) Read8Bit(x, 1)
#define Read8Bit2(x) Read8Bit(x, 2)
#define Read8Bit3(x) Read8Bit(x, 3)
#define Read8Bit4(x) Read8Bit(x, 4)
#define Read8Bit5(x) Read8Bit(x, 5)
#define Read8Bit6(x) Read8Bit(x, 6)
#define Read8Bit7(x) Read8Bit(x, 7)

#define Set16Bit0(x) Set16Bit(x, 0)
#define Set16Bit1(x) Set16Bit(x, 1)
#define Set16Bit2(x) Set16Bit(x, 2)
#define Set16Bit3(x) Set16Bit(x, 3)
#define Set16Bit4(x) Set16Bit(x, 4)
#define Set16Bit5(x) Set16Bit(x, 5)
#define Set16Bit6(x) Set16Bit(x, 6)
#define Set16Bit7(x) Set16Bit(x, 7)
#define Set16Bit8(x) Set16Bit(x, 8)
#define Set16Bit9(x) Set16Bit(x, 9)
#define Set16Bit10(x) Set16Bit(x, 10)
#define Set16Bit11(x) Set16Bit(x, 11)
#define Set16Bit12(x) Set16Bit(x, 12)
#define Set16Bit13(x) Set16Bit(x, 13)
#define Set16Bit14(x) Set16Bit(x, 14)
#define Set16Bit15(x) Set16Bit(x, 15)

#define Clr16Bit0(x) Clr16Bit(x, 0)
#define Clr16Bit1(x) Clr16Bit(x, 1)
#define Clr16Bit2(x) Clr16Bit(x, 2)
#define Clr16Bit3(x) Clr16Bit(x, 3)
#define Clr16Bit4(x) Clr16Bit(x, 4)
#define Clr16Bit5(x) Clr16Bit(x, 5)
#define Clr16Bit6(x) Clr16Bit(x, 6)
#define Clr16Bit7(x) Clr16Bit(x, 7)
#define Clr16Bit8(x) Clr16Bit(x, 8)
#define Clr16Bit9(x) Clr16Bit(x, 9)
#define Clr16Bit10(x) Clr16Bit(x, 10)
#define Clr16Bit11(x) Clr16Bit(x, 11)
#define Clr16Bit12(x) Clr16Bit(x, 12)
#define Clr16Bit13(x) Clr16Bit(x, 13)
#define Clr16Bit14(x) Clr16Bit(x, 14)
#define Clr16Bit15(x) Clr16Bit(x, 15)

#define Read16Bit0(x) Read16Bit(x, 0)
#define Read16Bit1(x) Read16Bit(x, 1)
#define Read16Bit2(x) Read16Bit(x, 2)
#define Read16Bit3(x) Read16Bit(x, 3)
#define Read16Bit4(x) Read16Bit(x, 4)
#define Read16Bit5(x) Read16Bit(x, 5)
#define Read16Bit6(x) Read16Bit(x, 6)
#define Read16Bit7(x) Read16Bit(x, 7)
#define Read16Bit8(x) Read16Bit(x, 8)
#define Read16Bit9(x) Read16Bit(x, 9)
#define Read16Bit10(x) Read16Bit(x, 10)
#define Read16Bit11(x) Read16Bit(x, 11)
#define Read16Bit12(x) Read16Bit(x, 12)
#define Read16Bit13(x) Read16Bit(x, 13)
#define Read16Bit14(x) Read16Bit(x, 14)
#define Read16Bit15(x) Read16Bit(x, 15)

enum
{
    B0000, // Hex:0 Dec:0
    B0001, // Hex:1 Dec:1
    B0010, // Hex:2 Dec:2
    B0011, // Hex:3 Dec:3
    B0100, // Hex:4 Dec:4
    B0101, // Hex:5 Dec:5
    B0110, // Hex:6 Dec:6
    B0111, // Hex:7 Dec:7
    B1000, // Hex:8 Dec:8
    B1001, // Hex:9 Dec:9
    B1010, // Hex:A Dec:10
    B1011, // Hex:B Dec:11
    B1100, // Hex:C Dec:12
    B1101, // Hex:D Dec:13
    B1110, // Hex:E Dec:14
    B1111, // Hex:F Dec:15
};

enum
{
    B0000_0000, // Hex:0 Dec:0
    B0000_0001, // Hex:1 Dec:1
    B0000_0010, // Hex:2 Dec:2
    B0000_0011, // Hex:3 Dec:3
    B0000_0100, // Hex:4 Dec:4
    B0000_0101, // Hex:5 Dec:5
    B0000_0110, // Hex:6 Dec:6
    B0000_0111, // Hex:7 Dec:7
    B0000_1000, // Hex:8 Dec:8
    B0000_1001, // Hex:9 Dec:9
    B0000_1010, // Hex:A Dec:10
    B0000_1011, // Hex:B Dec:11
    B0000_1100, // Hex:C Dec:12
    B0000_1101, // Hex:D Dec:13
    B0000_1110, // Hex:E Dec:14
    B0000_1111, // Hex:F Dec:15
    B0001_0000, // Hex:10 Dec:16
    B0001_0001, // Hex:11 Dec:17
    B0001_0010, // Hex:12 Dec:18
    B0001_0011, // Hex:13 Dec:19
    B0001_0100, // Hex:14 Dec:20
    B0001_0101, // Hex:15 Dec:21
    B0001_0110, // Hex:16 Dec:22
    B0001_0111, // Hex:17 Dec:23
    B0001_1000, // Hex:18 Dec:24
    B0001_1001, // Hex:19 Dec:25
    B0001_1010, // Hex:1A Dec:26
    B0001_1011, // Hex:1B Dec:27
    B0001_1100, // Hex:1C Dec:28
    B0001_1101, // Hex:1D Dec:29
    B0001_1110, // Hex:1E Dec:30
    B0001_1111, // Hex:1F Dec:31
    B0010_0000, // Hex:20 Dec:32
    B0010_0001, // Hex:21 Dec:33
    B0010_0010, // Hex:22 Dec:34
    B0010_0011, // Hex:23 Dec:35
    B0010_0100, // Hex:24 Dec:36
    B0010_0101, // Hex:25 Dec:37
    B0010_0110, // Hex:26 Dec:38
    B0010_0111, // Hex:27 Dec:39
    B0010_1000, // Hex:28 Dec:40
    B0010_1001, // Hex:29 Dec:41
    B0010_1010, // Hex:2A Dec:42
    B0010_1011, // Hex:2B Dec:43
    B0010_1100, // Hex:2C Dec:44
    B0010_1101, // Hex:2D Dec:45
    B0010_1110, // Hex:2E Dec:46
    B0010_1111, // Hex:2F Dec:47
    B0011_0000, // Hex:30 Dec:48
    B0011_0001, // Hex:31 Dec:49
    B0011_0010, // Hex:32 Dec:50
    B0011_0011, // Hex:33 Dec:51
    B0011_0100, // Hex:34 Dec:52
    B0011_0101, // Hex:35 Dec:53
    B0011_0110, // Hex:36 Dec:54
    B0011_0111, // Hex:37 Dec:55
    B0011_1000, // Hex:38 Dec:56
    B0011_1001, // Hex:39 Dec:57
    B0011_1010, // Hex:3A Dec:58
    B0011_1011, // Hex:3B Dec:59
    B0011_1100, // Hex:3C Dec:60
    B0011_1101, // Hex:3D Dec:61
    B0011_1110, // Hex:3E Dec:62
    B0011_1111, // Hex:3F Dec:63
    B0100_0000, // Hex:40 Dec:64
    B0100_0001, // Hex:41 Dec:65
    B0100_0010, // Hex:42 Dec:66
    B0100_0011, // Hex:43 Dec:67
    B0100_0100, // Hex:44 Dec:68
    B0100_0101, // Hex:45 Dec:69
    B0100_0110, // Hex:46 Dec:70
    B0100_0111, // Hex:47 Dec:71
    B0100_1000, // Hex:48 Dec:72
    B0100_1001, // Hex:49 Dec:73
    B0100_1010, // Hex:4A Dec:74
    B0100_1011, // Hex:4B Dec:75
    B0100_1100, // Hex:4C Dec:76
    B0100_1101, // Hex:4D Dec:77
    B0100_1110, // Hex:4E Dec:78
    B0100_1111, // Hex:4F Dec:79
    B0101_0000, // Hex:50 Dec:80
    B0101_0001, // Hex:51 Dec:81
    B0101_0010, // Hex:52 Dec:82
    B0101_0011, // Hex:53 Dec:83
    B0101_0100, // Hex:54 Dec:84
    B0101_0101, // Hex:55 Dec:85
    B0101_0110, // Hex:56 Dec:86
    B0101_0111, // Hex:57 Dec:87
    B0101_1000, // Hex:58 Dec:88
    B0101_1001, // Hex:59 Dec:89
    B0101_1010, // Hex:5A Dec:90
    B0101_1011, // Hex:5B Dec:91
    B0101_1100, // Hex:5C Dec:92
    B0101_1101, // Hex:5D Dec:93
    B0101_1110, // Hex:5E Dec:94
    B0101_1111, // Hex:5F Dec:95
    B0110_0000, // Hex:60 Dec:96
    B0110_0001, // Hex:61 Dec:97
    B0110_0010, // Hex:62 Dec:98
    B0110_0011, // Hex:63 Dec:99
    B0110_0100, // Hex:64 Dec:100
    B0110_0101, // Hex:65 Dec:101
    B0110_0110, // Hex:66 Dec:102
    B0110_0111, // Hex:67 Dec:103
    B0110_1000, // Hex:68 Dec:104
    B0110_1001, // Hex:69 Dec:105
    B0110_1010, // Hex:6A Dec:106
    B0110_1011, // Hex:6B Dec:107
    B0110_1100, // Hex:6C Dec:108
    B0110_1101, // Hex:6D Dec:109
    B0110_1110, // Hex:6E Dec:110
    B0110_1111, // Hex:6F Dec:111
    B0111_0000, // Hex:70 Dec:112
    B0111_0001, // Hex:71 Dec:113
    B0111_0010, // Hex:72 Dec:114
    B0111_0011, // Hex:73 Dec:115
    B0111_0100, // Hex:74 Dec:116
    B0111_0101, // Hex:75 Dec:117
    B0111_0110, // Hex:76 Dec:118
    B0111_0111, // Hex:77 Dec:119
    B0111_1000, // Hex:78 Dec:120
    B0111_1001, // Hex:79 Dec:121
    B0111_1010, // Hex:7A Dec:122
    B0111_1011, // Hex:7B Dec:123
    B0111_1100, // Hex:7C Dec:124
    B0111_1101, // Hex:7D Dec:125
    B0111_1110, // Hex:7E Dec:126
    B0111_1111, // Hex:7F Dec:127
    B1000_0000, // Hex:80 Dec:128
    B1000_0001, // Hex:81 Dec:129
    B1000_0010, // Hex:82 Dec:130
    B1000_0011, // Hex:83 Dec:131
    B1000_0100, // Hex:84 Dec:132
    B1000_0101, // Hex:85 Dec:133
    B1000_0110, // Hex:86 Dec:134
    B1000_0111, // Hex:87 Dec:135
    B1000_1000, // Hex:88 Dec:136
    B1000_1001, // Hex:89 Dec:137
    B1000_1010, // Hex:8A Dec:138
    B1000_1011, // Hex:8B Dec:139
    B1000_1100, // Hex:8C Dec:140
    B1000_1101, // Hex:8D Dec:141
    B1000_1110, // Hex:8E Dec:142
    B1000_1111, // Hex:8F Dec:143
    B1001_0000, // Hex:90 Dec:144
    B1001_0001, // Hex:91 Dec:145
    B1001_0010, // Hex:92 Dec:146
    B1001_0011, // Hex:93 Dec:147
    B1001_0100, // Hex:94 Dec:148
    B1001_0101, // Hex:95 Dec:149
    B1001_0110, // Hex:96 Dec:150
    B1001_0111, // Hex:97 Dec:151
    B1001_1000, // Hex:98 Dec:152
    B1001_1001, // Hex:99 Dec:153
    B1001_1010, // Hex:9A Dec:154
    B1001_1011, // Hex:9B Dec:155
    B1001_1100, // Hex:9C Dec:156
    B1001_1101, // Hex:9D Dec:157
    B1001_1110, // Hex:9E Dec:158
    B1001_1111, // Hex:9F Dec:159
    B1010_0000, // Hex:A0 Dec:160
    B1010_0001, // Hex:A1 Dec:161
    B1010_0010, // Hex:A2 Dec:162
    B1010_0011, // Hex:A3 Dec:163
    B1010_0100, // Hex:A4 Dec:164
    B1010_0101, // Hex:A5 Dec:165
    B1010_0110, // Hex:A6 Dec:166
    B1010_0111, // Hex:A7 Dec:167
    B1010_1000, // Hex:A8 Dec:168
    B1010_1001, // Hex:A9 Dec:169
    B1010_1010, // Hex:AA Dec:170
    B1010_1011, // Hex:AB Dec:171
    B1010_1100, // Hex:AC Dec:172
    B1010_1101, // Hex:AD Dec:173
    B1010_1110, // Hex:AE Dec:174
    B1010_1111, // Hex:AF Dec:175
    B1011_0000, // Hex:B0 Dec:176
    B1011_0001, // Hex:B1 Dec:177
    B1011_0010, // Hex:B2 Dec:178
    B1011_0011, // Hex:B3 Dec:179
    B1011_0100, // Hex:B4 Dec:180
    B1011_0101, // Hex:B5 Dec:181
    B1011_0110, // Hex:B6 Dec:182
    B1011_0111, // Hex:B7 Dec:183
    B1011_1000, // Hex:B8 Dec:184
    B1011_1001, // Hex:B9 Dec:185
    B1011_1010, // Hex:BA Dec:186
    B1011_1011, // Hex:BB Dec:187
    B1011_1100, // Hex:BC Dec:188
    B1011_1101, // Hex:BD Dec:189
    B1011_1110, // Hex:BE Dec:190
    B1011_1111, // Hex:BF Dec:191
    B1100_0000, // Hex:C0 Dec:192
    B1100_0001, // Hex:C1 Dec:193
    B1100_0010, // Hex:C2 Dec:194
    B1100_0011, // Hex:C3 Dec:195
    B1100_0100, // Hex:C4 Dec:196
    B1100_0101, // Hex:C5 Dec:197
    B1100_0110, // Hex:C6 Dec:198
    B1100_0111, // Hex:C7 Dec:199
    B1100_1000, // Hex:C8 Dec:200
    B1100_1001, // Hex:C9 Dec:201
    B1100_1010, // Hex:CA Dec:202
    B1100_1011, // Hex:CB Dec:203
    B1100_1100, // Hex:CC Dec:204
    B1100_1101, // Hex:CD Dec:205
    B1100_1110, // Hex:CE Dec:206
    B1100_1111, // Hex:CF Dec:207
    B1101_0000, // Hex:D0 Dec:208
    B1101_0001, // Hex:D1 Dec:209
    B1101_0010, // Hex:D2 Dec:210
    B1101_0011, // Hex:D3 Dec:211
    B1101_0100, // Hex:D4 Dec:212
    B1101_0101, // Hex:D5 Dec:213
    B1101_0110, // Hex:D6 Dec:214
    B1101_0111, // Hex:D7 Dec:215
    B1101_1000, // Hex:D8 Dec:216
    B1101_1001, // Hex:D9 Dec:217
    B1101_1010, // Hex:DA Dec:218
    B1101_1011, // Hex:DB Dec:219
    B1101_1100, // Hex:DC Dec:220
    B1101_1101, // Hex:DD Dec:221
    B1101_1110, // Hex:DE Dec:222
    B1101_1111, // Hex:DF Dec:223
    B1110_0000, // Hex:E0 Dec:224
    B1110_0001, // Hex:E1 Dec:225
    B1110_0010, // Hex:E2 Dec:226
    B1110_0011, // Hex:E3 Dec:227
    B1110_0100, // Hex:E4 Dec:228
    B1110_0101, // Hex:E5 Dec:229
    B1110_0110, // Hex:E6 Dec:230
    B1110_0111, // Hex:E7 Dec:231
    B1110_1000, // Hex:E8 Dec:232
    B1110_1001, // Hex:E9 Dec:233
    B1110_1010, // Hex:EA Dec:234
    B1110_1011, // Hex:EB Dec:235
    B1110_1100, // Hex:EC Dec:236
    B1110_1101, // Hex:ED Dec:237
    B1110_1110, // Hex:EE Dec:238
    B1110_1111, // Hex:EF Dec:239
    B1111_0000, // Hex:F0 Dec:240
    B1111_0001, // Hex:F1 Dec:241
    B1111_0010, // Hex:F2 Dec:242
    B1111_0011, // Hex:F3 Dec:243
    B1111_0100, // Hex:F4 Dec:244
    B1111_0101, // Hex:F5 Dec:245
    B1111_0110, // Hex:F6 Dec:246
    B1111_0111, // Hex:F7 Dec:247
    B1111_1000, // Hex:F8 Dec:248
    B1111_1001, // Hex:F9 Dec:249
    B1111_1010, // Hex:FA Dec:250
    B1111_1011, // Hex:FB Dec:251
    B1111_1100, // Hex:FC Dec:252
    B1111_1101, // Hex:FD Dec:253
    B1111_1110, // Hex:FE Dec:254
    B1111_1111, // Hex:FF Dec:255
};

#endif /* End of MBITS_H */
