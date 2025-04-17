#include <iostream>
#include <windows.h>    // Dieu khien console
#include <conio.h>      // _kbhit(), _getch()
#include <ctime>        // time()
#include <cstdlib>      // rand(), srand()
#include <fstream>      // Doc/ghi file

using namespace std;

#define MAX_LENGTH 1000  // độ dài tối đa của rắn
const char HUONG_LEN = 'U';   // ký hiệu hướng
const char HUONG_XUONG = 'D';
const char HUONG_TRAI = 'L';
const char HUONG_PHAI = 'R';

int consoleWidth, consoleHeight; // lưu kích thước màn hình console

void layKichThuocManHinh() {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(hConsole, &csbi);
    consoleHeight = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
    consoleWidth = csbi.srWindow.Right - csbi.srWindow.Left + 1;
}

void diChuyenConTro(int x, int y) {
    COORD pos = { (SHORT)x, (SHORT)y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

void doiMau(int color) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

void inCanGiua(string text, int y) {
    int x = (consoleWidth - text.length()) / 2;
    diChuyenConTro(x, y);
    cout << text;
}

void veKhungMap() {
    for (int x = 0; x < consoleWidth; x++) {
        diChuyenConTro(x, 1); cout << "=";
        diChuyenConTro(x, consoleHeight - 1); cout << "=";
    }
    for (int y = 1; y < consoleHeight; y++) {
        diChuyenConTro(0, y); cout << "|";
        diChuyenConTro(consoleWidth - 1, y); cout << "|";
    }
}

struct Diem {
    int x, y;
    Diem() {}
    Diem(int xMoi, int yMoi) : x(xMoi), y(yMoi) {}
};

class Ran {
    int doDai;
    char huong;
public:
    Diem than[MAX_LENGTH];

    Ran(int x, int y) {
        doDai = 1;
        than[0] = Diem(x, y);
        huong = HUONG_PHAI;
    }

    void doiHuong(char phim) {
        if (phim == 'w' && huong != HUONG_XUONG) huong = HUONG_LEN;
        else if (phim == 's' && huong != HUONG_LEN) huong = HUONG_XUONG;
        else if (phim == 'a' && huong != HUONG_PHAI) huong = HUONG_TRAI;
        else if (phim == 'd' && huong != HUONG_TRAI) huong = HUONG_PHAI;
    }

    int layDoDai() { return doDai; }

    bool diChuyen(Diem thucAn) {
        for (int i = doDai - 1; i > 0; i--) {
            than[i] = than[i - 1];
        }

        switch (huong) {
        case HUONG_LEN:    than[0].y--; break;
        case HUONG_XUONG:  than[0].y++; break;
        case HUONG_TRAI:   than[0].x--; break;
        case HUONG_PHAI:   than[0].x++; break;
        }

        if (than[0].x <= 0 || than[0].x >= consoleWidth - 1 ||
            than[0].y <= 1 || than[0].y >= consoleHeight - 1)
            return false;

        for (int i = 1; i < doDai; i++) {
            if (than[0].x == than[i].x && than[0].y == than[i].y)
                return false;
        }

        if (than[0].x == thucAn.x && than[0].y == thucAn.y && doDai < MAX_LENGTH - 1)
            doDai++;

        return true;
    }
};

void hienDiem(int diem) {
    doiMau(14);
    diChuyenConTro(2, 0);
    cout << "Diem: " << diem;
    doiMau(7);
}

void luuDiemCao(int diem) {
    int diemCao = 0;
    ifstream f("score.txt");
    if (f >> diemCao) f.close();
    if (diem > diemCao) {
        ofstream o("score.txt");
        o << diem; o.close();
    }
}

int layDiemCao() {
    int diemCao = 0;
    ifstream f("score.txt");
    if (f >> diemCao) f.close();
    return diemCao;
}

void hienMenuChinh() {
    system("cls");
    layKichThuocManHinh();
    inCanGiua("======= TRO CHOI RAN =======", 5);
    inCanGiua("1. Bat dau choi", 7);
    inCanGiua("2. Xem diem cao", 8);
    inCanGiua("3. Thoat", 9);
    inCanGiua("Chon: ", 11);
}

void hienMenuKetThuc() {
    inCanGiua("===== GAME OVER =====", 12);
    inCanGiua("1. Choi lai", 14);
    inCanGiua("2. Xem diem cao", 15);
    inCanGiua("3. Thoat", 16);
    inCanGiua("Chon: ", 18);
}

int choiGame() {
    srand(time(0));
    layKichThuocManHinh();

    Ran ran(consoleWidth / 2, consoleHeight / 2);
    Diem thucAn(rand() % (consoleWidth - 2) + 1, rand() % (consoleHeight - 3) + 2);
    int diem = 0;

    while (true) {
        if (!ran.diChuyen(thucAn)) break;

        if (ran.than[0].x == thucAn.x && ran.than[0].y == thucAn.y) {
            diem++;
            thucAn = Diem(rand() % (consoleWidth - 2) + 1, rand() % (consoleHeight - 3) + 2);
        }

        system("cls");
        veKhungMap();
        hienDiem(diem);

        doiMau(10); // Rắn xanh
        for (int i = 0; i < ran.layDoDai(); i++) {
            diChuyenConTro(ran.than[i].x, ran.than[i].y);
            cout << "O";
        }

        doiMau(12); // Mồi đỏ
        diChuyenConTro(thucAn.x, thucAn.y);
        cout << "*";

        doiMau(7); // Màu mặc định

        if (_kbhit()) {
            char phim = _getch();
            ran.doiHuong(phim);
        }

        Sleep(100);
    }
    return diem;
}

int main() {
    bool choiTiep = true;

    while (choiTiep) {
        hienMenuChinh();
        int luaChon; cin >> luaChon;

        if (luaChon == 1) {
            int diem = choiGame();
            luuDiemCao(diem);
            cout << "\nDiem cua ban: " << diem << endl;

            hienMenuKetThuc();
            cin >> luaChon;
            if (luaChon == 1) continue;
            else if (luaChon == 2) {
                cout << "Diem cao nhat: " << layDiemCao() << endl;
                system("pause");
            }
            else choiTiep = false;
        }
        else if (luaChon == 2) {
            cout << "Diem cao nhat: " << layDiemCao() << endl;
            system("pause");
        }
        else choiTiep = false;
    }

    cout << "Hen gap lai! Cam on da choi.\n";
    return 0;
}
