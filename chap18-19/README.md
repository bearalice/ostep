## chap 18

**Q1: Before doing any translations, let’s use the simulator to study how linear page tables change size given different parameters. Compute the size of linear page tables as different parameters change. Some suggested inputs are below; by using the ```-v``` flag,you can see how many page-table entries are filled. First, to understand how linear page table size changes as the address space grows:**

```./paging-linear-translate.py -P 1k -a 1m -p 512m -v -n 0```
```./paging-linear-translate.py -P 1k -a 2m -p 512m -v -n 0```
```./paging-linear-translate.py -P 1k -a 4m -p 512m -v -n 0```

**Then, to understand how linear page table size changes as page size grows:**

```./paging-linear-translate.py -P 1k -a 1m -p 512m -v -n 0``` 
```./paging-linear-translate.py -P 2k -a 1m -p 512m -v -n 0``` (255)
```./paging-linear-translate.py -P 4k -a 1m -p 512m -v -n 0``` (511)

**Before running any of these, try to think about the expected trends. How should page-table size change as the address space grows? As the page size grows? Why not use big pages in general?**

Ans:
page table size = address space / page size

page table size grows as address space grows, and shrinks as page size increases

Big pages are not efficient. 

**Q2: Now let’s do some translations. Start with some small examples, and change the number of pages that are allocated to the address space with the ```-u``` flag. For example:**

```./paging-linear-translate.py -P 1k -a 16k -p 32k -v -u 0```
```./paging-linear-translate.py -P 1k -a 16k -p 32k -v -u 25```
```./paging-linear-translate.py -P 1k -a 16k -p 32k -v -u 50```
```./paging-linear-translate.py -P 1k -a 16k -p 32k -v -u 75```
```./paging-linear-translate.py -P 1k -a 16k -p 32k -v -u 100```

**What happens as you increase the percentage of pages that are allocated in each address space?**

Ans:

As we increase percent of virtual address space that is used, more pages are valid.

**Q3: Now let’s try some different random seeds, and some different (and sometimes quite crazy) address-space parameters, for variety:**

```./paging-linear-translate.py -P 8  -a 32   -p 1024 -v -s 1```
```./paging-linear-translate.py -P 8k -a 32k  -p 1m   -v -s 2```
```./paging-linear-translate.py -P 1m -a 256m -p 512m -v -s 3```

**Which of these parameter combinations are unrealistic? Why?**

Ans:

The first is not realistic as page size is too small; and the third is not because the page size is too large.

**Q4: Use the program to try out some other problems. Can you find the limits of where the program doesn’t work anymore? For example, what happens if the address-space size is bigger than physical memory?**


```python paging-linear-translate.py -P 1k -a 16k -p 15k -v -u 25 -c```

ARG seed 0
ARG address space size 16k
ARG phys mem size 15k
ARG page size 1k
ARG verbose True
ARG addresses -1

Error: physical memory size must be GREATER than address space size (for this simulation)


## chap 19

**Q1: For timing, you’ll need to use a timer (e.g., ```gettimeofday()```). How precise is such a timer? How long does an operation have to take in order for you to time it precisely? (this will help determine how many times, in a loop, you’ll have to repeat a page access in order to time it successfully)**

Ans:
For ```gettimeofday()```, the minimum unit is microsecond. 
CLOCK_PROCESS_CPUTIME_ID has resolution of 1 nanosecond on Linux, and 1000 nanoseconds on macOS.

**Q2: Write the program, called ```tlb.c```, that can roughly measure the cost of accessing each page. Inputs to the program should be: the number of pages to touch and the number of trials.**

**Q3: Now write a script in your favorite scripting language (```bash```?) to run this program, while varying the number of pages accessed from 1 up to a few thousand, perhaps incrementing by a factor of two per iteration. Run the script on different machines and gather some data. How many trials are needed to get reliable measurements?**

Ans: 
100,000 trials.

**Q4: Next, graph the results, making a graph that looks similar to the one above. Use a good tool like ```ploticus``` or even ```zplot```. Visualization usually makes the data much easier to digest; why do you think that is?**

We can see the trend. 

**Q5: One thing to watch out for is compiler optimization. Compilers do all sorts of clever things, including removing loops which increment values that no other part of the program subsequently uses. How can you ensure the compiler does not remove the main loop above from your TLB size estimator?**

Ans:
To disable optimization, use ```gcc -O0```.

**Q6: Another thing to watch out for is the fact that most systems today ship with multiple CPUs, and each CPU, of course, has its own TLB hierarchy. To really get good measurements, you have to run your code on just one CPU, instead of letting the scheduler bounce it from one CPU to the next. How can you do that? (hint: look up “pinning a thread” on Google for some clues) What will happen if you don’t do this, and the code moves from one CPU to the other?**

Ans:
Use sched_setaffinity(2). 

**Q7: Another issue that might arise relates to initialization. If you don’t initialize the array ```a``` above before accessing it, the first time you access it will be very expensive, due to initial access costs such as demand zeroing. Will this affect your code and its timing? What can you do to counterbalance these potential costs?**

Ans:
Use ```calloc()```, which allocates the memory and also initializes every byte in the allocated memory to 0.