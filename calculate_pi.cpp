#include <iostream>
#include <cstdlib>
#include <pthread.h>
#include <stdlib.h>
#include <array>
#include <math.h>
#include <random>
#include <ctime>

/*
* @authors Noah Sperling and Devin Ajimine
* @version 3/2/2018
*/

using namespace std;

// stores the number of coordinates in the circle, incremented by each thread
static int coords_in_circle = 0;

// generates a random coordinate, checks to see if it is in the circle
void *check_coordinate(void *t) {
  // calculates an x coordinate
  int r = rand() % 1000000;
  double x = (double)r/(double)1000000;

  // calculates a y coordinate
  r = rand() % 1000000;
  double y = (double)r/(double)1000000;

  // if the coordinate is in the circle, increment coords_in_circle
  if(x <= cos(asin(y))) {
    coords_in_circle++;
  }
  return 0;
}

int main(int argc, char *argv[]) {
  // checks command line arguments
  if(argc != 2) {
    cout << "Invalid command line arguments" << endl;
    return 1;
  }
  // casts number of coordinates to try to int
  int n = atoi(argv[1]);

  // an array of threads
  pthread_t threads[n];

  // seeds the rand() function
  srand(time(NULL));

  //variables
  int i;
  int return_val;
  // creates n threads to generate and check n coordinates
  for(i = 0; i < n; i++) {
    return_val = pthread_create(&threads[i], NULL, check_coordinate, NULL);
    if(return_val) {
      cout << "\nerror, unable to create thread " << return_val << endl;
      exit(-1);
    }
  }

  //calculates estimate of pi
  double pi = (double)(coords_in_circle * 4)/ (double)n;

  // prints result
  cout << "With " << n << " coordinates, we estimate pi to be " << pi << endl;

  return 0;
}
