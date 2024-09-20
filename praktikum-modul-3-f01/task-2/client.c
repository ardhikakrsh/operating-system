#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/msg.h>

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

int main()
{
    key_t key_server = 1234, key_user = 5678;
    int server_id = shmget(key_server, sizeof(struct client), 0666);
    int user_id = shmget(key_user, 5 * sizeof(struct user), 0666);
    struct client *cl = shmat(server_id, NULL, 0);
    struct user *usr = shmat(user_id, NULL, 0);
    if (cl->total == 0)
    {
        printf("Sedang menunggu server...\n");
    }
    while (cl->total < 1 || cl->total > 5)
    {
        // printf("%d\n", cl->total);
    }
    printf("Server akan melayani %d client\n", cl->total);
    int myTurn = 0;
    if (cl->length == 0)
    {
        printf("Masukan username anda:");
        scanf("%s", cl->username);
        strcpy(usr[0].nama, cl->username);
        cl->length = 1;
        myTurn = cl->length;
        if (cl->total > 1)
        {
            printf("Menunggu user lain...\n");
        }
        while (cl->length != cl->total)
        {
        }
    }
    else
    {
        // printf("%s",usr[0].nama);
        while (1)
        {
            printf("Masukan username anda:");
            scanf("%s", cl->username);
            int check = 0;
            for (int i = 0; i < cl->length; i++)
            {
                if (strcmp(cl->username, usr[i].nama) == 0)
                {
                    printf("Username telah dipakai, silahkan pilih username lain\n");
                    check = 1;
                    break;
                }
                if (i == cl->length - 1 && check == 0)
                {
                    // printf("Input username berhasil.\nUsername anda: %s\n", cl->username);
                    strcpy(usr[cl->length].nama, cl->username);
                    cl->length++;
                    myTurn = cl->length;
                    break;
                    // printf("%d %d\n", cl->total, cl->length);
                }
            }
            if (myTurn != 0)
            {
                break;
            }
        }
        if (cl->total > cl->length)
        {
            printf("Menunggu user lain...\n");
        }
        while (cl->length != cl->total)
        {
        }
    }
    printf("\nPemilihan user telah berhasil.\n\n\tSelamat datang %s\n", usr[myTurn - 1].nama);

    // MESSAGED QUEUE
    key_t mess_key = 9101;
    int mess_id = msgget(mess_key, 0666 | IPC_CREAT);
    int turn = 1;
    while (1)
    {
        if (turn == myTurn)
        {
            printf("Masukan nama container:");
            scanf(" %[^\n]", mess.nama_container);
            printf("Masukan nama image:");
            scanf(" %[^\n]", mess.image);
            printf("Masukan command yang akan dijalankan:");
            scanf(" %[^\n]", mess.command);
            printf("Masukan volume:");
            scanf(" %[^\n]", mess.volume);
            mess.turn = myTurn;
            mess.mess_type = 1;
            msgsnd(mess_id, &mess, sizeof(mess), 0);
        }
        else
        {
            printf("Menunggu client lain...\n");
        }
        msgrcv(mess_id, &mess, sizeof(mess), myTurn, 0);
        if (turn == cl->total)
        {
            turn = 1;
            continue;
        }
        turn++;
    }

    shmdt(cl);
    shmctl(server_id, IPC_RMID, NULL);
    shmdt(usr);
    shmctl(user_id, IPC_RMID, NULL);
}