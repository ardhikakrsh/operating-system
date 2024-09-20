# MengOS

Nori is a student who is learning to make an operating system. She wants to create a simple operating system that can display and read strings from users. Since Nori is in a lazy phase, she asks for your help to create the program. Help Nori to complete her task!

### Directory Structure

```
task-3/
├── bin/
├── include/
│   ├── kernel.h
│   ├── std_lib.h
│   └── std_type.h
├── src/
│   ├── bootloader.asm
│   ├── kernel.asm
│   ├── kernel.c
│   └── std_lib.c
├── bochsrc.txt
└── makefile
```

- [`bin/`](./bin/) : contains compiled files
- [`include/`](./include/) : contains header files
- [`src/`](./src/) : contains source code
- [`bochsrc.txt`](./bochsrc.txt) : Bochs configuration
- [`makefile`](./makefile) : make configuration file

### Kernel

#### [`kernel.asm`](./src/kernel.asm)

```asm
global _putInMemory
global _interrupt

; void putInMemory(int segment, int address, char character)
_putInMemory:
	push bp
	mov bp,sp
	push ds
	mov ax,[bp+4]
	mov si,[bp+6]
	mov cl,[bp+8]
	mov ds,ax
	mov [si],cl
	pop ds
	pop bp
	ret

; int interrupt(int number, int AX, int BX, int CX, int DX)
_interrupt:
	push bp
	mov bp,sp
	mov ax,[bp+4]
	push ds
	mov bx,cs
	mov ds,bx
	mov si,intr
	mov [si+1],al
	pop ds
	mov ax,[bp+6]
	mov bx,[bp+8]
	mov cx,[bp+10]
	mov dx,[bp+12]

intr:	int 0x00

	mov ah,0
	pop bp
	ret
```

- `_putInMemory` : function to write character to memory (explanation in modul)

- `_interrupt` : function to do interrupt

  - `number` : interrupt vector number
  - `AX`, `BX`, `CX`, `DX` : registers to be filled
  - Register `AX` is a combination of `AH` and `AL`
  - Register `BX` is a combination of `BH` and `BL`
  - Register `CX` is a combination of `CH` and `CL`
  - Register `DX` is a combination of `DH` and `DL`
  - To combine two `H` and `L` type registers into `X`, you can use one of the following methods.
    ```c
    AX = AH << 8 | AL // first method
    AX = AH * 256 + AL // second method
    ```

#### [`kernel.c`](./src/kernel.c)

```c
#include "kernel.h"
#include "std_lib.h"

int main() {
  char buf[128];
  clearScreen();
  printString("Welcome to MengOS\n");

  while (true) {
    printString("$ ");
    readString(buf);
    printString(buf);
    printString("\n");
  }
}
```

- The kernel will wait for input from the user and display the input back to the screen as long as the operating system is running.

- Do not change the contents of the `main` function.

### Header Files

#### [`std_type.h`](./include/std_type.h)

This header contains the definition of data types that will be frequently used in the program.

```c
typedef unsigned char byte;

typedef char bool;
#define true 1
#define false 0
```

- `byte` : unsigned char data type, used to store numbers in the range 0-255 (`0x00`-`0xFF`)

- `bool` : boolean data type, used to store `true` or `false` values

  This data type is created because there is no boolean data type in the C language.

#### [`std_lib.h`](./include/std_lib.h)

This header contains the declaration of functions that will be frequently used in the program.

```c
int div(int a, int b);
int mod(int a, int b);

void memcpy(byte* src, byte* dst, unsigned int size);
unsigned int strlen(char* str);
bool strcmp(char* str1, char* str2);
void strcpy(char* src, char* dst);
void clear(byte* buf, unsigned int size);
```

- `div` : division function

- `mod` : modulo function

  The `div` and `mod` functions need to be created because there is no division and modulo operator in assembly language. Create these functions using available operators, such as `+`, `-`, `*`, `<<`, and `>>`.

- `memcpy` : function to copy memory

- `strlen` : function to count string length

- `strcmp` : function to compare two strings

- `strcpy` : function to copy string

- `clear` : function to fill memory with zeros

#### [`kernel.h`](./include/kernel.h)

