# Operating Systems - Banker's Algorithm
	
Features

	There are five commands provided: RQ, RL, Status, Run, and Exit.
	RQ - Takes the request from the user in the format: {RQ [int customer number] [int resource 1] [int resource 2] [int resource 3] [int resource 4])}
	RL - Releases a specified process in the format: {RL [int customer number] [int resource 1] [int resource 2] [int resource 3] [int resource 4])}
	Status - Prints the current values of: available, maximum, allocation, and need
	Exit - Ends the program
	
Test cases

	osc@ubuntu:~$ make runq1
	./Question1 10 5 7 8
	Number of Customers:5
	CurrentlyAvailableresources:10 5 7 8
	Maximum resources from file:
	6 4 7 3
	4 2 3 2 
	2 5 3 3 
	6 3 3 2 
	5 5 7 5 
	Enter Command: RQ 0 1 0 0 1
	State is safe,and request is satisfied
	Enter Command: RQ 1 1 1 1 1
	State is safe,and request is satisfied
	Enter Command: RQ 2 2 2 2 2
	State is safe,and request is satisfied
	Enter Command: RQ 3 1 1 1 1
	State is safe,and request is satisfied
	Enter Command: RQ 4 1 0 0 0
	State is safe,and request is satisfied
	Enter Command: Status
	Available Resources:
	4 1 3 3
	Maximum Resources:
	6 4 7 3
	4 2 3 2
	2 5 3 3
	6 3 3 2
	5 5 7 5
	Allocated Resources:
	1 0 0 1
	1 1 1 1
	2 2 2 2
	1 1 1 1
	1 0 0 0
	Need Resources:
	5 4 7 2
	3 1 2 1
	0 3 1 1
	5 2 2 1
	4 5 7 5
	Enter Command: Run
	Safe Sequence is: 1 3 2 4 0
	--> Customer/Thread 1
		 Allocated resources: 1 1 1 1
		 Needed: 3 1 2 1
		 Available: 4 1 3 3
		 Thread has started
		 Thread has finished
		 Thread is releasing resources
		 New Available: 5 2 4 4
	--> Customer/Thread 3
		 Allocated resources: 1 1 1 1
		 Needed: 5 2 2 1
		 Available: 5 2 4 4
		 Thread has started
		 Thread has finished
		 Thread is releasing resources
		New Available: 6 3 5 5
	--> Customer/Thread 2
		 Allocated resources: 2 2 2 2
		 Needed: 0 3 1 1
		 Available: 6 3 5 5
		 Thread has started
		 Thread has finished
		 Thread is releasing resources
		 New Available: 8 5 7 7
	--> Customer/Thread 4
		 Allocated resources: 1 0 0 0
		 Needed: 4 5 7 5
		 Available: 8 5 7 7
		 Thread has started
		 Thread has finished
		 Thread is releasing resources
		
	--> Customer/Thread 0
		 Allocated resources: 1 0 0 1
		 Needed: 5 4 7 2
		 Available: 9 5 7 7
		 Thread has started
		 Thread has finished
		 Thread is releasing resources
		 New Available: 10 5 7 8
	Enter Command:

Use examples

	This can be used in any such scenario in which there are limited resources and they need to be applied appropritatly in order to avoid any problems
	with deadlocks. The program is built in order to accomodate any file structure that includes more or less customers and resources.
	
Individual contribution
	
	All code provided has been implemented by myself and citations have been provided accordingly.

Installation

	In order to implement this program, the user simply needs access to a linux based operating system, which can then run the 
	program through the makefile provided.
	
About Developers

	I am a student at Wilfrid Laurier University in the discipline of computer science.

Author
	
	Zack Dewan

License
