#include <stdio.h>
#include <stdlib.h>
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
void shift(int row, int col, int [row][col], int);

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
		printf("\n\t**********************************************************\n\n");
	}
}

void fcfs(int row){
	int col = 3, choice = 1;
	
	int serviceTime = 0;
	float totalTAT = 0, totalWT = 0;

	int (*arr)[col];
	arr = malloc(row * sizeof(*arr));

	getValue(row, col, arr, choice);
	sortArray(row, col, arr, ARRIVAL_T, choice);

	printf("\n\tGANTT CHART");
	if (arr[0][0] != 0){
		printf("\n\t0 [IDLE] %d", arr[0][0]);
		serviceTime+=arr[0][0];
	}
	else{
		printf("\n\t%d", arr[0][0]);
	}

	for(int i = 0; i < row; i++){
		// if next process already arrived
		if (arr[i][0] <= serviceTime){
			serviceTime += arr[i][1];
			printf(" [P%d] %d", arr[i][2], serviceTime);
			totalTAT += serviceTime - arr[i][0];
			totalWT += serviceTime - arr[i][1] - arr[i][0];
		}
		else{
			printf(" [IDLE] %d", arr[i][0]);
			serviceTime=arr[i][0];
			i--;
		}
	}

	printf("\n\n\t ** Average Turn Around Time: 	%f **",(totalTAT/row));
	printf("\n\t ** Average Waiting Time: %f **",totalWT/row);
	free(arr);
}
	
void sjf(int row){	
	int col = 3, choice = 2, serviceTime = 0;
	float totalTAT = 0, totalWT = 0;
	
	int (*arr)[col];
	arr = malloc(row * sizeof(*arr));

	getValue(row, col, arr, choice);
	
	// check if processes arrived at the same time
	for (int i = 0; i < row - 1; i++) {
		for (int j = i + 1; j < row; j++) {
			// if at least one process arrived at a different time, sort by AT
			if (arr[j][0] != arr[i][0]){
				sortArray(row, col, arr, ARRIVAL_T, choice);
				goto next;
			}
        }
		break;
    }
	
	// if all processes arrived at the same time, sort by BT
	sortArray(row, col, arr, BURST_T, choice);
	
	printf("\n\tGANTT CHART");
	if (arr[0][0] != 0){
		printf("\n\t0 [IDLE] %d", arr[0][0]);
		serviceTime+=arr[0][0];
	}
	else{
		printf("\n\t0");
	}

	for(int i = 0; i < row; i++){
		serviceTime+=arr[i][1];
		printf(" [P%d] %d", arr[i][2], serviceTime);
		totalTAT += serviceTime - arr[i][0];
		totalWT += serviceTime - arr[i][1] - arr[i][0];
	}
	
	goto end;

	next:
		int shortestBT;
		int shortestBurstIndex;

		printf("\tGANTT CHART");
		if (arr[0][0] != 0){
			printf("\n\t0 [IDLE] %d", arr[0][0]);
			serviceTime+=arr[0][0];
		}
		else{
			printf("\n\t0");
		}
		
		for(int k = 0; k < row; k++){
			shortestBT = -1;
			shortestBurstIndex = -1;
			for (int i = 0; i < row; i++) {
				// check if AT is less than or equal to certain value
				if (arr[i][0] <= serviceTime && arr[i][2] != -1) {
					// check if lowest BT value is -1 or current row's BT is lower
					if (shortestBT == -1 || arr[i][1] < shortestBT) {
						// update lowest BT value and row index
						shortestBT = arr[i][1];
						shortestBurstIndex = i;	
					}
				}
			}
		
			// idle time detected
			if (shortestBurstIndex == -1){
				for(int i = 0; i < row; i++){
					if (arr[i][0] > serviceTime){
						shortestBurstIndex = i;
						break;
					}	
				}

				printf(" [IDLE] %d", arr[shortestBurstIndex][0]);
				serviceTime = arr[shortestBurstIndex][0];
				k--;
				continue;
			}
			
			serviceTime += arr[shortestBurstIndex][1];
			printf(" [P%d] %d", arr[shortestBurstIndex][2], serviceTime);
			arr[shortestBurstIndex][2] = -1;
			totalTAT += serviceTime - arr[shortestBurstIndex][0];
			totalWT += serviceTime - arr[shortestBurstIndex][1] - arr[shortestBurstIndex][0];
		} // end of outer for loop
			
	end:
		printf("\n\n\t ** Average Turn Around Time: %f **",(totalTAT/row));
		printf("\n\t ** Average Waiting Time: %f **",totalWT/row);
		free(arr);
}

