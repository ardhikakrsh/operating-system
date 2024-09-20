#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <time.h>
#include <glob.h>

#define ZIP_FILE_URL "https://dl.dropboxusercontent.com/scl/fi/gmy0qvbysh3h7vdexso9k/task_sisop.zip?rlkey=ism9qfjh82awg2tbtzfbrylg4&dl=0"
#define ZIP_FILE_NAME "tasksisop.zip"
#define EXTRACT_FOLDER "extract"
#define MAX_PATH_LENGTH 256
#define LOG_FILE "recap.txt"
#define TASK_DIRECTORY "/home/faiq/Documents/Kuliah/Sisop/Praktikum_2/praktikum-modul-2-f01/task-2/task"

const char baseUrls[] = "https://source.unsplash.com/random/";

void downloadZip() {
    pid_t pid = fork();
    int status;

    // Child process
    if (pid == 0) {
        // Create the "extract" folder
        if (mkdir(EXTRACT_FOLDER, 0700) == -1 && errno != EEXIST) {
            perror("FAILED TO CREATE EXTRACT FOLDER");
            exit(EXIT_FAILURE);
        }
        execlp("wget", "wget", "-O", ZIP_FILE_NAME, ZIP_FILE_URL, NULL);
        perror("FAILED TO DOWNLOAD FILE");
        exit(EXIT_FAILURE);
    } 
    // Parent process
    else if (pid > 0) {
        // Wait for child process to finish
        wait(&status);
        if (WIFEXITED(status) && WEXITSTATUS(status) == 0) {
            printf("FILE DOWNLOADED SUCCESSFULLY\n");

            // Fork untuk mengekstrak file ZIP
            pid_t pid2 = fork();
            if (pid2 == 0) {
                execlp("unzip", "unzip", ZIP_FILE_NAME, "-d", EXTRACT_FOLDER, NULL);
                perror("FAILED TO EXTRACT FILE");
                exit(EXIT_FAILURE);
            } 
            // Parent process
            else if (pid2 > 0) {
                // Wait for child process to finish
                wait(&status);
                if (WIFEXITED(status) && WEXITSTATUS(status) == 0) {
                    printf("FILE EXTRACTED SUCCESSFULLY\n");

                    // Hapus file zip setelah proses selesai
                    if (remove(ZIP_FILE_NAME) == 0) {
                        printf("ZIP FILE DELETED SUCCESSFULLY\n");
                    } else {
                        perror("FAILED TO DELETE ZIP FILE");
                        exit(EXIT_FAILURE);
                    }

                    // Membuat folder "task"
                    if (mkdir(TASK_DIRECTORY, 0700) == -1 && errno != EEXIST) {
                        perror("FAILED TO CREATE TASK FOLDER");
                        exit(EXIT_FAILURE);
                    }

                    // Pindahkan file ke folder "task"
                    pid_t pid3 = fork();
                    if (pid3 == 0) {
                        execlp("find", "find", EXTRACT_FOLDER, "-type", "f", "-name", "task_*.txt", "-exec", "mv", "{}", TASK_DIRECTORY, ";", NULL);
                        perror("FAILED TO MOVE FILES TO TASK FOLDER");
                        exit(EXIT_FAILURE);
                    } else if (pid3 > 0) {
                        wait(&status);
                        if (WIFEXITED(status) && WEXITSTATUS(status) == 0) {
                            printf("FILES MOVED TO TASK FOLDER SUCCESSFULLY\n");

                            execlp("rm", "rm", "-r", EXTRACT_FOLDER, NULL);
                            perror("FAILED TO DELETE EXTRACT FOLDER");
                            exit(EXIT_FAILURE);
                        } else {
                            perror("FAILED TO MOVE FILES TO TASK FOLDER");
                            exit(EXIT_FAILURE);
                        }
                    } else {
                        perror("FORK 3 - FAILED\n");
                        exit(EXIT_FAILURE);
                    }
                } else {
                    perror("EXTRACTION FAILED");
                    exit(EXIT_FAILURE);
                }
            } else {
                perror("FORK 2 - FAILED\n");
                exit(EXIT_FAILURE);
            }
        } else {
            perror("DOWNLOAD FAILED\n");
            exit(EXIT_FAILURE);
        }
    } else {
        perror("FORK FAILED\n");
        exit(EXIT_FAILURE);
    }
}

