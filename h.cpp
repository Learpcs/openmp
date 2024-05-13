#include <iostream>
#include <mpi.h>
#include <random>
int Size, Rank;

const int N = 500;

double Get_A(int m, int n) {
	double Tmp=n-m;
	Tmp*=Tmp;
	double S=(sin(1.0*m)+cos(1.0*m))/(Tmp+1);
	for (int k=2; k<=N; k++)
		S+=(sin(1.0*m)+cos(m-2.0*k))/(Tmp+k*k);
	return S;
}

void first_task() {
	int N_S=N/(Size-1);
	int n_s=N % (Size-1);
	if (Rank==0){//Ведущий процесс
		double *A=new double[N*N];
		double *B=new double[N*N];
		MPI_Request *Req=new MPI_Request[Size-1];
		double Tms=MPI_Wtime();
		for (int k = 1; k < Size; k++)//Неблокирующий прием
			MPI_Irecv(&A[N_S*N*(k - 1)], N_S*N, MPI_DOUBLE, k, 5,
					MPI_COMM_WORLD, &Req[k - 1]);
		for (int k=0; k<Size-1; k++){
			int Idx;
			MPI_Status St;
			MPI_Waitany(Size - 1, Req, &Idx, &St);
			//Данные получены от ведомого
			for (int k=N_S*Idx; k<N_S*(Idx+1); k++) //процесса Idx
				for (int j=0; j<N; j++) //Транпонирование
					B[N*k+j]=A[N*j+k];
		}
		for (int k=N_S*(Size-1); k<N; k++) //Возможно, последние строки матрицы
			for (int j=0; j<N; j++){//не распределены между
				A[N*k+j]=Get_A(k+1,j+1);//ведомыми процессами
				B[N*j+k]=A[N*k+j];
			}
		Tms=MPI_Wtime()-Tms;
		std::cout<<"Time="<<Tms<<" s"<< std::endl;
		delete[] Req;
		delete[] B;
		delete[] A;
	}
	else{//Ведомые процессы
		double *A=new double[N_S*N];
		for (int k=0; k<N_S; k++)
			for (int j=0; j<N; j++)
				A[N_S*k+j]=Get_A(1+N_S*(Rank-1)+k,j+1);
		MPI_Send(A, N_S*N, MPI_DOUBLE, 0, 5, MPI_COMM_WORLD);
		delete[] A;
	}
}

double MPI_Simps(double a, double b, int N, double Func(double), int & ProcID)
{
	double h=(b-a)/(2*N);
	int k, NumProc = Size;
	double S1, mpi_S1, S2, mpi_S2, Tmp;
	MPI_Bcast(&N, 1, MPI_INT, 0, MPI_COMM_WORLD);
	mpi_S1=0;
	mpi_S2=0;
	for (k=1+ProcID; k<N; k+=NumProc){
		Tmp=a+(2*k-1)*h;
		mpi_S1+=Func(Tmp);
		mpi_S2+=Func(Tmp+h);
	}
	MPI_Reduce(&mpi_S1, &S1, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
	MPI_Reduce(&mpi_S2, &S2, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
	S1+=Func(b-h);
	return h*(Func(a) +Func(b) +4.0*S1 +2.0*S2)/3.0;
}

double My_F(double x) {
	return 1.0/(1.0+x*x);
}

void third_task() {

	double Tms=clock();
	double Intgr=MPI_Simps(0, 1000000, int(1e9), My_F, Rank);
	Tms=(clock()-Tms)/CLOCKS_PER_SEC;
	if(!Rank){
		std::cout<<"Time="<<Tms<<" sec"<<std::endl;
		std::cout.precision(8);
		std::cout<<"Intgr="<<Intgr<<" "<<M_PI/2.0<<std::endl;
	}

}

int const N_new =8;
void UsrFunc(void *In, void *InOut, int *len, MPI_Datatype *Ty)
{
	int *InV=(int *)In;
	int *InOutV=(int *)InOut;
	for (int k=0; k<(*len); k++)
		InOutV[k]=(InV[k]+InOutV[k]) % 8;
}

void fifth_task() {
	int *A=new int[N_new];
	MPI_Op My_Op;
	MPI_Op_create(&UsrFunc, true, &My_Op);

	MPI_Group Gr;
	MPI_Comm_group(MPI_COMM_WORLD, &Gr);
	int Size;
	MPI_Comm_size(MPI_COMM_WORLD, &Size);
	int Rank;
	MPI_Comm_rank(MPI_COMM_WORLD, &Rank);
	MPI_Group New_Group;
	if(Rank<Size/2){
		int * Rks=new int[Size/2];
		for (int k=0; k<Size/2; k++)
			Rks[k]=k;
		MPI_Group_incl(Gr, Size/2, Rks, &New_Group);
		delete[] Rks;
	}
	else{int * Rks=new int[Size-Size/2];
		for (int k=0; k<Size-Size/2; k++)
			Rks[k]=k+Size/2;
		MPI_Group_incl(Gr, Size - Size / 2, Rks, &New_Group);
		delete[] Rks;
	}
	MPI_Comm New_Comm;
	MPI_Comm_create(MPI_COMM_WORLD, New_Group, &New_Comm);
	MPI_Comm_rank(New_Comm, &Rank);
	for (int k=0; k<N_new; k++)
		A[k]=k+Rank;
	if(Rank==0){
		int *B=new int[N_new];
		MPI_Reduce(A, B, N_new, MPI_INT, My_Op, 0, New_Comm);
		for (int k=0; k<N_new; k++)
			std::cout<<B[k]<<" ";
		std::cout<<std::endl;
		delete[] B;
	}
	else{
		MPI_Reduce(A, NULL, N_new, MPI_INT, My_Op, 0, New_Comm);
	}
	MPI_Comm_free(&New_Comm);
	MPI_Op_free(&My_Op);
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