void priorityBased(int row){
	int col = 4, choice = 3, serviceTime = 0;
	float totalTAT = 0, totalWT = 0;

	int (*arr)[col];
	arr = malloc(row * sizeof(*arr));
	
	getValue(row, col, arr, choice);
	
	// check if processes arrived at the same time
	for (int i = 0; i < row - 1; i++) {
		for (int j = i + 1; j < row; j++) {
			// if at least one process arrived at a different time, sort by AT
			if (arr[j][0] != arr[i][0]){
				sortArray(row, col, arr, ARRIVAL_T, choice);
				goto next;
			}
        }
    }
	// if all processes arrived at the same time, sort by Priority
	sortArray(row, col, arr, PRIORITY, choice);

	printf("\n\tGANTT CHART");
	if (arr[0][0] != 0){
		printf("\n\t0 [IDLE] %d", arr[0][0]);
		serviceTime+=arr[0][0];
	}
	else{
		printf("\n\t0");
	}

	for(int i = 0; i < row; i++){
		serviceTime+=arr[i][1];
		printf(" [P%d] %d", arr[i][2], serviceTime);
		totalTAT += serviceTime - arr[i][0];
		totalWT += serviceTime - arr[i][1] - arr[i][0];
	}

	goto end;

	next:
		int highestPrio = -1;
		int highestPrioIndex = -1;

		printf("\n\tGANTT CHART");
		if (arr[0][0] != 0){
			printf("\n\t0 [IDLE] %d", arr[0][0]);
			serviceTime+=arr[0][0];
		}
		else{
			printf("\n\t0");
		}
		
		for(int k = 0; k < row; k++){
			highestPrio = -1;
			highestPrioIndex = -1;
			for (int i = 0; i < row; i++) {
				// check if AT is less than or equal to certain value
				if (arr[i][0] <= serviceTime && arr[i][2] != -1) {
					// check if lowest prio is -1 or current row's prio is lower
					if (highestPrio == -1 || arr[i][3] < highestPrio) {
						// update lowest BT value and row index
						highestPrio = arr[i][3];
						highestPrioIndex = i;	
					}
				}
			}
		
			// idle time detected
			if (highestPrioIndex == -1){
				for(int i = 0; i < row; i++){
					if (arr[i][0] > serviceTime){
						highestPrioIndex = i;
						break;
					}	
				}

				printf(" [IDLE] %d", arr[highestPrioIndex][0]);
				serviceTime = arr[highestPrioIndex][0];
				k--;
				continue;
			}
			
			serviceTime += arr[highestPrioIndex][1];
			printf(" [P%d] %d", arr[highestPrioIndex][2], serviceTime);
			arr[highestPrioIndex][2] = -1;
			totalTAT += serviceTime - arr[highestPrioIndex][0];
			totalWT += serviceTime - arr[highestPrioIndex][1] - arr[highestPrioIndex][0];
		} // end of outer for loop
		
		end:
			printf("\n\n\t ** Average Turn Around Time: %f **",(totalTAT/row));
			printf("\n\t ** Average Waiting Time: %f **",totalWT/row);
			free(arr);
}
	
