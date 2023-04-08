#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ARRIVAL_T 0
#define BURST_T 1
#define PROCESS_NUM 2
#define PRIORITY 3

void fcfs(int row);
void sjf(int row);
void priorityBased(int row);
void getValue(int row, int col, int [row][col], int);
void sortArray(int row, int col, int [row][col], int, int);

int main(){
	int choice;
	int processes;
	
	printf("\t----  Scheduling Algorithms ----\n");
	printf("\t1. FCFS\n\t2. SJF\n\t3. Priority Based\n\t4. Round Robin\n\t5. Exit\n");
	
	printf("\tEnter your choice: ");
	scanf("%d", &choice);
	if(choice > 5 || choice < 1){return 0;}
	
	printf("\tHow many processes?: ");
	scanf("%d", &processes);
	
	switch(choice){
	case 1:	
		fcfs(processes);
		break;
	case 2:
		sjf(processes);
		break;
	case 3:
		priorityBased(processes);
		break;
	case 4:
		printf("Round Robin");
		break;
	default: 
		break;
	}
	printf("\n");
	return 0;
}

void fcfs(int row){
	int col = 3, choice = 1, sortBy = ARRIVAL_T, serviceTime = 0;
	float totalWaitingT=0, totalTAT = 0;
	
	int (*arr)[col];
	arr = malloc(row * sizeof(*arr));

	getValue(row, col, arr, choice);
	sortArray(row, col, arr, sortBy, choice);
	 	
	// print the process numbers
	printf("\n\tGANTT CHART\n");
	for(int i = 0; i < row; i++){
		if (i == 0){
			printf("\t\tP%d", arr[i][2]);
		}
		else{
			printf("\tP%d", arr[i][2]);
		}
	}
	
	// print the values
	for(int i = 0; i <= row; i++){
		if(i == 0){
			printf("\n\t\t%d", arr[0][0]);
		}
		else{ 
			serviceTime += arr[i-1][1];
			printf("\t%d", serviceTime);

			// Completion time - Arrival Time
			totalTAT += serviceTime - arr[i-1][0]; 

			if(i != 1){
				// Completion time - Arrival time - Burst time
				totalWaitingT += serviceTime - arr[i-1][0] - arr[i-1][1];	
			}
		}
	}
	
	// printf("\n\ntotal waiting time: %f", totalWaitingT);
	// printf("\n\ntotal burst time: %f", totalBurstT);
	printf("\n\n\t ** Average Turn Around Time: 	%f **",(totalTAT/row));
	printf("\n\t ** Average Waiting Time: %f **",totalWaitingT/row);
	free(arr);
}

