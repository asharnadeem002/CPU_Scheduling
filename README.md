# CPU_Scheduling
A CPU Scheduler implemented through operating systems basics in c++


Implement the following CPU Scheduling algorithms and choose parameters (i.e waiting time burst time etc) for CPU Scheduling accordingly.
● Multilevel Queue Scheduling
You have to implement 3 queues; each queue will implement a different scheduling algorithm.
First queue will implement priority scheduling, 2nd will implement RR and third queue will be implemented using SRJF(shortest remaining job first).
● Multilevel Feedback Queue Scheduling
A process can move between the various queues; aging can be
   
implemented this way
Multilevel-feedback-queue scheduler defined by the following parameters:
number of queues(3)
scheduling algorithms for each queue
method used to determine when to upgrade a process method used to determine when to demote a process method used to determine which queue a process will enter when that process needs service
Similarly for MLFQ you will again implement 3 queues, each queue will implement a different scheduling algorithm.
First queue will implement FCFS, 2nd will implement SJF and third queue will be implemented using RR (shortest remaining job first).
● Longest Job First
● Lowest Feedback Ratio Next Scheduling
Feedback Ratio is calculated by the given formula:
algorithms the
Lowest Feedback Ratio Next is one of the scheduling
a non-preemptive algorithm in which the scheduling is done on
extra parameter called Feedback Ratio. A Feedback Ratio is calculated for each
of the available jobs and the Job with the lowest feedback ratio
. This is
basis of an
is given priority
over the others.
1. Feedback Ratio = (W+S)/S
Where: W is Waiting Time, S is Service Time or Burst Time
