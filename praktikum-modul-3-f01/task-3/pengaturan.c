#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char command[255];

void create(char *namaContainer) {

    snprintf(command, sizeof(command), "sudo docker run -d --name %s dockermessage:v2", namaContainer);
    system(command);    
}

void delete(char *namaContainer) {
    snprintf(command, sizeof(command), "sudo docker rm -f %s", namaContainer);
    system(command);
}

void list() {
    system("sudo docker ps -a");
}

void login(char *namaContainer) {
    snprintf(command, sizeof(command), "sudo docker exec -it %s /bin/sh", namaContainer);
    system(command);
}

void connect(char *sndrContainer, char *rcvrContainer) {
    //hapus container yang sudah dibuat di fungsi create
    snprintf(command, sizeof(command), "sudo docker rm -f %s", sndrContainer);
    system(command);
    snprintf(command, sizeof(command), "sudo docker rm -f %s", rcvrContainer);
    system(command);

    system("sudo docker volume create --name messageVol");
    //pasang shared volume pada container yang sudah dibuat di fungsi create
    snprintf(command, sizeof(command), "sudo docker run -e DOCKER_CONTAINER_NAME=%s -d --name=%s -v messageVol:/app dockermessage:v2", sndrContainer, sndrContainer);
    system(command);
    snprintf(command, sizeof(command), "sudo docker run -e DOCKER_CONTAINER_NAME=%s -d --name=%s --volumes-from %s dockermessage:v2", rcvrContainer, rcvrContainer, sndrContainer);
    system(command);
}

void unconnect(char *sndrContainer, char *rcvrContainer) {
    //hapus shared volume dari container
    snprintf(command, sizeof(command), "sudo docker rm -f %s", sndrContainer);
    system(command);
    snprintf(command, sizeof(command), "sudo docker rm -f %s", rcvrContainer);
    system(command);

    snprintf(command, sizeof(command), "sudo docker volume rm messageVol");
    system(command);
}


int main(int argc, char *argv[]){
    if(argc < 2){
        perror("Error perintah kurang jelas!");
        exit(EXIT_FAILURE);
    }

    char *options = argv[1];
    char *namaContainer = NULL;
    char *sndrContainer = NULL;
    char *rcvrContainer = NULL;

    if (strcmp(options, "create") == 0) {
        if (argc < 3) {
            perror("Error tambahkan nama containernya!");
            exit(EXIT_FAILURE);
        }
        namaContainer = argv[2];
        create(namaContainer);
        printf("Container '%s' berhasil dibuat\n", namaContainer);
    }

    else if (strcmp(options, "delete") == 0) {
        if (argc < 3) {
            perror("Error tambahkan nama containernya!");
            exit(EXIT_FAILURE);
        }
        namaContainer = argv[2];
        delete(namaContainer);
        printf("Container '%s' berhasil dihapus\n", namaContainer);
    }

    else if (strcmp(options, "list") == 0) {
        list();
        printf("Berhasil melihat semua list container\n");
    } 
    
    else if (strcmp(options, "login") == 0) {
        if (argc < 3) {
            perror("Error tambahkan nama containernya!");
            exit(EXIT_FAILURE);
        } 
        namaContainer = argv[2];
        login(namaContainer);
        printf("Berhasil login ke dalam container '%s'\n", namaContainer);
    }

    else if (strcmp(options, "connect") == 0) {
        if (argc < 4) {
            perror("Error tambahkan sender dan receiver containernya!");
            exit(EXIT_FAILURE);
        }
        sndrContainer = argv[2];
        rcvrContainer = argv[3];
        connect(sndrContainer, rcvrContainer);
    }

    else if (strcmp(options, "unconnect") == 0) {
        if (argc < 4) {
            perror("Error tambahkan sender dan receiver containernya!");
            exit(EXIT_FAILURE);
        }
        sndrContainer = argv[2];
        rcvrContainer = argv[3];
        unconnect(sndrContainer, rcvrContainer);
    }

    else{
        perror("Error perintah salah");
        exit(EXIT_FAILURE);
    }
}