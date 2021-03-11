/**
 * @author Anurag Dhadse
 */

#include <iostream>
#include <vector>
#include <deque>
#include <algorithm>

using namespace std;

class Process {
    public:
        int processId;
        int arrivalTime;
        int burstTime;
        int priority;
        int completionTime;
        int turnAroundTime;
        int waitingTime;
        int remainingBurstTime;
        bool completed= false;

        Process(int proccesId, int arrivalTime, int burstTime) {
            this->processId = proccesId;
            this->arrivalTime = arrivalTime;
            this->burstTime = burstTime;
            this->remainingBurstTime = burstTime;
        }
        Process(int proccesId, int arrivalTime, int burstTime, int priority) {
            this->processId = proccesId;
            this->arrivalTime = arrivalTime;
            this->burstTime = burstTime;
            this->remainingBurstTime = burstTime;
            this->priority = priority;
        }

        void setTurnAroundTime(){
            this->turnAroundTime = this->completionTime - this->arrivalTime;
        }
        void setWaitingTime(){
            this->waitingTime = this->turnAroundTime - this->burstTime; 
        }
        
};

class Scheduler {
    protected:
        int clock = 0;
        std::vector<Process> processList;
    public:
        Scheduler(){};
        Scheduler(int clock, std::vector<Process> processList) : clock{clock}, processList{processList}{};
        vector<Process*> inCompleteProcesses() {
            // Returns ALL incomplete processes
            vector<Process*> result;
            // cout<<"In incompleteprocesses\n";
            for (auto & process : processList) {
                if (!process.completed) {
                    result.push_back(&process);
                }
            }
            return result;
        }
        Process* minByArrivalTime(vector<Process*> arrivedProcesses) {
            // Finds process by minimum arrival time 
            // in arrived Process List
            Process* resultProcess = arrivedProcesses[0];
            for (auto & process: arrivedProcesses) {
                if (process->arrivalTime < resultProcess->arrivalTime) {
                    resultProcess = process;
                } 
            }
            return resultProcess;
        }
        Process* minByBurstTime(vector<Process*> arrivedProcesses) {
            // Finds process by minimum burst time 
            // in arrived Process List
            Process* resultProcess = arrivedProcesses[0];
            for (auto & process: arrivedProcesses) {
                if (process->burstTime < resultProcess->burstTime) {
                    resultProcess = process;
                } 
            }
            return resultProcess;
        }
        Process* highestPriority(vector<Process*> arrivedProcesses) {
            // Finds process by highest priority 
            // in arrived Process List
            Process* resultProcess = arrivedProcesses[0];
            for (auto & process: arrivedProcesses) {
                if (process->priority > resultProcess->priority) {
                    resultProcess = process;
                } 
            }
            return resultProcess;
        }
        Process* shortestRemainingTime(vector<Process*> arrivedProcesses) {
            // Finds process by highest priority 
            // in arrived Process List
            Process* resultProcess = arrivedProcesses[0];
            for (auto & process: arrivedProcesses) {
                if (process->remainingBurstTime < resultProcess->remainingBurstTime) {
                    resultProcess = process;
                } 
            }
            return resultProcess;
        }
        
        vector<Process*> arrivedProcesses() {
            // This function creates a list of all process which are not completed
            // and has arrived after CPU clock finished executing process.
            vector<Process*> incompleteProcesses = inCompleteProcesses();
            vector<Process*> arrivedProcesses;
            if(!incompleteProcesses.empty()) {
                // If there are Incomplete Processes
                for (Process* process: incompleteProcesses) {
                    if ((*process).arrivalTime <= clock) {
                        // If process has already arrived push back those process
                        arrivedProcesses.push_back(process);
                    } else
                    if (arrivedProcesses.empty()) {
                        // When CPU is IDLE, no process has arrived
                        arrivedProcesses.push_back(minByArrivalTime(incompleteProcesses));
                    }   
                }
                return arrivedProcesses;
            }
            else { throw string("No Arrived Processes!");}
        } 

