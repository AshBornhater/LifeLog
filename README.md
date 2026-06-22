# 📝 LifeLog v1.0

<p align="center">
  <img src="https://img.shields.io/badge/Language-C%2B%2B-blue?style=for-the-badge" alt="C++">
  <img src="https://img.shields.io/badge/Interface-Terminal_UI-green?style=for-the-badge" alt="Terminal UI">
  <img src="https://img.shields.io/badge/Application-CLI-orange?style=for-the-badge" alt="CLI">
</p>

<p align="center">
  <b>LifeLog</b> adalah aplikasi jurnal harian berbasis terminal yang membantu pengguna mencatat aktivitas, mood, dan produktivitas setiap hari secara sederhana dan interaktif.
</p>

---

## 📖 Tentang Proyek

LifeLog merupakan aplikasi **Command Line Interface (CLI)** yang dikembangkan menggunakan bahasa **C++** sebagai proyek akhir mata kuliah **Algoritma dan Pemrograman**.

Aplikasi ini dirancang untuk membantu pengguna membangun kebiasaan refleksi diri melalui jurnal harian. Setiap entri yang ditulis akan disimpan secara lokal sehingga pengguna dapat melihat kembali perjalanan, perkembangan mood, tingkat produktivitas, serta pola aktivitas yang terbentuk dari waktu ke waktu.

Selain berfungsi sebagai tempat menyimpan catatan harian, LifeLog juga dilengkapi dengan sistem statistik dan achievement yang memberikan gambaran perkembangan pengguna secara lebih menarik.

Karena seluruh data disimpan secara lokal, aplikasi ini tidak membutuhkan koneksi internet maupun layanan cloud.

---

## Fitur Utama

### 🔐 Sistem Login & Register

Sebelum menggunakan aplikasi, pengguna harus memiliki akun terlebih dahulu.

#### 📝 Register

Pengguna dapat membuat akun baru dengan memasukkan:

- Username
- Password

Sistem akan memeriksa apakah username sudah digunakan sebelumnya. Jika username tersedia, akun akan disimpan ke dalam database lokal dan siap digunakan untuk login.

#### 🔑 Login

Pengguna yang sudah memiliki akun dapat masuk menggunakan username dan password yang terdaftar.

Fitur login memungkinkan:

- Identifikasi pengguna secara personal
- Pemisahan data jurnal antar pengguna
- Penyimpanan riwayat jurnal yang aman dan terorganisir
- Akses ke statistik, achievement, dan profil masing-masing pengguna

Untuk meningkatkan keamanan data, password tidak disimpan dalam bentuk teks asli (*plain text*). Sebelum ditulis ke file penyimpanan, password akan dienkripsi menggunakan algoritma enkripsi sederhana yang memanfaatkan **key khusus** yang telah ditentukan dalam program.

Dengan mekanisme ini, password asli pengguna tidak dapat langsung dibaca ketika file akun dibuka secara manual melalui text editor.

Contoh:

Password yang dimasukkan pengguna:

```text
mypassword123
```

Data yang tersimpan pada file:

```text
~J#Kq!9P@2xL...
```

#### 👥 Multi User Support

LifeLog mendukung banyak pengguna dalam satu aplikasi.

Setiap akun memiliki:

- Riwayat jurnal sendiri
- Statistik pribadi
- Achievement pribadi
- Rekor streak masing-masing

Ini memungkinkan beberapa pengguna dapat menggunakan aplikasi yang sama tanpa data saling bercampur.

---

### 📔 Jurnal Harian

Fitur utama yang digunakan untuk mencatat aktivitas dan pengalaman harian pengguna.

#### Tulis Jurnal

Saat membuat jurnal baru, sistem akan secara otomatis:

* Mengambil username pengguna yang sedang login
* Mengambil tanggal saat ini dari sistem
* Meminta pengguna mengisi:

  * Mood (1–10)
  * Produktivitas (1–10)
  * Catatan harian

Data yang dimasukkan kemudian disimpan ke file lokal sehingga dapat diakses kembali kapan saja.

#### 📚 Lihat Semua Jurnal

Menampilkan seluruh jurnal yang pernah dibuat oleh pengguna.

Informasi yang ditampilkan meliputi:

* Tanggal
* Nilai mood
* Nilai produktivitas
* Isi catatan

#### 🔍 Cari Jurnal

LifeLog menyediakan beberapa metode pencarian jurnal:

* Berdasarkan tanggal
* Berdasarkan kategori mood
* Berdasarkan tingkat produktivitas
* Berdasarkan kata kunci pada catatan

Kategori mood yang tersedia:

| Kategori   | Rentang |
| ---------- | ------- |
| Mood Buruk | ≤ 3     |
| Mood Biasa | 4 – 6   |
| Mood Bagus | ≥ 7     |

#### 🗑️ Hapus Jurnal

Pengguna dapat menghapus jurnal berdasarkan nomor urut entri yang dipilih.

> Penghapusan bersifat permanen dan tidak dapat dibatalkan.

---

## 📊 Statistik

LifeLog menyediakan fitur statistik untuk membantu pengguna memahami pola aktivitas dan perkembangan dirinya.

### 📈 Ringkasan Keseluruhan

Informasi yang ditampilkan:

* Total jurnal yang tersimpan
* Rata-rata mood
* Rata-rata produktivitas
* Mood tertinggi dan terendah
* Produktivitas tertinggi dan terendah

### 📅 Statistik Mingguan

Data jurnal dikelompokkan berdasarkan minggu sehingga pengguna dapat melihat perkembangan dalam jangka pendek.

Statistik yang ditampilkan:

* Jumlah jurnal per minggu
* Rata-rata mood mingguan
* Rata-rata produktivitas mingguan
* Grafik batang horizontal berbasis karakter terminal

