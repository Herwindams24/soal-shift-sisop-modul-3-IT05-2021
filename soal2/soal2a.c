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

int matA[N][M];
int matB[M][K];
int matC[N][K];

struct value {
	int i, j;
};
// fungsi perkalian
void *kali(void *arg) {
	struct value *num = arg;
	int res = 0;

	for(int i=0; i<M; i++) {
		res += matA[num->i][i] * matB[i][num->j];
	}

	matC[num->i][num->j] = res;
	pthread_exit(0);
}

int main() {
	int count = 0;
	key_t key = 1500;
	int *share, l = 0;

	int sizemem = sizeof(*share);
	int shmid = shmget(key, 20, IPC_CREAT | 0666);

	share = (int *)shmat(shmid, 0, 0);
	// user input matriks a
	clear();
	for(int i=0; i<N; i++) {
		for(int j=0; j<M; j++) {
			printf("Input matriks A [%d][%d] :", i, j);
            scanf("%d", &matA[i][j]);
			clear();
		}
	}
	// user input matriks b
	for(int i=0; i<M; i++) {
		for(int j=0; j<K; j++) {
			printf("Input matriks B [%d][%d] :", i, j);
            scanf("%d", &matB[i][j]);
			clear();
		}
	}
	// print matriks a
	printf("\nMatriks A :\n");
	for(int i=0; i<N; i++) {
		for(int j=0; j<M; j++) {
            printf("%d\t", matA[i][j]);
		}
        printf("\n");
	}
	// print matriks b
	printf("\nMatriks B :\n");
	for(int i=0; i<M; i++) {
		for(int j=0; j<K; j++) {
            printf("%d\t", matB[i][j]);
		}
        printf("\n");
	}
	// memanggil fungsi perkalian
	pthread_t threads[MAX_THREADS];
	for(int i=0; i<N; i++) {
		for(int j=0; j<K; j++) {
			struct value *num = (struct value *)malloc(sizeof(struct value));
			num->i = i;
			num->j = j;	
			pthread_create(&threads[count], NULL, kali, num);
			pthread_join(threads[count], NULL);
			count++;
		}
	}
	for(int i=0; i<MAX_THREADS; i++) {
		pthread_join(threads[i], NULL);
	}
	// print hasil perkalian
	printf("\nMatriks hasil perkalian :\n");
	for(int i=0; i<N; i++) {
		for(int j=0; j<K; j++) {
			share[l] = matC[i][j];
			l++;
			printf("%d\t", matC[i][j]);

		}
		printf("\n");
	}
	shmdt((void *) share);
	return 0;
}