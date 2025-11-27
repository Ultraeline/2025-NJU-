#pragma once

#include"map.h"
#include<vector>

extern const char* MAP1[Len][Len];
extern const char* MAP2[Len][Len];
extern const char* MAP3[Len][Len];
extern const char* MAP4[Len][Len];
extern const char* MAP5[Len][Len];

void CreateMap1(Maps& map);
void CreateMap2(Maps& map);
void CreateMap3(Maps& map);
void CreateMap4(Maps& map);
void CreateMap5(Maps& map);




void (*CreateMap[])(Maps&);