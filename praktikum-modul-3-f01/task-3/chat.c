#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#define MAX_WORD_LENGTH 100
#define MAX_nama_file_LENGTH 100
#define MAX_THREADS 100

// Struktur data yang akan dikirimkan ke fungsi thread
struct ThreadArgs {
    char *rcvrContainer;
    char word[MAX_WORD_LENGTH];
};

// Fungsi yang akan dijalankan oleh setiap thread
void *processWord(void *arg) {
    struct ThreadArgs *args = (struct ThreadArgs *)arg;
    char *rcvrContainer = args->rcvrContainer;
    char *word = args->word;

    pthread_t tid = pthread_self();
    // printf("Thread %lu: %s\n", tid, word);

    //buat file dari char *rcvrContainer = argv[argc - 1] sekarang
    char filename[MAX_WORD_LENGTH * 10];
    sprintf(filename, "%s.txt", rcvrContainer);
    FILE *file = fopen(filename, "a");
    if (file == NULL) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    fprintf(file, "%s ", word);

    fclose(file);
    return NULL;
}

void writeMesg(char *rcvrContainer, char *message) {
    char *containerLingkungan = getenv("DOCKER_CONTAINER_NAME");
    if (containerLingkungan == NULL) {
        printf("Error : Pengguna tidak ditemukan.\n");
        return;
    }
    // Menghitung jumlah kata dalam pesan
    int jumlahKata = 0;
    char *token = strtok(message, " ");
    pthread_t threads[MAX_THREADS];
    struct ThreadArgs args[MAX_THREADS];

    // Membuat thread untuk setiap kata
    while (token != NULL && jumlahKata < MAX_THREADS) {
        args[jumlahKata].rcvrContainer = rcvrContainer;
        strcpy(args[jumlahKata].word, token);
        // printf("Thread %d: %s\n\n", jumlahKata + 1, args[jumlahKata].word); // Menampilkan kata yang diproses oleh thread
        pthread_create(&threads[jumlahKata], NULL, processWord, &args[jumlahKata]);
        sleep(1); 
        jumlahKata++;
        token = strtok(NULL, " ");
    }

    // Menunggu semua thread selesai sebelum melanjutkan
    for (int i = 0; i < jumlahKata; i++) {
        pthread_join(threads[i], NULL);
    }

    // printf("Jumlah thread yang berjalan: %d\n", jumlahKata);
}

void readMesg(char *containerLingkungan) {
    if (containerLingkungan == NULL) {
        printf("Error : Pengguna tidak ditemukan.\n");
        return;
    }
    //buka file dari char *containerLingkungan = getenv("DOCKER_CONTAINER_NAME") sekarang
    char filename[MAX_WORD_LENGTH * 10];
    sprintf(filename, "%s.txt", containerLingkungan);
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    char line[MAX_WORD_LENGTH * 10];
    // Loop through each line in the file
    while (fgets(line, sizeof(line), file) != NULL) {
        printf("%s", line);
    }

    fclose(file);
}

int main(int argc, char *argv[]) {
    char *containerLingkungan = getenv("DOCKER_CONTAINER_NAME");

    if(argc < 2){
        perror("Error: ./chat [option] [message] [rcvrContainer]");
        exit(EXIT_FAILURE);
    }

    char *option = argv[1];
    char *rcvrContainer = argv[argc - 1];

    if (strcmp(option, "write") == 0 && argv[argc - 1]) {
        if (argc < 3) {
            perror("Usage: ./chat write [pesan] [rcvrContainer]");
            exit(EXIT_FAILURE);
        }
        char message[MAX_WORD_LENGTH * 10];
        strcpy(message, argv[2]);
        for (int i = 3; i < argc - 1; i++) {
            strcat(message, " ");
            strcat(message, argv[i]);
        }

        writeMesg(rcvrContainer, message);
        char filename[MAX_WORD_LENGTH * 10];
        sprintf(filename, "%s.txt", rcvrContainer);
        FILE *file = fopen(filename, "a");
        if (file == NULL) {
            perror("Error opening file");
            exit(EXIT_FAILURE);
        }

        fprintf(file, "from %s\n", containerLingkungan);
        fclose(file);        
    }

    else if (strcmp(option, "read") == 0 && argc == 2) {
        readMesg(containerLingkungan);
    }
    
    else {
        printf("Invalid option!\n");
        exit(EXIT_FAILURE);
    }

    return 0;
}