void logCompletion(const char *filename, int i, const char *name, int jumlah, const char *kategori, const char *resolusi) {
    FILE *file = fopen(filename, "a");
    if (file == NULL) {
        perror("fopen");
        return;
    }

    time_t current_time;
    struct tm *local_time;
    char timeString[20];

    time(&current_time);
    local_time = localtime(&current_time);
    strftime(timeString, sizeof(timeString), "%Y-%m-%d %H:%M:%S", local_time);

    fprintf(file, "[%s]-[%s] Task %d completed, download %d %s images with %s resolution\n",
            timeString, name, i, jumlah, kategori, resolusi);

    fclose(file);
}

void yuantask(const char *directory) {
    for (int i = 0; i < 10; ++i) {
        char filename[MAX_PATH_LENGTH];
        snprintf(filename, sizeof(filename), "%s/task_%d_*.txt", directory, i);
        printf("YUAN BEKERJA\n");

        glob_t glob_result;
        glob(filename, GLOB_TILDE, NULL, &glob_result);

        for (size_t j = 0; j < glob_result.gl_pathc; ++j) {
            FILE *file = fopen(glob_result.gl_pathv[j], "r");
            if (file == NULL) {
                perror("Error opening file");
                continue;
            }

            int jumlah;
            char kategori[20], resolusi[20];
            fscanf(file, "%d %s %s", &jumlah, resolusi, kategori); // Skip id
            fclose(file);

            logCompletion(LOG_FILE, i, "yuan", jumlah, kategori, resolusi);
        }

        globfree(&glob_result);
    }
}

void bubutask(const char *directory) {
    for (int i = 19; i >= 10; --i) {
        char filename[MAX_PATH_LENGTH];
        snprintf(filename, sizeof(filename), "%s/task_%d_*.txt", directory, i);
        printf("BUBU BEKERJA\n");

        glob_t glob_result;
        glob(filename, GLOB_TILDE, NULL, &glob_result);

        for (size_t j = 0; j < glob_result.gl_pathc; ++j) {
            FILE *file = fopen(glob_result.gl_pathv[j], "r");
            if (file == NULL) {
                perror("Error opening file");
                continue;
            }

            int jumlah;
            char kategori[20], resolusi[20];
            fscanf(file, "%d %s %s", &jumlah, resolusi, kategori); // Skip id
            fclose(file);

            logCompletion(LOG_FILE, i, "bubu", jumlah, kategori, resolusi);
        }

        globfree(&glob_result);
    }
}

void runTasks(const char *task_directory) {
    pid_t pid_yuan, pid_bubu;

    pid_yuan = fork();
    if (pid_yuan == 0) {
        yuantask(task_directory);
        exit(EXIT_SUCCESS);
    } else if (pid_yuan < 0) {
        perror("Fork failed");
        exit(EXIT_FAILURE);
    }

    pid_bubu = fork();
    if (pid_bubu == 0) {
        bubutask(task_directory);
        exit(EXIT_SUCCESS);
    } else if (pid_bubu < 0) {
        perror("Fork failed");
        exit(EXIT_FAILURE);
    }

    // Menunggu kedua proses anak selesai
    waitpid(pid_yuan, NULL, 0);
    waitpid(pid_bubu, NULL, 0);
}

