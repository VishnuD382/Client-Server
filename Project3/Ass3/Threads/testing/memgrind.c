#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "mymalloc.h"

int main() {


  struct timeval start;
  struct timeval end;
  int i = 0;
  int j = 0;
  void *ptr_array[150];
  void *partC_array[50];
  int random = rand() % 2;

  void* partD_array[50];

  double aTime = 0;
  double bTime = 0;
  double cTime = 0;
  double dTime = 0;
  double eTime = 0;
  double fTime = 0;

  for (j = 0; j < 100; j++) {

    gettimeofday(&start, 0);

    for (i = 0; i < 150; i++) { // Part A
      ptr_array[i] = (char *) malloc(1);
      free(ptr_array[i]);
      ptr_array[i] = NULL;
    }

    gettimeofday(&end, 0);
    aTime = aTime + ((end.tv_sec - start.tv_sec) * 1000000 + (end.tv_usec - start.tv_usec));
  }

  printf("\n-----------'PART A DONE'--------------------\n");

  for (j = 0; j < 100; j++) {
    gettimeofday(&start, 0);

    for (i = 0; i < 150; i++) { // Part B
      ptr_array[i] = (char *) malloc(1);
    }
    for (i = 0; i < 150; i++) { // Part B
      free(ptr_array[i]);
      ptr_array[i] = NULL;
    }

    gettimeofday(&end, 0);
    bTime = bTime + ((end.tv_sec - start.tv_sec) * 1000000 + (end.tv_usec - start.tv_usec));
  }

  printf("\n-----------'PART B DONE'--------------------\n");


  int malloc_count_C = 0;  // Part C
  int malloc_index = 0;
  int free_index = 0;
  int free_count = 0;
  int rounds = 1;

  for (j = 0; j < 100; j++) {
    gettimeofday(&start, 0);
    for (i = 0; i < 50; i++) {
      partC_array[i] == NULL;
    }

    while (malloc_count_C < 50) {

      random = rand() % 2;
      printf("\nRandom: %d \n", random);
      if ((random == 1 || random == 0) && malloc_count_C == 0) {
        partC_array[malloc_index] = malloc(1);
        malloc_index++;
        malloc_count_C++;
      }

      if (random == 0 || malloc_count_C == 0) { // if have not allocated OR random is 0 malloc.
        partC_array[malloc_index] = malloc(1);
        malloc_index++;
        malloc_count_C++;
        printf("\n----%d----\n", malloc_count_C);
      }
      else if (random == 1) { // if random is 1 free.
        if (free_index >= malloc_index) { // ensures nothing that is not malloced is being free
          printf("Trying to Free a ptr not set. \n");
          continue;
        }
        else if (free_index <= malloc_index) {
          free(partC_array[free_index]);
          ptr_array[free_index] == NULL;
          free_index++;
          free_count++;
        }
      }
    }
    for (i = free_count; i < malloc_count_C; i++) { // free the rest of the pointers.
      if (partC_array[i] != NULL) {
        free(partC_array[i]);
        partC_array[i] = NULL;
      }
      if (partC_array[i] == -1) {
        break;
      }
    }


    gettimeofday(&end, 0);
    cTime = cTime + ((end.tv_sec - start.tv_sec) * 1000000 + (end.tv_usec - start.tv_usec));
  }

  printf("\n-----------'PART C DONE'--------------------\n");


// part D

  for (j = 0; j < 100; j++) {
    gettimeofday(&start, 0);

    int malloc_count_D = 0;
    free_count = 0;
    malloc_index = 0;
    free_index = 0;
    i = 0;


    while (i < 150) {

      ptr_array[i] = NULL;
      i++;

    }

    while (malloc_count_D < 50) {


      random = rand() % 2;

      int picksize = rand() % 64 + 1; // random number between 1 to 64.

      if (random == 0) { //does malloc_count

        ptr_array[malloc_index] = malloc(picksize);

        if (!(ptr_array[malloc_index] == NULL)) { // if no more room to malloc

          malloc_index++;
          malloc_count_D++;
        }

        else {

          malloc_index--;
          free(ptr_array[malloc_index]);
          ptr_array[malloc_index] = NULL;

        }

      }

      else if (random == 1) { //does free

        if (free(ptr_array[free_index]) != 0) { //can't free

          ptr_array[malloc_index] = (int *) malloc(picksize);
          malloc_index++;
          malloc_count_D++; // <- if malloc is a result of a failed free, it doesn't count towards the 1000 mallocs

        }

        else {

          ptr_array[malloc_index] = NULL;

        }
      }

      random = rand() % 2;
    }

    for (i = free_index; i < 50; i++) { // frees everything, some of which could have been free.

      free(ptr_array[i]);
      ptr_array[i] = NULL;

    }
    gettimeofday(&end, 0);
    dTime = dTime + ((end.tv_sec - start.tv_sec) * 1000000 + (end.tv_usec - start.tv_usec));
  }

  printf("\n-----------'PART D DONE'--------------------\n");

  //Part E
  //tests to check for memory allocation that is greater than 4096-Metadata = 4080

  for (j = 0; j < 100; j++) {
    gettimeofday(&start, 0);
    for (i = 0; i < 50; i++) {
      random = rand() % 5000; // chooses a random number between 0 and 4999
      ptr_array[i] = malloc(random);
      if (ptr_array[i] != NULL) {
        free(ptr_array[i]);
        ptr_array[i] = NULL;
        continue;
      }
      else {
        continue;
      }

    }

    gettimeofday(&end, 0);
    eTime = eTime + ((end.tv_sec - start.tv_sec) * 1000000 + (end.tv_usec - start.tv_usec));
  }

  printf("\n-----------'PART E DONE'--------------------\n");
  
  void* ptr_array1[150];

  // part F
  for (j = 0; j < 100; j++) {

    gettimeofday(&start, 0);
    int malloc_count_F = 0;
    int malloc_index_F = 0;
    int free_index_F = 0;

    for (i = 0; i < 150; i++) {
      ptr_array1[i] = malloc(1);
    }
    for (i = 0; i < 150; i++) {
      free(ptr_array1[i]);
      ptr_array1[i] = NULL;
    }
    for(i = 0; i < 150; i++){ // checks to see if ptr is being freed twice.
      free(ptr_array1[i]);
    }

    
    gettimeofday(&end, 0);
    fTime = fTime + ((end.tv_sec - start.tv_sec) * 1000000 + (end.tv_usec - start.tv_usec));
  }


  printf("\n-----------'PART F DONE'--------------------\n");

  printf("Average A: %lf microseconds\n", aTime);
  printf("Average B: %lf microseconds\n", bTime);
  printf("Average C: %lf microseconds\n", cTime);
  printf("Average D: %lf microseconds\n", dTime);
  printf("Average E: %lf microseconds\n", eTime);
  printf("Average F: %lf microseconds\n", fTime);


  return 0;
}