This header contains the declaration of functions that have implementations in `kernel.asm` and will be implemented in `kernel.c`.

```c
extern void putInMemory(int segment, int address, char character);
extern int interrupt(int number, int AX, int BX, int CX, int DX);

void printString(char* str);
void readString(char* buf);
void clearScreen();
```

- `putInMemory` : the implementation of `_putInMemory` function is in `kernel.asm`
- `interrupt` : the implementation of `_interrupt` function is in `kernel.asm`
- `printString` : function to display string
- `readString` : function to read string
- `clearScreen` : function to clear the screen

### Makefile

```makefile
prepare:

bootloader:

stdlib:

kernel:

link:

build: prepare bootloader stdlib kernel link

run:
```

- `prepare` : create a new disk image `floppy.img` in the `bin/` directory with a size of 1.44 MB.

- `bootloader` : compile `bootloader.asm` into `bootloader.bin` in the `bin/` directory.

- `stdlib` : compile `std_lib.c` into `std_lib.o` in the `bin/` directory.

- `kernel` : compile `kernel.c` into `kernel.o` in the `bin/` directory and compile `kernel.asm` into `kernel_asm.o` in the `bin/` directory.

- `link` : merge `bootloader.bin`, `kernel.o`, `kernel_asm.o`, and `std_lib.o` into `floppy.img`.

- `build` : run `prepare`, `bootloader`, `stdlib`, `kernel`, and `link` commands.

- `run` : run Bochs with the `bochsrc.txt` configuration.

### Command Reference

#### `dd`

```bash
dd if=<input_file> of=<output_file> bs=<block_size> count=<block_count> seek=<seek_count> conv=<conversion>
```

#### `nasm`

```bash
nasm -f <format> <input_file> -o <output_file>
```

#### `bcc`

```bash
bcc -ansi -c <input_file> -o <output_file>
```

#### `ld86`

```bash
ld86 -o <output_file> -d [file1.o file2.o ...]
```

### Tasks

- a. Implement the `printString` function in `kernel.c` that will display a string on the screen.

  The implementation can use the `interrupt` function with service `int 10h` and parameter `AH = 0x0E` for teletype output. The character to be displayed can be placed in the `AL` register. This function will display a null-terminated ASCII string on the screen.

- b. Implement the `readString` function in `kernel.c` that will read a string from the keyboard.

  The implementation can use the `interrupt` function with service `int 16h` and parameter `AH = 0x00` for keyboard input. This function will read non-control ASCII characters entered by the user and store them in a buffer until the `Enter` key is pressed. The `Backspace` key handling is left to the students.

- c. Implement the `clearScreen` function in `kernel.c` that will clear the screen.

  The screen size is `80x25` characters. After the screen is cleared, the cursor will return to the initial position `(0, 0)` and the video buffer for character color will be changed to white. The implementation can use the `interrupt` function with service `int 10h` or use the `putInMemory` function to fill the video memory.

- d. Complete the implementation of the functions in [`std_lib.h`](./include/std_lib.h) in [`std_lib.c`](./src/std_lib.c).

  The above functions can be used to simplify the implementation of the `printString`, `readString`, and `clearScreen` functions.

- e. Create automation to compile by completing the [`makefile`](./makefile) file.

  To compile the program, the `make build` command will be used. All compiled program results will be stored in the [`bin/`](./bin) directory. To run the program, the `make run` command will be used.

# MengOS

Nori adalah seorang mahasiswa yang sedang belajar membuat sistem operasi. Ia ingin membuat sebuah sistem operasi sederhana yang dapat menampilkan dan membaca string dari pengguna. Karena Nori sedang pada fase malas, dia meminta bantuan Anda untuk membuat program tersebut. Bantulah Nori untuk menyelesaikan tugasnya!

### Struktur Direktori

```
task-3/
├── bin/
├── include/
│   ├── kernel.h
│   ├── std_lib.h
│   └── std_type.h
├── src/
│   ├── bootloader.asm
│   ├── kernel.asm
│   ├── kernel.c
│   └── std_lib.c
├── bochsrc.txt
└── makefile
```