Contoh visualisasi:

```text
2026-W25  [###############-----] 7.20
2026-W26  [##################--] 8.50
```

### 🗓️ Statistik Bulanan

Menampilkan analisis perkembangan berdasarkan bulan.

Fitur ini membantu pengguna melihat:

* Perubahan mood dari bulan ke bulan
* Perubahan tingkat produktivitas
* Tren perkembangan diri dalam jangka panjang

---

## 🏆 Achievement System

Untuk meningkatkan motivasi pengguna dalam menulis jurnal secara konsisten, LifeLog menyediakan sistem achievement yang dapat diperoleh berdasarkan pencapaian tertentu.

Achievement yang tersedia:

| Achievement     | Deskripsi                                     |
| --------------- | --------------------------------------------- |
| First Entry     | Menulis jurnal pertama                        |
| 7-Day Streak    | Menulis jurnal selama 7 hari berturut-turut   |
| 30-Day Streak   | Menulis jurnal selama 30 hari berturut-turut  |
| One Year Streak | Menulis jurnal selama 365 hari berturut-turut |

Sistem akan secara otomatis memeriksa achievement yang berhasil diperoleh pengguna.

---

## 👤 Profil Pengguna

Menu profil menampilkan ringkasan perkembangan akun pengguna.

Informasi yang ditampilkan:

* Username aktif
* Total jurnal yang ditulis
* Jumlah achievement yang diperoleh
* Rekor streak tertinggi
* Status rank pengguna

### 🎖️ Sistem Rank

Semakin banyak jurnal yang ditulis, semakin tinggi rank yang diperoleh.

| Rank              | Syarat       |
| ----------------- | ------------ |
| Novice Diarist    | < 10 jurnal  |
| Journeyman Writer | ≥ 10 jurnal  |
| Senior Chronicler | ≥ 50 jurnal  |
| Master Scribe     | ≥ 100 jurnal |
| Mythic Archivist  | ≥ 300 jurnal |

---

## 🎨 Terminal User Interface

LifeLog menggunakan antarmuka terminal interaktif yang memanfaatkan:

* ANSI Escape Sequence
* Pewarnaan teks terminal
* Navigasi menggunakan tombol panah
* Border dan panel ASCII
* Posisi kursor dinamis
* ASCII Art Logo

Hal ini membuat pengalaman penggunaan lebih menarik dibandingkan aplikasi CLI biasa.

---

## 💾 Penyimpanan Data

Seluruh data disimpan secara lokal menggunakan file teks.

```text
data/
├── userAccounts.txt
├── userData.txt
└── achievements.txt
```

Format penyimpanan menggunakan delimiter:

```text
<|>
```

Contoh data jurnal:

```text
iqul<|>2026-06-20<|>8<|>9<|>Hari ini berhasil menyelesaikan tugas besar.
```

---

## 🛠️ Teknologi, Library, dan Konsep yang Digunakan

### Bahasa Pemrograman

- C++

### Standard Library

#### `<iostream>`
Digunakan untuk:
- Input pengguna (`cin`)
- Output terminal (`cout`)
- Pembuatan antarmuka berbasis terminal

#### `<string>`
Digunakan untuk:
- Penyimpanan username dan password
- Pengolahan isi jurnal
- Manipulasi teks dan pencarian data

#### `<fstream>`
Digunakan untuk:
- Membaca data akun
- Menyimpan jurnal pengguna
- Menyimpan data achievement
- Pengelolaan database berbasis file `.txt`

#### `<ctime>`
Digunakan untuk:
- Mengambil tanggal sistem secara otomatis
- Menentukan tanggal pembuatan jurnal
- Perhitungan streak dan statistik berbasis waktu

### Non-Standard Library

#### `<conio.h>`
Digunakan untuk:
- Membaca input keyboard tanpa menunggu tombol Enter
- Navigasi menu menggunakan tombol panah
- Pembuatan antarmuka terminal yang lebih interaktif

### Konsep Pemrograman yang Diterapkan

- Modular Programming
- Struct
- Pointer
- Dynamic Memory Allocation (`new` dan `delete`)
- Dynamic Array
- File Handling
- Data Parsing
- Searching
- Sorting
- Authentication System
- Password Encryption dengan Custom Key
- Multi User Support
- Terminal User Interface (TUI)
- ANSI Escape Sequence
---

## 🦎 Cara Menjalankan

### 1. Clone Repository

```bash
git clone https://github.com/AshBornhater/LifeLog.git
```

### 2. Masuk ke Direktori Proyek

```bash
cd LifeLog
```

### 3. Compile Program

```bash
g++ src/*.cpp -o main
```

### 4. Jalankan Program

**Linux / macOS**

```bash
./main
```

**Windows**

```powershell
main.exe
```

---

## 🎯 Tujuan Pengembangan

Proyek ini dibuat untuk menerapkan berbagai konsep yang dipelajari pada mata kuliah **Algoritma dan Pemrograman**, di antaranya:

* PROGRAMMING BASICS
* CONTROL STATEMENTS AND LOOPING
* ARRAY
* FUNCTION
* STRUCT
* POINTER
---

## 👨‍💻 Developers

**LifeLog v1.0**

| Nama                            | NIM         |
| ------------------------------- | ----------- |
| Vidia Iqmalia Putri             | F1D02510029 |
| Lale Juliesti Ayuningkusuma     | F1D02510064 |
| Lalu Muhammad Izza Abda Maulana | F1D02510067 |
| Muhammad Bijak Rizqullah        | F1D02510081 |
| Zidny Ilma                      | F1D02510100 |
| Andika Ramdhanu                 | F1D02510104 |
| Nurlaeli Hidayati               | F1D02510129 |

---
