## Chap 10

**Q1: To start things off, let’s learn how to use the simulator to study how to build an effective multi-processor scheduler. The first simulation will run just one job, which has a run-time of 30, and a working-set size of 200. Run this job (called job ’a’ here) on one simulated CPU as follows: ```./multi.py -n 1 -L a:30:200```. How long will it take to complete? Turn on the ```-c``` flag to see a final answer, and the ```-t``` flag to see a tick-by-tick trace of the job and how it is scheduled.**

Finished time 30.

**Q2: Now increase the cache size so as to make the job’s working set (size=200) fit into the cache (which, by default, is size=100); for example, run ```./multi.py -n 1 -L a:30:200 -M 300```. Can you predict how fast the job will run once it fits in cache? (hint: remember the key parameter of the warm rate, which is set by the ```-r``` flag) Check your answer by running with the solve flag (```-c```) enabled.**

Total is 20. 10 for warming up cache, and 10 with faster speed. 

**Q3: One cool thing about multi.py is that you can see more detail about what is going on with different tracing flags. Run the same simulation as above, but this time with time left tracing enabled (```-T```). This flag shows both the job that was scheduled on a CPU at each time step, as well as how much run-time that job has left after each tick has run. What do you notice about how that second column decreases?**

After warming up for 10 time units, the process ran with its speed doubeld. (first it decreases 1 per row, then it decreases 2 per row.)

**Q4: Now add one more bit of tracing, to show the status of each CPU cache for each job, with the ```-C``` flag. For each job, each cache will either show a blank space (if the cache is cold for that job) or a ’w’ (if the cache is warm for that job). At what point does the cache become warm for job ’a’ in this simple example? What happens as you change the warmup time parameter (```-w```) to lower or higher values than the default?**

at wth time unit.

default warm up time: 
```$ python ./multi.py -n 1 -L a:30:200 -M 300 -T -C -c```
finish time: 20

lower warm up time:
```$ python ./multi.py -n 1 -L a:30:200 -M 300 -T -C -w 5 -c```
finish time: 18

higher warm up time:
```$ python ./multi.py -n 1 -L a:30:200 -M 300 -T -C -w 15 -c```
finish time: 23

finish time is longer with higher warm up time. 

**Q5: At this point, you should have a good idea of how the simulator works for a single job running on a single CPU. But hey, isn’t this a multi-processor CPU scheduling chapter? Oh yeah! So let’s start working with multiple jobs. Specifically, let’s run the following three jobs on a two-CPU system (i.e., type ```./multi.py -n 2 -L a:100:100,b:100:50,c:100:50```) Can you predict how long this will take, given a round-robin centralized scheduler? Use -c to see if you were right, and then dive down into details with ```-t``` to see a step-by-step and then ```-C``` to see whether caches got warmed effectively for these jobs. What do you notice?**

Cache was cleared every 10 time units, processes never ran faster, 100 + 100 + 100/2 = 150.

**Q6: Now we’ll apply some explicit controls to study cache affinity, as described in the chapter. To do this, you’ll need the ```-A``` flag. This flag can be used to limit which CPUs the scheduler can place a particular job upon. In this case, let’s use it to place jobs ’b’ and ’c’ on CPU 1, while restricting ’a’ to CPU 0. This magic is accomplished by typing this ```./multi.py -n 2 -L a:100:100,b:100:50,c:100:50 -A a:0,b:1,c:1```; don’t forget to turn on various tracing options to see what is really happening! Can you predict how fast this version will run? Why does it do better? Will other combinations of ’a’, ’b’, and ’c’ onto the two processors run faster or slower?**

for a, 10 warm up + (100-10)/2 = 55
for b and c, 20 warm up in total + (200-20)/2 = 100

This is the faster combination since a, b, c all stick to their original cpu which means they only need to warm up cache once.

