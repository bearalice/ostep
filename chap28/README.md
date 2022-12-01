## Chap 28 Locks

Q1: Examine `flag.s`. This code “implements” locking with a single memory flag. Can you understand the assembly?

yes

**Q2: When you run with the defaults, does `flag.s` work? Use the `-M` and `-R` flags to trace variables and registers (and turn on `-c` to see their values). Can you predict what value will end up in flag?**

Ans:

`python3 x86.py -p flag.s -M flag,count -R ax,bx -c`

0

**Q3: Change the value of the register `%bx` with the `-a` flag(e.g., `-a bx=2,bx=2` if you are running just two threads). What does the code do? How does it change your answer for the question above?**

%bx represents the number of loops. Each thread runs two loops. 

flag = 0

**Q4: Set `bx` to a high value for each thread, and then use the `-i` flag to generate different interrupt frequencies; what values lead to a bad outcomes? Which lead to good outcomes?**

bad outcomes
`python3 x86.py -p flag.s -t 2 -M flag,count -R ax,bx -a bx=3 -c -i 4`

good outcomes
`python 3 x86.py -p flag.s -t 2 -M flag,count -R ax,bx -a bx=3 -c -i 8`

**Q5: Now let’s look at the program `test-and-set.s`. First, try to understand the code, which uses the xchg instruction to build a simple locking primitive. How is the lock acquire written? How about lock release?**

lock: 
xchg %ax, mutex (whether mutex is 0(free) or 1 it is resonbale to set mutex as 1(in %ax))

unlock: mov $0, mutex

**Q6: Now run the code, changing the value of the interrupt interval (`-i`) again, and making sure to loop for a number of times. Does the code always work as expected? Does it sometimes lead to an inefficient use of the CPU? How could you quantify that?**

It works. 

**Q7: Use the `-P` flag to generate specific tests of the locking code. For example, run a schedule that grabs the lock in the first thread, but then tries to acquire it in the second. Does the right thing happen? What else should you test?**

`python3 x86.py -p test-and-set.s -M mutex,count -R ax,bx -c -a bx=10,bx=10 -P 0011`

Yes.
We should also test fairness and performance.

**Q8: Now let’s look at the code in `peterson.s`, which implements Peterson’s algorithm (mentioned in a sidebar in the text). Study the code and see if you can make sense of it.**

**Q9: Now run the code with different values of `-i`. What kinds of different behavior do you see? Make sure to set the thread IDs appropriately (using -a bx=0,bx=1 for example) as the code assumes it.**

`python3 x86.py -p peterson.s -M count,flag,turn -R ax,cx -a bx=0,bx=1 -c -i 2`

Q10: Can you control the scheduling (with the `-P` flag) to “prove” that the code works? What are the different cases you should show hold? Think about mutual exclusion and deadlock avoidance.

`python3 x86.py -p peterson.s -M count,flag,turn -R ax,cx -a bx=0,bx=1 -c -P 0000011111`

Q11: Now study the code for the ticket lock in `ticket.s`. Does it match the code in the chapter? Then run with the following flags: `-a bx=1000,bx=1000` (causing each thread to loop through the critical section 1000 times). Watch what happens; do the threads spend much time spin-waiting for the lock?

Ans:

`python3 x86.py -p ticket.s -M count,ticket,turn -R ax,bx,cx -a bx=1000,bx=1000 -c`

Yes.

Q12: How does the code behave as you add more threads?

Q13: Now examine `yield.s`, in which a yield instruction enables one thread to yield control of the CPU (realistically, this would be an OS primitive, but for the simplicity, we assume an instruction does the task). Find a scenario where `test-and-set.s` wastes cycles spinning, but `yield.s` does not. How many instructions are saved? In what scenarios do these savings arise?

Ans:

`python3 x86.py -p test-and-set.s -M count,mutex -R ax,bx -a bx=5,bx=5 -c -i 7`
`python3 x86.py -p yield.s -M count,mutex -R ax,bx -a bx=5,bx=5 -c -i 7`

Saved one instruction each cycle. When the thread doesn't have the mutex.

Q14: Finally, examine test-and-test-and-set.s. What does this lock do? What kind of savings does it introduce as compared to test-and-set.s?

Ans: set mutex as 1 only when mutex is not held.
----
## questions to ask

A question for the reader: What would happen if the release of the guard lock came after the park(), and not before?
