//Quicksort en cuatro hilos
// se compila así­:
// gcc QS2H.c -o QS2H -l pthread
// se ejecuta: ./QS2H

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <pthread.h>  //Para usar la librería de los hilos

long inicio1;
long final1;

long inicio2;
long final2;

long inicio3;
long final3;

long inicio4;
long final4;

void swap(long *x,long *y) //Intercambia elementos en el vector
{
   long temp;
   temp = *x;
   *x = *y;
   *y = temp;
}

long choose_pivot(long i,long j ) //Escoje como pivote el elemento central del vector
{
   return((i+j) / 2);
}

void printvector(long vector[]) //Muestra el vector en pantalla
{
   long i;
   for(i = inicio1; i < final1 + 1; i++)
      printf("%12li\n", vector[i]);
   printf("\n");
}

void quicksort(long vector[],long m, long n)
{
   long key,i,j,k;
   if( m < n)
   {
      k = choose_pivot(m,n);
      swap(&vector[m],&vector[k]);
      key = vector[m];
      i = m+1;
      j = n;
      while(i <= j)
      {
         while((i <= n) && (vector[i] <= key))
                i++;
         while((j >= m) && (vector[j] > key))
                j--;
         if( i < j)
              swap(&vector[i],&vector[j]);
      }
      //Intercambia (swap) dos elementos
      swap(&vector[m],&vector[j]);
      // Efectúa los llamados recursivos
      quicksort(vector,m,j-1);
      quicksort(vector,j+1,n);
   }
}
double timeval_diff(struct timeval *a, struct timeval *b)
{
  return
    (double)(a->tv_sec + (double)a->tv_usec/1000000) -
    (double)(b->tv_sec + (double)b->tv_usec/1000000);
}

void *HiloQuickSort1( void *ptr)
{
    long *vector;
    vector = (long *) ptr;
    quicksort(vector, inicio1, final1);
}

void *HiloQuickSort2( void *ptr)
{
    long *vector;
    vector = (long *) ptr;
    quicksort(vector, inicio2, final2);
}

void *HiloQuickSort3( void *ptr)
{
    long *vector;
    vector = (long *) ptr;
    quicksort(vector, inicio3, final3);
}

void *HiloQuickSort4( void *ptr)
{
    long *vector;
    vector = (long *) ptr;
    quicksort(vector, inicio4, final4);
}


//Este método mueve el pivote y devuelve la posición donde se colocó
long MuevePivote(long vector[],long m,long n)
{
   long key,i,j,k;
   if( m < n)
   {
      k = choose_pivot(m,n);
      swap(&vector[m],&vector[k]);
      key = vector[m];
      i = m+1;
      j = n;
      while(i <= j)
      {
         while((i <= n) && (vector[i] <= key))
                i++;
         while((j >= m) && (vector[j] > key))
                j--;
         if( i < j)
              swap(&vector[i],&vector[j]);
      }
      // swap two elements
      swap(&vector[m],&vector[j]);
      return j;
   }
}

//===============================================

void main()
{
     pthread_t thread1, thread2, thread3 , thread4;  //Se declaran los cuatro hilos
     int  iret1, iret2, iret3, iret4;

     struct timeval t_ini, t_fin;
     double secs;

     long const MAX_ELEMENTS = 100000000; //Tamaño del vector

     long *vector = malloc(MAX_ELEMENTS * sizeof(long));

     long i = 0;
     long pos;  //variable para determinar donde se colocó el pivote
     long pos2;
     long pos3;
     //Se genera el vector aleatorio
     long iseed = (long)time(NULL);
     srand (iseed);
     for(i = 0; i < MAX_ELEMENTS; i++ ){
         vector[i] = rand() % 999999999999;
     }

/*
     printf("\nMuestra el vector recién generado:\n");
     printvector(vector);
*/
     gettimeofday(&t_ini, NULL);

     //Escoje y coloca el pivote para dividir el vector en dos
     inicio1 = 0;
     final4  = MAX_ELEMENTS - 1;
     pos = MuevePivote(vector, inicio1, final4);
     final2 = pos - 1;
     pos2 = MuevePivote(vector, inicio1, final2);
     inicio3 = pos + 1;
 pos3 = MuevePivote(vector, inicio3, final4);
     // Se ejecuta el hilo que tiene la parte izquierda del vector
     final1 = pos2 - 1;
     iret1 = pthread_create( &thread1, NULL, HiloQuickSort1, (void*) vector);
     inicio2 = pos2 + 1;
     iret2 = pthread_create( &thread2, NULL, HiloQuickSort2, (void*) vector);
     final3 = pos3 - 1;
     iret3 = pthread_create( &thread3, NULL, HiloQuickSort3, (void*) vector);
     inicio4 = pos3 + 1;
     iret4 = pthread_create( &thread4, NULL, HiloQuickSort4, (void*) vector);


     pthread_join( thread1, NULL);
     pthread_join( thread2, NULL);
     pthread_join( thread3, NULL);
     pthread_join( thread4, NULL);

     gettimeofday(&t_fin, NULL);
     secs = timeval_diff(&t_fin, &t_ini);
     printf("%.8g milliseconds\n", secs * 1000.0);

     //printf("\nVector ordenado:\n");
     //inicio1 = 0;
     //final1  = MAX_ELEMENTS - 1;
     //printvector(vector);
     //printf("Thread 1 returns: %d\n",iret1);
     //printf("Thread 2 returns: %d\n",iret2);
     exit(0);
}