**Q7: One interesting aspect of caching multiprocessors is the opportunity for better-than-expected speed up of jobs when using multiple CPUs (and their caches) as compared to running jobs on a single processor. Specifically, when you run on N CPUs, sometimes you can speed up by more than a factor of N, a situation entitled super-linear speedup. To experiment with this, use the job description here (```-L a:100:100,b:100:100,c:100:100```) with a small cache (```-M 50```) to create three jobs. Run this on systems with 1, 2, and 3 CPUs (```-n 1, -n 2, -n 3```). Now, do the same, but with a larger per-CPU cache of size 100. What do you notice about performance as the number of CPUs scales? Use ```-c``` to confirm your guesses, and other tracing flags to dive even deeper.**

```$ python ./multi.py -n 1 -L a:100:100,b:100:100,c:100:100 -M 50 -c -t -T -C```
finish time: 300, no cache was utilized

```$ python ./multi.py -n 2 -L a:100:100,b:100:100,c:100:100 -M 50 -c -t -T -C```
finish time: 150 (due to 2 cpus), no cache was utilized

```$ python ./multi.py -n 3 -L a:100:100,b:100:100,c:100:100 -M 50 -c -t -T -C```
finish time: 100 (due to 3 cpus), no cache was utilized

larger cache size:

```$ python ./multi.py -n 1 -L a:100:100,b:100:100,c:100:100 -M 100 -c -t -T -C```
finish time: 300, no cache was utilized (warmed but not used as cache was cleared for next process)

```$ python ./multi.py -n 2 -L a:100:100,b:100:100,c:100:100 -M 100 -c -t -T -C```
finish time: 150 (due to 2 cpus), no cache was utilized (warmed but not used as cache was cleared for next process)

```$ python ./multi.py -n 3 -L a:100:100,b:100:100,c:100:100 -M 100 -c -t -T -C```
finish time: 50 (due to 3 cpus), cache was utilized

each process stick to a cpu, so after warm up, they ran one time more faster (speed was doubled)
they ended at the same time: 10 + (100-10)/2 = 55

**Q8: One other aspect of the simulator worth studying is the per-CPU scheduling option, the ```-p``` flag. Run with two CPUs again, and this three job configuration (```-L a:100:100,b:100:50,c:100:50```). How does this option do, as opposed to the hand-controlled affinity limits you put in place above? How does performance change as you alter the ’peek interval’ (```-P```) to lower or higher values? How does this per-CPU approach work as the number of CPUs scales?**

```$ python ./multi.py -n 2 -L a:100:100,b:100:100,c:100:100 -p -c -t -T -C```
finish time 100 
60 (b finished at 55) + 10 (a warm up) + (100 - 30 - 10)/2

```$ python ./multi.py -n 2 -L a:100:100,b:100:100,c:100:100 -p -c -t -T -C -P 10```
finish time 100
same as above 

```$ python ./multi.py -n 2 -L a:100:100,b:100:100,c:100:100 -p -c -t -T -C -P 40``` 
finish time 115
peek happened at 80
80 + 10 + (100-40-10)/2 = 115


As the number of CPUs scales:

```$ python ./multi.py -n 3 -L a:100:100,b:100:100,c:100:100 -p -c -t -T -C```
finish time 55 (no need to peek)
  
```$ python ./multi.py -n 4 -L a:100:100,b:100:100,c:100:100 -p -c -t -T -C```    
finish time 60 as peek changes cache affinity

```$ python ./multi.py -n 5 -L a:100:100,b:100:100,c:100:100 -p -c -t -T -C```
finish time 55 as cache affinity remains (why?)

**Q9: Finally, feel free to just generate random workloads and see if you can predict their performance on different numbers of processors, cache sizes, and scheduling options. If you do this, you’ll soon be a multi-processor scheduling master, which is a pretty awesome thing to be. Good luck!**

Thank you.


## Chap 13

**Q1: The first Linux tool you should check out is the very simple tool ```free```. First, type man free and read its entire manual page; it’s short, don’t worry!**

```free```  displays  the total amount of free and used physical and swap memory in the system, as well as the buffers and caches used by the kernel.

**Q2: Now, run ```free```, perhaps using some of the arguments that might be useful (e.g., -m, to display memory totals in megabytes). How much memory is in your system? How much is free? Do these numbers match your intuition?**

              total        used        free      shared  buff/cache   available
