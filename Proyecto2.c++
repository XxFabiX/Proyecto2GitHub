#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

struct Guardian {
    string nombre;
    int poder;
    string maestro;
    string ciudad;
};

using Grafo = unordered_map<string, unordered_set<string>>;
using Arbol = unordered_map<string, vector<Guardian>>;


bool compararPorPoder(const Guardian &a, const Guardian &b) {
    return a.poder > b.poder;
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

void agregarCamino(Grafo &grafo, unordered_set<string> &ciudadesUnicas, const unordered_map<string, char> &ciudadLetra) {
    cout << "\nLista de ciudades:\n";
    for (const auto &par : ciudadLetra) {
        cout << par.second << ":" << par.first << "\n";
    }

    char letraOrigen, letraDestino;

    cout << "Elija la letra de la ciudad de origen para agregar un camino: ";
    cin >> letraOrigen;

    cout << "Elija la letra de la ciudad de destino para conectarla: ";
    cin >> letraDestino;

    // Obtener nombres de ciudades desde las letras
    string ciudadOrigen, ciudadDestino;
    for (const auto &par : ciudadLetra) {
        if (par.second == letraOrigen) {
            ciudadOrigen = par.first;
        }
        if (par.second == letraDestino) {
            ciudadDestino = par.first;
        }
    }

    // Verificar si las ciudades son validas
    if (ciudadOrigen.empty() || ciudadDestino.empty()) {
        cout << "Letras de ciudad no validas. Asegurese de elegir letras de ciudad existentes.\n";
        return;
    }

    // Agregar conexion al grafo
    grafo[ciudadOrigen].insert(ciudadDestino);
    grafo[ciudadDestino].insert(ciudadOrigen);

    // Agregar ciudades al conjunto
    ciudadesUnicas.insert(ciudadOrigen);
    ciudadesUnicas.insert(ciudadDestino);

    cout << "Se ha agregado un camino entre " << ciudadOrigen << " y " << ciudadDestino << ".\n";
}

void eliminarCamino(Grafo &grafo, unordered_set<string> &ciudadesUnicas, const unordered_map<string, char> &ciudadLetra) {
    cout << "\nLista de ciudades:\n";
    for (const auto &par : ciudadLetra) {
        cout << par.second << ":" << par.first << "\n";
    }

    char letraOrigen, letraDestino;

    cout << "Elija la letra de la ciudad de origen para eliminar un camino: ";
    cin >> letraOrigen;

    cout << "Elija la letra de la ciudad de destino para desconectarla: ";
    cin >> letraDestino;

    // Obtener nombres de ciudades desde las letras
    string ciudadOrigen, ciudadDestino;
    for (const auto &par : ciudadLetra) {
        if (par.second == letraOrigen) {
            ciudadOrigen = par.first;
        }
        if (par.second == letraDestino) {
            ciudadDestino = par.first;
        }
    }

    // Verificar si las ciudades son validas
    if (ciudadOrigen.empty() || ciudadDestino.empty()) {
        cout << "Letras de ciudad no validas. Asegurese de elegir letras de ciudad existentes.\n";
        return;
    }

    // Verificar si existe un camino entre las ciudades
    if (grafo[ciudadOrigen].count(ciudadDestino) == 0) {
        cout << "No hay un camino entre " << ciudadOrigen << " y " << ciudadDestino << ".\n";
        return;
    }

    // Eliminar conexion del grafo
    grafo[ciudadOrigen].erase(ciudadDestino);
    grafo[ciudadDestino].erase(ciudadOrigen);

    cout << "Se ha eliminado el camino entre " << ciudadOrigen << " y " << ciudadDestino << ".\n";
}

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
        ss.ignore(); // Ignorar la coma después del poder
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

void mostrarArbol(const Arbol &arbol) {
    cout << "\nArbol de Guardianes:\n";
    imprimirArbol(arbol, "None", 0);
}

void mostrarListadoCiudades(const unordered_set<string> &ciudadesUnicas, const unordered_map<string, char> &ciudadLetra) {
    cout << "\nListado de Ciudades:\n";
    for (const auto &ciudad : ciudadesUnicas) {
        cout << ciudadLetra.at(ciudad) << ":" << ciudad << "\n";
    }
}

void consultarGuardianesEnCiudad(const Grafo &grafo, const Arbol &arbol, const unordered_set<string> &ciudadesUnicas, const unordered_map<string, char> &ciudadLetra) {
    mostrarListadoCiudades(ciudadesUnicas, ciudadLetra);

    char letraCiudad;
    cout << "Elija la letra de la ciudad que desea consultar: ";
    cin >> letraCiudad;

    // Obtener nombre de la ciudad desde la letra
    string ciudadConsulta;
    for (const auto &par : ciudadLetra) {
        if (par.second == letraCiudad) {
            ciudadConsulta = par.first;
            break;
        }
    }

    if (ciudadConsulta.empty()) {
        cout << "Letra de ciudad no valida. Asegurese de elegir una letra de ciudad existente.\n";
        return;
    }

    // Mostrar guardianes en la ciudad seleccionada
    cout << "\nEn la ciudad " << ciudadConsulta << " estan los siguientes guardianes:\n";
    for (const auto &par : arbol) {
        for (const auto &guardian : par.second) {
            if (guardian.ciudad == ciudadConsulta) {
                cout << "- " << guardian.nombre << " (Maestro: " << par.first << ", Poder: " << guardian.poder << ")\n";
            }
        }
    }

    // Mostrar ciudades conectadas a la ciudad seleccionada
    cout << "\nLa ciudad " << ciudadConsulta << " esta conectada con las siguientes ciudades:\n";
    for (const auto &vecino : grafo.at(ciudadConsulta)) {
        cout << "- " << vecino << "\n";
    }
}

// Función para mostrar el ranking de guardianes por puntos de batalla
void mostrarRanking(const vector<Guardian> &guardianes) {
    cout << "\nRanking de Guardianes por Poder de Batalla:\n";
    for (size_t i = 0; i < guardianes.size(); ++i) {
        cout << i + 1 << ".-" << guardianes[i].nombre << " (" << guardianes[i].poder << ")\n";
    }
}


int main() {
    Grafo grafo;
    unordered_set<string> ciudadesUnicas;
    unordered_map<string, char> ciudadLetra;
    Arbol arbol;
    Arbol arbolRanking;  // Nuevo árbol para construir el ranking
    bool arbolConstruido = false;  // Indicar si el árbol general se ha construido
    bool arbolRankingConstruido = false;  // Indicar si el árbol de ranking se ha construido
    vector<Guardian> guardianes;    // Vector para almacenar guardianes

    // Construir el árbol general al inicio del programa
    leerGuardianes("guardianes.txt", arbol);
    arbolConstruido = true;

    int opcion;

    do {
        cout << "\nMenu:\n";
        cout << "1. Leer ciudades y mostrar grafo\n";
        cout << "2. Agregar un camino entre dos ciudades\n";
        cout << "3. Eliminar un camino entre dos ciudades\n";
        cout << "4. Mostrar Arbol de Guardianes\n";
        cout << "5. Consultar Guardianes en una Ciudad\n";
        cout << "6. Mostrar Ranking de Guardianes por Poder de Batalla\n";
        cout << "0. Salir\n";
        cout << "Ingrese la opcion: ";
        cin >> opcion;

        if (opcion == 1) {
            leerCiudades("ciudades.txt", grafo, ciudadesUnicas);

            // Asignar letras a las ciudades
            char letra = 'A';
            for (const auto &ciudad : ciudadesUnicas) {
                ciudadLetra[ciudad] = letra;
                ++letra;
            }

            // Imprimir asignación de letras a ciudades
            cout << "\nAsignacion de letras a ciudades:\n";
            for (const auto &par : ciudadLetra) {
                cout << par.second << ":" << par.first << "\n";
            }

            // Imprimir Matriz de Adyacencia con letras
            imprimirMatrizAdyacenciaConLetras(ciudadesUnicas, grafo, ciudadLetra);

            // Construir el árbol de ranking con la información actualizada
            leerGuardianes("guardianes.txt", arbolRanking);
            arbolRankingConstruido = true;

        } else if (opcion == 2) {
            agregarCamino(grafo, ciudadesUnicas, ciudadLetra);

        } else if (opcion == 3) {
            eliminarCamino(grafo, ciudadesUnicas, ciudadLetra);

        } else if (opcion == 4) {
            mostrarArbol(arbol);

        } else if (opcion == 5) {
            consultarGuardianesEnCiudad(grafo, arbol, ciudadesUnicas, ciudadLetra);

        } else if (opcion == 6) {
            // Construir el árbol de ranking cada vez que se selecciona la opción 6
            if (!arbolRankingConstruido) {
                leerGuardianes("guardianes.txt", arbolRanking);
                arbolRankingConstruido = true;
            }

            // Copiar los guardianes al vector y ordenar por poder de batalla
            guardianes.clear();
            for (const auto &par : arbolRanking) {
                for (const auto &guardian : par.second) {
                    guardianes.push_back(guardian);
                }
            }
            sort(guardianes.begin(), guardianes.end(), compararPorPoder);

            mostrarRanking(guardianes);

        } else if (opcion == 0) {
            cout << "Saliendo del programa.\n";

        } else {
            cout << "Opcion no valida. Intente de nuevo.\n";
        }

    } while (opcion != 0);

    return 0;
}




