# Fu53enC0d3D==

Digital security is an important aspect in the digital world, one of the most interesting topics is the security of a file. One way to secure the contents of a file is to encode the contents of the file. Encode is a process carried out to secure data and information and make it unreadable easily if there is no special knowledge. One of the most famous encodes is Base64 encode.

Your task is to design a security system for user files using FUSE in [fuse.c](./fuse.c), with the following task description:

- a. Clone the [target directory](https://github.com/hqlco/target.git) and create 4 users on Linux with usernames according to the contents of the folder in the target directory (password is up to you).

  | User | Folder |
  | ---- | ------ |
  | andi | andi   |
  | budi | budi   |
  | cony | cony   |
  | deni | deni   |

  Folder ownership table

- b. When accessed, the mount point folder of fuse that you’ve created goes directly to the target directory that has been cloned (not starting from root)

  ![image](https://github.com/arsitektur-jaringan-komputer/Modul-Sisop/assets/54766683/1383686d-9ba2-4493-9297-3cdfdb7dcdac)

- c. Make it so that each user can't add, change, or delete folders or files in folders they don't own.

- d. Make it so that users can add, change, or delete folders and files in their own folders.

  relevant command for points c and d:

  - mkdir
  - rmdir
  - touch
  - rm
  - cp
  - mv
  - etc.

- e. All the files inside of a folder will be encoded if accessed by a user who is not the owner of that folder. (using Base64 encoding).

- f. On the contrary, all files will be decoded if they are accessed by the user who owns that folder.

Example:

![image](https://github.com/arsitektur-jaringan-komputer/Modul-Sisop/assets/54766683/932c2acd-2f85-4abf-9fc9-9c38737d1605)

Note:

- Mounting folder name is up to you
- In order to make fuse accessible to all users, it is necessary to add the “-o allow_other” argument when mounting it.

# Fu53enC0d3D==

Keamanan digital adalah suatu aspek penting dalam dunia digital, salah satu topik yang sangat menarik adalah keamanan dari suatu file. Cara cara untuk mengamankan isi dari suatu file salah satunya dapat menggunakan encode pada isi file tersebut. Encode adalah sebuah proses yang dilakukan untuk mengamankan data dan informasi dan membuatnya menjadi tidak bisa dibaca dengan mudah jika tidak ada pengetahuan khusus. Adapun salah satu encode paling terkenal adalah encode Base64.

Tugas kalian adalah merancang suatu sistem keamanan pada file file user menggunakan FUSE pada file [fuse.c](./fuse.c), dengan penjabaran tugas sebagai berikut:

- a. Clone [direktori target](https://github.com/hqlco/target.git) dan buatlah 4 user pada linux dengan username sesuai isi folder pada direktori target (password dibebaskan).

  | User | Folder |
  | ---- | ------ |
  | andi | andi   |
  | budi | budi   |
  | cony | cony   |
  | deni | deni   |

  Tabel kepemilikan folder

- b. Ketika folder mount point dari fuse yang kalian buat diakses, akan langsung menuju ke dalam target folder yang telah di clone (tidak dimulai dari root)

  ![image](https://github.com/arsitektur-jaringan-komputer/Modul-Sisop/assets/54766683/1383686d-9ba2-4493-9297-3cdfdb7dcdac)

- c. Buatlah agar tiap user tidak dapat menambah, mengubah, maupun menghapus folder maupun file dalam folder yang bukan miliknya.

- d. Buatlah agar user dapat menambah, mengubah, maupun menghapus folder maupun file dalam folder miliknya.

  command yang relevan untuk poin c and d:
  
  - mkdir
  - rmdir
  - touch
  - rm
  - cp
  - mv
  - etc.

- e. Semua isi file akan ter-encode jika diakses oleh selain user pemilik folder tersebut (menggunakan encoding Base64).

- f. Sebaliknya, file akan ter-decode ketika diakses oleh user pemilik folder tersebut.

Contoh:

![image](https://github.com/arsitektur-jaringan-komputer/Modul-Sisop/assets/54766683/932c2acd-2f85-4abf-9fc9-9c38737d1605)

Note:

- Nama folder mounting dibebaskan
- Agar fuse dapat diakses oleh semua user, perlu ditambahkan argumen “-o allow_other” ketika melakukan mount
