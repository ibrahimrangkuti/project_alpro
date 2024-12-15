#include <iostream>
#include <string>
#include <fstream>
#include <iomanip>

using namespace std;

struct Fasilitas {
    string nama;
    string status;
    string tanggal_peminjaman;
    string nama_peminjam;
    string nim;
};

const int MAX_FASILITAS = 100;
Fasilitas daftarFasilitas[MAX_FASILITAS];
int jumlahFasilitas = 0;
string adminUsername, adminPassword;

// Function prototypes
bool loginAdmin();
void loadAdminData();
void loadFasilitasData();
void saveFasilitasData();
void tampilFasilitas();
void tambahFasilitas();
void hapusFasilitas();
void pinjamFasilitas();
void pengembalianFasilitas();
int cariFasilitas(const string& nama);

int main() {
    loadAdminData();
    loadFasilitasData();

    if (!loginAdmin()) {
        cout << "Login gagal. Program ditutup.\n";
        return 0;
    }

    int pilihan;
    do {
        cout << "\n===== Sistem Peminjaman Fasilitas Kampus =====\n";
        cout << "1. Tampil Fasilitas\n";
        cout << "2. Tambah Fasilitas\n";
        cout << "3. Hapus Fasilitas\n";
        cout << "4. Pinjam Fasilitas\n";
        cout << "5. Pengembalian Fasilitas\n";
        cout << "6. Keluar\n";
        cout << "Pilihan: ";
        cin >> pilihan;
        cin.ignore(); // Clear newline

        switch (pilihan) {
            case 1:
                tampilFasilitas();
                break;
            case 2:
                tambahFasilitas();
                break;
            case 3:
                hapusFasilitas();
                break;
            case 4:
                pinjamFasilitas();
                break;
            case 5:
                pengembalianFasilitas();
                break;
            case 6:
                saveFasilitasData();
                cout << "Keluar dari program.\n";
                break;
            default:
                cout << "Pilihan tidak valid.\n";
        }
    } while (pilihan != 6);

    return 0;
}

void loadAdminData() {
    ifstream file("admin.txt");
    if (file.is_open()) {
        getline(file, adminUsername);
        getline(file, adminPassword);
        file.close();
    } else {
        cout << "File admin.txt tidak ditemukan. Pastikan file tersedia.\n";
        exit(1);
    }
}

void loadFasilitasData() {
    ifstream file("fasilitas.txt");
    if (file.is_open()) {
        jumlahFasilitas = 0;
        while (file && jumlahFasilitas < MAX_FASILITAS) {
            getline(file, daftarFasilitas[jumlahFasilitas].nama);
            if (daftarFasilitas[jumlahFasilitas].nama.empty()) break;
            getline(file, daftarFasilitas[jumlahFasilitas].status);
            getline(file, daftarFasilitas[jumlahFasilitas].tanggal_peminjaman);
            getline(file, daftarFasilitas[jumlahFasilitas].nama_peminjam);
            getline(file, daftarFasilitas[jumlahFasilitas].nim);
            jumlahFasilitas++;
        }
        file.close();
    } else {
        cout << "File fasilitas.txt tidak ditemukan. Memulai dengan data kosong.\n";
    }
}

void saveFasilitasData() {
    ofstream file("fasilitas.txt");
    if (file.is_open()) {
        for (int i = 0; i < jumlahFasilitas; i++) {
            file << daftarFasilitas[i].nama << "\n"
                 << daftarFasilitas[i].status << "\n"
                 << daftarFasilitas[i].tanggal_peminjaman << "\n"
                 << daftarFasilitas[i].nama_peminjam << "\n"
                 << daftarFasilitas[i].nim << "\n";
        }
        file.close();
    } else {
        cout << "Gagal menyimpan data fasilitas ke file.\n";
    }
}

bool loginAdmin() {
    string username, password;
    cout << "===== Login Admin =====\n";
    cout << "Username: ";
    cin >> username;
    cout << "Password: ";
    cin >> password;

    return username == adminUsername && password == adminPassword;
}

