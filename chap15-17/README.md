## Chap 15

**Q1: Run with seeds 1, 2, and 3, and compute whether each virtual address generated by the process is in or out of bounds. If in bounds, compute the translation.**

```$ python relocation.py -s 1 -c```
Base-and-Bounds register information:

  Base   : 0x0000363c (decimal 13884)
  Limit  : 290

Virtual Address Trace
  VA  0: 0x0000030e (decimal:  782) --> SEGMENTATION VIOLATION
  VA  1: 0x00000105 (decimal:  261) --> VALID: 0x00003741 (decimal: 14145)
  VA  2: 0x000001fb (decimal:  507) --> SEGMENTATION VIOLATION
  VA  3: 0x000001cc (decimal:  460) --> SEGMENTATION VIOLATION
  VA  4: 0x0000029b (decimal:  667) --> SEGMENTATION VIOLATION

```$ python relocation.py -s 2 -c```
Base-and-Bounds register information:

  Base   : 0x00003ca9 (decimal 15529)
  Limit  : 500

Virtual Address Trace
  VA  0: 0x00000039 (decimal:   57) --> VALID: 0x00003ce2 (decimal: 15586)
  VA  1: 0x00000056 (decimal:   86) --> VALID: 0x00003cff (decimal: 15615)
  VA  2: 0x00000357 (decimal:  855) --> SEGMENTATION VIOLATION
  VA  3: 0x000002f1 (decimal:  753) --> SEGMENTATION VIOLATION
  VA  4: 0x000002ad (decimal:  685) --> SEGMENTATION VIOLATION

```$ python relocation.py -s 3 -c```
Base-and-Bounds register information:

  Base   : 0x000022d4 (decimal 8916)
  Limit  : 316

Virtual Address Trace
  VA  0: 0x0000017a (decimal:  378) --> SEGMENTATION VIOLATION
  VA  1: 0x0000026a (decimal:  618) --> SEGMENTATION VIOLATION
  VA  2: 0x00000280 (decimal:  640) --> SEGMENTATION VIOLATION
  VA  3: 0x00000043 (decimal:   67) --> VALID: 0x00002317 (decimal: 8983)
  VA  4: 0x0000000d (decimal:   13) --> VALID: 0x000022e1 (decimal: 8929)


**Q2: Run with these flags: ```-s 0 -n 10```. What value do you have set -l (the bounds register) to in order to ensure that all the generated virtual addresses are within bounds?**

1k

reason:   
vaddr = int(asize * random.random())
asize for address space size

**Q3: Run with these flags: ```-s 1 -n 10 -l 100```. What is the maximum value that base can be set to, such that the address space still fits into physical memory in its entirety?**

16 * 1024 - 100 = 16284

**Q4: Run some of the same problems above, but with larger address spaces (```-a```) and physical memories (```-p```).**

**Q5: What fraction of randomly-generated virtual addresses are valid, as a function of the value of the bounds register? Make a graph from running with different random seeds, with limit values ranging from 0 up to the maximum size of the address space.**



## Chap 16

**Q1: First let’s use a tiny address space to translate some addresses. Here’s a simple set of parameters with a few different random seeds; can you translate the addresses?**

```segmentation.py -a 128 -p 512 -b 0 -l 20 -B 512 -L 20 -s 0```
```segmentation.py -a 128 -p 512 -b 0 -l 20 -B 512 -L 20 -s 1```
```segmentation.py -a 128 -p 512 -b 0 -l 20 -B 512 -L 20 -s 2```

ARG seed 0
ARG address space size 128
ARG phys mem size 512

Segment register information:

  Segment 0 base  (grows positive) : 0x00000000 (decimal 0)
  Segment 0 limit                  : 20

  Segment 1 base  (grows negative) : 0x00000200 (decimal 512)
  Segment 1 limit                  : 20

Virtual Address Trace
  VA  0: 0x0000006c (decimal:  108) --> VALID in SEG1: 0x000001ec (decimal:  492)
  VA  1: 0x00000061 (decimal:   97) --> SEGMENTATION VIOLATION (SEG1)
  VA  2: 0x00000035 (decimal:   53) --> SEGMENTATION VIOLATION (SEG0)
  VA  3: 0x00000021 (decimal:   33) --> SEGMENTATION VIOLATION (SEG0)
  VA  4: 0x00000041 (decimal:   65) --> SEGMENTATION VIOLATION (SEG1)

