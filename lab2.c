#include <stdio.h>
#include <stdlib.h>

void fcfs();
void sortArray(int[][3], int);		

int main(){
	int input;
	
	printf("\t----  Scheduling Algorithms ----\n");
	printf("\t1. FCFS\n\t2. SJF\n\t3. Priority Based\n\t4. Round Robin\n\t5. Exit\n");
	
	printf("\tEnter your choice: ");
	scanf("%d", &input);
	
	switch(input){
		case 1:
			fcfs();
			break;
		case 2:
			printf("\tSJF");
			break;
		case 3:
			printf("\tPriority Based");
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

void fcfs(){
	int processes;
	float totalBurstT=0, totalWaitingT=0;
	
	printf("\tHow many processes?: ");
	scanf("%d", &processes);
	printf("\n\tEnter the values (Arrival Time and Burst Time)\n");
	
	int arr[processes][3];
    for (int i = 0; i < processes; i++) {
        printf("\tEnter the values for process %d: ", i);
        scanf("%d %d", &arr[i][0], &arr[i][1]);
        arr[i][2] = i;
    }
	
	// sort to find the process that arrived first
    sortArray(arr, processes);
 	
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
			
			//gets the totalWaitingT
			if(i < processes){
				totalWaitingT += sum;
			}
		}
	}
	
	// printf("\n\ntotal waiting time: %f", totalWaitingT);
	// printf("\n\ntotal burst time: %f", totalBurstT);
	printf("\n\n\t ** Average Turn Around Time: %f **",((totalBurstT/processes)));
	printf("\n\t ** Average Waiting Time: %f **",totalWaitingT/processes);
	
}

void sortArray(int arr[][3], int rows) {
    int i, j, temp;
    for (i = 0; i < rows - 1; i++) {
        for (j = i + 1; j < rows; j++) {
            if (arr[j][0] < arr[i][0]) {
                temp = arr[i][0];
                arr[i][0] = arr[j][0];
                arr[j][0] = temp;
                temp = arr[i][1];
                arr[i][1] = arr[j][1];
                arr[j][1] = temp;
                temp = arr[i][2];
                arr[i][2] = arr[j][2];
                arr[j][2] = temp;
            }
        }
    }
}



