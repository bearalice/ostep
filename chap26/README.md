## chap 26

Q1: Let’s examine a simple program, “loop.s”. First, just read and understand it. Then, run it with these arguments(`./x86.py -p loop.s -t 1 -i 100 -R dx`) This specifies a single thread, an interrupt every 100 instructions, and tracing of register `%dx`. What will `%dx` be during the run? Use the `-c` flag to check your answers; the answers, on the left, show the value of the register (or memory value) after the instruction on the right has run.

Ans:

   dx          Thread 0         
    0   
   -1   1000 sub  $1,%dx
   -1   1001 test $0,%dx
   -1   1002 jgte .top
   -1   1003 halt

Q2: Same code, different flags: (`./x86.py -p loop.s -t 2 -i 100 -a dx=3,dx=3 -R dx`)This specifies two threads, and initializes each %dx to 3. What values will %dx see? Run with `-c` to check. Does the presence of multiple threads affect your calculations? Is there a race in this code?

dx          Thread 0                Thread 1         
    3   
    2   1000 sub  $1,%dx
    2   1001 test $0,%dx
    2   1002 jgte .top
    1   1000 sub  $1,%dx
    1   1001 test $0,%dx
    1   1002 jgte .top
    0   1000 sub  $1,%dx
    0   1001 test $0,%dx
    0   1002 jgte .top
   -1   1000 sub  $1,%dx
   -1   1001 test $0,%dx
   -1   1002 jgte .top
   -1   1003 halt
    3   ----- Halt;Switch -----  ----- Halt;Switch -----  
    2                            1000 sub  $1,%dx
    2                            1001 test $0,%dx
    2                            1002 jgte .top
    1                            1000 sub  $1,%dx
    1                            1001 test $0,%dx
    1                            1002 jgte .top
    0                            1000 sub  $1,%dx
    0                            1001 test $0,%dx
    0                            1002 jgte .top
   -1                            1000 sub  $1,%dx
   -1                            1001 test $0,%dx
   -1                            1002 jgte .top
   -1                            1003 halt

%dx is 3. The presence of multiple threads does NOT affect the calculations? There is NO race in this code.

Q3: Run this:`./x86.py -p loop.s -t 2 -i 3 -r -a dx=3,dx=3 -R dx` This makes the interrupt interval small/random; use different seeds (-s) to see different interleavings. Does the interrupt frequency change anything?

Ans:

No data race, thread_1 thread_2 decreases their own register %dx. 
Interrupt frequency does not change anything.

Q4: Now, a different program, `looping-race-nolock.s`, which accesses a shared variable located at address 2000; we’ll call this variable value. Run it with a single thread to confirm your understanding: `./x86.py -p looping-race-nolock.s -t 1 -M 2000` What is value(i.e.,at memory address 2000) throughout the run? Use -c to check.

Ans:

`python3 x86.py -p looping-race-nolock.s -t 1 -M 2000 -R ax,bx -c`

 2000      ax    bx          Thread 0         
    0       0     0   
    0       0     0   1000 mov 2000, %ax
    0       1     0   1001 add $1, %ax
    1       1     0   1002 mov %ax, 2000
    1       1    -1   1003 sub  $1, %bx
    1       1    -1   1004 test $0, %bx
    1       1    -1   1005 jgt .top
    1       1    -1   1006 halt

Q5: Run with multiple iterations/threads: `./x86.py -p looping-race-nolock.s -t 2 -a bx=3 -M 2000` Why does each thread loop three times? What is final value of value?

Ans:

Because %bx has been initialized to 3.
Final value is 6.

2000      ax    bx          Thread 0                Thread 1         
    0       0     3   
    0       0     3   1000 mov 2000, %ax
    0       1     3   1001 add $1, %ax
    1       1     3   1002 mov %ax, 2000
    1       1     2   1003 sub  $1, %bx
    1       1     2   1004 test $0, %bx
    1       1     2   1005 jgt .top
    1       1     2   1000 mov 2000, %ax
    1       2     2   1001 add $1, %ax
    2       2     2   1002 mov %ax, 2000
    2       2     1   1003 sub  $1, %bx
    2       2     1   1004 test $0, %bx
    2       2     1   1005 jgt .top
    2       2     1   1000 mov 2000, %ax
    2       3     1   1001 add $1, %ax
    3       3     1   1002 mov %ax, 2000
    3       3     0   1003 sub  $1, %bx
    3       3     0   1004 test $0, %bx
    3       3     0   1005 jgt .top
    3       3     0   1006 halt
    3       0     3   ----- Halt;Switch -----  ----- Halt;Switch -----  
    3       3     3                            1000 mov 2000, %ax
    3       4     3                            1001 add $1, %ax
    4       4     3                            1002 mov %ax, 2000
    4       4     2                            1003 sub  $1, %bx
    4       4     2                            1004 test $0, %bx
    4       4     2                            1005 jgt .top
    4       4     2                            1000 mov 2000, %ax
    4       5     2                            1001 add $1, %ax
    5       5     2                            1002 mov %ax, 2000
    5       5     1                            1003 sub  $1, %bx
    5       5     1                            1004 test $0, %bx
    5       5     1                            1005 jgt .top
    5       5     1                            1000 mov 2000, %ax
    5       6     1                            1001 add $1, %ax
    6       6     1                            1002 mov %ax, 2000
    6       6     0                            1003 sub  $1, %bx
    6       6     0                            1004 test $0, %bx
    6       6     0                            1005 jgt .top
    6       6     0                            1006 halt

