# Project_Kelompok_5

> **project praktikum alpro 2026**

---

## Sebelum Mulai

Pastikan kamu sudah:
- ✅ Menginstal **Git** → https://git-scm.com/downloads
- ✅ Memiliki akun **GitHub** → https://github.com
- ✅ Mengonfigurasi **SSH key** ke akun GitHub kamu

---

## Clone Repository (Lakukan Sekali Saja)

Buka terminal / cmd, lalu jalankan:

```bash
git clone git@github.com:IzzaMaulana/Project-Alpro-Kelompok-5.git
cd Project-Alpro-Kelompok-5
```

---

## Membuat Branch Sendiri (Lakukan Sekali Saja)

> Jangan pernah kerja langsung di `main`. Selalu buat branch sendiri.

```bash
git switch -c nama_branch
```

Verifikasi: branch aktif ditandai `*`:

```bash
git branch
```

Output:

```
* nama_branch
  main
```

---

## Workflow Harian

Lakukan langkah ini **setiap kali mau mulai kerja**.

### Langkah 1: Ambil Update Terbaru dari Main

Sebelum kerja, selalu sinkronkan branch kamu dengan `main` agar tidak tertinggal:

```bash
git switch main
git pull origin main
git switch nama_branch
git merge main
```

Jika muncul conflict setelah merge, lihat bagian [Mengatasi Conflict](#️-mengatasi-conflict).

---

### Langkah 2: Kerjakan Tugas

Edit atau tambahkan file sesuai bagian yang kamu kerjakan.

---

### Langkah 3: Cek Perubahan

```bash
git status
```

Pastikan hanya file yang kamu ubah yang muncul. Jika ada file yang tidak sengaja ikut, jangan di-add.

---

### Langkah 4: Tambahkan File ke Staging

Semua file sekaligus:

```bash
git add .
```

Atau file tertentu saja:

```bash
git add nama_file.cpp
```

---

### Langkah 5: Commit Perubahan

```bash
git commit -m "tipe: deskripsi singkat perubahan"
```

#### Format Tipe Commit

| Tipe | Kapan Dipakai |
|------|---------------|
| `feat` | Menambah fitur baru |
| `fix` | Memperbaiki bug |
| `docs` | Mengubah dokumentasi |
| `refactor` | Merapikan kode tanpa ubah fungsi |
| `chore` | Hal-hal kecil lainnya |

Contoh:

```bash
git commit -m "feat: menambahkan fungsi hitung total"
git commit -m "fix: memperbaiki validasi input kosong"
git commit -m "docs: memperbarui README"
git commit -m "refactor: merapikan kode"
```

---

### Langkah 6: Push ke GitHub

Push pertama kali dari branch ini (hanya sekali):

```bash
git push -u origin nama_branch
```

Push berikutnya cukup:

```bash
git push
```

---

### Langkah 7: Buat Pull Request

Setelah push, minta perubahan kamu digabungkan ke `main` lewat Pull Request:

1. Buka repository di GitHub: https://github.com/IzzaMaulana/Project-Alpro-Kelompok-5
2. Klik tab **Pull Requests**
3. Klik **New Pull Request**
4. Pilih branch kamu sebagai **compare**, `main` sebagai **base**
5. Klik **Create Pull Request**
6. Isi judul dan deskripsi singkat apa yang kamu kerjakan
7. Klik **Create Pull Request**

---

## Mengatasi Conflict

Conflict terjadi ketika dua orang mengubah baris yang sama pada file yang sama.

### 1. Cek file yang conflict

```bash
git status
```

File yang conflict ditandai `both modified`.

### 2. Buka file tersebut

Git menandai bagian yang conflict seperti ini:

```
<<<<<<< HEAD
kode milik kamu
=======
kode dari main / anggota lain
>>>>>>> main
```

### 3. Edit: pilih atau gabungkan kode yang benar

Hapus penanda `<<<<<<<`, `=======`, dan `>>>>>>>`, lalu simpan file.

### 4. Selesaikan dan push

```bash
git add .
git commit -m "fix: resolve conflict"
git push
```

> Conflict bisa dihindari dengan selalu `pull` dari `main` sebelum mulai kerja.

---

## Checklist Sebelum Push

- [ ] Sudah `pull` dari `main` sebelum mulai kerja
- [ ] Berada di branch yang benar, bukan `main`
- [ ] Sudah `git status`, tidak ada file yang tidak sengaja ikut
- [ ] Sudah `git add` file yang diperlukan
- [ ] Pesan commit jelas dan menggunakan format yang benar
- [ ] Kode bisa di-compile / tidak ada error besar
- [ ] Sudah `git push`
- [ ] Sudah buat Pull Request

---

## Referensi Command

```bash
git status                        # cek status file
git branch                        # cek branch aktif
git switch nama_branch            # pindah ke branch
git switch -c nama_branch         # buat branch baru sekaligus pindah
git add .                         # staging semua file
git add nama_file                 # staging file tertentu
git commit -m "pesan"             # commit
git push                          # push (setelah push pertama)
git push -u origin nama_branch    # push pertama kali
git pull origin main              # ambil update dari main
git merge main                    # gabungkan main ke branch aktif
```

---
