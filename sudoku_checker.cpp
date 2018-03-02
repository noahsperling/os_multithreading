#include <iostream>
#include <cstdlib>
#include <pthread.h>
#include <stdlib.h>
#include <array>

/*
* @authors Noah Sperling and Devin Ajimine
* @version 3/2/2018
*/

using namespace std;

#define NUM_THREADS 27

static bool func_returns = true;

// contains args for verification functions
struct arg_struct {
  // sudoku grid as a one dimensional array
  std::array<int, 81>* grid;
  // start location in grid to begin checking
  int start_index;
};

// checks a a 3x3 grid to see if it contains numbers 1-9
void *check_square(void *arguments) {
  // cast arguments to an arg_struct
  struct arg_struct *args = (struct arg_struct *)arguments;

  // represents whether or not numbers 1-9 have been found
  std::array<bool, 9> test_vals = {false, false, false, false, false, false, false, false, false};

  // index for use during loops
  int cur_index = args->start_index;

  // iterators
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
    // jumps to next row in square
    if(i % 3 == 0) {
      cur_index += 7;
    // moves to next index in square
    } else {
      cur_index++;
    }
  }

  // determines if the square is valid
  bool return_bool = true;
  for(i = 0; i < 9; i++) {
    return_bool = return_bool && test_vals[i];
  }

  // updates the overall validity of sudoku grid
  if (!func_returns){
    return 0;
  } else if (func_returns && !return_bool) {
    func_returns = false;
    cout << "\ncheck_square fail, func_returns set to false" << endl;
    return 0;
  } else {
    return 0;
  }
}

// checks a row from a start position to see if it contains the numbers 1 - 9
void *check_row(void *arguments) {
  // cast arguments to an arg_struct
  struct arg_struct *args = (struct arg_struct *)arguments;

  // represents whether or not numbers 1-9 have been found
  std::array<bool, 9> test_vals = {false, false, false, false, false, false, false, false, false};

  // index for use during loops
  int cur_index = args->start_index;

  // iterators
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
    // moves to next index in row
    cur_index++;
  }

  // determines if the row is valid
  bool return_bool = true;
  for(i = 0; i < 9; i++) {
    return_bool = return_bool && test_vals[i];
  }

  // updates the overall validity of sudoku grid
  if (!func_returns){
    return 0;
  } else if (func_returns && !return_bool) {
    func_returns = false;
    cout << "\ncheck_row fail, func_returns set to false" << endl;
    return 0;
  } else {
    return 0;
  }
}

// checks a column from a start position to see if it contains the numbers 1 - 9
void *check_column(void *arguments) {
  // casts arguments to an arg_struct
  struct arg_struct *args = (struct arg_struct *)arguments;

  // represents whether or not numbers 1-9 have been found
  std::array<bool, 9> test_vals = {false, false, false, false, false, false, false, false, false};

  // index for use during loops
  int cur_index = args->start_index;

  // iterators
  int i;
  int j;

  // iterates through index in column
  for(i = 1; i < 10; i++) {
    // iterates through number 1 - 9
    for(j = 1; j < 10; j++) {
      if(args->grid->at(cur_index) == j) {
        // marks which number is found in temp list
        test_vals[j - 1] = true;
        break;
      }
    }
    // jumps to next number in column
    cur_index += 9;
  }

  // determines if the column is valid
  bool return_bool = true;
  for(i = 0; i < 9; i++) {
    return_bool = return_bool && test_vals[i];
  }

  // updates the overall validity of sudoku grid
  if (!func_returns){
    return 0;
  } else if (func_returns && !return_bool) {
    func_returns = false;
    cout << "\ncheck_column fail, func_returns set to false" << endl;
    return 0;
  } else {
    return 0;
  }
}

bool check_sudoku_grid(std::array<int, 81>* input_grid) {
  pthread_t threads[NUM_THREADS];

  // start indices for rows
  std::array<int, 9> ssi = {0,3,6,27,30,33,54,57,60};
  int return_val;
  int i;
  for(i = 0; i < NUM_THREADS; i++) {
    // checks columns
    if(i < 9){
      struct arg_struct temp;
      temp.start_index = i;
      temp.grid = input_grid;
      return_val = pthread_create(&threads[i], NULL, check_column, (void *)&temp);
    // checks rows
    } else if(i >= 9 && i < 18) {
      struct arg_struct temp;
      temp.start_index = (i - 9) * 9;
      temp.grid = input_grid;
      return_val = pthread_create(&threads[i], NULL, check_row, (void *)&temp);
    // checks squares
    } else {
      struct arg_struct temp;
      temp.start_index = ssi[i - 18];
      temp.grid = input_grid;
      return_val = pthread_create(&threads[i], NULL, check_square, (void *)&temp);
    }
    // if there is an issue creating a thread, exit the program
    if(return_val) {
      cout << "\nerror, unable to create thread " << return_val << endl;
      exit(-1);
    }
  }

  // stores whether or not the sudoku grid is valid in a new variable
  return_val = func_returns;

  // resets value for next run
  func_returns = true;

  //returns whether or not the sudoku grid is valid
  return return_val;
}

int main() {
  // a valid sudoku array
  std::array<int, 81> test_pass = {1,2,3,4,5,6,7,8,9,
                                   4,5,6,7,8,9,1,2,3,
                                   7,8,9,1,2,3,4,5,6,
                                   2,3,1,5,6,4,8,9,7,
                                   5,6,4,8,9,7,2,3,1,
                                   8,9,7,2,3,1,5,6,4,
                                   3,1,2,6,4,5,9,7,8,
                                   6,4,5,9,7,8,3,1,2,
                                   9,7,8,3,1,2,6,4,5};

  // an invalid sudokiu array
  std::array<int, 81> test_fail = {1,2,3,4,5,6,7,8,9,
                                   4,5,6,7,8,9,1,2,3,
                                   7,8,9,1,2,3,4,5,6,
                                   2,3,1,5,6,4,8,9,7,
                                   5,6,4,8,9,7,2,3,1,
                                   8,9,7,2,3,1,5,6,4,
                                   3,1,2,6,4,5,9,7,8,
                                   6,4,5,9,7,8,3,1,2,
                                   9,7,8,3,1,2,6,4,4};

  // valid sudoku grid test case
  bool pass = check_sudoku_grid(&test_pass);

  if (pass) {
    cout << "\nPass test valid" << endl;
  } else {
    cout << "\nPass test failed :(" << endl;
  }

  // valid sudoku grid test case
  bool fail = check_sudoku_grid(&test_fail);

  if (fail) {
    cout << "\nFail test valid :(" << endl;
  } else {
    cout << "\nFail test failed" << endl;
  }
}
