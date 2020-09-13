/*

This scheduler simulator is used to visualize how a scheduler would allocate the processes to the processor or the CPU. 
The scheduler utilizes different scheduling algorithms to manage process distribution and maximize utilization of PC hardware. 
In this simulator of a process scheduler there are two variants which are First come First Served (FCFS) and Round Robin, the difference between these two variants are the algorithms and the different manners in which processed are scheduled to run. 

FCFS (First Come First Serve): 

This scheduling algorithm takes all process execution requests and executes them in a queue, based on arrival times of the processes. 

Therefore all the process are executed in a sequential order (in the order which they arrive).  Each process is executed completely and therefore this algorithm is an no-preemptive scheduling algorithm because the scheduler does not have control of how long a process should execute for. In this scheduling algorithm, successive processes wait for the complete execution of their predecessors, before being allowed to execute to their completion.

*/


#include<stdio.h>	
#include<stdlib.h>	
#include "process-visualiser.h"		//To Utilize ioL Functionality 
#include <unistd.h>

typedef enum{
	READY,RUNNING,EXIT
}process_state;



typedef struct {
    char processName[11];			// Char array for process name 
    int entryTime;				// The time process enters system 
    int serviceTime;				// The time for which process executes
    int remainingTime ; 			//remaining service time until completion .
    process_state state;			// The state of the systme;  Enum: READY, RUNNING,EXIT 
    int isSorted;				// Holds 1 if sorted, 0 Otherwise 

    int waitTime;				// The time for which process has to wait in ready state 
    int activationTime;				// The time at which process starts executing
    int exitTime;				// The time at which the process is scheduled to exit the system 
   
    
} proc_data_t;					// name of struct 


char * fileRead_function(int argc, char* argv[])
{	
	//This fucntion is used to return the name of the userGIven filename or default filename
	// Return: the name of the filename.
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
	This scheduling algorithm takes all process execution requests and executes them in a queue, based on arrival times of the processes. 

	Therefore all the process are executed in a sequential order (in the order which they arrive).  
	Each process is executed completely and therefore this algorithm is an no-preemptive scheduling algorithm because the scheduler 
	does not have control of how long a process should execute for. In this scheduling algorithm, successive processes wait for the 
	complete execution of their predecessors, before being allowed to execute to their completion.
	
	return 0:  when func completes executing 
	*/
	
	char *filename=fileRead_function(argc,argv);
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
			totalTime+=sortedProcessInfo[i].entryTime;								//Cal total Time 
			//sortedProcessInfo[i].waitTime = sortedProcessInfo[i].entryTime;			
			sortedProcessInfo[i].waitTime = totalTime - sortedProcessInfo[i].entryTime;				//Cal wait Time 
			sortedProcessInfo[i].activationTime = sortedProcessInfo[i].waitTime;	//Cal activation Time 
			sortedProcessInfo[i].exitTime = sortedProcessInfo[i].activationTime + sortedProcessInfo[i].serviceTime;	//Cal total Time 
		}
		else
		{
			sortedProcessInfo[i].waitTime = totalTime - sortedProcessInfo[i].entryTime;				//Cal wait  Time 
			sortedProcessInfo[i].activationTime = sortedProcessInfo[i].waitTime +sortedProcessInfo[i].entryTime;	//Cal activation Time 
			sortedProcessInfo[i].exitTime = sortedProcessInfo[i].activationTime +sortedProcessInfo[i].serviceTime;	//Cal exit Time 
		}
		totalTime+=sortedProcessInfo[i].serviceTime;	
	}
	
	
	/*for(int i=0; i<totalProcessCount; i++)
	{
	printf("\nWait Time: %d	   Activation Time: %d	   Exit Time: %d",sortedProcessInfo[i].waitTime,  sortedProcessInfo[i].activationTime, sortedProcessInfo[i].exitTime);
	}*/
	
	


//_____________________________________________________________________________________________________________________________________________________________________________

	// Processor
		
	int currentTime = 0;
	initInterface("black","white");
	while(1)									// Time Loop
	{
		if(currentTime == totalTime+1)						// Condition to break out of time loop
		{
			break;
		}
		
		for(int i=0; i<totalProcessCount; i++)	// in each second,  we iterate over the elemets of the sorted PCB to check if currentTIme == entryTime, activationTime
		{					// or exitTime;
			if(sortedProcessInfo[i].entryTime == currentTime)		// If currentTIme == entry time of any PCB 
			{
				if(i==0 && sortedProcessInfo[i].entryTime == 0)		// If currentTIme == entry time of any PCB 
				{
					appendRow(sortedProcessInfo[i].processName);
					sortedProcessInfo[i].state = READY;
					printf("</n>%s entered the system at %d seconds",sortedProcessInfo[i].processName, currentTime);
					continue;
					
					
				}
				else if(i==0 && sortedProcessInfo[i].entryTime > 0)	// If currentTIme == entry time of any PCB 
				{
					appendRow(sortedProcessInfo[i].processName);
					appendBlank(i, sortedProcessInfo[i].entryTime);
					sortedProcessInfo[i].state = READY;
					printf("</n>%s entered the system at %d seconds",sortedProcessInfo[i].processName, currentTime);
					continue;
					
				}
				else
				{
				appendRow(sortedProcessInfo[i].processName);		// appending row 
				appendBlank(i, sortedProcessInfo[i].entryTime);		// appending blank line 
				sortedProcessInfo[i].state = READY;			// setting PCB state as READY 
				printf("</n>%s entered the system at %d seconds",sortedProcessInfo[i].processName, currentTime);
				}
			}
			
			if(sortedProcessInfo[i].activationTime == currentTime)		// if current time equals the activation time of any PCB
			{
				
				//appendBar(i, sortedProcessInfo[i].waitTime, "red", (const char *)sortedProcessInfo[i].state, 1);
				appendBar(i, sortedProcessInfo[i].waitTime, "red", "READY", 1);	// Printing ready block when Running starts 
				sortedProcessInfo[i].state = RUNNING;		// Settinng PCB state as RUNNING 
				sleep(sortedProcessInfo[i].serviceTime);	// sleep for service time 
				printf("</n>%s is in RUNNING State.",sortedProcessInfo[i].processName);
				
				
			}
			if(sortedProcessInfo[i].exitTime == currentTime)	// if current time equals the exit time of any PCB
			{
				//appendBar(i, sortedProcessInfo[i].serviceTime, "blue", (const char *)sortedProcessInfo[i].state, 0);
				appendBar(i, sortedProcessInfo[i].serviceTime, "blue", "RUNNING", 0);	// Printing ready block when Running starts 
				sortedProcessInfo[i].state = EXIT;					// Setting PCB state as exit 
				printf("</n>%s completed. Turnaround Time: %d Waitig Time: %d",sortedProcessInfo[i].processName,sortedProcessInfo[i].serviceTime+sortedProcessInfo[i].waitTime, sortedProcessInfo[i].waitTime );
			}
			
		}
		currentTime++;		// Incrementing Time
		
		
	}
	waitExit();
	
	
	
	return 0;
}




