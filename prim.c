/*C program to implement Prims algorithm to compute Minimum Spanning Tree of a weighted
undirected graph using min-heap and adjacency list representation*/

#include <stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<limits.h>

#define INFINITY INT_MAX

//structure to create Adjacency list node
typedef struct AdjListNode{
    int dest;
    int weight;
    struct AdjListNode* next;
}AdjListNode;

//struture to create Adjacency List
typedef struct AdjList{
    AdjListNode* Head;
}AdjList;

//structure for Graph representation
typedef struct Graph{
    int Vertex;
    AdjList* array;
}Graph;

//function to create new Adjacency List Node
AdjListNode* createAdjListNode(int dest,int weight)
{
    AdjListNode* node=(AdjListNode*)malloc(sizeof(AdjListNode));
    node->dest=dest;
    node->weight=weight;
    node->next=NULL;
    return node;
}

//function to create graph of vertices V
Graph* createGraph(int V)
{
    Graph* g=(Graph*)malloc(sizeof(Graph));
    g->Vertex=V;
    g->array=(AdjList*)malloc(V*sizeof(AdjList));
    int i=0;
    while(i<V)
    {
        g->array[i].Head=NULL;
        i++;
    }
    return g;
}

//function to add edge at the beginning of adjacency list of src in a Graph
void insertEdge(Graph* g,int start,int end,int weight){
    
    AdjListNode* node=createAdjListNode(end,weight);
    node->next=g->array[start].Head;
    g->array[start].Head=node;

    //same Operation for reverse as it is undirected Graph
    node=createAdjListNode(start,weight);
    node->next=g->array[end].Head;
    g->array[end].Head=node;


}

//funtion to print graph
void printGraph(Graph* g,int V)
{
    int i;
    printf("\n\t\tGraph Repesentation using Adjacency List:\n");
    printf("\t\t----------------------------------------------");
    for(i=0;i<V;i++)
    {
        printf("\n\t\t%d->",i);
        AdjListNode* current=g->array[i].Head;
        while(current!=NULL)
        {
            printf("(%d,%d)->",current->dest,current->weight);
            current=current->next;
        }
        printf("NULL\t\t\n");
    }
}

//Structure to define a min heap node 
typedef struct MinHeapNode{ 
	int v; 
	int key; 
}MinHeapNode; 

//Structure to create a min heap 
typedef struct MinHeap{ 
	int size; // Number of nodes present in Min Heap 
	int capacity; // Capacity of min heap 
	int* pos; // needed for decreaseKey() 
	MinHeapNode** array; 
}MinHeap; 

// function to create a new Min Heap Node 
MinHeapNode* newMinHeapNode(int v, int key) 
{ 
	MinHeapNode* minHeapNode = (MinHeapNode*)malloc(sizeof(MinHeapNode)); 
	minHeapNode->v = v; 
	minHeapNode->key = key; 
	return minHeapNode; 
} 

//function to create a Min Heap 
MinHeap* createMinHeap(int capacity) 
{ 
	MinHeap* minHeap = (MinHeap*)malloc(sizeof(MinHeap)); 
	minHeap->pos = (int*)malloc(capacity * sizeof(int)); 
	minHeap->size = 0; 
	minHeap->capacity = capacity; 
	minHeap->array = (MinHeapNode**)malloc(capacity * sizeof(MinHeapNode*)); 
	return minHeap; 
} 

//function to swap two nodes of min heap 
void swapMinHeapNode(MinHeapNode** a, MinHeapNode** b) 
{ 
	struct MinHeapNode* temp = *a; 
	*a = *b; 
	*b = temp; 
} 

//function to heapify at given index 
// Position is needed for decreaseKey() 
void minHeapify(MinHeap* minHeap, int idx) 
{ 
	int smallest, left, right; 
	smallest = idx; 
	left = 2 * idx + 1; 
	right = 2 * idx + 2; 

	if (left < minHeap->size && minHeap->array[left]->key < minHeap->array[smallest]->key) 
		smallest = left; 

	if (right < minHeap->size && minHeap->array[right]->key < minHeap->array[smallest]->key) 
		smallest = right; 

	if (smallest != idx) { 
		MinHeapNode* smallestNode = minHeap->array[smallest]; 
		MinHeapNode* idxNode = minHeap->array[idx]; 

		minHeap->pos[smallestNode->v] = idx; 
		minHeap->pos[idxNode->v] = smallest; 

		swapMinHeapNode(&minHeap->array[smallest], &minHeap->array[idx]); 
        minHeapify(minHeap, smallest); 
	} 
} 

//function to check if the given Min Heap is empty or not 
int isEmpty(MinHeap* minHeap) 
{ 
	return minHeap->size == 0; 
} 

