#include <stdio.h>
#include <mpi.h>
#include<stdlib.h>
#include <unistd.h>
//AUTOR: ROBERTO URETA
int rank, size;
MPI_Status status;

double tiempoEjecucion(double ini, double fin){
	return fin-ini;
}

float F1(int i){
	sleep(1);
	return i+1;
}

float F2(int j){
	sleep(1);
	return j+2;
}

float F3(int k){
	sleep(1);
	return k+3;
}

float F4(float a, float b, float c){
	sleep(2);
	return (a+b+c)*2;
}

float F5(float a, float d){
	sleep(3);
	return (a/d)+1;
}

float F6(float b, float d){
	sleep(3);
	return (b/d)+2;
}

float F7(float c, float d){
	sleep(3);
	return (c/d)*2;
}
//funcion secuencial
float operaciones(int i, int j, int k){
	float a,b,c,d,x,y,z;
	a=F1(i);
	b=F2(j);
	c=F3(k);
	d=F4(a,b,c);
	x=F5(a,d);
	y=F6(b,d);
	z=F7(c,d);
	return x+y+z;
}
//funcion que necesita 3 procesos para trabajar de manera paralela
float operaciones1(int i, int j, int k){
	float a,b,c,d,x,y,z;
	if(rank == 0){
		a=F1(i);

		MPI_Recv(&b,1,MPI_FLOAT,1,99,MPI_COMM_WORLD,&status);
		MPI_Recv(&c,1,MPI_FLOAT,2,99,MPI_COMM_WORLD,&status);
		d=F4(a,b,c);

		MPI_Send(&d,1,MPI_FLOAT,1,99,MPI_COMM_WORLD);
		MPI_Send(&d,1,MPI_FLOAT,2,99,MPI_COMM_WORLD);

		x=F5(a,d);
		MPI_Recv(&y,1,MPI_FLOAT,1,99,MPI_COMM_WORLD,&status);
		MPI_Recv(&z,1,MPI_FLOAT,2,99,MPI_COMM_WORLD,&status);
		return x+y+z;
		
	}
	else if(rank==1){
		b=F2(j);
		MPI_Send(&b,1,MPI_FLOAT,0,99,MPI_COMM_WORLD);

		MPI_Recv(&d,1,MPI_FLOAT,0,99,MPI_COMM_WORLD,&status);
		y=F6(b,d);
		MPI_Send(&y,1,MPI_FLOAT,0,99,MPI_COMM_WORLD);
		
	}
	else if (rank==2)
	{
		c=F3(k);
		MPI_Send(&c,1,MPI_FLOAT,0,99,MPI_COMM_WORLD);
		
		MPI_Recv(&d,1,MPI_FLOAT,0,99,MPI_COMM_WORLD,&status);
		z=F7(c,d);
		MPI_Send(&z,1,MPI_FLOAT,0,99,MPI_COMM_WORLD);		
	}
	return 0;
}

//SE DEBE UTILIZAR 3 PROCESOS PARA QUE FUNCIONE DE MANERA PARALELA.
int
main(int argc, char *argv[]) {
   double inicio,fin;
   //cambiar variables para probar con otros resultados.
   int var1,var2,var3;
   var1 = 4;
   var2 = 2;
   var3 = 3; 
   MPI_Init(&argc, &argv);
   MPI_Comm_rank(MPI_COMM_WORLD, &rank);
   MPI_Comm_size(MPI_COMM_WORLD, &size);
   //Se ocupa un solo proceso para probar las operaciones de manera secuencial.
   if (rank==0)
   {
   		inicio = MPI_Wtime();
   		float resultado = operaciones(var1,var2,var3);
   		fin = MPI_Wtime();
   		printf("resultado: %f\n", resultado);
   		double tiempo = tiempoEjecucion(inicio,fin);
   		printf("Tiempo de ejecucion secuencial: %f miliseg\n",tiempo*1000);
   		printf("Tiempo de ejecucion secuencial: %f segundos\n", tiempo);
   }
   inicio = MPI_Wtime();
   //funcion que utiliza los 3 procesos para probar paralelizacion.
   float res2 = operaciones1(var1,var2,var3);
   fin = MPI_Wtime();
   if (res2!=0)
   {
   	 printf("resultado usando 3 procesos: %f\n", res2);
   	 double tiempo = tiempoEjecucion(inicio,fin);
   	 printf("Tiempo de ejecucion usando paralelizacion: %f miliseg\n",tiempo*1000);
   	 printf("Tiempo de ejecucion usando paralelizacion: %f segundos\n", tiempo);
   }
   
   MPI_Finalize();


}




