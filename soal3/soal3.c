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

//periksa file atau folder dan nama file
char *get_NamaFile(char *fName, char buffer[])
{
    char *token = strtok(fName, "/");
    while (token != NULL)
    {
        sprintf(buffer, "%s", token);
        token = strtok(NULL, "/");
    }
}

char *cek_Ext(char *fName, char buffer[])
{
    char bufferNamaFile[1000];
    char *token = strtok(fName, "/");

    for (token != NULL; token++;)
    {
        sprintf(bufferNamaFile, "%s", token);
        token = strtok(NULL, "/");
    }

    int count = 0;
    token = strtok(bufferNamaFile, ".");
    while (token != NULL)
    {
        count++;
        sprintf(buffer, "%s", token);
        token = strtok(NULL, ".");
    }
    if (count <= 1)
    {
        strcpy(buffer, "unknown");
    }

    return buffer;
}

void dirChecking(char buffer[])
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
        // printf("Sad, gagal :(\n", bufferFrom);
        pthread_exit(0);
    }

    DIR *dir = opendir(bufferFrom);
    if (dir)
    {
        printf("Sad, gagal :(\n", bufferFrom);
        pthread_exit(0);
    }
    closedir(dir);

    get_NamaFile(bufferFrom, bufferNamaFile);
    strcpy(bufferFrom, (char *)arg);

    cek_Ext(bufferFrom, bufferExt);

    int i = 0;
    while (i < sizeof(bufferExt))
    {
        bufferExt[i] = tolower(bufferExt[i]); // convert it to a lowercase  using tolower
        i++;
    }

    strcpy(bufferFrom, (char *)arg);
    dirChecking(bufferExt);
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

        for (int i = 2; i < argc; i++)
        {
            pthread_join(threadid[i - 2], NULL);
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

    pthread_t threadid[file_count];
    char buffer[file_count][1337]; //simpan absoloute path
    int iter = 0;

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
}
