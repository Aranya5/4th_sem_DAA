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
    else if (q > end)
    {
      tempArr[k++] = arr[p++];
    }
    else if (arr[p] < arr[q]) // Smaller one gets inserted
    {
      tempArr[k++] = arr[p++];
    }
    else
    {
      tempArr[k++] = arr[q++];
    }
  }
  // Copy it to the real array
  for (i = 0; i < k; i++)
  {
    arr[start++] = tempArr[i];
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

void printArray(int arr[], int size)
{
  for (int i = 0; i < size; i++)
  {
    printf("%d ", arr[i]);
  }
  printf("\n");
}

int main()
{
  int c = 0;
  int A[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
  int B[] = {10, 9, 8, 7, 6, 5, 4, 3, 2, 1};
  int C[] = {10, 7, 5, 8, 2, 9, 1, 6, 3, 4};

  printf("Unsorted A: ");
  printArray(A, 10);
  mergeSort(A, 0, 9, &c);
  printf("Sorted A: ");
  printArray(A, 10);
  printf("Number of comparisons for ascending array: %d\n", c);
  c = 0;

  printf("\nUnsorted B: ");
  printArray(B, 10);
  mergeSort(B, 0, 9, &c);
  printf("Sorted B: ");
  printArray(B, 10);
  printf("Number of comparisons for descending array: %d\n", c);
  c = 0;

  printf("\nUnsorted C: ");
  printArray(C, 10);
  mergeSort(C, 0, 9, &c);
  printf("Sorted C: ");
  printArray(C, 10);
  printf("Number of comparisons for random array: %d\n", c);

  return 0;
}