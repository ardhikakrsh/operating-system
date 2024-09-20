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
		echo "REGISTER: ERROR $nama_pengusul is already existed"
        	log "REGISTER: ERROR $nama_pengusul is already existed"
        else
        	echo "REGISTER: ERROR $nama_pengusul's password is wrong"
        	log "REGISTER: ERROR $nama_pengusul's password is wrong"	
        fi
fi