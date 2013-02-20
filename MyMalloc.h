//MyMalloc.h
#pragma once
#include <cstdlib>
#include <iostream>
#include <assert.h>

const int MAX_BYTES = 16 * 1024 * 1024;
struct MallocStruct;

void  InitMalloc();
void* MyMalloc(int size);
void  MyFree(void* data);
void PrintFreeList();

struct MallocStruct{
	MallocStruct*  next;
	MallocStruct*  prev;
	unsigned char* buffer;
	size_t size;
};

static MallocStruct* freeListHeader;
static unsigned char DataBuffer[MAX_BYTES];