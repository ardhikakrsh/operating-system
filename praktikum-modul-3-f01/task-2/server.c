#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/msg.h>
#include <sys/wait.h>

struct client
{
    char username[100];
    int turn;
    int length;
    int total;
};

struct user
{
    char nama[100];
    char nama_container[100];
    char image[100];
    char command[200];
    char volume[200];
};

struct message
{
    long mess_type;
    char nama_container[100];
    char image[100];
    char command[200];
    char volume[200];
    int turn;
} mess;

void createDockerCompose(int total, struct user *usr)
{
    FILE *file = fopen("docker-compose.yml", "w");
    if (file == NULL)
    {
        fprintf(stderr, "Failed to open file.\n");
        return;
    }

    fprintf(file, "version: '3.8'\n");
    fprintf(file, "services:\n");
    for (int i = 0; i < total; i++)
    {
        fprintf(file, "  %s:\n", usr[i].nama_container);
        fprintf(file, "    container_name: %s\n", usr[i].nama_container);
        fprintf(file, "    image: %s\n", usr[i].image);
        fprintf(file, "    command: %s\n", usr[i].command);
        fprintf(file, "    volumes:\n");
        fprintf(file, "      - %s\n", usr[i].volume);
    }

    fclose(file);
    printf("docker-compose.yml has been created successfully.\n");
}

int main()
{
    key_t key_server = 1234, key_user = 5678;
    int server_id = shmget(key_server, sizeof(struct client), IPC_CREAT | 0666);
    int user_id = shmget(key_user, 5 * sizeof(struct user), IPC_CREAT | 0666);
    struct client *cl = shmat(server_id, NULL, 0);
    struct user *usr = shmat(user_id, NULL, 0);
    cl->length = 0;
    cl->turn = 0;
    cl->total = 0;
    cl->username[0] = '*';
    printf("Total client yang akan dilayani (1-5): ");
    scanf("%d", &cl->total);
    while (cl->total != cl->length)
    {
    }
    printf("\nSemua client sudah berkumpul\n");

    key_t mess_key = 9101;
    int mess_id = msgget(mess_key, 0666 | IPC_CREAT);
    int ronde = 1;
    while (1)
    {
        // MESSAGED QUEUE
        msgrcv(mess_id, &mess, sizeof(mess), 1, 0);
        printf("\nService dari client %s berhasil diterima\n", usr[mess.turn - 1].nama);

        printf("Nama container: %s\n", mess.nama_container);
        strcpy(usr[mess.turn - 1].nama_container, mess.nama_container);

        printf("Image: %s\n", mess.image);
        strcpy(usr[mess.turn - 1].image, mess.image);

        printf("Command: %s\n", mess.command);
        strcpy(usr[mess.turn - 1].command, mess.command);

        printf("Volume: %s\n", mess.volume);
        strcpy(usr[mess.turn - 1].volume, mess.volume);
        // END MESSAGED QUEUE

        if (mess.turn == cl->total)
        {
            pid_t child_pid;
            child_pid = fork();
            if (child_pid == -1)
            {
                perror("Failed to fork");
                return 1;
            }
            if (child_pid == 0)
            {
                if (ronde > 1)
                {
                    pid_t child_pid_docker_compose;
                    child_pid_docker_compose = fork();
                    if (child_pid_docker_compose == 0)
                    {
                        pid_t child_pid_docker_rm;
                        child_pid_docker_rm = fork();
                        if (child_pid_docker_rm == 0)
                        {
                            if (execlp("sh", "sh", "-c", "docker rm $(docker container ps -aq)", NULL) == -1)
                            {
                                perror("Failed to execute command");
                            }
                        }
                        else
                        {
                            wait(NULL);
                            char *argv[] = {"rm", "docker-compose.yml", NULL};
                            execv("/usr/bin/rm", argv);
                        }
                    }
                    else
                    {
                        wait(NULL);
                    }
                }
                createDockerCompose(cl->total, usr);
                if (execlp("docker-compose", "docker-compose", "up", NULL) == -1)
                {
                    perror("Error executing Docker Compose");
                    return 1;
                }
            }
            else
            {
                ronde++;
            }
        }

        for (int i = 1; i <= cl->total; i++)
        {
            mess.mess_type = i;
            msgsnd(mess_id, &mess, sizeof(mess), 0);
        }
    }
    shmdt(cl);
    shmctl(server_id, IPC_RMID, NULL);
    shmdt(usr);
    shmctl(user_id, IPC_RMID, NULL);
}