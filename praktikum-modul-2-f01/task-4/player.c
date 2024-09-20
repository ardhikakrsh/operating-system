#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/shm.h>
#include <string.h>
#include <stdbool.h>

#define SHM_SIZE sizeof(struct shared_memory)

struct message {
    long mesg_type;
    char mesg_text[100];
};

struct shared_memory{
    int player_X;
    int player_O;
    bool turn;
    bool gameEnd;
    bool lose;
    bool tie;
    char board[3][3];
};

void displayBoard(char board[3][3]){
    printf("\n");
    printf("  %c | %c | %c\n", board[0][0], board[0][1], board[0][2]);
    printf(" --- --- ---\n");
    printf("  %c | %c | %c\n", board[1][0], board[1][1], board[1][2]);
    printf(" --- --- ---\n");
    printf("  %c | %c | %c\n", board[2][0], board[2][1], board[2][2]);
}

int main(){
    key_t key;
    struct message msg;

    // Generate unique key
    key = ftok("game.c", 'R');
    int msgid = msgget(key, 0666 | IPC_CREAT);
    int shmid = shmget(key, SHM_SIZE, 0666 | IPC_CREAT);
    // Attach to shared memory
    struct shared_memory *shm = (struct shared_memory *)shmat(shmid, NULL, 0);

    int choose_player;
    char player_type;
    shm->tie = false;
    shm->lose = false;
    shm->gameEnd = false;

    // Loop until both players have chosen their types
    while (!(shm->player_X && shm->player_O)){
        printf("Available player type:\n");
        printf("1. X\n");
        printf("2. O\n");
        printf("Enter your choice (1 or 2): ");
        scanf("%d", &choose_player);
        break;
    }

    if (choose_player == 1){
        if (shm->player_X){
            printf("'X' has already been taken, choose other player types\n");
            exit(1);
        }
        shm->turn = true;
        player_type = 'X';
        shm->player_X = 1;
        if (shm->player_X == 1 && (!shm->player_O)){
            printf("Waiting for another player to join the game\n");
        }
    }

    else if (choose_player == 2){
        if (shm->player_O){
            printf("'O' has already been taken, choose other player types\n");
            exit(1);
        }
        shm->turn = true;
        player_type = 'O';
        shm->player_O = 1;
        if ((!shm->player_X) && shm->player_O == 1){
            printf("Waiting for another player to join the game\n");
        }
    }

    else{
        printf("Invalid choose player\n");
        exit(1);
    }

    while (1){
        if (shm->player_X == 1 && shm->player_O == 1){
            printf("\n----Game Started!----\n");
            printf("You play as '%c'\n", player_type);
            printf("1 | 2 | 3\n");
            printf("--- --- ---\n");
            printf("4 | 5 | 6\n");
            printf("--- --- ---\n");
            printf("7 | 8 | 9\n");
            printf("\nSelect the corresponding cell number to choose a cell.\n");

            if (player_type == 'O'){
                printf("\nPlayer X turn\n");
            }

            while (1){
                if (player_type == 'X' && shm->turn == true){
                    displayBoard(shm->board);
                    printf("\nYour turn\n");
                    int cell_number;
                    do{
                        printf("Enter a cell number: ");
                        scanf("%d", &cell_number);

                        // kirim message player type sm angka ke game server
                        msg.mesg_type = 1; // Player to game server
                        snprintf(msg.mesg_text, sizeof(msg.mesg_text), "%c%d", player_type, cell_number);
                        if (msgsnd(msgid, &msg, sizeof(msg.mesg_text), 0) == -1){
                            perror("gagal mengirim pesan ke server\n");
                            exit(1);
                        }
                        // nerima pesan cek valid input dari game
                        if (msgrcv(msgid, &msg, sizeof(msg.mesg_text), 2, 0) == -1){
                            perror("gagal menerima cek valid dari game\n");
                            exit(1);
                        }

                        if (strcmp(msg.mesg_text, "invalid") == 0){
                            printf("Wrong input, choose cell number 1-9\n");
                            printf("\n");
                        }
                        else if (strcmp(msg.mesg_text, "filled") == 0){
                            int row = (cell_number - 1) / 3;
                            int col = (cell_number - 1) % 3;                            
                            printf("Choose other cell, %d is filled by '%c'\n", cell_number, shm->board[row][col]);
                            printf("\n");
                        }
                    } while (strcmp(msg.mesg_text, "valid") != 0);

                    displayBoard(shm->board);

                    if (msgrcv(msgid, &msg, sizeof(msg.mesg_text), 3, 0) == -1){
                        perror("gagal menerima pesan cek game over dari game\n");
                        exit(1);
                    }

                    if (strcmp(msg.mesg_text, "win") == 0){
                        printf("\n---Game Finished!---\n");
                        printf("You Win!\n");
                        shm->lose = true;
                        goto out_of_loops;
                    }

                    if (strcmp(msg.mesg_text, "tie") == 0){
                        printf("\n---Game Finished!---\n");
                        printf("It's a Tie!\n");
                        shm->tie = true;
                        goto out_of_loops;
                    }

                    if (strcmp(msg.mesg_text, "ongoing") == 0){
                        printf("\nPlayer O turn\n");
                        shm->turn = false;
                        continue;
                    }   
                }

                else if (player_type == 'O' && shm->turn == false){
                    displayBoard(shm->board);
                    printf("\nYour turn\n");
                    int cell_number;
                    do{
                        printf("Enter a cell number: ");
                        scanf("%d", &cell_number);
                        // kirim message player type sm angka ke game server
                        msg.mesg_type = 1; // Player to game server
                        snprintf(msg.mesg_text, sizeof(msg.mesg_text), "%c%d", player_type, cell_number);
                        if (msgsnd(msgid, &msg, sizeof(msg.mesg_text), 0) == -1){
                            perror("gagal mengirim pesan ke game\n");
                            exit(1);
                        }
                        // nerima pesan cek valid input dari game
                        if (msgrcv(msgid, &msg, sizeof(msg.mesg_text), 2, 0) == -1){
                            perror("gagal menerima cek valid dari game\n");
                            exit(1);
                        }

                        if (strcmp(msg.mesg_text, "invalid") == 0){
                            printf("Wrong input, choose cell number 1-9\n");
                            printf("\n");
                        }
                        else if (strcmp(msg.mesg_text, "filled") == 0){
                            int row = (cell_number - 1) / 3;
                            int col = (cell_number - 1) % 3;
                            printf("Choose other cell, %d is filled by '%c'\n", cell_number, shm->board[row][col]);
                            printf("\n");
                        }
                    } while (strcmp(msg.mesg_text, "valid") != 0);

                    displayBoard(shm->board);

                    if (msgrcv(msgid, &msg, sizeof(msg.mesg_text), 3, 0) == -1){
                        perror("gagal menerima pesan cek game over dari game\n");
                        exit(1);
                    }

                    if (strcmp(msg.mesg_text, "win") == 0){
                        printf("\n---Game Finished!---\n");
                        printf("You Win!\n");
                        shm->lose = true;
                        goto out_of_loops;
                    }

                    if (strcmp(msg.mesg_text, "tie") == 0){
                        printf("\n---Game Finished!---\n");
                        printf("It's a Tie!\n");
                        shm->tie = true;
                        goto out_of_loops;
                    }

                    if (strcmp(msg.mesg_text, "ongoing") == 0){
                        printf("\nPlayer X turn\n");
                        shm->turn = true;
                        continue;
                    }                    
                }

                if (shm->gameEnd == true){
                    int cell_number;
                    shm->board[(cell_number - 1) / 3][(cell_number - 1) % 3] = player_type;
                    displayBoard(shm->board);
                    printf("\n---Game Finished!---\n");
                    if(shm->lose == true){
                        printf("You Lose!\n");
                    }
                    else if(shm->tie == true){
                        printf("It's a Tie!\n");
                    }

                    shm->turn = false;
                    shm->lose = false;
                    shm->tie = false;
                    shm->gameEnd = false;
                    shm->player_X = 0;
                    shm->player_O = 0;
                    memset(shm->board, ' ', sizeof(shm->board));
                    shmdt(shm);
                    break;
                }
            }
            break;
        }
    }
    out_of_loops:
    return 0;
}