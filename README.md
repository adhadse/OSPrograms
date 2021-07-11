# OS_programs
This repo holds my OS Lab programs for the Fourth Semester. 
## Feel free to use and learn from them.

### 📜CPUScheduler.cpp 
This file holds `Scheduler` class for most commonly used functions and well as `Process` class for holding attributes of Process (Sort of Process Control Block). 

The `Scheduler` class and classes which inherits this class uses `Process` objects to manipulate and simulate a certain type of scheduling. Examples of using these specialized class can be seen from the function written at end of the file, to learn the execution pattern.

The scheduler implemented are :
- Shortest Job First (SJF)
- Priority Scheduler with Non-Premption
- Priority Scheduler with Premption
- Shortest Remaining Time First (SRTF) 
- Round Robin

The classes consist of (usually) 3 function, either inherited or overrided depending on the requirement of scehduler. These are:
- `arrivedProcesses()`: Describe which processes have arrived at a certain period of time for clock cycle.
- `nextSuitableProcess()`: Find the next suitable process from process list returned by `arrivedProcesses()` to be executed next (depending on the scheduling algorithm). 
- `execute()`: This function execute the process one by one by calling `nextSuitableProcess()`. This is either implemented in Pre-emptive or Non-Premptive mode.

### 📜 BankersAlgorithm.cpp 
This file implement and simulate a bankers algorithm in `BankersAlgorithm` on process objects of `Process` class.

### 📜 PageReplacer.kt
This file simulate a First-In First-Out (FIFO) Page Replacement algorithm in Kotlin

### 📜 CPUScheduler.kt 
This file was originally written for scheduler class but later on I wrote most of them in C++. 

If you love ♥ to see the code in KOTLIN, which is much more easier to read; read this to get a hang of the code before switching to C++ programs.
