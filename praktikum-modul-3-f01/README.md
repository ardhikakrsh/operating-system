[![Review Assignment Due Date](https://classroom.github.com/assets/deadline-readme-button-24ddc0f5d75046c5622901739e7c5dd533143b0c8e959d652212380cedb1ea36.svg)](https://classroom.github.com/a/jYae_yK9)

<div align=center>

|    NRP     |          Name           |
| :--------: | :---------------------: |
| 5025221006 |     Ardhika Krishna     |
| 5025221100 | Riyanda Cavin Sinambela |
| 5025221294 |   Faiq Lidan Baihaqi    |

# Praktikum Modul 3 _(Module 1 Lab Work)_

</div>

### Daftar Soal _(Task List)_

- [Task 1 - Jess No Limit Push Rank](/task-1/)

- [Task 2 - BNBWT](/task-2/)

- [Task 3 - Volume Expansion](/task-3/)

- [Task 4 - WOTA Competition](/task-4/)

### Laporan Resmi Praktikum Modul 1 _(Module 1 Lab Work Report)_

# 🎮 Nomor 1 Jess No Limit Push Rank

## Soal

Jess No Limit sudah siap untuk comeback ke dunia professional esport Mobile Legend. Dia sudah bersusah payah untuk pushrank hingga ke Mythic Immortal dan membuktikan ke dunia bahwa dia adalah pemain yang paling handal. Ketika dia dan timnya kembali latihan, mereka kesulitan untuk menentukan role yang cocok bagi Jess No Limit. Mereka butuh bantuan kalian dalam hal ini, karena hero ML yang begitu banyak pada dataset mobile legend.

Catatan :

- Pada poin c, command yang dijalankan pada docker file tidak terdapat argumen dan akan menampilkan semua role.

### Kode lengkap

```c
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
```

## 1A

Download dataset menggunakan `kaggle datasets download -d irwansyah10010/mobilelegend` setelah berhasil mendownload dengan format file .zip, langkah selanjutnya adalah mengekstrak file tersebut. Kalian melakukannya di dalam file [ayamgoreng.c](./ayamgoreng.c) untuk semua pengerjaannya.

### Langkah penyelesaian

- [ ] Membuat fungsi download dengan perintah dari soal.
- [ ] Membuat fungsi ekstrak file zip.
- [ ] Menggunakan fork agar program berjalan berurutan.
- [ ] Menambahkan fungsi untuk menghapus file zip yang sudah di download sebelumnya.

### Jawaban

Berdasarkan langkah penyelesaian, berikut code yang saya kerjakan :

```c
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
}
```

### Penjelasain lebih detail mengenai code

Penjelasan lebih detailnya sebagai berikut :

1. Membuat fungsi download.

```c
    if (system("kaggle datasets download -d irwansyah10010/mobilelegend") == -1) {
        perror("system");
        return 1;
    }
```

- Menggunakan `systen` untuk mendownload file dari `kaggle`.
- Menggunakan perintah yang diberikan, yaitu `kaggle datasets download -d irwansyah10010/mobilelegend`.

2. Pembuatan fork.

```c
    pid_t child_pid = fork();
    if (child_pid == -1) {
        perror("fork");
        return 1;
    }
```

- Membuat fork untuk menjalankan fungsi berikutnya secara bergantian.
- `perror("fork")` jika gagal maka akan mengeluarkan pesan error.

3. Fungsi untuk mengekstrak file zip.

```c
    if (child_pid == 0) {
        if (execlp("unzip", "unzip", "mobilelegend.zip", "-d", ".", NULL) == -1) {
            perror("execlp");
            return 1;
        }
    }
```

- Menggunakan `execlp` dan perintah `unzip` untuk mengekstrak file zipnya.
- `mobilelegend.zip` adalah nama file zipnya dan `"-d", ".",` menyimpan file hasil eksrak di direktori saat ini.
- `NULL` menandakan akhir bagi argumen.

4. Menambahkan fungsi untuk menghapus file zip yang sudah di download sebelumnya.

```c
else {
        wait(NULL); // Menunggu proses anak selesai

        // Menghapus file zip setelah proses unzip selesai
        if (remove("mobilelegend.zip") != 0) {
            perror("remove");
            return 1;
        }
}
```

- Menggunakan `remove` untuk menghapus file `mobilelegend.zip`.
- `wait(NULL)` untuk menunggu agar proses anak (ekstrak file) selesai dahulu.

### Hasil Output dan SS

- Output di terminal
  ![1 terminal](https://github.com/sisop-its-s24/praktikum-modul-3-f01/assets/144328529/8129ff47-4e2b-4eca-8ef4-b5875220f21e)

- Perubahan di direktori
  ![1 direktori](https://github.com/sisop-its-s24/praktikum-modul-3-f01/assets/144328529/0bcbf114-589f-43fc-803b-745755f3c1a5)

### Kendala

- Tidak ada kendala yang saya alami selama mengerjakan soal a ini.

## 1B

Jess No Limit ingin mengetahui hero apa saja yang terbaik dari setiap role yang ada dengan perhitungan seperti berikut:

|   Role   |        Rumus Skillpoints        |
| :------: | :-----------------------------: |
|   Tank   |               HP                |
| Assassin |         Physical Attack         |
| Fighter  |      HP + Physical Attack       |
| Marksman | Physical Attack \* Attack Speed |
|   Mage   |        Mana / Mana Regen        |
| Support  |         Mana Regen + HP         |

`ayamgoreng.c` harus bisa **mengoutputkan semua hero yang terbaik pada setiap role secara default tanpa argumen**, namun juga bisa menampilkan sesuai dengan **role yang diberikan pada argumen** pada saat pemanggilan program (`./ayamgoreng [role]`). Data hero beserta rolenya dapat dilihat pada file identitas.csv, sedangkan data hero beserta atributnya dapat dilihat pada file atribut-hero.csv. Keluarkan informasi mengenai hero dan skillpointsnya dengan format `Hero terbaik adalah (nama hero) dengan skillpoint (jumlah skillpoint)`.

### Langkah penyelesaian

- [ ] Membaca file `identitas.csv` dan `atribut-hero.csv`.
- [ ] Mengambil header dari kedua file tersebut.
- [ ] Mengambil data nama dan role dari `identitas.csv`.
- [ ] Mengambil data nama hingga mana regen dari dari `atribut-hero.csv`
- [ ] Menggunakan perulangan untuk membaca setiap skillpoints hero.
- [ ] Memasukkan rumus skillpoints untuk setiap role.
- [ ] Mencari skillpoints terbesar dan menyimpannya lalu dioutputkan sesuai output yang diminta pada soal.
- [ ] Menambahkan if jika tidak ada argumen tambahan maka akan mengoutpukan semua role jika ada outputnya hanya sesuai role yang di inputkan.

### Jawaban

Berdasarkan langkah penyelesaian, berikut code yang saya kerjakan :

```c
else {
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
```

### Penjelasain lebih detail mengenai code

Penjelasan lebih detailnya sebagai berikut :

1. Membaca file `identitas.csv` dan `atribut-hero.csv`

```c
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
```

- `fopen` untuk membuka foldernya dan `r` yang berarti read maka, kita akan membaca folder tersebut.
- `file_identitas` definisi dari file `identitas.csv` yang dibaca dan
- `file_atribut` definisi dari file `atribut-hero.csv` yang dibaca.

2. Mengambil header dari kedua file csv

```c
        char line[MAX_LINE_LENGTH];

        fgets(line, MAX_LINE_LENGTH, file_identitas);
        fgets(line, MAX_LINE_LENGTH, file_atribut);
```

- `line` untuk panjang array setiap satu baris.
- `fgets` untuk membaca header atau baris awal dari kedua file csv tersebut.

3. Melakukan loop untuk mengambil atribut setiap hero yang diperlukan untuk perhitungan nanti.

```c
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
```

- Pada `while` tersebut memeriksa apakah fgets() berhasil membaca baris dari file file_atribut. Jika fgets() mengembalikan NULL, itu berarti sudah mencapai akhir file atau terjadi kesalahan, sehingga loop berhenti dan
- memeriksa apakah jumlah hero yang telah dibaca `num_heroes` masih kurang dari jumlah maksimum hero yang diperbolehkan `MAX_HEROES`.
- Setiap atribut akan dijadikan token dengan delimiter `;`.
- Menggunakan `float` agar perhitungan presisi dan untuk merubah string token tersebut menjadi `float` menggunakan `atof`.
- `strcpy` untuk memasukkan string nama kedalam struck `atribut_heroes[num_heroes].nama`.
- Memasukkan atribut lain(hp sampai mana regen) kedalam struct `atribut_heroes`.
- Terakhir menambahkan` num_heroes` agar jika sudah selesai maka loop ini akan berakhir.

4. Mendefinisikan role yang ada dan memeriksa argumen di command line.

```c
        char roles[][MAX_FIELD_LENGTH] = {"TANK", "ASSASSIN", "FIGHTER", "MM", "MAGE", "SUPPORT"};

        char role[MAX_FIELD_LENGTH] = "";
        if (argc > 1) {
            strcpy(role, argv[1]);
            toUpperCase(role);
        }
```

- `roles` digunakan untuk menyimpan daftar semua role yang ada.
- `if (argc > 1)` memeriksa apakah ada argumen yang diberikan ketika program dijalankan melalui command line.
- Jika ada argumen yang diberikan, maka argumen tersebut akan disalin ke dalam variabel `role` menggunakan `strcpy()`.
- Kemudian, fungsi `toUpperCase()` dipanggil untuk mengonversi argumen menjadi huruf besar agar sesuai dengan format yang telah ditentukan dalam array `roles`.

5. Loop untuk memeriksa setiap role yang ada.

```c
        for (int i = 0; i < sizeof(roles) / sizeof(roles[0]); i++) {
            if (argc > 1 && strcmp(role, roles[i]) != 0) {
                continue;
            }

            char hero_terbaik[MAX_FIELD_LENGTH] = "";
            float max_skillpoint = 0.0;

            rewind(file_identitas);
            fgets(line, MAX_LINE_LENGTH, file_identitas);
```

- Loop for digunakan untuk mengiterasi melalui array roles, yang berisi daftar role-role yang ada.
- `if (argc > 1 && strcmp(role, roles[i]) != 0)` mengecek apakah ada argumen lain dan apakah role tersebut sudah sesuai dengan yang ada.
- Jika tidak ada argumen tambahan yang diberikan maka argc > 1 akan bernilai salah, dan program akan melanjutkan untuk mencetak hero terbaik untuk setiap peran yang telah ditentukan dalam array roles.
- varibael `hero_terbaik` adalah array untuk menyimpan hero terbaik.
- variabel `max_skillpoint` bernilai `0.0`.
- rewind(file_identitas) digunakan untuk mengatur posisi pembacaan file kembali ke awal.
- lalu akan dibaca lagi dengan `fgets`.

6. Loop untuk membaca semua baris di `file_identitas` dan mengambil nama dan rolenya.

```c
            while (fgets(line, MAX_LINE_LENGTH, file_identitas) != NULL) {
                char *nama_karakter = strtok(line, ";");
                char *role_karakter = strtok(NULL, ";");
```

7. Mencari hero terbaik.

```c
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
```

- `if (strstr(role_karakter, roles[i]) != NULL)` memeriksa apakah `roles[i]` ada pada `role_karakter`.
- `for (int j = 0; j < num_heroes; j++)` dilakukan loop untuk membaca atribut setiap hero yang ada.
- `if (strcmp(nama_karakter, atribut_heroes[j].nama) == 0)` memeriksa apakah nama yang sedang dieksekusi di `identitas.csv` sama dengan nama di `atribut-hero.csv`.
- Menggunakan `strcmp` untuk menentukan rolenya apa dan melakukan perhitungan skillpointnya.
- `if (skillpoint > max_skillpoint)` jika ada skillpoint yang lebih tinggi maka akan dimasukkan ke variabel `max_skillpoint`.
- `strcpy(hero_terbaik, atribut_heroes[j].nama)` akan memasukkan hero dengan skillpoint tertinggi ke variabel `hero_terbaik`.
- Terakhir akan di outputkan sesuai output yang diminta

### Hasil Output dan SS

- Output dengan perintah `./ayamgoreng` dan `./ayamgoreng tank`
  ![2 terminal](https://github.com/sisop-its-s24/praktikum-modul-3-f01/assets/144328529/642ccf20-d242-4966-931c-0b1588eb8f3a)

- Outpur dengan perintah dengan berbagai role
  ![2 lainlain](https://github.com/sisop-its-s24/praktikum-modul-3-f01/assets/144328529/6f8b3166-6c97-4633-bbc8-6db39a6c7435)

### Kendala

- Kesusahan untuk mengoutputkan semua role jika tidak ada argumen role saat dijalankan.

## 1C

Jess No Limit menyadari bahwa sistem kalian sangat berguna dan ingin sistem ini bisa diakses oleh teman satu timnya. Oleh karena itu, kalian perlu menjadikan sistem yang dibuat ke sebuah Docker Container agar mudah di-distribute dan dijalankan di lingkungan lain tanpa perlu setup environment dari awal. Buatlah Dockerfile yang berisi semua langkah yang diperlukan untuk setup environment dan menentukan bagaimana aplikasi harus dijalankan.

### Langkah penyelesaian

- [ ] Membuat dockerfile.
- [ ] Menginstall kaggle agar dapat mendownload file fi kaggle.
- [ ] Menjalankan program c kita.

### Jawaban

Berdasarkan langkah penyelesaian, berikut code yang saya kerjakan :

```dockerfile
# Gunakan base image yang sesuai
FROM python:3.8

# Install kaggle CLI menggunakan pip
RUN pip install kaggle

# Menyalin kode program C ke dalam image
COPY ayamgoreng.c /usr/src/ayamgoreng.c

# Kompilasi program C
RUN gcc /usr/src/ayamgoreng.c -o /usr/src/ayamgoreng

# Menentukan direktori kerja
WORKDIR /usr/src

# Menjalankan program saat container dijalankan
CMD ["./ayamgoreng"]
```

### Penjelasain lebih detail mengenai code

Penjelasan lebih detail sesuai dengan langkah penyelesaian :

1. `FROM` untuk base image. Disini saya menggunakan python, karena untuk menginstall kaggle menggunakan python.
2. `RUN` pertama, digunakan untuk menginstall kaggle.
3. `COPY` untuk menyalin kode c kita.
4. `RUN` kedua, digunakan untuk menjalankan program c kita.
5. `WORKDIR` menentukan direktori kerja.
6. `CMD` untuk menjalankan program c kita.

### Hasil Output dan SS

- Build images
  ![3 images](https://github.com/sisop-its-s24/praktikum-modul-3-f01/assets/144328529/fe06dc80-95e8-4fa1-88b7-7482d26b8445)

- Docker container
  ![3 container](https://github.com/sisop-its-s24/praktikum-modul-3-f01/assets/144328529/d6c24a64-cf40-4ee6-bb9a-45ffae862ffc)

### Kendala

- Bingung apakah bisa, jika tidak memakai base image python.

## 1D

Setelah sukses membuat sistem berbasis Docker, Jess No Limit merasa bahwa sistem ini tidak hanya berguna untuk dirinya sendiri, tetapi juga akan akan membantu para fansnya yang terpencar di seluruh dunia dalam merekrut pemain berpotensi tinggi. Namun, satu tantangan muncul, yaitu bagaimana caranya para fans dapat mengakses dan menggunakan sistem yang telah diciptakan?

Merasa terpanggil untuk membantu Jess No Limit lebih jauh, kalian memutuskan untuk mem-publish Docker Image sistem ke Docker Hub, sebuah layanan cloud yang memungkinkan kalian untuk membagikan aplikasi Docker kalian ke seluruh dunia. Output dari pekerjaan ini adalah file Docker kalian bisa dilihat secara public pada `https://hub.docker.com/r/{Username}/joki-ml-bang`.

### Langkah penyelesaian

- [ ] Membuat akun docker hub.
- [ ] Membuat repo dengan nama `joki-ml-bang`.
- [ ] Login ke terminal.
- [ ] Tag docker image yang sudah kita buat.
- [ ] Melakukan push ke docker hub.

### Hasil Output dan SS

- Membuat akun docker hub dan repo `joki-ml-bang`
  ![4 dockerhub](https://github.com/sisop-its-s24/praktikum-modul-3-f01/assets/144328529/01742e2a-5c04-4067-b8d0-4b85b71a5faa)

- login, tag image, dan push
  ![4 push](https://github.com/sisop-its-s24/praktikum-modul-3-f01/assets/144328529/7c6a2112-febb-43e6-b79f-3cf60cb24b66)

- Hasil push di docker hub
  ![4 done](https://github.com/sisop-its-s24/praktikum-modul-3-f01/assets/144328529/34d114d0-38d0-4a11-b023-27eb2fc95279)

### Kendala

- Tidak ada kendala yang saya alami selama mengerjakan soal d ini.

## 1E

Melihat betapa banyaknya fans Jess No Limit, kalian diminta untuk menerapkan skala pada layanan menggunakan **Docker Compose** dengan instance sebanyak 6 (untuk setiap role). Pastikan setiap instance memanggil program untuk menampilkan hero terbaik sesuai dengan role yang diberikan pada argumen. Penamaan setiap instance sama dengan role yang akan ditampilkan (instance mage, instance fighter, dst).

### Langkah penyelesaian

- [ ] Membuat docker compose.
- [ ] Membuat service untuk setiap role yang ada.

### Jawaban

Berdasarkan langkah penyelesaian, berikut code yang saya kerjakan :

```yaml
version: "3"

services:
  tank:
    image: fqlidan/joki-ml-bang:tagname
    command: ["./ayamgoreng", "TANK"]

  assassin:
    image: fqlidan/joki-ml-bang:tagname
    command: ["./ayamgoreng", "ASSASSIN"]

  fighter:
    image: fqlidan/joki-ml-bang:tagname
    command: ["./ayamgoreng", "FIGHTER"]

  mm:
    image: fqlidan/joki-ml-bang:tagname
    command: ["./ayamgoreng", "MM"]

  mage:
    image: fqlidan/joki-ml-bang:tagname
    command: ["./ayamgoreng", "MAGE"]

  support:
    image: fqlidan/joki-ml-bang:tagname
    command: ["./ayamgoreng", "SUPPORT"]
```

## Penjelasain lebih detail mengenai code

Penjelasan lebih detail sesuai dengan langkah penyelesaian :

1. Setiap layanan didefinisikan sebagai sebuah blok di bawah kunci `services`.
2. Setiap layanan memiliki nama yang unik, seperti tank, assassin, fighter, dst.
3. Setiap layanan menggunakan gambar Docker yang sama, yang ditentukan oleh kunci `image`.
4. `command` digunakan untuk menentukan perintah yang akan dijalankan ketika container dimulai.
5. `tagname` disesuaikan dengan nama tagnamenya(biasanya latest).

### Hasil Output dan SS

- Output keseluruhan
  ![5 all output](https://github.com/sisop-its-s24/praktikum-modul-3-f01/assets/144328529/f549dea5-ba27-4291-82c2-680f8a4648a5)

- Output dengan role
  ![5 output role](https://github.com/sisop-its-s24/praktikum-modul-3-f01/assets/144328529/029f2b6f-f3a9-4650-8b16-149b2703597f)

### Kendala

- Tidak ada kendala yang saya alami selama mengerjakan soal e ini.

### Revisi Keseluruhan

Tidak ada revisi untuk soal no 1 ini.

# Nomor 2 BNBWT

### Notes:

## 4A

- Buatlah 2 file, yaitu [server.c](./server.c) dan [client.c](./client.c).

  Disaat `server.c` pertama kali dijalankan, dia akan meminta prompt user untuk mengetikkan berapa banyak client yang akan dilayani (range 1-5).

  Disaat `client.c` pertama kali dijalankan, dia akan meminta prompt user untuk mengetikkan username. Username ini kemudian dikirim ke server, untuk di check apakah sudah ada sebelumnya atau tidak. Jika sudah ada, maka akan meminta prompt lagi untuk mengetikkan username sampai diterima oleh server.

### Jawab (server)

server.c:

```
struct client
{
    char username[100];
    int turn;
    int length;
    int total;
};

key_t key_server = 1234, key_user = 5678;
int server_id = shmget(key_server, sizeof(struct client), 0666);
int user_id = shmget(key_user, 5 * sizeof(struct user), 0666);
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
```

### Penjelasan (server)

1. Buat struct yang akan dishare memory kan

   ```
   int server_id = shmget(key_server, sizeof(struct client), 0666);
   struct client *cl = shmat(server_id, NULL, 0);
   ```

2. Set semua member value dari cl menjadi 0
3. Print dan minta input dari user berapa banyak client yang akan dilayani?
4. Looping selama client total dan client length tidak sama
5. Jika valuenya sama maka tampilkan pesan semua client sudah terkumpul

### Jawab (client)

client.c:

```
struct client
{
    char username[100];
    int turn;
    int length;
    int total;
};
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
}
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
                strcpy(usr[cl->length].namanama, cl->username);
                cl->length++;
                myTurn = cl->length;
                break;
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
```

printf("Server akan melayani %d client\n", cl->total);

### Penjelasan (client)

1. Buat struct yang akan dishare memory kan

   ```
   int server_id = shmget(key_server, sizeof(struct client), 0666);
   struct client *cl = shmat(server_id, NULL, 0);
   ```

2. Karena struct udah dishare memory maka buat handler kalau client totalnya masih belum diinputkan maka tampilkan pesan sedang menunggu server

3. Ketika server sudah memberikan input maka dari sisi client akan menampilkan pesan 'server akan melayani n client'

4. Pengecekan value dari client length apakah sama dengan 0 atau tidak

5. Jika nomor 4 bernilai true maka client akan meminta username dan copy value username yang diterima ke dalam struct usr yang memiliki member nama, setelah itu client length diset ke 1

6. Karena client total itu berdasarkan input dari server maka buat handler untuk menunggu client lain jika client total > 1 dan tampilkan pesan menunggu user lain

7. Selama client lengthnya ga sama dengan client total buat clientnya ga ngapa2in

8. Jika nomor 4 tidak bernilai 0 maka masuk ke else dan jalankan code di dalamnya

9. Minta input dari username dan lakukan pengecekan apakah username yang diinputkan sudah ada atau belum, jika ada maka tampilkan username telah dipakai, tapi jika belum ada maka copy username yang baru saja diinputkan

10. Jika myTurn != maka keluar dari loop dan lanjut ke pengecekan berikut (jika client total > client length maka tampilkan pesan menunggu user lain)

11. Loop selama client length != client total

12. Setelah itu tampilkan pesan pemilihan user telah berhasil

### Output

<img src="https://cdn.discordapp.com/attachments/1204803896606855174/1238865464218746942/image.png?ex=6640d6df&is=663f855f&hm=070c159e84bce928c0175b902d3b2f66b6d2c55d0fecdffb8f1bc1749b768448&" width="auto" height="auto">

## 4B

- Ketika semua client yang dibutuhkan oleh server telah terkumpul, setiap client akan mulai diberikan prompt untuk mengetikkan service yang dibutuhkan, yaitu `Nama Container`, `Image yang Digunakan`, `Perintah Saat Kontainer Dimulai`, dan `Volume`. Urutan client yang mengirimkan request service ke server ditentukan dari waktu mereka register. Contoh: Jika username ragnar registrasi pertama kali, lalu diikuti username towel, maka ragnar yang akan mengirim request terlebih dahulu, lalu diikuti towel, dst.

### Jawab

```
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
```

### Penjelasan

1. Buat message queue
2. Loop
3. Cek jika turn == myTurn
4. Jika nomor 3 bernilai true maka minta input dari client mulai dari nama container, nama image, command yang akan dijalankan, dan volume.
5. Set mess turn = myTurn
6. Set mess mess_type = 1
7. Send message yang sudah diterima dari inputan sebelumnya
8. Jika value nomor 3 tidak sama maka tampilkan pesan menunggu client lain
9. Receive message dari server
10. Cek jika turn apakah sama dengan client total
11. Jika nomor 10 bernilai true maka turn = 1 dan continue
12. Setelah itu turn++

### Output

<img src="https://cdn.discordapp.com/attachments/1204803896606855174/1238865464218746942/image.png?ex=6640d6df&is=663f855f&hm=070c159e84bce928c0175b902d3b2f66b6d2c55d0fecdffb8f1bc1749b768448&" width="auto" height="auto">

<img src="https://cdn.discordapp.com/attachments/1204803896606855174/1238865464218746942/image.png?ex=6640d6df&is=663f855f&hm=070c159e84bce928c0175b902d3b2f66b6d2c55d0fecdffb8f1bc1749b768448&" width="auto" height="auto">

## 4C

- Setelah semua request service dari client terkumpul, server lalu menggabungkannya menjadi sebuah file docker compose. Lalu jalankan file docker compose yang dibuat tadi

```
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

```

## 4D

- Tidak sampai situ, setelah docker compose sebelumnya selesai dijalankan, `client.c` akan meminta prompt lagi dari user. Prompt yang diberikan sesuai dengan point (b). Jika setiap client melakukan hal yang sama pada point (b), maka server akan menghasilkan file docker compose yang baru. Lalu container yang telah dijalankan sebelumnya akan dihancurkan, yang kemudian akan digantikan oleh service terbaru yang telah di request dari client tadi.

# 💬 Task-3 Volume Expansion

> ## NOTE

- Untuk soal 3A, 3B, dan 3C code dan outputnya menjadi satu di soal 3D.
  <br>

> ## 3A

- Pada 3A membuat program.c yang dapat menulis dan membaca pesan dimana pesan yang akan ditulis menggunakan multithread yaitu 1 thread mewakili 1 kata.
- ### Logic Penyelesaian
- [ ] Membuat fungsi write
- [ ] Membuat fungsi read
- [ ] Membuat thread berdasarkan jumlah kata
      <br>

> ## 3B

- Pada 3B membuat docker image yang berisi chat.c, compile chat.c, dan beri nama dockermessage

- ### Logic Penyelesaian
- [ ] Menggunakan base image alpine
- [ ] Buat working directory
- [ ] Copy chat.c ke working directory
- [ ] Compile chat.c
- [ ] Jalankan chat.c dengan looping infinite
- [ ] Build docker image
      <br>

> ## 3C

- Pada 3C membuat pengaturan.c yang dapat membuat container baru, menghapus container dari host, dengan docker image yang sudah dibuat menggunakan Dockerfile.

- ### Logic Penyelesaian
- [ ] Membuat fungsi create container
- [ ] Membuat fungsi delete container
- [ ] Membuat fungsi list container
- [ ] Membuat fungsi login container
      <br>

> ## 3D

- Pada 3D memodifikasi chat.c dan ngebuild ulang menjadi docker image `dockermessage:v2`. Modifikasi pengaturan.c agar dapat mengirim chat ke container lain dan tambahan connect dan unconnect container agar container dapat chat dengan container tujuan namun container harus saling connect. Connect dulu agar dapat kirim pesan, lalu ketika unconnect tidak bisa kirim pesan.

- ### Logic Penyelesaian
- [ ] Menambahkan fungsi connect dan unconnect pada pengaturan.c
- [ ] Mendapatkan environment variable di dalam container docker yang sedang dibuat
- [ ] Gunakan share volume pada kedua container untuk mengirim pesan dan membaca pesan
- [ ] Panggil environment variable ke chat.c yang didapatkan dari pengaturan.c untuk membuat file, menghandle beberapa case soal, dan lain-lainnya

- Berikut penjelasan dari code Dockerfile :

```c
FROM alpine:latest

RUN apk add --no-cache gcc musl-dev bash docker-cli

WORKDIR /app

COPY chat.c .

RUN gcc chat.c -o chat

CMD ["sh", "-c", "while true; do ./chat; sleep 10; done"]
```

Gunakan base image alpine. Instal gcc, musl-dev, dan bash untuk kompilasi program C, eksekusi skrip bash, dan instalasi Docker CLI. Set working directory yaitu app. Salin file chat.c ke dalam app. Compile program chat.c menggunakan gcc. Jalankan chat.c dalam loop infinite dengan sleep 10.

- Berikut penjelasan dari beberapa potongan code pengaturan.c :

```c
void create(char *namaContainer) {
    snprintf(command, sizeof(command), "sudo docker run -d --name %s dockermessage:v2", namaContainer);
    system(command);
}
```

Membuat fungsi create yaitu membuat container Docker baru dengan nama yang diberikan `namaContainer` dan menggunakan Docker image `dockermessage:v2`

```c
void delete(char *namaContainer) {
    snprintf(command, sizeof(command), "sudo docker rm -f %s", namaContainer);
    system(command);
}
```

Membuat fungsi delete yaitu menghapus container Docker dengan nama yang diberikan `namaContainer`

```c
void list() {
    system("sudo docker ps -a");
}
```

Membuat fungsi list yaitu untuk menampilkan semua container Docker yang sedang berjalan di host.

```c
void login(char *namaContainer) {
    snprintf(command, sizeof(command), "sudo docker exec -it %s /bin/sh", namaContainer);
    system(command);
}
```

Membuat fungsi login yaitu untuk melakukan eksekusi shell interaktif `/bin/sh` di dalam container Docker yang diberikan `namaContainer`.

```c
void connect(char *sndrContainer, char *rcvrContainer) {
    snprintf(command, sizeof(command), "sudo docker rm -f %s", sndrContainer);
    system(command);
    snprintf(command, sizeof(command), "sudo docker rm -f %s", rcvrContainer);
    system(command);

    system("sudo docker volume create --name messageVol");
    //pasang shared volume pada container yang sudah dibuat di fungsi create
    snprintf(command, sizeof(command), "sudo docker run -e FIRST_CONTAINER=%s -d --name=%s -v messageVol:/app dockermessage:v2", sndrContainer, sndrContainer);
    system(command);
    snprintf(command, sizeof(command), "sudo docker run -e FIRST_CONTAINER=%s -d --name=%s --volumes-from %s dockermessage:v2", rcvrContainer, rcvrContainer, sndrContainer);
    system(command);
}
```

Membuat fungsi connect yaitu dengan menghapus container yang dimasukkan ke dalam fungsi connect, kemudian buat share volume bernama messageVol dengan perintah `sudo docker volume create --name messageVol`. Setelah itu buat ulang container dengan memasang share volume messageVol pada masing-masing container. Ambil variable environment dari masing-masing container dengan perintah `-e FIRST_CONTAINER=%s`.

```c
void unconnect(char *sndrContainer, char *rcvrContainer) {
    snprintf(command, sizeof(command), "sudo docker cp %s:/app/%s.txt /home/ardhikakrsh/Documents/Kuliah/Sisop/Praktikum-3/praktikum-modul-3-f01/task-3", sndrContainer, rcvrContainer);
    system(command);
    snprintf(command, sizeof(command), "sudo docker cp %s:/app/%s.txt /home/ardhikakrsh/Documents/Kuliah/Sisop/Praktikum-3/praktikum-modul-3-f01/task-3", rcvrContainer, sndrContainer);
    system(command);

    //hapus shared volume dari container
    snprintf(command, sizeof(command), "sudo docker rm -f %s", sndrContainer);
    system(command);
    snprintf(command, sizeof(command), "sudo docker rm -f %s", rcvrContainer);
    system(command);

    snprintf(command, sizeof(command), "sudo docker volume rm messageVol");
    system(command);

    snprintf(command, sizeof(command), "sudo docker run -e SECOND_CONTAINER=%s -d --name %s dockermessage:v2", sndrContainer, sndrContainer);
    system(command);
    snprintf(command, sizeof(command), "sudo docker run -e SECOND_CONTAINER=%s -d --name %s dockermessage:v2", rcvrContainer, rcvrContainer);
    system(command);

    sprintf(command, "sudo docker cp /home/ardhikakrsh/Documents/Kuliah/Sisop/Praktikum-3/praktikum-modul-3-f01/task-3/%s.txt %s:/app", sndrContainer, sndrContainer);
    system(command);
    sprintf(command, "sudo docker cp /home/ardhikakrsh/Documents/Kuliah/Sisop/Praktikum-3/praktikum-modul-3-f01/task-3/%s.txt %s:/app", rcvrContainer, rcvrContainer);
    system(command);
}
```

Membuat fungsi unconnect yaitu dengan langkah awal copy kedua file yang ada di shared volume ke direktori task-3, hapus share volume messageVol dengan perintah `sudo docker volume rm messageVol`. Setelah itu membuat container Docker baru dengan nama yang sesuai dan ambil environment variable dengan perintah `-e SECOND_CONTAINER=%s`. Lalu salin file dari host ke container receiver dan sender.

- Berikut penjelasan dari beberapa potongan code chat.c :

```c
void writeMesg(char *rcvrContainer, char *message, char *firstContainer, char *secondContainer) {
    //tulis nama firstContainer, secondContainer, thirdContainer di
    // printf("firstContainer: %s\n", firstContainer);
    // printf("secondContainer: %s\n", secondContainer);
    if (secondContainer == NULL && firstContainer == NULL) {
        printf("Error : Pengguna tidak ditemukan.\n");
        return;
    }

    else if (firstContainer) {
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

        printf("Pesan berhasil dikirim.\n");
    }

    else if (secondContainer) {
        printf("Error : Tidak dapat membuka file untuk penulisan.\n");
        return;
    }
    // Menghitung jumlah kata dalam pesan
}
```

Membuat fungsi write message untuk menulis pesan. Mengecek environment variable untuk menghandle login case pada masing-masing container jika belum ada maka tidak dapat mengirim pesan dan mengirim `Error : Pengguna tidak ditemukan.`. Jika first environment variable ada maka hitung jumlah kata dalam pesan dan membuat threads sebanyak jumlah kata nya. Dilakukan while looping untuk setiap kata akan diproses oleh thread yang dibuat dan masuk ke fungsi wordProcess untuk di cetak ke file.txt. Kemudian jika second environment variable ada maka print `Error : Tidak dapat membuka file untuk penulisan`.

```c
void readMesg(char *firstContainer, char *secondContainer) {
    if (secondContainer == NULL && firstContainer == NULL) {
        printf("Error : Pengguna tidak ditemukan.\n");
        return;
    }

    else if(firstContainer){
        char filename[MAX_WORD_LENGTH * 10];
        sprintf(filename, "%s.txt", firstContainer);
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

    else if(secondContainer){
        char filename[MAX_WORD_LENGTH * 10];
        sprintf(filename, "%s.txt", secondContainer);
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

    else {
        printf("Error : Pengguna tidak ditemukan.\n");
        return;
    }
}
```

Membuat fungsi read message untuk membaca pesan. Mengecek environment variable untuk menghandle login case pada masing-masing container jika belum ada maka tidak dapat mengirim pesan dan mengirim `Error : Pengguna tidak ditemukan.`. Jika first environment variable ada maka membuka sebuah file dari first environtment tersebut dan membaca isi dari pesan tersebut setiap line nya menggunakan while. Jika second environment variable ada maka dilakukan juga seperti case first environment variable.

```c
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
```

Membuat fungsi processWord untuk meproses kata setiap threadnya. Kemudian dibuat sebuah nama file berdasarkan nama rcvrContainer. File tersebut kemudian dibuka dalam mode append ("a") agar dapat menambahkan kata ke dalamnya. Kemudian menulis kata ke dalam rcvrContainer.txt.

> ## Output Code
>
> ![image](https://github.com/sisop-its-s24/praktikum-modul-3-f01/assets/116788471/c3580462-5a9b-46b3-a5d4-141219534d08) > ![image](https://github.com/sisop-its-s24/praktikum-modul-3-f01/assets/116788471/84704463-fa78-463a-82ae-1accc48c54fa) > ![image](https://github.com/sisop-its-s24/praktikum-modul-3-f01/assets/116788471/dae702ac-ba01-442e-bdf0-347b7e0f2f07) > ![image](https://github.com/sisop-its-s24/praktikum-modul-3-f01/assets/116788471/451aee02-a661-45c7-a88e-14e263d84d00) > ![image](https://github.com/sisop-its-s24/praktikum-modul-3-f01/assets/116788471/0664be9b-12ed-49cd-bde7-378c3600a609) > ![image](https://github.com/sisop-its-s24/praktikum-modul-3-f01/assets/116788471/4a665ea4-3200-488b-b20b-a8b9657e6078)

> ## Full code chat.c

```c
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

void writeMesg(char *rcvrContainer, char *message, char *firstContainer, char *secondContainer) {
    //tulis nama firstContainer, secondContainer, thirdContainer di
    // printf("firstContainer: %s\n", firstContainer);
    // printf("secondContainer: %s\n", secondContainer);
    if (secondContainer == NULL && firstContainer == NULL) {
        printf("Error : Pengguna tidak ditemukan.\n");
        return;
    }

    else if (firstContainer) {
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

        printf("Pesan berhasil dikirim.\n");
    }

    else if (secondContainer) {
        printf("Error : Tidak dapat membuka file untuk penulisan.\n");
        return;
    }
    // Menghitung jumlah kata dalam pesan
}

void readMesg(char *firstContainer, char *secondContainer) {
    if (secondContainer == NULL && firstContainer == NULL) {
        printf("Error : Pengguna tidak ditemukan.\n");
        return;
    }

    else if(firstContainer){
        char filename[MAX_WORD_LENGTH * 10];
        sprintf(filename, "%s.txt", firstContainer);
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

    else if(secondContainer){
        char filename[MAX_WORD_LENGTH * 10];
        sprintf(filename, "%s.txt", secondContainer);
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

    else {
        printf("Error : Pengguna tidak ditemukan.\n");
        return;
    }
}

int main(int argc, char *argv[]) {
    char *firstContainer = getenv("FIRST_CONTAINER");
    char *secondContainer = getenv("SECOND_CONTAINER");

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

        writeMesg(rcvrContainer, message, firstContainer, secondContainer);
        char filename[MAX_WORD_LENGTH * 10];
        sprintf(filename, "%s.txt", rcvrContainer);
        FILE *file = fopen(filename, "a");
        if (file == NULL) {
            perror("Error opening file");
            exit(EXIT_FAILURE);
        }

        fprintf(file, "from %s\n", firstContainer);
        fclose(file);
    }

    else if (strcmp(option, "read") == 0 && argc == 2) {
        readMesg(firstContainer, secondContainer);
    }

    else {
        printf("Invalid option!\n");
        exit(EXIT_FAILURE);
    }

    return 0;
}
```

> ## Full code pengaturan.c

```c
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
    //copy file shared
    //hapus container yang sudah dibuat di fungsi create
    snprintf(command, sizeof(command), "sudo docker rm -f %s", sndrContainer);
    system(command);
    snprintf(command, sizeof(command), "sudo docker rm -f %s", rcvrContainer);
    system(command);

    system("sudo docker volume create --name messageVol");
    //pasang shared volume pada container yang sudah dibuat di fungsi create
    snprintf(command, sizeof(command), "sudo docker run -e FIRST_CONTAINER=%s -d --name=%s -v messageVol:/app dockermessage:v2", sndrContainer, sndrContainer);
    system(command);
    snprintf(command, sizeof(command), "sudo docker run -e FIRST_CONTAINER=%s -d --name=%s --volumes-from %s dockermessage:v2", rcvrContainer, rcvrContainer, sndrContainer);
    system(command);
}

void unconnect(char *sndrContainer, char *rcvrContainer) {
    //copy kedua file yang ada di shared volume ke direktori task-3
    snprintf(command, sizeof(command), "sudo docker cp %s:/app/%s.txt /home/ardhikakrsh/Documents/Kuliah/Sisop/Praktikum-3/praktikum-modul-3-f01/task-3", sndrContainer, rcvrContainer);
    system(command);
    snprintf(command, sizeof(command), "sudo docker cp %s:/app/%s.txt /home/ardhikakrsh/Documents/Kuliah/Sisop/Praktikum-3/praktikum-modul-3-f01/task-3", rcvrContainer, sndrContainer);
    system(command);

    //hapus shared volume dari container
    snprintf(command, sizeof(command), "sudo docker rm -f %s", sndrContainer);
    system(command);
    snprintf(command, sizeof(command), "sudo docker rm -f %s", rcvrContainer);
    system(command);

    snprintf(command, sizeof(command), "sudo docker volume rm messageVol");
    system(command);

    snprintf(command, sizeof(command), "sudo docker run -e SECOND_CONTAINER=%s -d --name %s dockermessage:v2", sndrContainer, sndrContainer);
    system(command);
    snprintf(command, sizeof(command), "sudo docker run -e SECOND_CONTAINER=%s -d --name %s dockermessage:v2", rcvrContainer, rcvrContainer);
    system(command);

    sprintf(command, "sudo docker cp /home/ardhikakrsh/Documents/Kuliah/Sisop/Praktikum-3/praktikum-modul-3-f01/task-3/%s.txt %s:/app", sndrContainer, sndrContainer);
    system(command);
    sprintf(command, "sudo docker cp /home/ardhikakrsh/Documents/Kuliah/Sisop/Praktikum-3/praktikum-modul-3-f01/task-3/%s.txt %s:/app", rcvrContainer, rcvrContainer);
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
```

> ## Full code Dockerfile

```c
FROM alpine:latest

RUN apk add --no-cache gcc musl-dev bash docker-cli

WORKDIR /app

COPY chat.c .

RUN gcc chat.c -o chat

CMD ["sh", "-c", "while true; do ./chat; sleep 10; done"]
```

> ## Kendala Pengerjaan

- [ ] Kesusahan saat melakukan connect kedua container agar dapat saling mengirim message dan baca message
- [ ] Bingung menerapkan container dan docker image

> ## Revisi

- Tidak ada revisi untuk task-3

- Tidak ada revisi untuk task-3.

# 📚 Nomor 4 WOTA Competition

## 4A

- a. Akses URL berikut untuk mendapatkan resource yang dibutuhkan, yaitu sebuah aplikasi yang sedang dibuat oleh Shaniz696969!
  https://drive.google.com/file/d/1iMCSmUzlQ1OqIU72o9IGbPMeghFbj_zK/view?usp=sharing

  It’s kinda legit, right? Aneh juga ya dia tidak pakai GitHub atau semacamnya tapi malah pakai Google Drive (kocak). Tapi gapapa lah ya. Nah, tugas kamu adalah untuk membuat image dari aplikasi tersebut!

#### Jawab

Langkah-langkah pengerjaan:

1. Open vscode, drag and drop folder praktikum-modul-3-f01 ke vscode
2. cd ke task-4 dari terminal
3. run command di bawah ini secara berurutan

```
sudo apt update

sudo apt install python3-pip

pip install gdown

gdown https://drive.google.com/uc?id=1iMCSmUzlQ1OqIU72o9IGbPMeghFbj_zK

unzip unnamed-memory.zip

nano Dockerfile
```

4. Penjelasan

Sebelum kita mulai, pastikan Anda telah menginstal Python dan pip3 di sistem Anda. Meskipun sebagian besar distribusi Linux dilengkapi dengan Python yang sudah diinstal sebelumnya, Anda mungkin perlu menginstal pip3 secara terpisah.

Untuk menginstal pip3 di Ubuntu, jalankan perintah berikut:

```
sudo apt update
sudo apt install python3-pip
```

Sekarang kita telah menginstal pip3, kita dapat melanjutkan untuk menginstal gdown. Buka terminal Anda dan gunakan perintah berikut:

```
pip3 install gdown
```

1. Untuk mendownload file dari Google Drive menggunakan gdown, Anda memerlukan ID unik file tersebut
   Untuk mendapatkan idnya tinggal ke ambil dari linknya aja

   <img src="https://media.discordapp.net/attachments/1204803896606855174/1238778539336138873/image.png?ex=664085eb&is=663f346b&hm=aa842cf945667ccb9e022c93d01aa62a86117245e406fc374adb8793824c3763&=&format=webp&quality=lossless&width=529&height=36" width="auto" height="auto">

   mulai setelah d/ sampai sebelum /view (jadi id = 1iMCSmUzlQ1OqIU72o9IGbPMeghFbj_zK)

2. Gunakan gdown untuk Mengunduh File:

   ```
   gdown https://drive.google.com/uc?id=ID_FILE
   ```

3. Paste ke Dockerfile

   ```
   FROM php:8.1.0-apache
   WORKDIR /var/www/html

   RUN a2enmod rewrite

   RUN apt-get update -y && apt-get install -y \
   libicu-dev \
   libmariadb-dev \
   unzip zip \
   zlib1g-dev \
   libpng-dev \
   libjpeg-dev \
   libfreetype6-dev \
   libjpeg62-turbo-dev \
   libpng-dev

   COPY --from=composer:latest /usr/bin/composer /usr/bin/composer

   RUN docker-php-ext-install gettext intl pdo_mysql gd

   RUN docker-php-ext-configure gd --enable-gd --with-freetype --with-jpeg \
   && docker-php-ext-install -j$(nproc) gd
   ```

#### Logic Penyelesaian:

- [ ] Download file
- [ ] Unzip file
- [ ] Buat Dockerfile

#### Kendala:

1. Tidak ada

#### Output:

<img  width="auto" height="auto" src="https://cdn.discordapp.com/attachments/1204803896606855174/1238776117423509504/image.png?ex=664083a9&is=663f3229&hm=76f523600e10ebf8031a37bc1feebc3a33f6c7fc93e20659c04dfa7a759f525d&">

<img  width="auto" height="auto" src="https://media.discordapp.net/attachments/1204803896606855174/1238787632880947302/image.png?ex=66408e63&is=663f3ce3&hm=0bf964cafabef7291e2f57d557b41b75c66d930b6acd2361fd3c6fa88f75b0bf&=&format=webp&quality=lossless&width=1175&height=661">

#### Output (sesudah revisi):

Tidak ada revisi

## 4B,4C,4D

- b. Selanjutnya, karena Shaniz696969 mengatakan bahwa aplikasi ini menggunakan relational database berupa MySQL, mau tidak mau kamu harus membuat container yang menjalankan image dari MySQL! Pastikan container berjalan sebagai daemon.

  Psst, ingat, kamu bisa mendapatkan image MySQL dari Docker Hub.

  c. Kamu ingat bahwa sudah membuat image dari aplikasi Shaniz696969? Nah, sekarang, jalankan image tersebut sebagai container. Pastikan container berjalan sebagai daemon.

  d. Apa lagi yang kurang? Yup, kamu harus melakukan koneksi antara aplikasi backend tersebut dengan database yang telah dijalankan. Tentunya, dalam container yang telah kita jalankan di poin-poin sebelumnya, kan? Nah, lakukan koneksi antara dua container tersebut dalam satu network bernama Shani-net!

  Hint: [belum tersedia] [ada, tapi nanti]

#### Jawab

Langkah-langkah pengerjaan:

1. Open vscode, drag and drop folder praktikum-modul-3-f01 ke vscode
2. cd ke task-4 dari terminal
3. Paste command di bawah ini ke dalam docker-compose.yml

```
services:
  laravel-with-docker:
    container_name: laravel-with-docker
    build: .
    volumes:
      - ./laravel-onion-template:/var/www/html
    ports:
      - 9000:80

  database:
    image: mysql:latest
    environment:
      MYSQL_DATABASE: keyisa
      MYSQL_ROOT_PASSWORD: root
    ports:
      - 3306:3306

  phpmyadmin:
    image: phpmyadmin:latest
    ports:
      - 1000:80
    environment:
      - PMA_ARBITRARY=1
```

4. Penjelasan

buat 2/3 service (phpmyadmin optional, tapi jika ingin melihat database lewat GUI bisa menggunakannya)

    1.  service pertama saya beri nama 'laravel-with-docker' dan nama containernya juga sama

    lalu `build: .`: menginstruksikan Docker untuk membangun image dari Dockerfile yang berada di direktori saat ini (.).

    volumes: menyediakan mounting dari direktori host ke direktori dalam container, dalam hal ini, mengaitkan direktori host laravel-onion-template ke /var/www/html dalam container.

    ports: menyediakan port, dalam hal ini, port 9000 dari host diarahkan ke port 80 dalam container.

    2.  service kedua saya beri nama 'database'

    `image: mysql:latest`: menggunakan image MySQL terbaru dari Docker Hub.

    environment: mengatur variabel lingkungan yang digunakan oleh container MySQL, dalam hal ini, menentukan nama database (MYSQL_DATABASE) dan kata sandi root (MYSQL_ROOT_PASSWORD).

    ports: memetakan port 3306 dari host ke port 3306 dalam container, yang memungkinkan koneksi dari host ke database MySQL.

    3.  service ketiga saya beri nama 'phpmyadmin'

    image: phpmyadmin:latest: Menggunakan image PHPMyAdmin terbaru dari Docker Hub.

    ports: Memetakan port 1000 dari host ke port 80 dalam container, yang memungkinkan akses ke antarmuka web PHPMyAdmin.

    environment: Pengaturan yang disediakan untuk PHPMyAdmin, di sini, PMA_ARBITRARY=1 diatur untuk memungkinkan penggunaan koneksi ke host dan port database yang dapat diubah secara bebas.

5.  Ubah value dari file .env yang ada di folder laravel-onion-template

```
DB_CONNECTION=mysql
DB_HOST=database (sesuaikan dengan nama service)
DB_PORT=3306
DB_DATABASE=keyisa
DB_USERNAME=root
DB_PASSWORD=root
```

6. tambah file .htaccess ke dalam folder l-o-t dan paste

```
RewriteEngine on
RewriteCond %{REQUEST_URI} !^public
RewriteRule ^(.*)$ public/$1 [L]
```

7. Membuat image untuk service yang didefinisikan di dalam konfigurasi Docker Compose

```
docker-compose build --no-cache --force-rm
```

8. Membuat dan memulai container sesuai dengan konfigurasi di dalam file Docker Compose.

```
docker-compose up -d
```

9. Ubah permission dari folder l-o-t

```
chmod -R 777 laravel-onion-template/
```

10. Memperbarui dependensi aplikasi Laravel yang berjalan di dalam container Docker yang disebut laravel-with-docker

#### Kendala:

1. Tidak ada

#### Output:

<img  width="auto" height="auto" src="https://cdn.discordapp.com/attachments/1204803896606855174/1238820523245305878/image.png?ex=6640ad04&is=663f5b84&hm=cc55b6c68c1bcb3f56ebe376bb1d81d26ff20bcf3db52edc59f878872605dd49&">

<img  width="auto" height="auto" src="https://cdn.discordapp.com/attachments/1204803896606855174/1238820626995609690/image.png?ex=6640ad1d&is=663f5b9d&hm=aa16399f63ebda763226fb201b7011d2918d4bd5748a03b6995ca8c1b0b5fc1a&">

<img  width="auto" height="auto" src="https://cdn.discordapp.com/attachments/1204803896606855174/1238820523245305878/image.png?ex=6640ad04&is=663f5b84&hm=cc55b6c68c1bcb3f56ebe376bb1d81d26ff20bcf3db52edc59f878872605dd49&">

<img  width="auto" height="auto" src="https://cdn.discordapp.com/attachments/1204803896606855174/1238823550299934770/image.png?ex=6640afd6&is=663f5e56&hm=c45d3e949866875a8309e61f71b587365ddb7d9bb4767ec30bf5dec4b0f9e396& ">

<img  width="auto" height="auto" src="https://cdn.discordapp.com/attachments/1204803896606855174/1238823411137122324/image.png?ex=6640afb5&is=663f5e35&hm=38c868f0f0e12196d64f8ae93122540309da8a2ceeb4d5d6227018089fa61e93&">

#### Output (sesudah revisi):

Tidak ada revisi

## 4E

e. Terakhir, kamu harus lakukan setup pada aplikasi backend tersebut dengan menggunakan dokumentasi Laravel sebagai panduan kalian! Aplikasi harus bisa diakses melalui http://localhost:9090. Agar lebih legit, kalian bisa melakukan hit pada endpoint /api/province.

#### Jawab

Langkah-langkah pengerjaan:

1. Open vscode, drag and drop folder praktikum-modul-3-f01 ke vscode
2. cd ke task-4 dari terminal
3. Run command di bawah ini secara berurutan

```
docker exec laravel-with-docker bash -c "php artisan migrate"

docker exec laravel-with-docker bash -c "php artisan db:seed"
```

4. Penjelasan

Kedua perintah ini penting dalam proses development dan penerapan aplikasi Laravel. Migrasi membantu dalam mempertahankan struktur database dengan aman, sementara seeder membantu dalam menyediakan data awal yang diperlukan untuk testing dan development.

#### Kendala:

1. Tidak ada

#### Output:

<img  width="auto" height="auto" src="https://cdn.discordapp.com/attachments/1204803896606855174/1238829050986889267/image.png?ex=6640b4f6&is=663f6376&hm=5684decd3512ae16e81b768ab64992e5a6d8c69bc0902b5d5b8ac73aaff1ca6b&">

<img  width="auto" height="auto" src="https://cdn.discordapp.com/attachments/1204803896606855174/1238829267811569664/image.png?ex=6640b529&is=663f63a9&hm=60b8eb01908e0e7e9b730b147d0e438adc57b0cf0131249c6e5951a7bb2ebf93&">

#### Output (sesudah revisi):

Tidak ada revisi
