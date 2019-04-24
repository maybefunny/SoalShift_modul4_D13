# Laporan Penjelasan Soal Shift Modul 4

## Contents :
- [soal 1](#soal_1)
- [soal 2](#soal_2)
- [soal 3](#soal_3)
- [soal 4](#soal_4)
- [soal 5](#soal_5)

## soal 1 <a name="soal_1"></a>
Semua nama file dan folder harus terenkripsi
Enkripsi yang Atta inginkan sangat sederhana, yaitu Caesar cipher. Namun, Kusuma mengatakan enkripsi tersebut sangat mudah dipecahkan. Dia menyarankan untuk character list diekspansi,tidak hanya alfabet, dan diacak. Berikut character list yang dipakai:

    qE1~ YMUR2"`hNIdPzi%^t@(Ao:=CQ,nx4S[7mHFye#aT6+v)DfKL$r?bkOGB>}!9_wV']jcp5JZ&Xl|\8s;g<{3.u*W-0

Misalkan ada file bernama “halo” di dalam folder “INI_FOLDER”, dan key yang dipakai adalah 17, maka:
“INI_FOLDER/halo” saat belum di-mount maka akan bernama “n,nsbZ]wio/QBE#”, saat telah di-mount maka akan otomatis terdekripsi kembali menjadi “INI_FOLDER/halo” .
Perhatian: Karakter ‘/’ adalah karakter ilegal dalam penamaan file atau folder dalam *NIX, maka dari itu dapat diabaikan


### Jawab :
- buat fungsi dekripsi dan enkripsi caesar chiper dengan alfabet dan key yang telah ditentukan.
- atur fungsi readdir sesuai permintaan soal.

### Langkah :

- siapkan hash table untuk setiap karakter.
- buat fungsi untuk mentranslate kata (dekripsi dan enkripsi).
- lakukan dekripsi ketika akan menampilkan nama file pada fungsi readdir

## soal 2 <a name="soal_2"></a>
Semua file video yang tersimpan secara terpecah-pecah (splitted) harus secara otomatis tergabung (joined) dan diletakkan dalam folder “Videos”
Urutan operasi dari kebutuhan ini adalah:
- Tepat saat sebelum file system di-mount
    - Secara otomatis folder “Videos” terbuat di root directory file system
    - Misal ada sekumpulan file pecahan video bernama “computer.mkv.000”, “computer.mkv.001”, “computer.mkv.002”, dst. Maka secara otomatis file pecahan tersebut akan di-join menjadi file video “computer.mkv”
    - Untuk mempermudah kalian, dipastikan hanya video file saja yang terpecah menjadi beberapa file.
    - Karena mungkin file video sangat banyak sehingga mungkin saja saat menggabungkan file video, file system akan membutuhkan waktu yang lama untuk sukses ter-mount. Maka pastikan saat akan menggabungkan file pecahan video, file system akan membuat 1 thread/proses(fork) baru yang dikhususkan untuk menggabungkan file video tersebut
    - Pindahkan seluruh file video yang sudah ter-join ke dalam folder “Videos”
    - Jangan tampilkan file pecahan di direktori manapun
- Tepat saat file system akan di-unmount
    - Hapus semua file video yang berada di folder “Videos”, tapi jangan hapus file pecahan yang terdapat di root directory file system
    - Hapus folder “Videos” 


### Jawab :
atur fungsi init, readdir dan destroy fuse sesuai permintaan soal.

### Langkah :

- init :
    - buat folder Videos.
    - cari file dengan ekstensi `.000`
    - buat thread untuk men-join-kan file tersebut
- readdir :
    - cek ekstensi file, jika merupakan partisi file maka jangan tampilkan
- destroy :
    - hapus folder Videos secara rekursif.

## soal 3 <a name="soal_3"></a>
Sebelum diterapkannya file system ini, Atta pernah diserang oleh hacker LAPTOP_RUSAK yang menanamkan user bernama “chipset” dan “ic_controller” serta group “rusak” yang tidak bisa dihapus. Karena paranoid, Atta menerapkan aturan pada file system ini untuk menghapus “file bahaya” yang memiliki spesifikasi:
- Owner Name 	: ‘chipset’ atau ‘ic_controller’
- Group Name	: ‘rusak’
- Tidak dapat dibaca

Jika ditemukan file dengan spesifikasi tersebut ketika membuka direktori, Atta akan menyimpan nama file, group ID, owner ID, dan waktu terakhir diakses dalam file “filemiris.txt” (format waktu bebas, namun harus memiliki jam menit detik dan tanggal) lalu menghapus “file bahaya” tersebut untuk mencegah serangan lanjutan dari LAPTOP_RUSAK.


### Jawab :
ubah fungsi readdir sesuai permintaan soal.

### Langkah :
- cek jenis, owner dan group dari tiap file.
- jika memenuhi syarat maka hapus file tersebut.

## soal 4 <a name="soal_4"></a>
Pada folder YOUTUBER, setiap membuat folder permission foldernya akan otomatis menjadi 750. Juga ketika membuat file permissionnya akan otomatis menjadi 640 dan ekstensi filenya akan bertambah “.iz1”. File berekstensi “.iz1” tidak bisa diubah permissionnya dan memunculkan error bertuliskan “File ekstensi iz1 tidak boleh diubah permissionnya.”

### Jawab :
atur fungsi chown, mkdir, dan create sesuai permintaan soal.

### Langkah :
- create & mkdir :
    - cek nama direktori induk.
    - jika memenuhi syarat soal, maka berlakukan peraturan soal.
- chown :
    - cek ekstensi file
    - jika memenuhi syarat soal, maka berlakukan peraturan soal.

## soal 5 <a name="soal_5"></a>
Ketika mengedit suatu file dan melakukan save, maka akan terbuat folder baru bernama Backup kemudian hasil dari save tersebut akan disimpan pada backup dengan nama namafile_[timestamp].ekstensi. Dan ketika file asli dihapus, maka akan dibuat folder bernama RecycleBin, kemudian file yang dihapus beserta semua backup dari file yang dihapus tersebut (jika ada) di zip dengan nama namafile_deleted_[timestamp].zip dan ditaruh kedalam folder RecycleBin (file asli dan backup terhapus). Dengan format [timestamp] adalah yyyy-MM-dd_HH:mm:ss

### Jawab :
atur fungsi unlink dan write sesuai keinginan soal.

### Langkah :
- write :
    - buat folder `Backup` jika belum ada.
    - simpan salinan perubahan berkas ke dalam folder `Backup`.
    - simpan perubahan berkas.
- unlink :
    - buat folder `RecycleBin` jika belum ada.
    - masukkan file serta semua file backup-an dari file ke dalam zip pada folder `RecycleBin`.
    - hapus file.