#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define MAX_VERTICES 100
#define INF INT_MAX

int numVertices; // Global variable to store the number of vertices

typedef struct
{
  int vertex;
  int weight;
} Edge;

typedef struct
{
  Edge *edges[MAX_VERTICES];
  int size[MAX_VERTICES];
  int numVertices;
} Graph;

typedef struct
{
  int vertex;
  int key;
} HeapNode;

typedef struct
{
  HeapNode *array[MAX_VERTICES];
  int size;
  int capacity;
} MinHeap;

// Function prototypes
Graph *createGraph(int numVertices);
void addEdge(Graph *graph, int src, int dest, int weight);
MinHeap *createMinHeap(int capacity);
void swapHeapNodes(HeapNode **a, HeapNode **b);
void minHeapify(MinHeap *minHeap, int idx);
HeapNode *extractMin(MinHeap *minHeap);
void decreaseKey(MinHeap *minHeap, int vertex, int key);
int isInMinHeap(MinHeap *minHeap, int vertex);
void printArr(int dist[], int n);
void dijkstra(Graph *graph, int src);
void primMST(Graph *graph, int src);
void readGraphFromFile(Graph **graph, const char *filename); // Updated to pass Graph**
void printGraph(Graph *graph);                               // Debug function
void freeGraph(Graph *graph);
void freeMinHeap(MinHeap *minHeap);

// Create a graph with a given number of vertices
Graph *createGraph(int numVertices)
{
  Graph *graph = (Graph *)malloc(sizeof(Graph));
  if (!graph)
  {
    perror("Failed to allocate memory for graph");
    exit(EXIT_FAILURE);
  }
  graph->numVertices = numVertices;
  for (int i = 0; i < numVertices; i++)
  {
    graph->edges[i] = (Edge *)malloc(MAX_VERTICES * sizeof(Edge));
    if (!graph->edges[i])
    {
      perror("Failed to allocate memory for edges");
      exit(EXIT_FAILURE);
    }
    graph->size[i] = 0;
  }
  return graph;
}

// Add an edge to the graph
void addEdge(Graph *graph, int src, int dest, int weight)
{
  if (src < 0 || src >= graph->numVertices || dest < 0 || dest >= graph->numVertices)
  {
    printf("Invalid edge: %d - %d (%d)\n", src, dest, weight);
    return;
  }

  // Add edge from src to dest (only once)
  graph->edges[src][graph->size[src]].vertex = dest;
  graph->edges[src][graph->size[src]].weight = weight;
  graph->size[src]++;

  // printf("Added edge: %d - %d (%d)\n", src, dest, weight); // Debug statement
}

// Create a min-heap with a given capacity
MinHeap *createMinHeap(int capacity)
{
  MinHeap *minHeap = (MinHeap *)malloc(sizeof(MinHeap));
  if (!minHeap)
  {
    perror("Failed to allocate memory for min-heap");
    exit(EXIT_FAILURE);
  }
  minHeap->size = 0;
  minHeap->capacity = capacity;
  return minHeap;
}

// Swap two heap nodes
void swapHeapNodes(HeapNode **a, HeapNode **b)
{
  HeapNode *t = *a;
  *a = *b;
  *b = t;
}

// Heapify the min-heap
void minHeapify(MinHeap *minHeap, int idx)
{
  int smallest = idx;
  int left = 2 * idx + 1;
  int right = 2 * idx + 2;

  if (left < minHeap->size && minHeap->array[left]->key < minHeap->array[smallest]->key)
    smallest = left;

  if (right < minHeap->size && minHeap->array[right]->key < minHeap->array[smallest]->key)
    smallest = right;

  if (smallest != idx)
  {
    swapHeapNodes(&minHeap->array[smallest], &minHeap->array[idx]);
    minHeapify(minHeap, smallest);
  }
}

