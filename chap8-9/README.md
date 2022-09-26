## Chap8 MLFQ

**Q1: Run a few randomly-generated problems with just two jobs and two queues; compute the MLFQ execution trace for each. Make your life easier by limiting the length of each job and turning off I/Os.**

```$python ./mlfq.py -n 2 -j 2 -m 20 -M 0 -c```
Job List:
  Job  0: startTime   0 - runTime  17 - ioFreq   0
  Job  1: startTime   0 - runTime   8 - ioFreq   0
Final statistics:
  Job  0: startTime   0 - response   0 - turnaround  25
  Job  1: startTime   0 - response  10 - turnaround  18

  Avg  1: startTime n/a - response 5.00 - turnaround 21.50

```$python ./mlfq.py -n 2 -j 2 -m 10 -M 0 -c```
Job List:
  Job  0: startTime   0 - runTime   8 - ioFreq   0
  Job  1: startTime   0 - runTime   4 - ioFreq   0
Final statistics:
  Job  0: startTime   0 - response   0 - turnaround   8
  Job  1: startTime   0 - response   8 - turnaround  12

  Avg  1: startTime n/a - response 4.00 - turnaround 10.00

**Q2: How would you run the scheduler to reproduce each of the examples in the chapter?**

8.2 Long-running Job Over Time
```$python ./mlfq.py -n 3 -q 10 -l 0,200,0 -c```

8.3 Along Came An Interactive Job
```$python ./mlfq.py -n 3 -q 10 -l 0,180,0:100,20,0 -c```

8.4 A Mixed I/O-intensive and CPU-intensive Workload
```$python ./mlfq.py -n 3 -q 10 -l 0,175,0:50,25,1 -i 5 -S -c```

8.5 without priority boost
```$python ./mlfq.py -n 3 -q 10 -l 0,120,0:100,50,1:100,50,1 -i 1 -S -c```

8.5 with priority boost
```$python $ ./mlfq.py -n 3 -q 10 -l 0,120,0:100,50,1:100,50,1 -i 1 -S -B 50 -c```

8.6 without gaming tolerance
```$python ./mlfq.py -n 3 -q 10 -i 1 -S -l 0,200,0:80,100,9 -c```

8.6 with gaming tolerance
```$python ./mlfq.py -n 3 -q 10 -i 1 -l 0,200,0:80,100,9 -c```

8.7 Lower Priority, Longer Quanta
```$python ./mlfq.py -n 3 -a 2 -Q 10,20,40 -l 0,200,0:0,200,0 -c```

**Q3: How would you configure the scheduler parameters to behave just like a round-robin scheduler?**

Make length of time slice (```-q```) smaller or equal to max run time of job (```-m```) divided by number of jobs (```-j```). 

**Q4: Craft a workload with two jobs and scheduler parameters so that one job takes advantage of the older Rules 4a and 4b (turned on with the -S flag) to game the scheduler and obtain 99% of the CPU over a particular time interval.**

```$python ./mlfq.py -n 3 -q 100 -l 0,500,0:100,200,99 -i 1 -S -c```

**Q5: Given a system with a quantum length of 10ms in its highest queue, how often would you have to boost jobs back to the highest priority level (with the -B flag) in order to guarantee that a single long-running (and potentially-starving) job gets at least 5% of the CPU?**

200ms. Everytime it gets boosted back it can run for 10 ms, so 10 /(5%) = 200ms.

**Q6: One question that arises in scheduling is which end of a queue to add a job that just finished I/O; the -I flag changes this behavior for this scheduling simulator. Play around with some workloads and see if you can see the effect of this flag.**

```$ python ./mlfq.py -n 2 -q 10 -l 0,50,0:0,50,12 -i 1 -S -c```
```$ python ./mlfq.py -n 2 -q 10 -l 0,50,0:0,50,12 -i 1 -S -I -c```

with -I, turn around time for the other job will be deplayed. 



## Chap9 

**Q1: Compute the solutions for simulations with 3 jobs and random seeds of 1, 2, and 3.**

```
$python ./lottery.py -s 1 -j 3 -c
$python ./lottery.py -s 2 -j 3 -c
$python ./lottery.py -s 3 -j 3 -c
```

result (in order):

  Job 0 ( length = 1, tickets = 84 )
  Job 1 ( length = 7, tickets = 25 )
  Job 2 ( length = 4, tickets = 44 )

  Job 0 ( length = 9, tickets = 94 )
  Job 1 ( length = 8, tickets = 73 )
  Job 2 ( length = 6, tickets = 30 )

  Job 0 ( length = 2, tickets = 54 )
  Job 1 ( length = 3, tickets = 60 )
  Job 2 ( length = 6, tickets = 6 )


**Q2: Now run with two specific jobs: each of length 10, but one (job 0) with just 1 ticket and the other (job 1) with 100 (e.g., ```-l 10:1,10:100```). What happens when the number of tickets is so imbalanced? Will job 0 ever run before job 1 completes? How often? In general, what does such a ticket imbalance do to the behavior of lottery scheduling?**

result:
--> JOB 1 DONE at time 10
--> JOB 0 DONE at time 20
fairness metric R = 0.5

This is not fair for job 0, which only has 1 ticket compared with 100 tickets of job 1. Job 1 is much more likely to be selected to run - 100/101 compared with 1/101.

**Q3: When running with two jobs of length 100 and equal ticket allocations of 100 (```-l 100:100,100:100```), how unfair is the scheduler? Run with some different random seeds to determine the (probabilistic) answer; let unfairness be determined by how much earlier one job finishes than the other.**

define UR = |(time_job_a_done - time_job_b_done)/time_job_b_done| (job a finishes earlier) 

```$python ./lottery.py -s 0 -l 100:100,100:100 -c```
 JOB 0 DONE at time 192
 JOB 1 DONE at time 200
 UR = 0.04

```$python ./lottery.py -s 1 -l 100:100,100:100 -c```
--> JOB 1 DONE at time 196
--> JOB 0 DONE at time 200
UR = 0.02

```$python ./lottery.py -s 2 -l 100:100,100:100 -c```
--> JOB 1 DONE at time 190
--> JOB 0 DONE at time 200
UR = 0.05

```$python ./lottery.py -s 3 -l 100:100,100:100 -c```
--> JOB 0 DONE at time 196
--> JOB 1 DONE at time 200
UR = 0.02

It's close to complete fairness.

**Q4: How does your answer to the previous question change as the quantum size (-q) gets larger?**

As the quantum size gets larger, fairness tends to get lower (more unfair) - but it is not linear. 

**Q5: Can you make a version of the graph that is found in the chapter? What else would be worth exploring? How would the graph look with a stride scheduler?**

See chap9-1.png.

Generally, speaking, for a certain quantum size (smaller than job length), the larger the job length, the larger the average fariness; for a certain job length (larger than quantum size), the smaller the quantum size, the larger the average fariness.


For stride scheduler, see chap9-2.png.



