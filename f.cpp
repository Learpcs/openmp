#include <iostream>
#include <mpi.h>
#include <random>
#include <cassert>

int NProc, ProcId;

void first_task() {
	MPI_Comm_size(MPI_COMM_WORLD, &NProc);
	MPI_Comm_rank(MPI_COMM_WORLD, &ProcId);

	std::cout << "FIRST TASK\n";
	std::cout << "Total number of proccesses: " << NProc << std::endl;
	std::cout << "Process identifier: " << ProcId << std::endl;
}

void third_task() {
	std::cout << "THIRD TASK\n";
	if (ProcId == 0) {
		std::mt19937 mt(time(0));
		double x = std::uniform_real_distribution<double>()(mt);
		int sz = sizeof(double) + MPI_BSEND_OVERHEAD;

		char *Buff = new char[sz];
		MPI_Buffer_attach(Buff, sz);
		MPI_Bsend(&x, 1, MPI_DOUBLE, 1, 5, MPI_COMM_WORLD);
		MPI_Buffer_detach(Buff, &sz);
		delete Buff;
	}
	else if (ProcId == 1) {
		double x;
		MPI_Status status;
		MPI_Recv(&x, 1, MPI_DOUBLE, 0, 5, MPI_COMM_WORLD, &status);
		std::cout << "Got the value: " << x << std::endl;
	}
	else {
		std::cout << "Nothing happening" << std::endl;
	}
}

void fifth_task() {
	std::cout << "FIFTH TASK\n";
	//assert(NProc >= 3);
	if(ProcId==1){
		double DVal=7.7;
		MPI_Send(&DVal, 1, MPI_DOUBLE, 0, 5, MPI_COMM_WORLD);
	}
	if(ProcId==2){
		int IVal=3;
		MPI_Send(&IVal, 1, MPI_INTEGER, 0, 6, MPI_COMM_WORLD);
	}
	if(ProcId==0){
		double DVal=0;
		int IVal=0;
		MPI_Status St;
		MPI_Probe(MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &St);
		if (St.MPI_TAG == 5){
			MPI_Recv(&DVal, 1, MPI_DOUBLE, St.MPI_SOURCE, 5, MPI_COMM_WORLD, &St);
			std::cout << "Process 0 has received from process " << St.MPI_SOURCE << " DVal=" << DVal << std::endl;
		}
		if (St.MPI_TAG == 6){
			MPI_Recv(&IVal, 1, MPI_INTEGER, St.MPI_SOURCE, 6, MPI_COMM_WORLD, &St);
			std::cout << "Process 0 has received from process " << St.MPI_SOURCE << " IVal=" << IVal << std::endl;
		}
	}
}

int main() {
	MPI_Init(0, 0);

	first_task();
	MPI_Barrier(MPI_COMM_WORLD);
	if (ProcId == 0) std::cout << std::endl;
	MPI_Barrier(MPI_COMM_WORLD);

	third_task();
	MPI_Barrier(MPI_COMM_WORLD);
	if (ProcId == 0) std::cout << std::endl;
	MPI_Barrier(MPI_COMM_WORLD);

	fifth_task();
	MPI_Barrier(MPI_COMM_WORLD);
	if (ProcId == 0) std::cout << std::endl;
	MPI_Barrier(MPI_COMM_WORLD);

	MPI_Finalize();
}