- [`bin/`](./bin/) : menyimpan file hasil kompilasi
- [`include/`](./include/) : berisi kode file header
- [`src/`](./src/) : berisi kode sumber
- [`bochsrc.txt`](./bochsrc.txt) : konfigurasi Bochs
- [`makefile`](./makefile) : file konfigurasi make

### Kernel

#### [`kernel.asm`](./src/kernel.asm)

```asm
global _putInMemory
global _interrupt

; void putInMemory(int segment, int address, char character)
_putInMemory:
	push bp
	mov bp,sp
	push ds
	mov ax,[bp+4]
	mov si,[bp+6]
	mov cl,[bp+8]
	mov ds,ax
	mov [si],cl
	pop ds
	pop bp
	ret

; int interrupt(int number, int AX, int BX, int CX, int DX)
_interrupt:
	push bp
	mov bp,sp
	mov ax,[bp+4]
	push ds
	mov bx,cs
	mov ds,bx
	mov si,intr
	mov [si+1],al
	pop ds
	mov ax,[bp+6]
	mov bx,[bp+8]
	mov cx,[bp+10]
	mov dx,[bp+12]

intr:	int 0x00

	mov ah,0
	pop bp
	ret
```

- `_putInMemory` : fungsi untuk menulis karakter ke memori (penjelasan ada di modul)

- `_interrupt` : fungsi untuk melakukan interrupt

  - `number` : nomor vektor interrupt
  - `AX`, `BX`, `CX`, `DX` : register yang akan diisi
  - Register `AX` merupakan gabungan dari `AH` dan `AL`
  - Register `BX` merupakan gabungan dari `BH` dan `BL`
  - Register `CX` merupakan gabungan dari `CH` dan `CL`
  - Register `DX` merupakan gabungan dari `DH` dan `DL`
  - Untuk menggabungkan dua regester tipe `H` dan `L` menjadi `X` dapat menggunakan salah satu dari cara berikut.
    ```c
    AX = AH << 8 | AL // cara pertama
    AX = AH * 256 + AL // cara kedua
    ```

#### [`kernel.c`](./src/kernel.c)

```c
#include "kernel.h"
#include "std_lib.h"

int main() {
  char buf[128];
  clearScreen();
  printString("Welcome to MengOS\n");

  while (true) {
    printString("$ ");
    readString(buf);
    printString(buf);
    printString("\n");
  }
}
```

- Kernel akan menunggu input dari pengguna dan menampilkan input tersebut kembali ke layar selama sistem operasi berjalan.

- Dilarang mengubah isi dari fungsi `main`.

### File Header

#### [`std_type.h`](./include/std_type.h)

Header ini berisi definisi tipe data yang akan sering digunakan dalam program.

```c
typedef unsigned char byte;

typedef char bool;
#define true 1
#define false 0
```

- `byte` : tipe data unsigned char, digunakan untuk menyimpan angka dengan rentang 0-255 (`0x00`-`0xFF`)

- `bool` : tipe data boolean, digunakan untuk menyimpan nilai `true` atau `false`

  Tipe data ini dibuat karena tidak ada tipe data boolean di bahasa C.

#### [`std_lib.h`](./include/std_lib.h)

Header ini berisi deklarasi fungsi yang akan sering digunakan dalam program.

```c
int div(int a, int b);
int mod(int a, int b);

void memcpy(byte* src, byte* dst, unsigned int size);
unsigned int strlen(char* str);
bool strcmp(char* str1, char* str2);
void strcpy(char* src, char* dst);
void clear(byte* buf, unsigned int size);
```

- `div` : fungsi pembagian

- `mod` : fungsi modulo

  Fungsi `div` dan `mod` perlu dibuat karena tidak ada operator pembagian dan modulo pada bahasa assembly. Buatlah fungsi tersebut dengan menggunakan operator yang tersedia, seperti `+`, `-`, `*`, `<<`, dan `>>`.

- `memcpy` : fungsi untuk menyalin memori

- `strlen` : fungsi untuk menghitung panjang string

- `strcmp` : fungsi untuk membandingkan dua string

- `strcpy` : fungsi untuk menyalin string

- `clear` : fungsi untuk mengisi memori dengan nol

#### [`kernel.h`](./include/kernel.h)

Header ini berisi deklarasi fungsi yang memiliki implementasi pada `kernel.asm` dan akan diimplementasikan pada `kernel.c`.

