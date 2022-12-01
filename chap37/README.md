## Chap 37 Hard Disk Drives

This homework uses disk.py to familiarize you with how a modern hard drive works. It has a lot of different options, and unlike most of the other simulations, has a graphical animator to show you exactly what happens when the disk is in action. See the README for details.

**Q1: Compute the seek, rotation, and transfer times for the following sets of requests: `-a 0,-a 6,-a 30,-a 7,30,8`, and finally `-a 10,11,12,13`.**

Ans:

`python3 disk.py -a 0 -G`
Block:   0  Seek:  0  Rotate:165  Transfer: 30  Total: 195

`python3 disk.py -a 6 -G`
Block:   6  Seek:  0  Rotate:345  Transfer: 30  Total: 375

`python3 disk.py -a 30 -G`
Block:  30  Seek: 80  Rotate:265  Transfer: 30  Total: 375

`python3 disk.py -a 7,30,8 -G`
Block:   7  Seek:  0  Rotate: 15  Transfer: 30  Total:  45
Block:  30  Seek: 80  Rotate:220  Transfer: 30  Total: 330
Block:   8  Seek: 80  Rotate:310  Transfer: 30  Total: 420

TOTALS      Seek:160  Rotate:545  Transfer: 90  Total: 795

`python3 disk.py -a 10,11,12,13 -G`
Block:  10  Seek:  0  Rotate:105  Transfer: 30  Total: 135
Block:  11  Seek:  0  Rotate:  0  Transfer: 30  Total:  30
Block:  12  Seek: 40  Rotate:320  Transfer: 30  Total: 390
Block:  13  Seek:  0  Rotate:  0  Transfer: 30  Total:  30

TOTALS      Seek: 40  Rotate:425  Transfer:120  Total: 585

**Q2: Do the same requests above, but change the seek rate to different values: `-S 2,-S 4,-S 8,-S 10,-S 40,-S 0.1`. How do the times change?**

Ans:

`python3 disk.py -a 7,30,8 -G -S 2`
Block:   7  Seek:  0  Rotate: 15  Transfer: 30  Total:  45
Block:  30  Seek: 40  Rotate:260  Transfer: 30  Total: 330
Block:   8  Seek: 40  Rotate:350  Transfer: 30  Total: 420

TOTALS      Seek: 80  Rotate:625  Transfer: 90  Total: 795

`python3 disk.py -a 7,30,8 -G -S 10`
Block:   7  Seek:  0  Rotate: 15  Transfer: 30  Total:  45
Block:  30  Seek:  8  Rotate:292  Transfer: 30  Total: 330
Block:   8  Seek:  8  Rotate: 22  Transfer: 30  Total:  60

TOTALS      Seek: 16  Rotate:329  Transfer: 90  Total: 435


in genegral, faster seekSpeed will result in shorter time in total.
default seekSpeed is 1

**Q3: Do the same requests above, but change the rotation rate: `-R 0.1,-R 0.5,-R 0.01`. How do the times change?**

Ans:

`python3 disk.py -a 7,30,8 -G -R 0.1`
Block:   7  Seek:  0  Rotate:150  Transfer:299  Total: 449
Block:  30  Seek: 80  Rotate:2920  Transfer:301  Total:3301
Block:   8  Seek: 80  Rotate:219  Transfer:300  Total: 599

TOTALS      Seek:160  Rotate:3289  Transfer:900  Total:4349
Default value is 1. Rotate time and transfer time (which is related to rotation speed since it need to go thru one unit) are longer.

**Q4: FIFO is not always best, e.g., with the request stream `-a 7,30,8`, what order should the requests be processed in? Run the shortest seek-time first (SSTF) scheduler (-p SSTF) on this workload; how long should it take (seek, rotation, transfer) for each request to be served?**

Ans:
FIFO, total is 795

`python3 disk.py -a 7,30,8 -c -p SSTF`
Block:   7  Seek:  0  Rotate: 15  Transfer: 30  Total:  45
Block:   8  Seek:  0  Rotate:  0  Transfer: 30  Total:  30
Block:  30  Seek: 80  Rotate:190  Transfer: 30  Total: 300