Mem:      394637612    10622336   347055700      376472    36959576   382500720
Swap:       4194300      877596     3316704

**Q3: Next, create a little program that uses a certain amount of memory, called ```memory-user.c```. This program should take one command-line argument: the number of megabytes of memory it will use. When run, it should allocate an array, and constantly stream through the array, touching each entry. The program should do this indefinitely, or, perhaps, for a certain amount of time also specified at the command line.**

see code.

**Q4: Now, while running your memory-user program, also (in a different terminal window, but on the same machine) run the free tool. How do the memory usage totals change when your program is running? How about when you kill the memory-user program? Do the numbers match your expectations? Try this for different amounts of memory usage. What happens when you use really large amounts of memory?**

              total        used        free      shared  buff/cache   available
Mem:      394637612    10648740   347029276      376432    36959596   382474356
Swap:       4194300      877596     3316704

```used``` increased. 
Yes. 

**Q5: Let’s try one more tool, known as ```pmap```. Spend some time, and read the pmap manual page in detail.**

**Q6: To use ```pmap```, you have to know the process ID of the process you’re interested in. Thus, first run```ps auxw``` to see a list of all processes; then, pick an interesting one, such as a browser. You can also use your memory-user program in this case (indeed, you can even have that program call getpid() and print out its PID for your convenience).**

one item from ```ps auxw```:
yuzhesh+ 190798  0.0  0.0 113284  1416 ?        S    Sep24   0:00 sh /home/yuzheshen/.vscode-server/bin/74b1f979648cc44d385a2286793c

results:
0000000000400000    888K r-x-- bash
00000000006dd000      4K r---- bash
00000000006de000     36K rw--- bash
00000000006e7000     24K rw---   [ anon ]
0000000002098000    132K rw---   [ anon ]
00007f147a789000 103692K r---- locale-archive
00007f1480ccc000   1808K r-x-- libc-2.17.so
00007f1480e90000   2044K ----- libc-2.17.so
00007f148108f000     16K r---- libc-2.17.so
00007f1481093000      8K rw--- libc-2.17.so
00007f1481095000     20K rw---   [ anon ]
00007f148109a000      8K r-x-- libdl-2.17.so
00007f148109c000   2048K ----- libdl-2.17.so
00007f148129c000      4K r---- libdl-2.17.so
00007f148129d000      4K rw--- libdl-2.17.so
00007f148129e000    148K r-x-- libtinfo.so.5.9
00007f14812c3000   2048K ----- libtinfo.so.5.9
00007f14814c3000     16K r---- libtinfo.so.5.9
00007f14814c7000      4K rw--- libtinfo.so.5.9
00007f14814c8000    136K r-x-- ld-2.17.so
00007f14816c8000     12K rw---   [ anon ]
00007f14816e1000     28K r--s- gconv-modules.cache
00007f14816e8000      4K rw---   [ anon ]
00007f14816e9000      4K r---- ld-2.17.so
00007f14816ea000      4K rw--- ld-2.17.so
00007f14816eb000      4K rw---   [ anon ]
00007ffde8154000    132K rw---   [ stack ]
00007ffde81a6000      8K r-x--   [ anon ]
ffffffffff600000      4K r-x--   [ anon ]
 total           113288K

**Q7: Now run ```pmap``` on some of these processes, using various flags (like ```-X```) to reveal many details about the process. What do you see? How many different entities make up a modern address space, as opposed to our simple conception of code/stack/heap?**

Address Perm   Offset Device    Inode   Size  Rss Pss Referenced Anonymous Swap Locked ProtectionKey Mapping

**Q8: Finally,let’s run pmap on your memory-user program, with different amounts of used memory. What do you see here? Does the output from pmap match your expectations?**

Used memory will increase accordingly. 


## Chap 14

**Q1: First, write a simple program called ```null.c``` that creates a pointer to an integer, sets it to NULL, and then tries to dereference it. Compile this into an executable called null. What happens when you run this program?**

Segmentation fault

