#include <stdio.h>
#include <stdlib.h>
#include <time.h>
// 20171643 Park Junhyeok Data Structure Assignment 3 - MST
#define MAX_EDGES		50000000
#define MAX_VERTICES	10000
#define TRUE			1
#define FALSE			0

typedef struct _edge {
	int u, v;			// u to v
	int weight;
}Edge;
typedef int Vertex;

Vertex vertices[MAX_VERTICES];
Edge *minheap;
Edge MST[MAX_VERTICES];

int numOfEdges = 0;
int numOfVertices = 0;
int numOfMSTV = 0;
int mstIdx = 0;
int sizeOfHeap = 0;

long long totalCost = 0;
int connect = 0;

int IsHeapFull(int size);
int IsHeapEmpty(int size);
void HeapInsert(Edge item);
Edge HeapDelete(void);

void InitDisjointSet(void);
void WeightedUnion(int i, int j);
int FindTheTopIdx(int i);

void ReadTheGraph(int n, char *filename);
void MinimumSpanningTree(void);
void PrintTheMST(void);

int main(int argc, char *argv[]) {
	clock_t start, stop;
	double duration = 0;
	start = clock();

	ReadTheGraph(argc, argv[1]);
	InitDisjointSet();
	MinimumSpanningTree();
	PrintTheMST();

	stop = clock();
	duration = (double)(stop - start) / CLOCKS_PER_SEC;
	printf("output written to hw3_result.txt.\n");
	printf("running time: %.6lf seconds\n", duration);

	return 0;
}

int IsHeapFull(int size) {
	if (size == MAX_EDGES)
		return TRUE;
	return FALSE;
}

int IsHeapEmpty(int size) {
	if (size == 0)
		return TRUE;
	return FALSE;
}

void HeapInsert(Edge item) {
	int i = sizeOfHeap;

	if (IsHeapFull(i++)) {
		fprintf(stderr, "The Heap is full.\n");
		exit(1);
	}

	while ((i != 1) && (item.weight < minheap[i / 2].weight)) {
		minheap[i] = minheap[i / 2];
		i /= 2;
	}

	minheap[i] = item;
	sizeOfHeap++;
}

Edge HeapDelete(void) {
	int parent, child;
	Edge item, temp;
	int size = sizeOfHeap;

	if (IsHeapEmpty(size)) {
		fprintf(stderr, "The Heap is empty");
		exit(1);
	}

	parent = 1;
	child = 2;

	item = minheap[1];
	temp = minheap[sizeOfHeap--];

	while (child <= sizeOfHeap) {
		if ((child < sizeOfHeap) && (minheap[child].weight > minheap[child + 1].weight))
			child++;
		if (temp.weight <= minheap[child].weight)
			break;
		minheap[parent] = minheap[child];
		parent = child;
		child *= 2;
	}
	minheap[parent] = temp;

	return item;
}

void InitDisjointSet(void) {
	int i;

	for (i = 0; i < numOfVertices; i++)
		vertices[i] = -1;
}

int FindTheTopIdx(int i) {
	for (; vertices[i] >= 0; i = vertices[i])
		;
	return i;
}

void WeightedUnion(int i, int j) {
	int temp = vertices[i] + vertices[j];

	if (vertices[i] > vertices[j]) {
		vertices[i] = j;
		vertices[j] = temp;
	}
	else {
		vertices[j] = i;
		vertices[i] = temp;
	}
}

void ReadTheGraph(int n, char *filename) {
	FILE*fp;
	Edge item;

	if (n != 2) {
		fprintf(stderr, "usage: ./hw3 input_filename\n");
		exit(1);
	}

	if (!(fp = fopen(filename, "rt"))) {
		fprintf(stderr, "The input file does not exist.\n");
		exit(1);
	}

	fscanf(fp, "%d", &numOfVertices);
	fscanf(fp, "%d", &numOfEdges);
	minheap = (Edge*)malloc(sizeof(Edge)*(numOfEdges + 1));

	while (1) {
		if (fscanf(fp, "%d %d %d", &item.u, &item.v, &item.weight) == EOF)
			break;

		HeapInsert(item);
	}

	fclose(fp);
}

void MinimumSpanningTree(void) {
	Edge item;
	int i, topU, topV;

	if (numOfVertices == 1 && numOfEdges == 0)
		connect = 1;

	for (i = 0; i < numOfEdges; i++) {
		item = HeapDelete();
		
		topU = FindTheTopIdx(item.u);
		topV = FindTheTopIdx(item.v);

		if (topU == topV)
			continue;
		if (vertices[topU] == -1)
			numOfMSTV++;
		if (vertices[topV] == -1)
			numOfMSTV++;
		WeightedUnion(topU, topV);

		MST[mstIdx++] = item;
		totalCost += item.weight;

		if (numOfMSTV == numOfVertices && mstIdx == (numOfVertices - 1)) {
			connect = 1;
			break;
		}
	}

	free(minheap);
}

void PrintTheMST(void) {
	FILE*fp = fopen("hw3_result.txt", "wt");
	int i;

	for (i = 0; i < mstIdx; i++)
		fprintf(fp, "%d %d %d\n", MST[i].u, MST[i].v, MST[i].weight);
	fprintf(fp, "%lld\n", totalCost);
	if (connect)
		fprintf(fp, "CONNECTED\n");
	else
		fprintf(fp, "DISCONNECTED\n");

	fclose(fp);
}