TOTALS      Seek: 80  Rotate:205  Transfer: 90  Total: 375

SSTF order: 7, 8, 30

total time is shorter.

**Q5: Now use the shortest access-time first(`SATF`) scheduler(`-p SATF`). Does it make any difference for `-a 7,30,8` workload? Find a set of requests where SATF outperforms SSTF; more generally, when is SATF better than SSTF?**

Ans:

`python3 disk.py -a 31,6 -c -p SATF -S 40 -R 1`
`python3 disk.py -a 31,6 -c -p SSTF -S 40 -R 1`

SATF and SSTF are the same. no diff for this one.(both 7, 8, 30) (but shorter than FIFO)

it makes a difference when seek time is shorter then rotate time. 

**Q6: Here is a request stream to try: `-a 10,11,12,13`. What goes poorly when it runs? Try adding track skew to address this problem (`-o skew`). Given the default seek rate, what should the skew be to maximize performance? What about for different seek rates (e.g., `-S 2, -S 4`)? In general, could you write a formula to figure out the skew?**

Ans:

`python3 disk.py -a 10,11,12,13 -c -G`
Block:  10  Seek:  0  Rotate:105  Transfer: 30  Total: 135
Block:  11  Seek:  0  Rotate:  0  Transfer: 30  Total:  30
Block:  12  Seek: 40  Rotate:320  Transfer: 30  Total: 390
Block:  13  Seek:  0  Rotate:  0  Transfer: 30  Total:  30

TOTALS      Seek: 40  Rotate:425  Transfer:120  Total: 585

from 11 to 12 goes poorly when it runs.

`python3 disk.py -a 10,11,12,13 -o 2 -c`
Block:  10  Seek:  0  Rotate:105  Transfer: 30  Total: 135
Block:  11  Seek:  0  Rotate:  0  Transfer: 30  Total:  30
Block:  12  Seek: 40  Rotate: 20  Transfer: 30  Total:  90
Block:  13  Seek:  0  Rotate:  0  Transfer: 30  Total:  30

TOTALS      Seek: 40  Rotate:125  Transfer:120  Total: 285

skew = (track-distance / seek-speed) * rotation-speed / rotational-space-degrees(360 / 12) = math.ceil((40 / 1) * 1 / 30) = 2

The distance between each track is by default 40 distance units, and the default rate of seeking is 1 distance unit per unit time. 

-S seek speed
-R rotation speed

**Q7: Specify a disk with different density per zone, e.g., `-z 10,20,30`, which specifies the angular difference between blocks on the outer, middle, and inner tracks. Run some random requests (e.g., `-a -1 -A 5,-1,0`, which specifies that random requests should be used via the -a -1 flag and that five requests ranging from 0 to the max be generated), and compute the seek, rotation, and transfer times. Use different random seeds. What is the bandwidth (in sectors per unit time) on the outer, middle, and inner tracks?**

Ans:
`python3 disk.py -z 10,20,30 -a -1 -A 5,-1,0 -c -G` (-s default is 0)
Block:  45  Seek: 40  Rotate:310  Transfer: 20  Total: 370
Block:  40  Seek:  0  Rotate:240  Transfer: 20  Total: 260
Block:  22  Seek: 40  Rotate: 85  Transfer: 10  Total: 135
Block:  13  Seek:  0  Rotate:260  Transfer: 10  Total: 270
Block:  27  Seek:  0  Rotate:130  Transfer: 10  Total: 140

TOTALS      Seek: 80  Rotate:1025  Transfer: 70  Total:1175

outer: 3/(135+270+140)=0.0055
middle: 2/(370+260)=0.0032

`python3 disk.py -z 10,20,30 -a -1 -A 5,-1,0 -s 1 -c -G`
Block:   7  Seek:  0  Rotate:245  Transfer: 10  Total: 255
Block:  45  Seek: 40  Rotate: 55  Transfer: 20  Total: 115
Block:  41  Seek:  0  Rotate:260  Transfer: 20  Total: 280
Block:  13  Seek: 40  Rotate:335  Transfer: 10  Total: 385
Block:  26  Seek:  0  Rotate:120  Transfer: 10  Total: 130