void downloadImagesFromTasks(const char *baseFilenames) {
    for (int y = 0; y < 10; y++) {
        int jumlah;
        char kategori[20], resolusi[20], baseDirectory[1000];
        DIR *directory;
        struct dirent *entry;
        sprintf(baseDirectory, "%s/Yuan/task%d", TASK_DIRECTORY, y);
        directory = opendir(baseDirectory);
        if (!directory) {
            perror("opendir");
            exit(EXIT_FAILURE);
        }
        char angkaFile[5];

        while ((entry = readdir(directory)) != NULL) {
            snprintf(angkaFile, sizeof(angkaFile), "%d", y);
            if (strstr(entry->d_name, angkaFile) != NULL) {
                char filepath[150];
                char filename[100];
                strcpy(filename, entry->d_name);

                snprintf(filepath, sizeof(filepath), "%s/%s", baseDirectory, filename);
                FILE *file = fopen(filepath, "r");
                if (!file) {
                    perror("fopen");
                    exit(EXIT_FAILURE);
                }
                fscanf(file, "%d %s %s", &jumlah, resolusi, kategori);
                fclose(file);

                for (int i = 1; i <= jumlah; i++) {
                    pid_t pid = fork(); 
                    if (pid == 0) {
                        char url[1000]; 
                        char filename[100]; 
                  
                        sprintf(url, "%s%s/?%s", baseUrls, resolusi, kategori);
                        sprintf(filename, "%s/Yuan/task%d/gambar%d.jpeg", baseFilenames, y, i);   
                        
                        execlp("wget", "wget", "-O", filename, url, NULL);
                        
                        perror("exec");
                        _exit(1); 
                    } else if (pid < 0) {
                        perror("fork");
                        _exit(1); 
                    } else {
                        wait(NULL);
                    }
                }
            }
        }
        closedir(directory);
    }

    for (int y = 19; y >= 10; y--) {
        int jumlah;
        char kategori[20], resolusi[20], baseDirectory[1000];
        DIR *directory;
        struct dirent *entry;
        sprintf(baseDirectory, "%s/Bubu/task%d", TASK_DIRECTORY, y);
        directory = opendir(baseDirectory);
        if (!directory) {
            perror("opendir");
            exit(EXIT_FAILURE);
        }
        char angkaFile[5];

        while ((entry = readdir(directory)) != NULL) {
            snprintf(angkaFile, sizeof(angkaFile), "%d", y);
            if (strstr(entry->d_name, angkaFile) != NULL) {
                char filepath[150];
                char filename[100];
                strcpy(filename, entry->d_name);

                snprintf(filepath, sizeof(filepath), "%s/%s", baseDirectory, filename);
                FILE *file = fopen(filepath, "r");
                if (!file) {
                    perror("fopen");
                    exit(EXIT_FAILURE);
                }
                fscanf(file, "%d %s %s", &jumlah, resolusi, kategori);
                fclose(file);

                for (int i = 1; i <= jumlah; i++) {
                    pid_t pid = fork(); 
                    if (pid == 0) {
                        char url[100]; 
                        char filename[1000]; 
                  
                        sprintf(url, "%s%s/?%s", baseUrls, resolusi, kategori);
                        sprintf(filename, "%s/Bubu/task%d/gambar%d.jpeg", baseFilenames, y, i);   
                        
                        execlp("wget", "wget", "-O", filename, url, NULL);
                        
                        perror("exec");
                        _exit(1); 
                    } else if (pid < 0) {
                        perror("fork");
                        _exit(1); 
                    } else {
                        wait(NULL);
                    }
                }
            }
        }
        closedir(directory);
    }
}

void moveFilesToDirectories() {
    DIR *dir;
    struct dirent *entry;
    char sourceFolder[1024];
    snprintf(sourceFolder, sizeof(sourceFolder), "%s", TASK_DIRECTORY);
    char destinationFolder[1024];

    dir = opendir(sourceFolder);
    if (dir == NULL) {
        perror("opendir");
        exit(EXIT_FAILURE);
    }

    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue; // Skip "." and ".." directories
        }

        struct stat st;
        char filePath[1024];
        snprintf(filePath, sizeof(filePath), "%s/%s", sourceFolder, entry->d_name);

        if (stat(filePath, &st) == -1) {
            perror("stat");
            exit(EXIT_FAILURE);
        }

        if (!S_ISREG(st.st_mode)) {
            continue; // Skip non-regular files (e.g., directories)
        }

        int taskNumber;
        if (sscanf(entry->d_name, "task_%d_", &taskNumber) != 1 || taskNumber < 0 || taskNumber > 19) {
            fprintf(stderr, "Invalid file name format: %s\n", entry->d_name);
            continue;
        }

        if (taskNumber >= 0 && taskNumber <= 9) {
            snprintf(destinationFolder, sizeof(destinationFolder), "%s/Yuan/task%d/%s", sourceFolder, taskNumber, entry->d_name);
        } else if (taskNumber >= 10 && taskNumber <= 19) {
            snprintf(destinationFolder, sizeof(destinationFolder), "%s/Bubu/task%d/%s", sourceFolder, taskNumber, entry->d_name);
        } else {
            fprintf(stderr, "Invalid task number: %d\n", taskNumber);
            continue;
        }

        if (rename(filePath, destinationFolder) == -1) {
            perror("rename");
            exit(EXIT_FAILURE);
        }
    }

    closedir(dir);
}

void taskC() {
    pid_t pid;

    char *yuanFolder = "/home/faiq/Documents/Kuliah/Sisop/Praktikum_2/praktikum-modul-2-f01/task-2/task/Yuan";
    if (mkdir(yuanFolder, 0777) == -1 && errno != EEXIST) {
        perror("mkdir");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < 10; i++) {
        char taskFolder[100];
        snprintf(taskFolder, sizeof(taskFolder), "%s/task%d", yuanFolder, i);
        if (mkdir(taskFolder, 0777) == -1 && errno != EEXIST) {
            perror("mkdir");
            exit(EXIT_FAILURE);
        }
    }

    char *bubuFolder = "/home/faiq/Documents/Kuliah/Sisop/Praktikum_2/praktikum-modul-2-f01/task-2/task/Bubu";
    if (mkdir(bubuFolder, 0777) == -1 && errno != EEXIST) {
        perror("mkdir");
        exit(EXIT_FAILURE);
    }

    for (int i = 19; i >= 10; i--) {
        char taskFolder[100];
        snprintf(taskFolder, sizeof(taskFolder), "%s/task%d", bubuFolder, i);
        if (mkdir(taskFolder, 0777) == -1 && errno != EEXIST) {
            perror("mkdir");
            exit(EXIT_FAILURE);
        }
    }
}

