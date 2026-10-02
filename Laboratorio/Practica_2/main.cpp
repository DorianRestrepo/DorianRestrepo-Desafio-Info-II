#include <iostream>
using namespace std;

int longitud(const char *c) {
    int n = 0;
    while (c[n] != '\0') n++;
    return n;
}

void problema1() {
    const int den[10] = {50000, 20000, 10000, 5000, 2000, 1000, 500, 200, 100, 50};
    int cant[10];
    long cantidad;

    cout << "Cantidad de dinero: ";
    cin >> cantidad;

    long resto = cantidad;
    for (int i = 0; i < 10; i++) {
        cant[i] = resto / den[i];
        resto = resto % den[i];
    }
    for (int i = 0; i < 10; i++)
        cout << den[i] << ": " << cant[i] << endl;
    cout << "Faltante: " << resto << endl;
}

bool sonIguales(const char *a, const char *b) {
    if (longitud(a) != longitud(b)) return false;
    for (int i = 0; a[i] != '\0'; i++)
        if (a[i] != b[i]) return false;
    return true;
}

void problema3() {
    char a[100], b[100];
    cin.ignore(1000, '\n');
    cout << "Cadena 1: ";
    cin.getline(a, 100);
    cout << "Cadena 2: ";
    cin.getline(b, 100);

    if (sonIguales(a, b)) cout << "Las cadenas son iguales (verdadero)" << endl;
    else                  cout << "Las cadenas son diferentes (falso)" << endl;
}


void enteroACadena(int numero, char *cad) {
    long long n = numero;
    char inv[12];
    int k = 0, p = 0;

    if (n < 0) {
        cad[p++] = '-';
        n = -n;
    }
    if (n == 0) inv[k++] = '0';
    while (n > 0) {
        inv[k++] = '0' + (n % 10);
        n = n / 10;
    }
    while (k > 0) cad[p++] = inv[--k];
    cad[p] = '\0';
}

void problema5() {
    int numero;
    char cad[12];
    cout << "Numero entero: ";
    cin >> numero;
    enteroACadena(numero, cad);
    cout << "Cadena: \"" << cad << "\"" << endl;
}

void problema7() {
    char orig[100], res[100];
    cin.ignore(1000, '\n');
    cout << "Cadena: ";
    cin.getline(orig, 100);

    int k = 0;
    for (int i = 0; orig[i] != '\0'; i++) {
        bool repetido = false;
        for (int j = 0; j < k; j++)
            if (res[j] == orig[i]) {
                repetido = true;
                break; }
        if (!repetido) res[k++] = orig[i];
    }
    res[k] = '\0';

    cout << "Original: " << orig << ". Sin repetidos: " << res << "." << endl;
}


void problema9() {
    int n;
    char num[100];
    cout << "Cifras por grupo (n): ";
    cin >> n;
    cout << "Cadena numerica: ";
    cin >> num;

    int L = longitud(num);
    int resto = L % n;
    int pos = 0;
    long long suma = 0;

    if (resto > 0) {
        long long v = 0;
        for (int i = 0; i < resto; i++) v = v * 10 + (num[pos++] - '0');
        suma += v;
    }
    while (pos < L) {
        long long v = 0;
        for (int i = 0; i < n; i++) v = v * 10 + (num[pos++] - '0');
        suma += v;
    }

    cout << "Original: " << num << "." << endl;
    cout << "Suma: " << suma << "." << endl;
}


void mostrarSala(char sala[15][20]) {
    cout << "\n    ";
    for (int c = 1; c <= 20; c++) {
        if (c < 10) cout << " ";
        cout << c << " ";
    }
    cout << endl;
    for (int f = 0; f < 15; f++) {
        cout << " " << (char)('A' + f) << "  ";
        for (int c = 0; c < 20; c++) cout << " " << sala[f][c] << " ";
        cout << endl;
    }
    cout << endl;
}

void problema11() {
    char sala[15][20];
    for (int f = 0; f < 15; f++)
        for (int c = 0; c < 20; c++) sala[f][c] = '-';

    int op = -1;
    while (op != 0) {
        mostrarSala(sala);
        cout << "1. Reservar  2. Cancelar  0. Volver al menu: ";
        cin >> op;
        if (op == 1 || op == 2) {
            char fila;
            int asiento;
            cout << "Fila (A-O): ";
            cin >> fila;
            if (fila >= 'a' && fila <= 'o') fila = fila - 'a' + 'A';
            cout << "Asiento (1-20): ";
            cin >> asiento;

            if (fila < 'A' || fila > 'O' || asiento < 1 || asiento > 20) {
                cout << "Fila o asiento invalido." << endl;
            } else {
                char &s = sala[fila - 'A'][asiento - 1];
                if (op == 1) {
                    if (s == '+') cout << "Ese asiento ya esta reservado." << endl;
                    else { s = '+'; cout << "Reserva realizada." << endl; }
                } else {
                    if (s == '-') cout << "Ese asiento no estaba reservado." << endl;
                    else { s = '-'; cout << "Reserva cancelada." << endl; }
                }
            }
        }
    }
}


