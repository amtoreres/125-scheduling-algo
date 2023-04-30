#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>

#define ARRIVAL_T 0
#define BURST_T 1
#define PROCESS_NUM 2
#define PRIORITY 3

void fcfs(int);
void sjf(int);
void priorityBased(int);
void roundRobin(int);
void getValue(int row, int col, int [row][col], int);
void sortArray(int row, int col, int [row][col], int, int);
void showChart(int row, int col, int [row][col]);
void shift(int [], int);

int main(){
	int choice;
	int processes;
	int isContinue = 1;

	while(isContinue){
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
			roundRobin(processes);
			break;
		default: 
			break;
		}
		printf("\n\n\tWould you like to continue? 1/0: ");
		scanf("%d", &isContinue);
		printf("\n\t***********************************************************\n");
	}
}

void fcfs(int row){
	int col = 3, choice = 1, sortBy = ARRIVAL_T;
	
	int (*arr)[col];
	arr = malloc(row * sizeof(*arr));

	getValue(row, col, arr, choice);
	sortArray(row, col, arr, sortBy, choice);
	showChart(row, col, arr); 	
	
	free(arr);
	}
	


void sjf(int row){	
	int col = 3, choice = 2, serviceTime = 0, index = 0, count;
	
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
	showChart(row, col, rawArr);
	goto end;
	
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

		showChart(row, col, sortedArr);
	
	end:
		free(rawArr);
		free(sortedArr);
}

void priorityBased(int row){
	int col = 4, choice = 3, serviceTime = 0, index = 0, count;

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
	showChart(row, col, rawArr);
	goto end;
	
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
	
		showChart(row, col, sortedArr);
	
	end:
		free(rawArr);
		free(sortedArr);
}

//	0,	1,	2,	3, 4
// AT, BT, PN, RT, added or not
void roundRobin(int row){
	int col = 5, choice = 4, serviceTime = 0, index = 0, timeQuantum, sortedLen = 0, mark = 0;
	bool flag = false;
	float totalTAT = 0, totalWT = 0;
	int temp_index = 0;
	int (*rawArr)[col];
	int tempArr[row];
	int (*sortedArr)[2];

	rawArr = malloc(row * sizeof(*rawArr));
	sortedArr = malloc(row * sizeof(*sortedArr));


	printf("\tEnter your time quantum: ");
	scanf("%d", &timeQuantum);

	getValue(row, col, rawArr, choice);
	sortArray(row, col, rawArr, ARRIVAL_T, choice);

	// get the final length of the gantt chart
	for(int i = 0; i < row; i++){
		sortedLen += (int) ceil((float)rawArr[i][1]/timeQuantum);
	}

	//	0,	1
	// PN, ST
	sortedArr = malloc(sortedLen * sizeof(*sortedArr));

	// serviceTime = rawArr[0][0];
	while (true){
		// copy the PN of marked row in rawArr and the service tiem to sortedArr
		sortedArr[index][0] = rawArr[mark][2];
		sortedArr[index][1] = serviceTime;

		// check if remaining time is greater than or equal to time quantum
		if(rawArr[mark][3] >= timeQuantum){
			// if yes, decrement remaining by TQ
			rawArr[mark][3] -= timeQuantum;
			// increment service time by TQ
			serviceTime += timeQuantum;

			// if remaining time is equal to 0
			if (rawArr[mark][3] == 0){
				// mark the AT of the row
				totalTAT += serviceTime - rawArr[mark][0];
				totalWT += serviceTime - rawArr[mark][1] - rawArr[mark][0];
				rawArr[mark][0] = -1;
				

				// mark the finished process in tempArr
				for(int i = 0; i < temp_index; i++){
					if(tempArr[i] == mark){
						tempArr[i] = -1;
					}
				}	
			}
		}

		// if remaining time is less than TQ
		else{
			// add the remaining time to service time
			serviceTime += rawArr[mark][3];

			totalTAT += serviceTime - rawArr[mark][0];
			totalWT += serviceTime - rawArr[mark][1] - rawArr[mark][0];
			
			// mark the AT of the row
			rawArr[mark][0] = -1;
			rawArr[mark][3] = 0;

			// mark the finished process in tempArr
			for(int i = 0; i < temp_index; i++){
				if(tempArr[i] == mark){
					tempArr[i] = -1;
				}
			}
		}

		for (int i = 0; i < row; i++) {
			if (rawArr[i][0] <= serviceTime && rawArr[i][4] == 0) {
				tempArr[temp_index] = rawArr[i][2];
				rawArr[i][4] = 1;
				temp_index++;
			}
		}

		shift(tempArr, temp_index);

		for (int i = 0; i < row; i++){
			if(tempArr[i] != -1){
				mark = tempArr[i];
				break;
			}
		}

	
		index++;

		// check if all ATs have been marked -1
		for(int i = 0; i < row; i++){
			if(rawArr[i][0] == -1){
				flag = true;
			}
			else{
				flag = false;
				break;
			}
		}

		if (flag == true){
			break;
		}	
	} // end while


	printf("\n\tGANTT CHART\n");
	for (int i = 0; i < sortedLen; i++){
		printf("\tP%d", sortedArr[i][0]);
	}
	printf("\n");
	for (int i = 0; i < sortedLen; i++){
		printf("\t%d", sortedArr[i][1]);
	}
	printf("\t%d", serviceTime);

	printf("\n\n\t ** Average Turn Around Time: 	%f **",(totalTAT/row));
	printf("\n\t ** Average Waiting Time: %f **",totalWT/row);
		
	free(sortedArr);
	free(rawArr);
} // end round robin



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
			if (choice == 4){
				arr[i][3] = arr[i][1];
				arr[i][4] = 0;
			}
		}
	}
}

void showChart(int row, int col, int arr[row][col]){
	int serviceTime = 0;
	float totalTAT = 0, totalWaitingT = 0;
	
	// print the process numbers
	printf("\n\tGANTT CHART\n");
	for(int i = 0; i < row; i++){
		printf("\tP%d", arr[i][2]);
	}

	// print the values
	for(int i = 0; i <= row; i++){
		if(i == 0){
			printf("\n\t%d", arr[0][0]);
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
}

void shift(int arr[], int len){
	int temp = arr[0];

    // Shift elements to the left
    for (int i = 0; i < len-1; i++) {
        arr[i] = arr[i+1];
    }

    // Assign the temporary variable to the last element
    arr[len-1] = temp;
}