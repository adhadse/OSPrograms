/**
 * @author Anurag Dhadse
 * Bankers' Algorithm for DeadLock Avoidance
 */

#include <iostream>
#include <vector>
#include <list>

using namespace std;

class Process {
    public: 
        const int processID;
        vector<int> allocatedResources;
        vector<int> maxNeedOfResources;
        vector<int> remainingNeedOfResources;

        void findRemainingNeedOfResources() {
            for (size_t index=0; index<this->allocatedResources.size(); index++) {
                this->remainingNeedOfResources.push_back(
                    maxNeedOfResources[index] - allocatedResources[index]);
            }
        }

        Process (int processID, std::vector<int> allocatedResources, std::vector<int> maxNeedOfResources) :
            processID(processID),
            allocatedResources{allocatedResources},
            maxNeedOfResources{maxNeedOfResources} {
                findRemainingNeedOfResources();
            } 
        
        bool operator==(const Process &prc) const {
            return prc.processID == this->processID;
        }

};

class BankersAlgorithm {
    private:
        vector<int> const totalResources;
        vector<int> remainingResources;
        list<Process> processList;

        // add up all the allocated resources 
        // and substract from totalResources
        // Puts it in remaining Resources
        void findRemainingResources() {
            // initializing empty 0 vector for storing total of resources already allocated
            vector<int> totalAllocatedResources(totalResources.size()); 
            for (auto & process: processList) {
                for (size_t index=0; index<totalAllocatedResources.size(); index++) {
                    totalAllocatedResources[index] += process.allocatedResources[index];
                }
            }
            
            for (size_t index=0; index<totalAllocatedResources.size(); index++) {
                this->remainingResources.push_back(totalResources[index] - totalAllocatedResources[index]);
            }
        }

        // Find Process which satisfy current Remaining Resources
        Process findNextProcess() {
            for (auto & process: processList) {
                if (process.remainingNeedOfResources <= this->remainingResources) {
                    return process;
                }
            }
            throw string("No process anymore");
        }

        void printvector(vector<int> vec) {
            cout<<"["; 
            for (auto element: vec) {
                cout<<element<<", ";
            } cout<<"]"<<endl;
        }
            
    public:
        // Constructor
        BankersAlgorithm(std::list<Process> processList, std::vector<int> totalResources) : 
                processList{processList}, 
                totalResources{totalResources}{};
    
        void run() {
            findRemainingResources();
            //setRemainingNeedOfResources();
            while (!processList.empty()) {
                try{
                    Process process = findNextProcess();
                    cout<< "Executing ProcessID " << process.processID<<"\n";
                    for (size_t index=0; index<process.allocatedResources.size(); index++) {
                        // Set the current remaining resources after the process finished execution 
                        remainingResources[index] += process.allocatedResources[index];
                    }

                    processList.remove(process);
                }
                catch (string std) { cout<<std; return;}
            }

            //Check if Total Resources are reaquired
            bool flag = true;
            for (size_t index=0; index<remainingResources.size(); index++) {
                if (totalResources[index] == remainingResources[index]) {
                    if  (flag == false) {} else {flag=true;}
                } else { flag = false;}
            }
            cout<<"\nTotal Resources                     : "; printvector(this->totalResources);
            cout<<"Available Resources after Execution : "; printvector(this->remainingResources);

            (flag == true)? cout<<"Test passed\n" : cout<<"Test Failed\n";
        }

};

int main() {
    vector<int> vect1={0, 1, 0};
    vector<int> vect2={7, 5, 3};
    vector<int> vect3={2, 0, 0};
    vector<int> vect4={3, 2, 2};
    vector<int> vect5={3, 0, 2};
    vector<int> vect6={9, 0, 2};
    vector<int> vect7={2, 1, 1};
    vector<int> vect8={4, 2, 2};
    vector<int> vect9={0, 0, 2};
    vector<int> vect10={5, 3, 3};

    //Process(Processid, vec<int> allocatedRes, vec<int> maxNeedOfRes)
    list<Process> processList = {
        Process(1, vect1, vect2),
        Process(2, vect3, vect4),
        Process(3, vect5, vect6),
        Process(4, vect7, vect8),
        Process(5, vect9, vect10)
    };
    vector<int> totalres = {10, 5, 7};
    BankersAlgorithm ba(processList, totalres);
    ba.run();
    return 0;
}