void tampilFasilitas() {
    if (jumlahFasilitas == 0) {
        cout << "Tidak ada fasilitas yang tersedia.\n";
        return;
    }

    cout << left << setw(20) << "Nama Fasilitas" << setw(15) << "Status" 
         << setw(20) << "Tanggal Peminjaman" << setw(20) << "Nama Peminjam" 
         << setw(10) << "NIM" << endl;
    for (int i = 0; i < jumlahFasilitas; i++) {
        cout << left << setw(20) << daftarFasilitas[i].nama 
             << setw(15) << daftarFasilitas[i].status 
             << setw(20) << daftarFasilitas[i].tanggal_peminjaman 
             << setw(20) << daftarFasilitas[i].nama_peminjam 
             << setw(10) << daftarFasilitas[i].nim << endl;
    }
}

void tambahFasilitas() {
    if (jumlahFasilitas >= MAX_FASILITAS) {
        cout << "Kapasitas fasilitas penuh. Tidak dapat menambah fasilitas baru.\n";
        return;
    }

    Fasilitas fasilitas;
    cin.ignore();
    cout << "Masukkan nama fasilitas: ";
    getline(cin, fasilitas.nama);
    fasilitas.status = "Tersedia";
    fasilitas.tanggal_peminjaman = "-";
    fasilitas.nama_peminjam = "-";
    fasilitas.nim = "-";
    daftarFasilitas[jumlahFasilitas++] = fasilitas;
    cout << "Fasilitas berhasil ditambahkan.\n";
}

void hapusFasilitas() {
    string nama;
    cout << "Masukkan nama fasilitas yang ingin dihapus: ";
    cin.ignore();
    getline(cin, nama);

    int index = cariFasilitas(nama);
    if (index != -1) {
        for (int i = index; i < jumlahFasilitas - 1; i++) {
            daftarFasilitas[i] = daftarFasilitas[i + 1];
        }
        jumlahFasilitas--;
        cout << "Fasilitas berhasil dihapus.\n";
    } else {
        cout << "Fasilitas tidak ditemukan.\n";
    }
}

void pinjamFasilitas() {
    string nama, nama_peminjam, nim, tanggal;
    cout << "Masukkan nama fasilitas yang ingin dipinjam: ";
    cin.ignore();
    getline(cin, nama);

    int index = cariFasilitas(nama);
    if (index != -1) {
        if (daftarFasilitas[index].status == "Tersedia") {
            cout << "Masukkan nama peminjam: ";
            getline(cin, nama_peminjam);
            cout << "Masukkan NIM: ";
            getline(cin, nim);
            cout << "Masukkan tanggal peminjaman (DD-MM-YYYY): ";
            getline(cin, tanggal);

            daftarFasilitas[index].status = "Dipinjam";
            daftarFasilitas[index].nama_peminjam = nama_peminjam;
            daftarFasilitas[index].nim = nim;
            daftarFasilitas[index].tanggal_peminjaman = tanggal;

            cout << "Fasilitas berhasil dipinjam.\n";
        } else {
            cout << "Fasilitas sedang dipinjam.\n";
        }
    } else {
        cout << "Fasilitas tidak ditemukan.\n";
    }
}

void pengembalianFasilitas() {
    string nama;
    cout << "Masukkan nama fasilitas yang ingin dikembalikan: ";
    cin.ignore();
    getline(cin, nama);

    int index = cariFasilitas(nama);
    if (index != -1) {
        if (daftarFasilitas[index].status == "Dipinjam") {
            daftarFasilitas[index].status = "Tersedia";
            daftarFasilitas[index].nama_peminjam = "-";
            daftarFasilitas[index].nim = "-";
            daftarFasilitas[index].tanggal_peminjaman = "-";
            cout << "Fasilitas berhasil dikembalikan.\n";
        } else {
            cout << "Fasilitas tidak sedang dipinjam.\n";
        }
    } else {
        cout << "Fasilitas tidak ditemukan.\n";
    }
}

int cariFasilitas(const string& nama) {
    for (int i = 0; i < jumlahFasilitas; i++) {
        if (daftarFasilitas[i].nama == nama) {
            return i;
        }
    }
    return -1;
}

