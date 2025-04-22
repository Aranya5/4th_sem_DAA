// Matrix Chain Multiplication Problem
#include <stdio.h>
#include <limits.h>

// Function to find the minimum number of multiplications
int matrixChainMultiplication(int p[], int n)
{
  int m[n][n];

  // cost is zero when multiplying one matrix
  for (int i = 1; i < n; i++)
    m[i][i] = 0;

  // l is the chain length
  for (int l = 2; l < n; l++)
  {
    for (int i = 1; i < n - l + 1; i++)
    {
      int j = i + l - 1;
      m[i][j] = INT_MAX;

      for (int k = i; k < j; k++)
      {
        int cost = m[i][k] + m[k + 1][j] + p[i - 1] * p[k] * p[j];
        if (cost < m[i][j])
          m[i][j] = cost;
      }
    }
  }

  return m[1][n - 1];
}

int main()
{
  // Example dimensions: matrices of sizes 10x30, 30x5, 5x60
  int dimensions[] = {10, 30, 5, 60};
  int n = sizeof(dimensions) / sizeof(dimensions[0]);

  int minMultiplications = matrixChainMultiplication(dimensions, n);
  printf("Minimum number of multiplications: %d\n", minMultiplications);

  return 0;
}