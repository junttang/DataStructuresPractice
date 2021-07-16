#include <stdio.h>
#include <string.h>
// 20171643 Park Junhyeok Data Structure Assignment 1 - Naive
#define MAX_STR_SIZE	10000000
#define MAX_PAT_SIZE	3000
char string[MAX_STR_SIZE];
char pat[MAX_PAT_SIZE];
int matchIdx[MAX_STR_SIZE];
int cnt = 0;

void FindMatchIdx(char *string, char *pat) {
	int i, j, k;
	int lens = strlen(string) - 1;
	int lenp = strlen(pat) - 1;
	for (i = 0; i <= lens - lenp; i++) {
		for (j = 0, k = i; j < lenp && string[k] == pat[j]; k++, j++);
		if (j == lenp)
			matchIdx[cnt++] = i;
	}
}

int main(void) {
	int i;
	FILE*fp1, *fp2, *fp3;
	if ((fp1 = fopen("string.txt", "rt")) == NULL || (fp2 = fopen("pattern.txt", "rt")) == NULL) {
		puts("The string file does not exist.");
		return -1;
	}
	fgets(string, sizeof(string), fp1);
	fgets(pat, sizeof(pat), fp2);

	FindMatchIdx(string, pat);

	fp3 = fopen("result_naive.txt", "wt");

	fprintf(fp3, "%d\n", cnt);
	for (i = 0; i < cnt; i++)
		fprintf(fp3, "%d ", matchIdx[i]);
	fprintf(fp3, "\n");

	puts("Program complete. Result saved to result_naive.txt");

	fclose(fp1);
	fclose(fp2);
	fclose(fp3);
	return 0;
}