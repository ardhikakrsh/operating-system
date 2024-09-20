#define FUSE_USE_VERSION 31

#include <fuse.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <dirent.h>
#include <fcntl.h>
#include <unistd.h>
#include <libgen.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>

static const char *target_dir = "/home/faiq/Documents/Kuliah/Sisop/Praktikum_4/praktikum-modul-4-f01/task-1/Storage";
static const char *trash_dir = "/home/faiq/Documents/Kuliah/Sisop/Praktikum_4/praktikum-modul-4-f01/task-1/Storage/trash";

void write_log(const char *action, const char *path1, const char *path2) {
    FILE *log_file = fopen("/home/faiq/Documents/Kuliah/Sisop/Praktikum_4/praktikum-modul-4-f01/task-1/Storage/trash.log", "a");
    if (log_file != NULL) {
        time_t t = time(NULL);
        struct tm *tm = localtime(&t);
        char timestamp[20];
        strftime(timestamp, sizeof(timestamp), "%y%m%d-%H:%M:%S", tm);

        if (strcmp(action, "MOVED") == 0) {
            fprintf(log_file, "%s %s %s TO TRASH\n", timestamp, action, path1);
        } else if (strcmp(action, "PERMANENTLY REMOVED") == 0) {
            fprintf(log_file, "%s %s %s\n", timestamp, action, path1);
        } else if (strcmp(action, "RESTORED") == 0) {
            fprintf(log_file, "%s %s %s FROM TRASH TO %s\n", timestamp, action, path1, path2);
        } else if (strncmp(action, "FAILED TO CHMOD", 15) == 0 || strncmp(action, "FAILED TO CHOWN", 15) == 0 || strncmp(action, "FAILED TO RENAME", 16) == 0) {
            fprintf(log_file, "%s %s %s\n", timestamp, action, path1);
        } else if (strcmp(action, "FAILED TO REMOVE TRASH DIRECTORY") == 0 || strcmp(action, "FAILED TO MOVE TRASH DIRECTORY") == 0 || strcmp(action, "FAILED TO RENAME TRASH DIRECTORY") == 0) {
            fprintf(log_file, "%s %s\n", timestamp, action);
        } else if (strcmp(action, "FAILED TO CREATE TRASH DIRECTORY") == 0 || strcmp(action, "FAILED TO CREATE RESTORE DIRECTORY") == 0) {
            fprintf(log_file, "%s %s DIRECTORY\n", timestamp, action);
        }

        fclose(log_file);
    } else {
        perror("Error opening log file");
    }
}

static int my_getattr(const char *path, struct stat *stbuf)
{
    int res = 0;
    char fpath[1000];
    sprintf(fpath, "%s%s", target_dir, path);

    res = lstat(fpath, stbuf);
    if (res == -1)
        return -errno;

    return 0;
}

static int my_readdir(const char *path, void *buf, fuse_fill_dir_t filler, off_t offset, struct fuse_file_info *fi)
{
    DIR *dp;
    struct dirent *de;
    char fpath[1000];
    sprintf(fpath, "%s%s", target_dir, path);

    dp = opendir(fpath);
    if (dp == NULL)
        return -errno;

    while ((de = readdir(dp)) != NULL) {
        struct stat st;
        memset(&st, 0, sizeof(st));
        st.st_ino = de->d_ino;
        st.st_mode = de->d_type << 12;
        if (filler(buf, de->d_name, &st, 0))
            break;
    }

    closedir(dp);
    return 0;
}

static int my_read(const char *path, char *buf, size_t size, off_t offset, struct fuse_file_info *fi)
{
    int fd;
    int res;
    char fpath[1000];
    sprintf(fpath, "%s%s", target_dir, path);

    (void)fi;
    fd = open(fpath, O_RDONLY);
    if (fd == -1)
        return -errno;

    res = pread(fd, buf, size, offset);
    if (res == -1)
        res = -errno;

    close(fd);
    return res;
}

static int my_mkdir(const char *path, mode_t mode) {
    int res;
    char fpath[1000];
    sprintf(fpath, "%s%s", target_dir, path);

    if (strcmp(basename(fpath), "trash") == 0 || strcmp(basename(fpath), "restore") == 0) {
        write_log("FAILED TO CREATE DIRECTORY", fpath, NULL);
        return -EPERM;
    }

    res = mkdir(fpath, mode);
    if (res == -1)
        return -errno;

    return 0;
}

