# Gunakan base image alpine
FROM alpine:latest

# Instal gcc, musl-dev, dan bash untuk kompilasi program C, eksekusi skrip bash, dan instalasi Docker CLI
RUN apk add --no-cache gcc musl-dev bash docker-cli

# Set working directory
WORKDIR /app

# Salin file chat.c ke dalam app
COPY chat.c .

# Kompilasi program chat.c dan pengaturan.c menggunakan gcc
RUN gcc chat.c -o chat

# Jalankan chat dalam loop tak terbatas
CMD ["sh", "-c", "while true; do ./chat; sleep 10; done"]
