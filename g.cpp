#include <iostream>
#include <mpi.h>
#include <random>
#include <cassert>

int Size, Rank;

int const NMAX = 9000000, NTIMES = 10;
double a[NMAX];


void first_task() {
	std::cout << "FIRST TASK" << std::endl;
	double _2_d20=pow(2.0,20);
	int n = 0, lmax = 0;
	double max = 0;
	while(n <= NMAX){
		double time_start = MPI_Wtime();
		for (int i=0; i<NTIMES; i++){
			if (Rank==0){
				MPI_Status St;
				MPI_Send(a, n, MPI_DOUBLE, 1, 5, MPI_COMM_WORLD);
				MPI_Recv(a, n, MPI_DOUBLE, 1, 5, MPI_COMM_WORLD, &St);
			}
			if (Rank==1){
				MPI_Status St;
				MPI_Recv(a, n, MPI_DOUBLE, 0, 5, MPI_COMM_WORLD, &St);
				MPI_Send(a, n, MPI_DOUBLE, 0, 5, MPI_COMM_WORLD);
			}
		}
		double time=(MPI_Wtime()-time_start)/(2*NTIMES);
		double bandwidth = (8.0*n/_2_d20)/time;
		if(max < bandwidth){
			max = bandwidth;
			lmax = 8*n;
		}
		if (Rank==0){
			if (n==0) 
				std::cout<<"latency: " <<time<<" s"<< std::endl;
			else 
				std::cout<<8*n<<" bytes" <<" bandwidth "<<bandwidth<<"MB/s"<< std::endl;
		}
		if(n==0) n=1;
		else n*=2;
	}
	if (Rank==0){
		std::cout<<"max bandwidth "<<max <<" MB/s length "<<lmax<<" bytes" << std::endl;
	}
}

void third_task() {
	double a=0, b=0, c=0;
	int Posl=(Rank+1) % Size; //Номер последующего процесса
	int Pred=Rank ? Rank-1 : Size-1; //Номер предшествующего процесса
	a=Rank+0.7;
	MPI_Status St;
	MPI_Sendrecv(&a, 1, MPI_DOUBLE, Posl, 5, &b, 1, MPI_DOUBLE, Pred, 5, MPI_COMM_WORLD, &St);
	MPI_Sendrecv(&a, 1, MPI_DOUBLE, Pred, 5, &c, 1, MPI_DOUBLE, Posl, 5, MPI_COMM_WORLD, &St);
	std::cout<<"Process "<<Rank<<" a="<<a<<" b="<<b<<" c="<<c<<std::endl;
	std::cout << "THIRD TASK\n";
}

void fifth_task() {
	double a=0, b=0,c=0;
	MPI_Request Req[4];
	MPI_Status St[4];
	int Posl=(Rank+1) % Size;
	int Pred=Rank ? Rank-1 : Size-1; 
	a=Rank+0.4;
	MPI_Send_init(&a, 1, MPI_DOUBLE, Posl, 5, MPI_COMM_WORLD, &Req[0]);
	MPI_Recv_init(&b, 1, MPI_DOUBLE, Pred, 5, MPI_COMM_WORLD, &Req[1]);
	MPI_Send_init(&a, 1, MPI_DOUBLE, Pred, 5, MPI_COMM_WORLD, &Req[2]);
	MPI_Recv_init(&c, 1, MPI_DOUBLE, Posl, 5, MPI_COMM_WORLD, &Req[3]);
	for (int k=1; k<=3; k++){
		a+=0.1;
		MPI_Startall(4,Req);
		MPI_Waitall(4,Req,St);
	}
	for (int k = 0; k < 4; k++)
		MPI_Request_free(&Req[k]);
	std::cout<<"Process "<<Rank<<" a="<<a<<" b="<<b<<" c="<<c<<std::endl;
	std::cout << "FIFTH TASK\n";
}

int main() {
	MPI_Init(0, 0);
	MPI_Comm_size(MPI_COMM_WORLD, &Size);
	MPI_Comm_rank(MPI_COMM_WORLD, &Rank);

	first_task();
	MPI_Barrier(MPI_COMM_WORLD);
	if (Rank == 0) std::cout << std::endl;
	MPI_Barrier(MPI_COMM_WORLD);

	third_task();
	MPI_Barrier(MPI_COMM_WORLD);
	if (Rank == 0) std::cout << std::endl;
	MPI_Barrier(MPI_COMM_WORLD);

	fifth_task();
	MPI_Barrier(MPI_COMM_WORLD);
	if (Rank == 0) std::cout << std::endl;
	MPI_Barrier(MPI_COMM_WORLD);

	MPI_Finalize();
}