static int create_metadata(const char *original_path, const char *new_path, mode_t original_mode) {
    char metadata_path[1000];
    snprintf(metadata_path, sizeof(metadata_path), "%s.metadata", new_path);

    FILE *metadata_file = fopen(metadata_path, "w");
    if (!metadata_file) {
        return -errno;
    }

    fprintf(metadata_file, "%s ", original_path);
    fprintf(metadata_file, "%o", original_mode & 0777);

    fclose(metadata_file);
    return 0;
}

static int read_metadata(const char *metadata_path, char *original_path, size_t path_size, mode_t *original_mode) {
    FILE *metadata_file = fopen(metadata_path, "r");
    if (!metadata_file) {
        return -errno;
    }

    char line[1000];
    if (fgets(line, sizeof(line), metadata_file) == NULL) {
        fclose(metadata_file);
        return -errno;
    }
    
    fclose(metadata_file);

    char *space_pos = strchr(line, ' ');
    if (!space_pos) {
        return -EINVAL;
    }

    *space_pos = '\0';
    strncpy(original_path, line, path_size - 1);
    original_path[path_size - 1] = '\0';

    *original_mode = strtol(space_pos + 1, NULL, 8);

    return 0;
}

static void create_directory_recursive(char *path) {
    char *dir = dirname(strdup(path));
    if (access(dir, F_OK) == -1) {
        create_directory_recursive(dir);
        mkdir(dir, 0755);
    }
    free(dir);
}

static int move_to_trash(const char *path) {
    struct stat statbuf;
    char trash_path[1000];
    char cmd[1000];

    if (stat(path, &statbuf) == -1) {
        perror("Error getting file status");
        return -1;
    }

    snprintf(trash_path, sizeof(trash_path), "%s/%s", trash_dir, basename((char *)path));

    snprintf(cmd, sizeof(cmd), "mv -f %s %s", path, trash_path);
    int status = system(cmd);
    if (status == -1) {
        perror("Error in moving to trash");
        return -1;
    }

    write_log("MOVED", path, trash_path);

    if (chmod(trash_path, 0000) == -1) {
        perror("Error in setting permissions for file in trash");
        return -1;
    }

    int res = create_metadata(path, trash_path, statbuf.st_mode);
    if (res != 0) {
        perror("Error in creating metadata");
        return -1;
    }

    return 0;
}

static int delete_from_trash(const char *path) {
    char cmd[1000];
    sprintf(cmd, "rm -rf %s", path);
    int status = system(cmd);
    if (status == -1) {
        perror("Error in deleting from trash");
        return -1;
    }

    write_log("PERMANENTLY REMOVED", path, NULL);

    // Hapus file metadata jika yang dihapus adalah file atau direktori di dalam direktori trash
    if (strstr(path, trash_dir) != NULL) {
        char metadata_path[1000];
        snprintf(metadata_path, sizeof(metadata_path), "%s.metadata", path);
        if (remove(metadata_path) == -1) {
            perror("Error removing metadata file");
            return -1;
        }
    }

    return 0;
}

static int my_unlink(const char *path) {
    int res;
    char fpath[1000];
    sprintf(fpath, "%s%s", target_dir, path);

    if (strstr(fpath, trash_dir) != NULL) {
        res = delete_from_trash(fpath);
        if (res == -1)
            return -errno;
    } else {
        res = move_to_trash(fpath);
        if (res == -1)
            return -errno;
    }

    return 0;
}

static int my_rmdir(const char *path) {
    int res;
    char fpath[1000];
    sprintf(fpath, "%s%s", target_dir, path);

    if (strcmp(fpath, trash_dir) == 0) {
        write_log("FAILED TO REMOVE TRASH DIRECTORY", NULL, NULL);
        return -EPERM;
    } else if (strstr(fpath, trash_dir) != NULL) {
        res = delete_from_trash(fpath);
        if (res == -1)
            return -errno;
    } else {
        res = move_to_trash(fpath);
        if (res == -1)
            return -errno;
    }

    return 0;
}

