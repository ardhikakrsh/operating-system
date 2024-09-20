#include <stdio.h>
#include <stdlib.h>
#include <curl/curl.h>
#include <zip.h>
#include <sys/stat.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>
#include <dirent.h>

#define URL "https://drive.usercontent.google.com/download?id=1gJkoTbiHZcJ1M36vqZPTrj3qTQkRMElQ&export=download&authuser=0&confirm=t&uuid=577a6f0e-793c-44ca-abb8-164be6cbf53d&at=APZUnTWaH_cimb1yMRAaBq9hTCBJ:1713540025651"
#define FILENAME "horse.zip"
#define MAX_HORSES 100
#define MAX_LINE_LENGTH 100
#define MAX_FILENAME_LEN 20

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

int main()
{
    const char *extract_dir = "horse";
    if (HorseFetcher(URL, FILENAME, extract_dir) != 0)
        return 1;

    // choose one to print
    // 1st
    // for (int i = 1; i <= 3; i++)
    // {
    //     char filename[50];
    //     sprintf(filename, "%s/horse_%d.txt", extract_dir, i);
    //     HorseRaceHooray(filename, i);
    // }

    // 2nd
    HorseRaceHooray("horse/horse_1.txt", 1);
    HorseChampionLeaderboard();

    return 0;
}