        virtual Process* nextSuitableProcess() {
            //This  function finds the next suitable process for execution
            // Override in child class
            cout<<"Using Scheduler Class(Default)\n";
            throw string("No suitable Process Found!(Default)\n");
        }

        virtual void execute() {
            // This function execute the process one 
            // by one by calling nextSuitableProcess()
            while (true) {
                try {
                    Process* process = nextSuitableProcess();
                    cout<<"Executing ProcessID "<< (*process).processId<<"\n";

                    // Set the clock to arrival of late arrived process arrival time i.e. CPU was sitting idle.
                    if (clock < (*process).arrivalTime) {clock = (*process).arrivalTime;}
                    clock += (*process).burstTime;
                    (*process).completionTime = clock;
                    (*process).setTurnAroundTime();
                    (*process).setWaitingTime();
                    (*process).completed=true;
                } 
                catch (string std) {
                    // If no suitable processes
                    cout<<std;
                    return;
                } 
            }
        }

        virtual void showSchedule() {
            // Prints the scheduling time and showing the times of process
            cout<<"Process_id Arrival_Time Burst_Time Completion_Time Turn_Around_Time Waiting_Time\n";
            float avgTurnAroundTime = 0.0f;
            float avgWaitingTime = 0.0f;
            for (auto & process : processList) {
                cout<<process.processId
                    <<"          "<<process.arrivalTime
                    <<"            "<<process.burstTime
                    <<"          "<<process.completionTime
                    <<"                "<<process.turnAroundTime
                    <<"                "<<process.waitingTime<<"\n";
            avgTurnAroundTime += process.turnAroundTime;
            avgWaitingTime += process.waitingTime;
            }
            cout<<"---------------------\nAverage Turn Around Time "<<avgTurnAroundTime/processList.size()<<"\n";
            cout<<"Average Waiting Time "<<avgWaitingTime/processList.size()<<"\n";
        }
};

// Inherits From Scheduler Class
class ShortestJobFirst: public Scheduler {
    public:
        ShortestJobFirst(int clock, vector<Process> processList) : Scheduler(clock, processList) {}
    
        Process* nextSuitableProcess() override {
            try{
                auto process = minByBurstTime( arrivedProcesses() );
                return process;
            }
            catch (string std) {
                // If No arrived Processes;
                cout<<std<<"\n";
                throw string("No Suitable (SJF) Process Found!\n");
            }
        }
};

// Inherits From Scheduler Class
class PriorityWithNonPreEmption: public Scheduler {
    public:
        PriorityWithNonPreEmption(int clock, vector<Process> processList) : Scheduler(clock, processList) {}

        Process* nextSuitableProcess() override {
            try{
                auto process = highestPriority( arrivedProcesses() );
                return process;
            }
            catch (string std) {
                //If NO arrived Processes;
                cout<<std<<"\n";
                throw string("No Suitable (Priority) Process Found!\n");
            }
        } 

        void showSchedule() override {
            // Prints the scheduling time and priority of process
            cout<<"Process_id Priority Arrival_Time Burst_Time Completion_Time Turn_Around_Time Waiting_Time\n";
            float avgTurnAroundTime = 0.0f;
            float avgWaitingTime = 0.0f;
            for (auto & process : processList) {
                cout<<process.processId
                    <<"          "<<process.priority
                    <<"        "<<process.arrivalTime
                    <<"            "<<process.burstTime
                    <<"          "<<process.completionTime
                    <<"                "<<process.turnAroundTime
                    <<"                "<<process.waitingTime<<"\n";
            avgTurnAroundTime += process.turnAroundTime;
            avgWaitingTime += process.waitingTime;
            }
            cout<<"---------------------\nAverage Turn Around Time "<<avgTurnAroundTime/processList.size()<<"\n";
            cout<<"Average Waiting Time "<<avgWaitingTime/processList.size()<<"\n";
        }
};

// Inherits From Scheduler Class
class ShortestRemainingTimeFirst: public Scheduler {
    public:
        ShortestRemainingTimeFirst(int clock, vector<Process> processList) : Scheduler(clock, processList) {}

