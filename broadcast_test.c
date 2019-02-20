#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mpi.h>
#include "broadcast.c"

void run_test_broadcast_p0_pow2(MPI_Comm comm);
void run_test_broadcast_pow2(MPI_Comm comm);
void run_test_broadcast(MPI_Comm comm);
void run_test_suite01();
void run_test_suite02();
void run_test_suite03();
MPI_Comm create_pow2_comm();

int main (int argc, char ** argv) {

	MPI_Init(&argc, &argv);
	int id;
	MPI_Comm_rank(MPI_COMM_WORLD, &id);

	if (argc == 1 && !id) {
		printf("\nNo command-line arguments given; no tests will be run\n");
		printf("Please write the method name as a command line argument to test it\n");
		printf("Methods available for testing are: broadcast_p0_p02, broadcast_p02, broadcast\n");
		printf("Tester varies input based on number of processes. Use 4 or more for the best effect.\n\n");
	}

	for (int i = 1; i < argc; i++) {

		if (strcmp(*(argv+i), "all") == 0) {
			if (i > 1) continue;
			run_test_suite01();
			run_test_suite02();
			run_test_suite03();
			break;
		}
		else if (strcmp(*(argv+i), "broadcast_p0_pow2") == 0) {
			run_test_suite01();
		}
		else if (strcmp(*(argv+i), "broadcast_pow2") == 0) {
			run_test_suite02();
		}
		else if (strcmp(*(argv+i), "broadcast") == 0) {
			run_test_suite03();
		}
		else {
			//printf("Ignoring invalid command line argument\n");
		}
	}

	MPI_Finalize();

}


void run_test_suite01() {

	// Exec Test on 2^n Communicator
	MPI_Group world_group, local_group;
	MPI_Comm_group(MPI_COMM_WORLD, &world_group);


	MPI_Comm comm;
	comm  = create_pow2_comm(0);

	if (MPI_COMM_NULL != comm) run_test_broadcast_p0_pow2(comm);

	int ranks[] = { 0 };
	MPI_Group_incl(world_group, 1, ranks, &local_group);

	MPI_Comm_create(MPI_COMM_WORLD, local_group, &comm);

	MPI_Barrier(MPI_COMM_WORLD);

	if (MPI_COMM_NULL != comm) {
		run_test_broadcast_p0_pow2(comm);
	}

	MPI_Barrier(MPI_COMM_WORLD);

	if (MPI_COMM_NULL != comm) {
		MPI_Comm_free(&comm);
	}
	MPI_Group_free(&world_group);
	MPI_Group_free(&local_group);
}

void run_test_suite02() {

	// Exec Test on 2^n Communicator
	MPI_Group world_group, local_group;
	MPI_Comm_group(MPI_COMM_WORLD, &world_group);


	MPI_Comm comm;
	comm  = create_pow2_comm(0);

	if (MPI_COMM_NULL != comm) run_test_broadcast_pow2(comm);

	int ranks[] = { 0 };
	MPI_Group_incl(world_group, 1, ranks, &local_group);

	MPI_Comm_create(MPI_COMM_WORLD, local_group, &comm);

	MPI_Barrier(MPI_COMM_WORLD);

	if (MPI_COMM_NULL != comm) {
		run_test_broadcast_pow2(comm);
	}

	MPI_Barrier(MPI_COMM_WORLD);

	if (MPI_COMM_NULL != comm) {
		MPI_Comm_free(&comm);
	}
	MPI_Group_free(&world_group);
	MPI_Group_free(&local_group);
}

