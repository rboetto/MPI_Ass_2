#include <stdio.h>
#include <mpi.h>

int check_pow2(int x);

int main (int argc, char ** argv) {

printf("0 %i\n", check_pow2(0));
printf("0 %i\n", check_pow2(1));
printf("1 %i\n", check_pow2(2));
printf("0 %i\n", check_pow2(3));
printf("1 %i\n", check_pow2(4));
printf("1 %i\n", check_pow2(8));
printf("0 %i\n", check_pow2(10));
printf("1 %i\n", check_pow2(16));
printf("1 %i\n", check_pow2(32));



return 0;
}

void broadcard_p0_pow2(int * x, MPI_COMM comm) {



int id, p;

MPI_Get_rank(&id, comm);
MPI_Get_size(&p, comm);

if (!check_pow2(p)) return 1;

}

int check_pow2(int x) {
if (!x || ((x & 1) == 1)) return 0;
while((x & 1) != 1) x = x >> 1;
return x >> 1 ? 0 : 1;

}
