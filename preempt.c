/* 

This scheduler simulator is used to visualize how a scheduler would allocate the processes to the processor or the CPU. 
The scheduler utilizes different scheduling algorithms to manage process distribution and maximize utilization of PC hardware. 
In this simulator of a process scheduler there are two variants which are First come First Served (FCFS) and Round Robin, the difference between these two variants are the algorithms and the different manners in which processed are scheduled to run. 

Round Robin:

This scheduling algorithm, takes all processes and executes them based on their arrival time and the service time required. 
There is a fixed time quantum of 2 seconds implemented. 
This algorithm operated by means  of adding each active (Ready) PCB to a ‘Ready’ Queue. 
Each process at the front of the queue is allowed to execute for its time quantum of 2 seconds 
(or 1 second given the process only has 1 second of time remaining to complete execution). 
After the process has finished its time quantum it is pushed to the back of the queue 
and the next process in line is allowed to execute. If the process had finished executing completely and reached EXIT state, 
it is removed from the queue. It is clear that this scheduling algorithm is a preemptive scheduling algorithm because 
the scheduler takes control over how long a process should execute for and also execute 
them in patterns based on decisions made due to arrival and completion of other process in the queue. 


*/



#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "process-visualiser.h"
#include <unistd.h>

typedef enum{
	READY,RUNNING,EXIT,DONE
}process_state;


typedef struct {
    char processName[11];			// Char array for process name 
    int entryTime;				// The time process enters system 
    int serviceTime;				// The time for which process executes
    int remainingTime ; 			//remaining service time until completion .
    process_state state;			// The state of the systme;  Enum: READY, RUNNING,EXIT, DONE
    int isSorted;				// Holds 1 if sorted, 0 Otherwise 

    int waitTime;				// The time for which process has to wait in ready state 
    int activationTime;				// The time at which process starts executing
    int exitTime;				// The time at which the process is scheduled to exit the system 
    
    int nextInQueue;				// ri indicate if PCB is next in queue 
    int slice;					// the slice remaining for that particular PCB
    int active; 				// if the process is active or dead 
    
} proc_data_t;


typedef struct{
int quePos;
int processIndex;
}que; 




char * fileRead_function(int argc, char* argv[])
/*
	This fucntion is used to return the name of the userGIven filename or default filename
	Return: the name of the filename.
*/
{
	char *fileName;						// inititializing local variable 
	if(argc==2){						// if arg counter is 2
		fileName = argv[1];				// storing filename
	}else if(argc == 1){					// if arguments are passed
		fileName = "process-data.txt";			// stores default filename
	}else{							// when more than expected inputs are given
		printf("</n>%s</n>","Too many arguments provided.");
		exit(-1);					// exits with code -1
	}
	return fileName;					// returning the char pointer 
	

}