const int FILAS = 6;
const int COLS = 8;

int contarEstrellas(int (*m)[COLS], int filas) {
    int estrellas = 0;
    for (int i = 1; i < filas - 1; i++) {
        for (int j = 1; j < COLS - 1; j++) {
            int suma = m[i][j] + m[i][j - 1] + m[i][j + 1] + m[i - 1][j] + m[i + 1][j];
            if (suma > 6 * 5) estrellas++;
        }
    }
    return estrellas;
}

void problema13() {
    int imagen[FILAS][COLS] = {
        {0,  3, 4,  0, 0, 0, 6,  8},
        {5, 13, 6,  0, 0, 0, 2,  3},
        {2,  6, 2,  7, 3, 0, 10, 0},
        {0,  0, 4, 15, 4, 1, 6,  0},
        {0,  0, 7, 12, 6, 9, 10, 4},
        {5,  0, 6, 10, 6, 4, 8,  0}
    };
    cout << "Estrellas encontradas: " << contarEstrellas(imagen, FILAS) << endl;
}


bool interseccion(const int A[4], const int B[4], int C[4]) {
    int x1 = (A[0] > B[0]) ? A[0] : B[0];
    int y1 = (A[1] > B[1]) ? A[1] : B[1];
    int x2 = (A[0] + A[2] < B[0] + B[2]) ? A[0] + A[2] : B[0] + B[2];
    int y2 = (A[1] + A[3] < B[1] + B[3]) ? A[1] + A[3] : B[1] + B[3];

    if (x2 > x1 && y2 > y1) {
        C[0] = x1;
        C[1] = y1;
        C[2] = x2 - x1;
        C[3] = y2 - y1;
        return true;
    }
    for (int i = 0; i < 4; i++) C[i] = 0;
    return false;
}

void problema15() {
    int A[4], B[4], C[4];
    cout << "Rectangulo A (x y ancho alto): ";
    for (int i = 0; i < 4; i++) cin >> A[i];
    cout << "Rectangulo B (x y ancho alto): ";
    for (int i = 0; i < 4; i++) cin >> B[i];

    if (interseccion(A, B, C))
        cout << "C = {" << C[0] << ", " << C[1] << ", " << C[2] << ", " << C[3] << "}" << endl;
    else
        cout << "Los rectangulos no se intersectan." << endl;
}


long sumaDivisores(long x) {          // divisores propios (sin x)
    if (x < 2) return 0;
    long suma = 1;
    for (long i = 2; i * i <= x; i++) {
        if (x % i == 0) {
            suma += i;
            if (i != x / i) suma += x / i;
        }
    }
    return suma;
}

void problema17() {
    long n;
    cout << "Numero: ";
    cin >> n;

    long total = 0;
    for (long a = 2; a < n; a++) {
        long b = sumaDivisores(a);
        if (b != a && sumaDivisores(b) == a) total += a;
    }
    cout << "El resultado de la suma es: " << total << endl;
}

int main() {
    int opcion = -1;
    while (opcion != 0) {
        cout << "\n=== Practica 2 - Problemas impares ===" << endl;
        cout << " 1. Billetes y monedas" << endl;
        cout << " 3. Comparar cadenas" << endl;
        cout << " 5. Entero a cadena" << endl;
        cout << " 7. Eliminar caracteres repetidos" << endl;
        cout << " 9. Separar en n cifras y sumar" << endl;
        cout << "11. Reservas de cine" << endl;
        cout << "13. Estrellas en la imagen" << endl;
        cout << "15. Interseccion de rectangulos" << endl;
        cout << "17. Numeros amigables" << endl;
        cout << " 0. Salir" << endl;
        cout << "Opcion: ";
        cin >> opcion;
        cout << endl;

        switch (opcion) {
        case 1:  problema1();  break;
        case 3:  problema3();  break;
        case 5:  problema5();  break;
        case 7:  problema7();  break;
        case 9:  problema9();  break;
        case 11: problema11(); break;
        case 13: problema13(); break;
        case 15: problema15(); break;
        case 17: problema17(); break;
        case 0:  cout << "Hasta luego." << endl; break;
        default: cout << "Opcion no valida." << endl;
        }
    }
    return 0;
}

