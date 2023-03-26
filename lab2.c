#include <stdio.h>
#include <stdlib.h>

void fcfs(int [][3], int);
void sjf(int [][3], int);
void priorityBased(int);
void sortArray(int[][3], int, int);	
void getValue(int [][3], int);

int main(){
	int input;
	int processes;
	
	printf("\t----  Scheduling Algorithms ----\n");
	printf("\t1. FCFS\n\t2. SJF\n\t3. Priority Based\n\t4. Round Robin\n\t5. Exit\n");
	
	printf("\tEnter your choice: ");
	scanf("%d", &input);
	if(input==5){exit(0);}
	
	printf("\tHow many processes?: ");
	scanf("%d", &processes);
	int arr[processes][3];
	
	switch(input){
		case 1:
			getValue(arr, processes);		
			fcfs(arr, processes);
			break;
		case 2:
			getValue(arr, processes);
			sjf(arr, processes);
			break;
		case 3:
			priorityBased(processes);
			break;
		case 4:
			printf("\tRound Robin");
			break;
		default: 
			break;
	}
	printf("\n");
	return 0;
}

void fcfs(int arr[][3], int processes){
	float totalBurstT=0, totalWaitingT=0;
	
	sortArray(arr, processes, 0);
	 	
	// printing the process number
	printf("\n\tGANTT CHART\n");
	for(int i = 0; i < processes; i++){
		if (i == 0){
			printf("\t\tP%d", arr[i][2]);
		}
		else{
			printf("\tP%d", arr[i][2]);
		}
	}
	
	// prints the values
	for(int i = 0; i <= processes; i++){
		static int sum = 0;
		if(i == 0){
			printf("\n\t\t%d", arr[0][0]);
		}
		else{
			sum += arr[i-1][1];
			printf("\t%d", sum);
			
			// gets totalBurstT
			totalBurstT += (sum - arr[i-1][0]);
			
			// gets totalWaitingT
			if(i < processes){
				totalWaitingT += sum;
			}
		}
	}
	
	// printf("\n\ntotal waiting time: %f", totalWaitingT);
	// printf("\n\ntotal burst time: %f", totalBurstT);
	printf("\n\n\t ** Average Turn Around Time: 	%f **",((totalBurstT/processes)));
	printf("\n\t ** Average Waiting Time: %f **",totalWaitingT/processes);
}

void sortArray(int arr[][3], int rows, int time) {
    int temp;
    for (int i = 0; i < rows - 1; i++) {
        for (int j = i + 1; j < rows; j++) {
			
			// check if arrival time right is less than arrival time left
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
            }
        }
    }
}

void sjf(int arr[][3], int processes){
	// check if all arrival times are equal
	for (int i = 0; i < processes - 1; i++) {
		for (int j = i + 1; j < processes; j++) {
			
			// if at least one AT is different from other ATs
			if (arr[j][0] != arr[i][0]){
				fcfs(arr, processes);
				return;
			}
        }
    }
	// if all processes arrived at the same time, sort by BT
	sortArray(arr, processes, 1);
	fcfs(arr, processes);
}

void priorityBased(int processes){
	// extra column for processes;
	int arr[processes][4];
	int temp;
	float totalBurstT=0, totalWaitingT=0;
	
	printf("\n\tEnter the values (Arrival Time, Burst Time, Priority)\n");
	for (int i = 0; i < processes; i++) {
        printf("\tEnter the values for process %d: ", i);
        scanf("%d %d %d", &arr[i][0], &arr[i][1], &arr[i][2]);
        arr[i][3] = i;
    }
	
	// sort by priority
    for (int i = 0; i < processes - 1; i++) {
        for (int j = i + 1; j < processes; j++) {
			// check if arrival time right is less than arrival time left
            if (arr[j][2] < arr[i][2]) {
				// swap arrival times
                temp = arr[i][0];
                arr[i][0] = arr[j][0];
                arr[j][0] = temp;
				
				// swap burst times
                temp = arr[i][1];
                arr[i][1] = arr[j][1];
                arr[j][1] = temp;
				
				// swap priority
                temp = arr[i][2];
                arr[i][2] = arr[j][2];
                arr[j][2] = temp;
				
				// swap process number
                temp = arr[i][3];
                arr[i][3] = arr[j][3];
                arr[j][3] = temp;
            }
        }
    }


	printf("\n\tGANTT CHART\n");
	for(int i = 0; i < processes; i++){
		if (i == 0){
			printf("\t\tP%d", arr[i][2]);
		}
		else{
			printf("\tP%d", arr[i][2]);
		}
	}
	
	// prints the values
	for(int i = 0; i <= processes; i++){
		static int sum = 0;
		if(i == 0){
			printf("\n\t\t%d", arr[0][0]);
		}
		else{
			sum += arr[i-1][1];
			printf("\t%d", sum);
			
			// gets totalBurstT
			totalBurstT += (sum - arr[i-1][0]);
			
			// gets totalWaitingT
			if(i < processes){
				totalWaitingT += sum;
			}
		}
	}
	
	printf("\n\n\t ** Average Turn Around Time: 	%f **",((totalBurstT/processes)));
	printf("\n\t ** Average Waiting Time: %f **",totalWaitingT/processes);
}

void getValue(int arr[][3], int processes){
	printf("\n\tEnter the values (Arrival Time and Burst Time)\n");
    for (int i = 0; i < processes; i++) {
        printf("\tEnter the values for process %d: ", i);
        scanf("%d %d", &arr[i][0], &arr[i][1]);
        arr[i][2] = i;
    }
}