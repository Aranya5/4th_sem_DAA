#include <stdio.h>
#include <limits.h>

#define MAX 100

// Function to print the matrix of minimum multiplications
void printTable(int n, int m[MAX][MAX])
{
  printf("Matrix of minimum multiplications:\n");
  for (int i = 1; i < n; i++)
  {
    for (int j = 1; j < n; j++)
    {
      if (i > j)
        printf("   -   ");
      else
        printf("%6d ", m[i][j]);
    }
    printf("\n");
  }
}

// Recursive function to print the optimal parenthesization
void printOptimalParens(int i, int j, int s[MAX][MAX])
{
  if (i == j)
  {
    printf("A%d", i);
  }
  else
  {
    printf("(");
    printOptimalParens(i, s[i][j], s);
    printOptimalParens(s[i][j] + 1, j, s);
    printf(")");
  }
}

// Function to calculate minimum multiplications and parenthesization
int matrixChainOrder(int p[], int n)
{
  int m[MAX][MAX]; // matrix for cost
  int s[MAX][MAX]; // matrix for parenthesis split points

  // Initialize diagonal to 0 (cost is zero when multiplying one matrix)
  for (int i = 1; i < n; i++)
    m[i][i] = 0;

  // l is chain length
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
        {
          m[i][j] = cost;
          s[i][j] = k;
        }
      }
    }
  }

  printTable(n, m);
  printf("Optimal Parenthesization: ");
  printOptimalParens(1, n - 1, s);
  printf("\n");

  return m[1][n - 1];
}

int main()
{
  int arr[] = {30, 35, 15, 5, 10, 20, 25};
  int size = sizeof(arr) / sizeof(arr[0]);

  int minMultiplications = matrixChainOrder(arr, size);
  printf("Minimum number of multiplications is %d\n", minMultiplications);
  return 0;
}