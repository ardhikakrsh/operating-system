#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <errno.h>
#include <sys/stat.h>
#include <time.h>

void logMessage(const char *message){
    FILE *logFile = fopen("logbook.txt", "a");
    if (logFile == NULL) {
        perror("Failed to open logbook.txt");
        exit(EXIT_FAILURE);
    }

    time_t rawtime;
    char timestamp[20];
    struct tm *info;

    time(&rawtime);
    info = localtime(&rawtime);
    strftime(timestamp, 20, "%y-%m-%d %H:%M:%S", info);

    fprintf(logFile, "[%s] %s\n", timestamp, message);
    fclose(logFile);
}

void downloadUnzipRemove() {
    if (mkdir("weather", 0700) == -1) {
        perror("FAILED TO CREATE weather DIRECTORY");
        exit(EXIT_FAILURE);
    }
    // fork 1 - download zip from link
    pid_t pid = fork();
    int status;

    // child process
    if (pid == 0) {
        execlp("wget", "wget", "-O", "weather.zip", "https://drive.usercontent.google.com/u/0/uc?id=1U1-9v9pJeX5MoofwoBHCcD9nA7YWy_np&export=download", NULL);
        perror("FAILED TO DOWNLOAD WEATHER DATA");
        exit(EXIT_FAILURE);
    } 

    // parent process
    else if (pid > 0) {
        wait(&status);
        if (WIFEXITED(status) && WEXITSTATUS(status) == 0) {
            printf("FILE DOWNLOADED SUCCESSFULLY\n");
            logMessage("Successfully download file..");

            // fork 2 - unzip filenya
            pid_t pid2 = fork();
            if (pid2 == 0) {
                execlp("unzip", "unzip", "weather.zip", "-d", "weather", NULL);
                perror("FAILED TO EXTRACT WEATHER DATA");
                exit(EXIT_FAILURE);
            } 
            // parent process
            else if (pid2 > 0) {
                wait(&status);
                if( WIFEXITED(status) && WEXITSTATUS(status) == 0){
                    printf("FILE UNZIPED SUCCESSFULLY\n");
                    logMessage("Successfully unzip file..");

                    // delete the zip file
                    int delete_status = remove("weather.zip");
                    if (delete_status == 0)
                        printf("ZIP FILE DELETED SUCCESSFULLY\n");
                    else {
                        perror("FAILED TO DELETING ZIP FILE");
                        exit(EXIT_FAILURE);
                    }
                }
                else{
                    perror("UNZIP FAILED");
                    exit(EXIT_FAILURE);
                }
            } 
            // Jika gagal membuat child process kedua, cetak pesan kesalahan
            else {
                perror("FORK 2 - FAILED\n");
                exit(EXIT_FAILURE);
            }
        } 
        // Jika gagal mendownload file, cetak pesan kesalahan
        else {
            perror("DOWNLOAD FAILED\n");
            exit(EXIT_FAILURE);
        }
    } 
    // Jika gagal membuat child process pertama, cetak pesan kesalahan
    else {
        perror("FORK 1 - FAILED\n");
        exit(EXIT_FAILURE);
    }
}

void preprocessData(){
    pid_t pid = fork();
    int status;

    if (pid == 0){
        execlp("/bin/sh", "sh", "-c", "awk -F, '{ if ($4 <= 35) print $0 }' weather/kecamatanforecast.csv > weather/kecamatanforecast_temp.csv", NULL);
        perror("FAILED TO EXECUTE AWK");
        exit(EXIT_FAILURE);
    }

    else if (pid>0){
        wait(&status);
        if (WIFEXITED(status) && WEXITSTATUS(status) == 0){
            printf("PREPROCESSING DATA SUCESSFULLY\n");
            FILE *file = fopen("weather/kecamatanforecast.csv", "r");

            if (file != NULL){
                char line[1024];
                while (fgets(line, sizeof(line), file)){
                    int suhu;
                    sscanf(line, "%*[^,],%*[^,],%*[^,],%d", &suhu);
                    if(suhu > 35){
                        sprintf(line, "Successfully drop row with air temperature %d degree..", suhu);
                        logMessage(line);
                    }
                }
            }

            else {
                perror("FAILED TO OPEN TEMP FILE");
                exit(EXIT_FAILURE);
            }

            fclose(file);
            remove("weather/kecamatanforecast.csv");
            rename("weather/kecamatanforecast_temp.csv", "weather/kecamatanforecast.csv");
        }
        
        else {
            perror("FAILED TO PREPROCESSING DATA");
            exit(EXIT_FAILURE);
        }
    }

    else{
        perror("FORK PREPROCESSING DATA - FAILED");
        exit(EXIT_FAILURE);
    }
}

void newDataSet(){
    pid_t pid = fork();
    int status;

    if (pid == 0){
        execlp("mkdir", "mkdir", "-p", "city_group", NULL);
        perror("FAILED TO CREATE city_group DIRECTORY");
        exit(EXIT_FAILURE);
    }

    else if (pid > 0){
        wait(&status);
        if (WIFEXITED(status) && WEXITSTATUS(status) == 0){
            printf("MAKE DIRECTORY city_GROUP SUCCESSFULLY\n");

            pid_t pid2 = fork();
            if (pid2 == 0) {
                execlp("awk", "awk", "-F,", "NR > 1 { print > \"city_group/city_\"$1\".csv\" }", "weather/kecamatanforecast.csv", NULL);
                perror("FAILED TO EXECUTE AWK");
                exit(EXIT_FAILURE);
            } 

            else if (pid2 > 0){
                wait(&status);
                if (WIFEXITED(status) && WEXITSTATUS(status) == 0){
                    printf("GROUPING SUCCESSFULLY\n");
                    logMessage("Successfully data grouping..");
                }

                else {
                    perror("FAILED GROUPING DATA");
                    exit(EXIT_FAILURE);
                }
            }

            else{
                perror("FAILED AWK");
                exit(EXIT_FAILURE);
            }
        }

        else {
            perror("FAILED TO MAKE DIRECTORY");
            exit(EXIT_FAILURE);
        }
    }

    else{
        perror("FAILED FORK GROUPING DATA");
        exit(EXIT_FAILURE);
    }
}

int main() {
    //no 3a
    downloadUnzipRemove();

    //no 3b
    preprocessData();

    //no 3c
    newDataSet();
    return 0;
}