```c
extern void putInMemory(int segment, int address, char character);
extern int interrupt(int number, int AX, int BX, int CX, int DX);

void printString(char* str);
void readString(char* buf);
void clearScreen();
```

- `putInMemory` : implementasi fungsi `_putInMemory` pada `kernel.asm`
- `interrupt` : implementasi fungsi `_interrupt` pada `kernel.asm`
- `printString` : fungsi untuk menampilkan string
- `readString` : fungsi untuk membaca string
- `clearScreen` : fungsi untuk membersihkan layar

### Makefile

```makefile
prepare:

bootloader:

stdlib:

kernel:

link:

build: prepare bootloader stdlib kernel link

run:
```

- `prepare` : membuat disk image baru `floppy.img` pada direktori `bin/` dengan ukuran 1.44 MB.

- `bootloader` : mengkompilasi `bootloader.asm` menjadi `bootloader.bin` pada direktori `bin/`.

- `stdlib` : mengkompilasi `std_lib.c` menjadi `std_lib.o` pada direktori `bin/`.

- `kernel` : mengkompilasi `kernel.c` menjadi `kernel.o` pada direktori `bin/` dan mengkompilasi `kernel.asm` menjadi `kernel_asm.o` pada direktori `bin/`.

- `link` : menggabungkan `bootloader.bin`, `kernel.o`, `kernel_asm.o`, dan `std_lib.o` menjadi `floppy.img`.

- `build` : menjalankan perintah `prepare`, `bootloader`, `stdlib`, `kernel`, dan `link`.

- `run` : menjalankan Bochs dengan konfigurasi `bochsrc.txt`.

### Referensi Perintah

#### `dd`

```bash
dd if=<input_file> of=<output_file> bs=<block_size> count=<block_count> seek=<seek_count> conv=<conversion>
```

#### `nasm`

```bash
nasm -f <format> <input_file> -o <output_file>
```

#### `bcc`

```bash
bcc -ansi -c <input_file> -o <output_file>
```

#### `ld86`

```bash
ld86 -o <output_file> -d [file1.o file2.o ...]
```

### Deskripsi Tugas

- a. Implementasikan fungsi `printString` pada `kernel.c` yang akan menampilkan string ke layar.

  Implementasi dapat menggunakan fungsi `interrupt` dengan service `int 10h` dengan parameter `AH = 0x0E` untuk _teletype output_. Karakter yang ditampilkan dapat dimasukkan pada register `AL`. Fungsi ini akan menampilkan string karakter ASCII (_null-terminated_) ke layar.

- b. Implementasikan fungsi `readString` pada `kernel.c` yang akan membaca string dari keyboard.

  Implementasi dapat menggunakan fungsi `interrupt` dengan service `int 16h` dengan parameter `AH = 0x00` untuk _keyboard input_. Fungsi ini akan membaca karakter ASCII (_non-control_) yang dimasukkan oleh pengguna dan menyimpannya pada buffer hingga menekan tombol `Enter`. Handle tombol `Backspace` dibebaskan kepada praktikan.

- c. Implementasikan fungsi `clearScreen` pada `kernel.c` yang akan membersihkan layar.

  Ukuran layar adalah `80x25` karakter. Setelah layar dibersihkan, kursor akan kembali ke posisi awal yaitu `(0, 0)` dan buffer video untuk warna karakter diubah ke warna putih. Implementasi dapat menggunakan fungsi `interrupt` dengan service `int 10h` atau menggunakan fungsi `putInMemory` untuk mengisi memori video.

- d. Lengkapi implementasi fungsi yang terdapat pada [`std_lib.h`](./include/std_lib.h) pada [`std_lib.c`](./src/std_lib.c).

  Fungsi-fungsi di atas dapat digunakan untuk mempermudah implementasi fungsi `printString`, `readString`, dan `clearScreen`.

- e. Buat automasi untuk melakukan kompilasi dengan melengkapi file [`makefile`](./makefile).

  Untuk melakukan kompilasi program, akan digunakan perintah `make build`. Semua hasil kompilasi program akan disimpan pada direktori [`bin/`](./bin). Untuk menjalankan program, akan digunakan perintah `make run`.
