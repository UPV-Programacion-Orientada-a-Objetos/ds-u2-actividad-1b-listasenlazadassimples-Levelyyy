#ifndef LISTA_H
#define LISTA_H

#include <iostream>
using namespace std;


template <class T>
class Nodo {
public:
    T dato;
    Nodo<T>* siguiente;

    Nodo(T valor) {
        dato = valor;
        siguiente = NULL;
    }
};


template <class T>
class Lista {
private:
    Nodo<T>* cabeza;

public:
    Lista() {
        cabeza = NULL;
    }

    void insertarFinal(T valor) {
        Nodo<T>* nuevo = new Nodo<T>(valor);
        if (cabeza == NULL) {
            cabeza = nuevo;
        } else {
            Nodo<T>* aux = cabeza;
            while (aux->siguiente != NULL) {
                aux = aux->siguiente;
            }
            aux->siguiente = nuevo;
        }
    }

    void mostrar() {
        Nodo<T>* aux = cabeza;
        while (aux != NULL) {
            cout << aux->dato << " -> ";
            aux = aux->siguiente;
        }
        cout << "NULL" << endl;
    }

    ~Lista() {
        Nodo<T>* aux = cabeza;
        while (aux != NULL) {
            Nodo<T>* temp = aux;
            aux = aux->siguiente;
            delete temp;
        }
    }
};

#endif
