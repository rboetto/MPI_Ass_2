Message Passing Interface Assignment 2

#TO BUILD TESTER:

~$ make

#TO EXECUTE ALL TESTS

~$ make test

"make test" will run the tester for all implemented functions (broadcast_p0_pow2, broadcast_pow2, and broadcast) with 23 base processes.
The testing program will build communicators with 2^n and 1 processes and execute tests on those communicators on its own.
The testing program will not test functions against an invalid number of processes, as those will cause the program to abort.
To run tests on individual functions, use:

~$ mpi run -np <n> tester.o <function list>

<n> is the number of processes to exec
<function list> can contain broadcast_p0_pow, broadcast_pow2, and broadcast in any order

#TO CLEAN

~$ make clean

Will remove all .o files.
