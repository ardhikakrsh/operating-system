[![Review Assignment Due Date](https://classroom.github.com/assets/deadline-readme-button-24ddc0f5d75046c5622901739e7c5dd533143b0c8e959d652212380cedb1ea36.svg)](https://classroom.github.com/a/nyzv0V02)

<div align=center>

|    NRP     |          Name           |
| :--------: | :---------------------: |
| 5025221100 | Riyanda Cavin Sinambela |
| 5025221006 | Ardhika Krishna Wijaya  |
| 5025221294 |   Faiq Lidan Baihaqi    |

# Praktikum Modul 1 _(Module 1 Lab Work)_

</div>

### Daftar Soal _(Task List)_

- [Task 1 - Tim Kawal Bubu _(Bubu's Monitoring Team)_](/task-1/)

- [Task 2 - Gabut Yuan _(Yuan's Boredom)_](/task-2/)

- [Task 3 - Magang Bubu _(Bubu's Internship)_](/task-3/)

- [Task 4 - LDR Isabel _(Isabel Sad Relationship)_](/task-4/)

### Laporan Resmi Praktikum Modul 1 _(Module 1 Lab Work Report)_

# ✨ Nomor 1

### 1A

- Karena Belmawa menetapkan judul maksimum proposal 20 kata, maka komandan ingin mencari data siapa saja tim yang tidak memenuhi ketentuan ini. Tampilkan nama pengusul, beserta departemennya yang judulnya lebih dari 20 kata. Pisahkan spasi dan hapus underscore "\_" pada nama pengusul.

#### Jawab

Langkah-langkah pengerjaan:

1. Karena terdapat perubahan file dari .csv ke .tsv maka download filenya terlebih dahulu dan pindahkan ke dalam folder resources (command di bawah hanya contoh karena path/to/praktikum-modul-1-f01 punya saya berbeda)

```
mv Downloads/DataPKM.tsv praktikum-modul-1-f01/resources/
```

2. Open vscode, drag and drop folder praktikum-modul-1-f01 ke vscode
3. Cari folder task-1 lalu edit file pkm-a.sh
4. Dalam pengerjaan digunakan beberapa command yaitu awk, gsub, split, dan printf. Berikut adalah shell script yang dibuat :

```
# ! /bin/bash

tsv_file="../resources/DataPKM.tsv"

awk -F '\t' '{
    gsub(/_/, " ", $2);
    maxCharacterTitle=20
    num_words = split($5, words, " ");
    if (num_words > maxCharacterTitle)
        printf NR". %s\nDepartemen: %s\n\n", $2, $3
}' "$tsv_file"
```

5. Penjelasan command

- awk untuk mengambil catatan/record tertentu dalam sebuah file dan melakukan sebuah/beberapa operasi terhadap catatan/record tersebut.
  1. -F untuk mendefinisikan separator
  2. \t, merupakan delimiter yang digunakan dalam tsv
  3. $tsv_file sebagai input-file
  4. {action}
- gsub akan mengganti string atau karakter dalam vektor atau bingkai data dengan string tertentu
  1. gsub(pola, pengganti, vektor atau bingkai data untuk menggantikan string)
- split akan mengambil string, array, dan pola pembatas sebagai argumen. Kemudian, string tersebut dipisahkan menjadi array berdasarkan pola pembatas

#### Logic Penyelesaian:

- [ ] Gunakan awk untuk print nama pengusul dan departemen
- [ ] Gunakan gsub untuk menghapus underscore pada nama pengusul `gsub(/_/, " ", $2);`
- [ ] Buat sebuah variabel untuk menampung angka 20 (judul maksimum proposal) anti hard code
- [ ] Hitung tiap judul ada berapa kata dengan fungsi split(string, array, pembatas) `num_words = split($5, words, " ");`
- [ ] Gunakan if untuk ngehandle judul yang lebih dari 20 kata, contohnya `if (num_words > maxCharacterTitle)`

#### Kendala:

1. Salah meletakkan variabel maxCharacterTitle, harusnya di awk bagian dalam action. Berikut kode dan outputnya yang salah (sebelum direvisi)

```
# ! /bin/bash

tsv_file="../resources/DataPKM.tsv"
maxCharacterTitle=20

awk -F '\t' '{
   	gsub(/_/, " ", $2);
   	num_words = split($5, words, " ");
   	if (num_words > $maxCharacterTitle)
   		printf NR". %s\nDepartemen: %s\n\n", $2, $3
}' "$tsv_file"

```

<img width="auto" height="auto" src="https://media.discordapp.net/attachments/1223328224281497610/1223328482398961754/image.png?ex=6619746e&is=6606ff6e&hm=1659c9a0a135ba5560bd2a09cac3a51b623f1d81ec5b488edb294de482c120c1&=&format=webp&quality=lossless&width=1080&height=608">

#### Output (sesudah revisi):

```
# ! /bin/bash

tsv_file="../resources/DataPKM.tsv"

awk -F '\t' '{
    gsub(/_/, " ", $2);
    maxCharacterTitle=20
    num_words = split($5, words, " ");
    if (num_words > maxCharacterTitle)
    	printf NR". %s\nDepartemen: %s\n\n", $2, $3
}' "$tsv_file"

```

<img width="auto" height="auto" src="https://media.discordapp.net/attachments/1223328224281497610/1223334153970516079/image.png?ex=661979b6&is=660704b6&hm=af1f8c1378e983aa35f7baabeceb8d2a830c5873369f715104f0d6e028ebc736&=&format=webp&quality=lossless&width=1080&height=608">

### 1B

- Komandan PKM juga tertarik ingin tahu antusiasme dan partisipasi mahasiswa sehingga meminta Bubu menampilkan bidang paling banyak diminati oleh mahasiswa. Tampilkan nama skema saja.

#### Jawab

1. Edit shell script pkm-b.sh yang telah disediakan
2. Dalam pengerjaan digunakan beberapa command yaitu cut, sort, uniq, dan head. Sebelumnya sudah bertanya sama probset soal 1 kalau tidak menggunakan awk apakah bisa dan diperbolehkan asal menggunakan shell script

Berikut adalah shell script yang dibuat :

```
# ! /bin/bash

tsv_file="../resources/DataPKM.tsv"

cut -f7 "$tsv_file" \
| sort \
| uniq -c \
| sort -nr \
| head -1
```

3. Penjelasan command

- cut untuk mengambil kolom tertentu dari file
  1.  -f untuk memilih kolom mana dan -f7 berarti milih kolom 7
  2.  tsv_file sebagai input file
- sort untuk mengurutkan isi file teks, baris demi baris.
- uniq untuk memfilter baris yang duplikat dari data yang diurutkan dan -c untuk menghitung kemunculan setiap nilai unik dan menampilkan hitungan beserta nilai itu sendiri.
- sort `-n` untuk mengurutkan file dengan data numerik dan `-r` dapat digabungkan dengannya (misalnya, sort -nr number.txt) untuk menghasilkan pengurutan numerik terbalik (descending).
- head digunakan untuk membatasi banyak data yang diambil.
  1.  `-1` hanya ngambil satu baris teratas.

#### Logic Penyelesaian

- [ ] Filter/pilih kolom yang mau diambil menggunakan cut
- [ ] Lalu urutkan semua baris (ascending atau a to z)
- [ ] Setelah itu filter baris-baris yang nilainya lebih dari 1 dan langsung dihitung yang nilainya lebih dari 1 ada berapa
- [ ] Setelah itu urutkan secara descending (z to a)
- [ ] Ambil 1 nilai yang sudah diurutkan tadi

#### Kendala

1. Karena saya ingin mengeksplor perintah-perintah yang lain selain yang diajarkan dari modul, jadi membutuhkan waktu sekitar 6 jam.
2. Melakukan modifikasi beberapa command untuk mengetahui outputnya seperti apa
3. Tidak ada revisi

#### Output

<img src="https://cdn.discordapp.com/attachments/1223328224281497610/1223501472516542484/image.png?ex=661a158a&is=6607a08a&hm=2cd96b34be4d3f947eb7209a2a66cf2c8c182330af55403bf67ea676b11dcb15&">

### 1C

- Karena ada aturan baru dimana 1 mahasiswa hanya boleh submit 1 proposal, maka komandan juga meminta Bubu untuk memberikan list mahasiswa yang mengajukan 2 proposal. Tampilkan data pembimbingnya karena ingin di kontak komandan.

#### Jawab

1. Edit shell script pkm-c.sh yang telah disediakan
2. Dalam pengerjaan shell script hanya menggunakan command awk

Berikut adalah shell script yang dibuat :

```
tsv_file="../resources/DataPKM.tsv"

awk -F '\t' 'NR > 1 {
		proposals[$2]++;
		advisors[$2][$6] = 1;
}
END {
	for (student in proposals) {
		if (proposals[student] > 1) {
			print "Pembimbing:";
			for (advisor in advisors[student]) {
				print "- ", advisor;
			}
		}
	}
}' "$tsv_file"

```

3. Penjelasan command

- awk untuk mengambil catatan/record tertentu dalam sebuah file dan melakukan sebuah/beberapa operasi terhadap catatan/record tersebut.
  1.  `-F` untuk mendefinisikan separator/pemisah
  2.  `\t` separator yang digunakan

#### Logic Penyelesaian:

- [ ] Gunakan awk untuk mengambil data mahasiswa dan dosen
- [ ] Membuat array asosiatif untuk menghitung jumlah proposal yang diajukan oleh setiap mahasiswa
- [ ] Membuat array asosiatif untuk menyimpan data pembimbing
- [ ] Setelah itu looping menggunakan foreach melalui array proposals untuk mencari mahasiswa yang mengajukan lebih dari satu proposal
- [ ] Buat percabangan untuk menghandle case di mana proposal mahasiswa lebih dari 1
- [ ] Jika kondisi sebelumnya `true` tampilkan pembimbingnya
- [ ] Untuk ngehandle case agar semua dosen yang mahasiswa tersebut mengajukan proposal lebih dari 1 kali, gunakan loop foreach melalui array advisors untuk mencetak semua pembimbing dari mahasiswa tersebut

#### Kendala:

1. Sama seperti sebelumnya yaitu, perlu explore tentang awk di internet/repo/youtube

2. Tidak ada revisi

#### Output:

<img src="https://cdn.discordapp.com/attachments/1223328224281497610/1223525860506599434/image.png?ex=661a2c41&is=6607b741&hm=9458d636dc37e06d1c9cae11f04483a8ada568b793ecfe76c768e7d81f9fcfdb&" width="auto" height="auto">

# No. 2 Gabut Yuan (Yuan's Boredom) <br>

> ## Pengerjaan Soal :

- Dimulai dari task 2C kemudian dilanjutkan 2B atau 2A, disebabkan untuk soal 2A saya menggunakan data users.txt yang dimana harus mengerjakan 2C dahulu agar nantinya data users.txt terbuat(sudah mengonfirmasi ke pembuat soal bahwa tidak apa menggunakan users.txt sebagai datanya). <br>

> ## 2A <br>

- Pada 2A ini kita disuruh membuat login tanpa melakukan register dengan memasukkan `username`(nama pertama nama_pengusul) kemudian memasukkan `password` yaitu fakultas+nipd. Setiap percobaan akan tercatat pada log.txt yaitu `YY/MM/DD hh:mm:ss` dan pesan yaitu `LOGIN: SUCCESS USER_NAME is logged in` dan `LOGIN: ERROR Failed login attempt on USER_NAME`. <br>

- #### Logic Penyelesaian

- [ ] Bikin Fungsi log untuk mencatat ke log.txt
- [ ] Masukkan username dan password kemudian cek pada users.txt menggunakan grep -q

- Berdasarkan permasalahan diatas, berikut adalah kode `yu_login.sh` yang saya kerjakan : <br>

```ruby
# mencatat log dengan tanggal
log() {
    echo "$(date +'%Y/%m/%d %H:%M:%S') $1" >> log.txt
}

# memasukkan input username dan pass
read -p "Masukkan username: " username
read -p "Masukkan password: " password

#ngecek username:password sesuai apa nggak di users.txt
if grep -q "$username:$password" users.txt; then
  	echo "LOGIN: SUCCESS $username is logged in"
        log "LOGIN: SUCCESS $username is logged in"
else
     	echo "LOGIN: ERROR Failed login attempt on $username"
     	log "LOGIN: ERROR Failed login attempt on $username"
fi
```

- Berikut adalah penjelasan lebih detail mengenai kode `yu_login.sh` diatas: <br>

```ruby
log() {
    echo "$(date +'%Y/%m/%d %H:%M:%S') $1" >> log.txt
}
```

Mencatat log pada `log.txt` dengan menambahkan pesan dengan format waktu yang disertakan oleh `date +'%Y/%m/%d %H:%M:%S'`, diikuti oleh pesan yang diberikan sebagai argumen fungsi ($1). Pastikan sudah memiliki file log.txt di directory. <br>

```ruby
read -p "Masukkan username: " username
read -p "Masukkan password: " password
```

Meminta pengguna untuk memasukkan username dan password. Inputan dari pengguna akan disimpan pada variabel username dan password. <br>

```ruby
if grep -q "$username:$password" users.txt; then
  	echo "LOGIN: SUCCESS $username is logged in"
        log "LOGIN: SUCCESS $username is logged in"
else
     	echo "LOGIN: ERROR Failed login attempt on $username"
     	log "LOGIN: ERROR Failed login attempt on $username"
fi
```

Menggunakan `if grep -q` untuk mencari `username:password` yang dimasukkan pengguna di dalam file users.txt. Jika ditemukan maka akan mencetak `LOGIN: SUCCESS $username is logged in` dan log akan dicatat melalui fungsi `log()`. Apabila tidak ditemukan maka akan mencetak `LOGIN: ERROR Failed login attempt on $username` dan log akan dicatat melalui fungsi `log()`. <br>

- Berikut adalah input dan output dari code yu_login.sh <br>
  <img width="593" alt="input2a" src="https://github.com/sisop-its-s24/praktikum-modul-1-f01/assets/116788471/098250d5-2c9d-4982-9364-0b1c8c2f838b">
  <img width="593" alt="output2a" src="https://github.com/sisop-its-s24/praktikum-modul-1-f01/assets/116788471/6563724e-4d04-457e-a06d-e9dfc2b20000"> <br>

- Kendala yang saya alami saat mengerjakan soal 2A : <br>
  Kesusahan saat mengecek username dan password menggunakan data.pkm.csv, namun sudah diatasi dengan menggunakan data users.txt sebelum praktikum berakhir. <br>

> ## 2B <br>

- Pada 2B ini kita disuruh membuat register yang akan nantinya akan disimpan di data-pkm.csv dengan memasukkan nama_pengusul, asal departemen, fakultas, judul proposal, dosen pendamping (nidn), skema pkm yang sesuaikan dengan data-pkm.csv. Setiap percobaan tercatat pada log.txt yaitu `YY/MM/DD hh:mm:ss` dan pesan yaitu `REGISTER: SUCCESS USER_NAME is registered. Proposal PROPOSAL_NUMBER is added` dan `REGISTER: ERROR USER_NAME is already existed`. <br>

- #### Logic Penyelesaian

- [ ] Bikin fungsi log untuk mencatat ke log.txt
- [ ] Ubah nama pengusul menjadi \_ dan ambil nipd nya saja. Buat password Inputan menggunakan nama depan + nipd inputan.
- [ ] Cek apabila username tidak ada di data-pkm.csv, maka masukkan inputan semua data ke dalam data-pkm.csv
- [ ] Jika tidak ada buat password untuk memvalidasi password inputan dengan mencari fakultas dan mengambil nipd pada data-pkm.csv dengan nama_pengusul inputan.
- [ ] Cek password input dengan password buatan sesuai data-pkm.csv

- Berdasarkan permasalahan diatas, berikut adalah kode `yu_register.sh` yang saya kerjakan : <br>

```ruby
log() {
    echo "$(date +'%Y/%m/%d %H:%M:%S') $1" >> log.txt
}

path="/home/ardhikakrsh/Documents/Kuliah/Sisop/Praktikum-1/praktikum-modul-1/resources"

# Masukkan semua data
read -p "Masukkan Nama_Pengusul: " nama_pengusul
read -p "Masukkan Departemen: " departemen
read -p "Masukkan Fakultas: " fakultas
read -p "Masukkan Judul: " judul
read -p "Masukkan Nama Pendamping (NIPD): " nama_pendamping
read -p "Masukkan Skema: " skema

nama_pengusul=$(echo "$nama_pengusul" | sed 's/ /_/g')
nipd=$(echo "$nama_pendamping" | grep -o '[0-9]*')
passInput="$fakultas$nipd"

# Periksa apakah nama_pengusul sudah ada di data-pkm.csv
if ! grep -q "$nama_pengusul," "$path/data-pkm.csv"; then
	last_proposal_number=$(tail -n 1 "$path/data-pkm.csv" | cut -d ',' -f 1)
	proposal_number=$((last_proposal_number + 1))
	echo "$proposal_number,$nama_pengusul,$departemen,$fakultas,$judul,$nama_pendamping,$skema" >> "$path/data-pkm.csv"
	echo "REGISTER: SUCCESS $nama_pengusul is registered. Proposal $proposal_number is added"
	log "REGISTER: SUCCESS $nama_pengusul is registered. Proposal $proposal_number is added"

else
	fakultasCSV=$(awk -F ',' -v nama="$nama_pengusul" '$2 ~ nama {print $4; exit}' "$path/data-pkm.csv")
	nipdCSV=$(awk -F ',' -v nama="$nama_pengusul" '$2 ~ nama {gsub(/[^0-9]/, "", $6); print $6; exit}' "$path/data-pkm.csv")
	password="$fakultasCSV$nipdCSV"

	if [[ "$password" == "$passInput" ]]; then
		username=$(echo "$nama_pengusul" | cut -d ' ' -f 1)
		echo "REGISTER: ERROR $nama_pengusul is already existed"
        	log "REGISTER: ERROR $nama_pengusul is already existed"
        else
        	echo "REGISTER: ERROR $nama_pengusul's password is wrong"
        	log "REGISTER: ERROR $nama_pengusul's password is wrong"
        fi
fi
```

- Berikut adalah penjelasan lebih detail mengenai kode `yu_register.sh` diatas: <br>

```ruby
log() {
    echo "$(date +'%Y/%m/%d %H:%M:%S') $1" >> log.txt
}

path="/home/ardhikakrsh/Documents/Kuliah/Sisop/Praktikum-1/praktikum-modul-1/resources"
```

Mencatat log pada `log.txt` dengan menambahkan pesan dengan format waktu yang disertakan oleh `date +'%Y/%m/%d %H:%M:%S'`, diikuti oleh pesan yang diberikan sebagai argumen fungsi ($1). Pastikan sudah memiliki file log.txt di directory. Path sebagai variabel untuk menyimpan path dari data-pkm.csv <br>

```ruby
read -p "Masukkan Nama_Pengusul: " nama_pengusul
read -p "Masukkan Departemen: " departemen
read -p "Masukkan Fakultas: " fakultas
read -p "Masukkan Judul: " judul
read -p "Masukkan Nama Pendamping (NIPD): " nama_pendamping
read -p "Masukkan Skema: " skema
```

Menggunakan read -p untuk membaca input dan memasukkan nama_pengusul yang akan disimpan di variabel nama_pengusul. Memasukkan departemen yang akan disimpan di variabel departemen. Memasukkan lain-lainnya sesuai ketentuan soal dan masing-masing inputan disimpan pada variabelnya masing-masing. <br>

```ruby
nama_pengusul=$(echo "$nama_pengusul" | sed 's/ /_/g')
nipd=$(echo "$nama_pendamping" | grep -o '[0-9]*')
passInput="$fakultas$nipd"
```

Mengubah spasi " " pada nama*pengusul menjadi underscore "\_" menggunakan `sed 's/ /*/g'`. Mengambil nipd yaitu angka dari $nama_pendamping menggunakan ` grep -o '[0-9]\*'`. Menggabungkan fakultas+nipd dan disimpan pada variabel passInput guna untuk validasi passInput dengan password pada data sebenarnya. <br>

```ruby
if ! grep -q "$nama_pengusul," "$path/data-pkm.csv"; then
	last_proposal_number=$(tail -n 1 "$path/data-pkm.csv" | cut -d ',' -f 1)
	proposal_number=$((last_proposal_number + 1))
	echo "$proposal_number,$nama_pengusul,$departemen,$fakultas,$judul,$nama_pendamping,$skema" >> "$path/data-pkm.csv"
	echo "REGISTER: SUCCESS $nama_pengusul is registered. Proposal $proposal_number is added"
	log "REGISTER: SUCCESS $nama_pengusul is registered. Proposal $proposal_number is added"
```

Mengecek nama pengusul pada data-pkm.csv jika tidak ada maka akan mengambil nomor terakhir pada data-pkm.csv menggunakan `tail -n 1 "$path/data-pkm.csv" | cut -d ',' -f 1` dan nomor terakhirnya akan +1 karena kita akan memasukkan inputan di awal ke dalam data-pkm.csv. `$proposal_number,$nama_pengusul,$departemen,$fakultas,$judul,$nama_pendamping,$skema" >> "$path/data-pkm.csv` setelah dimasukkan ke dalam data-pkm.csv maka akan mencetak `REGISTER: SUCCESS $nama_pengusul is registered. Proposal $proposal_number is added`. <br>

```ruby
else
	fakultasCSV=$(awk -F ',' -v nama="$nama_pengusul" '$2 ~ nama {print $4; exit}' "$path/data-pkm.csv")
	nipdCSV=$(awk -F ',' -v nama="$nama_pengusul" '$2 ~ nama {gsub(/[^0-9]/, "", $6); print $6; exit}' "$path/data-pkm.csv")
	password="$fakultasCSV$nipdCSV"
```

Jika username/nama pengusul ada maka akan mengambil fakultas dan nipd di data-pkm.csv dari username tersebut menggunakan `fakultasCSV=$(awk -F ',' -v nama="$nama_pengusul" '$2 ~ nama {print $4; exit}' "$path/data-pkm.csv")` dan `nipdCSV=$(awk -F ',' -v nama="$nama_pengusul" '$2 ~ nama {gsub(/[^0-9]/, "", $6); print $6; exit}' "$path/data-pkm.csv")`. Setelah mendapatkan fakultas dan nipd dari username itu maka akan dibuat password yang sesuai dengan data yang diambil pada data-pkm.csv yaitu `password="$fakultasCSV$nipdCSV"`. <br>

```ruby
        if [[ "$password" == "$passInput" ]]; then
		username=$(echo "$nama_pengusul" | cut -d ' ' -f 1)
		echo "REGISTER: ERROR $username is already existed"
        	log "REGISTER: ERROR $username is already existed"
        else
        	echo "REGISTER: ERROR $username's password is wrong"
        	log "REGISTER: ERROR $username's password is wrong"
        fi
fi
```

Lalu setelah membuat password yang sesuai dengan data di data-pkm.csv maka akan menggunakan if lagi untuk dicek apakah password(data-pkm.csv) sama dengan passInput(password yang dibuat berdasarkan inputan data pengguna). Jika password sesuai maka mencetak `REGISTER: ERROR $username is already existed` dan log akan dicatat melalui fungsi `log()`. Jika password tidak sesuai maka mencetak `REGISTER: ERROR $username's password is wrong` dan log akan dicatat melalui fungsi `log()`. <br>

- Berikut adalah input dan output dari code yu_login.sh <br>
  <img width="838" alt="Screenshot 2024-03-29 130633" src="https://github.com/sisop-its-s24/praktikum-modul-1-f01/assets/116788471/a05944bd-3097-4035-ae25-148d2f4651e1">
  <img width="846" alt="Screenshot 2024-03-29 130800" src="https://github.com/sisop-its-s24/praktikum-modul-1-f01/assets/116788471/4f6247cb-7836-483b-9ade-b25bdcd36f7f">
  <img width="845" alt="Screenshot 2024-03-29 131236" src="https://github.com/sisop-its-s24/praktikum-modul-1-f01/assets/116788471/06c56736-1cf4-4cc5-aa49-62b1f12e5c17">
  <img width="842" alt="Screenshot 2024-03-29 131257" src="https://github.com/sisop-its-s24/praktikum-modul-1-f01/assets/116788471/91b8c861-be2e-4319-ae27-f2f516964d6d">

- Kendala yang saya alami saat mengerjakan soal 2B : <br>
  Kesusahan saat mengecek username dan password menggunakan data-pkm.csv, namun sudah diatasi dengan tetap memvalidasi username dan password menggunakan data-pkm.csv sebelum praktikum berakhir. <br>

> 2C <br>

- Pada 2C ini kita disuruh membuat database yang dapat membuat file users.txt untuk menyimpan username dan password. File users.txt akan di update setiap 1 jam sekali dan cron akan disimpan pada crontabs.

- #### Logic Penyelesaian

- [ ] Buat file users.txt apabila di directory tidak ada
- [ ] Pisahkan nama pengusul berdasarkan \_ dengan memasukannya ke array
- [ ] Deklarasikan fakultas
- [ ] Ambil nipd dengan gsub pada kolom pendamping dan menggunakan NF-1(ambil dari belakang) disebabkan ada delimiter koma
- [ ] Buat password dengan menggabungkan fakultas+nipd
- [ ] Catat pada users.txt
- [ ] Buat crontab untuk update data users.txt setiap 1 jam

- Berdasarkan permasalahan diatas, berikut adalah kode `yu_database.sh` yang saya kerjakan : <br>

```ruby
#!/bin/bash

# untuk ngambil path dan ngecek udah ada users.txt belom
# klo belom nanti dibuat
dir=$(pwd)
if [ ! -f "$dir"/users.txt ]; then
    sudo touch users.txt
    sudo chmod 777 users.txt
    echo "users.txt sudah dibuat"
fi


path="/home/ardhikakrsh/Documents/Kuliah/Sisop/Praktikum-1/praktikum-modul-1/resources"

# fungsi untuk menyimpan username dan password
store_user_pass() {
    awk -F ',' 'NR > 1 {
        split($2, nama, "_");
        fakultas=$4;
        gsub(/[^0-9]/, "", $(NF-1));
        nipd=$(NF-1);
        password=fakultas nipd;
        printf "%s:%s\n", nama[1], password
    }' ""$path"/data-pkm.csv" >> "users.txt"
}

store_user_pass
```

- Berikut adalah penjelasan lebih detail mengenai kode `yu_database.sh` diatas: <br>

```ruby
dir=$(pwd)
if [ ! -f "$dir"/users.txt ]; then
    sudo touch users.txt
    sudo chmod 777 users.txt
    echo "users.txt sudah dibuat"
fi
```

`dir=$(pwd)` adalah berisi path dari direktori saat ini, di mana skrip ini dijalankan. Kemudian mengecek apakah di path direktori ini sudah ada file users.txt, jika belum maka membuat file users.txt dan diberi akses menggunakan `chmod 777`.<br>

```ruby
path="/home/ardhikakrsh/Documents/Kuliah/Sisop/Praktikum-1/praktikum-modul-1/resources"
```

Variabel path yang berisi path ke direktori yang berisi file data-pkm.csv. <br>

```ruby
store_user_pass() {
```

Sebuah fungsi bernama store_user_pass. <br>

```ruby
split($2, nama, "_");
fakultas=$4;
```

Menggunakan awk untuk membaca data-pkm.csv dan memproses baris-barisnya. Menggunakan split untuk memisahkan isi kolom pada $2 yaitu kolom nama_pengusul dengan delimiter \_ untuk memisahkan isi kolom kedua menjadi potongan-potongan. Potongan ini akan disimpan ke dalam array. Simpan isi kolom keempat $4 ke dalam variabel fakultas. <br>

```ruby
gsub(/[^0-9]/, "", $(NF-1));
nipd=$(NF-1);
```

Digunakan untuk menghapus semua karakter yang bukan angka dari kolom kedua terakhir (`$(NF-1)`). Kemudian hasilnya dimasukan disimpan pada variabel nipd. <br>

```ruby
	password=fakultas nipd;
	printf "%s:%s\n", nama[1], password
}' ""$path"/data-pkm.csv" >> "users.txt"
```

membuat password yaitu fakultas dan nipd. Kemudian di print nama depan dan password yaitu `%s:%s\n`, nama[1], password. Diambil dari data-pkm.csv kemudian username dan password dimasukan ke dalam users.txt <br>

```ruby
0 */1 * * * cd /home/ardhikakrsh/Documents/Kuliah/Sisop/Praktikum-1/praktikum-modul-1/task-2 && ./yu_database.sh
```

Menggunakan crontab 1 jam sekali, cd path dari file yang ingin di run && ./sh untuk file yang di run. <br>

- Berikut adalah input dan output dari code yu_database.sh <br>
  <img width="599" alt="image" src="https://github.com/sisop-its-s24/praktikum-modul-1-f01/assets/116788471/3995bb25-5422-463f-bca6-614ed1580e49">

- Kendala yang saya alami saat mengerjakan soal 2C : <br>
  Awalnya code masih error dengan bisa dan tidak bisanya membuat file users.txt disebabkan path yang salah dan tidak jalannya crontab disebabkan syntax yang salah. Namun kedua hal dapat diatas sebelum praktikum selesai.

---

# 🌱 Nomor 3

### 3A

- Masukkan semua metrics ke dalam suatu file log bernama `metrics_{YmdHms}.log`. `{YmdHms}` adalah waktu disaat file script bash kalian dijalankan. Misal dijalankan pada 2024-03-30 15:00:00, maka file log yang akan tergenerate adalah `metrics_20240330150000.log`.

#### Jawab

Langkah-langkah pengerjaan:

1. Edit file `minute_log.sh`
2. Dalam pengerjaan shell script menggunakan command mkdir, while loop, variabel constant, echo, free, du, date

Berikut adalah shell script yang dibuat :

```
#!/bin/bash

i=0

metrics_dir="/home/$(whoami)/metrics"

if [ ! -d "$metrics_dir" ]; then
    mkdir -p "$metrics_dir"
fi

while read -r line
do
    random_directory[$i]="$line"
    (( i++ ))
done < <(cd ~ && ls -d */)

declare -r DIRECTORY=${random_directory[$RANDOM% ${#random_directory[@]}]}
timestamp=$(date +'%Y%m%d%H%M%S')

memory_metrics=$(free -m | awk 'NR==2 {print $2","$3","$4","$5","$6","$7}')
swap_metrics=$(free -m | awk 'NR==3 {print $2","$3","$4}')
disk_metrics=$(du -sh "/home/$(whoami)/$DIRECTORY" | awk '{print $1}')

echo "mem_total,mem_used,mem_free,mem_shared,mem_buff,mem_available,swap_total,swap_used,swap_free,path,path_size" >> /home/$(whoami)/metrics/metrics_${timestamp}.log
echo "$memory_metrics,$swap_metrics,/home/$(whoami)/$DIRECTORY,$disk_metrics" >> /home/$(whoami)/metrics/metrics_${timestamp}.log
```

3. Penjelasan command:

- echo untuk menampilkan string ke terminal
- mkdir untuk membuat directory baru
  1.  -p -> memberitahukan perintah untuk membuat direktori induk yang belum ada
- while loop untuk perulangan

  1.  while read -r line -> membaca baris input dan menyimpannya di variabel line, option 'r' itu mencegah garis miring terbalik diinterpretasikan sebagai karakter escape.
  2.  random_directory[$i]="$line" -> isi dari variabel line akan disimpan ke dalam array random_directory[sesuai dengan indexnya]
  3.  (( i++ )) -> increment variabel i agar indexnya tetap naik
  4.  done < <(cd ~ && ls -d \*/) -> akan menjadi input bagi while loop5
  5.  cd ~ && ls -d \*/ -> masuk ke directory ~ lalu list yang folder aja

- declare di shell script sama halnya seperti konstanta

  1.  option -r ditetapkan sebagai read-only. Hal ini mencegah modifikasi nilai variabel yang tidak disengaja di kemudian hari dalam skrip.
  2.  declare -r DIRECTORY=${random_directory[$RANDOM% ${#random_directory[@]}]} -> membuat konstanta dengan nama DIRECTORY (konvensional konstanta) = array dari random_directory[lalu isinya dirandom menggunakan fungsi random]
  3.  $RANDOM% ${#random_directory[@]} -> @ itu seperti ngambil semua yang ada jadi dirandom terus dipilih salah satu dari semua directory yang ada

- RANDOM function untuk ngerandom
- date untuk mengambil/menyimpan tanggal dan waktu sesuai yang diinginkan
- free -m untuk melihat memory dan swap
- du sh untuk melihat size dari directory tersebut

#### Logic Penyelesaian

- [ ] Simpan path to directory metrics ke dalam suatu variabel (saya menggunakan metrics_dir sebagai variabel untuk menyimpannya) ini akan digunakan untuk pembuatan folder
- [ ] Cek apakah directory metrics_dir sudah ada atau belum, jika belum maka buat foldernya terlebih dahulu
- [ ] Inisialisasi sebuah variabel dan assign nilai 0 (ini akan digunakan untuk looping)
- [ ] Karena di soal tidak menyebutkan folder apa yang harus digunakan, saya juga bertanya kepada probset apakah boleh menggunakan directory random yang berada di home/user/, dan jawabannya iya (dibebasin), maka saya menggunakan/menyimpan semua folder yang ada di home/user/ dan kemudian diassign ke dalam suatu variabel
- [ ] Karena semua folder sudah disimpan ke dalam suatu variabel maka untuk menampilkan satu folder yang sama dengan command `free -m` dan `du -sh` (kayak pasti gitu nilainya tidak mungkin berubah) maka saya menerapkan variabel constant yang isinya itu salah satu folder yang telah dirandom(diacak)
- [ ] Lalu setelah itu dapatkan date dengan konfigurasi Year month date Hour MINUTE SECOND (supaya sesuai dengan permintaan soal)
- [ ] Setelah itu gunakan piping untuk menghubungkan output suatu command ke input command lain tanpa file sementara apa pun
- [ ] memory_metrics=$(free -m | awk 'NR==2 {print $2","$3","$4","$5","$6","$7}') jadi output dari free -m akan dijadikan inputan dari awk, NR==2 itu untuk mengambil baris kedua dan print mulai dari kolom 2,3,4,5,6,7
- [ ] swap_metrics=$(free -m | awk 'NR==3 {print $2","$3","$4}') sama seperti di atas, output dari free -m akan dijadikan inputan dari awk, NR==3 itu untuk mengambil baris ketiga dan print mulai dari kolom 2,3,4
- [ ] disk_metrics=$(du -sh "/home/$(whoami)/$DIRECTORY" | awk '{print $1}')
- [ ] lanjutan -> pertama akan ngejalanin command `du -sh /home/$(whoami)/$DIRECTORY` lalu dipiping ke awk dan print kolom ke-1 saja
- [ ] Setelah itu tinggal ikutin sesuai permintaan soal untuk menampilkan `mem_total,mem_used,mem_free,mem_shared,mem_buff,mem_available,swap_total,swap_used,swap_free,path,path_size` tinggal tambahin echo "{diisi oleh mem_total sampai path_size}"
- [ ] Nah kan di soal dikasih tau "Semua file log terletak di `/home/{user}/metrics`" berarti cara untuk nyimpan/rewrite kalimat tersebut ke dalam suatu file bisa menggunakan tanda >> lalu diikuti dengan path/to/file, kalau codingan yang saya berikan seperti ini `/home/$(whoami)/metrics/metrics_${timestamp}.log`
- [ ] Lanjut sekarang tampilkan isi dari memory*metrics, swap_metrics, /home/$(whoami)/$DIRECTORY, dan disk_metrics. Ini akan ditampilkan di bawah/baris baru dari command sebelumnya, caranya sama tinggal gunakan `>> /home/$(whoami)/metrics/metrics*${timestamp}.log`

#### Kendala

1. Tidak ada sama sekali karena menurut saya soal ini paling asik banget untuk dikerjain
2. Tidak ada revisi

#### Output

<img src="https://cdn.discordapp.com/attachments/1223328224281497610/1223634888540946492/image.png?ex=661a91cb&is=66081ccb&hm=9dbb61813af4333a584e00c05dde3463bb642ad0509b8dbc5574c035cc33736f&">

### 3B

- Script untuk mencatat metrics diatas diharapkan dapat berjalan otomatis setiap menit.

#### Jawab

```
	* * * * * /home/riyanda/TeknikInformatika/Semester4/Sisop/praktikum-modul-1-f01/task-3/minute_log.sh 2>> error.log
```

#### Logic Penyelesaian

untuk crontab:
minute hour day(month) month day(week)
dijalankan tiap menit dan jika ada error tampilkan ke file error.log

#### Kendala

1. Tidak ada

#### Output

<img src="https://cdn.discordapp.com/attachments/1223328224281497610/1223636262464913428/image.png?ex=661a9313&is=66081e13&hm=d24f447c3e0e119577af846ae374d33be5a91dcab5074b363c6eb1f9126bf683&">

<br>

<img src="https://cdn.discordapp.com/attachments/1223328224281497610/1223636635753910383/image.png?ex=661a936c&is=66081e6c&hm=d6bb177fc44643c2fc7ad5f79c05e78f78b6ce031114958b56cb8185d95a9055&">

### 3C

- Kemudian, buat satu script untuk membuat aggregasi file log ke satuan jam. Script aggregasi akan memiliki info dari file-file yang tergenerate tiap menit. Dalam hasil file aggregasi tersebut, terdapat nilai minimum, maximum, dan rata-rata dari tiap-tiap metrics. File aggregasi akan ditrigger untuk dijalankan setiap jam secara otomatis. Berikut contoh nama file hasil aggregasi `metrics_agg_2023033015.log` dengan format `metrics_agg_{YmdH}.log`.

#### Jawab

Langkah-langkah pengerjaan:

1. Edit file `aggregate_minutes_to_hourly_log.sh`
2. Dalam pengerjaan shell script kali ini kurang lebih sama seperti soal sebelumnya

Berikut adalah shell script yang dibuat :

```
#!/bin/bash

metrics_dir="/home/$(whoami)/metrics"
agg_files=$(ls "$metrics_dir"/metrics* | grep metrics_2024)
value_path_size=()
maximum=0
minimum=999653638144
total_size=0
count=0
min_entry=""
max_entry=""
avg_entry=""
timestamp=$(date +'%Y%m%d%H')

for file in $agg_files; do
	values=$(awk 'NR==2{print $0}' "$file")
	avg_value=$(awk -F ',' 'NR==2{ print $1 $2 $3 $4 $5 $6 $7 $8 $9 $10}' "$file")
	path_sizes=$(awk -F ',' 'NR==2{print $11}' "$file")

	for path_size in $path_sizes; do
		if [ -z "$path_size" ]; then
			continue
		fi

		numeric=$(echo "$path_size" | sed 's/[^0-9]//g')
		unit=$(echo "$path_size" | sed 's/[0-9]*//g')

		case $unit in
			"K") size_bytes=$(($numeric * 1024));;
			"M") size_bytes=$(($numeric * 1024 * 1024));;
			"G") size_bytes=$(($numeric * 1024 * 1024 * 1024));;
			*) size_bytes=$numeric;;
		esac

		if [ "$size_bytes" -lt "$minimum" ]; then
			minimum=$size_bytes
			min_entry="$values"
		fi
		if [ "$size_bytes" -gt "$maximum" ]; then
			maximum=$size_bytes
			max_entry="$values"
		fi

		total_size=$(($total_size + $size_bytes))
		count=$(($count + 1))
	done
done

if [ "$count" -gt 0 ]; then
	avg=$(($total_size / $count))

	if [ "$avg" -ge 1073741824 ]; then
		avg=$(($avg / 1073741824))
		unit="G"
	elif [ "$avg" -ge 1048576 ]; then
		avg=$(($avg / 1048576))
		unit="M"
	elif [ "$avg" -ge 1024 ]; then
		avg=$(($avg / 1024))
		unit="K"
	else
		unit="B"
	fi

	avg_entry="$avg_value,$avg$unit"
fi

echo "mem_total,mem_used,mem_free,mem_shared,mem_buff,mem_available,swap_total,swap_used,swap_free,path,path_size" >> /home/$(whoami)/metrics/metrics_agg_${timestamp}.log
echo "minimum,$min_entry" >> /home/$(whoami)/metrics/metrics_agg_${timestamp}.log
echo "maximum,$max_entry" >> /home/$(whoami)/metrics/metrics_agg_${timestamp}.log
echo "average,$avg_entry" >> /home/$(whoami)/metrics/metrics_agg_${timestamp}.log

```

Untuk crontabnya seperti di bawah ini:

```
0 * * * * /home/riyanda/TeknikInformatika/Semester4/Sisop/praktikum-modul-1-f01/task-3/aggregate_minutes_to_hourly_log.sh 2>> error2.log
```

#### Logic Penyelesaian

Untuk shell script sendiri:

- [ ] Karena proses pembuatan folder metrics_dir sudah dilakukan di poin sebelumnya maka tidak perlu lagi untuk membuat foldernya
- [ ] simpan path/to/metrics ke dalam suatu variabel, seperti metrics_dir="/home/$(whoami)/metrics"
- [ ] lalu simpan semua file yang bernama metrics_2024 ke dalam suatu variabel, seperti agg_files=$(ls "$metrics_dir"/metrics\* | grep metrics_2024) karena saat file minute_log.sh dijalankan filenya yang dihasilkan pasti diawali dengan metrics_2024
- [ ] lalu buat array dan tidak perlu diisi nilainya, seperti value_path_size=()
- [ ] Buat sebuah variabel min dan max untuk mencari path_value yang min/max nantinya, seperti maximum=0 dan minimum=999653638144 (untuk minimum saya ambil nilai terbesar dari GB dan max tentunya 0)
- [ ] Saya juga membuat variabel min_entry="", max_entry="", dan avg_entry="" untuk mengisi string di dalamnya
- [ ] Saya juga membuat variabel total_size=0 dan count=0, untuk menghitung semua size
- [ ] Saya juga membuat variabel timestamp untuk menyimpan format waktu yang diminta oleh soal, seperti timestamp=$(date +'%Y%m%d%H') dengan konfigurasi Year month date Hour
- [ ] Lakukan loop foreach dengan variabel agg_files
- [ ] Lalu saya buat 3 variabel
      values=$(awk 'NR==2{print $0}' "$file") untuk ngeprint baris kedua dan semua kolom
      avg_value=$(awk -F ',' 'NR==2{ print $1 $2 $3 $4 $5 $6 $7 $8 $9 $10}' "$file") untuk ngprint start dari baris kedua dan kolom 1-10
      path_sizes=$(awk -F ',' 'NR==2{print $11}' "$file") untuk ngambil nilainya saja
- [ ] Lalu ngecek apakah path_size ini kosong atau tidak, jika kosong maka lanjutkan ke iterasi file berikutnya
- [ ] Lalu ngefilter path_size kan ada 100M, 10G, 200K, dst. Nah itu antara angka dan hurufnya dipisah menggunakan command di bawah
      numeric=$(echo "$path_size" | sed 's/[^0-9]//g')
      unit=$(echo "$path_size" | sed 's/[0-9]\*//g')
- [ ] Setelah itu buat case kalau unitnya "K" maka size_bytesnya = numeric dikali 1024, jika "M" maka dikalikan 1.048.576, dst jika tidak maka size_bytesnya = numeric itu sendiri
- [ ] Lalu dilakukan pengecekan apakah size_bytesnya itu < minimum jika iya maka minimum = size_bytesnya dan min_entry menyimpan nilai values
- [ ] Lalu dilakukan pengecekan apakah size_bytesnya itu > max jika iya maka minimum = size_bytesnya dan max_entry menyimpan nilai values
- [ ] Lalu size_bytes semuanya akan dijumlah dan diassign ke variabel total_size
- [ ] Lalu tiap iterasi path_size itu akan dihitung oleh var count (jadi tau berapa folder yang digunakan dan akan dipakai sebagai nilai averagenya)
- [ ] Lalu lakukan pengecekan apakah countnya itu lebih besar dari 0, jika iya lakukan operasi pembagian total_size/count dan disimpan ke var avg
- [ ] Setelah itu dicek lagi apakah nilai avg itu lebih besar sama dengan 1073741824, jika iya maka avg akan dibagi dengan 1073741824 lalu buat semacam flag/var kalau itu Gb/G. Jika kondisi pertama tidak terpenuhi cek apakah avg >= 1048576 maka avg = avg itu sendiri dibagi 1048576 dan beri flag/var kalau itu M/Megabyte. Jika kondisi kedua tidak terpenuhi cek apakah avg >= 1024 maka avg = avg itu sendiri dibagi 1024 dan beri flag/var kalau itu K/Kilobyte, dan kondisi terakhir adalah B/byte
- [ ] Lalu saya buat untuk menyimpan nilai-nilai average itu ke dalam variabel khusus seperti command di bawah ini
      avg_entry="$avg_value,$avg$unit" (string dari avg_value disatukan dengan avg dan unitnya)
- [ ] Setelah itu tinggal tampilkan sesuai dengan apa yang diminta soal
      echo "mem_total,mem_used,mem_free,mem_shared,mem_buff,mem_available,swap_total,swap_used,swap_free,path,path_size" >> /home/$(whoami)/metrics/metrics_agg*${timestamp}.log
      echo "minimum,$min*entry" >> /home/$(whoami)/metrics/metrics_agg*${timestamp}.log
	  echo "maximum,$max*entry" >> /home/$(whoami)/metrics/metrics_agg*${timestamp}.log
      echo "average,$avg*entry" >> /home/$(whoami)/metrics/metrics_agg\*${timestamp}.log

Untuk crontab sendiri:
minute hour day(month) month day(week)
dijalankan tiap jam menit 00 dan jika ada error tampilkan ke file error2.log

#### Kendala

1. Kendala yang saya alami yaitu, bingung tidak ada pnejelasan tentang directory yang digunakan bebas atau tidak
2. Directory untuk nilai avg itu maksudnya min sama max dibagi dua atau gimana soalnya saya liat dari contoh seperti dibagi dua, tapi saya tanya kepada probset jika avgnya itu dipakai dari semua file metrics_2024 apakah diperbolehkan dan diperbolehkan sama probset
3. Revisi (lupa nambahin crontabnya)

#### Output

<img src="https://cdn.discordapp.com/attachments/1223328224281497610/1223637242208194620/image.png?ex=661a93fc&is=66081efc&hm=c578f483f681f480f1bad2529d61abe20b6561c2ca6f74907ea79da1a7f8f65b&">

### 3D

- Selanjutnya agar lebih menghemat penyimpan, buatlah script [backup_metrics.sh](./bakcup_metrics.sh). Dimana script ini akan menyimpan semua log metrics aggregasi mulai dari pukul 00:00 sampai 23:59 didalam 1 file .gz menggunakan gunzip. Contoh nama file hasil zip `backup_metrics_{date_YmdH}.gz`

#### Jawab

Langkah-langkah pengerjaan:

1. Edit file `backup_metrics.sh`
2. Dalam pengerjaan shell script kali ini menggunakan command tar, gzip, chmod, cd

Berikut adalah shell script yang dibuat :

```
#!/bin/bash

timestamp=$(date +'%Y%m%d%H')

backup_file="backup_metrics_${timestamp}.tar.gz"

cd /home/$(whoami)/metrics/

log_files=$(ls metrics_agg_*.log 2>/dev/null)
log=$(ls metrics_*.log)

if [ -n "$log_files" ]; then
	tar -cf "${backup_file}" ${log_files}
	chmod 400 ${log}
	gzip "${backup_file}"
fi

```

untuk crontab:

```
0 0 * * * /home/riyanda/TeknikInformatika/Semester4/Sisop/praktikum-modul-1-f01/task-3/bakcup_metrics.sh 2>> error3.log
```

3. Penjelasan command:

- cd untuk change directory atau berpindah folder
- tar untuk membuat file backup
  1.  -c digunakan untuk membuat file baru (create), dan -f digunakan untuk menentukan nama file backup yang akan dibuat.
- gzip untuk mengkompres file

#### Logic Penyelesaian

untuk shell script:

- [ ] Dapatkan waktu sekarang dengan fungsi data dengan konfigurasi Year month date Hour dan simpan ke variabel timestamp
- [ ] Buat nama file backup*metrics*{dengan waktu dari timestamp}.tar.gz dan simpan ke variabel
- [ ] Pindah directory ke metrics
- [ ] Buat variabel log_files nanti akan diisi oleh semua file yang namanya metrics_agg dan jika ada error lempar ke /dev/null
- [ ] Buat variabel log nanti akan diisi oleh semua file yang namanya metrics\_\*.log, ini akan dipakai buat ngepastiin file log hanya dapat dibaca oleh user pemilik file
- [ ] Lalu dicek apakah log_files tidak kosong, jika iya buat file backup dengan cara `tar -cf {nama_file} {file apa aja yang diinclude}`
- [ ] Setelah itu ganti permission agar hanya user pemilik file dapat membaca filenya sendiri, dengan cara `chmod 400 {file apa aja yang akan diincludekan}
- [ ] Setelah itu baru dikompress menggunakan gzip {nama_file}

Untuk crontab sendiri:
minute hour day(month) month day(week)
dijalankan tiap jam 00 menit 00 dan jika ada error tampilkan ke file error3.log

#### Kendala

1. Sebelum ada announcement udah cari di internet bahkan youtube dan memang tidak bisa jika tidak menggunakan tar
2. Setelah itu saya kirim beberapa link dari internet yang memberitahukan hal yang sama
3. Revisi (lupa nambahin crontabnya)

#### Output

<img src="https://cdn.discordapp.com/attachments/1223328224281497610/1223638002786504815/image.png?ex=661a94b1&is=66081fb1&hm=9f8c5826459de3d8eb1bc1ae59e77786c13f3da1cf1a82f4e3d211ead6858834&">

### 3E

- e. Karena file log bersifat sensitif pastikan semua file log hanya dapat dibaca oleh user pemilik file.

#### Jawab

Udah terjawab di soal sebelumnya

## 🔅 Nomor 4

### 4A

Isabel meminta tolong untuk menyimpan semua foto-foto yang dikirim oleh pacarnya dengan ketentuan

- Gambarnya di download setiap 5 jam sekali mulai dari saat script dijalankan dan memperhatikan waktu sekarang.
- Jika waktu genap, maka simpan foto sebanyak 5x.
- Jika waktu ganjil, maka simpan foto sebanyak 3x
- Jika pukul menunjukkan 00.00, maka simpan foto sebanyak 10x
- File yang didownload memiliki format nama `foto_NOMOR.FILE` dengan `NOMOR.FILE` adalah urutan file yang download (`foto_1`, `foto_2`, dst)
- File batch yang didownload akan dimasukkan ke dalam folder dengan format nama `folder_NOMOR.FOLDER` dengan `NOMOR.FOLDER` adalah urutan folder saat dibuat (`folder_1`, `folder_2`, dst)

#### Jawab

Langkah-langkah pengerjaan:

1. Mencari gambar "Mingyu Seventeen” di browser dan mengcopy link gambarnya.
2. Open vscode dan melakukan clone praktikum-modul-1-f01 ke vscode
3. Membuat dan mengedit file `isabel.sh`
4. Membuat script untuk menyelesaikan problem yang diminta dengan menggunakan command `mkdir`, `ls`, `awk`, dan `wget`. Berikut adalah shell script yang saya buat :

```ruby
# Fungsi untuk mengunduh gambar Mingyu (soal A)
download_mingyu_image() {
    #Image yang akan di download
    image_url="https://w0.peakpx.com/wallpaper/238/866/HD-wallpaper-mingyu-kim-kpop-music-pledis-seventeen-svt.jpg"

    last_folder=$(ls -d "../task-4/folder_"* | tail -n 1)

    # Jika tidak ada folder yang ditemukan, folder_counter=1
    if [ -z "$last_folder" ]; then
        folder_counter=1
    else
        # Mencari nomor folder terakhir
        last_folder_number=$(basename "$last_folder" | awk -F '_' '{print $2}')
        # Menetapkan folder_counter menjadi nomor folder terakhir + 1
        folder_counter=$((last_folder_number + 1))
    fi

    # Membuat direktori baru
    mkdir -p "../task-4/folder_$folder_counter"

    current_hour=$(date +%H)
    current_minute=$(date +%M)
    current_time=$current_hour$current_minute

    # Menghitung jumlah foto yang akan diunduh berdasarkan aturan yang diberikan
    if [[ $current_time == "0000" ]]; then
        download_count=10
    elif [[ $(($current_hour % 2)) -eq 0 ]]; then
        download_count=5
    else
        download_count=3
    fi

    # Mengunduh foto sesuai dengan jumlah yang ditentukan
    for ((i=1; i<=$download_count; i++)); do
        wget -q "$image_url" -O "../task-4/folder_$folder_counter/foto_${i}.jpg"
    done
}
```

4. Membuat cronjob untuk menjalankan fungsi sesuai waktu yang diminta yaitu, setiap 5 jam sekali

```ruby
    0 */5 * * * cd /home/faiq/Documents/Kuliah/Sisop/'Praktikum 1'/praktikum-modul-1-f01/task-4/ && ./isabel.sh download_mingyu_image
```

5. Penjelasan command

- `ls -d` digunakan untuk mencari semua folder yang dimulai dengan `../task-4/folder_`.
- `tail -n 1` digunakan untuk memilih folder terakhir dari daftar folder yang ditemukan.
- `-z` untuk mengecek apakah sebuah variabel bernilai null.
- `awk -F '_' '{print $2}'` memiliki fungsi untuk memproses input yang dipisahkan oleh karakter underscore (`_`) dan kemudian mencetak bagian kedua dari setiap baris input.
- `mkdir -p` membuat direktori target beserta seluruh struktur direktori di atasnya jika belum ada.
- `wget` digunakan untuk mengunduh file dari URL yang ditentukan. Adapun command lainya :
  1. `-q` digunakan untuk menonaktifkan output atau pesan yang dihasilkan oleh `wget`.
  2. `-O` digunakan untuk menentukan lokasi dan nama file untuk menyimpan gambar yang diunduh.

#### Output dari Script:

- Ketika waktu ganjil
  gambar ganjil

- Ketika waktu 00:00
  gambar 0000

#### Logic Penyelesaian:

- [ ] Membuat variabel `image_url` untuk menyimpan link gambar yang ingin di download.
- [ ] Menggunakan `ls -d` untuk mengecek isi direktori dan `tail -n 1` untuk memilih folder terakhir saja.
- [ ] Membuat variabel `folder_counter` untuk menentukan folder ke berapa.
- [ ] Menggunakan `awk` untuk mengambil kata terakhir dari `folder_*`
- [ ] Membuat direktori `folder_*` menggunakan `mkdir`
- [ ] Gunakan `if`,`elif`, dan `else` untuk ngehandle berapa file yang akan didowload untuk waktu genap, ganjil, dan 00:00.
- [ ] Membuat perulangan untuk mendownload gambar(menggunakan `wget`) sesuai dengan waktu sekarang menggunakan.

#### Kendala:

Sejauh saya mengerjakan tidak ada kendala yang dialami.

### 4B

Karena memory laptop Isabel penuh, maka bantulah Isabel untuk zip foto-foto tersebut dengan ketentuan:

- Melakukan zip setiap 1 jam dengan nama zip ayang_NOMOR.ZIP dengan NOMOR.ZIP adalah urutan folder saat dibuat (ayang_1, ayang_2, dst). Yang di ZIP hanyalah folder dari soal di atas.

#### Jawab

Langkah-langkah pengerjaan:

1. Menambahkan fungsi `create_zip_files` kedalam file `isabel.sh`
2. Membuat script untuk menyelesaikan problem yang diminta dengan menggunakan command `echo`, `ls`, dan `zip`. Berikut adalah shell script yang saya buat :

```ruby
# Membuat file zip yang menyimpan folder soal A (Soal B)
create_zip_files() {
    # Mendapatkan daftar folder yang akan di-zip
    folders=$(ls -d "folder_"*)

    # Menentukan nomor zip berdasarkan jumlah file zip yang sudah ada
    zip_count=$(ls "ayang_"*.zip | wc -l)
    if [ $zip_count -eq 0 ]; then
        zip_counter=1
    else
        zip_counter=$((zip_count + 1))
    fi

    # # Memeriksa apakah ada folder yang akan di-zip
    # if [ -z "$folders" ]; then
    #     echo "Tidak ada folder yang ditemukan di dalam direktori ../task-4."
    #     exit 1
    # fi

    # Melakukan zip untuk setiap folder
    for folder in $folders; do
        folder_name=$(basename "$folder")
        zip_file="../task-4/ayang_$zip_counter.zip"
        zip -r "$zip_file" "$folder"
        # echo "Folder $folder_name telah di-zip ke $zip_file pada $(date)."
        zip_counter=$((zip_counter + 1))
    done
}
```

4. Membuat cronjob untuk menjalankan fungsi sesuai waktu yang diminta yaitu, setiap 10 jam sekali

```ruby
    0 */10 * * * cd /home/faiq/Documents/Kuliah/Sisop/'Praktikum 1'/praktikum-modul-1-f01/task-4/ && ./isabel.sh create_zip_files
```

5. Penjelasan command

- `ls` digunakan untuk mencari folder `"ayang_"*.zip`.
- `wc -l` digunakan untuk menghitung jumlah baris dalam sebuah file teks.
- `zip` digunakan untuk membuat file arsip dalam format ZIP dari satu atau lebih direktori atau file.
- `-r` menandakan bahwa zip akan dibuat secara rekursif, sehingga semua file dan direktori di dalamnya akan disertakan.

#### Output dari Script:

-Hasil zip

-isi zip

#### Logic Penyelesaian:

- [ ] Membuat variabel `folders` untuk mendapatkan folder yang akan di zip.
- [ ] Menentukan nomor zip menggunakan `zip_count` dimana akan dilihat dari jumlah file.zip yang ada menggunakan `ls`
- [ ] Menggunakan `if`, `else` untuk menentukan `zip_count`nya.
- [ ] Membuat perulangan untuk menzip setiap folder pada 1 satu file.zip

#### Kendala:

Sejauh saya mengerjakan tidak ada kendala yang dialami.

### 4C

Ternyata laptop Isabel masih penuh, bantulah dia untuk delete semua folder dan zip setiap hari pada pukul 02.00!

#### Jawab

Langkah-langkah pengerjaan:

1. Menambahkan fungsi `delete_folders_and_zip_files` kedalam file `isabel.sh`
2. Membuat script untuk menyelesaikan problem yang diminta dengan menggunakan command `rm` Berikut adalah shell script yang saya buat :

```ruby
# Fungsi untuk menghapus folder dan zip file (Soal C)
delete_folders_and_zip_files() {
    # Menghapus semua folder yang ada di dalam direktori ../task-4
    rm -rf ../task-4/folder_*

    # Menghapus semua file zip yang ada di dalam direktori ../task-4
    rm -f ../task-4/*.zip
}
```

3. Membuat cronjob untuk menjalankan fungsi sesuai waktu yang diminta yaitu, setiap jam 02:00

```ruby
    0 2 * * * cd /home/faiq/Documents/Kuliah/Sisop/'Praktikum 1'/praktikum-modul-1-f01/task-4/ && ./isabel.sh delete_folders_and_zip_files
```

4. Penjelasan command

- `rm -rf` digunakan untuk menghapus direktori/folder/file beserta seluruh isinya secara rekursif dan tanpa konfirmasi

#### Output dari Script:

-Hasil zip

#### Logic Penyelesaian:

- [ ] Membuat script yang dapat menghapus semua `folder_*` dan semua file zip yang ada.

#### Kendala:

Sejauh saya mengerjakan tidak ada kendala yang dialami.

### 4D

Untuk mengisi laptopnya kembali, Isabel ingin mendownload gambar Levi dan Eren secara bergantian setiap harinya dengan ketentuan nama file:

- Jika Levi, maka nama file nya menjadi `levi_YYYYMMDD` (Dengan `YYYYMMDD` adalah tahun, bulan, dan tanggal gambar tersebut di download)

- Jika Eren, maka nama file nya menjadi `eren_YYYYMMDD` (Dengan `YYYYMMDD` adalah tahun, bulan, dan tanggal gambar tersebut di download)

#### Jawab

Langkah-langkah pengerjaan:

1. Mencari gambar "levi” dan "eren" di browser dan mengcopy link gambarnya.
2. Menambahkan fungsi `download_levi_or_eren_image` kedalam file `isabel.sh`
3. Membuat script untuk menyelesaikan problem yang diminta dengan menggunakan command `echo`, `mkdir`, dan `wget` Berikut adalah shell script yang saya buat :

```ruby
# Fungsi untuk mengunduh gambar Levi atau Eren (Soal D)
download_levi_or_eren_image() {
    # Count the number of files in the "anime" folder
    num_files=$(ls "../task-4/anime" | wc -l)

    # Determine the character based on the number of files
    if [ $((num_files % 2)) -eq 0 ]; then
        next_character="eren"
    else
        next_character="levi"
    fi

    echo "$next_character"

    # URL for the image (replace with the appropriate URL)
    if [ "$next_character" == "levi" ]; then
        image_url="https://w0.peakpx.com/wallpaper/347/5/HD-wallpaper-levi-ackerman-naruto-atackontitan-aot-eren-mikasa.jpg"
    else
        image_url="https://w0.peakpx.com/wallpaper/170/582/HD-wallpaper-eren-yeager-attack-on-titan-attack-on-titan-s4-eren-jaeger-shingeki-no-kyojin-shingeki-no-kyojin-s4.jpg"
    fi

    # Create the "anime" folder if it doesn't exist
    mkdir -p "../task-4/anime"

    # Download the image with the filename based on the character and date into the "anime" folder
    wget -q -P "../task-4/anime" "$image_url" -O "../task-4/anime/${next_character}_$(date "+%Y%m%d").jpg"
}
```

4. Membuat cronjob untuk menjalankan fungsi sesuai waktu yang diminta yaitu, setiap jam 00:00

```ruby
    0 0 * * * cd /home/faiq/Documents/Kuliah/Sisop/'Praktikum 1'/praktikum-modul-1-f01/task-4/ && ./isabel.sh download_levi_or_eren_image
```

5. Penjelasan command

- `ls` digunakan untuk mencari folder `"anime`.
- `wc -l` digunakan untuk menghitung jumlah baris dalam sebuah file teks.
- `wget` digunakan untuk mengunduh file dari URL yang ditentukan. Adapun command lainya :
  1. `-q` digunakan untuk menonaktifkan output atau pesan yang dihasilkan oleh `wget`.
  2. `-O` digunakan untuk menentukan lokasi dan nama file untuk menyimpan gambar yang diunduh.

#### Output dari Script:

-Hasil anime 1

-Hasil anime 1

#### Logic Penyelesaian:

- [ ] Membuat variabel `num_files` untuk melihat ada berapa file yang ada pada folder `anime`.
- [ ] Menggunakan `if` dan `else` untuk menentukan gambar apa yang akan didownload sesuai banyaknya file didalam folder `anime`.
- [ ] Memasukkan link gambar pada variabel `image_url`.
- [ ] Membuat folder anime untuk menampung gambar yang di download menggunakan `mkdir`.
- [ ] Mendownload gambar "eren" atau "levi" menggunakan `wget`.

#### Kendala:

Tidak mendapatkan fungsi yang tepat agar mendownload secara bergantian. Pada awalnya saya menggunakan `current_day` tapi akan salah jika setelah tanggal 31 lalu tanggal 1, hingga akhirnya saya menemukan cara diatas

### Revisi Keseluruhan

1. Revisi file `isabel-a`, `isabel-b`, `isabel-c`, dan `isabel-d` menjadi satu file saja yaitu `isabel.sh` yang dapat menangani masalah di soal A, B, C, dan D. Hasil kode saya setalelah revisi tersebut :

foto

> disini saya menggunakan main program yang dimana jika dipanggil fungsinya akan menjalan fungsi yang diminta saja.

2. Revisi minor : isi zip

- dimana isi zip saya ada folder `...`, `task-4`, dan `folder_*` yang seharusnya langsung `folder_*` saja :

- Script sebelum

- Script sesudah

> disini saya menghapus `...` dan `tas-4`

3. Revisi fungsi mendownload bergantian `eren` dan `levi` :

- Script sebelum

-Script sesudah

> Untuk revisinya saya menggunakan pendekatan menghitung jumlah file yang ada di dalam folder anime.
