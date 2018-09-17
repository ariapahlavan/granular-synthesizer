// SoundArrays.h
// Runs on TM4C123
// todo: type description
// Aria Pahlavan, Mehrad Yousefi

#ifndef EE445L_SOUNDARRAYS_H
#define EE445L_SOUNDARRAYS_H

#include "Music.h"
#include "inc/tm4c123gh6pm.h"

//#define C2   1194   // 2093 Hz
//#define B2   1265   // 1975.5 Hz
//#define BF2  1341   // 1864.7 Hz
//#define A2   1420   // 1760 Hz
//#define AF2  1505   // 1661.2 Hz
//#define G2   1594   // 1568 Hz
//#define GF2  1689   // 1480 Hz
//#define F2   1790   // 1396.9 Hz
//#define E2   1896   // 1318.5 Hz
//#define EF2  2009   // 1244.5 Hz
//#define D2   2128   // 1174.7 Hz
//#define DF2  2255   // 1108.7 Hz
//#define C1    2389   // 1046.5 Hz
//#define B1    2531   // 987.8 Hz
//#define BF1   2681   // 932.3 Hz
//#define A1    2841   // 880 Hz
//#define AF1   3010   // 830.6 Hz
//#define G1    3189   // 784 Hz
//#define GF1  3378   // 740 Hz
//#define F1   3579   // 698.5 Hz
//#define E1   3792   // 659.3 Hz
//#define EF1  4018   // 622.3 Hz
//#define D1   4257   // 587.3 Hz
//#define DF1  4510   // 554.4 Hz
//#define C  4778   // 523.3 Hz
//#define B  5062   // 493.9 Hz
//#define BF 5363   // 466.2 Hz
//#define A  5682   // 440 Hz
//#define AF 6020   // 415.3 Hz
//#define G  6378   // 392 Hz
//#define GF 6757   // 370 Hz
//#define F  7159   // 349.2 Hz
//#define E  7584   // 329.6 Hz
//#define EF 8035   // 311.1 Hz
//#define D  8513   // 293.7 Hz
//#define DF 9019   // 277.2 Hz
//#define C0  9556   // 261.6 Hz
//#define B0  10124   // 246.9 Hz
//#define BF0 10726   // 233.1 Hz
//#define A0  11364   // 220 Hz
//#define AF0 12039   // 207.7 Hz
//#define G0  12755   // 196 Hz
//#define GF0 13514   // 185 Hz
//#define F0  14317   // 174.6 Hz
//#define E0  15169   // 164.8 Hz
//#define EF0 16071   // 155.6 Hz
//#define D0  17026   // 146.8 Hz
//#define DF0 18039   // 138.6 Hz
//#define C_1  19111   // 130.8 Hz

/** @NOTE assuming bus freq = 50MHz and sine wave has 64 samples */
#define C_2 11945   // 65.406 Hz
#define DF_1 11274   // 69.296 Hz
#define D_1 10641   // 73.416 Hz
#define EF_1 10044   // 77.782 Hz
#define E_1 9480   // 82.407 Hz
#define F_1 8948   // 87.307 Hz
#define GF_1 8446   // 92.499 Hz
#define G_1 7972   // 97.999 Hz
#define AF_1 7525   // 103.826 Hz
#define A_1 7102   // 110.000 Hz
#define BF_1 6704   // 116.541 Hz
#define B_1 6327   // 123.471 Hz
#define C_1 5972   // 130.813 Hz
#define DF0 5637   // 138.591 Hz
#define D0 5321   // 146.832 Hz
#define EF0 5022   // 155.563 Hz
#define E0 4740   // 164.814 Hz
#define F0 4474   // 174.614 Hz
#define GF0 4223   // 184.997 Hz
#define G0 3986   // 195.998 Hz
#define AF0 3762   // 207.652 Hz
#define A0 3551   // 220.000 Hz
#define BF0 3352   // 233.082 Hz
#define B0 3164   // 246.942 Hz
#define C0 2986   // 261.626 Hz
#define DF 2819   // 277.183 Hz
#define D 2660   // 293.665 Hz
#define EF 2511   // 311.127 Hz
#define E 2370   // 329.628 Hz
#define F 2237   // 349.228 Hz
#define GF 2112   // 369.994 Hz
#define G 1993   // 391.995 Hz
#define AF 1881   // 415.305 Hz
#define A 1776   // 440.000 Hz
#define BF 1676   // 466.164 Hz
#define B 1582   // 493.883 Hz
#define C 1493   // 523.251 Hz
#define DF1 1409   // 554.365 Hz
#define D1 1330   // 587.330 Hz
#define EF1 1256   // 622.254 Hz
#define E1 1185   // 659.255 Hz
#define F1 1119   // 698.456 Hz
#define GF1 1056   // 739.989 Hz
#define G1 997   // 783.991 Hz
#define AF1 941   // 830.609 Hz
#define A1 888   // 880.000 Hz
#define BF1 838   // 932.328 Hz
#define B1 791   // 987.767 Hz
#define C1 747   // 1046.502 Hz
#define DF2 705   // 1108.731 Hz
#define D2 665   // 1174.659 Hz
#define EF2 628   // 1244.508 Hz
#define E2 593   // 1318.510 Hz
#define F2 559   // 1396.913 Hz
#define GF2 528   // 1479.978 Hz
#define G2 498   // 1567.982 Hz
#define AF2 470   // 1661.219 Hz
#define A2 444   // 1760.000 Hz
#define BF2 419   // 1864.655 Hz
#define B2 395   // 1975.533 Hz
#define C2 373   // 2093.005 Hz

#define MUTE 0
#define Z1   1

#define POKEMON_LENGTH 88
#define TETRIS_LENGTH  133

#define wave_t const unsigned short
/*
extern wave_t sineWave32[32];

extern wave_t sineWave64[64];

extern wave_t flute64[64];

extern wave_t horn64[64];

extern wave_t trumpet64[64];

extern wave_t bassoon64[64];

extern wave_t oboe64[64];

extern wave_t guitar64[64];

extern wave_t envelope64[64];

extern const uint32_t envelope100[100];

extern wave_t oboe128[128];

extern wave_t envelope128[128];

extern const Note pokemonNotes[POKEMON_LENGTH];

extern Song pokemonMusic;

extern const Note pokemonNotes2[POKEMON_LENGTH];

extern int PokemonSong[280];

extern int TetrisSong[TETRIS_LENGTH];*/

#endif //EE445L_SOUNDARRAYS_H