//function to extract minimum node(root node) from Min heap 
MinHeapNode* extractMin(MinHeap* minHeap) 
{ 
	if (isEmpty(minHeap)) 
		return NULL; 

	MinHeapNode* root = minHeap->array[0]; 

	MinHeapNode* lastNode = minHeap->array[minHeap->size - 1]; 
	minHeap->array[0] = lastNode; 

	minHeap->pos[root->v] = minHeap->size - 1; 
	minHeap->pos[lastNode->v] = 0; 

	--minHeap->size; 
	minHeapify(minHeap, 0); 

	return root; 
} 

//function to decrease key value of a given vertex v
// uses pos[] of min heap to get the current index of node in min heap 
void decreaseKey(MinHeap* minHeap, int v, int key) 
{ 
	//get the index of v in heap array 
	int i = minHeap->pos[v]; 

	minHeap->array[i]->key = key; 

	while (i && minHeap->array[i]->key < minHeap->array[(i - 1) / 2]->key) {  
		minHeap->pos[minHeap->array[i]->v] = (i - 1) / 2; 
		minHeap->pos[minHeap->array[(i - 1) / 2]->v] = i; 
		swapMinHeapNode(&minHeap->array[i], &minHeap->array[(i - 1) / 2]);  
		i = (i - 1) / 2; 
	} 
} 

//function to check if a given vertex 'v' is in min heap or not 
bool isInMinHeap(MinHeap* minHeap, int v) 
{ 
	if (minHeap->pos[v] < minHeap->size) 
		return true; 
	return false; 
} 

//function used to print the constructed MST 
void printArr(int arr[],int weightMST[],int n) 
{ 
	printf("\n\n\t\tMinimum Spanning Tree:\n");
    printf("\t\t--------------------------");
	printf("\n\t\tSource\t--> \tweight\n");
	for (int i = 1; i < n; ++i) 
		printf("\n\t\t%d\t--> \t%d\n", arr[i], weightMST[arr[i]]); 
} 

//function that constructs MST using Prim's algorithm 
void PrimMST(Graph* graph) 
{ 
	int V = graph->Vertex; // Get the number of vertices in graph 
	int parent[V]; //used to store constructed MST 
	int key[V]; //used to pick minimum weight edge in cut 

	MinHeap* minHeap = createMinHeap(V); 

    for (int v = 1; v < V; v++) { 
		parent[v] = -1; 
		key[v] = INT_MAX; 
		minHeap->array[v] = newMinHeapNode(v, key[v]); 
		minHeap->pos[v] = v; 
	} 

    key[0] = 0; 
	minHeap->array[0] = newMinHeapNode(0, key[0]); 
	minHeap->pos[0] = 0; 

	minHeap->size = V; 

	while (!isEmpty(minHeap)) {  
		MinHeapNode* minHeapNode = extractMin(minHeap); 
		int u = minHeapNode->v; 

		AdjListNode* pCrawl = graph->array[u].Head; 
		while (pCrawl != NULL) { 
			int v = pCrawl->dest; 

			if (isInMinHeap(minHeap, v) && pCrawl->weight < key[v]) { 
				key[v] = pCrawl->weight; 
				parent[v] = u; 
				decreaseKey(minHeap, v, key[v]); 
			} 
			pCrawl = pCrawl->next; 
		} 
	} 

	//print edges of MST 
	printArr(parent,key,V); 
} 

//driver main funtion
int main()
{
    int V = 9; 
	struct Graph* graph = createGraph(V); 
	insertEdge(graph, 0, 1, 4); 
	insertEdge(graph, 0, 7, 8); 
	insertEdge(graph, 1, 2, 8); 
	insertEdge(graph, 1, 7, 11); 
	insertEdge(graph, 2, 3, 7); 
	insertEdge(graph, 2, 8, 2); 
	insertEdge(graph, 2, 5, 4); 
	insertEdge(graph, 3, 4, 9); 
	insertEdge(graph, 3, 5, 14); 
	insertEdge(graph, 4, 5, 10); 
	insertEdge(graph, 5, 6, 2); 
	insertEdge(graph, 6, 7, 1); 
	insertEdge(graph, 6, 8, 6); 
	insertEdge(graph, 7, 8, 7); 

 /* int V,E,start,end,w,i;
  printf("\t\tPrims Algorithm Implementation");  
  printf("\n\t\t------------------------------");
  printf("\n\t\tEnter the Number of vertices present in Graph:");
  scanf("%d",&V);
  Graph* graph=createGraph(V);
  printf("\t\tEnter the Numbers of Edges present in between the vertices:");
  scanf("%d",&E);
  for(i=1;i<=E; )
  {
      printf("\n\t\tEnter the Start vertex of an edge:");
      scanf("%d",&start);
      printf("\t\tEnter the End vertex of an edge:");
      scanf("%d",&end);
      //assumption vertices ranges from 0 V-1
      if(start>V-1 || end>V-1){
          printf("\t\tInput error:start and end vertex should be less than no of vertices ");
          continue;
      }
      printf("\t\tEnter the weigth of an edge:");
      scanf("%d",&w);
      insertEdge(graph,start,end,w);
      i++;
  }*/
  printGraph(graph,V);
  PrimMST(graph);
  return 0;
}