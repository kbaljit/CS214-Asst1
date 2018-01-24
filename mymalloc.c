/*
 * CS214: Systems Programming, Fall 2016
 * Assignment 1: A better malloc() and free()
 * mymalloc.c
 * Baljit Kaur & Amy Guinto
 */

#define BLOCKSIZE 5000

#include	<unistd.h> 
#include	<stdio.h> 
#include	<string.h> 
#include	<errno.h>

typedef struct Node_t{
	unsigned int size; 
	unsigned int isFree : 1;
	struct Node_t *next;
	struct Node_t *prev;
}Node;

static char block[BLOCKSIZE];	// memory block
static Node * head;				// head node
static int initialized = 0;		// keeps track if memory was initialized (i.e. if malloc ran)

void* mymalloc(unsigned int size, char * file, int line){ 
	Node * ptr; 
	Node * next;
	
	// initializes block on first malloc run
	if(!initialized){
		head = (Node *)block;
 		head->prev = NULL;
 		head->next = NULL;
		head->size = BLOCKSIZE;
 		head->isFree = 1;	 
		initialized = 1;
	}
	
	ptr = head;

	// checks if size of malloc requested is greater than memory block capacity (returns NULL if true)
	if(size > (5000-sizeof(Node))) {
		fprintf(stderr, "No space available to allocate, returning null pointer.\n Error occurred at line %d of file %s.\n",__LINE__,__FILE__); 
		return NULL;
	}
	
	do { 
		// if current pointer is in use or its size is insufficient, move to next pointer
 		if (ptr->isFree == 0 || ptr->size < size){
			ptr = ptr->next; 
		}
		// if size is exactly the same as current pointer's capacity, update metadata and return pointer
		else if(ptr->size == size) {
			ptr->isFree = 0;
			//printf("malloced pointer address: %p, memory block address: %p\n", (void *)(ptr+1), block);
 			return (void *)(ptr+1);
		}
		/*
		 * if current pointer's capacity is greater than the size + sizeof(Node),
		 * update pointer's metadata, create or update the next pointer, and return pointer
		 */
 		else if (ptr->size > (size + sizeof(Node))){
 			ptr->isFree = 0;
 			ptr->size = size;
 			if(ptr->next == NULL) {
 				next = (Node *)(((char *)(ptr+1))+size);
 				next->isFree = 1;
 				next->size = &block[4999] - (char*)(next+1);
 				next->next = NULL;
 				next->prev = ptr;
 				ptr->next = next;
 			} else {
 				next = (Node *)(((char *)(ptr+1))+size);
 				next->isFree = 1;
 				next->size = (ptr->next) - (next+1);
 				next->next = ptr->next;
 				next->prev = ptr;
 				ptr->next = next;
 			}
 			//printf("malloced pointer address: %p, memory block address: %p\n", (void *)(ptr+1), block);
 			return (void *)(ptr+1);
 		} 
 		else{
 			ptr = ptr->next;
 		} 
 	} while(ptr != NULL); 

 	fprintf(stderr, "No space available to allocate, returning null pointer.\nError occurred at line %d of file %s.\n", line, file); 
	return NULL; 
}


void myfree(void * p, char * file, int line){
	if(initialized == 0){ 
		fprintf(stderr, "Memory block uninitialized. Nothing to free.\nError occurred at line %d of file %s.\n", line, file); 
 		return;
	} 
	
	Node * ptr = head;
	//Node * prev;
	Node * next; 

	// initial check to see if given pointer is in memory block
	if((char *)p < block || (char *)p >= (block + BLOCKSIZE)) {
		fprintf(stderr, "Pointer was not allocated using malloc.\n Error occurred at line %d of file %s.\n", line, file); 
		return;
	}

	// check if user tried to free a NULL pointer
	if(p == NULL) {
		fprintf(stderr, "Cannot free NULL pointer.\nError occurred at line %d of file %s.\n", line, file); 
 		return; 
	}

	while(ptr != NULL){
		// if given pointer is a valid pointer in memory block, free it if it's in use and report error if already freed
		if(p == (void *)(ptr+1)) {
			if(ptr->isFree == 0) {
				ptr->isFree = 1;
				next = ptr->next;
				if(next->isFree == 1 && next->next == NULL) {
					ptr->size = BLOCKSIZE;
					ptr->next = NULL;
				}
				return;
			} else {
				fprintf(stderr, "Error: Redudant free()ing of the same pointer. Occurred at line %d of file %s.\n", line, file); 
				return;
			}
		} else if(p < (void *)(ptr+1)) {
			fprintf(stderr, "Error: Free()ing pointer not allocated by malloc(). Occurred at line %d of file %s.\n", line, file); 
			return;
		} else {
			ptr = ptr->next;
		}
 	}

 	fprintf(stderr, "Error: Free()ing pointer not allocated by malloc(). Occurred at line %d of file %s.\n", line, file); 
 	return;
}