        Process* nextSuitableProcess() override {
            try {
                auto process = shortestRemainingTime( arrivedProcesses() );
                return process;
            }
            catch (string std) {
                //If NO arrived Processes;
                cout<<std<<"\n";
                throw string("No Suitable (Shortest Remaining Time) Process Found!\n");
            }
        }

        void execute() override {
            // This function execute the process one 
            // by one by calling nextSuitableProcess()
            while (true) {
                try {
                    Process* process = nextSuitableProcess();
                    cout<<"Executing ProcessID "<< (*process).processId<<"\n";

                    // Set the clock to arrival of late arrived process arrival time i.e. CPU was sitting idle.
                    if (clock < process->arrivalTime) {clock = process->arrivalTime;}
                    clock += 1;
                    process->remainingBurstTime -= 1;
                    if (process->remainingBurstTime == 0) {
                        process->completionTime = clock;
                        process->setTurnAroundTime();
                        process->setWaitingTime();
                        process->completed=true;
                        continue;
                    }   
                }
                catch (string std) {
                    // If no suitable processes
                    cout<<std;
                    return;
                } 
            }
        }

};

//Inherits from Scheduler Class
class RoundRobin: public Scheduler {
    int const quantum;
    deque<Process*> readyQueue;
    Process* executedProcess;
    // bool alreadyInExecutedProcess(Process* arrivedprocess) {
    //     if (std::find(executedProcess.begin(), executedProcess.end(), arrivedprocess ) != executedProcess.end()) {
    //         return true;
    //     }
    //     return false;
    // }
    bool alreadyInReadyQueue(Process* arrivedprocess) {
        if (std::find(readyQueue.begin(), readyQueue.end(), arrivedprocess ) != readyQueue.end()) {
            return true;
        }
        return false;
    }
    public:
        RoundRobin(int clock, vector<Process> processList, int quantum=2) 
            : Scheduler(clock, processList),
              quantum(quantum) {}
        Process* nextSuitableProcess() override {
            try {
                vector<Process*> arrivedprocesses = arrivedProcesses();
                
                for (auto process: arrivedprocesses) {
                    if(alreadyInReadyQueue(process)){
                        // Don't Do anything 
                    } else {
                        this->readyQueue.push_back(process);
                    }
                }

                readyQueue.push_back(this->executedProcess);
                return this->readyQueue.front();
            }
            catch (string std) {
                //If no arrived Processes;
                cout<<std<<"\n";
                throw string("No Suitable (Round Robin) Process Found!\n"); 
            }
        }
        void execute() override {
            // This function execute the process one 
            // by one by calling nextSuitableProcess()
            while (true) {
                try {
                    Process* process = nextSuitableProcess();
                    cout<<"Executing ProcessID "<< (*process).processId<<"\n";

                    // Set the clock to arrival of late arrived process arrival time i.e. CPU was sitting idle.
                    if (this->clock < process->arrivalTime) {this->clock = process->arrivalTime;}
                    if(process->remainingBurstTime < quantum ) {
                        clock += process->remainingBurstTime;
                        process->remainingBurstTime = 0;
                    } else {
                        clock += quantum;
                        process->remainingBurstTime -= quantum;
                    }
                    
                    if (process->remainingBurstTime == 0) {
                        process->completionTime = clock;
                        process->setTurnAroundTime();
                        process->setWaitingTime();
                        process->completed=true;
                        this->readyQueue.pop_front();
                        continue;
                    }   else {
                            this->executedProcess = process;
                        this->readyQueue.pop_front();
                    }
                }
                catch (string std) {
                    // If no suitable processes
                    cout<<std;
                    return;
                } 
            }
        }

};

// Inherits From Scheduler Class
class PriorityWithPreEmption: public Scheduler {
    public:
        PriorityWithPreEmption(int clock, vector<Process> processList) : Scheduler(clock, processList) {}

