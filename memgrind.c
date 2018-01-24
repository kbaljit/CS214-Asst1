/*
 * CS214: Systems Programming, Fall 2016
 * Assignment 1: A better malloc() and free()
 * memgrind.c
 * Baljit Kaur & Amy Guinto
 */

#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>
#include "mymalloc.h"

int main() {
	struct timeval start, end;
	void *pointers[3000];
	int i, j, size = 0;
	long a, b, c, d, e, f;
	srand(time(NULL));
	
	// Workload E
	gettimeofday(&start, NULL);
	for(i = 0; i < 100; i++) {
		int size;
		for(j = 0; j < 3000; j++) {
			size = ((rand() % 4900)+1);
			free(malloc(size));
		}
	}
	gettimeofday(&end, NULL);
	e = (((end.tv_sec*1000000 + end.tv_usec) - (start.tv_sec*1000000 + start.tv_usec))/100);

	// Workload D
	gettimeofday(&start, NULL);
	for(i = 0; i < 100; i++) {
		int choice, mTotal = 0, fTotal = 0;

		do {
			choice = rand() % 2; // get random 0 or 1
			size = 77;

			// if number of mallocs = number of frees, malloc so that free will work if randomly chosen
			if(mTotal == fTotal) {
				pointers[mTotal] = malloc(size);
				mTotal++;
			}

			if(choice == 0) { // if 0, malloc and put into ptr array
				pointers[mTotal] = malloc(size);
				mTotal++;
			} else { // if 1, free earliest ptr in ptr array if not NULL
				if(pointers[fTotal] != NULL) {
					free(pointers[fTotal]);
					fTotal++;
				} else {
					fTotal++;
				}	
			}

		} while(mTotal < 3000);

		// free any remaining non-NULL pointers
		while(mTotal > fTotal) {
			if(pointers[fTotal] != NULL)
				free(pointers[fTotal]);
			fTotal++;
		}
	}
	gettimeofday(&end, NULL);
	d = (((end.tv_sec*1000000 + end.tv_usec) - (start.tv_sec*1000000 + start.tv_usec))/100);

	// Workload A
	gettimeofday(&start, NULL);
	for(i = 0; i < 100; i++) { 
		for(j = 0; j < 3000; j++) { 
			pointers[j] = malloc(1);
		}
		for(j = 0; j < 3000; j++) {
			free(pointers[j]);
		}
	}
	gettimeofday(&end, NULL);
	a = (((end.tv_sec*1000000 + end.tv_usec) - (start.tv_sec*1000000 + start.tv_usec))/100);

	// Workload B
	gettimeofday(&start, NULL);
	for(i = 0; i < 100; i++) {
		for(j = 0; j < 3000; j++) {
			free(malloc(1));
		};
	}
	gettimeofday(&end, NULL);
	b = (((end.tv_sec*1000000 + end.tv_usec) - (start.tv_sec*1000000 + start.tv_usec))/100);
	
	// Workload C
	gettimeofday(&start, NULL);
	for(i = 0; i < 100; i++) {
		int choice, mTotal = 0, fTotal = 0;

		do {
			// if number of mallocs = number of frees, malloc so that free will work if randomly chosen
			if(mTotal == fTotal) {
				pointers[mTotal] = malloc(1);
				mTotal++;
			}
			if((mTotal-fTotal) > 175) {
				free(pointers[fTotal]);
				fTotal++;
			}

			choice = rand() % 2; // get random 0 or 1

			if(choice == 0) { // if 0, malloc and put into ptr array
				pointers[mTotal] = malloc(1);
				mTotal++;
			} else { // if 1, free earliest ptr in ptr array if not NULL
				if(pointers[fTotal] != NULL) {
					free(pointers[fTotal]);
					fTotal++;
				} else {
					fTotal++;
				}	
			}

		} while(mTotal < 3000);

		// free any remaining non-NULL pointers
		while(mTotal > fTotal) {
			if(pointers[fTotal] != NULL)
				free(pointers[fTotal]);
			fTotal++;
		}
	}
	gettimeofday(&end, NULL);
	c = (((end.tv_sec*1000000 + end.tv_usec) - (start.tv_sec*1000000 + start.tv_usec))/100);	
	
	// Workload F
	gettimeofday(&start, NULL);
	for(i = 0; i < 100; i++) {
		for(j = 1; j <= 350; j++) {
			pointers[j] = (char *)malloc(1);
			j++;
			pointers[j] = (char *)malloc(1);
			free(pointers[j]);
		}
		for(j = 1; j <= 350; j++) {
			if(j % 2 == 1){
				free(pointers[j]);
			}
		}
	}
	gettimeofday(&end, NULL);
	f = (((end.tv_sec*1000000 + end.tv_usec) - (start.tv_sec*1000000 + start.tv_usec))/100);
	

	printf("\nAverage Time for Workload A: %ld microseconds\n", a);
	printf("\nAverage Time for Workload B: %ld microseconds\n", b);
	printf("\nAverage Time for Workload C: %ld microseconds\n", c);
	printf("\nAverage Time for Workload D: %ld microseconds\n", d);
	printf("\nAverage Time for Workload E: %ld microseconds\n", e);
	printf("\nAverage Time for Workload F: %ld microseconds\n", f);

	return 0;
}