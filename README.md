# 📝 LifeLog v1.0
> Aplikasi jurnal harian berbasis CLI yang dibuat sebagai proyek akhir mata kuliah Algoritma dan Pemrograman.

LifeLog adalah program terminal untuk mencatat jurnal harian, melacak mood dan produktivitas, serta melihat pola perkembangan diri dari waktu ke waktu. Semua data tersimpan secara lokal, tidak ada koneksi internet, tidak ada akun cloud. Cukup tulis, simpan, dan baca kembali kapan saja.

---

## 💻 Fitur

### Jurnal Harian
Ini adalah inti dari aplikasi. Kamu bisa menulis entri baru, membaca semua jurnal yang sudah tersimpan, atau mencarinya berdasarkan berbagai kriteria:

- **Tulis jurnal** - Sistem otomatis mencatat nama pengguna dan tanggal hari ini. Kamu tinggal isi skala mood (1-10), skala produktivitas (1-10), lalu tulis catatan bebas tentang hari kamu.
- **Lihat semua jurnal** - Menampilkan seluruh riwayat entri dari yang pertama sampai terbaru.
- **Cari jurnal** - Bisa dicari berdasarkan tanggal (format YYYY-MM-DD), kategori mood (buruk <= 3, biasa 4-6, bagus >= 7), tingkat produktivitas, atau kata kunci dari isi catatan.
- **Hapus jurnal** - Pilih nomor urut entri yang ingin dihapus, lalu konfirmasi. Penghapusan bersifat permanen.

### Statistik
Semacam dashboard pribadi. Di sini kamu bisa lihat

- Ringkasan keseluruhan: total entri, rata-rata mood dan produktivitas, tanggal mood tertinggi dan terendah.
- Statistik mingguan dan bulanan dalam bentuk tabel dan grafik batang horizontal sederhana menggunakan karakter `[#####---]`.

### Achievement
Sistem gamifikasi kecil-kecilan supaya nulis jurnal terasa lebih menyenangkan. Ada beberapa badge yang bisa di-unlock berdasarkan konsistensi, mulai dari *First Entry* sampai *One Year Streak*. Menu ini juga menampilkan rekor streak tertinggi yang pernah kamu capai.

### Profil Akun
Tampilan ringkasan seperti halaman profil di game. Berisi username, total jurnal, jumlah achievement, rekor streak, dan status rank yang naik seiring banyaknya entri yang kamu tulis (misalnya *Novice Diarist*, *Journeyman Writer*, *Senior Chronicler*).

---

## 🦎 Cara Menjalankan

Pastikan kamu sudah punya compiler C++ (GCC/G++) yang terinstall di sistem.

**1. Compile proyek**

Masuk ke direktori utama `LifeLog`, lalu jalankan:

```bash
g++ src/*.cpp -o main
```

**2. Jalankan program**

```bash
./main
```

---