void taskD() {
    FILE *recapFile = fopen("recap.txt", "a+");
    if (recapFile != NULL) {
        int totalImages = 0;
        int totalCategories = 0;
        char categories[100][150] = {0}; // Inisialisasi array categories
        int counts[100] = {0}; // Inisialisasi array counts

        char line[2000];
        while (fgets(line, sizeof(line), recapFile) != NULL) {
            int count;
            char name[20], resolusi[20], kategori[50];
            // Sesuaikan format string dengan format yang benar dalam file recap.txt
            if (sscanf(line, "[%*[^]]]-[%[^]]] Task %*d completed, download %d %[^ ] images with %[^ ] resolution", name, &count, kategori, resolusi) == 4) {
                int categoryIndex = -1;
                for (int i = 0; i < totalCategories; i++) {
                    if (strcmp(categories[i], kategori) == 0) {
                        categoryIndex = i;
                        break;
                    }
                }
                if (categoryIndex == -1) {
                    strcpy(categories[totalCategories], kategori);
                    categoryIndex = totalCategories;
                    totalCategories++;
                }
                counts[categoryIndex] += count;
                totalImages += count;
            }
        }

        // Menulis hasil perhitungan ke dalam file
        for (int i = 0; i < totalCategories; i++) {
            fprintf(recapFile, "%s : %d\n", categories[i], counts[i]);
        }
        fprintf(recapFile, "total images: %d\n", totalImages);
        fclose(recapFile);
    } else {
        perror("Failed to open file");
        exit(1);
    }
}

void taskE() {
    const char *directoryToZip = TASK_DIRECTORY;
    const char *outputZipFile = "/home/faiq/Documents/Kuliah/Sisop/Praktikum_2/praktikum-modul-2-f01/task-2/task/submissions.zip";

    execlp("zip", "zip", "-r", outputZipFile, "task/Yuan", "task/Bubu", NULL);

    perror("execlp");
    exit(1);
}

int main() {
  pid_t pid1, pid2, pid3a, pid3b, pid3c, pid4, pid5;

pid1 = fork();
    if (pid1 == 0) {
        downloadZip();
        _exit(0);
    } else if (pid1 < 0) {
        perror("Fork failed for task A");
        return 1;
    } else {
        wait(NULL);
        printf("Task A selesai dikerjakan.\n");
    }

pid2 = fork();
    if (pid2 == 0) {
        runTasks(TASK_DIRECTORY);
        _exit(0);
    } else if (pid2 < 0) {
        perror("Fork failed for task B");
        return 1;
    } else {
        wait(NULL);
        printf("Task B selesai dikerjakan.\n");
    }

pid3a = fork();
    if (pid3a == 0) {
        taskC();
        _exit(0);
    } else if (pid3a < 0) {
        perror("Fork failed for task Ca");
        return 1;
    } else {
        wait(NULL);
        printf("Task Ca selesai dikerjakan.\n");
    }

pid3b = fork();
    if (pid3b == 0) {
        moveFilesToDirectories();
        _exit(0);
    } else if (pid3b < 0) {
        perror("Fork failed for task Cb");
        return 1;
    } else {
        wait(NULL);
        printf("Task Cb selesai dikerjakan.\n");
    }

pid3c = fork();
    if (pid3c == 0) {
        downloadImagesFromTasks(TASK_DIRECTORY);
        _exit(0);
    } else if (pid3c < 0) {
        perror("Fork failed for task Cc");
        return 1;
    } else {
        wait(NULL);
        printf("Task Cc selesai dikerjakan.\n");
    }

pid4 = fork();
    if (pid4 == 0) {
        taskD();
        _exit(0);
    } else if (pid4 < 0) {
        perror("Fork failed for task D");
        return 1;
    } else {
        wait(NULL);
        printf("Task D selesai dikerjakan.\n");
    }

    pid5 = fork();
    if (pid5 == 0) {
        taskE();
        _exit(0);
    } else if (pid5 < 0) {
        perror("Fork failed for task E");
        return 1;
    } else {
        wait(NULL);
        printf("Task E selesai dikerjakan.\n");
    }

  return 0;
}