**Q6: Run with random interrupt intervals: `./x86.py -p looping-race-nolock.s -t 2 -M 2000 -i 4 -r -s 0` with different seeds (`-s 1`, `-s 2`, etc.) Can you tell by looking at the thread interleaving what the final value of value will be? Does the timing of the interrupt matter? Where can it safely occur? Where not? In other words, where is the critical section exactly?**

Ans:

The timing of the interrupt matters.
`i 3`
when -i < 3 data race occurs. (????)

mov 2000, %ax  # get 'value' at address 2000
add $1, %ax    # increment it
mov %ax, 2000  # store it back

**Q7: Now examine fixed interrupt intervals: `./x86.py -p looping-race-nolock.s -a bx=1 -t 2 -M 2000 -i 1` What will the final value of the shared variable value be? What about when you change `-i 2`, `-i 3`, etc.? For which interrupt intervals does the program give the “correct” answer?**

Ans:

-i 1 value = 1
-i 2 value = 1
-i 3 value = 2

correct:
-i ≥ 3

**Q8: Run the same for more loops (e.g., set `-a bx=100`). What interrupt intervals (-i) lead to a correct outcome? Which intervals are surprising?**

Ans:

-i equals the multiple of 6 (e.g. -i 6) 
(steps combined from one loop of the two threads = 6)

Q9: One last program: `wait-for-me.s`. Run: `./x86.py -p wait-for-me.s -a ax=1,ax=0 -R ax -M 2000` This sets the %ax register to 1 for thread 0, and 0 for thread 1, and watches %ax and memory location 2000. How should the code behave? How is the value at location 2000 being used by the threads? What will its final value be?

Ans:

`python3 x86.py -p wait-for-me.s -a ax=1,ax=0 -R ax -M 2000 -R ax,cx -c`

 2000      ax    cx          Thread 0                Thread 1         
    0       1     0   
    0       1     0   1000 test $1, %ax
    0       1     0   1001 je .signaller
    1       1     0   1006 mov  $1, 2000
    1       1     0   1007 halt
    1       0     0   ----- Halt;Switch -----  ----- Halt;Switch -----  
    1       0     0                            1000 test $1, %ax
    1       0     0                            1001 je .signaller
    1       0     1                            1002 mov  2000, %cx
    1       0     1                            1003 test $1, %cx
    1       0     1                            1004 jne .waiter
    1       0     1                            1005 halt
**(ask about line 164)**

Q10: Now switch the inputs: `./x86.py -p wait-for-me.s -a ax=0,ax=1 -R ax -M 2000` How do the threads behave? What is thread 0 doing? How would changing the interrupt interval (e.g., `-i 1000`, or perhaps to use random intervals) change the trace outcome? Is the program efficiently using the CPU?

Ans:
Thread 0 keeps running until thread 1 changes memory 2000 to 1.
No. The program is NOT efficiently using the CPU.


 2000      ax    cx          Thread 0                Thread 1         
    0       0     0   
    0       0     0   1000 test $1, %ax
    0       0     0   1001 je .signaller
    0       0     0   1002 mov  2000, %cx
    0       0     0   1003 test $1, %cx
    0       0     0   1004 jne .waiter
    0       0     0   1002 mov  2000, %cx
    0       0     0   1003 test $1, %cx
    0       0     0   1004 jne .waiter
    0       0     0   1002 mov  2000, %cx
    0       0     0   1003 test $1, %cx
    0       0     0   1004 jne .waiter
    0       0     0   1002 mov  2000, %cx
    0       0     0   1003 test $1, %cx
    0       0     0   1004 jne .waiter
    0       0     0   1002 mov  2000, %cx
    0       0     0   1003 test $1, %cx
    0       0     0   1004 jne .waiter
    0       0     0   1002 mov  2000, %cx
    0       0     0   1003 test $1, %cx
    0       0     0   1004 jne .waiter
    0       0     0   1002 mov  2000, %cx
    0       0     0   1003 test $1, %cx
    0       0     0   1004 jne .waiter
    0       0     0   1002 mov  2000, %cx
    0       0     0   1003 test $1, %cx
    0       0     0   1004 jne .waiter
    0       0     0   1002 mov  2000, %cx
    0       0     0   1003 test $1, %cx
    0       0     0   1004 jne .waiter
    0       0     0   1002 mov  2000, %cx
    0       0     0   1003 test $1, %cx
    0       0     0   1004 jne .waiter
    0       0     0   1002 mov  2000, %cx
    0       0     0   1003 test $1, %cx
    0       0     0   1004 jne .waiter
    0       0     0   1002 mov  2000, %cx
    0       0     0   1003 test $1, %cx
    0       0     0   1004 jne .waiter
    0       0     0   1002 mov  2000, %cx
    0       0     0   1003 test $1, %cx
    0       0     0   1004 jne .waiter
    0       0     0   1002 mov  2000, %cx
    0       0     0   1003 test $1, %cx
    0       0     0   1004 jne .waiter
    0       0     0   1002 mov  2000, %cx
    0       0     0   1003 test $1, %cx
    0       0     0   1004 jne .waiter
    0       0     0   1002 mov  2000, %cx
    0       0     0   1003 test $1, %cx
    0       0     0   1004 jne .waiter
    0       1     0   ------ Interrupt ------  ------ Interrupt ------  
    0       1     0                            1000 test $1, %ax
    0       1     0                            1001 je .signaller
    1       1     0                            1006 mov  $1, 2000
    1       1     0                            1007 halt
    1       0     0   ----- Halt;Switch -----  ----- Halt;Switch -----  
    1       0     1   1002 mov  2000, %cx
    1       0     1   1003 test $1, %cx
    1       0     1   1004 jne .waiter
    1       0     1   1005 halt