#ifndef __UTILS_H
#define __UTILS_H

#include <stdint.h>
#include "Define.h"
void Uint16ToBytes(uint16_t *u, uint8_t *b);
void BytesToUint16(uint8_t *b, uint16_t *u);

void Int16ToBytes(int16_t *i, uint8_t *b);
void BytesToInt16(uint8_t *b, int16_t *i);

void Uint32ToBytes(uint32_t *u, uint8_t *b);
void BytesToUint32(uint8_t *b, uint32_t *u);

void Real32ToBytes(real32_t *r, uint8_t *b);
void BytesToReal32(uint8_t *b, real32_t *r);

void String_Clear(uint8_t* string);
void String_RemoveCRLF(uint8_t* stringin, uint8_t* stringout);
void String_Copy(uint8_t* stringin, uint8_t* stringout);
void String_Split(uint8_t* stringin, uint8_t character, uint8_t startpos, uint8_t* stringout);
void String_Split_By_Lng(uint8_t* stringin, uint8_t startpos, uint8_t length, uint8_t* stringout);
void String_Merger(uint8_t* strfirst, uint8_t* strsecond, uint8_t character, uint8_t* stringout);
void String_Special(uint8_t* stringin, uint8_t character, uint8_t* stringout);
int8_t StringCompare(uint8_t* str1, uint8_t* str2);
double Str2Double(uint8_t *string);
double Str2Double_Dev(uint8_t* string);
int32_t Double2Int(double dbnum);
void Int2Str(int32_t inum, uint8_t* string);
void Double2Str(double dbnum,uint8_t num_of_frac,uint8_t* string);
void Matrix2_Add2(float MatrixOut[2][2],float MatrixIn1[2][2],float MatrixIn2[2][2]);
void Matrix2_Mul(float MatrixOut[2],float MatrixIn1[2][2],float MatrixIn2[2]);
void Matrix2_Mul12(float MatrixOut[1][2],float MatrixIn1[1][2],float MatrixIn2[2][2]);
void Matrix2_Mul2(float MatrixOut[2][2],float MatrixIn1[2][2],float MatrixIn2[2][2]);
void VectorAdd(float pfVectorOut[3], float pfVectorIn1[3],float pfVectorIn2[3]);
void VectorCrossProduct(float pfVectorOut[3], float pfVectorIn1[3],float pfVectorIn2[3]);
float VectorDotProduct(float pfVectorIn1[3], float pfVectorIn2[3]);
void VectorScale(float pfVectorOut[3], float pfVectorIn[3], float fScale);

#endif /* __UTILS_H */

