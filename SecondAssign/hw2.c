#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
// 20171643 Park Junhyeok Data Structure Assignment 2 - Heap
#define MAX_ELEMENTS	1000000
#define TRUE			1
#define FALSE			0

typedef enum { maxheap, minheap } typeOfHeap;
typedef enum { insert, ascend, descend } typeOfCommand;

typedef struct _elem {
	int key;
}element;

element maxHeap[MAX_ELEMENTS + 1];
element minHeap[MAX_ELEMENTS + 1];
element tempHeap[MAX_ELEMENTS + 1];
int maxHeapSize = 0;
int minHeapSize = 0;

int IsHeapFull(int size);
int IsHeapEmpty(int size);
void HeapInsert(element item, typeOfHeap type);
element HeapDelete(typeOfHeap type);
void HeapPrint(typeOfHeap type);
int firstPrintFlag = 0;
typeOfCommand ReadLine(char*eachLine, char*command, int*integer);
typeOfCommand GetCommandType(char*command);
void OperateCommand(typeOfCommand type, int*integer);

int main(int argc, char *argv[]) {
	element item;
	char eachLine[50];
	FILE*fp;
	clock_t start, stop;
	double duration = 0;
	start = clock();

	if (argc != 2) {
		fprintf(stderr, "usage: ./hw2 input_filename\n");
		exit(1);
	}

	if (!(fp = fopen(argv[1], "rt"))) {
		fprintf(stderr, "The input file does not exist.\n");
		exit(1);
	}

	while (fgets(eachLine, sizeof(eachLine), fp)) {
		char command[8];
		int integer = 0;
		typeOfCommand type;

		type = ReadLine(eachLine, command, &integer);
		OperateCommand(type, &integer);
	}

	stop = clock();
	duration = (double)(stop - start) / CLOCKS_PER_SEC;
	printf("output written to hw2_result.txt.\n");
	printf("running time: %.6lf seconds\n", duration);

	fclose(fp);
	return 0;
}

int IsHeapFull(int size) {
	if (size == MAX_ELEMENTS)
		return TRUE;
	return FALSE;
}

int IsHeapEmpty(int size) {
	if (size == 0)
		return TRUE;
	return FALSE;
}

void HeapInsert(element item, typeOfHeap type) {
	int i;

	if (type == maxheap)
		i = maxHeapSize;
	else if (type == minheap)
		i = minHeapSize;

	if (IsHeapFull(i++)) {
		fprintf(stderr, "The Heap is full.\n");
		exit(1);
	}

	if (type == maxheap) {
		while ((i != 1) && (item.key > maxHeap[i / 2].key)) {
			maxHeap[i] = maxHeap[i / 2];
			i /= 2;
		}

		maxHeap[i] = item;
		maxHeapSize++;
	}
	else if (type == minheap) {
		while ((i != 1) && (item.key < minHeap[i / 2].key)) {
			minHeap[i] = minHeap[i / 2];
			i /= 2;
		}

		minHeap[i] = item;
		minHeapSize++;
	}
	else {
		fprintf(stderr, "The Command is improper.\n");
		exit(1);
	}
}

element HeapDelete(typeOfHeap type) {
	int parent, child;
	element item, temp;
	int size;

	if (type == maxheap)
		size = maxHeapSize;
	else if (type == minheap)
		size = minHeapSize;

	if (IsHeapEmpty(size)) {
		fprintf(stderr, "The Heap is empty");
		exit(1);
	}

	parent = 1;
	child = 2;
	if (type == maxheap) {
		item = maxHeap[1];
		temp = maxHeap[maxHeapSize--];

		while (child <= maxHeapSize) {
			if ((child < maxHeapSize) && (maxHeap[child].key < maxHeap[child + 1].key))
				child++;
			if (temp.key >= maxHeap[child].key)
				break;
			maxHeap[parent] = maxHeap[child];
			parent = child;
			child *= 2;
		}

		maxHeap[parent] = temp;
	}
	else if (type == minheap) {
		item = minHeap[1];
		temp = minHeap[minHeapSize--];

		while (child <= minHeapSize) {
			if ((child < minHeapSize) && (minHeap[child].key > minHeap[child + 1].key))
				child++;
			if (temp.key <= minHeap[child].key)
				break;
			minHeap[parent] = minHeap[child];
			parent = child;
			child *= 2;
		}

		minHeap[parent] = temp;
	}
	else {
		fprintf(stderr, "The Command is improper.\n");
		exit(1);
	}

	return item;
}

void HeapPrint(typeOfHeap type) {
	int size;
	int i;
	
	FILE*fp;

	if (firstPrintFlag == 0) {
		firstPrintFlag = 1;
		if (!(fp = fopen("hw2_result.txt", "wt"))) {
			fprintf(stderr, "The Printing isn't working.\n");
			exit(1);
		}
	}
	else {
		if (!(fp = fopen("hw2_result.txt", "at"))) {
			fprintf(stderr, "The Printing isn't working.\n");
			exit(1);
		}
	}

	switch (type) {
	case maxheap:
		size = maxHeapSize;

		for (i = 1; i <= size; i++)
			tempHeap[i] = maxHeap[i];

		for (i = 1; i <= size; i++)
			fprintf(fp, "%d ", HeapDelete(maxheap).key);
		fprintf(fp, "\n");

		for (i = 1; i <= size; i++)
			maxHeap[i] = tempHeap[i];
		maxHeapSize = size;
		break;
	case minheap:
		size = minHeapSize;

		for (i = 1; i <= size; i++)
			tempHeap[i] = minHeap[i];

		for (i = 1; i <= size; i++)
			fprintf(fp, "%d ", HeapDelete(minheap).key);
		fprintf(fp, "\n");

		for (i = 1; i <= size; i++)
			minHeap[i] = tempHeap[i];
		minHeapSize = size;
		break;
	default:
		break;
	}

	fclose(fp);
}

typeOfCommand ReadLine(char*eachLine, char*command, int*integer) {
	if (eachLine[0] == 'I')
		sscanf(eachLine, "%s %d", command, integer);
	else if (eachLine[0] == 'A' || eachLine[0] == 'D')
		sscanf(eachLine, "%s", command);
	else {
		fprintf(stderr, "The Input file seems improper.\n");
		exit(1);
	}
	return GetCommandType(command);
}

typeOfCommand GetCommandType(char*command) {
	if (!strcmp(command, "INSERT"))
		return insert;
	else if (!strcmp(command, "ASCEND"))
		return ascend;
	else if (!strcmp(command, "DESCEND"))
		return descend;
	else {
		fprintf(stderr, "The Input file is improper.\n");
		exit(1);
	}
}

void OperateCommand(typeOfCommand type, int*integer) {
	element item;

	switch (type) {
	case insert:
		item.key = *integer;
		HeapInsert(item, maxheap);
		HeapInsert(item, minheap);
		break;
	case ascend:
		HeapPrint(minheap);
		break;
	case descend:
		HeapPrint(maxheap);
		break;
	default:
		break;
	}
}