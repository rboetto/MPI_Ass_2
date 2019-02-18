#include <stdio.h>
#include <mpi.h>

void broadcast_p0_pow2(int * x, MPI_Comm comm);
int inf_pow2(int id);

int main (int argc, char ** argv) {

MPI_Init(&argc, &argv);

int id, p, x;

MPI_Comm_rank(MPI_COMM_WORLD, &id);
MPI_Comm_size(MPI_COMM_WORLD, &p);

if (!id) x = 123456;

broadcast_p0_pow2(&x, MPI_COMM_WORLD);

printf("Process %i: %i\n", id, x);
}

void broadcast_p0_pow2(int * x, MPI_Comm comm) {

int id, p;

MPI_Comm_rank(comm, &id);
MPI_Comm_size(comm, &p);


if (p != inf_pow2(p) && !id) {
printf("Error: Please choose a number of processes that is a power of 2\n");
MPI_Abort(comm, 0);
}

int offset;
MPI_Status status;

if (id) {
	offset = inf_pow2(id);
	MPI_Recv(x, 1, MPI_INT, id - offset, MPI_ANY_TAG, comm, &status);
	offset = offset << 1; 
} else {
	offset = 1;
}

int target = id + offset;
while (target < p) {
	MPI_Send(x, 1, MPI_INT, target, 0, comm);
	offset = offset << 1;
	target = id + offset;

}
}

// Calculates the inferior power of 2 (inclusive)
// Returns 0 if input is <= 0
int inf_pow2(int id) {
if (id < 1) return 0;
int ct = -1;
while (id) {
	id = id >> 1;
	ct++;
}
return 1 << ct;
}
