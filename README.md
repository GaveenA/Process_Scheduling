Process Scheduling Algorithms:

This scheduler simulator is used to visualize how a scheduler would allocate the processes to the processor or the CPU. 
The scheduler utilizes different scheduling algorithms to manage process distribution and maximize utilization of PC hardware. 
In this simulator of a process scheduler there are two variants which are First come First Served (FCFS) and Round Robin, 
the difference between these two variants are the algorithms and the different manners in which processed are scheduled to run. 

FCFS (First Come First Serve): 

This scheduling algorithm takes all process execution requests and executes them in a queue, based on arrival times of the processes. 
Therefore all the process are executed in a sequential order (in the order which they arrive).  
Each process is executed completely and therefore this algorithm is an no-preemptive scheduling algorithm because the scheduler 
does not have control of how long a process should execute for. In this scheduling algorithm, successive processes wait for the 
complete execution of their predecessors, before being allowed to execute to their completion



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