void run_test_suite03() {

	// Exec Test on Global Communicator
	run_test_broadcast(MPI_COMM_WORLD);

	// Exec Test on 2^n Communicator
	MPI_Group world_group, local_group;
	MPI_Comm_group(MPI_COMM_WORLD, &world_group);


	MPI_Comm comm;
	comm  = create_pow2_comm(1);

	if (MPI_COMM_NULL != comm) run_test_broadcast(comm);

	int ranks[] = { 0 };
	MPI_Group_incl(world_group, 1, ranks, &local_group);

	MPI_Comm_create(MPI_COMM_WORLD, local_group, &comm);

	MPI_Barrier(MPI_COMM_WORLD);

	if (MPI_COMM_NULL != comm) {
		run_test_broadcast(comm);
	}

	MPI_Barrier(MPI_COMM_WORLD);

	if (MPI_COMM_NULL != comm) {
		MPI_Comm_free(&comm);
	}
	MPI_Group_free(&world_group);
	MPI_Group_free(&local_group);
}


void run_test_broadcast_p0_pow2(MPI_Comm comm) {

	int id, p;

	MPI_Comm_rank(comm, &id);
	MPI_Comm_size(comm, &p);

	int x = (id+1) * (p+id);
	int y = x;

	if (!id) {
		printf("\n\nInitiating \"broadcast_p0_pow2()\" tests with %i process(es)...\n", p);
		printf("Process 0 is broadcasting the value: %i\n", x);
	}

	MPI_Barrier(comm);

	broadcast_p0_pow2(&x, comm);

	if (!id) {
		printf("----- TEST RESULTS -----\n");
		printf("Process # : Val before broadcast -> after broadcast\n");
	}

	MPI_Barrier(comm);
	printf("%i : %i -> %i\n", id, y, x);
	MPI_Barrier(comm);
	if (!id) printf("----- TEST COMPLETE ----\n\n");

}

void run_test_broadcast_pow2(MPI_Comm comm) {

	int id, p, root;

	MPI_Comm_rank(comm, &id);
	MPI_Comm_size(comm, &p);

	root = (p * 197) % p;
	if (!root) root += p / 2;

	int x = (id+1) * (p+id);
	int y = x;

	if (id == root) {
		printf("\n\nInitiating \"broadcast_pow2()\" with %i process(es)...\n", p);
		printf("Process %i is broadcasting the value: %i\n", root, x);
	}

	MPI_Barrier(comm);

	broadcast_pow2(&x, root, comm);

	if (id == root) {
		printf("----- TEST RESULTS -----\n");
		printf("Process # : Val before broadcast -> after broadcast\n");
	}

	MPI_Barrier(comm);
	printf("%i : %i -> %i\n", id, y, x);
	MPI_Barrier(comm);
	if (!id) printf("----- TEST COMPLETE ----\n\n");

}

void run_test_broadcast(MPI_Comm comm) {

	int id, p, root;

	MPI_Comm_rank(comm, &id);
	MPI_Comm_size(comm, &p);

	root = (p * 197) % p;
	if (!root) root += p / 2;

	int x = (id+1) * (p+id);
	int y = x;

	if (id == root) {
		printf("\n\nInitiating \"broadcast()\" tests with %i process(es)...\n", p);
		printf("Process %i is broadcasting the value: %i\n", root, x);
	}

	MPI_Barrier(comm);

	broadcast(&x, root, comm);

	if (id == root) {
		printf("----- TEST RESULTS -----\n");
		printf("Process # : Val before broadcast -> after broadcast\n");
	}

	MPI_Barrier(comm);
	printf("%i : %i -> %i\n", id, y, x);
	MPI_Barrier(comm);
	if (!id) printf("----- TEST COMPLETE ----\n\n");

}



MPI_Comm create_pow2_comm (int flag) {

	int p;
	MPI_Comm_size(MPI_COMM_WORLD, &p);
	int inf = inf_pow2(p);
	if (flag) inf = inf >> 1;

	int size = p - inf;
	int * excl_ranks = calloc(size, sizeof(int));

	for (int i = 0; i < size; i++) {
	*(excl_ranks+i) = inf + i;
	}

	MPI_Group world_group, group;

	MPI_Comm_group(MPI_COMM_WORLD, &world_group);
	MPI_Group_excl(world_group, size, excl_ranks, &group);

	MPI_Comm newcomm;
	MPI_Comm_create(MPI_COMM_WORLD, group, &newcomm);
	return newcomm;

}
