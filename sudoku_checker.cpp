#include <iostream>
#include <cstdlib>
#include <pthread.h>
#include <stdlib.h>
#include <array>

using namespace std;

#define NUM_THREADS 27

static bool func_returns = true;

struct arg_struct {
   std::array<int, 81>* grid;
   int start_index;
};

void *check_square(void *arguments) {
  //cout << "\ncheck square called" << endl;
  struct arg_struct *args = (struct arg_struct *)arguments;

  std::array<bool, 9> test_vals = {false, false, false, false, false, false, false, false, false};

  int cur_index = args->start_index;
  int i;
  int j;
  // iterates through index in square
  for(i = 1; i < 10; i++) {
    // iterates through number 1 - 9
    for(j = 1; j < 10; j++) {
      if(args->grid->at(cur_index) == j) {
        test_vals[j - 1] = true;
        break;
      }
    }
    if(i % 3 == 0) {
      cur_index += 7;
    } else {
      cur_index++;
    }
  }

  bool return_bool = true;
  for(i = 0; i < 9; i++) {
    return_bool = return_bool && test_vals[i];
  }

  if (!func_returns){
    return 0;
  } else if (func_returns && !return_bool) {
    func_returns = false;
    cout << "\ncheck_square fail, func_returns set to false" << endl;
    return 0;
  } else {
    return 0;
  }
  //pthread_exit(NULL);
}

void *check_row(void *arguments) {
  //cout << "\ncheck row called" << endl;
  struct arg_struct *args = (struct arg_struct *)arguments;

  std::array<bool, 9> test_vals = {false, false, false, false, false, false, false, false, false};

  int cur_index = args->start_index;
  int i;
  int j;
  // iterates through index in square
  for(i = 1; i < 10; i++) {
    // iterates through number 1 - 9
    for(j = 1; j < 10; j++) {
      if(args->grid->at(cur_index) == j) {
        test_vals[j - 1] = true;
        break;
      }
    }
    cur_index++;
  }

  bool return_bool = true;
  for(i = 0; i < 9; i++) {
    return_bool = return_bool && test_vals[i];
  }

  if (!func_returns){
    return 0;
  } else if (func_returns && !return_bool) {
    func_returns = false;
    cout << "\ncheck_row fail, func_returns set to false" << endl;
    return 0;
  } else {
    return 0;
  }
  //pthread_exit(NULL);
}

void *check_column(void *arguments) {
  //cout << "\ncheck column called" << endl;
  struct arg_struct *args = (struct arg_struct *)arguments;

  std::array<bool, 9> test_vals = {false, false, false, false, false, false, false, false, false};

  int cur_index = args->start_index;
  int i;
  int j;
  // iterates through index in square
  for(i = 1; i < 10; i++) {
    // iterates through number 1 - 9
    for(j = 1; j < 10; j++) {
      if(args->grid->at(cur_index) == j) {
        test_vals[j - 1] = true;
        break;
      }
    }
    cur_index += 9;
  }

  bool return_bool = true;
  for(i = 0; i < 9; i++) {
    return_bool = return_bool && test_vals[i];
  }

  if (!func_returns){
    return 0;
  } else if (func_returns && !return_bool) {
    func_returns = false;
    cout << "\ncheck_column fail, func_returns set to false" << endl;
    return 0;
  } else {
    return 0;
  }
  //pthread_exit(NULL);
}

bool check_sudoku_grid(std::array<int, 81>* input_grid) {
  pthread_t threads[NUM_THREADS];

  std::array<int, 9> ssi = {0,3,6,27,30,33,54,57,60};
  int return_val;
  int i;
  for(i = 0; i < NUM_THREADS; i++) {
    //cout << "\nmain() : creating thread " << i << endl;
    // checks all the columns
    if(i < 9){
      struct arg_struct temp;
      temp.start_index = i;
      temp.grid = input_grid;
      return_val = pthread_create(&threads[i], NULL, check_column, (void *)&temp);
    } else if(i >= 9 && i < 18) {
      struct arg_struct temp;
      temp.start_index = (i - 9) * 9;
      temp.grid = input_grid;
      return_val = pthread_create(&threads[i], NULL, check_row, (void *)&temp);
    } else {
      struct arg_struct temp;
      temp.start_index = ssi[i - 18];
      temp.grid = input_grid;
      return_val = pthread_create(&threads[i], NULL, check_square, (void *)&temp);
    }

    if(return_val) {
      cout << "\nerror, unable to create thread " << return_val << endl;
      exit(-1);
    }
  }

  // pthread_exit(NULL);
  // cout << "\nexiting threads" << endl;
  // cout << "func_returns: " << func_returns << endl;
  return_val = func_returns;
  func_returns = true;
  return return_val;
}

int main() {
  std::array<int, 81> test_pass = {1,2,3,4,5,6,7,8,9,
                                   4,5,6,7,8,9,1,2,3,
                                   7,8,9,1,2,3,4,5,6,
                                   2,3,1,5,6,4,8,9,7,
                                   5,6,4,8,9,7,2,3,1,
                                   8,9,7,2,3,1,5,6,4,
                                   3,1,2,6,4,5,9,7,8,
                                   6,4,5,9,7,8,3,1,2,
                                   9,7,8,3,1,2,6,4,5};

  std::array<int, 81> test_fail = {1,2,3,4,5,6,7,8,9,
                                   4,5,6,7,8,9,1,2,3,
                                   7,8,9,1,2,3,4,5,6,
                                   2,3,1,5,6,4,8,9,7,
                                   5,6,4,8,9,7,2,3,1,
                                   8,9,7,2,3,1,5,6,4,
                                   3,1,2,6,4,5,9,7,8,
                                   6,4,5,9,7,8,3,1,2,
                                   9,7,8,3,1,2,6,4,4};
  bool pass = check_sudoku_grid(&test_pass);

  if (pass) {
    cout << "\nPass test valid" << endl;
  } else {
    cout << "\nPass test failed :(" << endl;
  }

  bool fail = check_sudoku_grid(&test_fail);
  if (fail) {
    cout << "\nFail test valid :(" << endl;
  } else {
    cout << "\nFail test failed" << endl;
  }
}
