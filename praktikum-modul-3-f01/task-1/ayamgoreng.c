#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <sys/types.h> 
#include <unistd.h>
#include <sys/wait.h> // Menambahkan header file untuk wait()

#define MAX_LINE_LENGTH 1024
#define MAX_FIELD_LENGTH 256
#define MAX_HEROES 200

// Struktur untuk menyimpan atribut hero
struct AtributHero {
    char nama[MAX_FIELD_LENGTH];
    float hp;
    float hp_regen;
    float physical_attack;
    float physical_defence;
    float attack_speed;
    float attack_speed_pc;
    float mana;
    float mana_regen;
    // Tambahkan atribut lainnya jika diperlukan
};

// Fungsi untuk mengonversi string menjadi huruf besar
void toUpperCase(char *str) {
    for (int i = 0; str[i] != '\0'; i++) {
        str[i] = toupper(str[i]);
    }
}

int main(int argc, char *argv[]) {

    // Mendownload dataset menggunakan kaggle datasets download
    if (system("kaggle datasets download -d irwansyah10010/mobilelegend") == -1) {
        perror("system");
        return 1;
    }

    // Menjalankan perintah unzip untuk mengekstrak file zip
    pid_t child_pid = fork();
    if (child_pid == -1) {
        perror("fork");
        return 1;
    }

    if (child_pid == 0) {
        if (execlp("unzip", "unzip", "mobilelegend.zip", "-d", ".", NULL) == -1) {
            perror("execlp");
            return 1;
        }
    } else {
        wait(NULL); // Menunggu proses anak selesai

        // Menghapus file zip setelah proses unzip selesai
        if (remove("mobilelegend.zip") != 0) {
            perror("remove");
            return 1;
        }

        FILE *file_identitas = fopen("Data Mobile Legend/identitas.csv", "r");
        if (file_identitas == NULL) {
            perror("Failed to open identitas file");
            return 1;
        }

        FILE *file_atribut = fopen("Data Mobile Legend/Atribut/atribut-hero.csv", "r");
        if (file_atribut == NULL) {
            perror("Failed to open atribut-hero file");
            return 1;
        }

        char line[MAX_LINE_LENGTH];

        fgets(line, MAX_LINE_LENGTH, file_identitas);
        fgets(line, MAX_LINE_LENGTH, file_atribut);

        struct AtributHero atribut_heroes[MAX_HEROES];
        int num_heroes = 0;
        while (fgets(line, MAX_LINE_LENGTH, file_atribut) != NULL && num_heroes < MAX_HEROES) {
            char *nama = strtok(line, ";");
            float hp = atof(strtok(NULL, ";"));
            float hp_regen = atof(strtok(NULL, ";"));
            float physical_attack = atof(strtok(NULL, ";"));
            float physical_defence = atof(strtok(NULL, ";"));
            float attack_speed = atof(strtok(NULL, ";"));
            float attack_speed_pc = atof(strtok(NULL, ";"));
            float mana = atof(strtok(NULL, ";"));
            float mana_regen = atof(strtok(NULL, ";"));

            strcpy(atribut_heroes[num_heroes].nama, nama);
            atribut_heroes[num_heroes].hp = hp;
            atribut_heroes[num_heroes].hp_regen = hp_regen;
            atribut_heroes[num_heroes].physical_attack = physical_attack;
            atribut_heroes[num_heroes].physical_defence = physical_defence;
            atribut_heroes[num_heroes].attack_speed = attack_speed;
            atribut_heroes[num_heroes].attack_speed_pc = attack_speed_pc;
            atribut_heroes[num_heroes].mana = mana;
            atribut_heroes[num_heroes].mana_regen = mana_regen;

            num_heroes++;
        }

        char roles[][MAX_FIELD_LENGTH] = {"TANK", "ASSASSIN", "FIGHTER", "MM", "MAGE", "SUPPORT"};

        char role[MAX_FIELD_LENGTH] = "";
        if (argc > 1) {
            strcpy(role, argv[1]);
            toUpperCase(role);
        }

        for (int i = 0; i < sizeof(roles) / sizeof(roles[0]); i++) {
            if (argc > 1 && strcmp(role, roles[i]) != 0) {
                continue;
            }

            char hero_terbaik[MAX_FIELD_LENGTH] = "";
            float max_skillpoint = 0.0;

            rewind(file_identitas);
            fgets(line, MAX_LINE_LENGTH, file_identitas);
            while (fgets(line, MAX_LINE_LENGTH, file_identitas) != NULL) {
                char *nama_karakter = strtok(line, ";");
                char *role_karakter = strtok(NULL, ";");

                if (strstr(role_karakter, roles[i]) != NULL) {
                    for (int j = 0; j < num_heroes; j++) {
                        if (strcmp(nama_karakter, atribut_heroes[j].nama) == 0) {
                            float skillpoint = 0.0;
                            if (strcmp(roles[i], "TANK") == 0) {
                                skillpoint = atribut_heroes[j].hp;
                            } else if (strcmp(roles[i], "ASSASSIN") == 0) {
                                skillpoint = atribut_heroes[j].physical_attack;
                            } else if (strcmp(roles[i], "FIGHTER") == 0) {
                                skillpoint = atribut_heroes[j].hp + atribut_heroes[j].physical_attack;
                            } else if (strcmp(roles[i], "MM") == 0) {
                                skillpoint = atribut_heroes[j].physical_attack * atribut_heroes[j].attack_speed;
                            } else if (strcmp(roles[i], "MAGE") == 0) {
                                skillpoint = atribut_heroes[j].mana / atribut_heroes[j].mana_regen;
                            } else if (strcmp(roles[i], "SUPPORT") == 0) {
                                skillpoint = atribut_heroes[j].mana_regen + atribut_heroes[j].hp;
                            }

                            if (skillpoint > max_skillpoint) {
                                max_skillpoint = skillpoint;
                                strcpy(hero_terbaik, atribut_heroes[j].nama);
                            }
                            break;
                        }
                    }
                }
            }

            printf("Hero terbaik adalah %s dengan skillpoint %.2f\n", hero_terbaik, max_skillpoint);
        }

        fclose(file_identitas);
        fclose(file_atribut);
    }

    return 0;
}