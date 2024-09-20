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
    bool tie;
    bool lose;
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

int isValidInput(char board[3][3], int cell) {
    int row = (cell - 1) / 3;
    int col = (cell - 1) % 3;

    if (cell < 1 || cell > 9) {
        return 1;
    } else if (board[row][col] != ' ') {
        return 2;
    } else {
        return 3;
    }
}

int isGameOver(char board[3][3]) {
    int i;
    // Check rows
    for (i = 0; i < 3; i++) {
        if (board[i][0] != ' ' && board[i][0] == board[i][1] && board[i][1] == board[i][2] && board[i][2] == board[i][0])
            return 1;
    }
    // Check columns
    for (i = 0; i < 3; i++) {
        if (board[0][i] != ' ' && board[0][i] == board[1][i] && board[1][i] == board[2][i] && board[2][i] == board[0][i])
            return 1;
    }
    // Check diagonals
    if (board[0][0] != ' ' && board[0][0] == board[1][1] && board[1][1] == board[2][2] && board[2][2] == board[0][0])
        return 1;
    if (board[0][2] != ' ' && board[0][2] == board[1][1] && board[1][1] == board[2][0] && board [2][0] == board[0][2])
        return 1;
    // Check for draw
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (board[i][j] == ' ')
                return 0; 
        }
    }
    return -1; // Draw
}

int main() {
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

    memset(shm->board, ' ', sizeof(shm->board));

    printf("Game Started\n");

    while (1) {
        if (msgrcv(msgid, &msg, sizeof(msg.mesg_text), 1, 0) == -1) {
            perror("gagal menerima pesan dari player");
            exit(1);
        }

        printf("New Message\n");
        printf("Sender: Player %c\n", msg.mesg_text[0]);
        printf("Message: %s\n", &msg.mesg_text[1]);

        char player_type = msg.mesg_text[0];
        int cell_number = atoi(&msg.mesg_text[1]);

        int cek = isValidInput(shm->board, cell_number);
 
        msg.mesg_type = 2;
        if(cek == 1){
            strcpy(msg.mesg_text, "invalid");
            if (msgsnd(msgid, &msg, sizeof(msg.mesg_text), 0) == -1) {
                perror("gagal mengirim pesan invalid ke player\n");
                exit(1);
            }
            continue;
            
        }
        else if(cek == 2){
            strcpy(msg.mesg_text, "filled");
            if (msgsnd(msgid, &msg, sizeof(msg.mesg_text), 0) == -1) {
                perror("gagal mengirim pesan filled ke player\n");
                exit(1);
            }
            continue;
        }
        else if(cek == 3){
            strcpy(msg.mesg_text, "valid");
            if (msgsnd(msgid, &msg, sizeof(msg.mesg_text), 0) == -1) {
                perror("gagal mengirim pesan valid ke player\n");
                exit(1);
            }
        }
        
        // Update board with player's move
        shm->board[(cell_number - 1) / 3][(cell_number - 1) % 3] = player_type;

        // Check if game is over
        int game_result = isGameOver(shm->board);
        // printf("%d\n", game_result);
        // displayBoard(shm->board);

        msg.mesg_type = 3;
        if(game_result == 1 || game_result == -1){
            printf("\n--- Game Finished! ---\n");
            shm->gameEnd = true;
        }

        if (game_result == 1){
            strcpy(msg.mesg_text, "win");
            if (msgsnd(msgid, &msg, sizeof(msg.mesg_text), 0) == -1) {
                perror("gagal mengirim pesan you win");
                exit(1);
            }
            break;
        }
        else if(game_result == -1){
            strcpy(msg.mesg_text, "tie");
            if (msgsnd(msgid, &msg, sizeof(msg.mesg_text), 0) == -1) {
                perror("gagal mengirim pesan it's a tie");
                exit(1);
            }   
            break;
        }
        else if(game_result == 0){
            strcpy(msg.mesg_text, "ongoing");
            if (msgsnd(msgid, &msg, sizeof(msg.mesg_text), 0) == -1) {
                perror("gagal mengirim pesan on going");
                exit(1);
            }   
            continue;
        }
    }
    return 0;
}