void sjf(int row){	
	int col = 3, choice = 2, serviceTime = 0, index = 0, count;
	float totalWaitingT=0, totalTAT = 0;
	
	int (*rawArr)[col];
	int (*sortedArr)[col];
	rawArr = malloc(row * sizeof(*rawArr));
	sortedArr = malloc(row * sizeof(*sortedArr));

	getValue(row, col, rawArr, choice);
	
	// check if processes arrived at the same time
	for (int i = 0; i < row - 1; i++) {
		for (int j = i + 1; j < row; j++) {
			// if at least one process arrived at a different time, sort by AT
			if (rawArr[j][0] != rawArr[i][0]){
				sortArray(row, col, rawArr, ARRIVAL_T, choice);
				goto next;
			}
        }
    }
	// if all processes arrived at the same time, sort by BT
	sortArray(row, col, rawArr, BURST_T, choice);
	
	// copy rawArr to sortedArr
	for (int i = 0; i < row; i++) {
        for (int j = 0; j < col; j++) {
            sortedArr[i][j] = rawArr[i][j];
        }
	}
	goto output;
	
	next:
		// add first process to new multidimensional array 
		for(int i = 0; i < col; i++){
			sortedArr[index][i] = rawArr[index][i];
		}

		index++;
		rawArr[0][0] = -1;
		serviceTime += sortedArr[0][1];
		count = row - 1;

		while(count != 0){
			int step = 0;
			int (*tempArr)[col];
			tempArr = malloc(row * sizeof(*tempArr));

			for (int i = 0; i < row; i++) {
				// if row's AT is <= to service time and not equal to -1, copy to tempArr
				if ((rawArr[i][0] <= serviceTime) && (rawArr[i][0] != -1)) {
					tempArr[step][0] = rawArr[i][0];
					tempArr[step][1] = rawArr[i][1];
					tempArr[step][2] = rawArr[i][2];
					step++;
				}
			}
		
			// sort tempArr by BT
			sortArray(step, col, tempArr, BURST_T, choice);
			
			// copy the process with shotest BT in tempArr to sortedArr
			for(int i = 0; i < col; i++){
				sortedArr[index][i] = tempArr[0][i];
			}

			// find the first row in tempArr in rawArr and change AT to -1
			for (int i = 0; i < row; i++) {
				if (rawArr[i][0] == tempArr[0][0] && rawArr[i][1] == tempArr[0][1] && rawArr[i][2] == tempArr[0][2]) {
					rawArr[i][0] = -1;
					break;
				}
			}
			
			index++;
			count--;
			serviceTime += tempArr[0][1];
			free(tempArr);
		}
	

	output:
		serviceTime = 0;
		
		// print the process numbers
		printf("\n\tGANTT CHART\n");
		for(int i = 0; i < row; i++){
			if (i == 0){
				printf("\t\tP%d", sortedArr[i][2]);
			}
			else{
				printf("\tP%d", sortedArr[i][2]);
			}
		}
		
		// print the values
		for(int i = 0; i <= row; i++){
			if(i == 0){
				printf("\n\t\t%d", sortedArr[0][0]);
			}
			else{ 
				serviceTime += sortedArr[i-1][1];
				printf("\t%d", serviceTime);

				// Completion time - Arrival Time
				totalTAT += serviceTime - sortedArr[i-1][0]; 

				if(i != 1){
					// Completion time - Arrival time - Burst time
					totalWaitingT += serviceTime - sortedArr[i-1][0] - sortedArr[i-1][1];	
				}
			}
		}
		
		printf("\n\n\t ** Average Turn Around Time: 	%f **",(totalTAT/row));
		printf("\n\t ** Average Waiting Time: %f **",totalWaitingT/row);
		free(rawArr);
		free(sortedArr);
}

