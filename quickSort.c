#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h> // Include for memcpy

void swap(int *a, int *b)
{
  int temp = *a;
  *a = *b;
  *b = temp;
}

int partition(int arr[], int low, int high, int *count)
{
  int pivot = arr[high];
  int j, i = low - 1;
  // smaller elemnt goes to the left of the pivot
  for (j = low; j < high; j++)
  {
    *count = *count + 1;
    if (arr[j] <= pivot)
    {
      i++;
      swap(&arr[i], &arr[j]);
    }
  }
  swap(&arr[i + 1], &arr[high]);
  return i + 1;
}

int partition_first(int arr[], int low, int high, int *count)
{
  swap(&arr[low], &arr[high]);
  return partition(arr, low, high, count);
}

int partition_mid(int arr[], int low, int high, int *count)
{
  int mid = low + ((high - low) / 2);
  swap(&arr[mid], &arr[high]);
  return partition(arr, low, high, count);
}

int partition_rand(int arr[], int low, int high, int *count)
{
  int ran = low + rand() % (high - low + 1);
  swap(&arr[ran], &arr[high]);
  return partition(arr, low, high, count);
}

void quickSort(int arr[], int low, int high, int pivotType, int *count)
{
  if (low < high)
  {
    int pi;
    switch (pivotType)
    {
    case 1:
      pi = partition_first(arr, low, high, count);
      break;
    case 2:
      pi = partition_mid(arr, low, high, count);
      break;
    case 3:
      pi = partition_rand(arr, low, high, count);
      break;
    case 4:
      pi = partition(arr, low, high, count);
      break;
    default:
      pi = partition(arr, low, high, count);
      break;
    }

    quickSort(arr, low, pi - 1, pivotType, count);
    quickSort(arr, pi + 1, high, pivotType, count);
  }
}

int main()
{
  srand(time(NULL)); // Initialize random seed
  int c = 0;
  int A[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
  int B[] = {10, 9, 8, 7, 6, 5, 4, 3, 2, 1};
  int C[] = {10, 7, 5, 8, 2, 9, 1, 6, 3, 4};
  int temp[10]; // Temporary array for copying

  printf("Pivot at First\t\tPivot at Mid\t\tPivot at Rand\t\tPivot at Last\n");
  printf("\nFor accending array\n");
  for (int i = 1; i <= 4; i++)
  {
    memcpy(temp, A, sizeof(A)); // Copy array A to temp(we need to copy the array because acceccing the first time will make it sorted already)
    quickSort(temp, 0, 9, i, &c);

    printf("Comparision :%d \t", c);
    c = 0;
  }

  printf("\nFor decending array\n");
  for (int i = 1; i <= 4; i++)
  {
    memcpy(temp, B, sizeof(B)); // Copy array B to temp
    quickSort(temp, 0, 9, i, &c);
    printf("Comparision :%d \t", c);
    c = 0;
  }

  printf("\nFor random array\n");
  for (int i = 1; i <= 4; i++)
  {
    memcpy(temp, C, sizeof(C)); // Copy array C to temp
    quickSort(temp, 0, 9, i, &c);
    printf("Comparision :%d \t", c);
    c = 0;
  }

  return 0;
}
