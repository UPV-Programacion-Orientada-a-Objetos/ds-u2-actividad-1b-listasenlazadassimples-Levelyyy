#ifndef SENSORES_H
#define SENSORES_H

#include <iostream>
#include <string>
using namespace std;


class Sensor {
protected:
    string nombre;
    float valorActual;

public:
    Sensor(string n) {
        nombre = n;
        valorActual = 0;
    }

    virtual void agregarLectura(float valor) {
        valorActual = valor;
    }

    virtual void mostrarDatos() {
        cout << nombre << ": " << valorActual << endl;
    }

    virtual ~Sensor() {}
};


class SensorTemperatura : public Sensor {
public:
    SensorTemperatura(string n) : Sensor(n) {}

    void agregarLectura(float valor) override {
        valorActual = valor;
    }

    void mostrarDatos() override {
        cout << "Temperatura (" << nombre << "): " << valorActual << " °C" << endl;
    }
};


class SensorPresion : public Sensor {
public:
    SensorPresion(string n) : Sensor(n) {}

    void agregarLectura(float valor) override {
        valorActual = valor;
    }

    void mostrarDatos() override {
        cout << "Presion (" << nombre << "): " << valorActual << " hPa" << endl;
    }
};

#endif