**Q2: Now, let’s see if we understand this tiny address space we’ve constructed (using the parameters from the question above). What is the highest legal virtual address in segment 0? What about the lowest legal virtual address in segment 1? What are the lowest and highest illegal addresses in this entire address space? Finally, how would you run ```segmentation.py``` with the ```-A``` flag to test if you are right?**

seg 0 valid virtual address 0~19
seg 1 valid virtual address 108 ~ 127

lowest illegal address 20
highest illegal address 107

```python segmentation.py -a 128 -p 512 -b 0 -l 20 -B 512 -L 20 -s 1 -A 19,108,20,107 -c```

**Q3: Let’s say we have a tiny 16-byte address space in a 128-byte physical memory. What base and bounds would you set up so as to get the simulator to generate the following translation results for the specified address stream: valid, valid, violation, ..., violation, valid, valid? Assume the following parameters:**

```python segmentation.py -a 16 -p 128 --b0 0 --l0 2 --b1 127 --l1 2 -A 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15 -c```

limit is 2, one from top and the other from bottom

**Q4: Assume we want to generate a problem where roughly 90% of the randomly-generated virtual addresses are valid (not segmentation violations). How should you configure the simulator to do so? Which parameters are important to getting this outcome?**

--b0 0 --l0 7 --b1 15 --l1 7

**Q5: Can you run the simulator such that no virtual addresses are valid? How?**

--b0 0 --l0 0 --b1 15 --l1 0



## Chap 17

**Q1: First run with the flags ```-n 10 -H 0 -p BEST -s 0``` to generate a few random allocations and frees. Can you predict what alloc()/free() will return? Can you guess the state of the free list after each request? What do you notice about the free list over time?**

The memory region was chopped up into pieces. The allcator needed to search more elements over time.

**Q2: How are the results different when using a WORST fit policy to search the freelist(```-p WORST```)? What changes?**

The memory region was divided into more pieces. The allocator searched more elements.

**Q3: What about when using FIRST fit (```-p FIRST```)? What speeds up when you use first fit?**

The allocator searched less elements.

**Q4: For the above questions, how the list is kept ordered can affect the time it takes to find a free location for some of the policies. Use the different free list orderings(```-l ADDRSORT,-l SIZESORT+,-l SIZESORT-```) to see how the policies and the list orderings interact.**

WORST and BEST still search every element.

**Q5: Coalescing of a free list can be quite important. Increase the number of random allocations (say to ```-n 1000```). What happens to larger allocation requests over time? Run with and without coalescing (i.e., without and with the ```-C``` flag). What differences in outcome do you see? How big is the free list over time in each case? Does the ordering of the list matter in this case?**

```python malloc.py -n 1000 -H 0 -p FIRST -s 0 -c```
```python malloc.py -n 1000 -H 0 -p FIRST -s 0 -C -c```

fail after 543 due to external fragmentation

ptr[543] = Alloc(8) returned -1 (searched 49 elements)

with ```-C``` this would work. 

Sort by address is more efficient for coalescing. 

without ```-C``` free list will be long. 

**Q6: What happens when you change the percent allocated fraction ```-P``` to higher than 50? What happens to allocations as it nears 100? What about as the percent nears 0?**

```python malloc.py -n 10 -H 0 -s 0 -P 100 -c```

More than 50 meaning more allocs and less frees so the allocator can run out of free space to allocate. 
Free list size does not grow.

```python malloc.py -n 10 -H 0 -s 0 -P 1 -c```

More frees than allocs, so the allocator will always have free space to allocate, and free list size grows.



**Q7: What kind of specific requests can you make to generate a highly-fragmented free space? Use the ```-A``` flag to create fragmented free lists, and see how different policies and options change the organization of the free list.**

first do allocations and then do frees.
```python malloc.py -c -A +20,+20,+20,+20,+20,-0,-1,-2,-3,-4```