**Q2: Next,compile this program with symbol information included(with the ```-g``` flag). Doing so let’s put more information into the executable, enabling the debugger to access more useful information about variable names and the like. Run the program under the debugger by typing ```gdb null``` and then, once gdb is running, typing ```run```. What does gdb show you?**

gdb) run
Starting program: /home/yuzheshen/practice/OSTEP-chap14/null 

Program received signal SIGSEGV, Segmentation fault.
0x0000000000400600 in main (argc=1, argv=0x7fffffffe2e8) at null.c:7
7           printf("%d\n", *val);
Missing separate debuginfos, use: debuginfo-install glibc-2.17-326.el7_9.x86_64
(gdb) 

**Q3: Finally, use the ```valgrind``` tool on this program. We’ll use the ```memcheck``` tool that is a part of valgrind to analyze what happens. Run this by typing in the following: ```valgrind --leak-check=yes null```. What happens when you run this? Can you interpret the output from the tool?**

Invalid read of size 4
==129207==    at 0x4005E6: main (in /home/yuzheshen/practice/OSTEP-chap14/null)
==129207==  Address 0x0 is not stack'd, malloc'd or (recently) free'd

**Q4: Write a simple program that allocates memory using ```malloc()``` but forgets to free it before exiting. What happens when this program runs? Can you use gdb to find any problems with it? How about valgrind (again with the ```--leak-check=yes``` flag)?**

gdb does not report bugs.

valgrind does.

==130547== HEAP SUMMARY:
==130547==     in use at exit: 4 bytes in 1 blocks
==130547==   total heap usage: 1 allocs, 0 frees, 4 bytes allocated
==130547== 
==130547== LEAK SUMMARY:
==130547==    definitely lost: 4 bytes in 1 blocks
==130547==    indirectly lost: 0 bytes in 0 blocks
==130547==      possibly lost: 0 bytes in 0 blocks
==130547==    still reachable: 0 bytes in 0 blocks
==130547==         suppressed: 0 bytes in 0 blocks
==130547== Rerun with --leak-check=full to see details of leaked memory
==130547== 
==130547== For lists of detected and suppressed errors, rerun with: -s
==130547== ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)

**Q5: Write a program that creates an array of integers called ```data``` of size 100 using ```malloc```; then, set ```data[100]``` to zero. What happens when you run this program? What happens when you run this program using ```valgrind```? Is the program correct?**

==131252== Invalid write of size 4
==131252==    at 0x4005A4: main (in /home/yuzheshen/practice/OSTEP-chap14/q5)
==131252==  Address 0x52051d0 is 224 bytes inside an unallocated block of size 4,194,032 in arena "client"

max valid index of data should be 99 since its size is 100.

**Q6: Create a program that allocates an array of integers (as above), frees them, and then tries to print the value of one of the elements of the array. Does the program run? What happens when you use valgrind on it?**

==139967== Invalid read of size 4
==139967==    at 0x4005EA: main (in /home/yuzheshen/practice/OSTEP-chap14/q6)
==139967==  Address 0x5205040 is 0 bytes inside a block of size 100 free'd
==139967==    at 0x4C2B06D: free (vg_replace_malloc.c:540)
==139967==    by 0x4005E5: main (in /home/yuzheshen/practice/OSTEP-chap14/q6)
==139967==  Block was alloc'd at
==139967==    at 0x4C29F73: malloc (vg_replace_malloc.c:309)
==139967==    by 0x4005D5: main (in /home/yuzheshen/practice/OSTEP-chap14/q6)
==139967== 

**Q7: Now pass a funny value to free (e.g., a pointer in the middle of the array you allocated above). What happens? Do you need tools to find this type of problem?**

 Error in `./q7': free(): invalid pointer: 0x00000000021c70d8 

**Q8: Try out some of the other interfaces to memory allocation. For example, create a simple vector-like data structure and related routines that use realloc() to manage the vector. Use an array to store the vectors elements; when a user adds an entry to the vector, use realloc() to allocate more space for it. How well does such a vector perform? How does it compare to a linked list? Use valgrind to help you find bugs.**

Depending on the operations. 
