
#include "MyMalloc.h"
#include <iostream>

int main(int argc,char** argv){
	
	InitMalloc();
	void* ptr1 = MyMalloc(32);
	void* ptr2 = MyMalloc(80);
	void* ptr3 = MyMalloc(121);
	void* ptr4 = MyMalloc(66);
	void* ptr5 = MyMalloc(1600);
	PrintFreeList();
	MyFree(ptr2);
	PrintFreeList();
	MyFree(ptr4);
	PrintFreeList();
	MyFree(ptr3);
	PrintFreeList();
	
	return 0;
}

