#define FUSE_USE_VERSION 28
#define O_BINARY 0
#include <fuse.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <errno.h>
#include <sys/time.h>
#include <sys/types.h>
#include <pwd.h>
#include <libgen.h>
#include <stdbool.h>
#include <b64/cencode.h>
#include <b64/cdecode.h>


#define MAX_USERNAME_LEN 256
#define SIZE 1000
static  const  char *dirpath = "/home/ardhikakrsh/Documents/Kuliah/Sisop/Praktikum-4/praktikum-modul-4-f01/task-2/target";

char *current_username() {
    struct fuse_context *context = fuse_get_context();
    struct passwd *pw = getpwuid(context->uid);
    if (pw == NULL) {
        perror("getpwuid");
        exit(EXIT_FAILURE);
    }
    return pw->pw_name;
}

char *top_level_dir(const char *path) {
    char *path_copy = strdup(path); // Duplicate the path string to avoid modifying the original
    char *top_dir = NULL; // Initialize a pointer to store the top-level directory

    // Tokenize the path and get the first directory
    char *token = strtok(path_copy, "/"); // Split the path string by '/' and get the first token
    if (token != NULL) {
        top_dir = strdup(token); // Duplicate the first token and store it in top_dir
    }

    free(path_copy); // Free the duplicated path string
    return top_dir; // Return the top-level directory
}


//fungsi unuk mengecek apakah nama dari superuser sama dengan nama dari parent directory
int is_owner(const char *path) {
    char *top_dir = top_level_dir(path); // Get the top-level directory from the path
    char *username = current_username(); // Get the current user's username

    printf("Username: %s\n", username);
    printf("Top-level Dir: %s\n", top_dir);

    int result = strcmp(username, top_dir) == 0; // Compare the username with the top-level directory
    printf("DEBUG: is_owner result: %d\n", result);


    free(top_dir); // Free the memory allocated for top_dir
    return result; // Return the result of the comparison
}

static  int  xmp_getattr(const char *path, struct stat *stbuf)
{
    int res;
    char fpath[1000];

    sprintf(fpath,"%s%s",dirpath,path);

    res = lstat(fpath, stbuf);

    if (res == -1) return -errno;

    return 0;
}

static int xmp_readdir(const char *path, void *buf, fuse_fill_dir_t filler, off_t offset, struct fuse_file_info *fi)
{
    char fpath[1000];

    if(strcmp(path,"/") == 0)
    {
        path=dirpath;
        sprintf(fpath,"%s",path);
    } else sprintf(fpath, "%s%s",dirpath,path);

    int res = 0;

    DIR *dp;
    struct dirent *de;
    (void) offset;
    (void) fi;

    dp = opendir(fpath);

    if (dp == NULL) return -errno;

    while ((de = readdir(dp)) != NULL) {
        struct stat st;

        memset(&st, 0, sizeof(st));

        st.st_ino = de->d_ino;
        st.st_mode = de->d_type << 12;
        res = (filler(buf, de->d_name, &st, 0));

        if(res!=0) break;
    }

    closedir(dp);

    return 0;
}

// Base64 encoding function
char* base64Encoder(const char *input_str, int len_str, int *output_len) {
    int buffer_size = (len_str / 3) * 4 + 1; // Base64 encoded size can be up to this value
    char *encoded_str = (char *) malloc(buffer_size * sizeof(char));
    if (encoded_str == NULL) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    base64_encodestate state;
    base64_init_encodestate(&state);
    int encoded_length = base64_encode_block(input_str, len_str, encoded_str, &state);
    encoded_length += base64_encode_blockend(encoded_str + encoded_length, &state);
    encoded_str[encoded_length] = '\0'; // Null-terminate the encoded string

    *output_len = encoded_length;
    return encoded_str;
}

// Base64 decoding function
char* base64Decoder(const char *input_str, int len_str, int *output_len) {
    int buffer_size = (len_str / 4) * 3 + 1; // Base64 decoded size can be up to this value
    char *decoded_str = (char *) malloc(buffer_size * sizeof(char));
    if (decoded_str == NULL) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    base64_decodestate state;
    base64_init_decodestate(&state);
    int decoded_length = base64_decode_block(input_str, len_str, decoded_str, &state);
    decoded_str[decoded_length] = '\0'; // Null-terminate the decoded string

    *output_len = decoded_length;
    return decoded_str;
}

static int xmp_write(const char *path, const char *buf, size_t size, off_t offset, struct fuse_file_info *fi) {
    char fpath[1000];
    if (strcmp(path, "/") == 0) {
        path = dirpath;
        sprintf(fpath, "%s", path);
    } else {
        sprintf(fpath, "%s%s", dirpath, path);
    }

    int fd = 0;
    int res = 0;

    (void) fi;

    fd = open(fpath, O_WRONLY | O_BINARY);

    if (fd == -1) return -errno;

    // Check if the accessing user is the owner
    bool isOwner = is_owner(path);

    // Encode the data using Base64 if accessed by a non-owner
    if (!isOwner) {
        int encoded_size = 0;
        char *encoded_data = base64Encoder((const char *)buf, size, &encoded_size);
        res = pwrite(fd, encoded_data, encoded_size, offset);
        free(encoded_data);
    } else {
        // Write the data to the file as is
        res = pwrite(fd, buf, size, offset);
    }

    if (res == -1) {
        close(fd);
        return -errno;
    }

    close(fd);
    return res;
}

