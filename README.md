#### This was implemented under the valuable guidance of Prof David Crandall at Indiana University in B551 Elements of AI during Fall 2019.

## Build instructions

Copy the file compile/Makedefs.EXAMPLE to compile/Makedefs and make appropriate changes if necessary.  Make sure that the correct COMPILER_ROOT, LIBGCC_LOC and CONF_LFLAGS are set.

The PLATFORM variable should be set to one of:

- arm-qemu
- arm-bbb
- x86-qemu
- x86-galileo

## Milestone 1

The aim of this milestone is to get familiar with Xinu operating system structure and with modifying and compiling the code. At the end of this project, you will be able to boot the QEMU using the compiled Xinu image file that you create.

In this project we will be modifying the Xinu shell, which currently supports a limited set of commands. Your job is to add a simple hello command which will take a single string argument. If there are more than or less than one string argument it should throw appropriate error like too few or too many arguments.
```
Syntax: 
	xsh$ hello <string>
Output 
	Hello <string>, Welcome to the world of Xinu!!
  ```
  
## Milestone 2

The aim of this project is to get familiar with the Xinu create() system call. At the end of this project. you should be able to create two processes and they should be able to exchange data using a global variable.

In this project we will be creating shell command named prodcons. This shell command will accept one optional parameter. Needless to say, you should implement appropriate error handling. Your shell command will have one global variable 'n' and local variable 'count'. Global variable 'n' will have an extern declaration in prodcons.h, Now we need to define variable 'n' in shell command. Local variable 'count' will have default value of 2000. If the first parameter is present then the count variable should be initialized with value of the first parameter. After that, create two processes named producer and consumer which will take count as an argument.
```
Syntax: 
	xsh$ prodcons [Integer]
  ```
 
## Milestone 3

The aim of this phase is to learn about multi-process synchronization. We have multiple processes sharing a common fixed-sized buffer. Your shell command will have a global integer array 'arr_q' shared among the processes and you will interpret this array as a circular queue (as processes remove elements from the front of the array, other processes can add more elements to those spaces). You will be using the run command implemented in the previous phase to execute your implementation.

```
Syntax:
       xsh$ run prodcons_bb [# of producer processes] [# of consumer processes] [# of iterations the producer runs] [# of iterations the consumer runs]
       
```

Make sure that the number of total producer iterations and the number of total consumer iterations are equal.

For ex:

Using the command $ run prodcons_bb 2 4 10 5:

2 producers, each running 10 iterations -> 20 total producer/write iterations

4 consumers, each running 5 iterations -> 20 total consumer/read iterations

This is so that for each value written to the queue there will be a corresponding read. This will be tested.


## Milestone 4

The aim of this assignment is to understand and implement futures in the Xinu operating system. In this project you will be responsible for developing and testing the system calls required for futures.

When flag -pc is given the program should use the code listed in Future ProdCons Snippet bellow.
```
Syntax:
       xsh$ run futures_test -pc
```
When flag -f is given the program should use the code listed in Future Fibonachchi Snippet bellow.
```
Syntax:
       xsh$ run futures_test -f [Integer]
```

## Milestone 5

This assignment will have you create a stream processing system. Imagine an embedded system that is targeted at processing sensor data from Internet of Things (IoT) devices. Sensors produce a stream of timestamp/value pairs. While each individual measurement is not necessarily interesting, some summary statistics about the behavior of their data is! You will create a system that accepts data -- initially from a static array -- and dispatches it to worker threads to do the summary computation.

You should parameterize the overall number of streams, the depth of the work queue between the producer and the consumer threads, the time window of data that should be stored for each stream and an output "time" which will produce summary data.
```
Syntax:
       xsh$ run tscdf -s [num of streams] -w [work queue depth] -t [time window] -o [output time]
```


## Milestone 6

The aim of this assignment is to implement futures in mode FUTURE_QUEUE and to implement tcsdf using futures in FUTURE_QUEUE mode. In assignment 5 you associated a stream struct with a stream. For this assignment you will associate a future with a stream.

When flag -fq1, -fq2 and -fq3 are given to the futures_test it should use the code listed in Future Queue ProdCons Snippet

```
Syntax:
       xsh$ run futures_test [-fq1|-fq2|fq3]
       
Syntax:
       xsh$ run tscdf_fq -s [num of streams] -w [work queue depth] -t [time window] -o [output time]
```


## Milestone 7

In this assignment, you will be implementing a file system that is modeled after the ext2 file system. This file system will be implemented completely in RAM. You are provided with the filesystem structures and functions to manipulate blocks. Additionally, you are provided with test functions and a shell command to exercise your solution.

You will be implementing core system calls to interact with the file system. fs is used as a prefix to the commands to differentiate between the functions already in Xinu and your new functions. These functions will be developed in fs.c.

```
Syntax:
       xsh$ run fstest
       
````
