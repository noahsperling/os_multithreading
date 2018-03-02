#include <iostream>
#include <cstdlib>
#include <pthread.h>
#include <stdlib.h>
#include <array>
#include <math.h>
#include <random>
#include <ctime>

using namespace std;

static int coords_in_circle = 0;

void *check_coordinate(void *t) {
  /*
  std::default_random_engine generator;
  std::uniform_int_distribution<int> distribution(0,1000);
  int r = distribution(generator);
  */
  int r = rand() % 1000;
  double x = (double)r/(double)1000;
  // cout << "x: " << x << endl;
  r = rand() % 1000;
  double y = (double)r/(double)1000;
  // cout << "y: " << y << endl;
  if(x <= cos(asin(y))) {
    coords_in_circle++;
  }
  return 0;
  /*
  if(x < cos(asin(y))) {
    cout << "true" << endl;
    return;
  }
  else {
    cout << "false" << endl;
    return;
  }
  */
}

int main(int argc, char *argv[]) {
  if(argc != 2) {
    cout << "Invalid command line arguments" << endl;
    return 1;
  }
  int n = atoi(argv[1]);
  pthread_t threads[n];
  srand(time(NULL));
  int i;
  int return_val;
  for(i = 0; i < n; i++) {
    return_val = pthread_create(&threads[i], NULL, check_coordinate, NULL);
    if(return_val) {
      cout << "\nerror, unable to create thread " << return_val << endl;
      exit(-1);
    }
  }
  double pi = (double)(coords_in_circle * 4)/ (double)n;
  cout << "With " << n << " coordinates, we estimate pi to be " << pi << endl;
  //check_coordinate();
  return 0;
}
