#include <stdio.h>
#include <string.h>
// 20171643 Park Junhyeok Data Structure Assignment 1 - KMP
#define MAX_STR_SIZE	10000000
#define MAX_PAT_SIZE	3000
char string[MAX_STR_SIZE];
char pat[MAX_PAT_SIZE];
int failure[MAX_PAT_SIZE];
int matchIdx[MAX_STR_SIZE];
int cnt = 0;

void FailFunc(char *pat) {
	int i, j, lenp = strlen(pat);

	failure[0] = -1;
	for (j = 1; j < lenp; j++) {
		i = failure[j - 1];

		while ((pat[j] != pat[i + 1]) && (i >= 0))
			i = failure[i];
		if (pat[j] == pat[i + 1])
			failure[j] = i + 1;
		else
			failure[j] = -1;
	}
}

int FindMatchIdx(char *string, char *pat, int *startIdx) {
	int i = *startIdx, j = 0, flag = 0;
	int lens = strlen(string) - 1;
	int lenp = strlen(pat) - 1;

	while (i < lens && j < lenp) {
		if (string[i] == pat[j]) {
			i++; j++;
		}
		else if (j == 0)
			i++;
		else
			j = failure[j - 1] + 1;
	}
	*startIdx = i - lenp + 1;
	if (j == lenp) matchIdx[cnt++] = i - lenp;
	else flag = -1;
	return flag;
}

int main(void) {
	int start = 0, i;
	int flag;
	FILE*fp1, *fp2, *fp3;
	if ((fp1 = fopen("string.txt", "rt")) == NULL || (fp2 = fopen("pattern.txt", "rt")) == NULL) {
		puts("The string file does not exist.");
		return -1;
	}
	fgets(string, sizeof(string), fp1);
	fgets(pat, sizeof(pat), fp2);

	FailFunc(pat);

	while (1) {
		if ((flag = FindMatchIdx(string, pat, &start)) == -1)
			break;
	}

	fp3 = fopen("result_kmp.txt", "wt");

	fprintf(fp3, "%d\n", cnt);
	for (i = 0; i < cnt; i++)
		fprintf(fp3, "%d ", matchIdx[i]);
	fprintf(fp3, "\n");

	puts("Program complete. Result saved to result_kmp.txt");

	fclose(fp1);
	fclose(fp2);
	fclose(fp3);
	return 0;
}