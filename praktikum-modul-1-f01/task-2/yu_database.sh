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

