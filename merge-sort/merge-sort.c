#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
/*
global array so as every thread can access it easly
*/
int *a;
// left index of array and right index for the merge sort function
typedef struct {
int l;
int r;
}myMerge;
// classical merge function without any change
// as it will not be involved with threding
void merge(int l , int m, int r){
//printf("hello \n");

int i,j,k;
int n1 = m-l+1;
int n2 = r-m;
int R[n2];

int L[n1];


for(i=0;i<n1;i++){
L[i] = a[l+i];
}

for (j=0;j<n2;j++){
R[j] = a[m+1+j];
}

k = l; i = 0; j = 0;
while(i<n1 &&j<n2){
if(L[i]<=R[j]){
a[k] = L[i];
i++;
}
else{
a[k] = R[j];
j++;
}
k++;
}
while(i<n1){
a[k] = L[i];
i++;
k++;
}
while (j<n2){
a[k] = R[j];
j++;
k++;
}

}
/* ~ the merge sort is where most of threading happens
   ~ i will take the struct from intial call that holds the 2 indexies
   ~ i will compute the mean (middle) of the array to begin dividing it
   ~ then create 2 threads one holds left sub array and the other right half
   ~ sending their own left right indexies with them in "myMerge" struct
   ~ and repeating till array size is equal to one then the merge function is called for each thread to sort my array

*/
void *mergeSortt(void * args){
//printf("flag\n");
myMerge *mm = (myMerge*)malloc(sizeof(myMerge));
mm = args;
int ll = mm->l;
int rr = mm->r;

if( ll < rr ){
int q = (ll+(rr-1))/2;

//printf("%d the Q \n ",q);
/*first merge call*/

//mm1->m = q;
pthread_t thread1;
pthread_t thread2;

myMerge *mm1 = (myMerge*)malloc(sizeof(myMerge));
mm1->l = ll;
mm1->r = q;
myMerge *mm2 = (myMerge*)malloc(sizeof(myMerge));
mm2->l = q+1;
mm2->r = rr;
//printf("left array l= %d r = %d  ",mm1->l,mm1->r);

pthread_create(&thread1,NULL, &mergeSortt, mm1);

pthread_create(&thread2,NULL, &mergeSortt, mm2);


pthread_join(thread1,NULL);
pthread_join(thread2,NULL);

/*second  merge call*/


merge(ll,q,rr);

}


}





int main()
{

FILE *fp;
fp = fopen("input.txt","r");
int m ;

fscanf(fp,"%d",&m);

a = (int*)malloc(m*sizeof(int));

int i;
for (i=0;i<m;i++){
fscanf(fp,"%d",&a[i]);
//printf(" %d ",a[i]);

}
//printf(" \n");

/* intial call of merge sort  */
myMerge *mm3 = (myMerge*)malloc(sizeof(myMerge));
mm3->l = 0;
mm3->r = (m-1);
pthread_t threads;

pthread_create(&threads,NULL,&mergeSortt, mm3);
pthread_join(threads,NULL);
for (i = 0 ;i<m ;i++){
printf(" %d ",a[i]);
}



}