// Extract the minimum node from the heap
HeapNode *extractMin(MinHeap *minHeap)
{
  if (minHeap->size == 0)
    return NULL;

  HeapNode *root = minHeap->array[0];
  HeapNode *lastNode = minHeap->array[minHeap->size - 1];
  minHeap->array[0] = lastNode;
  minHeap->size--;
  minHeapify(minHeap, 0);

  return root;
}

// Decrease the key of a vertex in the heap
void decreaseKey(MinHeap *minHeap, int vertex, int key)
{
  int i;
  for (i = 0; i < minHeap->size; i++)
  {
    if (minHeap->array[i]->vertex == vertex)
    {
      minHeap->array[i]->key = key;
      break;
    }
  }

  while (i && minHeap->array[i]->key < minHeap->array[(i - 1) / 2]->key)
  {
    swapHeapNodes(&minHeap->array[i], &minHeap->array[(i - 1) / 2]);
    i = (i - 1) / 2;
  }
}

// Check if a vertex is in the min-heap
int isInMinHeap(MinHeap *minHeap, int vertex)
{
  for (int i = 0; i < minHeap->size; i++)
  {
    if (minHeap->array[i]->vertex == vertex)
      return 1;
  }
  return 0;
}

// Print the distance array for Dijkstra's algorithm
void printArr(int dist[], int n)
{
  printf("Vertex   Distance from Source\n");
  for (int i = 0; i < n; ++i)
    printf("%d \t\t %d\n", i, dist[i]);
}

// Dijkstra's algorithm
void dijkstra(Graph *graph, int src)
{
  int V = graph->numVertices;
  int dist[V];

  MinHeap *minHeap = createMinHeap(V);

  for (int v = 0; v < V; ++v)
  {
    dist[v] = INF;
    minHeap->array[v] = (HeapNode *)malloc(sizeof(HeapNode));
    if (!minHeap->array[v])
    {
      perror("Failed to allocate memory for heap node");
      exit(EXIT_FAILURE);
    }
    minHeap->array[v]->vertex = v;
    minHeap->array[v]->key = dist[v];
    minHeap->size++;
  }

  dist[src] = 0;
  decreaseKey(minHeap, src, dist[src]);

  while (minHeap->size != 0)
  {
    HeapNode *minHeapNode = extractMin(minHeap);
    int u = minHeapNode->vertex;

    for (int i = 0; i < graph->size[u]; i++)
    {
      int v = graph->edges[u][i].vertex;
      int weight = graph->edges[u][i].weight;

      if (isInMinHeap(minHeap, v) && dist[u] != INF && dist[u] + weight < dist[v])
      {
        dist[v] = dist[u] + weight;
        decreaseKey(minHeap, v, dist[v]);
      }
    }
    free(minHeapNode); // Free the extracted node
  }

  printArr(dist, V);
  freeMinHeap(minHeap);
}

// Prim's algorithm with source vertex
void primMST(Graph *graph, int src)
{
  int V = graph->numVertices;
  int parent[V];
  int key[V];

  MinHeap *minHeap = createMinHeap(V);

  for (int v = 0; v < V; ++v)
  {
    parent[v] = -1;
    key[v] = INF;
    minHeap->array[v] = (HeapNode *)malloc(sizeof(HeapNode));
    if (!minHeap->array[v])
    {
      perror("Failed to allocate memory for heap node");
      exit(EXIT_FAILURE);
    }
    minHeap->array[v]->vertex = v;
    minHeap->array[v]->key = key[v];
    minHeap->size++;
  }

  key[src] = 0; // Start from the specified source vertex
  decreaseKey(minHeap, src, key[src]);

  while (minHeap->size != 0)
  {
    HeapNode *minHeapNode = extractMin(minHeap);
    int u = minHeapNode->vertex;

    for (int i = 0; i < graph->size[u]; i++)
    {
      int v = graph->edges[u][i].vertex;
      int weight = graph->edges[u][i].weight;

      if (isInMinHeap(minHeap, v) && weight < key[v])
      {
        key[v] = weight;
        parent[v] = u;
        decreaseKey(minHeap, v, key[v]);
      }
    }
    free(minHeapNode); // Free the extracted node
  }

  printf("Edge   Weight\n");
  for (int i = 0; i < V; ++i)
  {
    if (parent[i] != -1)
      printf("%d - %d    %d \n", parent[i], i, key[i]);
  }

  freeMinHeap(minHeap);
}

