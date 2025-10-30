#include <iostream>
#include <string>
#include <thread>
#include <chrono>
#include <windows.h>
#include "Sensores.h"
#include "Lista.h"

using namespace std;


string leerLinea(HANDLE hSerial) {
    char buffer[1];
    string linea;
    DWORD bytesLeidos;

    while (true) {
        if (ReadFile(hSerial, buffer, 1, &bytesLeidos, NULL)) {
            if (bytesLeidos == 0) {
                this_thread::sleep_for(chrono::milliseconds(10));
                continue;
            }
            if (buffer[0] == '\n') break;
            if (buffer[0] != '\r') linea += buffer[0];
        } else {
            break;
        }
    }
    return linea;
}

int main() {
    const char* puerto = "\\\\.\\COM4";  
    HANDLE hSerial = CreateFileA(
        puerto,
        GENERIC_READ | GENERIC_WRITE,
        0,
        0,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        0
    );

    if (hSerial == INVALID_HANDLE_VALUE) {
        cerr << "Error al abrir el puerto serial. Verifica el COM correcto.\n";
        return 1;
    }

    DCB dcbSerialParams = { 0 };
    dcbSerialParams.DCBlength = sizeof(dcbSerialParams);
    GetCommState(hSerial, &dcbSerialParams);
    dcbSerialParams.BaudRate = CBR_9600;
    dcbSerialParams.ByteSize = 8;
    dcbSerialParams.StopBits = ONESTOPBIT;
    dcbSerialParams.Parity = NOPARITY;
    SetCommState(hSerial, &dcbSerialParams);

    COMMTIMEOUTS timeouts = { 0 };
    timeouts.ReadIntervalTimeout = 50;
    timeouts.ReadTotalTimeoutConstant = 50;
    timeouts.ReadTotalTimeoutMultiplier = 10;
    SetCommTimeouts(hSerial, &timeouts);

    cout << "Conectado al Arduino en " << puerto << endl;
    cout << "Esperando datos...\n\n";

    SensorTemperatura st("TempArduino");
    SensorPresion sp("PresArduino");

    Lista<float> listaTemperaturas;
    Lista<float> listaPresiones;

    int contador = 0;

    while (true) {
        string linea = leerLinea(hSerial);
        if (!linea.empty()) {
            if (linea.rfind("T:", 0) == 0) {
                float valor = stof(linea.substr(2));
                st.agregarLectura(valor);
                listaTemperaturas.insertarFinal(valor);
                cout << "[T] Nueva lectura: " << valor << " °C" << endl;
            } else if (linea.rfind("P:", 0) == 0) {
                float valor = stof(linea.substr(2));
                sp.agregarLectura(valor);
                listaPresiones.insertarFinal(valor);
                cout << "[P] Nueva lectura: " << valor << " hPa" << endl;
            }

            contador++;
            if (contador % 5 == 0) {
                cout << "\n=== Temperaturas registradas ===\n";
                listaTemperaturas.mostrar();
                cout << "\n=== Presiones registradas ===\n";
                listaPresiones.mostrar();
                cout << endl;
            }
        }

        this_thread::sleep_for(chrono::milliseconds(50));
    }

    CloseHandle(hSerial);
    return 0;
}
