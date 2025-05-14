#include <stdio.h>

#include <stdlib.h>

#define INF 99999999

#define MAX 100

int dist[MAX][MAX];

int next[MAX][MAX];

int pred[MAX][MAX];

int vertices;

// Print shortest path from u to v

void printPath(int u, int v)
{

  if (next[u][v] == -1)
  {

    printf("No path\n");

    return;
  }

  printf("Path: %d", u);

  while (u != v)
  {

    u = next[u][v];

    printf(" -> %d", u);
  }

  printf("\n");
}

// Floyd-Warshall algorithm

void floydWarshall()
{

  for (int k = 0; k < vertices; k++)
  {

    for (int i = 0; i < vertices; i++)
    {

      for (int j = 0; j < vertices; j++)
      {

        if (dist[i][k] + dist[k][j] < dist[i][j])
        {

          dist[i][j] = dist[i][k] + dist[k][j];

          next[i][j] = next[i][k];

          pred[i][j] = pred[k][j];
        }
      }
    }
  }
}

// Print distance matrix

void printDistances()
{

  printf("\nShortest Distance Matrix:\n");

  for (int i = 0; i < vertices; i++)
  {

    for (int j = 0; j < vertices; j++)
    {

      if (dist[i][j] == INF)

        printf("INF\t");

      else

        printf("%d\t", dist[i][j]);
    }

    printf("\n");
  }
}

// Print predecessor matrix

void printPredecessors()
{

  printf("\nPredecessor Matrix (π[i][j]):\n");

  for (int i = 0; i < vertices; i++)
  {

    for (int j = 0; j < vertices; j++)
    {

      if (pred[i][j] == -1)

        printf("nil\t");

      else

        printf("%d\t", pred[i][j]);
    }

    printf("\n");
  }
}

int main()
{

  FILE *file = fopen("graph_all.txt", "r");

  if (!file)
  {

    printf("Error opening file.\n");

    return 1;
  }

  fscanf(file, "%d", &vertices);

  // Initialize matrices

  for (int i = 0; i < vertices; i++)
  {

    for (int j = 0; j < vertices; j++)
    {

      if (i == j)

        dist[i][j] = 0;

      else

        dist[i][j] = INF;

      next[i][j] = -1;

      pred[i][j] = -1;
    }
  }

  // Read edges

  for (int i = 0; i < vertices; i++)
  {

    int v, numEdges;

    fscanf(file, "%d %d", &v, &numEdges);

    for (int j = 0; j < numEdges; j++)
    {

      int to, weight;

      fscanf(file, "%d %d", &to, &weight);

      dist[v][to] = weight;

      next[v][to] = to;

      pred[v][to] = v;
    }
  }

  fclose(file);

  floydWarshall();

  printDistances();

  printPredecessors();

   printf("\nShortest Paths:\n");
  printf("Enter the source vertex: ");
  int source;
  scanf("%d", &source);
  printf("Enter the destination vertex: ");
  int destination;
  scanf("%d", &destination);
  printf("From %d to %d: ", source, destination);
  if (source != destination)
  {

    printf("From %d to %d: ", source, destination);

    if (dist[source][destination] == INF)

      printf("No path\n");

    else
    {

      printf("Cost = %d, ", dist[source][destination]);

      printPath(source, destination);
    }
  }

  return 0;
}