TOTALS      Seek: 80  Rotate:1015  Transfer: 70  Total:1165

outer: 3/(255+385+130)=0.0039
middle: 2/(115+280)=0.0051

Q8: A scheduling window determines how many requests the disk can examine at once. Generate random workloads (e.g., `-A 1000,-1,0`, with different seeds) and see how long the SATF scheduler takes when the scheduling window is changed from 1 up to the number of requests. How big of a window is needed to maximize performance? Hint: use the `-c` flag and don’t turn on graphics (`-G`) to run these quickly. When the scheduling window is set to 1, does it matter which policy you are using?

Ans:
`python3 disk.py -A 1000,-1,0 -p SATF -w 1 -c`      // 220125
`python3 disk.py -A 1000,-1,0 -p FIFO -w 1 -c`      // 220125
`python3 disk.py -A 1000,-1,0 -p SSTF -w 1 -c`      // 220125
`python3 disk.py -A 1000,-1,0 -p BSATF -w 1 -c`     // 220125
`python3 disk.py -A 1000,-1,0 -p SATF -w 1000 -c`   // 35475

Set to 1 is equal to FIFO. Maximize performance needs size of the disk.

Q9: Create a series of requests to starve a particular request, assuming an SATF policy. Given that sequence, how does it perform if you use a bounded SATF (BSATF) scheduling approach? In this approach, you specify the scheduling window (e.g., -w 4); the scheduler only moves onto the next window of requests when all requests in the current window have been serviced. Does this solve starvation? How does it perform, as compared to SATF? In general, how should a disk make this trade-off between performance and starvation avoidance?

Ans:

`python3 disk.py -a 24,7,8,9,10,11,0,1,2,3,4,5,6 -p SATF -G`

REQUESTS ['24', '7', '8', '9', '10', '11', '0', '1', '2', '3', '4', '5', '6']

2022-12-01 14:34:56.259 Python[61074:3910998] TSM AdjustCapsLockLEDForKeyTransitionHandling - _ISSetPhysicalKeyboardCapsLockLED Inhibit
Block:   7  Seek:  0  Rotate: 15  Transfer: 30  Total:  45
Block:   8  Seek:  0  Rotate:  0  Transfer: 30  Total:  30
Block:   9  Seek:  0  Rotate:  0  Transfer: 30  Total:  30
Block:  10  Seek:  0  Rotate:  0  Transfer: 30  Total:  30
Block:  11  Seek:  0  Rotate:  0  Transfer: 30  Total:  30
Block:   0  Seek:  0  Rotate:  0  Transfer: 30  Total:  30
Block:   1  Seek:  0  Rotate:  0  Transfer: 30  Total:  30
Block:   2  Seek:  0  Rotate:  0  Transfer: 30  Total:  30
Block:   3  Seek:  0  Rotate:  0  Transfer: 30  Total:  30
Block:   4  Seek:  0  Rotate:  0  Transfer: 30  Total:  30
Block:   5  Seek:  0  Rotate:  0  Transfer: 30  Total:  30
Block:   6  Seek:  0  Rotate:  0  Transfer: 30  Total:  30
Block:  24  Seek: 80  Rotate: 70  Transfer: 30  Total: 180

TOTALS      Seek: 80  Rotate: 85  Transfer:390  Total: 555


`pythons disk.py -a 24,7,8,9,10,11,0,1,2,3,4,5,6 -p BSATF -w 4 -G`
TOTALS      Seek:160  Rotate:545  Transfer:390  Total:1095

need to adjust window to balance starvation and io time.

Q10: All the scheduling policies we have looked at thus far are greedy; they pick the next best option instead of looking for an optimal schedule. Can you find a set of requests in which greedy is not optimal?

Ans: 


`python3 disk.py -a 9,20 -c`

TOTALS      Seek: 40  Rotate:335  Transfer: 60  Total: 435
`python3 disk.py -a 9,20 -c -p SATF`
TOTALS      Seek: 80  Rotate:325  Transfer: 60  Total: 465