// Read graph from a file in the new format
void readGraphFromFile(Graph **graph, const char *filename)
{
  FILE *file = fopen(filename, "r");
  if (!file)
  {
    perror("Unable to open file");
    exit(EXIT_FAILURE);
  }

  // Read the number of vertices
  if (fscanf(file, "%d", &numVertices) != 1)
  {
    printf("Error reading number of vertices from file.\n");
    fclose(file);
    exit(EXIT_FAILURE);
  }
  printf("Number of vertices: %d\n", numVertices);

  *graph = createGraph(numVertices); // Create the graph after reading numVertices

  // Read each vertex and its edges
  for (int i = 0; i < numVertices; i++)
  {
    int vertex, numEdges;
    if (fscanf(file, "%d %d", &vertex, &numEdges) != 2)
    {
      printf("Error reading vertex or number of edges.\n");
      fclose(file);
      exit(EXIT_FAILURE);
    }

    for (int j = 0; j < numEdges; j++)
    {
      int dest, weight;
      if (fscanf(file, "%d %d", &dest, &weight) != 2)
      {
        printf("Error reading edge or weight.\n");
        fclose(file);
        exit(EXIT_FAILURE);
      }
      addEdge(*graph, vertex, dest, weight);
    }
  }

  fclose(file);
  printf("Finished reading graph.\n");
}

// Print the graph adjacency list
void printGraph(Graph *graph)
{
  printf("Graph adjacency list:\n");
  for (int i = 0; i < graph->numVertices; i++)
  {
    printf("Vertex %d: ", i);
    for (int j = 0; j < graph->size[i]; j++)
    {
      printf("(%d, %d) ", graph->edges[i][j].vertex, graph->edges[i][j].weight);
    }
    printf("\n");
  }
}

// Free memory allocated for the graph
void freeGraph(Graph *graph)
{
  for (int i = 0; i < graph->numVertices; i++)
  {
    free(graph->edges[i]);
  }
  free(graph);
}

// Free memory allocated for the min-heap
void freeMinHeap(MinHeap *minHeap)
{
  for (int i = 0; i < minHeap->size; i++)
  {
    free(minHeap->array[i]);
  }
  free(minHeap);
}

int main()
{
  Graph *graph = NULL; // Initialize graph to NULL

  readGraphFromFile(&graph, "graph.txt"); // Pass the address of graph
  printGraph(graph);                      // Debug statement

  int choice;
  printf("1. Prim's Algorithm\n");
  printf("2. Dijkstra's Algorithm\n");
  printf("Enter your choice: ");
  scanf("%d", &choice);

  switch (choice)
  {
  case 1:
  {
    int src;
    printf("Enter the source vertex for Prim's algorithm: ");
    scanf("%d", &src);
    if (src < 0 || src >= numVertices)
    {
      printf("Invalid source vertex. Please enter a value between 0 and %d.\n", numVertices - 1);
      break;
    }
    primMST(graph, src); // Pass the source vertex to Prim's algorithm
    break;
  }
  case 2:
  {
    int src;
    printf("Enter the source vertex for Dijkstra's algorithm: ");
    scanf("%d", &src);
    if (src < 0 || src >= numVertices)
    {
      printf("Invalid source vertex. Please enter a value between 0 and %d.\n", numVertices - 1);
      break;
    }
    dijkstra(graph, src);
    break;
  }
  default:
    printf("Invalid choice\n");
  }

  freeGraph(graph);
  return 0;
}