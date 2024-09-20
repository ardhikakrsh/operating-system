# Gunakan base image yang sesuai
FROM python:3.8

# Install kaggle CLI menggunakan pip
RUN pip install kaggle

# Menyalin kode program C ke dalam image
COPY ayamgoreng.c /usr/src/ayamgoreng.c

# Kompilasi program C
RUN gcc /usr/src/ayamgoreng.c -o /usr/src/ayamgoreng

# Menentukan direktori kerja
WORKDIR /usr/src

# Menjalankan program saat container dijalankan
CMD ["./ayamgoreng"]