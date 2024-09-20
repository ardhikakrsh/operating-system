[![Review Assignment Due Date](https://classroom.github.com/assets/deadline-readme-button-24ddc0f5d75046c5622901739e7c5dd533143b0c8e959d652212380cedb1ea36.svg)](https://classroom.github.com/a/J5rciaaA)

<div align=center>

|    NRP     |          Name           |
| :--------: | :---------------------: |
| 5025221006 |     Ardhika Krishna     |
| 5025221100 | Riyanda Cavin Sinambela |
| 5025221294 |   Faiq Lidan Baihaqi    |

# Praktikum Modul 4 _(Module 4 Lab Work)_

</div>

### Daftar Soal _(Task List)_

- [Task 1 - TrashOps](/task-1/)

- [Task 2 - Fu53enC0d3D==](/task-2/)

- [Task 3 - MengOS](/task-3/)

### Laporan Resmi Praktikum Modul 4 _(Module 4 Lab Work Report)_

# 🚮 Nomor 1 TrashOps

## Soal

Pada kebanyakan sistem operasi Linux, perintah rm akan menghapus index yang mengarah pada entri file. Sehingga akan sulit untuk memulihkan file tersebut, atau dapat dikatakan telah terhapus permanen. Menariknya, anda akan membuat filesystem yang memodifikasi proses penghapusan dan pemulihan file dan direktori. Sebelum itu, silakan gunakan resource berikut sebagai direktori target: link.

Catatan :

- Direktori trash sudah terdapat pada resource yang akan anda gunakan.
- Anggap saja tidak ada kasus ketika menghapus file atau direktori dengan nama yang sama kedalam trash.
- Dilarang menggunakan fungsi system().

### Kode lengkap

```c
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
```

## 1A

Kode untuk membuat filesystem diletakkan pada trash.c. Anda harus menggunakan fungsi getattr, readdir, read, unlink, rmdir, rename, mkdir, chown, dan chmod pada struct fuse_operations. Dibebaskan jika ingin menambahkan fungsi lain selain 9 fungsi diatas.

### Langkah penyelesaian

- [ ] Membuat program Fuse untuk link dengan folder Storage.
- [ ] Membuat 9 fungsi yang diminta.

### Jawaban

Berdasarkan langkah penyelesaian, berikut 9 fungsi yang saya kerjakan :

1. Fungsi getattr.

```c
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
```

- `my_getattr` dideklarasikan dengan dua parameter: path yang menunjukkan path file atau direktori, dan `stbuf` yang merupakan pointer ke struktur stat yang akan diisi dengan informasi tentang file tersebut.
- Variabel `res` diinisialisasi dengan nilai 0. Ini akan digunakan untuk menyimpan hasil pemanggilan fungsi `lstat`.
- `sprintf` digunakan untuk menggabungkan direktori target (target_dir) dengan path yang diberikan, dan hasilnya disimpan dalam `fpath`.
- `lstat` dipanggil dengan `fpath` dan `stbuf`. `lstat` mengisi stbuf dengan informasi tentang file atau direktori yang ditunjuk oleh `fpath`. Hasil pemanggilan `lstat` disimpan dalam res

2. Fungsi readdir.

```c
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
```

- Fungsi menggabungkan `target_dir` dengan path untuk membentuk path lengkap (fpath), kemudian mencoba membuka direktori tersebut menggunakan `opendir`. Jika gagal, fungsi mengembalikan kode kesalahan.
- Fungsi membaca setiap entri dalam direktori menggunakan `readdir`. Untuk setiap entri, struktur stat diinisialisasi dan informasi entri dimasukkan ke dalam buffer `buf` menggunakan fungsi `filler`.
- Setelah semua entri dibaca, direktori ditutup dengan `closedir` dan fungsi mengembalikan 0 untuk menandakan operasi berhasil.

3. Fungsi read

```c
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
```

- Fungsi menggabungkan target_dir dengan path untuk membentuk path lengkap (fpath). Kemudian, fungsi mencoba membuka file tersebut dalam mode baca (O_RDONLY) menggunakan open. Jika gagal, fungsi mengembalikan kode kesalahan.
- Setelah file berhasil dibuka, fungsi membaca data dari file menggunakan pread. Data yang dibaca dimasukkan ke dalam buffer buf dengan ukuran maksimal size, dimulai dari offset yang diberikan.
- Setelah selesai membaca, file ditutup menggunakan close, dan fungsi mengembalikan nilai hasil baca (res). Jika tidak ada kesalahan, nilai res adalah jumlah byte yang berhasil dibaca; jika ada kesalahan, nilai res akan berupa kode kesalahan yang sesuai.

4. Fungsi mkdir

```c
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
```

- Fungsi menggabungkan target_dir dengan path untuk membentuk path lengkap (fpath). Kemudian, fungsi memeriksa apakah nama direktori yang akan dibuat adalah "trash" atau "restore" dengan menggunakan fungsi basename. Jika ya, fungsi menuliskan pesan ke log dan mengembalikan kode kesalahan -EPERM.
- Jika nama direktori tidak "trash" atau "restore", fungsi mencoba membuat direktori baru menggunakan mkdir dengan mode yang diberikan.
- Jika pembuatan direktori berhasil, fungsi mengembalikan nilai 0 untuk menandakan operasi berhasil.

5. Fungsi unlink

```c
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
```

- Fungsi menggabungkan target_dir dengan path untuk membentuk path lengkap (fpath). Kemudian, fungsi memeriksa apakah path file berada dalam direktori "trash" dengan menggunakan strstr.
- Jika file berada dalam "trash", fungsi memanggil delete_from_trash untuk menghapus file dari "trash". Jika gagal menghapus, fungsi mengembalikan kode kesalahan.
- Jika file tidak berada dalam "trash", fungsi memindahkan file ke "trash" menggunakan move_to_trash. Jika gagal memindahkan, fungsi mengembalikan kode kesalahan.
- Jika operasi berhasil, fungsi mengembalikan nilai 0 untuk menandakan operasi berhasil.

6. Fungsi rename

```c
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
```

- Fungsi membuat path lengkap untuk file lama (old_fpath) dan file baru (new_fpath) dengan menggabungkan target_dir dengan masing-masing oldpath dan newpath.
- Jika newpath adalah "/trash/restore", fungsi mengambil path asli file dari metadata, memindahkan file ke lokasi baru, mengembalikan izin file asli, dan menghapus metadata.
- Jika newpath adalah "/trash/", fungsi mencegah operasi dan mengembalikan kode kesalahan.
- Jika oldpath adalah "/trash/", fungsi mengembalikan izin file yang sebelumnya diubah ke mode aslinya.
- Jika oldpath adalah direktori sampah dan lokasinya berubah, fungsi mencegah operasi dan mengembalikan kode kesalahan.
- Fungsi memeriksa status file menggunakan stat dan menyimpan informasi izin file.
- Fungsi menggunakan rename untuk mengganti nama file, kemudian memperbaiki izin file dengan menggunakan chmod.
- Jika operasi gagal, fungsi menulis log dan mengembalikan kode kesalahan.

7. Fungsi rmdir

```c
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
```

- Fungsi menggabungkan target_dir dengan path untuk membentuk path lengkap (fpath). Kemudian, fungsi memeriksa apakah direktori yang akan dihapus adalah direktori "trash". Jika ya, fungsi menulis pesan log dan mengembalikan kode kesalahan -EPERM.
- Jika direktori tidak berada dalam "trash", fungsi memeriksa apakah direktori tersebut berada dalam "trash". Jika ya, fungsi memanggil delete_from_trash untuk menghapus direktori dari "trash". Jika gagal, fungsi mengembalikan kode kesalahan.
- Jika direktori tidak berada dalam "trash", fungsi memindahkan direktori ke "trash" menggunakan move_to_trash. Jika gagal, fungsi mengembalikan kode kesalahan.

8. Fungsi chmod

```c
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
```

- Fungsi menggabungkan target_dir dengan path untuk membentuk path lengkap (fpath). Kemudian, fungsi memeriksa apakah file yang akan diubah izinnya berada dalam direktori "trash". Jika ya, fungsi menulis pesan log dan mengembalikan kode kesalahan -EPERM.
- Jika file tidak berada dalam "trash", fungsi menggunakan chmod untuk mengubah izin file sesuai dengan mode yang diberikan.
- Jika gagal mengubah izin file, fungsi mengembalikan kode kesalahan.

9. Fungsi chown

```c
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
```

- Jika file tidak berada dalam "trash", fungsi menggunakan chown untuk mengubah kepemilikan file sesuai dengan uid dan gid yang diberikan.
- Jika gagal mengubah kepemilikan file, fungsi mengembalikan kode kesalahan.

### Hasil Output dan SS

- Fungsi mkdir
  ![1 mkdir](https://github.com/sisop-its-s24/praktikum-modul-4-f01/assets/144328529/d53e7e35-05bd-4b68-9b17-072fa18a1e13)

- Fungsi rmdir
  ![1 rmdir](https://github.com/sisop-its-s24/praktikum-modul-4-f01/assets/144328529/8232c8db-b693-4281-8206-334224f3303b)

- Fumgsi read
  ![1 read](https://github.com/sisop-its-s24/praktikum-modul-4-f01/assets/144328529/c27de08a-b724-49b3-b751-6b0aa6af6d52)

- Fungsi rename
  ![1 rename](https://github.com/sisop-its-s24/praktikum-modul-4-f01/assets/144328529/e7b97d97-2116-4e46-b877-478d5767569a)

- Fungsi chmod
  ![1 chmod](https://github.com/sisop-its-s24/praktikum-modul-4-f01/assets/144328529/5fe1ddf1-8f9a-4bd1-80b7-e38b5a14a240)

### Kendala

- Tidak ada kendala yang saya alami selama mengerjakan soal a ini.

## 1B

Ketika menggunakan perintah rm atau rmdir untuk file atau direktori yang berada diluar direktori trash, maka file atau direktori tersebut akan dipindahkan ke direktori trash dan menjadi tidak dapat diread, write, dan execute baik oleh pemilik, grup, maupun user lainnya.

Barulah ketika perintah rm atau rmdir digunakan untuk file atau direktori yang berada di dalam direktori trash, maka file atau direktori tersebut akan dihapus permanen.

### Langkah penyelesaian

- [ ] Memodifikasi fungsi rmdir.
- [ ] Menambahkan fungsi `move_to_trash`.
- [ ] Menambahkan fungsi `delete_from_trash`.

### Jawaban

Berdasarkan langkah penyelesaian, berikut code yang saya kerjakan :

1. Memodifikasi fungsi rmdir seperti di no 1 a.

2. Menambahkan fungsi `move_to_trash`.

```c
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
```

- Fungsi memeriksa status file yang akan dipindahkan ke tempat sampah menggunakan stat. Jika gagal, fungsi menampilkan pesan kesalahan dan mengembalikan -1.
- Fungsi membangun path lengkap untuk tempat sampah dengan menggunakan direktori tempat sampah (trash_dir) dan nama file yang diperoleh melalui fungsi basename.
- Fungsi membangun perintah mv untuk memindahkan file ke tempat sampah. Perintah ini dieksekusi menggunakan system.
- Jika gagal memindahkan file, fungsi menampilkan pesan kesalahan dan mengembalikan -1.
- Setelah pemindahan berhasil, izin file diatur menjadi 0000 agar tidak dapat diakses.
- Jika gagal menetapkan izin, fungsi menampilkan pesan kesalahan dan mengembalikan -1.
- Fungsi membuat metadata untuk file yang dipindahkan ke tempat sampah.

3. Menambahkan fungsi `delete_from_trash`.

```c
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
```

- Fungsi membangun perintah rm -rf untuk menghapus file atau direktori secara rekursif.
- Perintah ini dieksekusi menggunakan system.
- Jika gagal menghapus dari tempat sampah, fungsi menampilkan pesan kesalahan dan mengembalikan -1.
- Setelah penghapusan berhasil, fungsi menulis pesan log dan mengembalikan nilai 0 untuk menandakan operasi berhasil.
- Fungsi juga menghapus file metadata terkait jika yang dihapus adalah file atau direktori yang berada di dalam direktori sampah.

### Hasil Output dan SS

- Move to bin
  ![1 rmdir](https://github.com/sisop-its-s24/praktikum-modul-4-f01/assets/144328529/5171e5f4-9cdb-45e7-a5ad-d8574ade61a2)

- Delete permanently
  ![2 delete permanen](https://github.com/sisop-its-s24/praktikum-modul-4-f01/assets/144328529/92fa4ab4-0697-468e-b54a-fb677b5db0ac)

### Kendala

- Tidak ada kendala

## 1C

File atau direktori yang berada pada direktori trash tidak dapat diubah permission dan kepemilikannya, serta tidak dapat direname.

### Langkah penyelesaian

- [ ] Memodifikasi fungsi `my_rename`, `my_chmod`, dan `my_chown` dengan parameter berada di direktori `trash`.

### Jawaban

Fungsi `my_rename`, `my_chmod`, dan `my_chown` di no 1 a sudah sesuai untuk soal ini.

### Hasil Output dan SS

- Bukti ss
  ![3](https://github.com/sisop-its-s24/praktikum-modul-4-f01/assets/144328529/5c96ab88-9a49-422e-8cb3-178c181109e9)

### Kendala

- Tidak ada kendala

## 1D

Untuk memulihkan file atau direktori dari direktori trash, anda harus menggunakan perintah mv dengan format mv [path_file_dalam_trash] [arg]. Opsi pertama untuk arg adalah dengan path biasa sehingga file atau direktori akan dipindahkan dari direktori trash ke path tersebut. Opsi kedua untuk arg adalah restore sehingga file atau direktori akan kembali ke path asal sebelum ia dipindah ke trash. Permission untuk file atau direktori yang dipulihkan harus kembali seperti sebelum dimasukkan ke trash. Khusus untuk arg restore anda harus membuat path yang sesuai apabila path asal tidak ada (terhapus/dipindah).

### Langkah penyelesaian

- [ ] Memodifikasi fungsi `my_rename` agar dapat menerima `restore` dan memindahkan file atau folder keluar `trash`.
- [ ] Membuat metadata untuk menyimpan path origin dan permission awal.

### Jawaban

1. Fungsi `my_rename` di no 1 a sudah sesuai untuk soal ini.

2. Menambahkan fungsi `create_metadata`.

```c
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
```

- Fungsi membuat path lengkap untuk file metadata dengan menambahkan ekstensi ".metadata" pada path baru yang diberikan.
- Fungsi membuka file metadata dengan mode "w" (tulis saja) menggunakan fungsi fopen. Jika gagal membuka file, fungsi mengembalikan kode kesalahan.
- Setelah berhasil membuka file, fungsi menuliskan path asli dan mode file ke dalam file metadata menggunakan fprintf. Mode file diubah menjadi representasi oktal dan disimpan dalam file metadata.
- Setelah selesai menulis informasi, file ditutup.

3. Menambahkan fungsi `read_metadata`

```c
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
```

- Fungsi membuka file metadata dengan mode "r" (baca saja) menggunakan fungsi fopen. Jika gagal membuka file, fungsi mengembalikan kode kesalahan.
- Fungsi membaca baris pertama dari file metadata menggunakan fgets. Jika tidak dapat membaca baris, fungsi menutup file dan mengembalikan kode kesalahan.
- Setelah membaca baris, fungsi mencari posisi spasi pertama menggunakan strchr. Jika tidak ditemukan, fungsi mengembalikan kode kesalahan.
- Fungsi memisahkan baris menjadi dua bagian: path asli dan mode file.
- Path asli disalin ke original_path, dan mode file diubah menjadi nilai bilangan dan disimpan dalam original_mode.
- Setelah selesai membaca dan memproses metadata, file ditutup.

4. Menambahkan fungsi `create_directory_recursive`

```c
static void create_directory_recursive(char *path) {
    char *dir = dirname(strdup(path));
    if (access(dir, F_OK) == -1) {
        create_directory_recursive(dir);
        mkdir(dir, 0755);
    }
    free(dir);
}
```

- Fungsi bertujuan untuk membuat direktori secara rekursif berdasarkan path yang diberikan.
- Pertama, fungsi menggunakan fungsi dirname untuk mendapatkan direktori dari path yang diberikan.
- Selanjutnya, fungsi memeriksa apakah direktori tersebut sudah ada menggunakan access dengan mode F_OK. Jika tidak ada, fungsi akan melakukan rekursi dengan memanggil dirinya sendiri (create_directory_recursive) untuk membuat direktori induknya terlebih dahulu.
- Setelah itu, fungsi menggunakan mkdir untuk membuat direktori dengan izin 0755.
- Fungsi mengalokasikan memori untuk variabel dir menggunakan strdup. Setelah penggunaannya, memori tersebut perlu dibebaskan dengan menggunakan free.

### Hasil Output dan SS

- Menggunakan path
  ![4 path](https://github.com/sisop-its-s24/praktikum-modul-4-f01/assets/144328529/4ef482c7-99ea-487d-8544-f354d7cefc57)

- Menggunakan restore
  ![4 restore](https://github.com/sisop-its-s24/praktikum-modul-4-f01/assets/144328529/2362ae27-fc72-4e79-b302-6aa1a3ea644e)

### Kendala

- Tidak ada kendala.

## 1E

Direktori trash tidak dapat dihapus, dipindah, direname. Anda juga tidak dapat membuat direktori dengan nama trash atau restore.

### Langkah penyelesaian

- [ ] Menambahkan parameter jika itu trash pada fungsi `my_rmdir`dan `my_rename`.

### Jawaban

Fungsi `my_rmdir`dan `my_rename` di no 1 a sudah sesuai untuk soal ini.

### Hasil Output dan SS

- Bukti ss
  ![5](https://github.com/sisop-its-s24/praktikum-modul-4-f01/assets/144328529/fbe16f20-6356-47df-ac5b-9ced86460c9f)

### Kendala

- Tidak ada kendala.

## 1F

Catatlah log pada file trash.log. Format untuk tiap baris log adalah YYMMDD-HH:MM:SS KETERANGAN. Format keterangan akan berdasarkan aksi sebagai berikut:

| Aksi                                                                                                              | Keterangan                                           |
| ----------------------------------------------------------------------------------------------------------------- | ---------------------------------------------------- |
| Memindahkan file atau direktori ke direktori trash                                                                | `MOVED [path_awal] TO TRASH`                         |
| Menghapus file atau direktori secara permanen                                                                     | `PERMANENTLY REMOVED [path_di_trash]`                |
| Memulihkan file atau direktori dari direktori trash                                                               | `RESTORED [path_di_trash] FROM TRASH TO [path_baru]` |
| Percobaan mengubah permission, mengubah kepemilikan, atau merename file atau direktori yang berada di dalam trash | `FAILED TO [CHMOD/CHOWN/RENAME][path_di_trash]`      |
| Percobaan menghapus, memindah, merename direktori trash                                                           | `FAILED TO [REMOVE/MOVE/RENAME] TRASH DIRECTORY`     |
| Percobaan membuat direktori dengan nama 'trash' atau 'restore'                                                    | `FAILED TO CREATE [TRASH/RESTORE] DIRECTORY`         |

### Langkah penyelesaian

- [ ] Menambahkan fungsi `write_log` dan membuat fungsi-fungsi lain menuliskan lognya.

### Jawaban

1. Fungsi `write_log`.

```c
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
```

- Fungsi membuka file log untuk ditambahkan teks baru dengan mode "a" (append) menggunakan fopen. Jika gagal membuka file, fungsi menampilkan pesan kesalahan.
- Fungsi mendapatkan waktu saat ini menggunakan time dan mengonversi waktu tersebut ke dalam bentuk string dengan format yang sesuai menggunakan localtime dan strftime.
- Fungsi menuliskan entri log berdasarkan aksi yang dilakukan. Aksi yang didukung antara lain "MOVED", "PERMANENTLY REMOVED", "RESTORED", dan berbagai pesan kesalahan yang mungkin terjadi.

### Hasil Output dan SS

- Hasil log
  ![6](https://github.com/sisop-its-s24/praktikum-modul-4-f01/assets/144328529/264a07dd-f406-4bc0-9c44-43e8bb200641)

### Kendala

- Tidak ada kendala.

### Revisi Keseluruhan

Soal 1 d dan f adalah revisi.

# 🐰🐇 Task-2 Fu53enC0d3D==

> ## 2A

- Pada 2A diperintahkan untuk mengeclone target directory yang berisi sebuah folder dan membuat 4 user pada linux dengan username sesuai dengan nama folder.

- ### Logic Penyelesaian
- [ ] Clone folder pada link
- [ ] Membuat 4 user dengan username sesuai dengan nama folder
- [ ] Membuat password dari masing-masing user

- Berikut penjelasan dari code untuk membuat user :

```c
sudo adduser andi
sudo adduser budi
sudo adduser cony
sudo adduser deni
echo "andi:nabhylahehe1" | sudo chpasswd
echo "budi:nabhylahehe2" | sudo chpasswd
echo "cony:nabhylahehe3" | sudo chpasswd
echo "deni:nabhylahehe4" | sudo chpasswd
```

Menggunakan `sudo adduser` untuk membuat user dengan nama user `andi` dengan password yaitu `nabhylahehe1`. Kemudian untuk user berikutnya dilakukan hal yang sama yaitu user budi, cony, dan deni dengan memasukan passwordnya masing-masing.

> ## Output Code
>
> ![image](https://github.com/sisop-its-s24/praktikum-modul-4-f01/assets/116788471/1a484098-abfa-4032-b3f2-c0c569bc310e)

> ## 2B

- Pada 2B folder mount point dari fuse yang kalian buat diakses, akan langsung menuju ke dalam target folder yang telah di clone (tidak dimulai dari root).

- ### Logic Penyelesaian
- [ ] Membuat mount point
- [ ] Membuat dirpath untuk menuju folder mount point
- [ ] Membuat fuse

- Berikut penjelasan dari code fuse.c :

  ```c
  static  int  xmp_getattr(const char *path, struct stat *stbuf)
  {
      int res;
      char fpath[1000];

      sprintf(fpath,"%s%s",dirpath,path);

      res = lstat(fpath, stbuf);

      if (res == -1) return -errno;

      return 0;
  }
  ```

  Mendapatkan atribut menggunakan xmp_getattr dari file/directory seperti size, permission, dan lain lain. Code ini diambil dari github pada folder playground.

  ```c
  static int xmp_readdir(const char *path, void *buf, fuse_fill_dir_t filler, off_t offset, struct fuse_file_info *fi) {
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
  ```

  Menggunakan xmp_readdir untuk membaca dari isi directory. Code ini juga diambil dari github di folder playground.

> ## Output Code
>
> ![image](https://github.com/sisop-its-s24/praktikum-modul-4-f01/assets/116788471/c4b5597b-ec49-4df8-9f65-65b2a2f74e9a)

> ## 2C dan 2D

- Pada soal 2C dan 2D tiap user tidak dapat menambah, mengubah, maupun menghapus folder maupun file dalam folder yang bukan miliknya. Kemudian buat agar user dapat menambah, mengubah, maupun menghapus folder maupun file dalam folder miliknya. Tambahkan juga untuk perintah mkdir, rmdir, touch, cp, rm, dan mv.

- ### Logic Penyelesaian
- [ ] Membuat fungsi mkdir, rmdir, touch, cp, rm, dan mv
- [ ] Membuat fungsi untuk mengecek user yang sedang berjalan dan fungsi mengambil nama directory
- [ ] Membuat fungsi untuk mengecek apakah nama user sesuai dengan nama top folder directory.

- Berikut penjelasan dari code fuse.c :

```c
char *current_username() {
    struct fuse_context *context = fuse_get_context();
    struct passwd *pw = getpwuid(context->uid);
    if (pw == NULL) {
        perror("getpwuid");
        exit(EXIT_FAILURE);
    }
    return pw->pw_name;
}
```

Membuat fungsi mengambil username. Menggunakan `fuse_get_context` untuk mengambil user yang sedang login/berjalan. Kemudian mengambil id dan me-return username dari usern yang sedang login.

```c
char *top_level_dir(const char *path) {
    char *path_copy = strdup(path);
    char *top_dir = NULL;

    // Tokenize the path and get the first directory
    char *token = strtok(path_copy, "/");
    if (token != NULL) {
        top_dir = strdup(token);
    }

    free(path_copy);
    return top_dir;
}
```

Membuat fungsi untuk mengambil nama dari top directory. ` char *path_copy = strdup(path);` menduplikasi path string untuk menghindari perubahan dari originalnya. `char *token = strtok(path_copy, "/");` split path string menggunakan strtok dengan delimiternya adalah `/` dan mengambil first tokennya. `top_dir = strdup(token);` duplikasi first token dan simpan dalam variabel top_dir dan return top dir.

```c
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
```

Membuat fungsi is_owner untuk mengecek apakah bisa melakukan command-command sesuai perintah atau tidak. Dengan mengambil username dari fungsi current_username dan ambil top_dir dari fungsi top_level_dir. Kemudian bandingkan menggunakan `strcmp` dan simpan hasilnya di result. Return result yaitu hasil dari strcmp.

> ## Output Code
>
> ![image](https://github.com/sisop-its-s24/praktikum-modul-4-f01/assets/116788471/462abecc-f860-4721-9b6f-31843b54dc04) > ![image](https://github.com/sisop-its-s24/praktikum-modul-4-f01/assets/116788471/fd8dc483-3577-43c7-b05d-d42f0b904fc0)

> ## 2E dan 2F

- Pada soal 2E dan 2F semua isi file akan ter-encode jika diakses oleh selain user pemilik folder tersebut kemudian file akan ter-decode ketika diakses oleh user pemilik folder tersebut.

- ### Logic Penyelesaian
- [ ] Membuat fungsi encode
- [ ] Mengubah fungsi xmp_read

- Berikut penjelasan dari code fuse.c :

```c
char* base64Encoder(const char *input_str, int len_str, int *output_len) {
    int buffer_size = len_str * 2;
    char *encoded_str = (char *) malloc(buffer_size * sizeof(char));
    if (encoded_str == NULL) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    base64_encodestate state
    base64_init_encodestate(&state);
    int encoded_length = base64_encode_block(input_str, len_str, encoded_str, &state);
    encoded_length += base64_encode_blockend(encoded_str + encoded_length, &state);
    encoded_str[encoded_length] = '\0';

    *output_len = encoded_length;
    return encoded_str;
}
```

Membuat buffer_size yaitu 2x dari size input string. Alokasi memory untuk encoded string menggunakan malloc. Membuat encoding state untuk status encoding. Inisialisasi encoding state. Menggunakan encode block untuk mengencode blok data dari input string ke encoded string yang disimpan di encoded_length. Menambahkan block end untuk menandakan akhir dari encoding ke encoded string.

```c
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

    // Read the data from the file
    res = pread(fd, buf, size, offset);

    if (res == -1) {
        close(fd);
        return -errno;
    }

    close(fd);

    // Check if the accessing user is the owner
    bool isOwner = is_owner(path);

    // Encode the data using Base64 if accessed by a non-owner
    if (!isOwner) {
        int encoded_size = 0;
        char *encoded_data = base64Encoder(buf, res, &encoded_size); // encode data
        if (encoded_size <= size) {
            memcpy(buf, encoded_data, encoded_size); // copy encoded data ke buffer
            res = encoded_size; // set res ke encoded size
        } else {
            res = -EFBIG; // set res ke EFBIG yaitu file too big
        }
        free(encoded_data); // free encoded data
    }

    return res;
}
```

Memodifikasi fungsi read agar menghandle ketika bukan owner maka code akan di encode dan ketika owner maka decode dengan membaca file aslinya.

> ## Output Code
>
> ![image](https://github.com/sisop-its-s24/praktikum-modul-4-f01/assets/116788471/ca7b48fe-b967-4fb3-a4cc-8398ff721c00)

> ## Kendala Pengerjaan :

- [ ] Kesulitan saat melakukan encode dan access namun sudah teratasi.
- [ ] Tidak ada revisi.

> ## Full Code :

```c
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
    struct fuse_context *context = fuse_get_context(); //ngeambil informasi user yang sedang login/ menjalankan program
    struct passwd *pw = getpwuid(context->uid); // ngeambil user information berdasarkan user id
    if (pw == NULL) {
        perror("getpwuid");
        exit(EXIT_FAILURE);
    }
    return pw->pw_name; // ngereturn username dari user yang sedang login
}

char *top_level_dir(const char *path) {
    char *path_copy = strdup(path); // Duplicate the path string to avoid modifying the original
    char *top_dir = NULL; // Initialize a pointer to store the top-level directory

    // Tokenize the path and get the first directory
    char *token = strtok(path_copy, "/"); // Split the path string using strtok by '/' and get the first token
    if (token != NULL) {
        top_dir = strdup(token); // Duplicate the first token and store it in top_dir
    }

    free(path_copy); // Free the duplicated path string
    return top_dir; // Return the top-level directory
}

//fungsi untuk mengecek apakah nama dari superuser sama dengan nama dari parent directory
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

//mendapatkan atribut dari file/directory seperti size, permission, dan lain-lain
static  int  xmp_getattr(const char *path, struct stat *stbuf)
{
    int res;
    char fpath[1000];

    sprintf(fpath,"%s%s",dirpath,path);

    res = lstat(fpath, stbuf);

    if (res == -1) return -errno;

    return 0;
}

//membaca isi dari directory
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
    int buffer_size = len_str * 2; // Base64-encoded string will be at most 2x the size of the input string
    char *encoded_str = (char *) malloc(buffer_size * sizeof(char)); // Allocate memory for the encoded string
    if (encoded_str == NULL) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    base64_encodestate state; // Create an encoding state, untuk status encoding
    base64_init_encodestate(&state); // Initialize the encoding state
    // menggunakan encode block untuk mengencode blok data dari input string ke encoded string
    int encoded_length = base64_encode_block(input_str, len_str, encoded_str, &state);
    encoded_length += base64_encode_blockend(encoded_str + encoded_length, &state); // menambahkan block end untuk menandakan akhir dari encoding ke encoded string
    encoded_str[encoded_length] = '\0'; // Null-terminate the encoded string

    *output_len = encoded_length; // Set the output length to the length of the encoded string
    return encoded_str;
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
        res = pwrite(fd, encoded_data, encoded_size, offset); // tulis dengan encode
        free(encoded_data);
    } else {
        // Write the data to the file as is
        res = pwrite(fd, buf, size, offset); // tulis tanpa encode
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

    // Read the data from the file
    res = pread(fd, buf, size, offset);

    if (res == -1) {
        close(fd);
        return -errno;
    }

    close(fd);

    // Check if the accessing user is the owner
    bool isOwner = is_owner(path);

    // Encode the data using Base64 if accessed by a non-owner
    if (!isOwner) {
        int encoded_size = 0;
        char *encoded_data = base64Encoder(buf, res, &encoded_size); // encode data
        if (encoded_size <= size) {
            memcpy(buf, encoded_data, encoded_size); // copy encoded data ke buffer
            res = encoded_size; // set res ke encoded size
        } else {
            res = -EFBIG; // set res ke EFBIG yaitu file too big
        }
        free(encoded_data); // free encoded data
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
    printf("DEBUG: mkdir mode: %d\n", mode);

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
    sprintf(fpath, "%s%s", dirpath, path); //menggabungkan dirpath dan path menjadi fpath yaitu full path

    int res = creat(fpath, mode);

    if (res == -1) return -errno;

    fi->fh = res; //set file handler untuk file info
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

        int res = utimensat(0, fpath, ts, AT_SYMLINK_NOFOLLOW); // Update the timestamps of the file
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
```

# 📚 Nomor 3 MengOS

## 3A

- a. Implementasikan fungsi `printString` pada `kernel.c` yang akan menampilkan string ke layar.

  Implementasi dapat menggunakan fungsi `interrupt` dengan service `int 10h` dengan parameter `AH = 0x0E` untuk _teletype output_. Karakter yang ditampilkan dapat dimasukkan pada register `AL`. Fungsi ini akan menampilkan string karakter ASCII (_null-terminated_) ke layar.

### Jawab

source code yang digunakan

```
void printString(char *str)
{
  // used as a counter
  int i = 0;
  // print until we hit the end
  while (str[i] != '\0')
  {
    char ah = 0x0E;
    // calculate ax
    int ax;
    // check for the \n or \r (need to print both to get a new line)
    if (str[i] == 0x0D || str[i] == 0x0A)
    {
      ax = ah * 256 + 0x0A;
      interrupt(0x10, ax, 0, 0, 0);
      ax = ah * 256 + 0x0D;
      interrupt(0x10, ax, 0, 0, 0);
    }
    else
    {
      // print the character
      ax = ah * 256 + str[i];
      interrupt(0x10, ax, 0, 0, 0);
    }
    i++;
  }
}
```

### Penjelasan

1. Variabel counter `int i = 0;`. i: Variabel counter yang digunakan untuk increment saat loop string.

2. Lalu loop untuk mencetak string `while (str[i] != '\0')`. Loop berjalan sampai mencapai karakter null ('\0'), yang menunjukkan akhir string.

3. Variabel untuk Mengatur Interupsi BIOS

```
char ah = 0x0E;
int ax;
```

- ah: Ditetapkan ke 0x0E, yang merupakan layanan BIOS untuk menampilkan karakter di layar dalam mode teks.

- ax: Register yang digunakan untuk menyimpan kombinasi nilai ah dan karakter yang akan dicetak.

4. Memeriksa Karakter Newline dan Carriage Return `if (str[i] == 0x0D || str[i] == 0x0A)`

- Memeriksa apakah karakter saat ini adalah carriage return (0x0D) atau newline (0x0A).

5. Mencetak Newline dan Carriage Return

```
ax = ah * 256 + 0x0A;
interrupt(0x10, ax, 0, 0, 0);
ax = ah * 256 + 0x0D;
interrupt(0x10, ax, 0, 0, 0);
```

- ax = ah \_ 256 + 0x0A: Mengatur ax untuk mencetak newline.

- interrupt(0x10, ax, 0, 0, 0): Memanggil interrupt BIOS 0x10 dengan nilai ax untuk mencetak newline.

- ax = ah \_ 256 + 0x0D: Mengatur ax untuk mencetak carriage return.

- interrupt(0x10, ax, 0, 0, 0): Memanggil interrupt BIOS 0x10 dengan nilai ax untuk mencetak carriage return.

6. Mencetak Karakter Lainnya

```
ax = ah * 256 + str[i];
interrupt(0x10, ax, 0, 0, 0);
```

- ax = ah \* 256 + str[i]: Mengatur ax untuk mencetak karakter saat ini.

- interrupt(0x10, ax, 0, 0, 0): Memanggil interrupt BIOS 0x10 dengan nilai ax untuk mencetak karakter tersebut.

7. Increment Counter

- `i++;`

Contoh Eksekusi
Misalkan str adalah "Hello\nWorld":

H:

ax = 0x0E \* 256 + 'H'
interrupt(0x10, ax, 0, 0, 0) mencetak H.
e:

ax = 0x0E \* 256 + 'e'
interrupt(0x10, ax, 0, 0, 0) mencetak e.
l:

ax = 0x0E \* 256 + 'l'
interrupt(0x10, ax, 0, 0, 0) mencetak l.
l:

ax = 0x0E \* 256 + 'l'
interrupt(0x10, ax, 0, 0, 0) mencetak l.
o:

ax = 0x0E \* 256 + 'o'
interrupt(0x10, ax, 0, 0, 0) mencetak o.
\n (newline):

ax = 0x0E _ 256 + 0x0A
interrupt(0x10, ax, 0, 0, 0) mencetak newline.
ax = 0x0E _ 256 + 0x0D
interrupt(0x10, ax, 0, 0, 0) mencetak carriage return.
W:

ax = 0x0E \* 256 + 'W'
interrupt(0x10, ax, 0, 0, 0) mencetak W.
o:

ax = 0x0E \* 256 + 'o'
interrupt(0x10, ax, 0, 0, 0) mencetak o.
r:

ax = 0x0E \* 256 + 'r'
interrupt(0x10, ax, 0, 0, 0) mencetak r.
l:

ax = 0x0E \* 256 + 'l'
interrupt(0x10, ax, 0, 0, 0) mencetak l.
d:

ax = 0x0E \* 256 + 'd'
interrupt(0x10, ax, 0, 0, 0) mencetak d.

### Revisi

- Tidak ada

## 3B

- b. Implementasikan fungsi `readString` pada `kernel.c` yang akan membaca string dari keyboard.

  Implementasi dapat menggunakan fungsi `interrupt` dengan service `int 16h` dengan parameter `AH = 0x00` untuk _keyboard input_. Fungsi ini akan membaca karakter ASCII (_non-control_) yang dimasukkan oleh pengguna dan menyimpannya pada buffer hingga menekan tombol `Enter`. Handle tombol `Backspace` dibebaskan kepada praktikan.

### Jawab

source code yang digunakan

```

void readString(char *buf)
{
  int i = 0;
  char c = 0;
  char ah = 0x0E;
  int ax;

  while (true)
  {
    c = interrupt(0x16, 0, 0, 0, 0);
    if (c == 0x0D)
    { // Enter key
      ax = ah * 256 + 0x0A;
      interrupt(0x10, ax, 0, 0, 0);
      ax = ah * 256 + 0x0D;
      interrupt(0x10, ax, 0, 0, 0);
      buf[i] = '\0';
      break;
    }
    else if (c == 0x08 && i > 0)
    { // Backspace
      buf[i] = 0;
      i--;
      printString("\b \b"); // Erase character from screen
    }
    else if (c != 0x08)
    { // Regular character
      buf[i] = c;
      i++;
      interrupt(0x10, 0x0E00 | c, 0, 0, 0); // Echo character
    }
  }
}
```

### Penjelasan

1. Deklarasi dan Header

- `void printString(char *str)`

- void readString(char \*buf)

- void: Menunjukkan bahwa fungsi ini tidak mengembalikan nilai.

- readString: Nama fungsi.

- char \*buf: Parameter input, sebuah pointer ke buffer tempat string yang dibaca akan disimpan.

2. Variabel Counter dan Kontrol

- `int i = 0;`

- i: Variabel counter yang digunakan untuk melintasi string karakter.

- ```
  int i = 0;
  char c = 0;
  char ah = 0x0E;
  int ax;
  ```

- i: Variabel counter yang digunakan untuk melintasi buffer.

- c: Variabel untuk menyimpan karakter yang dibaca dari keyboard.

- ah: Ditetapkan ke 0x0E, yang merupakan layanan BIOS untuk menampilkan karakter di layar dalam mode teks.

- ax: Register yang digunakan untuk menyimpan kombinasi nilai ah dan karakter yang akan dicetak.

3. Loop untuk Membaca Input Keyboard

- `while (true)`

- Loop berjalan tanpa henti sampai dihentikan secara eksplisit dengan break.

4. Membaca Karakter dari Keyboard

- `c = interrupt(0x16, 0, 0, 0, 0);`

- interrupt(0x16, 0, 0, 0, 0): Memanggil interrupt BIOS 0x16 untuk membaca karakter dari keyboard. Karakter yang dibaca disimpan dalam c.

5. Memeriksa Karakter Enter

```
if (c == 0x0D)
{
  ax = ah _ 256 + 0x0A;
  interrupt(0x10, ax, 0, 0, 0);
  ax = ah _ 256 + 0x0D;
  interrupt(0x10, ax, 0, 0, 0);
  buf[i] = '\0';
  break;
}
```

- if (c == 0x0D): Memeriksa apakah karakter yang dibaca adalah Enter (0x0D).

- ax = ah \_ 256 + 0x0A: Mengatur ax untuk mencetak newline (0x0A).

- interrupt(0x10, ax, 0, 0, 0): Memanggil interrupt BIOS 0x10 dengan nilai ax untuk mencetak newline.

- ax = ah \_ 256 + 0x0D: Mengatur ax untuk mencetak carriage return (0x0D).

- interrupt(0x10, ax, 0, 0, 0): Memanggil interrupt BIOS 0x10 dengan nilai ax untuk mencetak carriage return.

- buf[i] = '\0': Menandai akhir string di buffer dengan karakter null ('\0').

- break: Menghentikan loop.

6. Memeriksa Karakter Backspace

```
else if (c == 0x08 && i > 0)
{
 buf[i] = 0;
 i--;
 printString("\b \b");
}
```

- else if (c == 0x08 && i > 0): Memeriksa apakah karakter yang dibaca adalah Backspace (0x08) dan apakah ada karakter di buffer untuk dihapus.

- buf[i] = 0: Menghapus karakter terakhir dari buffer.
- i--: Mengurangi nilai i untuk menggeser posisi ke belakang.
- printString("\b \b"): Memanggil fungsi printString untuk menghapus karakter dari layar. "\b \b" adalah urutan escape untuk menggeser kursor ke belakang (\b), mencetak spasi ( ), dan menggeser kursor ke belakang lagi (\b) untuk menghapus karakter dari layar.

7. Memproses Karakter Biasa

```
else if (c != 0x08)
{
 buf[i] = c;
 i++;
 interrupt(0x10, 0x0E00 | c, 0, 0, 0);
}
```

- else if (c != 0x08): Memeriksa apakah karakter yang dibaca bukan Backspace.

- buf[i] = c: Menyimpan karakter yang dibaca ke buffer.

- i++: Meningkatkan nilai i untuk melanjutkan ke posisi berikutnya dalam buffer.

- interrupt(0x10, 0x0E00 | c, 0, 0, 0): Memanggil interrupt BIOS 0x10 dengan nilai 0x0E00 | c untuk mencetak karakter yang dibaca ke layar.

### Revisi

- Tidak ada

## 3C

- c. Implementasikan fungsi `clearScreen` pada `kernel.c` yang akan membersihkan layar.

  Ukuran layar adalah `80x25` karakter. Setelah layar dibersihkan, kursor akan kembali ke posisi awal yaitu `(0, 0)` dan buffer video untuk warna karakter diubah ke warna putih. Implementasi dapat menggunakan fungsi `interrupt` dengan service `int 10h` atau menggunakan fungsi `putInMemory` untuk mengisi memori video.

### Jawab

Source code yang digunakan:

```
void clearScreen()
{
  // first actually clear the screen
  byte ah = 6;    // scrolling mode
  byte al = 0x00; // clears cells
  byte bh = 0x0E; // black with yellow text (dino theme!)
  byte ch = 0x00; // start at row 0
  byte cl = 0x00; // start at col 0
  byte dh = 25;   // end at row index 24
  byte dl = 80;   // end at col index
  // place in ax, bx, cx, dx
  int ax = ah * 256 + al;
  int bx = bh * 256;
  int cx = ch * 256 + cl;
  int dx = dh * 256 + dl;
  interrupt(0x10, ah * 256 + al, bh * 256, cx, dx);

  // disable weird blinking thing
  interrupt(0x10, 0x1003, 0, 0, 0);
  // full block cursor
  // interrupt(0x10, 0x0100, 0, 0x00FF, dx);
  // place cursor back at the top
  interrupt(0x10, 0x0200, 0, 0, 0); // ah=0x02, bh=page, dh=row, dl=col
}
```

### Penjelasan

1. ah = 6: Menetapkan nilai 6 ke ah yang menunjukkan mode scroll untuk interrupt BIOS 0x10.

2. al = 0x00: Menetapkan nilai 0x00 ke al, yang menunjukkan bahwa semua sel layar akan dibersihkan.

3. bh = 0x0E: Menetapkan nilai 0x0E ke bh, yang menunjukkan atribut tampilan teks (teks kuning di latar belakang hitam).

4. ch = 0x00: Menetapkan nilai 0x00 ke ch, yang menunjukkan baris awal (baris 0).

5. cl = 0x00: Menetapkan nilai 0x00 ke cl, yang menunjukkan kolom awal (kolom 0).

6. dh = 25: Menetapkan nilai 25 ke dh, yang menunjukkan baris akhir (baris 24, karena baris diindeks dari 0).

7. dl = 80: Menetapkan nilai 80 ke dl, yang menunjukkan kolom akhir (kolom 79, karena kolom diindeks dari 0).

8. ax = ah \* 256 + al: Menggabungkan ah dan al menjadi satu nilai 16-bit untuk register ax.

9. bx = bh \* 256: Menggabungkan bh dengan nilai 0 untuk register bx.

10. cx = ch \* 256 + cl: Menggabungkan ch dan cl menjadi satu nilai 16-bit untuk register cx.

11. dx = dh \* 256 + dl: Menggabungkan dh dan dl menjadi satu nilai 16-bit untuk register dx.

12. interrupt(0x10, ah _ 256 + al, bh _ 256, cx, dx): Memanggil interrupt BIOS 0x10 dengan parameter yang telah diatur untuk membersihkan layar. Nilai ah _ 256 + al menetapkan fungsi scroll dengan clear cells, bh _ 256 menetapkan atribut teks, dan cx serta dx menetapkan area layar yang akan dibersihkan.

13. interrupt(0x10, 0x1003, 0, 0, 0): Memanggil interrupt BIOS 0x10 dengan fungsi 0x1003 untuk menonaktifkan efek blinking pada teks.

14. interrupt(0x10, 0x0200, 0, 0, 0): Memanggil interrupt BIOS 0x10 dengan fungsi 0x02 untuk mengatur posisi kursor kembali ke baris 0, kolom 0.

### Revisi

- Tidak ada

## 3D

- d. Lengkapi implementasi fungsi yang terdapat pada [`std_lib.h`](./include/std_lib.h) pada [`std_lib.c`](./src/std_lib.c).

  Fungsi-fungsi di atas dapat digunakan untuk mempermudah implementasi fungsi `printString`, `readString`, dan `clearScreen`.

### Jawab

source code yang digunakan

```
#include "std_lib.h"

int div(int a, int b)
{
    if (a >= b)
    {
        return a / b;
    }

    return b / a;
}

int mod(int a, int b)
{
    if (a >= b)
    {
        int resultDiv = a / b;
        int resultMultiple = b * resultDiv;
        return a - resultMultiple;
    }

    return a;
}

void memcpy(byte *src, byte *dst, unsigned int size)
{
    unsigned int i;
    char *csrc = (char *)src;
    char *cdest = (char *)dst;

    for (i = 0; i < size; size++)
        cdest[i] = csrc[i];
}

int strlen(char *str)
{
    unsigned int length = 0;
    // Loop until the null terminator is encountered
    while (str[length] != '\0')
    {
        length++;
    }
    return length;
}

bool strcmp(char *str1, char *str2)
{
    // Loop through both strings until the end of either string is encountered
    while (*str1 != '\0' && *str2 != '\0')
    {
        // If characters at current position do not match, return false
        if (*str1 != *str2)
        {
            return false;
        }
        // Move to the next character in both strings
        str1++;
        str2++;
    }

    // If both strings reached the end, they are equal
    return *str1 == '\0' && *str2 == '\0';
}

void strcpy(char *dst, char *src)
{
    // Loop through each character of the source string
    while (*src != '\0')
    {
        *dst = *src; // Copy the character from src to dst
        dst++;       // Move to the next position in dst
        src++;       // Move to the next position in src
    }
    *dst = '\0'; // Add the null terminator at the end of dst
}

void clear(byte *buf, unsigned int size)
{
    unsigned int i;
    // Loop through each byte in the buffer
    for (i = 0; i < size; i++)
    {
        buf[i] = 0; // Set the current byte to zero
    }
}
```

### Penjelasan

1. Untuk function div sendiri

- if (a >= b): Jika a lebih besar atau sama dengan b, maka kembalikan hasil a / b.

- return b / a: Jika a kurang dari b, maka kembalikan hasil b / a.

2. Untuk function mod sendiri

- if (a >= b): Jika a lebih besar atau sama dengan b:
  - int resultDiv = a / b;: Hitung hasil pembagian a / b dan simpan dalam resultDiv.
  - int resultMultiple = b \* resultDiv;: Hitung resultDiv dikali b dan simpan dalam resultMultiple.
  - return a - resultMultiple;: Kembalikan hasil pengurangan a dengan resultMultiple.
- return a;: Jika a kurang dari b, kembalikan a.

3. Untuk fuction memcpy

- char _csrc = (char _)src; dan char _cdest = (char _)dst;: Menetapkan pointer src dan dst ke tipe char \*.

- for (i = 0; i < size; i++): Loop untuk setiap byte dari 0 sampai size - 1.
  - cdest[i] = csrc[i];: Salin byte dari csrc ke cdest.

4, Untuk function strlen

- unsigned int length = 0;: Inisialisasi panjang string dengan 0.
- while (str[length] != '\0'): Loop sampai menemukan karakter null (\0).
  - length++;: Tambahkan panjang string.
- return length;: Kembalikan panjang string.

5. Untuk function strcmp

- while (*str1 != '\0' && *str2 != '\0'): Loop selama kedua string belum mencapai karakter null (\0).
  - if (*str1 != *str2): Jika karakter pada posisi saat ini tidak sama, kembalikan false.
  - str1++; dan str2++;: Lanjutkan ke karakter berikutnya dalam kedua string.
- return *str1 == '\0' && *str2 == '\0';: Kembalikan true jika kedua string telah mencapai karakter null pada saat yang sama, menandakan kedua string sama panjang dan identik.

6. Untuk function strcpy

- while (\*src != '\0'): Loop sampai menemukan karakter null (\0).
  - *dst = *src;: Salin karakter dari src ke dst.
  - dst++; dan src++;: Lanjutkan ke posisi berikutnya dalam kedua string.
- \*dst = '\0';: Tambahkan karakter null di akhir string tujuan (dst).

7. Untuk function clear

- for (i = 0; i < size; i++): Loop untuk setiap byte dari 0 sampai size - 1.
  - buf[i] = 0;: Tetapkan byte saat ini ke 0.

### Revisi

- Tidak ada

## 3E

- e. Buat automasi untuk melakukan kompilasi dengan melengkapi file [`makefile`](./makefile).

  Untuk melakukan kompilasi program, akan digunakan perintah `make build`. Semua hasil kompilasi program akan disimpan pada direktori [`bin/`](./bin). Untuk menjalankan program, akan digunakan perintah `make run`.

### Jawab

```

prepare:
@echo "...prepare"
dd if=/dev/zero of=bin/floppy.img bs=512 count=2880

bootloader:
@echo "...bootloader"
nasm -f bin src/bootloader.asm -o bin/bootloader.bin
dd if=bin/bootloader.bin of=bin/floppy.img bs=512 count=1 conv=notrunc

kernel:
@echo "...kernel"
bcc -ansi -c src/std_lib.c -o bin/std_lib.o -I./include
nasm -f as86 src/kernel.asm -o bin/kernel_asm.o
bcc -ansi -c src/kernel.c -o bin/kernel.o -I./include

link:
@echo "...link"
ld86 -o bin/kernel.bin -d bin/kernel.o bin/kernel_asm.o bin/std_lib.o
dd if=bin/kernel.bin of=bin/floppy.img bs=512 seek=1 conv=notrunc

build: prepare bootloader kernel link

run:
bochs -q

```

### Penjelasan

1. Pada tahap prepare, pertama-tama buat disk imagenya terlebih dahulu

- Sistem operasi akan disimpan pada sebuah disk image. Disk image adalah file yang berisi data yang akan digunakan oleh sistem operasi. Disk image akan digunakan oleh emulator untuk menyimpan sistem operasi yang akan dijalankan. Kali ini, akan digunakan disk image dengan ukuran 1.44 MB (ukuran floppy disk). Disk image dapat dibuat menggunakan perintah berikut.

- `	dd if=/dev/zero of=bin/floppy.img bs=512 count=2880`

2. Pada tahap bootloader compile kode bootloader yang berada di directory src dan outputnya disimpan ke dalam directory bin

- `nasm -f bin src/bootloader.asm -o bin/bootloader.bin`

- f bin (format binary): Mengubah kode assembly menjadi kode biner.

3. Pada tahap kernel ada 3 perintah yang perlu dijalankan

- `bcc -ansi -c src/std_lib.c -o bin/std_lib.o -I./include` perintah ini untuk mengcompile file std_lib yang berada di directory src menjadi file objek yang berada di directory bin. `-I./include` sendiri untuk menambahkan header yang ada dari dalam directory include

- `nasm -f as86 src/kernel.asm -o bin/kernel_asm.o` perintah ini untuk mengcompile file kernel.asm yang berada di directory src dan kemudian outputnya disimpan ke dalam directory bin

- f as86 (format as86): Mengubah kode assembly menjadi kode objek.

- `bcc -ansi -c src/kernel.c -o bin/kernel.o -I./include` perintah ini untuk mengcompile file kernel.c yang berada di directory src menjadi file objek yang berada di directory bin. `-I./include` sendiri untuk menambahkan header yang ada dari dalam directory include

4. Pada tahap link ada 2 perintah yang perlu dijalankan

- Setelah membuat kernel assembly, library, dan kernel C, kita akan menggabungkan ketiga object file tersebut menjadi sebuah executable. Berikut adalah perintah yang digunakan: `ld86 -o bin/kernel.bin -d bin/kernel.o bin/kernel_asm.o bin/std_lib.o`

- Kita akan menggabungkan kernel tersebut menjadi sebuah disk image. Berikut adalah perintah yang digunakan: `dd if=bin/kernel.bin of=bin/floppy.img bs=512 seek=1 conv=notrunc`

5. Perintah echo sendiri saya buat untuk ngedebug apakah tahapan itu tereksekusi atau tidak

6. Pada tahapan build

- `build: prepare bootloader kernel link` ini untuk menjalankan tahap-tahapan yang sudah didefine

7. Pada tahapan run

- `run: bochs -q` jangan lupa untuk memberikan baris baru agar bisa drun, perintah ini untuk menjalankan bootloader tersebut menggunakan emulator Bochs

### Revisi

- Tidak ada

<img src="https://media.discordapp.net/attachments/1210251261450919959/1246835441940828341/Screenshot_from_2024-06-02_21-37-40.png?ex=665dd57e&is=665c83fe&hm=be67fcf2a6196f2e9704f822ffd0a46803ab44e5c5e068b87467d9ddd7910fb5&=&format=webp&quality=lossless&width=1039&height=584" height="auto" width="auto">
