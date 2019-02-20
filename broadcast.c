#include <stdio.h>
#include <mpi.h>

void broadcast_p0_pow2(int * x, MPI_Comm comm);
void broadcast_pow2(int * x, int root, MPI_Comm comm);
void broadcast(int * x, int root, MPI_Comm comm);
int inf_pow2(int id);

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
	}
else {
	offset = 1;
	}

int target = id + offset;
while (target < p) {
	MPI_Send(x, 1, MPI_INT, target, 0, comm);
	offset = offset << 1;
	target = id + offset;
	}
}

void broadcast_pow2(int * x, int root, MPI_Comm comm) {

int id, p;

MPI_Comm_rank(comm, &id);
MPI_Comm_size(comm, &p);

if (p != inf_pow2(p) && id == root) {
	printf("Error: Please choose a number of processes that is a power of 2\n");
	MPI_Abort(comm, 0);
}

int adjusted_id = id - root;
if (adjusted_id < 0) adjusted_id += p;

int offset, source;
MPI_Status status;


if (adjusted_id) {
	offset = inf_pow2(adjusted_id);
	source = id - offset;
	MPI_Recv(x, 1, MPI_INT, source < 0 ? source + p : source, MPI_ANY_TAG, comm,  &status);
	offset = offset << 1;
}
else {
	offset = 1;
}

int advance = adjusted_id;
int target = (id + offset) % p;
while (advance < p) {
	MPI_Send (x, 1, MPI_INT, target, 0, comm);
	advance += offset;
	offset = offset << 1;
	target = (id + offset) % p;
}

}

void broadcast(int * x, int root, MPI_Comm comm) {

int id, p;

MPI_Comm_rank(comm, &id);
MPI_Comm_size(comm, &p);

int adjusted_id = id - root;
if (adjusted_id < 0) adjusted_id += p;

int offset, source;
MPI_Status status;


if (adjusted_id) {
	offset = inf_pow2(adjusted_id);
	source = id - offset;
	MPI_Recv(x, 1, MPI_INT, source < 0 ? source + p : source, MPI_ANY_TAG, comm,  &status);
	offset = offset << 1;
}
else {
	offset = 1;
}

int advance = adjusted_id;
int target = (id + offset) % p;
while (advance < p) {
	MPI_Send (x, 1, MPI_INT, target, 0, comm);
	advance += offset;
	offset = offset << 1;
	target = (id + offset) % p;
}

}


// Calculates the inferior power of 2 (inclusive)
// Used to calculate source and target ids for individual message passing
int inf_pow2(int id) {
if (id < 1) return 0;
int ct = -1;
while (id) {
	id = id >> 1;
	ct++;
}
return 1 << ct;
}
