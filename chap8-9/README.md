## MLFQ

**Q1: Run a few randomly-generated problems with just two jobs and two queues; compute the MLFQ execution trace for each. Make your life easier by limiting the length of each job and turning off I/Os.**

$python ./mlfq.py -n 2 -j 2 -m 20 -M 0 -c
Job List:
  Job  0: startTime   0 - runTime  17 - ioFreq   0
  Job  1: startTime   0 - runTime   8 - ioFreq   0
Final statistics:
  Job  0: startTime   0 - response   0 - turnaround  25
  Job  1: startTime   0 - response  10 - turnaround  18

  Avg  1: startTime n/a - response 5.00 - turnaround 21.50

$python ./mlfq.py -n 2 -j 2 -m 10 -M 0 -c
Job List:
  Job  0: startTime   0 - runTime   8 - ioFreq   0
  Job  1: startTime   0 - runTime   4 - ioFreq   0
Final statistics:
  Job  0: startTime   0 - response   0 - turnaround   8
  Job  1: startTime   0 - response   8 - turnaround  12

  Avg  1: startTime n/a - response 4.00 - turnaround 10.00

**Q2: How would you run the scheduler to reproduce each of the examples in the chapter?**

8.2 Long-running Job Over Time
$python ./mlfq.py -n 3 -q 10 -l 0,200,0 -c

8.3 Along Came An Interactive Job
$python ./mlfq.py -n 3 -q 10 -l 0,180,0:100,20,0 -c

8.4 A Mixed I/O-intensive and CPU-intensive Workload
$python ./mlfq.py -n 3 -q 10 -l 0,175,0:50,25,1 -i 5 -S -c

8.5 without priority boost
$python ./mlfq.py -n 3 -q 10 -l 0,120,0:100,50,1:100,50,1 -i 1 -S -c

8.5 with priority boost
$python $ ./mlfq.py -n 3 -q 10 -l 0,120,0:100,50,1:100,50,1 -i 1 -S -B 50 -c

8.6 without gaming tolerance
$python ./mlfq.py -n 3 -q 10 -i 1 -S -l 0,200,0:80,100,9 -c

8.6 with gaming tolerance
$python ./mlfq.py -n 3 -q 10 -i 1 -l 0,200,0:80,100,9 -c

8.7 Lower Priority, Longer Quanta
$python ./mlfq.py -n 3 -a 2 -Q 10,20,40 -l 0,200,0:0,200,0 -c

**Q3: How would you configure the scheduler parameters to behave just like a round-robin scheduler?**

Make length of time slice (```-q```) smaller or equal to max run time of job (```-m```) divided by number of jobs (```-j```). 

**Q4: Craft a workload with two jobs and scheduler parameters so that one job takes advantage of the older Rules 4a and 4b (turned on with the -S flag) to game the scheduler and obtain 99% of the CPU over a particular time interval.**

$python ./mlfq.py -n 3 -q 100 -l 0,500,0:100,200,99 -i 1 -S -c

**Q5: Given a system with a quantum length of 10ms in its highest queue, how often would you have to boost jobs back to the highest priority level (with the -B flag) in order to guarantee that a single long-running (and potentially-starving) job gets at least 5% of the CPU?**

200ms. Everytime it gets boosted back it can run for 10 ms, so 10 /(5%) = 200ms.

**Q6: One question that arises in scheduling is which end of a queue to add a job that just finished I/O; the -I flag changes this behavior for this scheduling simulator. Play around with some workloads and see if you can see the effect of this flag.**

$ python ./mlfq.py -n 2 -q 10 -l 0,50,0:0,50,12 -i 1 -S -c
$ python ./mlfq.py -n 2 -q 10 -l 0,50,0:0,50,12 -i 1 -S -I -c

with -I, turn around time for the other job will be deplayed. 






