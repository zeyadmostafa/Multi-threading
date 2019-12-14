#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
int* matA;
int* matB;
int* matC;
int* matC2;
typedef struct {

int ai;// matrix a row index
int bi;// matrix b colm index
int anp;// num of elements in colm  of a and row of b
int totalColmsOfB;
}mymatrix;

/*
~ i scaned the matrices and put them in A and B
~ then a threads array in the elements method by size of new matrix and in row method by row number
~ then i send the sub matrcies indiciies  using structs as threads each one will be (element / row) calculate on ots own
~  and then in the function that calculates each thread  i will put each element in place
~ then i print each method ouput with its taken time in a file
*/
void *matrixmulti(void * mymatrixx){
//printf("this is mul \n");

mymatrix *m = (mymatrix*)malloc(sizeof(*m));
m = mymatrixx;
int i = 0;
int Newelement = 0;
int rowOfA;
int colmOfB;
int colmOfA_rowOfB;
int total_colB = m->totalColmsOfB;
colmOfA_rowOfB = m->anp;// anp is the colm num in mat A and row num in mat B
rowOfA=m->ai;//ai = row index of A
colmOfB = m->bi;//bi = colm index of B




for(i=0;i<colmOfA_rowOfB;i++){



Newelement = Newelement+ (matA[rowOfA * colmOfA_rowOfB +i] )*( matB[colmOfB + total_colB *i]);
}

matC[rowOfA*total_colB+colmOfB] = Newelement;

//printf("this is element %d = %d \n",rowOfA*total_colB+colmOfB,matC[rowOfA*total_colB+colmOfB]);




}

void* matmul(void* thematrix){
mymatrix *m2 = (mymatrix*)malloc(sizeof(*m2));
m2 = thematrix;
int i = 0;
int Newelement = 0;
int rowOfA;
int colmOfB;
int colmOfA_rowOfB;
int total_colB = m2->totalColmsOfB;
colmOfA_rowOfB = m2->anp;// anp is the colm num in mat A and row num in mat B
rowOfA=m2->ai;//ai = row index of A
colmOfB = m2->bi;//bi = colm index of B


int y = 0;
for(y=0;y<total_colB;y++){
for(i=0;i<colmOfA_rowOfB;i++){



Newelement = Newelement+ (matA[rowOfA * colmOfA_rowOfB +i] )*( matB[colmOfB + total_colB *i]);
}

matC2[rowOfA*total_colB+colmOfB] = Newelement;
//printf("this is element %d = %d \n",rowOfA*total_colB+colmOfB,matC2[rowOfA*total_colB+colmOfB]);

colmOfB++;
Newelement = 0;

}
}
int main()
{
clock_t start,end;
double time_used;
clock_t start2,end2;
double time_used2;
FILE* fp;

int m,n;//first matrix dimintions
int p,q;//second matrix dimintions
int i = 0;
int j = 0;
fp = fopen("file1.txt","r");
/* reading first 2 numbers , matrix A dimintions*/
fscanf(fp ,"%d",&m);

fscanf(fp ,"%d",&n);
/* reading first matrix a */
matA = (int*)malloc(m*n*sizeof(int));

for(i=0;i<m;i++){
for(j=0;j<n;j++){
fscanf(fp ,"%d",&matA[i*n+j]);
}

}
int x=0;
int y=0;
/* reading first 2 numbers , matrix B dimintions*/

fscanf(fp ,"%d",&p);

fscanf(fp ,"%d",&q);
//********* dont forget to handel if q!=n************//
/* reading first matrix A */
matB = (int*)malloc(p*q*sizeof(int));

for(x=0;x<p;x++){
for(y=0;y<q;y++){
fscanf(fp ,"%d",&matB[x*q+y]);
}

}


matC = (int*)malloc(m*q*sizeof(int));

pthread_t threads[m*q];
int ii ;
int jj ;
int ti=0 ;
start = clock();
for (ii=0;ii<m;ii++){
for(jj=0;jj<q;jj++){

mymatrix* mat = malloc(sizeof(mat));
mat->totalColmsOfB = q; // sending number of coloms of matrix b
mat->ai = ii; // ai = matrix a row index
mat->anp = n; // number of coloms in A and rows in B
mat->bi = jj; // bi = matrix B colom index

//printf("this is ai =%d bi = %d\n",mat->a[ii*q+jj],mat->b[ii*q+jj]);

pthread_create(&threads[ti],NULL, &matrixmulti, mat);

ti++;

}

}
int cc = 0;
for (cc=0;cc<m*q;cc++){

pthread_join(threads[cc],NULL);
}
end = clock();
time_used = ((double)(end-start))/CLOCKS_PER_SEC;
//pthread_exit(NULL);
FILE *f = fopen("output.txt","ab");
int lll =0;
int ddd = 0;
for(lll=0;lll<m;lll++){

for(ddd=0;ddd<q;ddd++){
//fprintf(f,"this child has ended %d\n",pid);
fprintf( f,"%d  ",*(matC+lll*q+ddd));

}
fprintf(f,"\n");

}
fprintf(f,"procrdure 1 time = %f\n",time_used);



//pthread_exit(NULL);

/***************************************************************************************/

matC2 = (int*)malloc(m*q*sizeof(int));
pthread_t threads2[m];
int ii2 ;
int jj2 ;
int ti2=0 ;
start2 = clock();
for (ii2=0;ii2<m;ii2++){

mymatrix* mat2 = malloc(sizeof(mat2));
mat2->totalColmsOfB = q; // sending number of coloms of matrix b
mat2->ai = ii2; // ai = matrix a row index
mat2->anp = n; // number of coloms in A and rows in B
mat2->bi = jj2; // bi = matrix B colom index

//printf("this is ai =%d bi = %d\n",mat->a[ii*q+jj],mat->b[ii*q+jj]);

pthread_create(&threads2[ti2],NULL, &matmul, mat2);

ti2++;



}

int cc2 = 0;
for (cc2=0;cc2<m;cc2++){

pthread_join(threads2[cc2],NULL);
}
end2 = clock();
time_used2 = ((double)(end2-start2))/CLOCKS_PER_SEC;
//pthread_exit(NULL);
//FILE *f = fopen("output.txt","ab");
int lll2 =0;
int ddd2 = 0;
for(lll2=0;lll2<m;lll2++){

for(ddd2=0;ddd2<q;ddd2++){
//fprintf(f,"this child has ended %d\n",pid);
fprintf( f,"%d  ",*(matC2+lll2*q+ddd2));

}
fprintf(f,"\n");

}
fprintf(f,"procrdure 2 time = %f",time_used2);





    return 0;
}
