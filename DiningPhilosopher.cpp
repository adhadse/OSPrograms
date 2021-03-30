/**
 * @author Anurag Dhadse
 * Dining Philosopher
 */

#include <iostream>
#include <vector>
#include <list>

using namespace std;

class Fork {
    public:
        int forkNo;
        bool taken;

        Fork(int forkNo, bool taken=false) :
            forkNo(forkNo),
            taken(taken) {}
};

class Philosopher {
    public:
        int philosopherNo;
        bool ate;
        int forkLeft;
        int forkRight;

        Philosopher(int philosopherNo, bool ate=false) :
            philosopherNo{philosopherNo},
            ate(ate) {
                forkRight = 0;
                forkLeft = 0;
            }
};

class Eat {
    private:
        vector<Fork> forkList;
        vector<Philosopher> philosopherList;
        
        bool allPhilosopherNotAte() {
            for(auto & philosopher: philosopherList) {
                if (philosopher.ate == false) {
                    return true;
                }
            }
            return false;
        }

        void printStatus() {
            for(auto & philosopher: philosopherList) {
                if (philosopher.forkRight == 0 || philosopher.forkRight == 0){
                     if (philosopher.ate == true) {
                            cout<<"Philosopher no: "<< philosopher.philosopherNo << " Finished Eating.\n";
                            continue;
                    }
                     cout<<"Philosopher no: "<< philosopher.philosopherNo << " Waiting.\n";
                     
                } else {
                    cout<<"Philosopher no: "<< philosopher.philosopherNo << " eating. ";
                    cout<<"Took Fork no: "<< philosopher.forkLeft <<" and "<< philosopher.forkRight<<"\n";
                }
            }
            cout<<"\n";
        }

    public:
        Eat(vector<Fork> forkList, vector<Philosopher> philosopherList):
            forkList(forkList),
            philosopherList(philosopherList) {}

        void eating() {
            printStatus();
            while(allPhilosopherNotAte()) {
                for (auto & philosopher: philosopherList) {
                    if (philosopher.ate == false) {
                        if (philosopher.philosopherNo == philosopherList.front().philosopherNo ) {
                            philosopher.ate = true;
                            philosopher.forkLeft = forkList.back().forkNo;
                            philosopher.forkRight = forkList.front().forkNo;
                            printStatus();
                            philosopher.forkLeft = 0;
                            philosopher.forkRight = 0;
                        } else {
                            if (philosopher.philosopherNo == philosopherList.back().philosopherNo) {
                                philosopher.ate = true;
                                philosopher.forkLeft = forkList.front().forkNo;
                                philosopher.forkRight = forkList.back().forkNo;
                                printStatus();
                                philosopher.forkLeft = 0;
                                philosopher.forkRight = 0;
                            } else {
                                philosopher.ate = true;
                                philosopher.forkLeft = forkList[philosopher.philosopherNo - 2].forkNo;
                                philosopher.forkRight = forkList[philosopher.philosopherNo - 1].forkNo;
                                printStatus();
                                philosopher.forkLeft = 0;
                                philosopher.forkRight = 0;
                            }
                        }
                    }
                }
                printStatus();
            }
        }
};

int main() {
    vector<Philosopher> phil = {1, 2, 3, 4};
    vector<Fork> fork = {1, 2, 3};
    Eat eat(fork, phil);
    eat.eating();
    return 0;
}