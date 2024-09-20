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