static int xmp_read(const char *path, char *buf, size_t size, off_t offset, struct fuse_file_info *fi) {
    char fpath[1000];
    if (strcmp(path, "/") == 0) {
        path = dirpath;
        sprintf(fpath, "%s", path);
    } else {
        sprintf(fpath, "%s%s", dirpath, path);
    }

    int fd = 0;
    int res = 0;

    (void) fi;

    fd = open(fpath, O_RDONLY | O_BINARY);

    if (fd == -1) return -errno;

    // Check if the accessing user is the owner
    bool isOwner = is_owner(path);

    // Read the data from the file
    res = pread(fd, buf, size, offset);

    if (res == -1) {
        close(fd);
        return -errno;
    }

    close(fd);

    // Decode the data using Base64 if accessed by a non-owner
    if (!isOwner) {
        for(int i = 0; i < res; i++){
            char character[2] = {buf[i], '\0'};
            int encoded_char_len = 0;
            char *encoded_char = base64Encoder((const char *)character, 1, &encoded_char_len);
            buf[i] = encoded_char[0];
            free(encoded_char);
        }
    }

    return res;
}

//fungsi mkdir, mkdir terbuat jika username == top_dir
static int xmp_mkdir(const char *path, mode_t mode) {
    if (!is_owner(path)) {
        return -EACCES;
    }

    char fpath[1000];
    sprintf(fpath, "%s%s", dirpath, path);
    //printf dirpath dan path
    printf("DEBUG: mkdir dirpath: %s\n", dirpath);
    printf("DEBUG: mkdir path: %s\n", path);

    int res = mkdir(fpath, mode);
    printf("DEBUG: mkdir res: %d\n", res);
    //printf path
    printf("DEBUG: mkdir path: %s\n", fpath);

    if (res == -1) return -errno;

    return 0;
}

//buat fungsi rmdir
static int xmp_rmdir(const char *path) {
    if (!is_owner(path)) {
        return -EACCES;
    }

    char fpath[1000];
    sprintf(fpath, "%s%s", dirpath, path);

    int res = rmdir(fpath);

    if (res == -1) return -errno;

    return 0;
}

//touch file
static int xmp_create(const char *path, mode_t mode, struct fuse_file_info *fi) {
    if (!is_owner(path)) {
        return -EACCES;
    }
    char fpath[1000];
    sprintf(fpath, "%s%s", dirpath, path);

    int res = creat(fpath, mode);

    if (res == -1) return -errno;

    fi->fh = res;
    return 0;
}


//remove file
static int xmp_unlink(const char *path) {
    if (!is_owner(path)) {
        return -EACCES;
    }

    char fpath[1000];
    sprintf(fpath, "%s%s", dirpath, path);

    int res = unlink(fpath);

    if (res == -1) return -errno;

    return 0;
}

//move file
static int xmp_rename(const char *from, const char *to) {
    if (!is_owner(from) || !is_owner(to)) {
        return -EACCES;
    }

    char fpath_from[1000];
    char fpath_to[1000];
    sprintf(fpath_from, "%s%s", dirpath, from);
    sprintf(fpath_to, "%s%s", dirpath, to);

    int res = rename(fpath_from, fpath_to);

    if (res == -1) return -errno;

    return 0;
}

//copy file
static int xmp_link(const char *from, const char *to) {
    if (!is_owner(from) || !is_owner(to)) {
        return -EACCES;
    }

    char fpath_from[1000];
    char fpath_to[1000];
    sprintf(fpath_from, "%s%s", dirpath, from);
    sprintf(fpath_to, "%s%s", dirpath, to);

    int res = link(fpath_from, fpath_to);

    if (res == -1) return -errno;

    return 0;
}

static int xmp_utimens(const char *path, const struct timespec ts[2]) {
    if (strstr(path, "touch") != NULL) {
        return xmp_create(path, 0666, NULL);
    } 
    
    else if(strstr(path, "rm") != NULL){
        return xmp_unlink(path);
    }

    else if(strstr(path, "cp") != NULL){
        char from[1000], to[1000];
        sscanf(path, "cp %s %s", from, to);
        return xmp_link(from, to);
    }

    else if(strstr(path, "mv") != NULL){
        return xmp_rename(path, "move");    
    }

    else {
        // Jika tidak, lanjutkan dengan pemrosesan utimens seperti biasa
        char fpath[1000];
        sprintf(fpath, "%s%s", dirpath, path);

        int res = utimensat(0, fpath, ts, AT_SYMLINK_NOFOLLOW);
        if (res == -1) return -errno;

        return 0;
    }
}

static struct fuse_operations xmp_oper = {
    .getattr = xmp_getattr,
    .readdir = xmp_readdir,
    .read = xmp_read,
    .mkdir = xmp_mkdir,
    .rmdir = xmp_rmdir,
    .utimens = xmp_utimens,
    .create = xmp_create,
    .unlink = xmp_unlink,
    .rename = xmp_rename,
    .link = xmp_link,
    .write = xmp_write,
};

int main(int argc, char *argv[]) {
    umask(0);
    return fuse_main(argc, argv, &xmp_oper, NULL);
}