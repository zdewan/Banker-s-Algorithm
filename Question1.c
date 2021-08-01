#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <sys/stat.h>
#include <time.h>
#include <semaphore.h>

int *available;
int **allocated;
int **need;

int totalCustomers = 0;
int availableSize;
int *bankerAns;

//Reads the input file and assigns totalCustomers its value
char *readFile() {
	FILE *sample = fopen("sample4_in.txt", "r");

	if (!sample) {
		printf("No File Found!\n");
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

//Checks the needed resources for each customer
void checkNeed(int i, int j, int **allocated, int maximum[j][i], int **needed) {
	for (int m = 0; m < j; m++) {
		for (int n = 0; n < i; n++)
			need[m][n] = maximum[m][n] - allocated[m][n];
	}
}

//Checks available resources
void checkAvailable() {
	int temp1, temp2;
	for (int m = 0; m < availableSize; m++) {
		temp1 = 0;

		for (int n = 0; n < totalCustomers; n++)
			temp1 = temp1 + allocated[n][m];
		temp2 = available[m] - temp1;
		available[m] = temp2;
	}
}
//Performs the bankers algorithm
//https://www.geeksforgeeks.org/bankers-algorithm-in-operating-system-2/ - citing this source as the framework used to implement this algorithm
int bankersAlgorithm() {
	int i, j, k;
	int f[totalCustomers], ind = 0;
	for (k = 0; k < totalCustomers; k++) {
		f[k] = 0;
	}
	int y = 0;
	for (k = 0; k < 5; k++) {
		for (i = 0; i < totalCustomers; i++) {
			if (f[i] == 0) {

				int flag = 0;
				for (j = 0; j < availableSize; j++) {
					if (need[i][j] > available[j]) {
						flag = 1;
						break;
					}
				}

				if (flag == 0) {
					bankerAns[ind++] = i;
					for (y = 0; y < availableSize; y++)
						available[y] += allocated[i][y];
					f[i] = 1;
				}
			}
		}
	}
	printf("Safe Sequence is: ");
	for (i = 0; i < totalCustomers; i++)
		printf("%d ", bankerAns[i]);
	printf("\n");

	return 0;

}
//Runner function for threads
void *runner(void *p) {

	int *client = (int *) p;

	printf("\tAllocated resources: ");
	for (int p = 0; p < availableSize; p++) {
		printf("%d ", allocated[*client][p]);
	}
	printf("\n");

	printf("\tNeeded:");
	for (int i = 0; i < availableSize; i++) {
		printf("%d ", need[*client][i]);
	}
	printf("\n");

	printf("\tAvailable: ");
	for (int i = 0; i < availableSize; i++) {
		printf("%d ", available[i]);
	}
	printf("\n");

	printf("\tThread has started\n");
	printf("\tThread has finished\n");
	printf("\tThread is releasing resources\n");
	printf("\tNew Available:\t");

	for (int i = 0; i < availableSize; i++) {
		available[i] = available[i] + allocated[*client][i];
		printf("%d ", available[i]);
	}

	printf("\n");
	return NULL;

}

int main(int argc, char *argv[]) {
	availableSize = argc - 1;
	char * fileInput = readFile();
	bankerAns = (int*) malloc(totalCustomers * sizeof(int));

//Dynamic allocation memory for:available, allocated, need, and maximum - in preparation for the bankers algorithm.

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

	//Maximum
	int maximum[totalCustomers][availableSize];
	for (int m = 0; m < totalCustomers; m++) {
		for (int n = 0; n < availableSize; n++) {
			maximum[m][n] = 0;
		}
	}

	//Token the input given in the file and store it into maximum[i][j]
	char* lines[totalCustomers];
	char *command = NULL;
	int i = 0;
	command = strtok(fileInput, "\r\n");
	while (command != NULL) {
		lines[i] = malloc(sizeof(command) * sizeof(char));
		strcpy(lines[i], command);
		i++;
		command = strtok(NULL, "\r\n");
	}

	for (int k = 0; k < totalCustomers; k++) {
		char* tok = NULL;
		int l = 0;
		tok = strtok(lines[k], ",");
		while (tok != NULL) {
			maximum[k][l] = atoi(tok);
			l++;
			tok = strtok(NULL, ",");
		}
	}

//Now that all the values have been initialized, we can begin to run the main part of the program in which the user can run various commands.

//Start by printing number of customers, available resources, and maximum resources.
	printf("Number of Customers:%d", totalCustomers);

	printf("\nCurrently Available resources: ");
	for (int n = 0; n < availableSize; n++) {
		printf("%d ", available[n]);
	}

	printf("\nMaximum resources from file:\n");
	for (int n = 0; n < totalCustomers; n++) {
		for (int m = 0; m < availableSize; m++) {
			printf("%d ", maximum[n][m]);
		}
		printf("\n");
	}

//Start the ongoing while loop which takes user input.
// It looks for RQ, RL, Status, Run, and Exit.

	char userInput[100];
	char tempCopy[100];
	printf("Enter Command: ");
	fgets(userInput, 100, stdin);
	strcpy(tempCopy, userInput);
	int safe = 0;

	while (1) {
		if (safe > 0) {
			printf("Enter Command: ");
			fgets(userInput, sizeof userInput, stdin);
			strcpy(tempCopy, userInput);
		}

		//Takes user input and parses it into individual sections in a new array
		int total_string = 0;
		for (int n = 0; tempCopy[n] != '\0'; n++) {
			if (tempCopy[n] == ' ' || tempCopy[n] == '\n'
					|| tempCopy[n] == '\t')
				total_string++;
		}

		char* tok = strtok(tempCopy, " ");
		char *splitInput[100];

		int i = 0;
		if (total_string >= 2) {
			while (tok != NULL && i <= totalCustomers) {
				splitInput[i] = tok;
				tok = strtok(NULL, " ");
				i++;
			}
			}

		else
			strcpy(splitInput[0], tempCopy);

		int inputLen = i;
		i = 0;

		//Performing each command
		if (strcmp(splitInput[0], "RQ") == 0) {
			if (atoi(splitInput[1]) >= totalCustomers) {
				printf("Request cannot be bigger that max customers\n");
			}
			else {
				for (int j = 2; j < (inputLen); j++) {
					allocated[atoi(splitInput[1])][j - 2] = atoi(splitInput[j]);
				}
				printf("State is safe, and request is satisfied\n");
			}
		} else if (strcmp(splitInput[0], "RL") == 0) {
			int flag;

			if (atoi(splitInput[1]) >= totalCustomers) {
				printf("Request cannot be bigger that max customers\n");
			}

			else {

				for (int q = 2; q < (inputLen); q++) {
					int releaseValue = allocated[atoi(splitInput[1])][q - 2]
							- atoi(splitInput[q]);

					if (releaseValue < 0) {
						printf("Release is unsatisfied\n");
						flag = 1;
						break;
					}

					else
						allocated[atoi(splitInput[1])][q - 2] = releaseValue;

					if (q == inputLen - 1)
						printf("Release is satisfied\n");
				}
				if (flag == 1)
					continue;
			}
		} else if (strcmp(splitInput[0], "Status\n") == 0) {
			printf("Available Resources:\n");
			checkAvailable();
			for (int j = 0; j < availableSize; j++) {
				printf("%d ", available[j]);
			}



			printf("\nMaximum Resources:\n");
			for (int n = 0; n < totalCustomers; n++) {
				for (int m = 0; m < availableSize; m++) {
					printf("%d ", maximum[n][m]);
				}
				printf("\n");
			}



			printf("Allocated Resources:\n");
			for (int l = 0; l < totalCustomers; l++) {
				for (int p = 0; p < availableSize; p++)
					printf("%d ", allocated[l][p]);
				printf("\n");
			}



			printf("Need Resources:\n");
			checkNeed(availableSize, totalCustomers, allocated, maximum, need);
			for (int l = 0; l < totalCustomers; l++) {
				for (int p = 0; p < availableSize; p++)
					printf("%d ", need[l][p]);

				printf("\n");
			}
		} else if (strcmp(splitInput[0], "Run\n") == 0) {
			checkNeed(availableSize, totalCustomers, allocated, maximum, need);
			int bankerCheck = bankersAlgorithm();

			if (bankerCheck != 0) {
				printf("No safe sequence\n");
				return -1;
			}

			for (int g = 0; g < totalCustomers; g++) {
				int arg = bankerAns[g];
				printf("--> Customer/Thread %d\n", arg);

				pthread_t t1;
				pthread_create(&t1, NULL, runner, &arg);
				pthread_join(t1, NULL);
			}
		} else if (strcmp(splitInput[0], "Exit\n") == 0) {
			return 1;
		} else {
			printf("Invalid input, please try again.\n");
		}
		safe++;
	}

	return 0;
}
