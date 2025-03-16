#include <stdio.h>

void merge(int arr[], int start, int mid, int end, int *count)
{
  int p = start, q = mid + 1;
  int tempArr[end - start + 1], k = 0, i;

  for (i = start; i <= end; i++)
  {
    (*count)++;
    if (p > mid) // Extra element solution
    {
      tempArr[k++] = arr[q++];
    }
    else if (q > mid)
    {
      tempArr[k++] = arr[p++];
    }
    else if (arr[p] < arr[q]) // Smaller one gets inserted
    {
      tempArr[k++] = tempArr[p++];
    }
    else
    {
      tempArr[k++] = tempArr[q++];
    }
  }
  // Copy it the real array
  for (i = 0; i < k; i++)
  {
    arr[start++] = tempArr[i++];
  }
}

void mergeSort(int arr[], int start, int end, int *count)
{
  if (start < end)
  {
    int mid = (start + end) / 2;
    // Divide
    mergeSort(arr, start, mid, count);
    mergeSort(arr, mid + 1, end, count);
    // Conquer
    merge(arr, start, mid, end, count);
  }
}

int main()
{
  int c = 0;
  int A[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
  int B[] = {10, 9, 8, 7, 6, 5, 4, 3, 2, 1};
  int C[] = {10, 7, 5, 8, 2, 9, 1, 6, 3, 4};
  int temp[10];

  mergeSort(A, 0, 9, &c);
  printf("Number of comparision for accending array:%d\n", c);
  c = 0;

  mergeSort(B, 0, 9, &c);
  printf("Number of comparision for descending array:%d\n", c);
  c = 0;

  mergeSort(C, 0, 9, &c);
  printf("Number of comparision for accending array:%d\n", c);
  return 0;
}