void roundRobin(int row){
	int col = 4, choice = 4, serviceTime = 0, len = 0, timeQuantum;
	float totalTAT = 0, totalWT = 0;
	bool firstFlag, secondFlag;
	
	int (*arr)[col];
	int (*queue)[col];	
	arr = malloc(row * sizeof(*arr));
	queue = malloc(row * sizeof(*queue));

	printf("\tEnter your time quantum: ");
	scanf("%d", &timeQuantum);

	getValue(row, col, arr, choice);
	sortArray(row, col, arr, ARRIVAL_T, choice);

	// put to the first process to queue arr
	for (int i = 0; i < col; i++) {
			queue[0][i] = arr[0][i];
	}
	arr[0][0] = -1;
	len++;

	printf("\n\tGANTT CHART");
	if (queue[0][0] != 0){
		printf("\n\t0 [IDLE] %d", queue[0][0]);
		serviceTime+=queue[0][0];
	}
	else{
		printf("\n\t0");
	}

	while (true){
		// if RT >= TQ
		if(queue[0][3] >= timeQuantum){
			queue[0][3] -= timeQuantum;
			serviceTime += timeQuantum;
			printf(" [P%d] %d", queue[0][2], serviceTime);	
			
			if(queue[0][3] == 0){
				totalTAT += serviceTime - queue[0][0];
				totalWT += serviceTime - queue[0][1] - queue[0][0];
			}	
		}
		// if RT < TQ
		else{
			serviceTime += queue[0][3];	
			queue[0][3] = 0;
			totalTAT += serviceTime - queue[0][0];
			totalWT += serviceTime - queue[0][1] - queue[0][0];
			printf(" [P%d] %d", queue[0][2], serviceTime);	
		}

		// add processes to queue
		for(int i = 0; i < row; i++){
			if(arr[i][0] <= serviceTime && arr[i][0] != -1){
				for(int j = 0; j < col; j++){
					queue[len][j] = arr[i][j];
				}
				// mark the arrival time to -1 if process is added to queue
				arr[i][0] = -1;	
				len++;
			}
		}

		// checks if all queued processes are done
		for(int i = 0; i < len; i++){
			if(queue[i][3] == 0){
				firstFlag = true;
			}
			else{
				firstFlag = false;
				break;
			}
		}

		// checks if all processes are added to the queue
		for(int i = 0; i < row; i++){
			if(arr[i][0] == -1){
				secondFlag = true;
			}
			else{
				secondFlag = false;
				break;
			}
		}

		// if all processes in queue are done but there are unqueued processes => idle time
		if(firstFlag == true && secondFlag == false){
			for(int i = 0; i < row; i++){
				if (arr[i][0] != -1){
					printf(" [IDLE] %d",arr[i][0]);
					serviceTime = arr[i][0];
					for (int k = 0; k < col; k++) {
						queue[len][k] = arr[i][k];
					}
					arr[i][0] = -1;
					len++;
					break;
				}
			}
		}

		// if all processes are queued and done
		if(firstFlag == true && secondFlag == true){
			break;
		}

		//shift until the RT of first process is not equal to zero
		while (true){
			shift(row, col, queue, len);
			if(queue[0][3] != 0){
				break;
			}
		}
	} // end of while loop

	printf("\n\n\t ** Average Turn Around Time: %f **",(totalTAT/row));
	printf("\n\t ** Average Waiting Time: %f **",totalWT/row);
	
	free(arr);
	free(queue);
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
			if (choice == 4){
				arr[i][3] = arr[i][1];
			}
		}
	}
}

void shift(int row, int col, int arr[row][col], int len) {
    int temp[4];

    // Store the first row in temp
    for (int j = 0; j < col; j++) {
        temp[j] = arr[0][j];
    }

    // Shift rows to the top
	for (int i = 0; i < len; i++) {
        for (int j = 0; j < 4; j++) {
            arr[i][j] = arr[i+1][j];
        }
    }

    // Assign the temporary variable to the last row
    for (int j = 0; j < col; j++) {
        arr[len-1][j] = temp[j];
    }
}
