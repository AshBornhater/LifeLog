# 📝 LifeLog v1.0 - Aplikasi Jurnal Harian Digital

**LifeLog v1.0** adalah aplikasi jurnal harian berbasis teks (CLI) yang dirancang untuk membantu Anda merekam, melacak, dan mengevaluasi keseharian Anda dengan mudah. Aplikasi ini memfokuskan pencatatan pada tingkat kebahagiaan (*mood*) dan produktivitas harian Anda agar Anda bisa memahami pola hidup serta perkembangan emosional Anda dari waktu ke waktu.

---

## ✨ Penjelasan Detail Fitur & Menu

Aplikasi ini dilengkapi dengan menu interaktif yang kaya akan informasi untuk menemani perjalanan menulis jurnal Anda:

### 1. Modul Jurnal Harian (`jurnalHarian`)
Menu ini adalah pusat pengelolaan catatan harian Anda, yang terbagi menjadi beberapa sub-menu:
* **Tulis Jurnal**: Fitur utama untuk merekam hari Anda. Sistem secara otomatis mencatat nama pengguna aktif dan tanggal hari ini. Anda akan dipandu untuk memasukkan angka skala 1-10 untuk *Mood* (suasana hati) dan *Productivity* (tingkat kefokusan kerja), lalu menuliskan cerita pendek atau catatan mengenai apa yang terjadi.
* **Lihat Semua Jurnal**: Menampilkan seluruh riwayat jurnal yang pernah Anda tulis dari awal hingga akhir secara berurutan, sehingga Anda bisa bernostalgia membaca momen-momen masa lalu.
* **Cari Jurnal**: Menu pencarian pintar yang memungkinkan Anda menyaring jurnal berdasarkan kriteria spesifik:
  * *Berdasarkan Tanggal*: Mencari jurnal pada hari tertentu secara presisi (Format: YYYY-MM-DD).
  * *Berdasarkan Mood*: Mengelompokkan pencarian ke dalam 3 kategori emosi (Mood Buruk $\le 3$, Mood Biasa 4-6, atau Mood Bagus $\ge 7$).
  * *Cari Produktivitas*: Menampilkan hari-hari di mana Anda mencapai tingkat produktivitas tertentu (Skala 1-10).
  * *Berdasarkan Kata*: Mencari jurnal berdasarkan potongan kata atau kalimat yang pernah Anda tulis di catatan (pencarian ini sudah mendukung huruf besar/kecil otomatis).
* **Hapus Jurnal**: Jika ada kesalahan catat, menu ini memandu Anda memilih nomor urut jurnal tertentu untuk dihapus secara permanen dari penyimpanan.

### 2. Modul Statistik (`statistik`)
Menu ini bertindak sebagai papan instrumen (*dashboard*) analisis data pribadi Anda:
* **Ringkasan Keseluruhan**: Menampilkan total entri jurnal Anda, nilai rata-rata mood dan produktivitas sepanjang waktu, serta mencantumkan tanggal persis di mana Anda merasa paling bahagia (Mood Tertinggi) atau paling terpuruk (Mood Terendah).
* **Statistik Mingguan & Bulanan**: Menyajikan data rata-rata dalam bentuk **Tabel berkala** dan **Grafik Batang Horizontal Visual** menggunakan karakter `[#####---]`. Fitur ini sangat berguna untuk melihat apakah bulan atau minggu ini Anda menjadi pribadi yang lebih produktif dan bahagia dibanding waktu sebelumnya.

### 3. Modul Achievement / Sistem Pencapaian (`achievement`)
Sebuah sistem gamifikasi yang dirancang untuk memotivasi Anda agar rutin menulis jurnal.
* **Cek Achievement Baru**: Sistem akan menghitung seluruh riwayat jurnal Anda dan memeriksa apakah Anda berhak mendapatkan medali penghargaan baru. Di menu ini, sistem juga akan melakukan *spill* total rekor *streak* (jumlah hari berturut-turut tanpa bolong) tertinggi yang pernah Anda capai.
* **Lihat Semua Achievement**: Menampilkan lemari pajangan berisi badge medali apa saja yang sudah berhasil Anda kunci (*unlocked*), seperti *First Entry* (Jurnal Pertama), *7-Day Streak*, *30-Day Streak*, hingga medali legendaris *One Year Streak* (1 Tahun Berturut-turut).

### 4. Profil Akun (`profilAkun`)
Menu ini menyajikan ringkasan eksklusif layaknya *gaming profile*. Di dalamnya termuat informasi nama petualang (Username ID Anda), jumlah total jurnal tersimpan, jumlah medali yang diraih, rekor *streak* tertinggi, serta **Status Rank khusus** (seperti *Senior Chronicler*, *Journeyman Writer*, atau *Novice Diarist*) yang otomatis meningkat seiring banyaknya kontribusi tulisan Anda.

---

## 🚀 Cara Menjalankan Aplikasi

Aplikasi ini dapat dijalankan dengan mudah melalui Terminal atau Command Prompt (CMD) menggunakan kompiler C++ (seperti GCC/G++).

### 1. Kompilasi (Compile) Proyek
Buka terminal, masuk ke direktori utama proyek `LifeLog`, lalu jalankan perintah berikut untuk menyatukan seluruh file kode menjadi satu program utuh:
```bash
g++ src/*.cpp -o main
