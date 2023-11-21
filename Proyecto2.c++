#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <vector>
#include <string>
#include <unordered_set>
#include <algorithm>

using namespace std;

struct Guardian {
    string nombre;
    int poder;
    string maestro;
    string ciudad;
};

using Arbol = unordered_map<string, vector<Guardian>>;
using Grafo = unordered_map<string, unordered_set<string>>;

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

        // Agregar el guardian al arbol
        arbol[guard.maestro].push_back(guard);
    }

    archivo.close();
}

void leerCiudades(const string &nombreArchivo, Grafo &grafo, unordered_set<string> &ciudadesUnicas) {
    ifstream archivo(nombreArchivo);

    if (!archivo.is_open()) {
        cout << "Error al abrir el archivo '" << nombreArchivo << "'.";
        return;
    }

    string linea;
    while (getline(archivo, linea)) {
        istringstream ss(linea);
        string ciudad1, ciudad2;
        getline(ss, ciudad1, ',');
        getline(ss, ciudad2);

        // Agregar conexiones al grafo no dirigido
        grafo[ciudad1].insert(ciudad2);
        grafo[ciudad2].insert(ciudad1);

        // Agregar ciudades al conjunto
        ciudadesUnicas.insert(ciudad1);
        ciudadesUnicas.insert(ciudad2);
    }

    archivo.close();
}

void imprimirGrafo(const Grafo &grafo) {
    cout << "Grafo de Ciudades:\n";
    for (const auto &par : grafo) {
        cout << par.first << ": ";
        for (const auto &vecino : par.second) {
            cout << vecino << " ";
        }
        cout << "\n";
    }
}

void imprimirMatrizAdyacenciaConLetras(const unordered_set<string> &ciudadesUnicas, const Grafo &grafo, const unordered_map<string, char> &ciudadLetra) {
    cout << "\nMatriz de Adyacencia con letras:\n";

    // Imprimir encabezado
    cout << "   ";
    for (const auto &ciudad : ciudadesUnicas) {
        cout << ciudadLetra.at(ciudad) << " ";
    }
    cout << "\n";

    // Imprimir filas de la matriz
    for (const auto &ciudad : ciudadesUnicas) {
        cout << ciudadLetra.at(ciudad) << ": ";
        for (const auto &otraCiudad : ciudadesUnicas) {
            cout << (grafo.at(ciudad).count(otraCiudad) > 0 ? "1 " : "0 ");
        }
        cout << "\n";
    }
}

void mostrarGrafoCiudades() {
    Grafo grafo;
    unordered_set<string> ciudadesUnicas;
    leerCiudades("ciudades.txt", grafo, ciudadesUnicas);

    imprimirGrafo(grafo);

    // Asignar letras a las ciudades
    unordered_map<string, char> ciudadLetra;
    char letra = 'A';
    for (const auto &ciudad : ciudadesUnicas) {
        ciudadLetra[ciudad] = letra;
        ++letra;
    }

    // Imprimir asignacion de letras a ciudades
    cout << "\nAsignación de letras a ciudades:\n";
    for (const auto &par : ciudadLetra) {
        cout << par.second << ":" << par.first << "\n";
    }

    // Imprimir Matriz de Adyacencia con letras
    imprimirMatrizAdyacenciaConLetras(ciudadesUnicas, grafo, ciudadLetra);
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
        cout << "2. Mostrar Grafo de Ciudades\n"; 
        cout << "0. Salir\n";
        cout << "Ingrese su opcion: ";
        cin >> opcion;

        switch (opcion) {
            case 1:
                cout << "\nArbol de Personajes:\n";
                imprimirArbol(arbol, "None", 0);
                break;
            case 2:
                mostrarGrafoCiudades(); // Llamar a la funcion para mostrar el grafo de ciudades
                break;
            case 0:
                cout << "Saliendo del programa. Hasta luego!\n";
                break;
            default:
                cout << "Opcion no valida. Intentelo de nuevo.\n";
        }

    } while (opcion != 0);

    return 0;
}





