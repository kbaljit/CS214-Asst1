/*
 * CS214: Systems Programming, Fall 2016
 * Assignment 1: A better malloc() and free()
 * mymalloc.h
 * Baljit Kaur & Amy Guinto
 */

#ifndef _MYMALLOC_H_
#define _MYMALLOC_H_

void* mymalloc(unsigned int size, char * file, int line);
void myfree(void * p, char * file, int line);

#define malloc(x) mymalloc(x, __FILE__, __LINE__)
#define free(x) myfree(x, __FILE__, __LINE__)

#endif // _MYMALLOC_H_