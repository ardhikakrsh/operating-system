[![Review Assignment Due Date](https://classroom.github.com/assets/deadline-readme-button-24ddc0f5d75046c5622901739e7c5dd533143b0c8e959d652212380cedb1ea36.svg)](https://classroom.github.com/a/o3jj1gXA)

<div align=center>

|    NRP     |          Name           |
| :--------: | :---------------------: |
| 5025221006 | Ardhika Krishna Wijaya  |
| 5025221100 | Riyanda Cavin Sinambela |
| 5025221294 |   Faiq Lidan Baihaqi    |

# Praktikum Modul 2 _(Module 2 Lab Work)_

</div>

### Daftar Soal _(Task List)_

- [Task 1 - HORSECHAMP](/task-1/)

- [Task 2 - Tugas Akhir Yuan dan Bubu _(Yuan's and Bubu's Final Project)_](/task-2/)

- [Task 3 - Ramalan Cuaca Rama _(Rama's Weather Forecast)_](/task-3/)

- [Task 4 - Tic Tac Toe](/task-4/)

### Laporan Resmi Praktikum Modul 1 _(Module 1 Lab Work Report)_

# ✨ Nomor 1

### 1A

- Buatlah fungsi **HorseFetcher**, yang bertugas mengambil data kuda dari internet dan menyimpannya dalam file dengan nama horse_1.txt, horse_2.txt, horse_3.txt, dan seterusnya. Kuda dan pemilik kuda dipisahkan dengan “:”, sebagai contoh “Morioh:DJumanto”, DJumanto adalah pemilik dari kuda Morioh

#### Jawab

Langkah-langkah pengerjaan:

1. Open vscode, drag and drop folder praktikum-modul-2-f01 ke vscode
2. Cari folder task-1 lalu edit file horsechamp.c
3. Berikut adalah potongan source codenya :

```
static size_t write_callback(void *ptr, size_t size, size_t nmemb, FILE *stream)
{
    return fwrite(ptr, size, nmemb, stream);
}

int downloadFile(const char *url, const char *filename)
{
    CURL *curl;
    FILE *fp;
    CURLcode res;

    curl = curl_easy_init();
    if (curl)
    {
        fp = fopen(filename, "wb");
        if (fp == NULL)
        {
            fprintf(stderr, "Error opening file.\n");
            return 1;
        }

        curl_easy_setopt(curl, CURLOPT_URL, url);

        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);

        curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);

        res = curl_easy_perform(curl);
        if (res != CURLE_OK)
        {
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
            fclose(fp);
            curl_easy_cleanup(curl);
            return 1;
        }

        curl_easy_cleanup(curl);
        fclose(fp);
    }
    else
    {
        fprintf(stderr, "Error initializing curl.\n");
        return 1;
    }

    printf("File downloaded successfully!\n");

    return 0;
}

int unzipFile(const char *zip_file, const char *extract_dir)
{
    struct stat st;
    if (stat(extract_dir, &st) == 0)
    {
        if (S_ISREG(st.st_mode))
        {
            if (remove(extract_dir) != 0)
            {
                fprintf(stderr, "Error removing existing file: %s\n", extract_dir);
                return 1;
            }
        }
        else if (!S_ISDIR(st.st_mode))
        {
            fprintf(stderr, "Error: %s is not a directory\n", extract_dir);
            return 1;
        }
    }

    if (mkdir(extract_dir, 0777) == -1)
    {
        fprintf(stderr, "Error creating directory\n");
        return 1;
    }

    struct zip *archive = zip_open(zip_file, 0, NULL);
    if (!archive)
    {
        fprintf(stderr, "Error opening zip file\n");
        return 1;
    }

    int num_files = zip_get_num_entries(archive, 0);
    if (num_files < 0)
    {
        fprintf(stderr, "Error getting number of entries\n");
        zip_close(archive);
        return 1;
    }

    for (int i = 0; i < num_files; i++)
    {
        struct zip_stat file_info;
        zip_stat_init(&file_info);
        if (zip_stat_index(archive, i, 0, &file_info) != 0)
        {
            fprintf(stderr, "Error getting file information\n");
            zip_close(archive);
            return 1;
        }

        char *contents = (char *)malloc(file_info.size);
        if (!contents)
        {
            fprintf(stderr, "Error allocating memory for file\n");
            zip_close(archive);
            return 1;
        }

        struct zip_file *file = zip_fopen_index(archive, i, 0);
        if (!file)
        {
            fprintf(stderr, "Error opening file in zip\n");
            zip_close(archive);
            return 1;
        }
        zip_fread(file, contents, file_info.size);
        zip_fclose(file);

        char extract_path[1000];
        snprintf(extract_path, sizeof(extract_path), "%s/%s", extract_dir, file_info.name);
        FILE *extracted_file = fopen(extract_path, "wb");
        if (!extracted_file)
        {
            fprintf(stderr, "Error creating file: %s\n", extract_path);
            perror("fopen");
            zip_close(archive);
            return 1;
        }

        fwrite(contents, sizeof(char), file_info.size, extracted_file);
        fclose(extracted_file);
        free(contents);
    }

    zip_close(archive);

    printf("Files extracted successfully!\n");

    return 0;
}

int HorseFetcher(const char *url, const char *filename, const char *extract_dir)
{
    if (downloadFile(url, filename) != 0)
    {
        fprintf(stderr, "Error downloading file.\n");
        return 1;
    }

    int result = unzipFile(filename, extract_dir);
    if (result != 0)
    {
        fprintf(stderr, "Error extracting files\n");
        return 1;
    }

    return 0;
}

int main()
{
    const char *extract_dir = "horse";
    if (HorseFetcher(URL, FILENAME, extract_dir) != 0)
        return 1;

    return 0;
}
```

7. Penjelasan

```
static size_t write_callback(void *ptr, size_t size, size_t nmemb, FILE *stream)
{
    return fwrite(ptr, size, nmemb, stream);
}
```

function callback di atas untuk menulis data ke file dan menerima parameter dengan variabel ptr(pointer) bertipe void, variabel size bertipe size_t, variabel nmemb bertipe size_t, variabel stream(pointer) bertipe file . Lalu mengembalikan(return) nilai(value) dalam bentuk file yang sudah dituliskan

ptr: sebuah pointer yang merujuk pada data yang akan dituliskan
size: ukuran tiap elemen yang akan dituliskan
nmemb: jumlah elemen yang akan ditulis
stream: sebuah pointer ke objek file dimana data akan ditulis

```
int downloadFile(const char *url, const char *filename)
{
    CURL *curl;
    FILE *fp;
    CURLcode res;

    curl = curl_easy_init();
    if (curl)
    {
        fp = fopen(filename, "wb");
        if (fp == NULL)
        {
            fprintf(stderr, "Error opening file.\n");
            return 1;
        }

        curl_easy_setopt(curl, CURLOPT_URL, url);

        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);

        curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);

        res = curl_easy_perform(curl);
        if (res != CURLE_OK)
        {
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
            fclose(fp);
            curl_easy_cleanup(curl);
            return 1;
        }

        curl_easy_cleanup(curl);
        fclose(fp);
    }
    else
    {
        fprintf(stderr, "Error initializing curl.\n");
        return 1;
    }

    printf("File downloaded successfully!\n");

    return 0;
}
```

function downloadFile menerima parameter dengan variabel url(pointer) bertipe const char, variabel filename(pointer) bertipe const char

```
        CURL *curl; -> deklarasikan sebuah pointer variabel bernama curl bertipe CURL
        FILE *fp; -> deklarasikan pointer variabel bernama fp bertipe FILE
        CURLcode res; -> deklarasikan sebuah pointer variabel bernama res bertipe CURLcode

        curl = curl_easy_init(); -> init curl terlebih dahulu
        if (curl) -> kalau curl bernilai true maka akan mengeksekusi kode di dalam bloknya
        {
            fp = fopen(filename, "wb");
        -> Buka file biner dalam mode tulis.

        if (fp == NULL)
            -> Kalau fp bernilai null maka akan mengeksekusi kode di dalam bloknya(memberitahu kalau terjai error saat membuka file) dan return value 1
        {
            fprintf(stderr, "Error opening file.\n");
            return 1;
        }

        -> curl_easy_setopt() is used to set various options for the libcurl easy handle (curl).

        curl_easy_setopt(curl, CURLOPT_URL, url);
        ->  CURLOPT_URL: mengatur URL dari mana file akan diunduh.

        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
        -> CURLOPT_WRITEFUNCTION: mengatur fungsi callback yang akan dipanggil untuk menulis data yang diterima ke file.
        -> Dalam case ini, disetel ke write_callback.


        curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
        -> CURLOPT_WRITEDATA mengatur file pointer untuk menulis data yang diterima.
        -> Di case ini, diatur ke file pointer fp yang diperoleh dari pembukaan file.
        -> Set file untuk menulis

        res = curl_easy_perform(curl);
        -> curl_easy_perform(curl) melakukan permintaan HTTP dengan opsi yang dikonfigurasi. Return CURL, yang menunjukkan hasil operasi. Jalankan permintaannya

        -> Kalau res tidak sama dengan curl maka tampilkan pesan failed
        if (res != CURLE_OK)
        {
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
            fclose(fp);
            curl_easy_cleanup(curl);
            return 1;
        }

        curl_easy_cleanup(curl);
        -> membersihkan isi dari curl

        fclose(fp);
        -> close file
    }
    else
    // return 1 dan tampilkan error di file
    {
        fprintf(stderr, "Error initializing curl.\n");
        return 1;
    }

    printf("File downloaded successfully!\n");

    return 0; -> return valuenya 0
```

```
int unzipFile(const char *zip_file, const char *extract_dir)
{
    struct stat st;
    if (stat(extract_dir, &st) == 0)
    {
        if (S_ISREG(st.st_mode))
        {
            if (remove(extract_dir) != 0)
            {
                fprintf(stderr, "Error removing existing file: %s\n", extract_dir);
                return 1;
            }
        }
        else if (!S_ISDIR(st.st_mode))
        {
            fprintf(stderr, "Error: %s is not a directory\n", extract_dir);
            return 1;
        }
    }

    if (mkdir(extract_dir, 0777) == -1)
    {
        fprintf(stderr, "Error creating directory\n");
        return 1;
    }

    struct zip *archive = zip_open(zip_file, 0, NULL);
    if (!archive)
    {
        fprintf(stderr, "Error opening zip file\n");
        return 1;
    }

    int num_files = zip_get_num_entries(archive, 0);
    if (num_files < 0)
    {
        fprintf(stderr, "Error getting number of entries\n");
        zip_close(archive);
        return 1;
    }

    for (int i = 0; i < num_files; i++)
    {
        struct zip_stat file_info;
        zip_stat_init(&file_info);
        if (zip_stat_index(archive, i, 0, &file_info) != 0)
        {
            fprintf(stderr, "Error getting file information\n");
            zip_close(archive);
            return 1;
        }

        char *contents = (char *)malloc(file_info.size);
        if (!contents)
        {
            fprintf(stderr, "Error allocating memory for file\n");
            zip_close(archive);
            return 1;
        }

        struct zip_file *file = zip_fopen_index(archive, i, 0);
        if (!file)
        {
            fprintf(stderr, "Error opening file in zip\n");
            zip_close(archive);
            return 1;
        }
        zip_fread(file, contents, file_info.size);
        zip_fclose(file);

        char extract_path[1000];
        snprintf(extract_path, sizeof(extract_path), "%s/%s", extract_dir, file_info.name);
        FILE *extracted_file = fopen(extract_path, "wb");
        if (!extracted_file)
        {
            fprintf(stderr, "Error creating file: %s\n", extract_path);
            perror("fopen");
            zip_close(archive);
            return 1;
        }

        fwrite(contents, sizeof(char), file_info.size, extracted_file);
        fclose(extracted_file);
        free(contents);
    }

    zip_close(archive);

    printf("Files extracted successfully!\n");

    return 0;
}
```

function unzipFile menerima parameter dengan variabel zip_file(pointer) bertipe const char, variabel extract_dir(pointer) bertipe const char

```
    struct stat st -> deklarasikan struct bertipe stat bernama st. struct ini digunakan untuk menyimpan informasi tentang file atau direktori yang ditentukan oleh ekstrak_dir.

    -> Ngecek apakah ada merupakan file biasa, jika iya maka hapus file tersebut
    if (S_ISREG(st.st_mode))
    {
        if (remove(extract_dir) != 0)
        {
            fprintf(stderr, "Error removing existing file: %s\n", extract_dir);
            return 1;
        }
    }
    -> Jika st ada tetapi bukan direktori, cetak error
    else if (!S_ISDIR(st.st_mode))
    {
        fprintf(stderr, "Error: %s is not a directory\n", extract_dir);
        return 1;
    }

    -> Buat extract_dir dengan full permissions (0777)
    if (mkdir(extract_dir, 0777) == -1)
    {
        fprintf(stderr, "Error creating directory\n");
        return 1;
    }

    -> Buka archive
    struct zip \*archive = zip_open(zip_file, 0, NULL);
    -> Jika archieve bernilai false maka cetak error
    if (!archive)
    {
        fprintf(stderr, "Error opening zip file\n");
        return 1;
    }

    -> zip_get_num_entries = Dapatkan jumlah file dalam arsip zip lalu diassign ke var num_files
    int num_files = zip_get_num_entries(archive, 0);
    -> jika num_files < 0 maka cetak error, tutup archie, dan return 1
    if (num_files < 0)
    {
        fprintf(stderr, "Error getting number of entries\n");
        zip_close(archive);
        return 1;
    }

    -> Ekstrak setiap file dari arsip
    for (int i = 0; i < num_files; i++)
    {
        struct zip_stat file_info;
        -> deklarasi struct bertipe zip_stat yang akan digunakan untuk menyimpan informasi tentang file dalam arsip zip, seperti ukurannya, waktu modifikasi, dan atribut lainnya.

        zip_stat_init(&file_info);
        -> menginisialisasi struktur atau objek yang berkaitan dengan pengelolaan file dalam format ZIP.


        if (zip_stat_index(archive, i, 0, &file_info) != 0)
        -> zip_stat_index adalah fungsi yang disediakan oleh library libzip.
        Ini mengambil informasi tentang file dalam arsip zip.
        {
            fprintf(stderr, "Error getting file information\n");
            zip_close(archive);
            return 1;
        }

        -> alokasi memori untuk content
        char *contents = (char *)malloc(file_info.size);

        -> jika bernilai false cetak error, close archive, dan return value 1
        if (!contents)
        {
            fprintf(stderr, "Error allocating memory for file\n");
            zip_close(archive);
            return 1;
        }

        struct zip_file *file = zip_fopen_index(archive, i, 0);
        -> Ini membuka file dalam arsip zip berdasarkan indeksnya.

        -> Jika bernilai false maka cetak error, close archieve, dan return 1
        if (!file)
        {
            fprintf(stderr, "Error opening file in zip\n");
            zip_close(archive);
            return 1;
        }

        -> baca file zip
        zip_fread(file, contents, file_info.size);

        -> tutup file zip
        zip_fclose(file);

        char extract_path[1000];
        snprintf(extract_path, sizeof(extract_path), "%s/%s", extract_dir, file_info.name);
        -> snprintf adalah fungsi dalam C yang memformat dan menyimpan serangkaian karakter ke dalam buffer. Cara kerjanya mirip dengan printf, tetapi alih-alih mencetak ke keluaran standar, ia menulis ke buffer string.

        -> buka file dengan mode write dan binary
        FILE *extracted_file = fopen(extract_path, "wb");
        if (!extracted_file)
        {
            fprintf(stderr, "Error creating file: %s\n", extract_path);
            perror("fopen");
            zip_close(archive);
            return 1;
        }

        -> tulis konten ke file
        fwrite(contents, sizeof(char), file_info.size, extracted_file);

        -> tutup file
        fclose(extracted_file);

        -> bebasin memori yang sudah dialokasi sebelumnya
        free(contents);
    }

    zip_close(archive);
    -> tutup zip
```

```
int HorseFetcher(const char *url, const char *filename, const char *extract_dir)
{
    if (downloadFile(url, filename) != 0)
    {
        fprintf(stderr, "Error downloading file.\n");
        return 1;
    }

    int result = unzipFile(filename, extract_dir);
    if (result != 0)
    {
        fprintf(stderr, "Error extracting files\n");
        return 1;
    }

    return 0;
}
```

function HorseFetcher menerima 3 parameter dengan nama url(pointer), filename(pointer), extract_dir(pointer) dan bertipe const char

```
    karena setiap function yang saya buat selalu return 0 maka function tersebut akan dipanggil kecuali ada error maka akan return 1

    code di bawah akan memanggil function downloadFile
    if (downloadFile(url, filename) != 0)
    {
        fprintf(stderr, "Error downloading file.\n");
        return 1;
    }

    return value dari function unzipFile akan disimpan ke result dan memanggil function tersebut
    int result = unzipFile(filename, extract_dir);
    if (result != 0)
    {
        fprintf(stderr, "Error extracting files\n");
        return 1;
    }
```

#### Logic Penyelesaian:

- [ ] Buat function HorseFetcher yang akan melakukan tugas download dan unzip file, setelah itu buat function tersendiri dan dipanggil dari function HorseFetcher
- [ ] SIsanya ada di bagian penjelasan karena saya rasa sudah lengkap

#### Kendala:

1. Karena telat buka discord jadi ga make command system
2. Tidak ada revisi

#### Output:

<img  width="auto" height="auto" src="https://cdn.discordapp.com/attachments/1210251261450919959/1234144388075229285/image.png?ex=662faa07&is=662e5887&hm=32b5501eca988a82d79a4191159e7f92c13525ccaa8556435ee390e47c6fe190&">

#### Output (sesudah revisi):

Tidak ada revisi

### 1B

- Buatlah fungsi **HorseRaceHooray**, yang bertugas melaksanakan balapan kuda. Parameter yang diguakan adalah salah satu file kuda yang telah kalian fetch sebelumnya. Gunakan thread untuk menjalankan perlombaan tersebut. Setiap kuda akan melakukan perhitungan bilangan prima dengan angka acak antara 1 sampai 4000. Kuda yang menyelesaikan perhitungan lebih awal akan menempati posisi lebih tinggi dari kuda lainnya. Setelah perlombaan selesai, simpan hasil lomba dalam format HorseRace\_(nomor lomba).txt.

#### Jawab

Langkah-langkah pengerjaan:

1. Open vscode, drag and drop folder praktikum-modul-2-f01 ke vscode
2. Cari folder task-1 lalu edit file horsechamp.c
3. Dalam pengerjaan digunakan beberapa command dari modul yang dijarkan yaitu thread. Berikut adalah potongan source codenya :

```
typedef struct
{
    char name[50];
    char owner[50];
    int position;
    int number_used;
    int is_prime;
} Horse;

int is_prime(int n)
{
    if (n <= 1)
        return 0;
    if (n <= 3)
        return 1;
    if (n % 2 == 0 || n % 3 == 0)
        return 0;
    for (int i = 5; i * i <= n; i += 6)
    {
        if (n % i == 0 || n % (i + 2) == 0)
            return 0;
    }
    return 1;
}

void *horse_race(void *arg)
{
    Horse *horse = (Horse *)arg;
    horse->number_used = rand() % 4000 + 1;
    horse->is_prime = is_prime(horse->number_used);
    horse->position = horse->is_prime ? 1 : 0;
    pthread_exit(NULL);
}

void HorseRaceHooray(const char *filename, int race_number)
{
    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
        perror("Error opening file");
        return;
    }

    Horse horses[MAX_HORSES];
    int num_horses = 0;

    char line[MAX_LINE_LENGTH];
    while (fgets(line, sizeof(line), file) != NULL && num_horses < MAX_HORSES)
    {
        sscanf(line, "%[^:]:%[^:]", horses[num_horses].name, horses[num_horses].owner);
        num_horses++;
    }

    fclose(file);

    pthread_t threads[MAX_HORSES];
    srand(time(NULL));
    printf("----Horse Race (Race number %d)------\n", race_number);
    for (int i = 0; i < num_horses; i++)
    {
        pthread_create(&threads[i], NULL, horse_race, &horses[i]);
    }

    for (int i = 0; i < num_horses; i++)
    {
        pthread_join(threads[i], NULL);
    }

    for (int i = 0; i < num_horses - 1; i++)
    {
        for (int j = 0; j < num_horses - i - 1; j++)
        {
            if (horses[j].position < horses[j + 1].position)
            {
                Horse temp = horses[j];
                horses[j] = horses[j + 1];
                horses[j + 1] = temp;
            }
        }
    }

    char output_filename[50];
    sprintf(output_filename, "HorseRace_%d.txt", race_number);
    FILE *output_file = fopen(output_filename, "w");
    if (output_file == NULL)
    {
        perror("Error creating output file");
        return;
    }

    time_t rawtime;
    struct tm *timeinfo;
    time(&rawtime);
    timeinfo = localtime(&rawtime);

    fprintf(output_file, "----Horse Race (Race number %d)------\n", race_number);
    fprintf(output_file, "Date: %02d/%02d/%d\n", timeinfo->tm_mday, timeinfo->tm_mon + 1, timeinfo->tm_year + 1900);
    fprintf(output_file, "Time: %02d:%02d:%02d\n\n", timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);
    for (int i = 0; i < num_horses; i++)
    {
        fprintf(output_file, "%d. %s: %s:%d\n", i + 1, horses[i].name, horses[i].owner, horses[i].number_used);
    }

    fclose(output_file);

    printf("Race results saved to %s\n", output_filename);
}
```

3. Penjelasan

```
buat struct Horse yang mempunya member name dan owner yang bertipe char, position, number_used, dan is_prime yang bertipe int
typedef struct
{
    char name[50];
    char owner[50];
    int position;
    int number_used;
    int is_prime;
} Horse;

buat function is_prime untuk cek apakah sebuah bilangan adalah bil prima atau bukan
int is_prime(int n)
{
    if (n <= 1)
        return 0;
    if (n <= 3)
        return 1;
    if (n % 2 == 0 || n % 3 == 0)
        return 0;
    for (int i = 5; i * i <= n; i += 6)
    {
        if (n % i == 0 || n % (i + 2) == 0)
            return 0;
    }
    return 1;
}

-> Function untuk simulasi balapan kuda
void *horse_race(void *arg)
{
    Horse *horse = (Horse *)arg;
    horse->number_used = rand() % 4000 + 1;
    -> Hasilkan angka acak antara 1 dan 4000
    horse->is_prime = is_prime(horse->number_used);
    -> Tentukan apakah bilangan tersebut prima
    horse->position = horse->is_prime ? 1 : 0;
    -> Menentukan posisi kuda berdasarkan perhitungan prima
    pthread_exit(NULL);
    -> digunakan untuk menunjukkan bahwa thread telah selesai tanpa mengembalikan nilai apa pun.
}

-> buat function HorseRaceHooray yang berfungsi untuk melakukan balapan kuda
void HorseRaceHooray(const char *filename, int race_number)
{
    -> buka file untuk membaca
    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
        perror("Error opening file");
        return;
    }

    -> variabel untuk menyimpan data kuda
    Horse horses[MAX_HORSES];
    int num_horses = 0;

    -> baca setiap baris dari file dan parsing data kuda
    char line[MAX_LINE_LENGTH];
    while (fgets(line, sizeof(line), file) != NULL && num_horses < MAX_HORSES)
    {
        sscanf(line, "%[^:]:%[^:]", horses[num_horses].name, horses[num_horses].owner);
        num_horses++;
    }

    -> tutup file
    fclose(file);

    -> lakukan balap kuda menggunakan thread
    pthread_t threads[MAX_HORSES];
    srand(time(NULL));
    -> mengeluarkan nilai acak
    printf("----Horse Race (Race number %d)------\n", race_number);
    -> buat thread sebanyak num_horses
    for (int i = 0; i < num_horses; i++)
    {
        pthread_create(&threads[i], NULL, horse_race, &horses[i]);
    }

    -> ketika thread di atas selesai, akan ada thread baru yang join
    for (int i = 0; i < num_horses; i++)
    {
        pthread_join(threads[i], NULL);
    }

    -> urutkan kuda berdasarkan posisinya
    for (int i = 0; i < num_horses - 1; i++)
    {
        for (int j = 0; j < num_horses - i - 1; j++)
        {
            if (horses[j].position < horses[j + 1].position)
            {
                Horse temp = horses[j];
                horses[j] = horses[j + 1];
                horses[j + 1] = temp;
            }
        }
    }

    -> simpan hasil lomba ke file
    char output_filename[50];
    sprintf(output_filename, "HorseRace_%d.txt", race_number);
    FILE *output_file = fopen(output_filename, "w");
    if (output_file == NULL)
    {
        perror("Error creating output file");
        return;
    }

    // dapatkan tanggal dan waktu sekarang
    time_t rawtime;
    struct tm *timeinfo;
    time(&rawtime);
    timeinfo = localtime(&rawtime);

    -> catat informasi balapan ke file output
    fprintf(output_file, "----Horse Race (Race number %d)------\n", race_number);
    fprintf(output_file, "Date: %02d/%02d/%d\n", timeinfo->tm_mday, timeinfo->tm_mon + 1, timeinfo->tm_year + 1900);
    fprintf(output_file, "Time: %02d:%02d:%02d\n\n", timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);
    for (int i = 0; i < num_horses; i++)
    {
        fprintf(output_file, "%d. %s: %s:%d\n", i + 1, horses[i].name, horses[i].owner, horses[i].number_used);
    }

    -> tutup file
    fclose(output_file);

    printf("Race results saved to %s\n", output_filename);
}

```

#### Logic Penyelesaian

- [ ] Buat function HorseRaceHooray, is_prime, horse_race, struct Horse, dan thread
- [ ] Sisanya ada di bagian penjelasan karena saya rasa sudah lengkap

#### Kendala

1. Tidak ada revisi

#### Output

<img  width="auto" height="auto" src="https://cdn.discordapp.com/attachments/1210251261450919959/1234144388075229285/image.png?ex=662faa07&is=662e5887&hm=32b5501eca988a82d79a4191159e7f92c13525ccaa8556435ee390e47c6fe190&">

### 1C

- Karena ada aturan baru dimana 1 mahasiswa hanya boleh submit 1 proposal, maka komandan juga meminta Bubu untuk memberikan list mahasiswa yang mengajukan 2 proposal. Tampilkan data pembimbingnya karena ingin di kontak komandan.

#### Jawab

1. Open vscode, drag and drop folder praktikum-modul-2-f01 ke vscode
2. Cari folder task-1 lalu edit file horsechamp.c
3. Berikut adalah potongan source codenya :

```
void HorseChampionLeaderboard()
{
    char filename[MAX_FILENAME_LEN];
    char line[MAX_LINE_LENGTH];
    int selected_race;
    FILE *file;
    DIR *dir;
    struct dirent *ent;
    int max_races = 0;

    dir = opendir(".");
    if (dir == NULL)
    {
        printf("Error opening directory\n");
        return;
    }

    while ((ent = readdir(dir)) != NULL)
    {
        if (strstr(ent->d_name, "HorseRace_") != NULL && strstr(ent->d_name, ".txt") != NULL)
        {
            max_races++;
        }
    }

    closedir(dir);

    if (max_races == 0)
    {
        printf("No race history available\n");
        return;
    }

    printf("Choose your Horse Race history:\n");
    for (int i = 1; i <= max_races; i++)
    {
        printf("Horse Race %d\n", i);
    }

    printf("Enter the number of the race you want to see: ");
    scanf("%d", &selected_race);

    if (selected_race < 1 || selected_race > max_races)
    {
        printf("Invalid race number\n");
        return;
    }

    snprintf(filename, MAX_FILENAME_LEN, "HorseRace_%d.txt", selected_race);

    file = fopen(filename, "r");
    if (file == NULL)
    {
        printf("Error opening file\n");
        return;
    }

    while (fgets(line, sizeof(line), file) != NULL)
    {
        printf("%s", line);
    }

    fclose(file);
}
```

3. Penjelasan

```
    -> buka directory saat ini, jika dir bernilai null maka cetak error dan langsung keluar dari function tersebut
    dir = opendir(".");
    if (dir == NULL)
    {
        printf("Error opening directory\n");
        return;
    }

    -> menghitung jumlah file balap kuda yang ada di directory saat ini
    while ((ent = readdir(dir)) != NULL)
    {
        if (strstr(ent->d_name, "HorseRace_") != NULL && strstr(ent->d_name, ".txt") != NULL)
        {
            max_races++;
        }
    }

    closedir(dir);
    -> tutup directory

    -> cek apakah max_races = 0, jika iya cetak error
    if (max_races == 0)
    {
        printf("No race history available\n");
        return;
    }

    -> tampilkan balap kuda yang ada dari directory saat ini
    printf("Choose your Horse Race history:\n");
    for (int i = 1; i <= max_races; i++)
    {
        printf("Horse Race %d\n", i);
    }

    -> minta user untuk memilih balap kuda mana yang mau diliat
    printf("Enter the number of the race you want to see: ");
    scanf("%d", &selected_race);


    -> misal balap kuda yang diinputkan user < 1 atau > max_races yang ada maka tampilkan error
    if (selected_race < 1 || selected_race > max_races)
    {
        printf("Invalid race number\n");
        return;
    }

    snprintf(filename, MAX_FILENAME_LEN, "HorseRace_%d.txt", selected_race);
    -> snprintf adalah fungsi dalam C yang memformat dan menyimpan serangkaian karakter ke dalam buffer. Cara kerjanya mirip dengan printf, tetapi alih-alih mencetak ke keluaran standar, ia menulis ke buffer string.

    -> buka filenya untuk baca
    file = fopen(filename, "r");
    if (file == NULL)
    {
        printf("Error opening file\n");
        return;
    }

    -> cetak hasil balapan kuda
    while (fgets(line, sizeof(line), file) != NULL)
    {
        printf("%s", line);
    }

    -> terakhir tutup file
    fclose(file);
```

#### Logic Penyelesaian:

- [ ] Buat function HorseChampionLeaderboard dan panggil di main
- [ ] Sisanya ada di penjelasan code

#### Kendala:

1. Tidak ada revisi

#### Output:

<img  width="auto" height="auto" src="https://cdn.discordapp.com/attachments/1210251261450919959/1234144388075229285/image.png?ex=662faa07&is=662e5887&hm=32b5501eca988a82d79a4191159e7f92c13525ccaa8556435ee390e47c6fe190&">

## 🖼️ Nomor 2 Yuan's and Bubu's Final Project

### Soal

Yuan dan Bubu ditugaskan untuk mengunduh gambar oleh dosennya, namun dengan beberapa tantangan. Mereka diberikan satu folder yang berisikan file random. Dimana pada folder tersebut, terdapat beberapa file yang berisi petunjuk terkait gambar yang harus diunduh oleh Yuan dan Bubu.

Catatan :

- **DILARANG MENGGUNAKAN SYSTEM() PADA SOAL INI.**

- Gunakan `exec()` dan `fork()` untuk melakukan semua proses di atas.

- Program task dijalankan satu kali untuk seluruh poin di atas.

- Gambar yang diunduh setiap task merupakan gambar random namun harus sesuai dengan kriteria mulai dari
  jumlah gambar, ukuran, dan kategori gambar.

- Poin d merupakan jumlah gambar setiap kategori dari seluruh task, tidak diperbolehkan mengoutputkan
  kategori yang sudah ditulis sebelumnya.

### Main Program

```c
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
```

> Menggunkan fork agar setiap fungsi yang mengerjakan setiap subtask akan berjalan berurutan.

### 2A

Pertama-tama, mereka harus mengunduh sebuah file zip dari [link berikut](https://dl.dropboxusercontent.com/scl/fi/gmy0qvbysh3h7vdexso9k/task_sisop.zip?rlkey=ism9qfjh82awg2tbtzfbrylg4&dl=0), kemudian mengekstraknya dan menghapus file zip tersebut. Setelah itu, mereka harus membuat folder baru dengan nama “task” dan memindahkan file yang berkaitan dengan petunjuk ke dalam folder “task”. File yang berkaitan memiliki penamaan dengan awalan "**task\_{id}**" dan berekstensi **txt**. Lalu hapus folder hasil zip sebelumya.

#### Langkah penyelesaian

- [ ] Menggunakan fork(1) yang child prosesnya membuat folder `extract` untuk menyimpan file-file yang diekstrak dari file zip yang akan di download menggunkan exec setelah folder tersebut terbuat.
- [ ] Lalu membuat fork(2) didalam parent fork(1).
- [ ] Child prosesnya melakukan unzip dan memindahkannya ke folder `extract`.
- [ ] Parent prosesnya prosesnya menghapus file zip dan membuat folder `task`.
- [ ] Melakukan fork(3) di dalam parent fork(2).
- [ ] Child prosesnya mencari file dengan format awalan "**task\_{id}**" dan berekstensi **txt** lalu memindahkannya ke folder `task`.
- [ ] Parent prosesnya menghapus folder `extract`.

### Jawaban

Berdasarkan langkah penyelesaian, berikut code yang saya kerjakan :

```c
#define ZIP_FILE_URL "https://dl.dropboxusercontent.com/scl/fi/gmy0qvbysh3h7vdexso9k/task_sisop.zip?rlkey=ism9qfjh82awg2tbtzfbrylg4&dl=0"
#define ZIP_FILE_NAME "tasksisop.zip"
#define EXTRACT_FOLDER "extract"
#define TASK_DIRECTORY "/home/faiq/Documents/Kuliah/Sisop/Praktikum_2/praktikum-modul-2-f01/task-2/task"

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
```

### Penjelasain lebih detail mengenai code

Penjelasan lebih detail sesuai dengan langkah penyelesaian :

1. Pembuatan fork pertama

```c
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
```

- Disini saya membuat parent id yang bernama `pid` lalu kita lakukan fork().Setelah itu membuat variabel `status` untuk menentukan apakah proses anak berhasil atau gagal.
- `pid == 0` yang menandakan child proses.
- `mkdir(EXTRACT_FOLDER, 0700) == -1` untuk membuat folder `extract`.
- `errno != EEXIST` untuk mengecek apakah folder `extract` sudah ada atau belum.
- `perror` jika terjadi kesalahan akan memunculkan pesan di dalamnya.
- `execlp("wget", "wget", "-O", ZIP_FILE_NAME, ZIP_FILE_URL, NULL)` menggunakan `wget` untuk mendowload, `-O` untuk menyimpan hasil unduhan ke file yang ditentukan, `ZIP_FILE_NAME` file untuk menyimpan hasil unduhan dan `ZIP_FILE_URL` yang merupakan link download.

2. Pembuatn fork kedua

```c
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
```

- `pid > 0` menandakan di parent proses.
- `wait(&status)` menunggu sampai child proses selesai.
- `WIFEXITED(status) && WEXITSTATUS(status) == 0` mengecek apakah proses di child berhasil atau gagal.
- Lalu saya membuat `pid2` untuk melakukan fork(2) lagi didalam parant proses fork(1).
- `execlp("unzip", "unzip", ZIP_FILE_NAME, "-d", EXTRACT_FOLDER, NULL)` untuk melakukan extract file menggunakan `unzip`, `-d` untuk mentukan tempat ekstraksi file, dan `EXTRACT_FOLDER` adalah tempatnya.
- `exit(EXIT_FAILURE)` untuk mengakhiri proses secara paksa jika terjadi kegagalan.

3. Parent proses fork kedua

```c
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
```

- `remove(ZIP_FILE_NAME) == 0` untuk menghapus file zip.
- `mkdir(TASK_DIRECTORY, 0700) == -1 && errno != EEXIST` membuat folder `task` dan mengecek apakah sebelumnya sudah ada.

4. Pembuatan fork ketiga

```c

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
                    }
```

- Di sini saya membuat `pid3` untuk melakukan fork(3) didalam parent fork(2).
- `find` digunakan untuk mencari file dan direktori di dalam sistem file.
- `EXTRACT_FOLDER` merupakan direktori tempat pencarian akan dilakukan.
- `-type` mencari berdasarkan tipe entitas dan `f` yang menentukan entitasnya berupa file.
- `-name` mencari berdasarkan nama dengan format `task_*.txt`.
- `-exec` untuk menjalankan perintah lain bersamaan.
- `mv` untuk memindahkan file yang ditemukan.
- `{}` penanda yang akan digantikan dengan nama setiap file yang ditemukan.

5. Sisanya hanya berupa pesan error.

### Hasil Output dan SS

- Output di terminal
  ![1 Terminal](https://github.com/sisop-its-s24/praktikum-modul-2-f01/assets/144328529/4bdb6b40-7982-4a8e-982f-5224c54ea658)

- Perubahan di direktori
  ![1 direktori](https://github.com/sisop-its-s24/praktikum-modul-2-f01/assets/144328529/f51acdd2-4870-44ba-9505-3408cd3a3297)

### 2B

Yuan dan Bubu membagi tugas secara efisien dengan mengerjakannya secara bersamaan **(overlapping)** dan membaginya sama banyak. Yuan akan mengerjakan task dari awal, sementara Bubu dari akhir. Misalnya, jika ada 20 task, Yuan akan memulai dari task0-task1-dst dan Bubu akan memulai dari task19-task18-dst. Lalu buatlah file “recap.txt” yang menyimpan log setiap kali mereka selesai melakukan task (kriteria setiap task akan dijelaskan di poin c)

#### Langkah penyelesaian

- [ ] Membuat sebuah fungsi yang menerima `recap.txt`, task berapa, jumlah, kategori, dan resolusi. Fungsi tersebut membuka `recap.txt` dan menuliskan pengerjaan yuan dan bubu.
- [ ] Membuat dua fungsi `yuantask` dan `bubutask` yang membuka folder task, membuka task.txt yang akan dikerjakan dan mengambil informasi didalam task.txt tersebut lalu memberikannya ke fungsi `logCompletion`.
- [ ] `yuantask` membuka `task_0 - task_9` dan `bubutask` membuka `task_19 - task_10`.
- [ ] Membuat fungsi yang akan menjalan `yuantask` dan `bubutask`secara bersamaan.

### Jawaban

Berdasarkan langkah penyelesaian, berikut code yang saya kerjakan :

```c
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
```

### Penjelasain lebih detail mengenai code

Penjelasan lebih detail sesuai dengan langkah penyelesaian :

1. Fungsi `logCompletion`

```c
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
```

- `FILE *file = fopen(filename, "a")` membuka file `recap.txt` diberikan dalam mode "a" (append), yang berarti data akan ditambahkan ke akhir file.
- Variabel `current_time` bertipe `time_t`, yang merupakan tipe data integral yang digunakan untuk menyimpan waktu dalam bentuk detik
- Variabel pointer `local_time` digunakan untuk menunjuk ke struktur `tm`, yang berisi informasi waktu terperinci, seperti tahun, bulan, tanggal, jam, menit, dan detik.
- Variabel timeString adalah array karakter yang akan digunakan untuk menyimpan waktu dalam bentuk string yang telah diformat.
- `time(&current_time)` digunakan untuk mendapatkan waktu saat ini dalam detik.
- `strftime()` digunakan untuk memformat waktu lokal menjadi string sesuai dengan format yang ditentukan.
- `fprintf()` digunakan untuk menulis ke file dengan format yang sudah ditentukan.
- Setelah menulis ke file log, file ditutup dengan menggunakan fungsi `fclose()` untuk memastikan data yang telah ditulis benar-benar tersimpan

2. Fungis `yuantask` dan `bubutask`

```c
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
```

- `for (int i = 0; i < 10; ++i)` mengiterasi dari 0 - 9, menggunakan variabel loop i untuk membedakan tugas-tugas yang berbeda.
- `snprintf(filename, sizeof(filename), "%s/task_%d_*.txt", directory, i)` untuk membentuk nama file txt yang sesuai di direktori.
- variabel `glob_result` sebagai tipe data `glob_t`. `glob_t` untuk menyimpan hasil dari pencarian file yang sesuai dengan pola file txt kita.
- `glob(filename, GLOB_TILDE, NULL, &glob_result)` untuk mencocokkan pola nama file yang disimpan dalam filename dengan file-file yang ada dalam direktori.
- `GLOB_TILDE` Parameter ini menunjukkan bahwa karakter tilde `(~)` dalam pola nama file akan diexpand menjadi direktori home pengguna.
- `fscanf(file, "%d %s %s", &jumlah, resolusi, kategori)` untuk megambil informati didalam file txt yaitu jumlah, resolusi, dan kategori.
- `logCompletion(LOG_FILE, i, "yuan", jumlah, kategori, resolusi)` memanggil fungsi `logCompletion` untuk memberikan informasi task berapa, nama pengerja, jumlah foto, kategorinya, dan resolusinya.
- `globfree(&glob_result)` digunakan untuk membebaskan memori yang dialokasikan oleh `glob()`.
- Untuk bubu sama seperti penjelasan yuan namun mengiterasi dari 19 - 10.

3. Fungsi `runTask`

```c
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
```

- Membuat 2 fork yang akan menjalankan fungsi `yuantask` dan `bubutask`.
- `waitpid` untuk yuan dan bubu agar kedua fungsi tadi dijalankan secara bersamaan.

### Hasil Output dan SS

- Output di terminal
  ![2 terminal](https://github.com/sisop-its-s24/praktikum-modul-2-f01/assets/144328529/b0fab54b-c8ac-4864-905c-0512a88f5a1e)

- Hasil di `recap.txt`
  ![2 recap](https://github.com/sisop-its-s24/praktikum-modul-2-f01/assets/144328529/d6ad19ea-6aee-47fb-9a0e-bc99073f2df2)

### 2C

Yuan dan Bubu menemukan petunjuk dalam setiap file task txt dengan format "**{jumlah} {resolusi} {kategori}**". Mereka harus mengunduh gambar sesuai dengan jumlah, resolusi, dan kategori yang disebutkan. Contohnya, jika task0 berisi "5 250x150 music", mereka mengunduh 5 gambar berukuran 250x150 dalam kategori music. Gambar diunduh dari [unsplash.com](unsplash.com) dan disimpan bersama file txt setiap task berdasarkan orang yang mengerjakan task.

#### Langkah penyelesaian

- [ ] Membuat fungsi untuk membuat folder `yuan` dan `bubu` dan memakai perulangan untuk membuat folder `task_*` yang mereka kerjakan.
- [ ] Membuat fungsi untuk memindahkan file txt yang berada pada folder `task`. Kemudian, memasukkan ke folder yuan atau bubu sesuai dengan yang mengerjakan dan ke folder `task_*` sesuai nomor tasknya.
- [ ] Membuat fungsi yang membaca informasi di file txt dan mengunduh gambar di link yang disediakan sesuai dengan informasi kategori dan resolusi yang ada di file txt.

### Jawaban

Berdasarkan langkah penyelesaian, berikut code yang saya kerjakan :

```c
const char baseUrls[] = "https://source.unsplash.com/random/";

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
```

### Penjelasain lebih detail mengenai code

Penjelasan lebih detail sesuai dengan langkah penyelesaian :

1. Fungsi `taskC`

```c
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
```

- `mkdir(yuanFolder, 0777) == -1 && errno != EEXIST` membuat folder yuan dan mengeceknya jika sudah ada.
- `for` disini untuk membuat folder `task_*` dari 0 - 9.

2. Fungsi `moveFilesToDirectories`

```c
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
```

- `(strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)` untuk memeriksa dimana di direktori mana kita sekarang.
- `snprintf(filePath, sizeof(filePath), "%s/%s", sourceFolder, entry->d_name)` menggabungkan `sourceFolder` dan nama file yang terbaca `entry->d_name`
- `stat` untuk mendapatkan informasi status file yang terkait dengan path yang telah dibuat.
- `if (sscanf(entry->d_name, "task_%d_", &taskNumber) != 1 || taskNumber < 0 || taskNumber > 19)` untuk memindai nama file `entry->d_name` dan mencoba mengekstrak nomor tugas dari format yang diberikan.

3. Fungsi `moveFilesToDirectories`

```c
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
```

> Di sini, kita memeriksa nilai `taskNumber` untuk menentukan ke direktori mana file tersebut harus dipindahkan. Jika `taskNumber` berada di antara 0 dan 9, maka file tersebut harus dipindahkan ke dalam direktori `Yuan/taskX`, di mana X adalah nomor tugas. Jika `taskNumber` berada di antara 10 dan 19, maka file tersebut harus dipindahkan ke dalam direktori `Bubu/taskX`. Jika `taskNumber` tidak dalam rentang tersebut, maka cetak pesan kesalahan dan lanjutkan ke file selanjutnya.

- `if (rename(filePath, destinationFolder) == -1)` untuk memindahkan file dari lokasi awal ke lokasi tujuannya.

4. Fungsi `downloadImagesFromTasks` disini saya menjelaskan yang bagian yuan saja, karena yuan dan bubu sama saja hanya beda pengerjaan dan ke direktori mana akan dimasukkan.

```c
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
```

- `baseDirectory` dibuat untuk menyimpan jalur direktori task-task yang ada di folder `Yuan`.
- `opendir` untuk membuka direktori tugas tersebut.

5. Fungsi `downloadImagesFromTasks`

```c
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
```

- `fscanf(file, "%d %s %s", &jumlah, resolusi, kategori)` untuk membaca informasi jumlah resolusi, dan kategori dari file tersebut.
- `sprintf(url, "%s%s/?%s", baseUrls, resolusi, kategori)` membuat string dengan menamaan `baseUrls` ditambah dengan resolusi dan kategori yang didapat sebelumnya.
- `sprintf(filename, "%s/Yuan/task%d/gambar%d.jpeg", baseFilenames, y, i)` membuat string untuk format nama gambar yang akan diunduh.
- `execlp("wget", "wget", "-O", filename, url, NULL)` untuk mengunduh gamabr sesuai `url` yang dibuat tadi dan dimasukkan ke direktori `filename`.

### Hasil Output dan SS

- Output di terminal
  ![3 terminal1](https://github.com/sisop-its-s24/praktikum-modul-2-f01/assets/144328529/1f186bae-08d6-48ef-b55b-db20f0440579)

- ![3 terminal2](https://github.com/sisop-its-s24/praktikum-modul-2-f01/assets/144328529/677d1579-64dd-4716-816b-9815d72641b3)

- Perubahan di direktori
  ![3 direktori1](https://github.com/sisop-its-s24/praktikum-modul-2-f01/assets/144328529/7a0d241b-4c03-4cde-bb18-dfd012f10f69)

- ![3 direktori2](https://github.com/sisop-its-s24/praktikum-modul-2-f01/assets/144328529/30c3e937-f9d8-4d8c-8343-3a07dc359da8)

- Bukti Kesesuaian unduhan gambar
  ![3 bukti1](https://github.com/sisop-its-s24/praktikum-modul-2-f01/assets/144328529/a8181191-fb89-44e3-9ab3-6d8a167619a7)

- ![3 bukti2](https://github.com/sisop-its-s24/praktikum-modul-2-f01/assets/144328529/9592d151-70c5-4537-8a1a-48e0699d23d7)

### 2D

Setelah semua tugas selesai, mereka menambahkan jumlah gambar dari setiap kategori ke dalam "recap.txt". dan pada baris terakhir tambahkan jumlah seluruh gambar yg di unduh.

#### Langkah penyelesaian

- [ ] Membuka file `recap.txt`.
- [ ] Dari informasi yang ada di file `recap.txt` kita akan mengambil semua katerogi yang ada dan berapa gambar yang diunduh untuk tiap kategori.
- [ ] Mengitung berapa total gambar yang diunduh.
- [ ] dan menuliskan semua itu kedalam `recap.txt`.

### Jawaban

Berdasarkan langkah penyelesaian, berikut code yang saya kerjakan :

```c
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
```

### Penjelasain lebih detail mengenai code

Penjelasan lebih detail sesuai dengan langkah penyelesaian :

1.

```c
    FILE *recapFile = fopen("recap.txt", "a+");
    if (recapFile != NULL) {
        int totalImages = 0;
        int totalCategories = 0;
        char categories[100][150] = {0}; // Inisialisasi array categories
        int counts[100] = {0}; // Inisialisasi array counts

        char line[2000];
```

- `FILE *recapFile = fopen("recap.txt", "a+")` membuka file "recap.txt" dalam mode "a+" (append and read)
- `if (recapFile != NULL)` untuk memeriksa apakah pembukaan file `recap.txt` berhasil atau tidak.
- `totalImages` untuk menyimpan total jumlah gambar yang telah diunduh.
- `totalCategories` untuk menyimpan jumlah kategori unik yang ditemukan dalam file `recap.txt`.
- `categories[100][150] = {0}` untuk menyimpan nama kategori.
- `counts[100] = {0}` untuk menyimpan jumlah gambar yang diunduh untuk setiap kategori.
- `line[2000]` untuk menyimpan setiap baris dari file recap.txt yang sedang dibaca.

2.

```c
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
```

- `while` disini digunakan untuk membaca tiap baris dari file `recap.txt`.
- `sscanf` untuk melakukan parsing terhadap baris yang telah dibaca `line` sesuai dengan format yang ada di dalam file `recap,txt`.
- `loop` for digunakan untuk mencari apakah kategori yang ditemukan sudah ada dalam array categories. Jika ditemukan, `categoryIndex` akan disetel ke indeks kategori yang ditemukan. Jika tidak ditemukan, kategori baru akan disalin ke dalam array `categories` dan `categoryIndex` akan disetel ke indeks kategori yang baru. Jumlah kategori atau `totalCategories` akan diupdate jika kategori baru ditemukan.
- `counts` dan jumlah total gambar atau `totalImages` akan diperbarui berdasarkan informasi yang diekstrak dari baris tersebut.

3.

```c
        for (int i = 0; i < totalCategories; i++) {
            fprintf(recapFile, "%s : %d\n", categories[i], counts[i]);
        }
        fprintf(recapFile, "total images: %d\n", totalImages);
        fclose(recapFile);
```

- `for` digunakan untuk mengiterasi melalui setiap kategori yang ditemukan dalam file `recap.txt`. Jumlah iterasi ditentukan oleh variabel `totalCategories`.
- `fprintf(recapFile, "%s : %d\n", categories[i], counts[i])` digunakan untuk menulis setiap kategori dan jumlah gambar ke dalam `recap.txt`
- `fprintf(recapFile, "total images: %d\n", totalImages)` setelah semua katego dan jumlahnya dituliskan menuliskan total gambar yang diunduh.

### Hasil Output dan SS

-Hasil di `recap.txt`
![4 recap](https://github.com/sisop-its-s24/praktikum-modul-2-f01/assets/144328529/88670733-dad8-4912-aae9-ae2e0d3ccd7d)

### 2E

Karena tugas sudah siap dikumpulkan, zip folder Yuan dan Bubu menjadi satu zip bernamakan submission.zip dan **JANGAN HAPUS** folder aslinya sehingga menyisakan struktur folder sebagai berikut:

```
task/
├── Yuan/
├── Bubu/
├── submission.zip
└── recap.txt
```

#### Langkah penyelesaian

- [ ] Membuat char untuk nama file.zipnya yang sudah ditentukan.
- [ ] Melakukan zip kepada folder `task`

### Jawaban

Berdasarkan langkah penyelesaian, berikut code yang saya kerjakan :

```c
void taskE() {
    const char *directoryToZip = TASK_DIRECTORY;
    const char *outputZipFile = "/home/faiq/Documents/Kuliah/Sisop/Praktikum_2/praktikum-modul-2-f01/task-2/task/submissions.zip";

    execlp("zip", "zip", "-r", outputZipFile, "task/Yuan", "task/Bubu", NULL);

    perror("execlp");
    exit(1);
}
```

### Penjelasain lebih detail mengenai code

Penjelasan lebih detail sesuai dengan langkah penyelesaian :

1. Fungsi `taskE`
   > `const char *directoryToZip = TASK_DIRECTORY` folder yang akan di zip.

- `const char *outputZipFile = "/home/faiq/Documents/Kuliah/Sisop/Praktikum_2/praktikum-modul-2-f01/task-2/task/submissions.zip"` nama folder zipnya.
- `execlp("zip", "zip", "-r", outputZipFile, "task/Yuan", "task/Bubu", NULL)` fungsi untuk menzip.

### Hasil Output dan SS

- Output di terminal
  ![5 terminal](https://github.com/sisop-its-s24/praktikum-modul-2-f01/assets/144328529/de95d736-5ac9-4250-8b11-9974152035a7)

- Perubahan di direktori
  ![5 direktori](https://github.com/sisop-its-s24/praktikum-modul-2-f01/assets/144328529/398f2780-50d9-4f4d-bd31-198e3015a6d8)

- Isi zip
  ![5 isi zip](https://github.com/sisop-its-s24/praktikum-modul-2-f01/assets/144328529/f2774c14-c525-4710-8f9b-c3ec5daca1e1)

### Revisi

Semua pengerjaan subtask C, D, dan E adalah revisi.

### Kendala Pengerjaan

Kendala yang dialami adalah

- bingung kenapa fungsi overlapping menggunakan fork hasilnya tidak tetap (kadang selang-seling kadang tidak).

<br>

## No. 3 Rama's Weather Forecast <br>

> ## 3A

- Pada 3A kita harus mendownload file.zip dan menyimpannya ke dalam lokal kemudian mengekstraknya ke folder bernama 'weather'. Setelah selesai diekstrak kemudian hapus file.zip.

- #### Logic penyelesaian
- [ ] Menggunakan 2 fork
- [ ] Child pertama pada fork kesatu mendownload file.zip
- [ ] Parent membuat fork kedua dimana child nya akan mengekstrak file.zipnya ke folder weather
- [ ] Child pada fork kedua menghapus file.zip

- Berdasarkan permasalahan diatas, berikut adalah code yang saya kerjakan : <br>

```c
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
```

- Berikut penjelasan lebih detail mengenai fungsi downloadUnzipRemove diatas :

```c
if (mkdir("weather", 0700) == -1) {
  perror("FAILED TO CREATE weather DIRECTORY");
  exit(EXIT_FAILURE);
}
```

Membuat direktori dengan nama 'weather' menggunakan mkdir

```c
execlp("wget", "wget", "-O", "weather.zip", "https://drive.usercontent.google.com/u/0/uc?id=1U1-9v9pJeX5MoofwoBHCcD9nA7YWy_np&export=download", NULL);
```

Menggunakan execlp dengan perintah wget untuk mendownload link tersebut. `-O` digunakan untuk menyimpan file dari link tersebut dengan nama tertentu dan kita menggunakan nama `weather.zip`.

```c
execlp("unzip", "unzip", "weather.zip", "-d", "weather", NULL);
```

Menggunakan execlp dengan perintah unzip dengan mengunzip yang nama filenya `weather.zip`. `-d` digunakan untuk menentukan direktori hasil ekstrasi yang akan disimpan yaitu direktori `weather`.

```c
logMessage("Successfully unzip file..");

// delete the zip file
int delete_status = remove("weather.zip");
if (delete_status == 0)
    printf("ZIP FILE DELETED SUCCESSFULLY\n");
else {
    perror("FAILED TO DELETING ZIP FILE");
    exit(EXIT_FAILURE);
}
```

Setelah unzip selesai akan dicatat menggunakan logMessage. Kemudian hapus weather.zip menggunakan remove.

### Berikut adalah output 3A :

![image](https://github.com/sisop-its-s24/praktikum-modul-2-f01/assets/116788471/953aaf86-7730-4c5b-92c7-661889425547)
![image](https://github.com/sisop-its-s24/praktikum-modul-2-f01/assets/116788471/6cadcd2a-5b5f-4f53-a957-c0943e811a92)

> ## 3B

- Pada 3B kita melakukan data preprocessing dengan menghapus baris yang memiliki kolom suhu udara diatas 35 derajat kemudian update dataset 'kecamatanforecast.csv'.

- #### Logic Penyelesaian
- [ ] Menggunakan 1 fork
- [ ] Child pertama memisahkan baris dengan kolom suhu dibawah 35 derajat ke file baru yaitu kecamatanforecast.csv
- [ ] Parent melakukan cek setiap baris pada kecamatanforecast.csv kemudian jika suhu diatas 35 maka print di logMessage.
- [ ] Hapus kecamatanforecast.csv
- [ ] Rename kecamatanforecast_temp.csv menjadi kecamatanforecast.csv

- Berdasarkan permasalahan diatas, berikut adalah code yang saya kerjakan : <br>

```c
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
```

- Berikut penjelasan lebih detail mengenai fungsi preprocessData diatas :

```c
execlp("/bin/sh", "sh", "-c", "awk -F, '{ if ($4 <= 35) print $0 }' weather/kecamatanforecast.csv > weather/kecamatanforecast_temp.csv", NULL);
```

Menggunakan execlp untuk menjalankan shell command. Lalu menggunakan awk untuk memproses file kecamatanforecast.csv. Jika baris dengan suhu kurang dari samadengan 35, maka akan dicetak dan disimpan file baru yaitu kecamatanforecast_temp.csv.

```c
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
```

Membuka file kecamatanforecast.csv kemudian mengecek setiap line nya menggunakan while. Menggunakan sscanf `"%*[^,],%*[^,],%*[^,],%d"` yang akan membaca line dengan membuang tiga kolom awal karena tidak diperlukan. Jika suhu diatas 35 maka print message dengan suhu diatas 35 menggunakan sprintf yang akan disimpan di line. Kemudian masukkan line ke dalam logMessage untuk dicetak messagenya. Setelah itu tutup file kecamatanforecast.csv kemudian hapus file kecamatanforecast.csv dan rename kecamatanforecast_temp.csv menjadi kecamatanforecast.csv.

### Berikut adalah output 3B :

![image](https://github.com/sisop-its-s24/praktikum-modul-2-f01/assets/116788471/53ac8e6a-2497-4dc4-b941-50ba05d0956b)

> ## 3C

- Pada 3C kelompokkan berdasarkan ID Lokasi dan pisahkan menjadi dataset baru "city\_{ID Lokasi}" contoh "city_5012725.csv".
- #### Logic Penyelesaian
- [ ] Menggunakan 2 fork
- [ ] Child fork pertama membuat direktori dengan nama city_group
- [ ] Parent membuat fork kedua
- [ ] Child fork kedua memisahkan data berdasarkan ID Lokasi menjadi dataset baru
- [ ] Parent fork kedua mengeksekusi logMessage

- Berdasarkan permasalahan diatas, berikut adalah code yang saya kerjakan : <br>

```c
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
```

- Berikut penjelasan lebih detail mengenai fungsi preprocessData diatas :

```c
execlp("mkdir", "mkdir", "-p", "city_group", NULL);
```

Menggunakan execlp dengan perintah mkdir untuk membuat direktori bernama city_group

```c
execlp("awk", "awk", "-F,", "NR > 1 { print > \"city_group/city_\"$1\".csv\" }", "weather/kecamatanforecast.csv", NULL);
```

Menggunakan execlp dengan perintah awk dan `-F,` menggunakan koma sebagai delimiter. NR>1 untuk skip bagian header file. Lalu mencetak baris dari file csv ini ke file lain dengan nama yang berasal dari nilai pada kolom pertama $1 yaitu ID Lokasi. Dicetak ke file dengan format "city*group/city*<nilai_kolom_pertama>.csv".

### Berikut adalah output 3C :

![image](https://github.com/sisop-its-s24/praktikum-modul-2-f01/assets/116788471/99380605-b221-48b1-af4e-b221ff58d7ce)
![image](https://github.com/sisop-its-s24/praktikum-modul-2-f01/assets/116788471/c08677ad-0320-4a47-b490-6f1370fadc09)

> ## 3D

- Pada 3D catat semua aktivitas di "logbook.txt" dengan format { [YYYY-MM-dd HH:mm:ss] [message]}

- #### Logic Penyelesaian
- [ ] Membuat fungsi log message
- [ ] Gabungkan message yang dibawa dari fungsi lain dan cetak bersama dengan waktunya
- [ ] Cetak ke dalam logbook.txt

- Berdasarkan permasalahan diatas, berikut adalah code yang saya kerjakan : <br>

```c
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
    info = localtime(&rawtime); ///mengonversi waktu
    strftime(timestamp, 20, "%y-%m-%d %H:%M:%S", info);

    fprintf(logFile, "[%s] %s\n", timestamp, message); //mengonversi informasi waktu dalam struct tm menjadi string
    fclose(logFile);
}
```

### Berikut adalah output 3D :

![image](https://github.com/sisop-its-s24/praktikum-modul-2-f01/assets/116788471/c4b4c76b-9d43-4849-8333-f9888cee6b28)

### Kendala Pengerjaan

- [ ] Pada awalnya masih bingung terhadap penggunaan fork, namun sudah aman setelah soal 3A
- [ ] Kesusahan saat mencetak log message pada data preprocessing disebabkan harus mencetak suhu udara yang diatas 35 derajat
- [ ] Namun semua permasalahan sudah dapat teratasi

## No. 4 Tic Tac Toe

> 4A

- Pada 4A pengguna dapat memilih tipe pemain('X' atau 'O') di player.c. Jika salah satu tipe pemain sudah dipilih maka "has already been taken, choose other player types". Tipe pemain yang sudah dipilih atau tidak akan disimpan di shared memory.

- #### Logic Penyelesaian
- [ ] Menandai player_type menggunakan shared memory ketika sudah diinput pengguna
- [ ] Jika salah satu pemain sudah dipilih, maka cetak "Waiting for another player to join the game
- [ ] Jika pemain X(sudah ditandain melalui shared memory) sudah dipilih maka print "'X' has already been taken, choose other player types". Berlaku sebaliknya untuk pemain O.

- Berdasarkan permasalahan diatas, berikut adalah potongan dari code yang saya kerjakan : <br>

```c
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
```

Melakukan while looping hingga kedua pemain sudah terpilih. Jika shm->player_X yaitu jika pemain X sudah dipilih maka cetak `'X' has already been taken, choose other player types`. Setelah salah satu pemain dipilih, dibedakan berdasarkan choose_playernya. Misal `choose_player == 1` maka tandai `shm->turn` menjadi `true` karena dikondisi apapun player X akan selalu jalan pertama di permainan dan tandain `shm->player_X = 1` pada shared memory. Kemudian jika pemain X sudah dipilih namun pemain O belum dipilih maka cetak `Waiting for another player to join the game`. Begitu jugaa sebaliknya pada saat `choose_player == 2` dilakukan hal yang sama.

### Berikut adalah output 4A :

![image](https://github.com/sisop-its-s24/praktikum-modul-2-f01/assets/116788471/b108e71f-73a9-4d78-af3a-8ea14b50fc2d)

> 4B

- Pada 4B di player.c menampilkan kotak tic tac toe dan pengguna memasukan inputan sesuai nomor kotak. Player X memulai game terlebih dahulu. Kemudian input pengguna dikirimkan ke game.c dan game.c menampilkan pesan dari player.c

- ### Logic Penyelesaian
- [ ] Kirim input pengguna ke game.c menggunakan shared memory dengan message queue
- [ ] Game.c menerima pesan dari player.c kemudian print pesan sesuai format

- Berdasarkan permasalahan diatas, berikut adalah potongan dari code yang saya kerjakan : <br>
- **player.c**

```c
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
```

```c
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
```

Melakukan looping agar masing-masing pemain dapat berjalan. Player X selalu jalan pertama kali karena memenuhi if yaitu shm->turn == true. Kemudian setelah diinput cell numbernya, Player.c akan mengirimkan sebuah pesan dengan mesg_type = 1 yang berisi player_type dan cell_number kepada game.c untuk dicetak sesuai format. Sedangkan untuk player O akan berjalan setelah player X selesai jalan dan melakukan hal yang sama seperti player X.

- **game.c**

```c
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
```

Game.c menerima inputan atau pesan dari player.c yang berisi player_type dan cell_number.Kemudian jika pesan berhasil, game.c akan mencetak sender berisi player type dan message sesuai yang diterima dari player.c.

### Berikut adalah output 4B :

![image](https://github.com/sisop-its-s24/praktikum-modul-2-f01/assets/116788471/a4d3410a-a703-401e-89d4-246df263fd45)
![image](https://github.com/sisop-its-s24/praktikum-modul-2-f01/assets/116788471/58b973d4-dec4-47df-b5c7-f30caf8956df)

> 4C

- Pada 4C ini game.c mengecek inputan user player.c sesuai atau tidak dengan kotak permainan. Jika tidak sesuai kirimkan pesan error.

- #### Logic Penyelesaian
- [ ] Buat fungsi isValidInput pada game.c untuk mengecek valid input player
- [ ] Jika invalid dan sudah terisi, pemain menginput ulang.
- [ ] Jika valid, maka permainan dilanjutkan.

- Berdasarkan permasalahan diatas, berikut adalah potongan dari code yang saya kerjakan : <br>
- **player.c**

```c
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
```

Setelah mengirim pesan berisi player_type dan cell_number ke game.c. Tahap berikutnya adalah pengecekkan inputan pemain apakah invalid atau valid dan apakah sudah terisi. Untuk validasi dari inputan akan dicek pada game.c kemudian akan dikirimkan kembali ke player.c. Jika invalid
akan mencetak `"Wrong input, choose cell number 1-9"`, jika filled akan mencetak `"Choose other cell, %d is filled by '%c'\n", cell_number, shm->board[row][col]"` dimana cell_number berisi angka cell_number yang terisi dan shm->board[row][col] berisi oleh jenis player pada board yang sudah terisi. Sedangkan jika valid maka akan keluar dari do-while dan mencetak displayBoard permainan terbaru.

- **game.c**

```c
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
```

Pada game.c ini mengecek validasi inputan dari pemain. Misal jika `cek == 1` maka `invalid` dan game.c akan mengirim pesan invalid ke player.c untuk diproses lebih lanjut dan begitupun untuk kasus pada `filled` dan `valid`.

### Berikut adalah output 4C :

![image](https://github.com/sisop-its-s24/praktikum-modul-2-f01/assets/116788471/e39395b8-e715-4329-b9f0-6dc4dcbf9d83)
![image](https://github.com/sisop-its-s24/praktikum-modul-2-f01/assets/116788471/069a7e6b-55f9-4aff-a723-5ee2a1a20c8d)

> 4D

- Pada 4D ini permainan akan berakhir atau selesai jika kotak telah terisi semua (seri) atau terdapat simbol (‘X’ atau ‘O’) yang membentuk garis vertikal, horizontal, maupun diagonal. Kotak tic tac toe yang telah diperbarui dan hasil akhir permainan akan dikirim ke player.c dan ditampilkan.

- #### Logic Penyelesaian
- [ ] Buat fungsi untuk mengecek apakah game itu seri, menang/kalah , dan masih lanjut bermain
- [ ] Lakukan pengecekan pada game.c kemudian setelah dicek kirim pesan ke player.c

- Berdasarkan permasalahan diatas, berikut adalah potongan dari code yang saya kerjakan : <br>
- **player.c**

```c
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
```

Player.c menunggu pesan dari game.c untuk mengecek apakah permainan seri, menang/kalah atau on going. Dengan menggunakan strcmp apakah pesan yang dikirim sesuai atau tidak. Jika pemain menerima `mesg_text == "win"` maka cetak `You Win!` untuk player yang menang dan langsung keluar dari semua looping dan permainan berakhir untuk pemain pertama. Sedangkan pemain kedua ditandai dengan `shm->lose = true;` yang nantinya akan masuk di if dibawah ini.

```c
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
```

Pada game.c nanti akan ditandai dengan `shm->gameEnd == true` yang menandakan permainan sudah selesai sehingga player kedua dapat masuk ke if ini dan dapat mencetak pesan yang sesuai. Pada kasus diatas pemain kedua ditandai dengan `shm->lose == true` sehingga pemain kedua akan mencetak `You Lose!` dan pada akhirnya semua di reset dari shared memory, board, dan nilai bool serta lain-lainnya. jadi semua ini pun berlaku sama pada kondisi tie dan player_type juga.

- **game.c**

```c
shm->board[(cell_number - 1) / 3][(cell_number - 1) % 3] = player_type;
int game_result = isGameOver(shm->board);

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
```

Pada game.c board akan diupdate sesuai inputan player yang valid dan akan diperbarui. Kemudian setiap board diupdate akan dilakukan pengecekan apakah pemainan sudah selesai atau belum. Jika belum maka game.c akan mengirimkan pesan `ongoing` ke player.c untuk melanjutkan permainan. Jika permainan sudah selesai maka ditandai dengan `shm->gameEnd = true;` dan akan masuk ke game_result yang sesuai kemudian dibreak.

### Berikut adalah output 4B :

**Kondisi Menang**
![image](https://github.com/sisop-its-s24/praktikum-modul-2-f01/assets/116788471/ae71423d-e6d9-428c-8f41-c8ca1e6d3be6)
**Kondisi Seri**
![image](https://github.com/sisop-its-s24/praktikum-modul-2-f01/assets/116788471/bc7abd05-552e-4e23-ac0d-194f15635fe5)

### Kendala Pengerjaan

- [ ] Code berantakan karena player tidak dapat berjalan secara sinkron
- [ ] Kesusahan saat membuat gameover state, kadang benar dan kadang salah
- [ ] Bingung terhadap shared memory dan message queue, namun setelah beberapa lama pengerjaan sudah paham
- [ ] Shared memory yang ngestuck namun sudah diatasi
- [ ] Kesalahan logika saat awal pengerjaan sehingga membuat pengerjaan menjadi lama disebabkan tidak membaca soal dengan teliti dimana seharusnya pengecekan validasi diletakkan di game.c
- [ ] Namun semua kendala sudah dapat teratasi
