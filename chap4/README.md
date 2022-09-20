## Chap 4 Homework Answers

**Q1: Run process-run.pywiththefollowingflags:-l5:100,5:100. What should the CPU utilization be (e.g., the percent of time the CPU is in use?) Why do you know this? Use the -c and -p flags to see if you were right.**

CPU utilisation is 100%, or 10 of 10 time units. Since both processes issue I/O, there is no wait time for CPU.

**Q2: Now run with these flags: ./process-run.py -l 4:100,1:0. These flags specify one process with 4 instructions (all to use the CPU), and one that simply issues an I/O and waits for it to be done. How long does it take to complete both processes? Use -c and -p to find out if you were right.**

It takes 11 time units: 
4 for process 0 to run on CPU
1 for process 1 to issue I/O
5 for process 1 to do I/O
1 for process 1 to end I/O

**Q3: Switch the order of the processes: -l 1:0,4:100. What happens now? Does switching the order matter? Why? (As always, use -c and -p to see if you were right)**

It takes 7 time units:
1 for process 0 to issue I/O
5 for process 0 to do I/O (at the same time there are 4 for process 1 to run on CPU)
1 for process 0 to end I/O

Reason is when process 1 is BLOCKED for I/O to finish, the CPU can work for process 0. 

**Q4: We’ll now explore some of the other flags. One important flag is -S, which determines how the system reacts when a process issues an I/O. With the flag set to SWITCH ON END, the system will NOT switch to another process while one is doing I/O, instead waiting until the process is completely finished. What happens when you run the following two processes (-l 1:0,4:100 -c -S SWITCH ON END), one doing I/O and the other doing CPU work?**

The same as Q2: 11 time units since the system does not switch to the other process 0 when the first process is on I/O.

**Q5: Now, run the same processes, but with the switching behavior set to switch to another process whenever one is WAITING for I/O (-l 1:0,4:100 -c -S SWITCH ON IO). What happens now? Use -c and -p to confirm that you are right.**

Same with Q3: 7 time units. 

**Q6: One other important behavior is what to do when an I/O completes. With -I IO RUN LATER, when an I/O completes, the process that issued it is not necessarily run right away; rather, whatever was running at the time keeps running. What happens when you run this combination of processes? (Run ./process-run.py -l 3:0,5:100,5:100,5:100 -S SWITCH ON IO -I IO RUN LATER -c -p) Are system resources being effectively utilized?**

Total time is 31 time units:
1 for PID 0 to issue I/O
5 for PID 0 to do I/O (at the same time PID 1 runs)
5 for PID 2 
5 for PID 3
1 for PID 0 to finish I/O
7 for PID 0 to issue/do/finish I/O
7 for PID 0 to issue/do/finish I/O

Stats: CPU Busy 21 (67.74%)

This is not efficient. System resources are not being effectively utilized since for the last two PID 0 I/O, CPU is idle most of the times, which can be used to run PID 2 or PID 3. 

**Q7: Now run the same processes,but with -I IO_RUN_IMMEDIATE set, which immediately runs the process that issued the I/O. How does this behavior differ? Why might running a process that just completed an I/O again be a good idea?**

Total time is 21 time units:
1 for PID 0 to issue I/O
5 for PID 0 to do I/O (at the same time PID 1 runs)
1 for PID 0 to finish I/O
(this repeats three times, hence 21)

Stats: CPU Busy 21 (100.00%)

This is more efficient. Runing a process that just completed I/O is good because that process is more likely to issue another I/O, and the CPU can use that BLOCKED time to work on other processes. 

**Q8: Now run with some randomly generated processes:-s1-l3:50,3:50 or -s 1 -l 3:50,3:50, -s 2 -l 3:50,3:50, -s 3 -l 3:50,3:50. See if you can predict how the trace will turn out. What happens when you use the flag -I IO_RUN_IMMEDIATE vs. -I IO_RUN_LATER? What happens when you use -S SWITCH_ON_IO vs. -S SWITCH_ON_END?**

For the three given cases, there are no differences between IO_RUN_IMMEDIATE and -I IO_RUN_LATER, because with 50% possibility of I/O for 3 instructions, the idle time (by default, I/Os take 5 time units to complete) can handle the task (max 3 time units).

SWITCH_ON_IO is faster than SWITCH_ON_END since each time we switch on I/O, there will be time saved (there will be task being done earlier than SWITCH_ON_END). 








