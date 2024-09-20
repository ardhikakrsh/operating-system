#!/bin/bash

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

# Fungsi untuk menghapus folder dan zip file (Soal C)
delete_folders_and_zip_files() {
    # Menghapus semua folder yang ada di dalam direktori ../task-4
    rm -rf ../task-4/folder_*

    # Menghapus semua file zip yang ada di dalam direktori ../task-4
    rm -f ../task-4/*.zip
}

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

# Main program
# Menjalankan fungsi berdasarkan argumen yang diberikan
case "$1" in
    "download_mingyu_image")
        download_mingyu_image
        ;;
    "create_zip_files")
        create_zip_files
        ;;
    "delete_folders_and_zip_files")
        delete_folders_and_zip_files
        ;;
    "download_levi_or_eren_image")
        download_levi_or_eren_image
        ;;
    *)
        echo "Usage: $0 {download_mingyu_image|create_zip_files|delete_folders_and_zip_files|download_levi_or_eren_image}"
        exit 1
        ;;
esac