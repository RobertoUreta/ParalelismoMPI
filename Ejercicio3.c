#include <stdio.h>
#include <mpi.h>
#include<stdlib.h>
#include <time.h>
#include <unistd.h>
//AUTOR: ROBERTO URETA
int rank, size;
MPI_Status status;
int N = 5;

double tiempoEjecucion(double ini, double fin){
	return fin-ini;
}

void F1(float a[],float* x){
	sleep(1);
	*x = *x + 1;
	int i = (rand()%N);
	a[i] = *x;
}

void F2(float b[],float* y){
	sleep(1);
	*y = *y + 2;
	int i = (rand()%N);
	b[i] = *y;	
}

void F3(float b[],float* x){
	sleep(2);
	*x = *x * 3;
	int i = (rand()%N);
	b[i] = *x;
}

void F4(float c[],float* y){
	sleep(2);
	*y = *y * 4;
	int i = (rand()%N);
	c[i] = *y;
}

void F5(float c[],float* x){
	sleep(3);
	*x = *x / 5;
	int i = (rand()%N);
	c[i] = *x;
}

void F6(float a[],float* y){
	sleep(3);
	*y = *y / 6;
	int i = (rand()%N);
	a[i] = *y;
}

void llenarArreglos(float a[],float b[], float c[]){
	int i;
	for(i = 0; i < N; i++)
	{
		a[i] = 0;
		b[i] = 0;
		c[i] = 0;
	}
}

void imprimirArreglos(float a[],float b[], float c[]){
	int i;
	printf("arreglo a\n");
	for(i = 0; i < N; i++)
	{
		printf(" %f,", a[i]);
	}
	printf("\n");
	printf("arreglo b\n");
	for(i = 0; i < N; i++)
	{
		printf(" %f,", b[i]);
	}
	printf("\n");
	printf("arreglo c\n");
	for(i = 0; i < N; i++)
	{
		printf(" %f,", c[i]);
	}
	printf("\n");
}

void secuencial(){
	float a[N], b[N], c[N], x=0,y=0;
	llenarArreglos(a,b,c);
	srand(time(NULL));
    //imprimirArreglos(a,b,c);
	F1(a,&x);
	F2(b,&y);
	F3(b,&x);
	F4(c,&y);
	F5(c,&x);
	F6(a,&y);
	imprimirArreglos(a,b,c);
}

void paralelo(){
	float a[N], b[N], c[N], x=0,y=0;
	llenarArreglos(a,b,c);
	if (rank==0)
	{
		double inicio = MPI_Wtime();
		F1(a,&x);
		MPI_Recv(&b,1,MPI_FLOAT,1,99,MPI_COMM_WORLD,&status);
		F3(b,&x);
		MPI_Send(&a,1,MPI_FLOAT,1,99,MPI_COMM_WORLD);
		MPI_Recv(&c,1,MPI_FLOAT,1,99,MPI_COMM_WORLD,&status);
		F5(c,&x);
		MPI_Recv(&a,1,MPI_FLOAT,1,99,MPI_COMM_WORLD,&status);
		imprimirArreglos(a,b,c);
		double fin = MPI_Wtime();
		printf("tiempo paralelo: %f seg\n", tiempoEjecucion(inicio,fin));
	}
	else if (rank == 1)
	{
		F2(b,&y);
		MPI_Send(&b,1,MPI_FLOAT,0,99,MPI_COMM_WORLD);

		F4(c,&y);
		MPI_Send(&c,1,MPI_FLOAT,0,99,MPI_COMM_WORLD);
		MPI_Recv(&a,1,MPI_FLOAT,0,99,MPI_COMM_WORLD,&status);
		F6(a,&y);
		MPI_Send(&a,1,MPI_FLOAT,0,99,MPI_COMM_WORLD);
	}	
}

//PARA ESTE EJERCICIO CONSIDERE QUE LAS VARIABLES X e Y 
//SERAN ASIGNADAS A LOS ARREGLOS SEGUN LO VISTO EN EL ENUNCIADO,
//LA POSICION EN EL ARREGLO ES ESCOGIDA DE MANERA AL AZAR.
//ADEMAS LAS VARIABLES X,Y VAN CAMBIANDO SUS VALORES A MEDIDA 
//QUE SE USAN LAS FUNCIONES F1,F2,F3,F4,F5,F6.
//PARA QUE FUNCIONE LA FUNCION PARALELO SE NECESITA UTILIZAR 2 PROCESOS.
int
main(int argc, char *argv[]) {
   double inicio,fin;
   
   MPI_Init(&argc, &argv);
   MPI_Comm_rank(MPI_COMM_WORLD, &rank);
   MPI_Comm_size(MPI_COMM_WORLD, &size);
   if (rank == 0)
   {
   	inicio = MPI_Wtime();
   	secuencial();
   	fin = MPI_Wtime();
   	printf("tiempo secuencial: %f seg\n", tiempoEjecucion(inicio,fin));
   }
   paralelo();
   
   MPI_Finalize();

}

