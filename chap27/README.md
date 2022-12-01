## chap 27

**Q1: First build `main-race.c`. Examine the code so you can see the (hopefully obvious) data race in the code. Now run `helgrind` (by typing `valgrind --tool=helgrind main-race`) to see how it reports the race. Does it point to the right lines of code? What other information does it give to you?**

Ans:

Yes, it points to the right lines of code. The conflict address and size, as well as Lock information.

----------------------------------------------------------------
==86100== 
==86100== Possible data race during read of size 4 at 0x601048 by thread #1
==86100== Locks held: none
==86100==    at 0x400672: main (main-race.c:15)
==86100== 
==86100== This conflicts with a previous write of size 4 by thread #2
==86100== Locks held: none
==86100==    at 0x40061E: worker (main-race.c:8)
==86100==    by 0x4C30EDE: mythread_wrapper (hg_intercepts.c:389)
==86100==    by 0x4E43EA4: start_thread (in /usr/lib64/libpthread-2.17.so)
==86100==    by 0x5156B0C: clone (in /usr/lib64/libc-2.17.so)
==86100==  Address 0x601048 is 0 bytes inside data symbol "balance"
==86100== 
==86100== ----------------------------------------------------------------
==86100== 
==86100== Possible data race during write of size 4 at 0x601048 by thread #1
==86100== Locks held: none
==86100==    at 0x40067B: main (main-race.c:15)
==86100== 
==86100== This conflicts with a previous write of size 4 by thread #2
==86100== Locks held: none
==86100==    at 0x40061E: worker (main-race.c:8)
==86100==    by 0x4C30EDE: mythread_wrapper (hg_intercepts.c:389)
==86100==    by 0x4E43EA4: start_thread (in /usr/lib64/libpthread-2.17.so)
==86100==    by 0x5156B0C: clone (in /usr/lib64/libc-2.17.so)
==86100==  Address 0x601048 is 0 bytes inside data symbol "balance"
==86100== 
==86100== 
==86100== Use --history-level=approx or =none to gain increased speed, at
==86100== the cost of reduced accuracy of conflicting-access information
==86100== For lists of detected and suppressed errors, rerun with: -s
==86100== ERROR SUMMARY: 2 errors from 2 contexts (suppressed: 0 from 0)

**Q2: What happens when you remove one of the offending lines of code? Now add a lock around one of the updates to the shared variable, and then around both. What does helgrind report in each of these cases?**

Ans:

When you remove one of the offending lines of code, there will be no errors reported by helgrind.

When there is only one lock around one of the updates, there will be errors.

When there are two locks aroud both updates, there will be NO errors.

See `main-race-lock.c`

**Q3: Now let’s look at `main-deadlock.c`. Examine the code. This code has a problem known as deadlock (which we discuss in much more depth in a forthcoming chapter). Can you see what problem it might have?**

Ans:

Two thread lock each other.

Thread #3: lock order "0x601080 (m1) before 0x6010C0" (m2) violated 

**Q4: Now run `helgrind` on this code. What does helgrind report?**

Thread #3: lock order "0x601080 (m1) before 0x6010C0" (m2) violated 

**Q5: Now run `helgrind` on `main-deadlock-global.c`. Examine the code; does it have the same problem that `main-deadlock.c` has? Should `helgrind` be reporting the same error? What does this tell you about tools like `helgrind`?**

Ans:
= Thread #3: lock order "0x6010C0 before 0x601100" violated
helgrind should not report any error, as there is an additional global lock g which would prevent deadlock, but it does. The tool is not perfect. 


**Q6: Let’s next look at `main-signal.c`. This code uses a variable (`done`) to signal that the child is done and that the parent can now continue. Why is this code inefficient? (what does the parent end up spending its time doing, particularly if the child thread takes a long time to complete?)**

Ans:
The parent falls in the loop and do nothing (but uses cpu resource). Line 17. 

**Q7: Now run `helgrind` on this program. What does it report? Is the code correct?**

Ans:
Possible data race during read of size 4 at 0x601048 by thread #1
Address 0x601048 is 0 bytes inside data symbol "done"

No, this is not correct, as `printf` only reads the value. 

**Q8: Now look at a slightly modified version of the code, which is found in `main-signal-cv.c`. This version uses a condition variable to do the signaling (and associated lock). Why is this code preferred to the previous version? Is it correctness, or performance, or both?**

Ans: 

Both. 

**Q9: Once again run `helgrind` on `main-signal-cv`. Does it report any errors?**

Ans: 
No errors.



Overview 

In this homework, you'll use a real tool on Linux to find problems in multi-threaded code. The tool is called helgrind (available as part of the valgrind suite of debugging tools).

See http://valgrind.org/docs/manual/hg-manual.htm for details about the tool, including how to download and install it (if it's not already on your Linux system).

You'll then look at a number of multi-threaded C programs to see how you can use the tool to debug problematic threaded code.

First things first: download and install valgrind and the related helgrind tool.

Then, type make to build all the different programs. Examine the Makefile for more details on how that works.

Then, you have a few different C programs to look at:

main-race.c: A simple race condition
main-deadlock.c: A simple deadlock
main-deadlock-global.c: A solution to the deadlock problem
main-signal.c: A simple child/parent signaling example
main-signal-cv.c: A more efficient signaling via condition variables
common_threads.h: Header file with wrappers to make code check errors and be more readable
With these programs, you can now answer the questions in the textbook.








## Questions to ask

Chapter 27, page 4, Pthread_create(&p, NULL, mythread, &args);
should it be pthread_create(&p, NULL, mythread, &args);

why does helgrind make mistakes in Q5?

the order of locks m1 m2