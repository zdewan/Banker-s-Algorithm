#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/*#include <pthread.h> */
#include <sys/stat.h>
#include <time.h>

int *available;
int **allocated;
int **need;

int totalCustomers = 1;


//Read the input file
char *readFile() {
	FILE *sample = fopen("sample4_in.txt", "r");

	if (!sample) {
		printf("FILE OPEN ERROR: File Not Found\n");
		return ("ERROR!");
	}

	struct stat st;
	fstat(fileno(sample), &st);
	char* fileContent = (char*) malloc(((int) st.st_size + 1) * sizeof(char));
	fileContent[0] = '\0';

	while (!feof(sample)) {
		char request[100];
		if (fgets(request, 100, sample) != NULL)
			strncat(fileContent, request, strlen(request));
		totalCustomers++;
	}

	fclose(sample);
	return fileContent;
}

void checkNeed(int a, int b, int **allocated, int maximum[a][b], int **need) {
	for (int c = 0; c < b; c++) {
		for (int d = 0; d < a; d++)
			need[c][d] = maximum[c][d] - allocated[c][d];
	}
}
int main(int argc, char *argv[]) {
	char * fileIn = readFile();
	int availableSize = argc - 1;
	int maximum[totalCustomers][availableSize];

	printf("%d", totalCustomers);

//Dynamic allocation for: memory for available, alocated, need, and maximum - in preparation for the bankers algorithm.

//available
	available = (int*) malloc(availableSize * sizeof(int));
	for (int i = 0; i < availableSize; i++) {
		available[i] = atoi(argv[i + 1]);
	}

	//allocated
	allocated = (int **) malloc(totalCustomers * sizeof(int *));
	for (int i = 0; i < totalCustomers; i++) {
		allocated[i] = (int *) malloc(availableSize * sizeof(int));
	}
	// Initialize allocated array to zeros
	for (int i = 0; i < totalCustomers; i++) {
		for (int j = 0; j < availableSize; j++)
			allocated[i][j] = 0;
	}

	//need
	need = (int **) malloc(totalCustomers * sizeof(int *));
	for (int i = 0; i < totalCustomers; i++) {
		need[i] = (int *) malloc(availableSize * sizeof(int));
	}
	return 1;
}