int main (int argc, char *argv[]){

	/*  	
	
	In this fucntion we create and simualate the scheduling algorithm.
	We read all the raw process data from the file and create a array of PCBs sorted based on entry time.
	We then add the PCBs to a 'READY Queue ' of PCB's in real time when any process becomes active at their arrival time into the system. 
	We takes all processes and executes them based on their arrival time and the service time required. 
	
	There is a fixed time quantum (time slice) of 2 seconds implemented. 
	
	This algorithm operated by means  of adding each active (Ready) PCB to a ‘Ready’ Queue. 
	Each process at the front of the queue is allowed to execute for its time quantum of 2 seconds 
	(or 1 second given the process only has 1 second of time remaining to complete execution).
	The process is in RUNNING state when its the first in line at the queue and executes for its time slice, after RUNNING 
	for the time slice the process returns to READY state and enters the back of the queue and the next process in line is allowed to execute. 
	If the process had finished executing completely and reached EXIT state, it is removed from the queue. 
	
	It is clear that this scheduling algorithm is a preemptive scheduling algorithm because 
	the scheduler takes control over how long a process should execute for and also execute 
	them in patterns based on decisions made due to arrival and completion of other process in the queue. 


	return 0:  when func completes executing 
	
	*/

	char *filename=fileRead_function(argc,argv);		// running fileRead_fuc and storing return filename in pointer
	FILE *fPointer;						// Initilizing pointer for file read
	
	fPointer = fopen(filename,"r");			// opening default file or userGivenFilessss
	if(!fPointer)
	{
		printf("</n> Filename Invalid, please re-enter with valid filename.");	// File Error Hadeling
	}
	
	char line[100];						// initilizing char array for line read 
	int lineCount = -1;				// int lineCOunt initilzed as -1 because indexes start from 0
	
	proc_data_t processInfo[10];			// Creating 10 pcb structures because there can be 10 at most 
	
	while(!feof(fPointer)){				// Loop executes while not end of file 
	
		fgets(line,100,fPointer);		// Reads in 100 characters from line (of file) into char array 
		lineCount++;				// lineCount incremented 
	
		char processName[10]={0};		// Creating char array to track name of process; in collumn 1 of line (before first space )
		char entryTime[10]={0};			// Creating char array to track the entry time data in collumn 2 of line (after first space )
		char serviceTime[10]={0};		// Creating char array to track service time data in collumn 3 of line  (after second space )
		int space = 0;				// Initialing space count as 0;
		
		int i = 0;				// Initializing count to track characters in line being read 
		int processNameCount = 0;		// Initializing a count to track characters read into processName char array 
		int entryTimeCount =0;			// Initializing a count to track characters read into entry Time  char array 
		int serviceTimeCount = 0;		// Initializing a count to track characters read into service time  char array 
		char buffer;			

		
		
		
		while(i < 100){				// Condition that only 100 possible character of each line will be read at Max

			if ((char)line[i] == ' ')	// if space is found, incrementing the space count 
			{
				space++;		// incrementing space count 
			
			}
			if ((char)line[i] == '\n')	// if new lie character is found, terminate the loop, because we only read the first line information
			{
				break;		
			}
			
			if(space == 0)		// if space == 0;  all the characters in line[i] are processName characters and must be appennded to the processName array
			{	
				processName[processNameCount] = line[i]; 	// using processNameCount tracker, append the character to the processName char array
				processNameCount++;				// Incrementing the processName array index count 
			}
			else if(space == 1)	// if space == 1;  all the characters in line[i] are processName characters and must be appennded to the processName array
			{
				entryTime[entryTimeCount] = line[i]; 	// using entryTimeCount tracker, append the character to the entryTime char array
				entryTimeCount++;			// Incrementing the entryTime array index count 
			}
			else if(space == 2)	// if space == 2;  all the characters in line[i] are serviceTime characters and must be appennded to the serviceTime array
			{
				serviceTime[serviceTimeCount] = line[i];	// using serviceTimeCount tracker, append the character to the serviceTime char array
				serviceTimeCount++;				// Incrementing the serviceTime array index count 
			}
			else if(space >=3)				//if space == 3; break the loop because we've read all 3 collumnns  whose data we're interested in
			{
				break;
			}
			
			
			//printf("%c",line[i]);
										//running the loop till the file is read completely
			i++;
			
		}
		
		for(i=0;i<sizeof(processName);i++)		// Updating the Struct with the data obtaied from line read, using lineCount index for this purpose 
		{
		processInfo[lineCount].processName[i] = processName[i];	// Updating the Struct with the data obtaied from line read, using lineCount index for this purpose 
		}
		int entryTimeVal;
		int serviceTimeVal;
		sscanf(entryTime, "%d", &entryTimeVal);			// coverting char entryTIme array to int using builtin function 
		sscanf(serviceTime, "%d", &serviceTimeVal);		// coverting char serviceTIme array to int using builtin function 
		
		processInfo[lineCount].entryTime = entryTimeVal;	// Updating struct with entryTime 
		processInfo[lineCount].serviceTime = serviceTimeVal;	// Updating struct with serviceTime
		
		
		
		//printf("\nProcessName: %s     EntryTime: %d	ServiceTime: %d	", processInfo[lineCount].processName,processInfo[lineCount].entryTime,processInfo[lineCount].serviceTime);
		
		//printf("\nProcessName: %s\n\n", processName);
		//printf("\nEntryTime: %s\n\n", entryTime);
		//printf("\nServiceTime: %s\n\n", serviceTime);
	}

	fclose(fPointer);		// Closing file after feof(file end of file)
	
	
//______________________________________________________________________________________________________________________________________________________________________________

	
		
	int totalProcessCount = lineCount+1;				//Total process Count set
	
	
	// SOrting the Read Process Data into a New list of Sorted Process Data 
	
	proc_data_t sortedProcessInfo[10];				// Creating new array to hold sorted process data 
	
	
	for(int i=0; i < totalProcessCount; i++)			// Iterating over array ad setting processInfo.isSorted to 0
	{
		processInfo[i].isSorted=0;
		//printf("%d",sortedProcessInfo[i].isSorted);
	}
	
	for(int i=0; i < totalProcessCount; i++)			//Iterating over array and setting sortedProcessInfo.isSorted to 0
	{
		sortedProcessInfo[i].isSorted=0;			
		//printf("%d",sortedProcessInfo[i].isSorted);
	}
	
	int sortedProcessIndex = 0;					// Tracking sotedProcessInfo index as it is filled with process data sorted by entryTime
	for(int i=0;i < totalProcessCount;i++)				// Iterating over number of processes 
	{	
		if(sortedProcessIndex == totalProcessCount)		// at any point when sortedProcessIndex becomes equal to totalProcessCount, break out of loop
		{
			break;
		}
		int min_pointer = 0;					// Min pointer set to 0
		
		for(int z=0;z<totalProcessCount;z++)			// iterating over processInfo, when an unsorted process is found, set min pointer to that index and brek
		{ 
			if(processInfo[z].isSorted == 0)
			{
				min_pointer = z;
				//printf("\n%d\n",min_pointer);
				break;
			}
			else 
			{
				continue;				// Else continue to enxt iteration
			}
		
			
		}
		
		
		for(int j=0;j<totalProcessCount;j++)			// Iterating over total num of processes. 
		{
			if(processInfo[j].isSorted == 0)		// Checking if idex of processInfo is already sorted 
			{
				if(processInfo[j].entryTime < processInfo[min_pointer].entryTime)	// if not sorted, checking if entry time < currentMin
				{
					min_pointer = j;						// if so, set minpointer to new index
				}
				else
				{
					continue; 							// Else, continue to next iteration
				}
			}
			else
			{
				continue;								// IF sorted, continue to next iteration
			}
		}
		
		// After finding the definite unsorted min_pointer index, setting data sortedProcessInfo[sortedProcessIndex] to processInfo[min_pointer]
		for(int i=0;i<sizeof(processInfo[min_pointer].processName);i++)
		{
			sortedProcessInfo[sortedProcessIndex].processName[i] = processInfo[min_pointer].processName[i];  //Adding process name to sorted block 
		}
		
		sortedProcessInfo[sortedProcessIndex].entryTime = processInfo[min_pointer].entryTime;		// adding entry time to sorted block 
		sortedProcessInfo[sortedProcessIndex].serviceTime = processInfo[min_pointer].serviceTime;	// adding service time to sorted block 
		
		
		
		
		//printf("\nProcessName: %s     EntryTime: %d	ServiceTime: %d	", sortedProcessInfo[sortedProcessIndex].processName,sortedProcessInfo[sortedProcessIndex].entryTime, sortedProcessInfo[sortedProcessIndex].serviceTime);
		
		sortedProcessIndex++;				// Incremennting sortedProcessIndex
		processInfo[min_pointer].isSorted = 1;		// setting processInfo[min_pointer] to sorted 
		/*
		printf("\nProcessName: %s     EntryTime: %d	ServiceTime: %d	", processInfo[min_pointer].processName,processInfo[min_pointer].entryTime, processInfo[min_pointer].serviceTime);
		*/
		
		//sortedProcessIndex++;
		
	
		
	}	// Closing outer most for loop
	
	
	
	//printf("\n%d\n\n",totalProcessCount);
	/*for(int i=0; i<totalProcessCount;i++)
	{
	
	printf("\nProcessName: %s     EntryTime: %d	ServiceTime: %d	", sortedProcessInfo[i].processName,sortedProcessInfo[i].entryTime, sortedProcessInfo[i].serviceTime);
	}*/
	



//__________________________________________________________________________________________________________________________________________________________________________
	
	// Setting mathematical values of waitTime, Activation Time, Exit Time for each process in sortedProcessInfo
	
	int totalTime =0;
	
	for(int i=0; i<totalProcessCount; i++)
	{
		
		
		if(i==0)
		{	/*if(sortedProcessInfo[i].entryTime > 0)
			{
				totalTime+=sortedProcessInfo[i].entryTime;
			}*/
			totalTime+=sortedProcessInfo[i].entryTime;									//Cal entry Time 
			sortedProcessInfo[i].waitTime = sortedProcessInfo[i].entryTime;							//Cal wait Time 
			sortedProcessInfo[i].activationTime = sortedProcessInfo[i].waitTime;						//Cal activation Time 
			sortedProcessInfo[i].exitTime = sortedProcessInfo[i].activationTime + sortedProcessInfo[i].serviceTime;		//Cal exit Time 
			sortedProcessInfo[i].remainingTime = sortedProcessInfo[i].serviceTime;						//Cal remainning Time 
			
		}
		else
		{
			sortedProcessInfo[i].waitTime = totalTime - sortedProcessInfo[i].entryTime;					//Cal wait Time 
			sortedProcessInfo[i].activationTime = sortedProcessInfo[i].waitTime +sortedProcessInfo[i].entryTime;		//Cal activation Time 
			sortedProcessInfo[i].exitTime = sortedProcessInfo[i].activationTime +sortedProcessInfo[i].serviceTime;		//Cal exit Time 
			sortedProcessInfo[i].remainingTime = sortedProcessInfo[i].serviceTime;						//Cal total Time 
		}
		totalTime+=sortedProcessInfo[i].serviceTime;										//Cal total Time 
	}
	
	/*for(int i=0; i<totalProcessCount; i++)
	{
	printf("\nWait Time: %d	   Activation Time: %d	   Exit Time: %d",sortedProcessInfo[i].waitTime,  sortedProcessInfo[i].activationTime, sortedProcessInfo[i].exitTime);
	}*/
	
	
	
	
	
//____________________________________________________________________________________________________________________________________________________________________________

//Processor 	
	
	
	// Setting struct Variables 
	for(int i=0; i < totalProcessCount; i++)
	{
		sortedProcessInfo[i].nextInQueue=0;
		sortedProcessInfo[i].slice=0;
		sortedProcessInfo[i].waitTime=0;	
	}

	sortedProcessInfo[0].nextInQueue=1;
	sortedProcessInfo[0].slice=2;		// GIving first process a slice of 2

	
	int currentTime = 0;			// Innitilalizig current time as 0
	que Queue[1000];			// Creating a 1000 size queue of type struct 
	
	for(int i=0; i<1000; i++)
	{
		Queue[i].quePos=0;		// Initializing variables; will be changed throughout execution process
	}
	
	
	Queue[0].quePos =0;			// setting que[0].pos to 0; which is the index of first process;  
	int queStartIndex = 0;			// start of queue index is set as 0
	int queEndIndex = -1;			// end of queue index is set as -1 
	int totalProcessExecuted=0;		// iitializing total execuated process as 0
	

	
	initInterface("black","white");
	while(1)
	{	
		if(totalProcessExecuted==totalProcessCount)	// The while loop executed while total executed processes is less than total process count
		{
			break;
		}
		
		for(int i=0; i < totalProcessCount; i++)
		{
			if(sortedProcessInfo[i].entryTime == currentTime)		//If currennt time is equal to entry time of any process in sorted list of PCBs
			{
				sortedProcessInfo[i].active == 1;			//
				sortedProcessInfo[i].state = READY;			//Settibng PCB state as Ready
				appendRow(sortedProcessInfo[i].processName);		//appending row
				appendBlank(i, sortedProcessInfo[i].entryTime);		//appendinng blank block
				queEndIndex++;						// Incrementinng end inndex of queue
				Queue[queEndIndex].processIndex = i;			// Adding PCB to ready Queue 
				printf("</n>%s entered the system at %d seconds",sortedProcessInfo[i].processName, currentTime);
				
			}
		}
		
		// THe next elemnt of queue is queStartIndex
		int nextQueIndex = Queue[queStartIndex].processIndex;
		
		if(sortedProcessInfo[nextQueIndex].remainingTime == sortedProcessInfo[nextQueIndex].serviceTime)
		{
			sortedProcessInfo[nextQueIndex].slice = 2;	// setting default service time as 2 sec
		}
		
		
		if(sortedProcessInfo[nextQueIndex].remainingTime>1)	// if remaning time is > 1
		{
			if(sortedProcessInfo[nextQueIndex].slice == 2)	// if slice is 2 
			{	
				sortedProcessInfo[nextQueIndex].remainingTime -= 1;	//substract 1 from remaining time 
				sortedProcessInfo[nextQueIndex].slice-=1;		//substract 1 from slice 
				sortedProcessInfo[nextQueIndex].state = RUNNING;	// set PCB to running 
				printf("</n>%s is in Running State",sortedProcessInfo[nextQueIndex].processName);
				sleep(1);
			}
			else if(sortedProcessInfo[nextQueIndex].slice==1)
			{
				sortedProcessInfo[nextQueIndex].remainingTime -= 1;	//substract 1 from remaining time 
				sortedProcessInfo[nextQueIndex].slice-=1;		//substract 1 from slice 
				printf("</n>%s is in Running State",sortedProcessInfo[nextQueIndex].processName);	
				sleep(1);			// sleep 1 
					
				//printf("\nCurrent Time: %d     ProcessName: %s     EntryTime: %d	ServiceTime: %d	     RemainingTime: %d ",currentTime, sortedProcessInfo[nextQueIndex].processName,sortedProcessInfo[nextQueIndex].entryTime, sortedProcessInfo[nextQueIndex].serviceTime, sortedProcessInfo[nextQueIndex].remainingTime);

				sortedProcessInfo[nextQueIndex].state = READY;		// Set PCB ti ready 
				queEndIndex++;						// incremeting end index of queue 
				Queue[queEndIndex].processIndex = nextQueIndex;		// if slice is now zero, pushing index to end of queue 
				queStartIndex++;					// incremeninng startIndex by 1   
				sortedProcessInfo[Queue[queStartIndex].processIndex].slice =2;	// setting the slice of next PCB in queue as 2 

			}
			
			
				
		}
		
		else if(sortedProcessInfo[nextQueIndex].remainingTime==1)		// if remaining time to execute for PCB is 1 
		{
			sortedProcessInfo[nextQueIndex].remainingTime -= 1;		// substracy 1 from remaining time 
			printf("</n>%s is in Running State",sortedProcessInfo[nextQueIndex].processName);
			sleep(1);							// sleep 1 
			
					
			sortedProcessInfo[nextQueIndex].state = EXIT;			// Setting PCB state as EXIT 
			printf("</n>%s completed. Turnaround Time: %d seconds. Total wait time is: %d seconds.",sortedProcessInfo[nextQueIndex].processName, sortedProcessInfo[nextQueIndex].serviceTime+sortedProcessInfo[nextQueIndex].waitTime, sortedProcessInfo[nextQueIndex].waitTime);
			queStartIndex++;						// Incrementing PCB que array start index
			sortedProcessInfo[Queue[queStartIndex].processIndex].slice =2;	// Setting slice of next PCB as 2 
			
		}
		
		
		
		for(int i = queStartIndex; i< queEndIndex+1;i++)		// Iteratinng over all the PCBs and drawing the blocks 
		{
			if(sortedProcessInfo[Queue[i].processIndex].state == RUNNING)	// IF PCB in Running State 
			{
				appendBar(Queue[i].processIndex, 1, "blue", "RUNNING", 0);
	
			}
			else if(sortedProcessInfo[Queue[i].processIndex].state == READY)	// If PCB in READY state 
			{	
				if(Queue[i].processIndex == nextQueIndex) 
				{
					appendBar(Queue[i].processIndex, 1, "blue", "RUNNING", 0);
				}
				else{
					appendBar(Queue[i].processIndex, 1, "red", "READY", 1);
					sortedProcessInfo[Queue[i].processIndex].waitTime++;
				}
			}
			
			
			
		}
		
		for(int i=0; i < totalProcessCount; i++)	
		{
			if(sortedProcessInfo[i].state == EXIT)		// IF PCB in exit state 
			{	
				sortedProcessInfo[i].state = DONE;
				totalProcessExecuted+=1;
				sortedProcessInfo[i].active == 0;
				appendBar(i, 1, "blue", "RUNNING", 0);
				
			}
		}
		
		currentTime++;		// Incrementing currentTime 
		
		
		
	}
	

	waitExit();
	
	
	
	return 0;
}




