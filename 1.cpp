#include <iostream>
#include <vector>
#include <climits>
#include <queue>
#include <cmath>
#include <algorithm>
#include <time.h>
#include <math.h>
#include <cmath>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
using namespace std;
int Time_Quantam_RR = 10;
int Time_Slice_MLFQ = 20;
bool isInit1 = false, isInit2 = false;
struct Process{
    int pid;
    int arrivalTime;
    int burstTime;
    int priority;
};
bool compareByPriority(const Process &p1, const Process &p2){
    return p1.priority > p2.priority;
}
bool compareByArrivalTimeAndInversePriority(const Process &p1, const Process &p2){
    if (p1.arrivalTime < p2.arrivalTime)
        return true;
    else if (p1.arrivalTime == p2.arrivalTime){
        if (p1.priority > p2.priority)
            return true;
        else
            return false;
    }
    else
        return false;
}
bool compareByArrivalTime(const Process &p1, const Process &p2){
    return p1.arrivalTime < p2.arrivalTime;
}
void controlPriority(vector<Process> priority, vector<int> &safeSeq, vector<int> &safe){
    vector<int> instance;
    for (int currentTime = 0; true; ++currentTime){
        int target = 0;
        for (int i = 1; i < priority.size(); ++i){
            if (currentTime >= priority[i].arrivalTime && priority[i].priority > priority[target].priority)
                target = i;
        }
        priority[target].burstTime--;
        if ((instance.size() == 0) || instance[instance.size() - 1] != priority[target].pid)
            instance.push_back(priority[target].pid);
        if (priority[target].burstTime == 0){
            safe[priority[target].pid] = currentTime;
            priority.erase(priority.begin() + target);
        }
        if (!priority.size())
            break;
    }
    for (int i = 0; i < instance.size(); ++i)
        safeSeq.push_back(instance[i]);
}
void controlRoundRobin(vector<Process> RoundRobin, vector<int> &safeSeq, vector<int> &safe){
    for (int i = 0, j = 0; true;){
        if (i >= RoundRobin[j].arrivalTime){
            if (RoundRobin[j].burstTime > 0){
                safeSeq.push_back(RoundRobin[j].pid);
                RoundRobin[j].burstTime -= Time_Quantam_RR;
                i += Time_Quantam_RR;
            }
            else{
                safe[RoundRobin[j].pid] = i;
                RoundRobin.erase(RoundRobin.begin() + j);
                if (RoundRobin.size() == 0)
                    break;
                --j;
            }
            j = (j + 1) % RoundRobin.size();
        }
        else
            i++;
    }
}
void controlSRJF(vector<Process> proc, vector<int> &safeSeq, vector<int> &safe){
    vector<int> instance;
    for (int currentTime = proc[0].arrivalTime; true; ++currentTime){
        int target = 0;
        for (int i = 1; i < proc.size(); ++i){
            if (currentTime >= proc[i].arrivalTime && proc[i].burstTime < proc[target].burstTime)
                target = i;
        }
        proc[target].burstTime--;
        if ((instance.size() == 0) || instance[instance.size() - 1] != proc[target].pid)
            instance.push_back(proc[target].pid);
        if (proc[target].burstTime == 0){
            safe[proc[target].pid] = currentTime;
            proc.erase(proc.begin() + target);
        }
        if (!proc.size())
            break;
    }
    for (int i = 0; i < instance.size(); ++i)
        safeSeq.push_back(instance[i]);
}
void controlSJF(vector<Process> shortestRJF, vector<int> &safeSeqRR, vector<int> &safe){
    int savePidState = shortestRJF[0].pid;
    for (int i = 0; i < shortestRJF.size(); ++i)
        shortestRJF[i].pid -= savePidState;
    int time = 0;
    int running_time = 0;
    int smallest_A_T = INT_MAX;
    int save_process_id = 0;
    int numOfProcesses = shortestRJF.size();
    smallest_A_T = shortestRJF[0].arrivalTime;
    for (int i = 1; i < numOfProcesses; i++){
        if (shortestRJF[i].arrivalTime < smallest_A_T){
            smallest_A_T = shortestRJF[i].arrivalTime;
            save_process_id = i;
        }
    }
    safeSeqRR.push_back(save_process_id + savePidState);
    int greatest_b_t = shortestRJF[save_process_id].burstTime;
    for (int i = 0; i < numOfProcesses; i++){
        if (shortestRJF[i].burstTime < greatest_b_t && shortestRJF[i].arrivalTime <= smallest_A_T){
            greatest_b_t = shortestRJF[i].burstTime;
            save_process_id = i;
        }
    }
    running_time += greatest_b_t;
    shortestRJF.erase(shortestRJF.begin() + save_process_id);
    int del_ind = 0;
    for (int j = 0; j < shortestRJF.size();){
        greatest_b_t = shortestRJF[j].burstTime;
        save_process_id = shortestRJF[j].pid;
        del_ind = j;
        for (int k = 0; k < shortestRJF.size(); k++){
            if (shortestRJF[k].burstTime < greatest_b_t && shortestRJF[k].arrivalTime <= running_time){
                del_ind = k;
                greatest_b_t = shortestRJF[k].burstTime;
                save_process_id = shortestRJF[k].pid;
            }
        }
        running_time += greatest_b_t;
        safeSeqRR.push_back(save_process_id + savePidState);
        shortestRJF.erase(shortestRJF.begin() + del_ind);
    }
}
bool MLFQDone(vector<vector<Process> > q){
    for (int i = 0; i < q.size(); ++i){
        if (q[i].size() != 0)
            return false;
    }
    return true;
}
char randomChar(){
    srand(time(0));
    return (char)((rand() % 26) + 97);
}
int getDecimalValue(int arr[], int start, int end){
    int total = 0;
    for (int i = end, j = 0; i >= start; --i, ++j)
        total += arr[i] * pow(2, j);
    return total;
}
void controlMLQS(){
    int numOfProcesses;
    cout << "Enter the number of processes: ";
    cin >> numOfProcesses;
    vector<Process> processes;
    processes.resize(numOfProcesses);
    cout << "PID\tArrival Time\tBurst Time\tPriority" << endl;
    for (int i = 0; i < numOfProcesses; ++i){
        cout << i << "\t";
        processes[i].pid = i;
        cin >> processes[i].arrivalTime >> processes[i].burstTime >> processes[i].priority;
    }
    sort(processes.begin(), processes.end(), compareByArrivalTime);
    vector<Process> priority;
    vector<Process> RoundRobin;
    vector<Process> shortestRJF;
    for (int i = 0; i < numOfProcesses; ++i){
        if (i < numOfProcesses / 3)
            priority.push_back(processes[i]);
        else if (i < (numOfProcesses * 2) / 3)
            RoundRobin.push_back(processes[i]);
        else
            shortestRJF.push_back(processes[i]);
    }
    vector<int> safeSeq;
    vector<int> safeCompletionTime;
    safeCompletionTime.resize(processes.size());
    controlPriority(priority, safeSeq, safeCompletionTime);
    controlRoundRobin(RoundRobin, safeSeq, safeCompletionTime);
    controlSRJF(shortestRJF, safeSeq, safeCompletionTime);
    cout << endl;
    cout << "Safe Sequence: | ";
    for (int i = 0; i < safeSeq.size(); ++i)
        cout << "P" << safeSeq[i] << " | ";
    cout << endl;
    cout << "PID\t\tTurnAround Time\t\tWaitingTime" << endl;
    for (int i = 0; i < processes.size(); ++i){
        int instance1, instance2;
        instance1 = safeCompletionTime[i] - processes[i].arrivalTime;
        instance1++;
        instance2 = instance1 - processes[i].burstTime;
        cout << i << "\t\t" << instance1 << "\t\t\t" << instance2 << endl;
    }
    cout << endl;
}
void controlMLFQ(){
    int numOfProcesses;
    cout << "Enter the number of processes: ";
    cin >> numOfProcesses;
    vector<Process> processes;
    processes.resize(numOfProcesses);
    cout << "PID\tArrival Time\tBurst Time\tPriority" << endl;
    for (int i = 0; i < numOfProcesses; ++i){
        cout << i << "\t";
        processes[i].pid = i;
        cin >> processes[i].arrivalTime >> processes[i].burstTime >> processes[i].priority;
    }
    vector<Process> FCFS;
    vector<Process> RoundRobin;
    vector<Process> shortestRJF;
    vector<vector<Process> > MLFQs;
    FCFS = processes;
    MLFQs.push_back(FCFS);
    MLFQs.push_back(RoundRobin);
    MLFQs.push_back(shortestRJF);
    vector<int> safeCompletionTime;
    safeCompletionTime.resize(processes.size());
    vector<int> safeSeq;{
        sort(MLFQs[0].begin(), MLFQs[0].end(), compareByArrivalTime);
        int currentQueue = 0;
        bool change = true;
        int currentProcessRR = 0;
        int smallestJob;
        for (int currentTime = 1; true; ++currentTime){
            if (currentTime % Time_Slice_MLFQ == 0){
                if (currentQueue == 1){
                    MLFQs[2].push_back(MLFQs[1][smallestJob]);
                    MLFQs[1].erase(MLFQs[1].begin() + smallestJob);
                }
                else{
                    MLFQs[((currentQueue + 1) % MLFQs.size())].push_back(MLFQs[currentQueue][0]);
                    MLFQs[currentQueue].erase(MLFQs[currentQueue].begin() + 0);                }
                if (MLFQs[currentQueue].size() == 0){
                    change = true;
                    currentQueue = (currentQueue + 1) % 3;
                }
                if (currentQueue == 0)
                    sort(MLFQs[0].begin(), MLFQs[0].end(), compareByArrivalTime);
            }
            else{
                if (currentQueue == 0){
                    if (currentTime >= MLFQs[0][0].arrivalTime){
                        MLFQs[0][0].burstTime -= 1;
                        if (change || safeSeq[safeSeq.size() - 1] != MLFQs[0][0].pid){
                            change = false;
                            safeSeq.push_back(MLFQs[0][0].pid);
                        }
                        if (MLFQs[0][0].burstTime == 0){
                            safeCompletionTime[MLFQs[0][0].pid] = currentTime;
                            MLFQs[0].erase(MLFQs[0].begin());
                        }
                        if (MLFQs[0].size() == 0){
                            change = true;
                            currentQueue = (currentQueue + 1) % 3;
                        }
                    }
                }
                else if (currentQueue == 1){
                    smallestJob = 0;
                    for (int i = 1; i < MLFQs[1].size(); ++i){
                        if (MLFQs[1][i].burstTime < MLFQs[1][smallestJob].burstTime){
                            smallestJob = i;
                        }
                    }
                    if (safeSeq[safeSeq.size() - 1] != MLFQs[1][smallestJob].pid || change){
                        change = false;
                        safeSeq.push_back(MLFQs[1][smallestJob].pid);
                    }
                    MLFQs[1][smallestJob].burstTime -= 1;
                    if (MLFQs[1][smallestJob].burstTime <= 0){
                        safeCompletionTime[MLFQs[1][smallestJob].pid] = currentTime;
                        MLFQs[1].erase(MLFQs[1].begin() + smallestJob);
                    }
                    if (MLFQs[1].size() == 0){
                        change = true;
                        currentQueue = (currentQueue + 1) % 3;
                    }
                }
                else{
                    if (currentTime % Time_Quantam_RR == 0){
                        change = true;
                        currentProcessRR = (currentProcessRR + 1) % MLFQs[2].size();
                    }
                    if (change || safeSeq[safeSeq.size() - 1] != MLFQs[2][currentProcessRR].pid){
                        change = false;
                        safeSeq.push_back(MLFQs[2][currentProcessRR].pid);
                    }
                    MLFQs[2][currentProcessRR].burstTime--;
                    if (MLFQs[2][currentProcessRR].burstTime <= 0){
                        safeCompletionTime[MLFQs[2][currentProcessRR].pid] = currentTime;
                        MLFQs[2].erase(MLFQs[2].begin() + currentProcessRR);
                    }
                    if (MLFQs[2].size() == 0){
                        change = true;
                        currentQueue = (currentQueue + 1) % 3;
                    }
                }
            }
            if (MLFQDone(MLFQs))
                break;
        }
    }
    cout << endl;
    cout << "Safe Sequence: | ";
    for (int i = 0; i < safeSeq.size(); ++i)
        cout << "P" << safeSeq[i] << " | ";
    cout << endl;
    cout << "PID\t\tTurnAround Time\t\tWaitingTime" << endl;
    for (int i = 0; i < processes.size(); ++i){
        int instance1, instance2;
        instance1 = safeCompletionTime[i] - processes[i].arrivalTime;
        instance2 = instance1 - processes[i].burstTime;
        cout << i << "\t\t" << instance1 << "\t\t\t" << instance2 << endl;
    }
    cout << endl;
}
void controlLJF(){
    int numOfProcesses;
    cout << "Enter the number of processes: ";
    cin >> numOfProcesses;
    vector<Process> processes;
    processes.resize(numOfProcesses);
    vector<int> safeSeq;
    vector<int> safeCompletionTime;
    vector<Process> safeProcesses;
    safeCompletionTime.resize(numOfProcesses);
    cout << "PID\tArrival Time\tBurst " << endl;
    for (int i = 0; i < numOfProcesses; ++i){
        cout << i << "\t";
        processes[i].pid = i;
        cin >> processes[i].arrivalTime >> processes[i].burstTime;
    }
    safeProcesses = processes;
    int time = 0;
    int running_time = 0;
    int smallest_A_T = INT_MAX;
    int save_process_id = 0;
    smallest_A_T = processes[0].arrivalTime;
    for (int i = 1; i < numOfProcesses; i++){
        if (processes[i].arrivalTime < smallest_A_T){
            smallest_A_T = processes[i].arrivalTime;
            save_process_id = i;
        }
    }
    int greatest_b_t = processes[save_process_id].burstTime;
    for (int i = 0; i < numOfProcesses; i++){
        if (processes[i].burstTime > greatest_b_t && processes[i].arrivalTime <= smallest_A_T){
            greatest_b_t = processes[i].burstTime;
            save_process_id = i;
        }
    }
    running_time += greatest_b_t;
    safeSeq.push_back(save_process_id);
    safeCompletionTime[save_process_id] = running_time;
    processes.erase(processes.begin() + save_process_id);
    int del_ind = 0;
    for (int j = 0; j < processes.size();){
        greatest_b_t = processes[j].burstTime;
        save_process_id = processes[j].pid;
        del_ind = j;
        for (int k = 0; k < processes.size(); k++){
            if (processes[k].burstTime > greatest_b_t && processes[k].arrivalTime <= running_time){
                del_ind = k;
                greatest_b_t = processes[k].burstTime;
                save_process_id = processes[k].pid;
            }
        }
        running_time += greatest_b_t;
        safeSeq.push_back(save_process_id);
        safeCompletionTime[save_process_id] = running_time;
        processes.erase(processes.begin() + del_ind);
    }
    processes = safeProcesses;
    cout << endl;
    cout << "Safe Sequence: | ";
    for (int i = 0; i < safeSeq.size(); ++i)
        cout << "P" << safeSeq[i] << " | ";

    cout << endl;
    cout << "PID\t\tTurnAround Time\t\tWaitingTime" << endl;
    for (int i = 0; i < numOfProcesses; ++i){
        int instance1, instance2;
        instance1 = safeCompletionTime[i] - processes[i].arrivalTime;
        instance2 = instance1 - processes[i].burstTime;
        cout << i << "\t\t" << instance1 << "\t\t\t" << instance2 << endl;
    }
}
void controlLFRNS(){
    int numOfProcesses;
    cout << "Enter the number of processes: ";
    cin >> numOfProcesses;
    vector<Process> processes;
    processes.resize(numOfProcesses);
    vector<int> safeSeq;
    vector<int> safeCompletionTime;
    vector<Process> safeProcesses;
    safeCompletionTime.resize(numOfProcesses);
    safeProcesses.resize(numOfProcesses);
    cout << "PID\tArrival Time\tBurst " << endl;
    for (int i = 0; i < numOfProcesses; ++i){
        cout << i << "\t";
        processes[i].pid = i;
        cin >> processes[i].arrivalTime >> processes[i].burstTime;
    }
    safeProcesses = processes;
    int time = 0;
    int running_time = 0;
    int smallest_A_T = INT_MAX;
    int save_process_id = 0;
    vector<float> ratios;
    ratios.resize(numOfProcesses);
    smallest_A_T = processes[0].arrivalTime;
    for (int i = 1; i < numOfProcesses; i++){
        if (processes[i].arrivalTime < smallest_A_T){
            smallest_A_T = processes[i].arrivalTime;
            save_process_id = i;
        }
    }
    float f_r = 0;
    f_r = (((float)running_time - (float)processes[save_process_id].arrivalTime) + (float)processes[save_process_id].burstTime) / (float)processes[save_process_id].burstTime;
    ratios[save_process_id] = f_r;
    running_time += processes[save_process_id].burstTime;
    safeCompletionTime[save_process_id] = running_time;
    safeSeq.push_back(save_process_id);
    processes.erase(processes.begin() + save_process_id);
    int del_ind = 0;
    int waiting_time = 0;
    for (int j = 0; j < processes.size();){
        save_process_id = processes[j].pid;
        del_ind = j;
        f_r = (((float)running_time - (float)processes[j].arrivalTime) + (float)processes[j].burstTime) / (float)processes[j].burstTime;
        for (int k = 0; k < processes.size(); k++){
            if ((((running_time - processes[k].arrivalTime) + processes[k].burstTime) / processes[k].burstTime) < f_r && processes[k].arrivalTime <= running_time){
                f_r = ((((float)running_time - (float)processes[k].arrivalTime) + (float)processes[k].burstTime) / (float)processes[k].burstTime);
                del_ind = k;
                save_process_id = processes[k].pid;
            }
        }
        running_time += processes[del_ind].burstTime;
        safeSeq.push_back(save_process_id);
        safeCompletionTime[save_process_id] = running_time;
        ratios[save_process_id] = f_r;
        processes.erase(processes.begin() + del_ind);
    }
    processes = safeProcesses;
    cout << endl;
    cout << "Safe Sequence: | ";
    for (int i = 0; i < safeSeq.size(); ++i)
        cout << "P" << safeSeq[i] << " | ";
    cout << endl;
    cout << endl;
    cout << "PID\t\tTurnAround Time\t\tWaitingTime\t\tFeedBack Ratio" << endl;
    for (int i = 0; i < numOfProcesses; ++i){
        int instance1, instance2;
        instance1 = safeCompletionTime[i] - processes[i].arrivalTime;
        instance2 = instance1 - processes[i].burstTime;
        cout << i << "\t\t" << instance1 << "\t\t\t" << instance2 << "\t\t\t" << ratios[i] << endl;
    }
}
int main(){
    cout<<"::::Multilevel Queue Scheduling::::"<<endl;
    controlMLQS();
    cout<<endl<<"::::Multilevel Feedback Queue Scheduling::::"<<endl;
    controlMLFQ();
    cout<<endl<<"::::Longest Job First::::"<<endl;
    controlLJF();
    cout<<endl<<"::::Lowest Feedback Ratio Next Scheduling::::"<<endl;
    controlLFRNS();
    cout<<endl<<"Program Ended!!"<<endl;
}
