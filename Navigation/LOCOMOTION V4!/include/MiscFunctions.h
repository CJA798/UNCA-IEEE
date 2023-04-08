#include <Arduino.h>
#include <macros.h>
void MatrixMultiply(mtx_type *A, mtx_type *B, int m, int p, int n, mtx_type *C)
  {
    // A = input matrix (m x p)
    // B = input matrix (p x n)
    // m = number of rows in A
    // p = number of columns in A = number of rows in B
    // n = number of columns in B
    // C = output matrix = A*B (m x n)
    int i, j, k;
    for (i = 0; i < m; i++)
      for (j = 0; j < n; j++)
      {
        C[n * i + j] = 0;
        for (k = 0; k < p; k++)
          C[n * i + j] = C[n * i + j] + A[p * i + k] * B[n * k + j];
      }
  }

  void PrintMatrix(mtx_type *A, int m, int n)
  {
    // A = input matrix (m x n)
    // m = number of rows in A
    // n = number of columns in A
    int i, j;
    for (i = 0; i < m; i++)
    {
      for (j = 0; j < n; j++)
        Serial.print(A[n * i + j], 4);
      Serial.print(" ");
      Serial.println();
    }
  }