#include <iostream>
#include <cstdlib>
#include <pthread.h>
#include <stdlib.h>
#include <array>
#include <math.h>
#include <random>

using namespace std;

void check_coordinate() {
  std::default_random_engine generator;
  std::uniform_int_distribution<int> distribution(0,1000);
  int r = distribution(generator);
  double x = (double)r/(double)1000;
  cout << "x: " << x << endl;
  r = distribution(generator);
  double y = (double)r/(double)1000;
  cout << "y: " << y << endl;
  if(x < cos(asin(y))) {
    cout << "true" << endl;
    return;
  }
  else {
    cout << "false" << endl;
    return;
  }
}

int main(int argc, char *argv[]) {
  if(argc != 2) {
    cout << "Invalid command line arguments" << endl;
    return 1;
  }
  int n = atoi(argv[1]);
  check_coordinate();
  return 0;
}
