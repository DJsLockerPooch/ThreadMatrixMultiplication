#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "thread.h"

/**
*    > gcc -g -Wall -Werror -D_POSIX_THREAD_SEMANTICS ThreadMultiply.c -c -o threadMultiply.o
*    > gcc -g -Wall -Werror -D_POSIX_THREAD_SEMANTICS thread.c -c -o thread.o
*    > gcc -pthread threadMultiply.o thread.o -o threadMultiply
*/

#define MAXSIZE 200                                                       // The size of the starting array
#define stringSize 36                                                     // The max size of string we take in
void readArray(char *fileName, int matrixNum);                            // Function to read from file fileName into matrix matrixNum
void multiplyArray(int row);                                              // Function to multiply our global matrices at row "row"
void printArrays();                                                       // Function to print all the arrays

int matrix1[MAXSIZE][MAXSIZE];                                            // The first matrix from the first file
int matrix1d[2];                                                          // Dimensions of the first matrix
int matrix2[MAXSIZE][MAXSIZE];                                            // The second matrix from the second file
int matrix2d[2];                                                          // Dimensions of the second matrix
int matrixAns[MAXSIZE][MAXSIZE];                                          // The answer matrix
int matrixAnsd[2];                                                        // Dimensions of the answer matrix
char file1[stringSize];                                                   // The array that will hold the first file name
char file2[stringSize];                                                   // The array that will hold the first file name
thread_t threads[MAXSIZE];                                                // The array of threads (At most will be of size MAXSIZE)

int main(){
  printf("Please enter the first matrix text file to multiply.\n>>> ");   // Prompting for first matrix
  scanf("%s", file1);                                                     // Getting the file name the user entered
  readArray(file1, 1);                                                    // Reading the array from the file into our "1" matrix (matrix1)

  printf("Please enter the second matrix text file to multiply.\n>>> ");  // Prompting for second matrix
  scanf("%s", file2);                                                     // Getting the second file and placing it in file2
  readArray(file2, 2);                                                    // Reading the array from the file into our "2" matrix (matrix2)

  if (matrix1d[1] == matrix2d[0]){                                        // Making sure our arrays dimensions are correct to multiply
    for (int i = 0; i < matrix1d[0]; i++){                                // For as many rows in our first matrix
      thread_create(&(threads[i]), &multiplyArray, i);                    // Create a thread with the multiplyArray function, and the i'th row
    }
    for (int i = 0; i < matrix1d[0]; i++){                                // For as many rows in our first matrix
      thread_join(threads[i]);                                            // Join the threads, ensuring they all finish before we finish the program
    }
  } else{                                                                 // If the dimensions don't line up
    printf("Sorry the dimensions of the matrices you entered didn't match up. Please try again.\n");
    printf("The dimensions you entered were: (%d, %d) and (%d, %d)\n", matrix1d[0],matrix1d[1], matrix2d[0], matrix2d[1]);
    main();
  }
  printf("\nMultiplication complete!\n");                                 // Completion messgae
  FILE *f = fopen("result.txt", "w");                                     // "Opening" or creating a result.txt file with write privilleges
  for (int i = 0; i < matrixAnsd[0]; i++){                                // For as many rows in our answer matrix
    for (int j = 0; j < matrixAnsd[1]; j++){                              // For as many cols in our answer matrix
      fprintf(f, "%d ", matrixAns[i][j]);                                 // Print into our result.txt file the value at our i'th row and j'th col
    }
    fprintf(f, "\n");                                                     // After the line, print a new line
  }
  fclose(f);                                                              // Close the result.txt file
  printArrays();
  return 1;                                                               // Exit successful
}

void readArray(char *fileName, int matrixNum){
  FILE *file;
  file = fopen(fileName, "r");                                            // Opening the fileName file
  int x = 0, y = 0;                                                       // The rows (x) and cols (y) values
  char line[MAXSIZE * 2];                                                 // Double the allowed size of a row of the matrix to account for whitespace
  while(fgets(line, MAXSIZE * 2, file) != NULL) {                         // While there is something
    x++;                                                                  // Incriment our rows
    y = 0;                                                                // Reset our cols
    char *token;                                                          // Create our token variable
    token = strtok(line, " \r\n");                                        // Get the first token
    while (token != NULL) {                                               // While the token is not NULL
      y++;                                                                // Incriment our cols
      if (matrixNum == 1){                                                // If we are inserting into our fist matrix
        matrix1[x - 1][y - 1] = atoi(token);                              // Insert the values into matrix1
      } else {                                                            // Else we are inserting into our second matrix
        matrix2[x - 1][y - 1] = atoi(token);                              // Insert the values into matrix2
      }
      token = strtok(NULL, " \r\n");                                      // Get the next token
    }
  }
  if (matrixNum == 1){                                                    // If we inserted into the first matrix
    matrix1d[0] = x;                                                      // Set the row dimension to x
    matrix1d[1] = y;                                                      // Set the col dimension to y
  } else {                                                                // Else we inserted into the second matrix
    matrix2d[0] = x;                                                      // Set the row dimension to x
    matrix2d[1] = y;                                                      // Set the col dimension to y
  }
  matrixAnsd[0] = matrix1d[0];                                            // Setting the answer matrix row dimension to the first matrix row dimension
  matrixAnsd[1] = matrix2d[1];                                            // Setting the answer matrix col dimension to the second matrix col dimension
}

void multiplyArray(int row){
  for (int i = 0; i < matrix2d[1]; i++){                                  // As many cols in the second matrix (the number of cols in the answer matrix)
    for (int j = 0; j < matrix2d[0]; j++){                                // As many rows in the second matrix (the number of additions/spot)
      matrixAns[row][i] += (matrix1[row][j] * matrix2[j][i]);             // Adding the multiplication to our answer matrix
    }
  }
}

void printArrays(){
  printf("Matrix 1:\n");
  for (int i=0; i < matrix1d[0]; i++) {
    for (int j=0; j < matrix1d[1]; j++){
      printf("%d ", matrix1[i][j]);
    }
    printf("\n");
  }
  printf("Matrix 2:\n");
  for (int i=0; i < matrix2d[0]; i++) {
    for (int j=0; j < matrix2d[1]; j++){
      printf("%d ", matrix2[i][j]);
    }
    printf("\n");
  }
  printf("Answer Matrix:\n");
  for (int i=0; i < matrixAnsd[0]; i++) {
    for (int j=0; j < matrixAnsd[1]; j++){
      printf("%d ", matrixAns[i][j]);
    }
    printf("\n");
  }
}