        Process* nextSuitableProcess() override {
            try{
                auto process = highestPriority( arrivedProcesses() );
                return process;
            }
            catch (string std) {
                //If NO arrived Processes;
                cout<<std<<"\n";
                throw string("No Suitable (Priority) Process Found!\n");
            }
        }

        void execute() override {
            // This function execute the process one 
            // by one by calling nextSuitableProcess()
            while (true) {
                try {
                    Process* process = nextSuitableProcess();
                    cout<<"Executing ProcessID "<< (*process).processId<<"\n";

                    // Set the clock to arrival of late arrived process arrival time i.e. CPU was sitting idle.
                    if (clock < process->arrivalTime) {clock = process->arrivalTime;}
                    clock += 1;
                    process->remainingBurstTime -= 1;
                    if (process->remainingBurstTime == 0) {
                        process->completionTime = clock;
                        process->setTurnAroundTime();
                        process->setWaitingTime();
                        process->completed=true;
                        continue;
                    }   
                }
                catch (string std) {
                    // If no suitable processes
                    cout<<std;
                    return;
                } 
            }
        }
        
        void showSchedule() override {
            // Prints the scheduling time and priority of process
            cout<<"Process_id Priority Arrival_Time Burst_Time Completion_Time Turn_Around_Time Waiting_Time\n";
            float avgTurnAroundTime = 0.0f;
            float avgWaitingTime = 0.0f;
            for (auto & process : processList) {
                cout<<process.processId
                    <<"          "<<process.priority
                    <<"        "<<process.arrivalTime
                    <<"            "<<process.burstTime
                    <<"          "<<process.completionTime
                    <<"                "<<process.turnAroundTime
                    <<"                "<<process.waitingTime<<"\n";
            avgTurnAroundTime += process.turnAroundTime;
            avgWaitingTime += process.waitingTime;
            }
            cout<<"---------------------\nAverage Turn Around Time "<<avgTurnAroundTime/processList.size()<<"\n";
            cout<<"Average Waiting Time "<<avgWaitingTime/processList.size()<<"\n";
        }    
};


void priorityWithNonPreEmption() {
    vector<Process> processList = {
        Process(1,0,4,2),
        Process(2,1,2,4),
        Process(3,2,3,6),
        Process(4,3,5,10),
        Process(5,4,1,8),
        Process(6,5,4,12),
        Process(7,6,6,9)
    };
    PriorityWithNonPreEmption pnp(0, processList);
    pnp.execute();
    pnp.showSchedule();
}
void sjf() {
    vector<Process> processList = {
        Process(1, 2, 6),
        Process(2, 5, 2),
        Process(3, 1, 8),
        Process(4, 0, 3),
        Process(5, 4, 4)
    };
    ShortestJobFirst sjf(0, processList);
    sjf.execute();
    sjf.showSchedule();
}
void shortestRemainingTime() {
    vector<Process> processList = {
        Process(1,0,8),
        Process(2,1,4),
        Process(3,2,2),
        Process(4,3,1),
        Process(5,4,3),
        Process(6,5,2),
    };
    ShortestRemainingTimeFirst srtf(0, processList);
    srtf.execute();
    srtf.showSchedule();
}
void roundRobin() {
    vector<Process> processList = {
        Process(1,0,5),
        Process(2,1,4),
        Process(3,2,2),
        Process(4,4,1)
    };
    vector<Process> processList2 = {
        Process(1, 1,   4),
        Process(2, 1, 	5),
        Process(3, 2, 	2),
        Process(4, 3, 	1),
        Process(5, 4,	6),
        Process(6, 6,	3),
        
    };
    RoundRobin rr(0, processList2);
    rr.execute();
    rr.showSchedule();
}

void priorityWithPreEmption(){
    vector<Process> processList = {
        Process(1,0,5,10),
        Process(2,1,4,20),
        Process(3,2,2,30),
        Process(4,4,1,40)
    };
    PriorityWithPreEmption pwp(0, processList);
    pwp.execute();
    pwp.showSchedule();
}
int main() {
    roundRobin();
    return 0;
}