void priorityBased(int row){
	int col = 4, choice = 3, serviceTime = 0, index = 0, count;
	float totalWaitingT=0, totalTAT = 0;
	
	int (*rawArr)[col];
	int (*sortedArr)[col];
	rawArr = malloc(row * sizeof(*rawArr));
	sortedArr = malloc(row * sizeof(*sortedArr));

	getValue(row, col, rawArr, choice);
	
	// check if processes arrived at the same time
	for (int i = 0; i < row - 1; i++) {
		for (int j = i + 1; j < row; j++) {
			// if at least one process arrived at a different time, sort by AT
			if (rawArr[j][0] != rawArr[i][0]){
				sortArray(row, col, rawArr, ARRIVAL_T, choice);
				goto next;
			}
        }
    }
	// if all processes arrived at the same time, sort by BT
	sortArray(row, col, rawArr, PRIORITY, choice);
	
	// copy rawArr to sortedArr
	for (int i = 0; i < row; i++) {
        for (int j = 0; j < col; j++) {
            sortedArr[i][j] = rawArr[i][j];
        }
	}	
	goto output;
	
	next:
		// add first process to new multidimensional array 
		for(int i = 0; i < col; i++){
			sortedArr[index][i] = rawArr[index][i];
		}

		index++;
		rawArr[0][0] = -1;
		serviceTime += sortedArr[0][1];
		count = row - 1;


		while(count != 0){
			int step = 0;
			int (*tempArr)[col];
			tempArr = malloc(row * sizeof(*tempArr));

			for (int i = 0; i < row; i++) {
				// if row's AT is <= to service time and not equal to -1, copy to tempArr
				if ((rawArr[i][0] <= serviceTime) && (rawArr[i][0] != -1)) {
					tempArr[step][0] = rawArr[i][0];
					tempArr[step][1] = rawArr[i][1];
					tempArr[step][2] = rawArr[i][2];
					tempArr[step][3] = rawArr[i][3];
					step++;
				}
			}


			// sort tempArr by priority
			sortArray(step, col, tempArr, PRIORITY, choice);

			// copy the process with highest priority in tempArr to sortedArr
			for(int i = 0; i < col; i++){
				sortedArr[index][i] = tempArr[0][i];
			}
		
			// find the first row in tempArr in rawArr and change AT to -1
			for (int i = 0; i < row; i++) {
				if (rawArr[i][0] == tempArr[0][0] && rawArr[i][1] == tempArr[0][1] && rawArr[i][2] == tempArr[0][2]) {
					rawArr[i][0] = -1;
					break;
				}
			}
			
			index++;
			count--;
			serviceTime += tempArr[0][1];
			free(tempArr);
		}
	

	output:
		serviceTime = 0;
		
		// print the process numbers
		printf("\n\tGANTT CHART\n");
		for(int i = 0; i < row; i++){
			if (i == 0){
				printf("\t\tP%d", sortedArr[i][2]);
			}
			else{
				printf("\tP%d", sortedArr[i][2]);
			}
		}
		
		// print the values
		for(int i = 0; i <= row; i++){
			if(i == 0){
				printf("\n\t\t%d", sortedArr[0][0]);
			}
			else{ 
				serviceTime += sortedArr[i-1][1];
				printf("\t%d", serviceTime);

				// Completion time - Arrival Time
				totalTAT += serviceTime - sortedArr[i-1][0]; 

				if(i != 1){
					// Completion time - Arrival time - Burst time
					totalWaitingT += serviceTime - sortedArr[i-1][0] - sortedArr[i-1][1];	
				}
			}
		}
		
		printf("\n\n\t ** Average Turn Around Time: 	%f **",(totalTAT/row));
		printf("\n\t ** Average Waiting Time: %f **",totalWaitingT/row);
		free(rawArr);
		free(sortedArr);
}

void sortArray(int row, int col, int arr[row][col], int time, int choice){
    int temp;
    for (int i = 0; i < row - 1; i++) {
        for (int j = i + 1; j < row; j++) {
			
            if (arr[j][time] < arr[i][time]) {
				
				// swap arrival times
                temp = arr[i][0];
                arr[i][0] = arr[j][0];
                arr[j][0] = temp;
				 
				// swap burst times
                temp = arr[i][1];
                arr[i][1] = arr[j][1];
                arr[j][1] = temp;
				
				// swap process number
                temp = arr[i][2];
                arr[i][2] = arr[j][2];
                arr[j][2] = temp;
				
				// swap priority
				if(choice == 3){
					temp = arr[i][3];
					arr[i][3] = arr[j][3];
					arr[j][3] = temp;
				}
            } 
        }
    }
}

void getValue(int row, int col, int arr[row][col], int choice){
	if(choice == 3){
		printf("\n\tEnter the values (Arrival Time, Burst Time, Priority)\n");
		for (int i = 0; i < row; i++) {
			printf("\tEnter the values for process %d: ", i);
			scanf("%d %d %d", &arr[i][0], &arr[i][1], &arr[i][3]);
			arr[i][2] = i;
		}
	}
	else{
		printf("\n\tEnter the values (Arrival Time and Burst Time)\n");
		for (int i = 0; i < row; i++) {
			printf("\tEnter the values for process %d: ", i);
			scanf("%d %d", &arr[i][0], &arr[i][1]);
			arr[i][2] = i;
		}
	}
}