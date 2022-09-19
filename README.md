## Explanations for HW Chap5-7

### Chap 5

Q1: 

When calling fork(), the child process copies the entire address space contents of the parent process to its own address space, which is also known as copy-on-white. When both the child and parent change the value of x, they are changing their own version of the variable. 

Q2 

Unlike Q1, the parent process and the child process share disk file and file descriptor. So changes from one process will change the same file and the changes will be reflected in the other process as well. 

Q3 

To ensure that the child process always prints first, we can call sleep() in the parent process. 

Q4

The variants of the same basic call, exec(), which are execl(), execle(), execlp(), execv(), execvp(), and execvpe(), allow users to pass different type of arguments to execute an executable file.

Q5

When wait() is used in parent, it will return the pid of child. When used in child, it will return -1.

Q6 

Compared with wait(), waitpid() allows us to specify which process to wait for. Results are the same with the last question.

Q7

If the child closes standard output (STDOUT FILENO), and then calls printf() to print some output, there will be nothing printed out from the child process. 

Q8

We use pipe and close its two end respectively. 


### Chap 6 

Result on Linux:

system call: 0.109273 us
context switch: 0.781123 us

### Chap 7 

#### Q1: Compute the response time and turnaround time when running three jobs of length 200 with the SJF and FIFO schedulers.

$ python ./scheduler.py -p SJF -j 3 -s 100 -l 200,200,200 -c

** Solutions **

Execution trace:
  [ time   0 ] Run job 0 for 200.00 secs ( DONE at 200.00 )
  [ time 200 ] Run job 1 for 200.00 secs ( DONE at 400.00 )
  [ time 400 ] Run job 2 for 200.00 secs ( DONE at 600.00 )

Final statistics:
  Job   0 -- Response: 0.00  Turnaround 200.00  Wait 0.00
  Job   1 -- Response: 200.00  Turnaround 400.00  Wait 200.00
  Job   2 -- Response: 400.00  Turnaround 600.00  Wait 400.00

  Average -- Response: 200.00  Turnaround 400.00  Wait 200.00

$ python ./scheduler.py -p FIFO -l 200,200,200 -c

** Solutions **

Execution trace:
  [ time   0 ] Run job 0 for 200.00 secs ( DONE at 200.00 )
  [ time 200 ] Run job 1 for 200.00 secs ( DONE at 400.00 )
  [ time 400 ] Run job 2 for 200.00 secs ( DONE at 600.00 )

Final statistics:
  Job   0 -- Response: 0.00  Turnaround 200.00  Wait 0.00
  Job   1 -- Response: 200.00  Turnaround 400.00  Wait 200.00
  Job   2 -- Response: 400.00  Turnaround 600.00  Wait 400.00

  Average -- Response: 200.00  Turnaround 400.00  Wait 200.00

#### Q2: Now do the same but with jobs of different lengths: 100, 200, and 300.

$ python ./scheduler.py -p SJF -l 100,200,300 -c

** Solutions **

Execution trace:
  [ time   0 ] Run job 0 for 100.00 secs ( DONE at 100.00 )
  [ time 100 ] Run job 1 for 200.00 secs ( DONE at 300.00 )
  [ time 300 ] Run job 2 for 300.00 secs ( DONE at 600.00 )

Final statistics:
  Job   0 -- Response: 0.00  Turnaround 100.00  Wait 0.00
  Job   1 -- Response: 100.00  Turnaround 300.00  Wait 100.00
  Job   2 -- Response: 300.00  Turnaround 600.00  Wait 300.00

  Average -- Response: 133.33  Turnaround 333.33  Wait 133.33



$ python ./scheduler.py -p FIFO -l 100,200,300 -c

** Solutions **

Execution trace:
  [ time   0 ] Run job 0 for 100.00 secs ( DONE at 100.00 )
  [ time 100 ] Run job 1 for 200.00 secs ( DONE at 300.00 )
  [ time 300 ] Run job 2 for 300.00 secs ( DONE at 600.00 )

Final statistics:
  Job   0 -- Response: 0.00  Turnaround 100.00  Wait 0.00
  Job   1 -- Response: 100.00  Turnaround 300.00  Wait 100.00
  Job   2 -- Response: 300.00  Turnaround 600.00  Wait 300.00

  Average -- Response: 133.33  Turnaround 333.33  Wait 133.33

#### Q3: Now do the same, but also with the RR scheduler and a time-slice of 1.

$ python ./scheduler.py -p RR -q 1 -l 100,200,300 -c

Final statistics:
  Job   0 -- Response: 0.00  Turnaround 298.00  Wait 198.00
  Job   1 -- Response: 1.00  Turnaround 499.00  Wait 299.00
  Job   2 -- Response: 2.00  Turnaround 600.00  Wait 300.00

  Average -- Response: 1.00  Turnaround 465.67  Wait 265.67

#### Q4: For what types of work loads does SJF deliver the same turnaround times as FIFO?

With -l, the list ofn job lengths are already ordered. 

#### Q5: For what types of workloads and quantum lengths does SJF deliver the same response times as RR?

(Note: I am not sure on this one.)

For the first n-1 job lengths (total n jobs), the job lengths are less or equal to quantum length. (otherwise ubnder RR response will be quantum length which can be shorter than job length.)

#### Q6: What happens to response time with SJF as job lengths increase? Can you use the simulator to demonstrate the trend?

Response time will increase as job lengths increase. 

$ python ./scheduler.py -p SJF -l 100,200,300 -c
Average -- Response: 133.33  Turnaround 333.33  Wait 133.33

$ python ./scheduler.py -p SJF -l 110,210,310 -c
Average -- Response: 143.33  Turnaround 353.33  Wait 143.33

#### Q7: What happens to response time with RR as quantum lengths increase? Can you write an equation that gives the worst-case response time, given N jobs?

For RR policy, since response time is decided by quantum length, response time will increase as quantum lengths increase. 

For the nth job, the response time is (n-1)*quantum length. 