static int my_rename(const char *oldpath, const char *newpath) {
    int res;
    char old_fpath[1000], new_fpath[1000];
    struct stat statbuf;

    snprintf(old_fpath, sizeof(old_fpath), "%s%s", target_dir, oldpath);
    snprintf(new_fpath, sizeof(new_fpath), "%s%s", target_dir, newpath);

    if (strncmp(newpath, "/trash/restore", 14) == 0) {
        char metadata_path[1000];
        snprintf(metadata_path, sizeof(metadata_path), "%s.metadata", old_fpath);
        char original_path[1000];
        mode_t original_mode;

        res = read_metadata(metadata_path, original_path, sizeof(original_path), &original_mode);
        if (res != 0) {
            return res;
        }

        snprintf(new_fpath, sizeof(new_fpath), "%s", original_path);

        if (strncmp(oldpath, "/trash/", 7) == 0) {
            chmod(old_fpath, 0755);
        }

        create_directory_recursive(new_fpath);

        res = rename(old_fpath, new_fpath);
        if (res == -1) {
            write_log("FAILED TO RENAME", old_fpath, new_fpath);
            return -errno;
        }

        res = chmod(new_fpath, original_mode);
        if (res == -1) {
            write_log("FAILED TO RESTORE PERMISSIONS", new_fpath, NULL);
            return -errno;
        }

        write_log("RESTORED", old_fpath, new_fpath);

        char fullMetadata[1000];
        snprintf(fullMetadata, sizeof(fullMetadata), "%s.metadata", old_fpath);
        unlink(fullMetadata);

        return 0;
    }

    if (strncmp(newpath, "/trash/", 7) == 0) {
        write_log("FAILED TO RENAME", oldpath, newpath);
        return -EPERM;
    }

    if (strncmp(oldpath, "/trash/", 7) == 0) {
        chmod(old_fpath, 0755);
    }

    if (strcmp(trash_dir, old_fpath) == 0) {
        if (strcmp(dirname(old_fpath), dirname(new_fpath)) == 0) {
            write_log("FAILED TO RENAME TRASH DIRECTORY", NULL, NULL);
            return -EPERM;
        }
        if (strcmp(dirname(old_fpath), dirname(new_fpath)) != 0) {
            write_log("FAILED TO MOVE TRASH DIRECTORY", NULL, NULL);
            return -EPERM;
        }
    }

    if (stat(old_fpath, &statbuf) == -1) {
        return -errno;
    }

    res = rename(old_fpath, new_fpath);
    if (res == -1) {
        write_log("FAILED TO RENAME", old_fpath, new_fpath);
        return -errno;
    }

    res = chmod(new_fpath, statbuf.st_mode);
    if (res == -1) {
        write_log("FAILED TO RESTORE PERMISSIONS", new_fpath, NULL);
        return -errno;
    }

    write_log("RESTORED", old_fpath, new_fpath);

    char fullMetadata[1000];
    snprintf(fullMetadata, sizeof(fullMetadata), "%s.metadata", old_fpath);
    unlink(fullMetadata);

    return 0;
}

static int my_chmod(const char *path, mode_t mode) {
    int res;
    char fpath[1000];
    sprintf(fpath, "%s%s", target_dir, path);

    if (strstr(fpath, trash_dir) != NULL) {
        write_log("FAILED TO CHMOD", fpath, NULL);
        return -EPERM;
    }

    res = chmod(fpath, mode);
    if (res == -1)
        return -errno;

    write_log("CHMOD", fpath, NULL);

    return 0;
}

static int my_chown(const char *path, uid_t uid, gid_t gid) {
    int res;
    char fpath[1000];
    sprintf(fpath, "%s%s", target_dir, path);

    if (strstr(fpath, trash_dir) != NULL) {
        write_log("FAILED TO CHOWN", fpath, NULL);
        return -EPERM;
    }

    res = chown(fpath, uid, gid);
    if (res == -1)
        return -errno;

    write_log("CHOWN", fpath, NULL);

    return 0;
}

static struct fuse_operations my_oper = {
    .getattr = my_getattr,
    .readdir = my_readdir,
    .read = my_read,
    .mkdir = my_mkdir,
    .unlink = my_unlink,
    .rmdir = my_rmdir,
    .rename = my_rename,
    .chown = my_chown,
    .chmod = my_chmod,
};

int main(int argc, char *argv[])
{
    umask(0);
    return fuse_main(argc, argv, &my_oper, NULL);
}