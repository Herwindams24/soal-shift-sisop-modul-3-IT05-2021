#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/ipc.h> 
#include <sys/shm.h> 
#include <unistd.h>

#define N 4
#define M 3
#define K 6
#define MAX_THREADS N * K
#define clear() printf("\033[H\033[J")

int matC[N][K];
int matD[N][K];
unsigned long long matE[N][K] = {
{0, 0, 0, 0, 0, 0},
{0, 0, 0, 0, 0, 0},
{0, 0, 0, 0, 0, 0},
{0, 0, 0, 0, 0, 0}
};
// fungsi faktorial
unsigned long long faktorial(unsigned long long num) {
  unsigned long long res;
  if(num <= 1){
    return 1;
  }
  res = num * faktorial(num-1);
  return res;
}
// melakukan perbandingan matriks hasil perkalian dengan matriks inputan user
void *comp() {

    for(int i=0; i<N; i++) {
		for(int j=0; j<K; j++) {
            unsigned long long res = 0;
            if (matC[i][j] == 0 || matD[i][j] == 0) {
                res = 0;
            } else if (matC[i][j] < matD[i][j]) {
				unsigned long long a = matC[i][j];
                res = faktorial(a);
            } else if (matC[i][j] >= matD[i][j]) {
                int a = matC[i][j];
				int b = matD[i][j];
                res = faktorial(a)/faktorial(a-b);
            }

            matE[i][j] = res;
		}
	}
	pthread_exit(0);
}

int main(){
	key_t key = 1500;
	int *share;
	int x, y;
	printf("Input dimensi matriks baru :");
    scanf("%d %d", &x, &y);
	// input matriks baru
	clear();
	for(int i=0; i<x; i++) {
		for(int j=0; j<y; j++) {
			printf("Input matriks baru [%d][%d] :", i, j);
            scanf("%d", &matD[i][j]);
			clear();
		}
	}

	// menerima matriks dari soal2a
	int shmid = shmget(key, 20, IPC_CREAT | 0666);

	share = (int *)shmat(shmid, 0, 0);
	// simpan dan print matriks dari soal2a
	printf("Matriks dari Shared Memory soal a :\n");
    int l=0;
	for(int i=0; i<N; i++) {
		for(int j=0; j<K; j++) {
            matC[i][j] = share[l];
            printf("%d\t", matC[i][j]);
			l++;
		}
        printf("\n");
	}

	shmdt((void *) share);
	shmctl(shmid, IPC_RMID, NULL);
	// print matriks pembanding
	printf("\nMatriks dari input user :\n");
	for(int i=0; i<N; i++) {
		for(int j=0; j<K; j++) {
            printf("%d\t", matD[i][j]);
		}
        printf("\n");
	}
	printf("\nMatriks hasil :\n");	
	pthread_t threads[MAX_THREADS];
	// memanggil fungsi comp
	for(int i=0; i<MAX_THREADS; i++) {
		pthread_create(&threads[i], NULL, comp, NULL);
		pthread_join(threads[i], NULL);
	}
	// print matriks hasil
    for(int i=0; i<N; i++) {
		for(int j=0; j<K; j++) {
			printf("%llu\t", matE[i][j]);
		}
		printf("\n");
	}

	for(int i=0; i<MAX_THREADS; i++) {
		pthread_join(threads[i], NULL);
	}

	return 0;
}


		