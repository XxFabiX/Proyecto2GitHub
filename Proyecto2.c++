#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <vector>
#include <string>

using namespace std;

struct Guardian {
    string nombre;
    int poder;
    string maestro;
    string ciudad;
};

using Arbol = unordered_map<string, vector<Guardian>>;

void leerGuardianes(const string &nombreArchivo, Arbol &arbol) {
    ifstream archivo(nombreArchivo);

    if (!archivo.is_open()) {
        cout << "Error al abrir el archivo '" << nombreArchivo << "'.";
        return;
    }

    string linea;
    while (getline(archivo, linea)) {
        istringstream ss(linea);
        Guardian guard;
        getline(ss, guard.nombre, ',');
        ss >> guard.poder;
        ss.ignore(); // Ignorar la coma despues del poder
        getline(ss, guard.maestro, ',');
        getline(ss, guard.ciudad);

        // Agregar el guardian al árbol
        arbol[guard.maestro].push_back(guard);
    }

    archivo.close();
}

void imprimirArbol(const Arbol &arbol, const string &raiz, int nivel) {
    if (arbol.find(raiz) == arbol.end()) {
        return;
    }

    for (const auto &guardian : arbol.at(raiz)) {
        for (int i = 0; i < nivel; ++i) {
            cout << "  ";
        }
        cout << "- " << guardian.nombre << " (" << guardian.ciudad << ")\n";

        // Llamada recursiva para imprimir los hijos del guardian actual
        imprimirArbol(arbol, guardian.nombre, nivel + 1);
    }
}

int main() {
    Arbol arbol;
    leerGuardianes("guardianes.txt", arbol);

    int opcion;
    do {
        cout << "\nMenu:\n";
        cout << "1. Mostrar Jerarquia\n";
        cout << "0. Salir\n";
        cout << "Ingrese su opción: ";
        cin >> opcion;

        switch (opcion) {
            case 1:
                cout << "\nArbol de Personajes:\n";
                imprimirArbol(arbol, "None", 0);
                break;
            case 0:
                cout << "Saliendo del programa. Hasta luego!\n";
                break;
            default:
                cout << "Opción no valida. Intentelo de nuevo.\n";
        }

    } while (opcion != 0);

    return 0;
}





