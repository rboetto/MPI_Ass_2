CC = mpicc

build: broadcast_test.c broadcast.c 
	$(CC) -o tester.o broadcast_test.c

test:
	mpirun -np 23 tester.o broadcast_p0_pow2 broadcast_pow2 broadcast

clean:
	$(RM) *.o
