#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <pthread.h>
#include <errno.h>

//periksa file atau folder dan nama file
char *get_NamaFile(char *NamaF, char buffer[])
{
    char *sign = strtok(NamaF, "/");
    while (sign != NULL)
    {
        sprintf(buffer, "%s", sign);
        sign = strtok(NULL, "/");
    }
}

char *cek_Ext(char *NamaF, char buffer[])
{
    char bufferNamaFile[1000];
    char *sign = strtok(NamaF, "/");

    for (sign != NULL; sign++;)
    {
        sprintf(bufferNamaFile, "%s", sign);
        sign = strtok(NULL, "/");
    }

    int count = 0;
    sign = strtok(bufferNamaFile, ".");
    while (sign != NULL)
    {
        count++;
        sprintf(buffer, "%s", sign);
        sign = strtok(NULL, ".");
    }
    if (count <= 1)
    {
        strcpy(buffer, "unknown");
    }

    return buffer;
}

void dir_Check(char buffer[])
{
    DIR *dr = opendir(buffer);
    if (ENOENT == errno)
    {
        mkdir(buffer, 0700);
        closedir(dr);
    }
}

void *move(void *arg)
{
    char bufferExt[100], bufferNamaFile[1000], bufferFrom[1000], bufferTo[1000], cwd[1000];
    getcwd(cwd, sizeof(cwd));
    strcpy(bufferFrom, (char *)arg);

    if (access(bufferFrom, F_OK) == -1)
    {
        printf("Sad, gagal :(\n", bufferFrom);
        pthread_exit(0);
    }

    DIR *dir = opendir(bufferFrom);
    if (dir)
    {
        printf("Sad, gagal :(\n", bufferFrom);
        pthread_exit(0);
    }
    closedir(dir);

    _(bufferFrom, bufferNamaFile);
    strcpy(bufferFrom, (char *)arg);

    cek_Ext(bufferFrom, bufferExt);

    int i = 0;
    while (i < sizeof(bufferExt))
    {
        bufferExt[i] = tolower(bufferExt[i]); // convert it to a lowercase  using tolower
        i++;
    }

    strcpy(bufferFrom, (char *)arg);

    dir_Check(bufferExt);

    sprintf(bufferTo, "%s/%s/%s", cwd, bufferExt, bufferNamaFile);
    rename(bufferFrom, bufferTo);
    printf("Berhasil Dikategorikan\n", bufferFrom);

    pthread_exit(0);
}

int main(int argc, char *argv[])
{

    if (strcmp(argv[1], "-f") == 0)
    {
        pthread_t threadid[argc - 2]; //inisialisasi thread

        for (int i = 2; i < argc; i++)
        {
            pthread_create(&threadid[i - 2], NULL, &move, (void *)argv[i]);
        }

        int i = 2;
        while (i < argc)
        {
            pthread_join(threadid[i - 2], NULL);
            i++;
        }
        exit(0);
    }

    char *directory;
    if (strcmp(argv[1], "*") == 0)
    {
        char buffer[1000];
        getcwd(buffer, sizeof(buffer));
        directory = buffer;
    }

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

    DIR *dir = opendir(directory);
    struct dirent *enter;
    int fCount = 0;
    while ((enter = readdir(dir)) != NULL)
    {
        if (enter->d_type == DT_REG)
        {
            fCount++;
        }
    }
    closedir(dir);

    pthread_t threadid[fCount];
    char buffer[fCount][1000]; //simpan absoloute path
    int iter = 0;

    dir = opendir(directory);

    //pengecekan tiap file
    while ((enter = readdir(dir)) != NULL) //looping sampai file di dir = NULL
    {
        if (enter->d_type == DT_REG)
        {
            sprintf(buffer[iter], "%s/%s", directory, enter->d_name); //masukan absolut path dari setiap file
            iter++;
        }
    }

    closedir(dir);

    for (int i = 0; i < fCount; i++) //Looping sebanyak jumlah file reguler yang sudah tersimpan di buffer
    {
        char *test = (char *)buffer[i]; //Simpan terlebih dahulu absolut path
        printf("%s\n", test);
        pthread_create(&threadid[i], NULL, &move, (void *)test);
    }

    int i = 0;
    while (i < fCount)
    {
        pthread_join(threadid[i], NULL);
        i++;
    }
}