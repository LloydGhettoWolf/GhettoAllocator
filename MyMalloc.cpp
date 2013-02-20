//MyMalloc.cpp
#include "MyMalloc.h"


void  InitMalloc(){
	freeListHeader = (MallocStruct*)DataBuffer;
	freeListHeader->next   = NULL;
	freeListHeader->prev   = NULL;
	freeListHeader->size   = MAX_BYTES - sizeof(MallocStruct);
	freeListHeader->buffer = (unsigned char*)(freeListHeader + sizeof(MallocStruct));
}

void* MyMalloc(int size){

	MallocStruct* ptr = freeListHeader;
	int mallocStructsz = sizeof(MallocStruct);

	if(size == 0) return NULL;

	//round up size
	size += ((4 - (size&3))&3);

	while(size + mallocStructsz > ptr->size){
		ptr = ptr->next;

		if(ptr == NULL) return NULL;
	}
	
	int newSize = ptr->size - (size + mallocStructsz);
	MallocStruct* next = ptr->next;
	MallocStruct* prev = ptr->prev;

	//now change the pointer in the list, and get a pointer to this current location

	ptr->next   = NULL;
	ptr->prev   = NULL;
	ptr->size   = size;
	ptr->buffer = (unsigned char*)(ptr) + mallocStructsz;

	unsigned char* data = ptr->buffer;

	//update the freelist pointer if necessary
	if(ptr == freeListHeader){
		unsigned char* ptr3 = ptr->buffer + ptr->size;
		freeListHeader = (MallocStruct*)ptr3;

		freeListHeader->buffer = (unsigned char*)ptr + mallocStructsz;
		freeListHeader->size = newSize;
		freeListHeader->next = next;
		freeListHeader->prev = prev;

		//if next or prev pointers aren't null, update their prev and next pointers
		if(freeListHeader->next != NULL) freeListHeader->next->prev = ptr;
		if(freeListHeader->prev != NULL) freeListHeader->prev->next = ptr;
	}

	return data;
}


void  MyFree(void* data){

	if(data == NULL) return;

	//create a free node, uncoalesced
	MallocStruct* newFree = (MallocStruct*)((unsigned char*)data - sizeof(MallocStruct));
	int size = newFree->size;

	//now insert it into the linked list
	

	//use two pointers to gague where the new ptr should go
	MallocStruct* ptr1 = freeListHeader;

	//case3 other wise needs to be inserted into a list somewhere...
	while(newFree > ptr1){

		ptr1 = ptr1->next;

		//if next is NULL we need to go to case 4 - inserting at the end
		if(ptr1 == NULL) break;
	}

	// end of the list
	if(ptr1 == NULL){
		ptr1->next = newFree;
		newFree->prev = ptr1;
	}else if(ptr1->prev == NULL){
		//beginning of the list
	  newFree->next = freeListHeader;
	  freeListHeader->prev = newFree;
	  freeListHeader = newFree;
	}else{
	  //middle of the list
	  ptr1->prev->next = newFree;
	  newFree->prev = ptr1->prev;
	  ptr1->prev = newFree;
	  newFree->next = ptr1;
	}

	newFree->size = size;

	//now....coalescing
	//check if can coalesce with previous block
	if(newFree->prev != NULL){
		if((newFree->prev->buffer + newFree->prev->size) == (unsigned char*)newFree){
			
			newFree->prev->next = newFree->next;
			newFree->next->prev = newFree->prev;
			newFree->prev->size += newFree->size + sizeof(MallocStruct);

			//reassign in case we need to coalesce too
			newFree = newFree->prev;
		}
	}

	//check with the next block
	if(newFree->next != NULL){
		if((newFree->buffer + newFree->size) == (unsigned char*)newFree->next){

				newFree->size += newFree->next->size + sizeof(MallocStruct);
				newFree->next = newFree->next->next;
				if(newFree->next!=NULL)newFree->next->prev = newFree;
			}
	}

	if(newFree == freeListHeader)
		freeListHeader = newFree;
}

void PrintFreeList(){
	MallocStruct* header = freeListHeader;
	int elem = 0;
	while(header!=NULL){
		std::cout<<"size of list elem"<< elem++<<": "<<header->size<<std::endl;
		header = header->next;
	}
}