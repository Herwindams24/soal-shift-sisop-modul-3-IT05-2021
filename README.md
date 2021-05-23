# Shift 3 SISOP 2021 - IT05
Penyelesaian Soal Shift 2 Sistem Operasi 2021\
Kelompok IT05
  * Herwinda Marwaa Salsabila (05311840000009)
  * Stefanus Lionel Carlo Nugroho (05311940000027)
  * I Gde Ardha Semaranatha Gunasatwika (05311940000034)

---

## Table of Contents

* [Soal 1](#soal-1)
* [Soal 2](#soal-2)
* [Soal 3](#soal-3)
* [Kendala](#Kendala)

---
## Soal 1
Source Code : [soal1server.c](https://github.com/Herwindams24/soal-shift-sisop-modul-3-IT05-2021/blob/main/soal1/Server/server.c)\
Source Code : [soal1client.c](https://github.com/Herwindams24/soal-shift-sisop-modul-3-IT05-2021/blob/main/soal1/Client/client.c)

### Deskripsi Soal
Pada soal, penulis diminta untuk membuat sebuah program server database buku. Program dapat melakukan register, login, serta dapat menerima multi connections. Ketika koneksi terjadi antara server dan client side, maka program akan membuat sebuah file bernama akun.txt yang akan menjadi tempat menginputkan id dan password ketika didaftarkan ke dalam program dengan format id:password.

Selanjutnya sistem akan membuat file yang bernama files.tsv yang akan berisikan path files saat berada dalam server, publisher, serta tahun publikasi. Kemudian sistem membuat folder bernama FILES yang akan memuat file-file yang terbentuk dalam program.

Program yang dibuat dapat melakukan perintah add untuk menambahkan data buku dalam server, download untuk mendownload buku, delete untuk menghapus data, see untuk melihat semua isi files.tsv, serta find untuk dapat melakukan pencarian dengan memberikan parameter string.

Sistem juga membuat suatu log untuk server yang bernama running.log yang dpat menambah 

Source Code : [soal1server.c](https://github.com/Herwindams24/soal-shift-sisop-modul-3-IT05-2021/blob/main/soal1/Server/server.c)\
Source Code : [soal1client.c](https://github.com/Herwindams24/soal-shift-sisop-modul-3-IT05-2021/blob/main/soal1/Client/client.c)

### Pembahasan (Server Side)
Berikut merupakan library-library yang penulis gunakan:

``` c
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
```

* `<sys/stat.h>` - Library untuk memanggil fungsi pembuatan sebuah direktori baru (e.g. `mkdir()`)
* `<stdio.h>` - Library untuk fungsi input-output (e.g. `printf(), sprintf()`)
* `<sys/socket.h>` - Library untuk memanggil fungsi pemanfaatan socket
* `<stdlib.h>` - Library untuk fungsi umum (e.g. `exit()`, `atoi()`)
* `<netinet/in.h>` - Library untuk memanggil fungsi pemanfaatan internet
* `<unistd.h>` - Library untuk mendapatkan lokasi current working direktory (e.g. `getcwd()`)
* `<string.h>` - Libraryy untuk manipulasi string
* `<pthread.h>` - Library untuk operasi thread (e.g. `pthread_create()`, `ptrhead_exit()` )

Pertama, penulis membuat inisialisasi struct user dengan struktur username bertipekan data char dan juga password bertipekan data char. Struct selanjutnya yang diinisialisasi adalah clientdata dengan struktur client bertipekan data integer dan juga skiplogin bertipekan data integer. Juga dilakukan inisialisasi struct bookdata dengan sourcecode sebagai berikut:

``` c
typedef struct user {
	char username[200];
	char password[200];
}user;

typedef struct clientdata {
	int client;
	int skiplogin;
}clientdata;
```
Selanjutnya penulis menyatakan fungsi serve yang akan digunakan dalam registrasi dan juga login ketika program awal dijalankan. Dalam hal registrasi dilakukan pada fungsi berikut:

``` c
else if (mode == 1) { // REGISTER
	accountwrite = 1;
	user newacc;
	strcpy(newacc.username, username);
	strcpy(newacc.password, password);
	accounts[accountn] = newacc;
	while (filewrite);
	filewrite = 1;
	FILE *f = fopen("akun.txt", "a");
	fprintf(f, "%s:%s\n", username, password);
	fclose(f);
	filewrite = 0;
	accountn++;
	accountwrite = 0;
	for (int i = 0; i < accountn; i++) {
	printf("%s\t%s\n", accounts[i].username, accounts[i].password);
}
```
Sementara dalam hal melakukan login menggunakan fungsi berikut:

``` c
if (mode == 0) { // LOGIN
	int success = 0;
	while (accountwrite);
	for (int i = 0; i < accountn; i++) {
		if (strcmp(username, accounts[i].username) == 0 && strcmp(password, accounts[i].password) == 0) {
			success = 1;
			break;
		}
	}
	send(client, &success, sizeof(success), 0);
	if (success) printf("Auth success\n");
	else {
		printf("Auth failed\n");
		goto screen1;
	}
```
Berikutnya setelah berhasil melakukan login maka akan melakukan inisialisasi koneksi dengan perintah sebagai berikut:

``` c
if (mode == 0) { // FIND MATCH
	while (connectedwrite);
	connectedwrite = 1;
	connected++;
	connectedwrite = 0;
	while (connected < 2);
```
Sementara jika user ingin melakukan logout maka akan menjalankan perintah sebagai berikut:

``` c
else if (mode == 5) { // LOGOUT
		goto screen1;
	}
	pthread_exit(0);
```

Saat berhasil melakukan inisialisasi koneksi, akan dilakukan pembuatan file akun.txt untuk menyimpan data user ke dalam server dengan menggunakan fungsi berikut pada fungsi main:
``` c
FILE* f = fopen("akun.txt", "a");
char username[200], password[200];
int index = 0;
while (fscanf(f, "%[^\t]\t%[^\n]\n", username, password) != EOF) {
	user newacc;
	strcpy(newacc.username, username);
	strcpy(newacc.password, password);
	accounts[index] = newacc;
	index++;
}
fclose(f);
```

Sistem juga membuat folder dengan nama files yang memuat file-file yang akan terbentuk dalam penjalanan program dengan menggunakan fungsi sebagai berikut:
```c
mkdir("FILES", 0777);
	FILE* fs = fopen("files.tsv", "a");
int indexfs = 0;
fclose(fs);
```

### Pembahasan (Client Side)
Berikut merupakan library-library yang penulis gunakan:

``` c
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h> 
#include <termios.h> 
```

* `<sys/stat.h>` - Library untuk memanggil fungsi pembuatan sebuah direktori baru (e.g. `mkdir()`)
* `<stdio.h>` - Library untuk fungsi input-output (e.g. `printf(), sprintf()`)
* `<sys/socket.h>` - Library untuk memanggil fungsi pemanfaatan socket
* `<stdlib.h>` - Library untuk fungsi umum (e.g. `exit()`, `atoi()`)
* `<netinet/in.h>` - Library untuk memanggil fungsi pemanfaatan internet
* `<unistd.h>` - Library untuk mendapatkan lokasi current working direktory (e.g. `getcwd()`)
* `<string.h>` - Libraryy untuk manipulasi string
* `<pthread.h>` - Library untuk operasi thread (e.g. `pthread_create()`, `ptrhead_exit()` )
* `<arpa/inet.h>` - Library untuk memanggil fungsi pemanfaatan serta manipulasi fungsi host menggunakan internet
* `<termios.h>` - Library untuk menjalankan fungsi dalam terminal

Dalam fungsi main, pertama-tama dilakukan inisialisasi socket dan juga struct socket untuk menginisialisasi socket yang akan digunakan untuk menjalankan program.
```c
 int sock;
    struct sockaddr_in address;
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\n Socket creation error \n");
        exit(EXIT_FAILURE);
    }
```

Selanjutnya dilakukan inisialisasi koneksi dari client side dengan server side dan dilakukan deklarasi if condition jika connection gagal dilakukan sebagai berikut:
```c
 if (connect(sock, (struct sockaddr *)&address, sizeof(address)) < 0) {
        printf("\nConnection Failed \n");
        exit(EXIT_FAILURE);
    }
```
Ketika koneksi sudah terbentuk maka dilakukan penampilan screen 1 yang menampilkan pilihan untuk melakukan login atau register, kemudian mengirimkan input respon yang dikirimkan user kepada server side, sebagai berikut:
```c
    screen1:;
    int mode = -1;
    while (mode == -1) {
        char input[20];
        printf("1. Login\n2. Register\nChoices: ");
        scanf("%s", input);
        if (strcmp(input, "login") == 0) {
            mode = 0;
            break;
        } else if (strcmp(input, "register") == 0) {
            mode = 1;
            break;
        }
        printf("\nInvalid input.\n");
    }
```

Selanjutnya, terdapat inisialiasi variabel username dan password yang nantinya akan digunakan untuk menjalankan fungsi login dan juga register, sebagai berikut:
```c
    char username[210], password[200];
    memset(username, 0, sizeof(username));
    memset(password, 0, sizeof(password));
    username[0] = mode + '0';
    username[1] = '\t';
    password[0] = '\t';
    printf("Username: ");
    getchar();
    scanf("%[^\n]", username+2);
    getchar();
    printf("Password: ");
    scanf("%[^\n]", password+1);
    strcat(username, password);
    send(sock, username, strlen(username), 0);
```

Berikutnya terdapat respond yang diberikan ketika berhasil melakukan login yang akan menampilkan “login success” ataupun gagal yang akan menampilkan pesan “login failed”.
```c
   if (mode == 0) {// LOGIN
        int resp;
        read(sock, &resp, sizeof(resp));
        if (resp) printf("login success\n");
        else {
            printf("login failed\n");
            goto screen1;
        }
```

Sementara dalam menjalankan register maka dari client side akan menampilkan respond “register succesfull” ketika berhasil mendaftarkan user ke dalam server. Adapun fungsi yang digunakan sebagai berikut:
```c
else if (mode == 1) { // REGISTER
        printf("register success\n");
    }
```

Saat sudah berhasil melakukan login, dari client side akan menampilkan pilihan fitur-fitur yang dapat dilakukan dalam program yang terdiri dari fitur: add, download, delete, see, find, serta logout. Adapun fungsi yang digunakan dalam menampilkan fitur adalah sebagai berikut:
```c
screen2:;
    mode = -1;
    while (mode == -1) {
        printf("1. add\n2. download\n3. delete\n4. see\n5. find\n6. logout\nChoices: ");
        char input[20];
        scanf("%s", input);
        if (strcmp(input, "add") == 0) {
            mode = 0;
            break;
        } else if (strcmp(input, "download") == 0) {
            mode = 1;
            break;
        } else if (strcmp(input, "delete") == 0) {
            mode = 2;
            break;
        } else if (strcmp(input, "see") == 0) {
            mode = 3;
            break;
        } else if (strcmp(input, "find") == 0) {
            mode = 4;
            break;
        } else if (strcmp(input, "logout") == 0) {
            mode = 5;
            break;
        }
        printf("\nInvalid input.\n");
    }
```
Jika user memilih opsi logout, maka client akan kembali menampilkan tampilan awal yang menampilkan opsi login atau register dengan menggunakan perintah berikut:
```c
else if (mode == 5) { // LOGOUT
        goto screen1;
    }
```
**DOKUMENTASI**


--
## Soal 2
Source Code : [soal2a.c](https://github.com/Herwindams24/soal-shift-sisop-modul-3-IT05-2021/blob/main/soal2/soal2a.c)\
Source Code : [soal2b.c](https://github.com/Herwindams24/soal-shift-sisop-modul-3-IT05-2021/blob/main/soal2/soal2b.c)\
Source Code : [soal2c.c](https://github.com/Herwindams24/soal-shift-sisop-modul-3-IT05-2021/blob/main/soal2/soal2c.c)

### Deskripsi Soal
Pada soal no 2, pertanyaan dipecah menjadi 3 buah soal diantaranya,

- (2a) :
  - Membuat program perkalian matrix (4x3 dengan 3x6) dan menampilkan hasilnya
  - Matriks nantinya akan berisi angka 1-20 (tidak perlu dibuat filter angka)
  - matriks berasal dari input ke program
- (2b) :
  - Membuat program dengan menggunakan matriks output dari program sebelumnya (program soal2a.c)
  - matriks tersebut akan dilakukan perhitungan dengan matrix baru (input user), dengan ketentuan sesuai yang diberikan soal
- (2c) :
  - membuat program (soal2c.c) untuk mengecek 5 proses teratas apa saja yang memakan resource komputer
  - menggunakan command “ps aux | sort -nrk 3,3 | head -5” 
  - Catatan!: Harus menggunakan IPC Pipes

### Pembahasan 2a

Berikut merupakan library-library yang penulis gunakan:

``` c
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/ipc.h> 
#include <sys/shm.h> 
#include <unistd.h>
```

* `<stdio.h>` - Library untuk fungsi input-output (e.g. `printf(), sprintf()`)
* `<stdlib.h>` - Library untuk fungsi umum (e.g. `exit()`, `atoi()`)
* `<pthread.h>` - Library untuk operasi thread (e.g. `pthread_create()`, `ptrhead_exit()` )
* `<sys/ipc.h>` - Library untuk membantu melakukan operasi shared memory (e.g. `IPC CREAT`)
* `<sys/shm.h>` - Library untuk dapat melakukan operasi shared memory (e.g. `schmdt()`)
* `<unistd.h>` - Library untuk mendapatkan lokasi current working direktory (e.g. `getcwd()`)

Pertama, penulis melakukan pendefinisian ukuran matriks dengan menggunakan `#define`

Tiap-tiap matriks direpresentasikan sebagai variable global menggunakan array 2 dimensi bernama matA dan matB, sedangkan matC merupakan matriks hasil perkalian.

``` c
#define N 4
#define M 3
#define K 6
#define MAX_THREADS N * K
#define clear() printf("\033[H\033[J")

int matA[N][M];
int matB[M][K];
int matC[N][K];
```

Kemudian dibuat sebuah struct bernama value yang berisi baris dan kolom dari matriks yang digunakan untuk mem-passing data ke threads.

``` c
struct value {
	int i, j;
};
```

Kemudian dibuat sebuah fungsi thread `kali` yang melakukan operasi perkalian pada elemen-elemen matriks.

``` c
void *kali(void *arg) {
	struct value *num = arg;
	int res = 0;

	for(int i=0; i<M; i++) {
		res += matA[num->i][i] * matB[i][num->j];
	}

	matC[num->i][num->j] = res;
	pthread_exit(0);
}
```

`struct value *data =arg;` digunakan untuk melakukan casting parameter ke pointer struct. Lalu, hasil perkalian di inisiasi dengan `int res = 0;`. Kemudian hasil perkalian tersebut dimasukkan sebagai elemen dari matC pada indeks `i`dan `j`

Dalam fungsi main, agar matriks hasil dapat diakses oleh soal2b, maka digunakan shared memory.

Untuk itu, digunakan pendeklarasian unique key `key_t key = 1500;` dan juga proses `write` array ke dalam shared memory

``` c
key_t key = 1500;
	int *share, l = 0;

	int sizemem = sizeof(*share);
	int shmid = shmget(key, 20, IPC_CREAT | 0666);

	share = (int *)shmat(shmid, 0, 0);
```

Pada variable shmid disimpan sebuah identifier untuk segmen shared memory yang didapat dari fungsi `shmget(key, 20, IPC_CREAT | 0666)`. Agar bisa digunakan, maka segmen shared memory perlu di attach ke alamat dari proses dengan menggunakan fungsi `shmat(shmid, 0, 0)`.

lalu selanjutnya, dibuat fungsi input untuk `matA` dan `matB` dengan melakukan for loop pada indeks `i` dan `j`

``` c
	for(int i=0; i<N; i++) {
		for(int j=0; j<M; j++) {
			printf("Input matriks A [%d][%d] :", i, j);
            scanf("%d", &matA[i][j]);
			clear();
		}
	}
```

``` c
	for(int i=0; i<M; i++) {
		for(int j=0; j<K; j++) {
			printf("Input matriks B [%d][%d] :", i, j);
            scanf("%d", &matB[i][j]);
			clear();
		}
	}
```

Setelah selesai diinput, kemudian selanjutnya `matA` dan `matB` akan diprint sehingga dapat dilihat oleh user

``` c
	printf("\nMatriks A :\n");
	for(int i=0; i<N; i++) {
		for(int j=0; j<M; j++) {
            printf("%d\t", matA[i][j]);
		}
        printf("\n");
	}
```

``` c
	printf("\nMatriks B :\n");
	for(int i=0; i<M; i++) {
		for(int j=0; j<K; j++) {
            printf("%d\t", matB[i][j]);
		}
        printf("\n");
	}
```

Dalam fungsi main, dilakukan deklarasi variable thread sebanyak MAX_THREADS yang telah di declare yaitu bernilai N * K = 24 yang merupakan perkalian jumlah baris matA dengan jumlah kolom matB.

``` c
pthread_t threads[MAX_THREADS];
```

Kemudian dilakukan loop untuk pembuatan threads

``` c
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
```

Didalam loop terdapat fungsi `pthread_create` untuk membuat thread dan mem-passing data sebagai parameter. Serta `pthread_join` untuk menunggu thread selesai.

Kemudian dibuat loop untuk menampilkan hasil dari perkalian yaitu elemen-elemen matriks `matC`

``` c
	printf("\nMatriks hasil perkalian :\n");
	for(int i=0; i<N; i++) {
		for(int j=0; j<K; j++) {
			share[l] = matC[i][j];
			l++;
			printf("%d\t", matC[i][j]);

		}
		printf("\n");
	}
```

### Pembahasan 2b

Berikut merupakan library-library yang penulis gunakan:

``` c
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/ipc.h> 
#include <sys/shm.h> 
#include <unistd.h>
```

* `<stdio.h>` - Library untuk fungsi input-output (e.g. `printf(), sprintf()`)
* `<stdlib.h>` - Library untuk fungsi umum (e.g. `exit()`, `atoi()`)
* `<pthread.h>` - Library untuk operasi thread (e.g. `pthread_create()`, `ptrhead_exit()` )
* `<sys/ipc.h>` - Library untuk membantu melakukan operasi shared memory (e.g. `IPC CREAT`)
* `<sys/shm.h>` - Library untuk dapat melakukan operasi shared memory (e.g. `schmdt()`)
* `<unistd.h>` - Library untuk mendapatkan lokasi current working direktory (e.g. `getcwd()`)

Pertama, penulis melakukan pendefinisian ukuran matriks dengan menggunakan `#define`

Tiap-tiap matriks direpresentasikan sebagai variable global menggunakan array 2 dimensi. `matC` adalah hasil perkalian pada soal 2a, `matD` adalah matriks yang akan diinputkan user sebagai pembanding `matC`, dan `matE` adalah matriks yang akan menyimpan hasil perrrhitungan.

Lalu, dilakukan inisialisasi dengan nilai `0` pada `matE`

``` c
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
```

Kemudian penulis membuat fungsi faktorial dengan menggunakan rekursif, fungsi ini akan membantu perhitungan faktorial pada saat pembandingan nilai matriks nanti.

``` c
unsigned long long faktorial(unsigned long long num) {
  unsigned long long res;
  if(num <= 1){
    return 1;
  }
  res = num * faktorial(num-1);
  return res;
}
```

Lalu selanjutnya, dibuat fungsi `comp` atau compare yang berfungsi membandingkan nilai elemen antar `matC` dengan `matD`. Setelah itu, akan dilakukan perhitungan dengan bantuan fungsi `faktorial()` dan hasilnya disimpan di dalam `res`. Nilai `res` akan dimasukkan kedalam matriks hasil `matE` sesuai indeks `i` dan `j`.

``` c
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
```

Setelah itu, user akan diminta untuk melakukan input matriks baru `matD`. User akan diminta untuk menginput ukuran matriks serta tiap elemen dari matriks. Program dibuat dengan melakukan for loop sesuai ukuran matriks yang diinginkan user.

``` c
	printf("Input dimensi matriks baru :");
  scanf("%d %d", &x, &y);
	clear();
	for(int i=0; i<x; i++) {
		for(int j=0; j<y; j++) {
			printf("Input matriks baru [%d][%d] :", i, j);
            scanf("%d", &matD[i][j]);
			clear();
		}
	}
```

Kemudian pada soal 2b, dilakukan bagian me-read data dari shared memory yang telah di write oleh program 2a. Prosesnya kurang lebih sama dengan proses write, perbedaannya adalah tidak ada pengalokasian memori pada proses read. `matC` dari soal 2a akan dibaca lalu disimpan secara lokal dan sekaligus diprint sehingga dapat dilihat oleh user.

Kemudian program melakukan detachment ketika telah selesai melakukan segmen shared memory dengan menggunakan fungsi `shmdt((void *) share);`.

``` c
key_t key = 1500;

...

int shmid = shmget(key, 20, IPC_CREAT | 0666);
share = (int *)shmat(shmid, 0, 0);

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
```

Selanjutnya, program akan melakukan print pada matriks inputan `matD` yang telah diinputkan oleh user.

``` c
printf("\nMatriks dari input user :\n");
	for(int i=0; i<N; i++) {
		for(int j=0; j<K; j++) {
      printf("%d\t", matD[i][j]);
		}
      printf("\n");
	}
```

Lalu, dilakukan deklarasi variable thread sebanyak MAX_THREADS yang telah di declare yaitu bernilai N * K = 24 yang merupakan ukuran dari `matC`.

``` c
pthread_t threads[MAX_THREADS];
```

Kemudian dilakukan loop untuk pembuatan threads

``` c
	for(int i=0; i<MAX_THREADS; i++) {
		pthread_create(&threads[i], NULL, comp, NULL);
		pthread_join(threads[i], NULL);
	}
```

Didalam loop terdapat fungsi `pthread_create` untuk membuat thread dan mem-passing data sebagai parameter. Serta `pthread_join` untuk menunggu thread selesai.

Diakhir, dibuat loop untuk menampilkan matriks hasil perhitungan `matE`

``` c
    for(int i=0; i<N; i++) {
		  for(int j=0; j<K; j++) {
			  printf("%llu\t", matE[i][j]);
		}
		printf("\n");
	}
```

Kemudian untuk menunggu thread selesai

``` c
	for(int i=0; i<MAX_THREADS; i++) {
		pthread_join(threads[i], NULL);
	}
```

### Pembahasan 2c

Berikut merupakan library-library yang penulis gunakan:

``` c
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
```

* `<stdlib.h>` - Library untuk fungsi umum (e.g. `exit()`, `atoi()`)
* `<unistd.h>` - Library untuk mendapatkan lokasi current working direktory (e.g. `getcwd()`)
* `<stdio.h>` - Library untuk fungsi input-output (e.g. `printf(), sprintf()`)
* `<sys/types.h>` - Library yang digunakan untuk identifikasi sebuah thread (e.g `pthreadid_t`)
* `<sys/wait.h>` - Library yang digunakan untuk identifikasi sebuah thread (e.g `pthreadid_t`)

pertama penulis membuat `#define die()` untuk digunakan pada pipe nantinya

``` c
#define die(e) do { fprintf(stderr, "%s\n", e); exit(EXIT_FAILURE); } while (0);
```

selanjutnya, dilakukan pendefinisian `int main()` dan pembuatan pipe. Apabila pipe bernilai -1, maka akan dilakukan `die()`

``` c
int main(){
	int status;
	int fd1[2];
  int fd2[2];

  if(pipe(fd1) == -1) {
    die("pipe");
	}
	
	if(pipe(fd2) == -1) {
		die("pipe");
	}
}
```

karena terdapat 2 buah pipe, maka dilakukan 2 kali fork untuk membuat 2 parent dan 2 child. Parent pertama akan melakukan command `ps aux`. Child pertama akan melakukan forking, Parent kedua akan melakukan command `sort -nrk 3,3`, dan Child kedua akan melakukan command `head -5`.

``` c
	if(fork() == 0) {
		close(fd1[0]);
		dup2(fd1[1], STDOUT_FILENO); //write end dari pipe fd1
		char *argv[] = {"ps", "aux", NULL};
		execv("/bin/ps", argv);
	} else {
		while((wait(&status)) > 0);

        if(fork() == 0) {
          close(fd1[1]);
          dup2(fd1[0], STDIN_FILENO); //read end dari pipe fd1
          close(fd2[0]);
          dup2(fd2[1], STDOUT_FILENO); //write end dari pipe fd2
          char *argv[] = {"sort", "-nrk", "3,3", NULL};
          execv("/bin/sort", argv);
        }else {
          close(fd1[1]);
		      close(fd2[1]);
          while((wait(&status)) > 0);
          dup2(fd2[0],STDIN_FILENO); //read end dari pipe fd2
          char *argv[] = {"head", "-5", NULL};
		      execv("/bin/head", argv);
        }
	}
```

**DOKUMENTASI**

### Screenshot Soal 2a
![image](https://raw.githubusercontent.com/Herwindams24/soal-shift-sisop-modul-3-IT05-2021/main/Gambar/2a.png)

### Screenshot Soal 2a
![image](https://raw.githubusercontent.com/Herwindams24/soal-shift-sisop-modul-3-IT05-2021/main/Gambar/2b.png)

### Screenshot Soal 2a
![image](https://raw.githubusercontent.com/Herwindams24/soal-shift-sisop-modul-3-IT05-2021/main/Gambar/2c.png)

--
## Soal 3
Source Code : [soal3.c](https://github.com/Herwindams24/soal-shift-sisop-modul-3-IT05-2021/blob/main/soal3/soal3.c)

### Deskripsi Soal
Pada soal, penulis diminta untuk membuat sebuah program mengkategorikan file menggunakan bahasa C. Di mana pada program ini terdapat thread. Program ini dapat menerima 3 jenis atau opsi argumen yang diinputkan dan memindahkan file ke dalam folder sesuai dengan ekstensi file tersebut. Selanjutnya, folder hasil akan dibuat di current working directory ketika program kategori tersebut dijalankan. 
Terdapat 3 opsi arguman yang dapat di inputkan yaitu (-f), (*) dan (-d). Dengan ketentuan sebagai berikut:

- (-f) :
  - Pengguna bisa menambahkan argumen file yang dapat dikategorikan sebanyak yang diinginkan. 
  - Jika terdapat file yang tidak memiliki ekstensi, file disimpan dalam folder “Unknown”. 
  - Jika file hidden, masuk folder “Hidden”.
  - Program akan mencetak ``Berhasil Dikategorikan`` (jika berhasil) dan ``Sad, gagal :(`` (jika gagal).

- (-d) :
  - Pengguna hanya dapat menginputkan 1 directory
  - Perintah di atas akan mengkategorikan file di /path/to/directory 
  - Folder hasil akan disimpan di working directory dimana program C tersebut berjalan (hasil kategori filenya tidak di /path/to/directory).
  - Program akan mencetak ``Direktori sukses disimpan!`` (jika berhasil) dan ``Yah, gagal disimpan :(`` (jika gagal).
  - Berlaku hal yang sama pada file ``hidden`` dan file ``tanpa ekstensi``

- (\*) :
  - Mengkategorikan seluruh file yang ada di working directory
  - Berlaku hal yang sama pada file ``hidden`` dan file ``tanpa ekstensi``


### Pembahasan

Berikut merupakan library-library yang penulis gunakan:

``` c
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <dirent.h>
#include <pthread.h>
#include <errno.h>
```

* `<sys/types.h>` - Library yang digunakan untuk identifikasi sebuah thread (e.g `pthreadid_t`)
* `<sys/stat.h>` - Library untuk memanggil fungsi pembuatan sebuah direktori baru (e.g. `mkdir()`)
* `<stdio.h>` - Library untuk fungsi input-output (e.g. `printf(), sprintf()`)
* `<stdlib.h>` - Library untuk fungsi umum (e.g. `exit()`, `atoi()`)
* `<unistd.h>` - Library untuk mendapatkan lokasi current working direktory (e.g. `getcwd()`)
* `<string.h>` - Libraryy untuk manipulasi string
* `<ctype.h>` - Library untuk pengkonversian ke lower case (e.g. `tolower_()`)
* `<dirent.h>` - Library untuk merepresentasikan directory stream & struct dirent(e.g. `struct dirent *entry`)
* `<pthread.h>` - Library untuk operasi thread (e.g. `pthread_create()`, `ptrhead_exit()` )
* `<errno.h>` - Library untuk error handling (e.g. `errno`)

Pertama, penilis membuat 3 fungsi dan 1 routine(thread) untuk program ini yaitu: `get_NamaFile`, `cek_Ext`, `dirChecking` dan `move`.

**Fungsi _get_NamaFile_**

```c
char *get_NamaFile(char *fName, char buffer[])
{
  char *token = strtok(fName, "/");
  while (token != NULL)
  {
    sprintf(buffer, "%s", token);
    token = strtok(NULL, "/");
  }
}
```
- Fungsi `get_NamaFile` bertipe data `char` ini didefinisikan menggunakan dua parameter yaitu `*fname` dan `buffer[]` untuk menyimpan hasil dari fungsi ini sendiri, dan akan mengembalikan nilai nama file beserta ekstensinya.
- fungsi `strtok()` mengambil nama file dengan mengurai string dengan dengan delimiter dan akan disimpan di dalam variabel `*token`.
- Lalu while loop akan berjalan selama token belum habis atau tidak sama dengan NULL 
- Nama file yang telah diambil akan di print kedalam `buffer` menggunakan `sprintf`.
- Fungsi `strtok()` akan dijalankan lagi dengan parameter pertama = NULL untuk mencari token selanjutnya hingga akhir dari input.

**Fungsi _cek_Ext_**
```c
char *cek_Ext(char *fName, char buffer[])
{
char bufferNamaFile[1000];
char *token = strtok(fName, "/");

for (token != NULL; token++;)
 {
   sprintf(bufferNamaFile, "%s", token);
   token = strtok(NULL, "/");
 }
```
- Fungsi `*cek_Ext` bertipe data `char` memiliki dua parameter yaitu `*fname` sebagai pointer dan `buffer[]` untuk menyimpan hasil dari fungsi dan akan mengembalikan nilai ekstensi dari sebuah file.
- Selanjutnya Fungsi akan melakukan hal yang sama persis seperti pada fungsi `get_NamaFile_`, namun pada kasus ini menggunakan `for()` loop, dan menghasilkan nama file beserta ekstensinya.
```c
int count = 0;
token = strtok(bufferNamaFile, ".");
while (token != NULL)
 {
   count++;
   sprintf(buffer, "%s", token);
   token = strtok(NULL, ".");
  }
```
- Disini NamaFile yang masih beserta ekstensinya akan duraikan kembali menggunakan fungsi strtok() dengan delimiter `.` sebagai pemisah antara nama file dengan ekstensi 
- Hasilnya akan disimpan kedalam `token*`
- Karna yang akan pertama di return oleh fungsi strtok() adalah kata pertama seblum delimiter, maka while loop akan berjalan selama token belum habis 
- Print ekstensi yang sudah didapat ke dalam `buffer`.
```c
if (count <= 1)
{
  strcpy(buffer, "unknown");
}
return buffer;
```
- Terdapat `if()` statement yang berfungsi untuk pengecekan untuk jumlah counter yang kurang dari 1 atau kondisi di mana file tidak ada ekstensi atau file memiliki karakter "." diawal maupun diakhir string.
- Untuk file yang tidak memiliki ekstensi, buffer akan berisi unknown.

**Fungsi _dirChecking_**

```c
void dirChecking(char buffer[])
{
 DIR *dr = opendir(buffer);
 if (ENOENT == errno)
 {
   mkdir(buffer, 0700);
   closedir(dr);
 }
}
```
* Fungsi berparameter yaitu `buffer[]` yang berfungsi untuk menyimpan hasil eksekusi dari fungsi ini sendiri
* Fungsi ini berfungsi untuk membuat directory baru menggunakan fungsi `mkdir()`
* Disini pembuatan directory baru akan dilakukan jika ada sebuah error yang dihasilakan oleh fungsi `opendir()` 
* `if` statement akan melakukan error handling.
* Fungsi `mkdir()` membuat directory dengan nama yang di return oleh parameter buffer dan memiliki permission [0700](http://www.filepermissions.com/file-permission/0700).
* Selanjutnya, tutup directory kembali.

**Fungsi _move_**
```c
char bufferExt[100], bufferNamaFile[1000], bufferFrom[1000], bufferTo[1000], cwd[1000];
getcwd(cwd, sizeof(cwd));
strcpy(bufferFrom, (char *)arg);
```
- Pada fungsi _move_ ini, Penulis mendefinisikan 5 buffer:
  - `bufferExt` menghandle ext
  - `bufferNamaFile` menghandle NamaFile
  - `bufferFrom` menghandle path input
  - `bufferTo` menghandle path to
  - `cwd` menghandle cwd 
- Buffer untuk cwd akan langsung diisi dengan fungsi `getcwd()` yang mereturn current working directory 
- Argumen path yang diinpukan user akan diambil menggunakan `*(char )arg`

```c
DIR *dir = opendir(bufferFrom);
    if (dir)
    {
        printf("Sad, gagal :(\n", bufferFrom);
        pthread_exit(0);
    }
    closedir(dir);
```
- Pengecekan bentuk file yang diinputkan adalah dengan menggunakan dir yang dimana jika terbuka sebagai directory maka akan ditampilkan error message `"Sad, gagal :(\n"` dan thread akan diselesaikan menggunakan `pthread_exit(0)`

```c
get_NamaFile(bufferFrom, bufferNamaFile);
strcpy(bufferFrom, (char *)arg);
```
- Selanjutnya, panggil fungsi `get_NamaFile()` dengan filename yang akan masuk ke buffer baru `bufferNamaFile`.

```c
cek_Ext(bufferFrom, bufferExt);
int i = 0;
while (i < sizeof(bufferExt))
{
  bufferExt[i] = tolower(bufferExt[i]); // convert it to a lowercase  using tolower
  i++;
}
```
- Panggil fungsi cek_Ext() untuk mengambil setiap extension dari filename yang ada di bufferFrom 
- Ubah tiap extension dari file yang ada menjadi lowercase menggunakan `while()` loop.
- Pada `while()` loop terdapat fungsi `tolower()` yang berfungsi untuk mengubah extension file menjadi huruf kecil dan `i` sebagai counternya.

```c
strcpy(bufferFrom, (char *)arg);
dirChecking(bufferExt);
sprintf(bufferTo, "%s/%s/%s", cwd, bufferExt, bufferNamaFile);
rename(bufferFrom, bufferTo);
printf("Berhasil Dikategorikan\n", bufferFrom);
pthread_exit(0);
```
- Selanjutnya panggil fungsi `dirChecking()` untuk membuat directory baru pada setiap ekstensi didalam `bufferExt` yang belum memiliki directory
- Lalu `bufferTo` akan diisi dengan value `string` setiap buffer yang sudah di set sebelumnya dengan urutan `cwd`,`bufferExt` dan `bufferNamaFile` menggunakan `sprintf()`. 
- File name yang ada di buffFrom akan di `rename()` dengan urutan dari `bufferTo`.
- Setelah semua proses berhasil berjalan print `"Berhasil Dikategorikan\n"`.
- Lalu akhiri fungsi dengan `pthread_exit(0)`.

**Fungsi _main_**
```c
if (strcmp(argv[1], "-f") == 0)
    {
        pthread_t threadid[argc - 2]; //inisialisasi thread

        for (int i = 2; i < argc; i++)
        {
            pthread_create(&threadid[i - 2], NULL, &move, (void *)argv[i]);
        }

        for (int i = 2; i < argc; i++)
        {
            pthread_join(threadid[i - 2], NULL);
        }
        exit(0);
    }
```
- Potongan program ini berfungsi untuk mengecek banyak argumen jika yang diinputkan adalah `-f` sekaligus membuat thread dengan men-set `threadid` bernilai `jumlah_argumen - 2` 
- `for()` loop akan berjalan sebanyak `i` kali, dimana `i` adalah jumlah argumen.
- `for()` loop akan membuat thread menggunakan `pthread_create()` untuk setiap argumennya dengan `threadid i - 2` dimana `i` tadi akan di increment setiap parameter `for()` terpenuhi
- Thread akan diajalankan dengan move kepada `(void *)argv[i])`
- Program akan men-join setiap thread yang sudah dibuat dengan `pthread_join()`.

```c
 char *directory;
    if (strcmp(argv[1], "*") == 0)
    {
        char buffer[1000];
        getcwd(buffer, sizeof(buffer));
        directory = buffer;
    }
```
- Potongan program ini berfungsi untuk mengecek banyak argumen jika yang diinputkan adalah *. 
- Jika argumen benar maka program akan menset `cwd` beserta sizenya menggunakan `getcwd()` kedalam buffer baru yang nantinya akan dimasukkan ke variabel directory
```c
if (strcmp(argv[1], "-d") == 0)
    {

        DIR *dir = opendir(argv[2]);
        if (dir)
        {
            directory = argv[2];
            printf("Direktori sukses disimpan!\n");
        }
        else if (ENOENT == errno)
        {
            printf("Yah, gagal disimpan :(\n");
            exit(1);
        }
        closedir(dir);
    }
```
- Potongan program ini berfungsi untuk mengecek banyak argumen jika yang diinputkan adalah -d. 
- Jika argumen yang diinputkan bukanlah sebuah directory, maka tampilkan error message `Yah, gagal disimpan:(` dan program akan ditutup dengan exit(1).
- Jika argumen benar, maka program akan membuka directory sesuai dengan argumen kedua yang menggunakan `opendir()` dan memasukan nama directory tersebut kedalam variabel `dir`.
- Contoh untuk kondisi else if() disini adalah file yang tidak memiliki ekstensi, jadi variabel dir berisi NULL dan directory tidak terbuka.
```c
int file_count = 0;
    DIR *dir = opendir(directory);
    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL)
    {
        if (entry->d_type == DT_REG)
        {
            file_count++;
        }
    }
    closedir(dir);
```
- Potongan program di atas berfungsi sebagai handler argumen -d dan *
- Pertama-tam buat file counter untuk setiap file yang ada dalam suatu directory
- Lalu, directory akan dibuka dengan `opendir()` dan di set ke dalam variabel dir untuk nantinya di cek
- `struct dirent *entry;` pendefinisian struct dirent untuk penggunaan fungsi `readdir()`
- Buat `while()` loop untuk pengecekan tiap filena di dalam `dir` yang sudah dibuka dengan menggunakan `entry->d_type `
- Untuk setiap file reguler yang ditemukan maka nilai counter akan di increment, `while()` akan berjalan sampai tiap file yang ada di dalam directory habis.
```c
pthread_t threadid[file_count];
char buffer[file_count][1337]; //simpan absoloute path
int iter = 0;
```
- Definisikan sebuah buffer untuk menyimpan absolut path dan satu variabel untuk iterasi
- Set threadid sejumlah nilai counter (banyak file reguler) di setiap directory untuk thread yang akan dibuat

```c
dir = opendir(directory);

    //pengecekan tiap file
    while ((entry = readdir(dir)) != NULL) //looping sampai file di dir = NULL
    {
        if (entry->d_type == DT_REG)
        {
            sprintf(buffer[iter], "%s/%s", directory, entry->d_name); //masukan absolut path dari setiap file
            iter++;
        }
    }

    closedir(dir);
```
- Buka directory dengan menggunakan fungsi `opendir()`.
- Setelah directory terbuka, lakukanlah pengecekan tiap file didalamnya.
- Lakukanlah while loop hingga tidak ada file lagi yang telah dibaca
- `sprintf(buff[iter], "%s/%s", directory, entry->d_name);` pada saat ini, penulis memasukan absolut path dari setiap file reguler itu sendiri, kondisi ini akan berjalan selama file dari sebuah directory belum habis sesuai parameter while loop.
- Size of buffer(`int iter`) akan di increment untuk setiap absolut path yang masuk ke dalam `buffer`.

```c
for (int i = 0; i < file_count; i++) //Looping sebanyak jumlah file reguler yang sudah tersimpan di buffer
    {
        char *test = (char *)buffer[i]; //Simpan terlebih dahulu absolut path
        printf("%s\n", test);
        pthread_create(&threadid[i], NULL, &move, (void *)test);
    }

    int i = 0;
    while (i < file_count)
    {
        pthread_join(threadid[i], NULL);
        i++;
    }
```

- `for() loop` disini akan berjalan sebanyak jumlah file reguler yang sudah tersimpan di buffer.
- `test` berfungsi uuntuk simpan absolut path dari setiap file sebelum dijalankan di dalam thread. Sehingga thread tidak langusng mengambil argumen keempatnya dari buffer
- Membuat `pthread_create(&threadid[i], NULL, &move, (void *)test)` dengan argumen keempat mengguanakan `test` yang telah menyimpan absolut path dari setiap file
- `While()` loop kedua akan men-join setiap thread yang sudah dibuat dan menunggu hingga thread yang diinginkan berstatus `Terminated`.

**DOKUMENTASI**


## Kendala
Soal:
 * Soal 1: - Kesulitan dalam melakukan input file baru dalam database, menghapus file, mendownload file, melihat isi file, melakukan pencarian file, serta mengenerate running.log untuk server.
 * Soal 2: - tipe data yang digunakan "unsigned long long" tidak dapat menampung nilai diatas 20!
 * Soal 3: - Kesulitan dalam memanipulasi string dan logika if-else
