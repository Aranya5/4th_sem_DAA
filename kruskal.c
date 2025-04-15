#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX_VERTICES 100
#define MAX_EDGES 100

// Node structure for Disjoint Set (Union-Find)
typedef struct Node
{
  int data;
  struct Node *next;
  struct Node *representative;
  int rank;
} Node;

// Edge structure
typedef struct Edge
{
  int src, dest, weight;
} Edge;

// Adjacency List Node
typedef struct AdjNode
{
  int dest, weight;
  struct AdjNode *next;
} AdjNode;

// Adjacency List
typedef struct
{
  AdjNode *head;
} AdjList;

AdjList graph[MAX_VERTICES]; // Graph as adjacency list

// Function to create a new set with a single element
Node *makeSet(int data)
{
  Node *newNode = (Node *)malloc(sizeof(Node));
  newNode->data = data;
  newNode->next = newNode;
  newNode->representative = newNode;
  newNode->rank = 0;
  return newNode;
}

// Find operation with Path Compression
Node *find(Node *node)
{
  if (node->representative != node)
  {
    node->representative = find(node->representative);
  }
  return node->representative;
}

// Function to print a set
void printSet(Node *rep)
{
  Node *temp = rep;
  printf("{ ");
  do
  {
    printf("%d ", temp->data);
    temp = temp->next;
  } while (temp != rep);
  printf("}\n");
}

// Union by Rank and Merge Sets
void unionSet(Node *node1, Node *node2)
{
  Node *rep1 = find(node1);
  Node *rep2 = find(node2);

  if (rep1 == rep2)
    return;

  printf("Set S1: ");
  printSet(rep1);
  printf("Set S2: ");
  printSet(rep2);

  if (rep1->rank > rep2->rank)
  {
    Node *temp = rep2;
    while (temp->next != rep2)
      temp = temp->next;
    temp->next = rep1->next;
    rep1->next = rep2;
    rep2->representative = rep1;
  }
  else if (rep1->rank < rep2->rank)
  {
    Node *temp = rep1;
    while (temp->next != rep1)
      temp = temp->next;
    temp->next = rep2->next;
    rep2->next = rep1;
    rep1->representative = rep2;
  }
  else
  {
    Node *temp = rep2;
    while (temp->next != rep2)
      temp = temp->next;
    temp->next = rep1->next;
    rep1->next = rep2;
    rep2->representative = rep1;
    rep1->rank++;
  }

  printf("Merged Set: ");
  printSet(find(rep1));
}

// Function to add edge to adjacency list
void addEdge(int src, int dest, int weight)
{
  AdjNode *newNode = (AdjNode *)malloc(sizeof(AdjNode));
  newNode->dest = dest;
  newNode->weight = weight;
  newNode->next = graph[src].head;
  graph[src].head = newNode;
}

// Merge function for Merge Sort
void merge(Edge edges[], int left, int mid, int right)
{
  int n1 = mid - left + 1;
  int n2 = right - mid;

  Edge L[n1], R[n2];

  for (int i = 0; i < n1; i++)
    L[i] = edges[left + i];

  for (int i = 0; i < n2; i++)
    R[i] = edges[mid + 1 + i];

  int i = 0, j = 0, k = left;

  while (i < n1 && j < n2)
  {
    if (L[i].weight <= R[j].weight)
    {
      edges[k] = L[i];
      i++;
    }
    else
    {
      edges[k] = R[j];
      j++;
    }
    k++;
  }

  while (i < n1)
  {
    edges[k] = L[i];
    i++;
    k++;
  }

  while (j < n2)
  {
    edges[k] = R[j];
    j++;
    k++;
  }
}

// Merge Sort function
void mergeSort(Edge edges[], int left, int right)
{
  if (left < right)
  {
    int mid = left + (right - left) / 2;
    mergeSort(edges, left, mid);
    mergeSort(edges, mid + 1, right);
    merge(edges, left, mid, right);
  }
}

// Function to print adjacency list
void printAdjacencyList(int V)
{
  printf("\nAdjacency List:\n");
  for (int i = 0; i < V; i++)
  {
    printf("%d -> ", i);
    AdjNode *temp = graph[i].head;
    while (temp)
    {
      printf("(%d, %d) ", temp->dest, temp->weight);
      temp = temp->next;
    }
    printf("\n");
  }
}

// Function to read graph data from a file
int readGraph(const char *filename, int *V, Edge edges[])
{
  FILE *file = fopen(filename, "r");
  if (!file)
  {
    printf("Error: Unable to open file.\n");
    exit(1);
  }

  fscanf(file, "%d", V);
  int edgeCount = 0;

  for (int i = 0; i < *V; i++)
  {
    graph[i].head = NULL;
  }

  for (int i = 0; i < *V; i++)
  {
    int vertex, numEdges;
    fscanf(file, "%d %d", &vertex, &numEdges);

    for (int j = 0; j < numEdges; j++)
    {
      int dest, weight;
      fscanf(file, "%d %d", &dest, &weight);

      if (vertex < dest)
      {
        edges[edgeCount].src = vertex;
        edges[edgeCount].dest = dest;
        edges[edgeCount].weight = weight;
        edgeCount++;

        addEdge(vertex, dest, weight);
        addEdge(dest, vertex, weight);
      }
    }
  }

  fclose(file);
  return edgeCount;
}

// Kruskal's Algorithm Implementation
void kruskalMST(int V, Edge edges[], int edgeCount)
{
  mergeSort(edges, 0, edgeCount - 1);

  Node *nodes[V];
  for (int i = 0; i < V; i++)
  {
    nodes[i] = makeSet(i);
  }

  Edge mst[V - 1];
  int mstCount = 0;
  int totalWeight = 0;

  printf("\nRunning Kruskal Algorithm...\n");
  for (int i = 0; i < edgeCount && mstCount < V - 1; i++)
  {
    Edge e = edges[i];

    if (find(nodes[e.src]) != find(nodes[e.dest]))
    {
      mst[mstCount++] = e;
      totalWeight += e.weight;
      unionSet(nodes[e.src], nodes[e.dest]);
    }
  }

  printf("\nMinimum Spanning Tree (Kruskal's Algorithm):\n");
  for (int i = 0; i < mstCount; i++)
  {
    printf("%d -- %d == %d\n", mst[i].src, mst[i].dest, mst[i].weight);
  }
  printf("Total Weight of MST: %d\n", totalWeight);

  for (int i = 0; i < V; i++)
  {
    free(nodes[i]);
  }
}

int main()
{
  int V, edgeCount;
  Edge edges[MAX_EDGES];

  edgeCount = readGraph("graph.txt", &V, edges);
  printAdjacencyList(V);
  kruskalMST(V, edges, edgeCount);

  return 0;
}