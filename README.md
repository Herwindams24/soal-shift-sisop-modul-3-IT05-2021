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

--
## Soal 2
Source Code : [soal2a.c](https://github.com/Herwindams24/soal-shift-sisop-modul-3-IT05-2021/blob/main/soal2/soal2a.c)\
Source Code : [soal2b.c](https://github.com/Herwindams24/soal-shift-sisop-modul-3-IT05-2021/blob/main/soal2/soal2b.c)\
Source Code : [soal2c.c](https://github.com/Herwindams24/soal-shift-sisop-modul-3-IT05-2021/blob/main/soal2/soal2c.c)

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

**Fungsi _cek_Ext_**
```c
char bufferNamaFile[1000];
char *token = strtok(fName, "/");

for (token != NULL; token++;)
 {
   sprintf(bufferNamaFile, "%s", token);
   token = strtok(NULL, "/");
 }
```

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

```c
if (count < 1)
{
  strcpy(buffer, "unknown");
}
if (count = 1)
{
  strcpy(buffer, "hidden");
}
return buffer;
```

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


**Fungsi _move_**
```c
char bufferExt[100], bufferNamaFile[1000], bufferFrom[1000], bufferTo[1000], cwd[1000];
getcwd(cwd, sizeof(cwd));
strcpy(bufferFrom, (char *)arg);
```

```c
DIR *dir = opendir(bufferFrom);
    if (dir)
    {
        printf("Sad, gagal :(\n", bufferFrom);
        pthread_exit(0);
    }
    closedir(dir);
```

```c
get_NamaFile(bufferFrom, bufferNamaFile);
strcpy(bufferFrom, (char *)arg);

cek_Ext(bufferFrom, bufferExt);
```

```c
int i = 0;
while (i < sizeof(bufferExt))
{
  bufferExt[i] = tolower(bufferExt[i]); // convert it to a lowercase  using tolower
  i++;
}
```

```c
strcpy(bufferFrom, (char *)arg);
dirChecking(bufferExt);
sprintf(bufferTo, "%s/%s/%s", cwd, bufferExt, bufferNamaFile);
rename(bufferFrom, bufferTo);
printf("Berhasil Dikategorikan\n", bufferFrom);
pthread_exit(0);
```

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

```c
 char *directory;
    if (strcmp(argv[1], "*") == 0)
    {
        char buffer[1000];
        getcwd(buffer, sizeof(buffer));
        directory = buffer;
    }
```

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

```c
pthread_t threadid[file_count];
char buffer[file_count][1337]; //simpan absoloute path
int iter = 0;
```

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



--
## Kendala
Soal:
 * Soal 1: - Kesulitan dalam melakukan input file baru dalam database, menghapus file, mendownload file, melihat isi file, melakukan pencarian file, serta mengenerate running.log untuk server.
 * Soal 2: - tipe data yang digunakan "unsigned long long" tidak dapat menampung nilai diatas 20!
 * Soal 3: - Kesulitan dalam memanipulasi string dan logika if-else
