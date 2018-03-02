all: sudoku_checker calculate_pi

program1: sudoku_checker.cpp
	g++ -o sudoku_checker sudoku_checker.c

program2: calculate_pi.cpp
	g++ -o calculate_pi calculate_pi.cpp
