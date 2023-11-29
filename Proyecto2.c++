#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <string>
#include <algorithm>
#include <queue>

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
        ss.ignore(); // Ignorar la coma despuus del poder
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

void mostrarRanking(const vector<Guardian> &guardianes) {
    cout << "\nRanking de Guardianes por Poder de Batalla:\n";
    for (size_t i = 0; i < guardianes.size(); ++i) {
        cout << i + 1 << ".-" << guardianes[i].nombre << " (" << guardianes[i].poder << ")";

        // Agregar categorizacion como "Caballeros" para poder entre 90 y 99
        if (guardianes[i].poder >= 90 && guardianes[i].poder <= 99) {
            cout << " - Caballero";
        }
        
        // Agregar categorizacion como "GRAN MAESTRO" para poder 100
        if (guardianes[i].poder == 100) {
            cout << " - GRAN MAESTRO";
        }

        cout << "\n";
    }
}


// Construir el arbol de ranking
void construirArbolRanking(const string &nombreArchivo, Arbol &arbolRanking) {
    leerGuardianes(nombreArchivo, arbolRanking);
}

void verGuardian(const Arbol &arbol) {
    cout << "Seleccione el número del guardian:\n";

    int contador = 1;
    for (const auto &par : arbol) {
        for (const auto &guardian : par.second) {
            cout << contador << ".-" << guardian.nombre << "\n";
            ++contador;
        }
    }

    int numeroGuardian;
    cout << "Ingrese el numero del guardian que desea ver: ";
    cin >> numeroGuardian;

    contador = 1;
    for (const auto &par : arbol) {
        for (const auto &guardian : par.second) {
            if (contador == numeroGuardian) {
                // Mostrar detalles del guardián
                cout << "\nDetalles del guardian " << guardian.nombre << ":\n";
                cout << "Poder: " << guardian.poder << "\n";
                cout << "Maestro: " << guardian.maestro << "\n";
                cout << "Ciudad: " << guardian.ciudad << "\n";

                // Mostrar aprendices si los tiene
                if (arbol.find(guardian.nombre) != arbol.end() && !arbol.at(guardian.nombre).empty()) {
                    cout << "Aprendices:\n";
                    for (const auto &aprendiz : arbol.at(guardian.nombre)) {
                        cout << "- " << aprendiz.nombre << "\n";
                    }
                } else {
                    cout << "No tiene aprendices.\n";
                }

                return;
            }
            ++contador;
        }
    }

    cout << "Numero de guardian no valido.\n";
}

void saberCamino(const Grafo &grafo, const unordered_map<string, char> &ciudadLetra) {
    cout << "Seleccione la letra de la ciudad de origen para saber el camino: ";
    char letraOrigen;
    cin >> letraOrigen;

    // Obtener nombre de la ciudad de origen desde la letra
    string ciudadOrigen;
    for (const auto &par : ciudadLetra) {
        if (par.second == letraOrigen) {
            ciudadOrigen = par.first;
            break;
        }
    }

    if (ciudadOrigen.empty()) {
        cout << "Letra de ciudad no valida. Asegúrate de elegir una letra de ciudad existente.\n";
        return;
    }

    cout << "Seleccione la letra de la ciudad de destino para saber el camino: ";
    char letraDestino;
    cin >> letraDestino;

    // Obtener nombre de la ciudad de destino desde la letra
    string ciudadDestino;
    for (const auto &par : ciudadLetra) {
        if (par.second == letraDestino) {
            ciudadDestino = par.first;
            break;
        }
    }

    if (ciudadDestino.empty()) {
        cout << "Letra de ciudad no valida. Asegúrate de elegir una letra de ciudad existente.\n";
        return;
    }

    // Verificar si existe un camino entre las ciudades
    if (grafo.at(ciudadOrigen).count(ciudadDestino) > 0) {
        cout << "Sí hay un camino directo entre " << ciudadOrigen << " y " << ciudadDestino << ".\n";
    } else {
        // Si no hay un camino directo, encontrar el camino usando BFS
        cout << "No hay un camino directo entre " << ciudadOrigen << " y " << ciudadDestino << ".\n";

        // Realizar la búsqueda en anchura para encontrar el camino
        unordered_map<string, string> padres; // Almacena los padres de cada nodo
        queue<string> cola;
        cola.push(ciudadOrigen);

        while (!cola.empty()) {
            string ciudadActual = cola.front();
            cola.pop();

            for (const auto &vecino : grafo.at(ciudadActual)) {
                if (padres.find(vecino) == padres.end()) {
                    padres[vecino] = ciudadActual;
                    cola.push(vecino);
                }
            }
        }

        // Construir el camino inverso desde la ciudad de destino hasta la ciudad de origen
        vector<string> caminoInverso;
        string ciudadActual = ciudadDestino;
        while (ciudadActual != ciudadOrigen) {
            caminoInverso.push_back(ciudadActual);
            ciudadActual = padres[ciudadActual];
        }
        caminoInverso.push_back(ciudadOrigen);

        // Mostrar el camino inverso
        cout << "Para llegar de " << ciudadOrigen << " a " << ciudadDestino << ", debes pasar por las siguientes ciudades:\n";
        for (auto it = caminoInverso.rbegin(); it != caminoInverso.rend(); ++it) {
            cout << *it;
            if (it + 1 != caminoInverso.rend()) {
                cout << " -> ";
            }
        }
        cout << "\n";
    }
}

void conocerAlReino(Grafo &grafo, unordered_set<string> &ciudadesUnicas, const unordered_map<string, char> &ciudadLetra) {
    char letraCiudadPartida;

    do {
        cout << "\nElige una ciudad por la cual partir:\n";
        for (const auto &par : ciudadLetra) {
            cout << par.second << ":" << par.first << "\n";
        }
        cout << "Escoja una (o ingrese 0 para volver al Menu Principal): ";
        cin >> letraCiudadPartida;

        if (letraCiudadPartida == '0') {
            cout << "Volviendo al Menu Principal.\n";
            break; // Salir del bucle si el usuario elige volver al Menú Principal
        }

        // Obtener nombre de la ciudad desde la letra
        string ciudadPartida;
        for (const auto &par : ciudadLetra) {
            if (par.second == letraCiudadPartida) {
                ciudadPartida = par.first;
                break;
            }
        }

        if (ciudadPartida.empty()) {
            cout << "Letra de ciudad no valida. Asegurese de elegir una letra de ciudad existente.\n";
            continue; // Volver al inicio del bucle
        }

        cout << "\nExcelente, estas en la ciudad: " << ciudadPartida << "!\n";

        int opcion;

        do {
            cout << "\nQue deseas hacer en " << ciudadPartida << "?\n";
            cout << "1. Agregar Camino\n";
            cout << "2. Eliminar Camino\n";
            cout << "3. Viajar\n";
            cout << "4. Saber Camino\n";
            cout << "0. Volver atras\n";
            cout << "Ingrese la opcion: ";
            cin >> opcion;

            switch (opcion) {
                case 1:
                    agregarCamino(grafo, ciudadesUnicas, ciudadLetra);
                    break;
                case 2:
                    eliminarCamino(grafo, ciudadesUnicas, ciudadLetra);
                    break;
                case 3:
                    // Viajar a otras ciudades
                    cout << "\nCiudades conectadas a " << ciudadPartida << ":\n";
                    for (const auto &vecino : grafo[ciudadPartida]) {
                        cout << ciudadLetra.at(vecino) << ": " << vecino << "\n";
                    }

                    char letraCiudadDestino;
                    cout << "A que ciudad deseas ir (o ingresa 0 para volver): ";
                    cin >> letraCiudadDestino;

                    if (letraCiudadDestino == '0') {
                        cout << "Volviendo a la opcion anterior.\n";
                    } else {
                        // Obtener nombre de la ciudad destino desde la letra
                        string ciudadDestino;
                        for (const auto &par : ciudadLetra) {
                            if (par.second == letraCiudadDestino) {
                                ciudadDestino = par.first;
                                break;
                            }
                        }

                        if (ciudadDestino.empty() || grafo[ciudadPartida].count(ciudadDestino) == 0) {
                            cout << "Letra de ciudad no valida o no conectada. Intentalo de nuevo.\n";
                        } else {
                            cout << "Viajando de " << ciudadPartida << " a " << ciudadDestino << ".\n";
                            ciudadPartida = ciudadDestino;
                        }
                    }
                    break;
                case 4:
                    saberCamino(grafo, ciudadLetra);
                    break;
                case 0:
                    cout << "Volviendo al Inicio.\n";
                    break;
                default:
                    cout << "Opcion no valida. Intentalo de nuevo.\n";
            }
        } while (opcion != 0);

    } while (true);
}


void presenciarBatalla(Arbol &arbol, const Grafo &grafo, const unordered_map<string, char> &ciudadLetra) {
    cout << "Seleccione un guardian con un rango de poder entre 0 y 89:\n";

    // Mostrar guardianes con poder entre 0 y 89
    int contador = 1;
    vector<Guardian> guardianesSeleccionables;  // Almacena los guardianes seleccionables
    for (const auto &par : arbol) {
        for (const auto &guardian : par.second) {
            if (guardian.poder >= 0 && guardian.poder <= 89) {
                cout << contador << ".-" << guardian.nombre << " (Poder: " << guardian.poder << ")\n";
                guardianesSeleccionables.push_back(guardian);
                ++contador;
            }
        }
    }

    int numeroGuardian;
    cout << "Ingrese el numero del guardian que desea seleccionar: ";
    cin >> numeroGuardian;

    if (numeroGuardian >= 1 && numeroGuardian <= guardianesSeleccionables.size()) {
        Guardian &guardianSeleccionado = guardianesSeleccionables[numeroGuardian - 1];
        cout << "\nExcelente, has seleccionado a " << guardianSeleccionado.nombre << "!\n";

        // Encontrar la ciudad del guardian seleccionado
        string ciudadGuardian;
        for (const auto &par : ciudadLetra) {
            if (par.second == ciudadLetra.at(guardianSeleccionado.ciudad)) {
                ciudadGuardian = par.first;
                break;
            }
        }

        if (!ciudadGuardian.empty()) {
            cout << "El guardian se encuentra en la ciudad " << ciudadGuardian << ".\n";
        } else {
            cout << "No se pudo determinar la ciudad del guardian.\n";
            return;  // Agregamos un return para salir de la función si no se encuentra la ciudad
        }

        int opcionBatalla;

        do {
            cout << "\nQue deseas hacer?\n";
            cout << "1. Pelear con un guardian en la misma ciudad\n";
            cout << "2. Cambiarse de ciudad\n";
            cout << "3. Mostrar Ranking Actualizado\n";
            cout << "4. Finalizar Juego\n";
            cout << "Ingrese la opcion: ";
            cin >> opcionBatalla;

            switch (opcionBatalla) {
                case 1: {
                    // Pelear con un guardian en la misma ciudad
					    cout << "\nGuardianes en la ciudad " << ciudadGuardian << ":\n";
					    int contadorGuardianesCiudad = 1;
					
					    for (const auto &par : arbol) {
					        for (const auto &guardian : par.second) {
					            if (guardian.ciudad == ciudadGuardian && guardian.nombre != guardianSeleccionado.nombre) {
					                cout << contadorGuardianesCiudad << ".-" << guardian.nombre << " (Poder: " << guardian.poder << ")\n";
					                ++contadorGuardianesCiudad;
					            }
					        }
					    }
					
					    int opcionPelea;
                    cout << "Ingrese el numero del guardian con el que deseas pelear (o 0 para volver): ";
                    cin >> opcionPelea;

                    if (opcionPelea >= 1 && opcionPelea <= contadorGuardianesCiudad - 1) {
                        Guardian &guardianRival = guardianesSeleccionables[opcionPelea - 1];
                        // Logica de pelea
                        bool ganador = (rand() % 10) < 4;  // Proporción de 4 a 6

                        if (ganador) {
                            cout << "Has ganado la batalla!\n";
                            if (guardianRival.maestro == "None") {
                                // Es un aprendiz, restar 3 puntos
                                guardianSeleccionado.poder += 3;
                                cout << "Ganaste 3 puntos.\n";
                            } else {
                                // Es un maestro, restar 5 puntos
                                guardianSeleccionado.poder += 5;
                                cout << "Ganaste 5 puntos.\n";
                            }

                            // Bajar puntaje a Guardian derrotado
                            guardianRival.poder -= 1;

                        } else {
                            cout << "Has perdido la batalla!\n";
                            // Bajar puntaje a Guardian derrotado
                            guardianSeleccionado.poder -= 1;
                        }
                    } else {
                        cout << "Numero de guardian no valido.\n";
                    }
                    break;
                }

                case 2: {
                    // Cambiarse de ciudad
                    cout << "\nCiudades conectadas a " << ciudadGuardian << ":\n";
                    for (const auto &vecino : grafo.at(ciudadGuardian)) {
                        cout << ciudadLetra.at(vecino) << ": " << vecino << "\n";
                    }

                    char letraCiudadDestino;
                    cout << "A que ciudad deseas ir (o ingresa 0 para volver): ";
                    cin >> letraCiudadDestino;

                    if (letraCiudadDestino == '0') {
                        cout << "Volviendo al menu de batalla.\n";
                    } else {
                        // Obtener nombre de la ciudad destino desde la letra
                        string ciudadDestino;
                        for (const auto &par : ciudadLetra) {
                            if (par.second == letraCiudadDestino) {
                                ciudadDestino = par.first;
                                break;
                            }
                        }

                        if (ciudadDestino.empty() || grafo.at(ciudadGuardian).count(ciudadDestino) == 0) {
                            cout << "Letra de ciudad no válida o no conectada. Inténtalo de nuevo.\n";
                        } else {
                            cout << "Viajando de " << ciudadGuardian << " a " << ciudadDestino << ".\n";
                            ciudadGuardian = ciudadDestino;
                        }
                    }
                    break;
                }

                case 3:
                    // Mostrar Ranking Actualizado
                    
                    sort(guardianesSeleccionables.begin(), guardianesSeleccionables.end(), compararPorPoder);
                    mostrarRanking(guardianesSeleccionables);
                    
                    break;

                case 4:
                    // Finalizar Juego
                    if (guardianSeleccionado.poder >= 90) {
                        cout << "Felicitaciones! Has ganado el título de Caballero. Fin del juego.\n";
                    } else {
                        cout << "Fin del juego.\n";
                    }
                    break;

                default:
                    cout << "Opcion no valida. Intente de nuevo.\n";
            }
        } while (opcionBatalla != 4);

    } else {
        cout << "Numero de guardian no valido.\n";
    }
}



int main() {
    Grafo grafo;
    unordered_set<string> ciudadesUnicas;
    unordered_map<string, char> ciudadLetra;
    Arbol arbol;
    Arbol arbolRanking;
    bool arbolConstruido = false;
    bool arbolRankingConstruido = false;
    vector<Guardian> guardianes;

    // Leer ciudades al inicio del programa
    leerCiudades("ciudades.txt", grafo, ciudadesUnicas);

    // Asignar letras a las ciudades
    char letra = 'A';
    for (const auto &ciudad : ciudadesUnicas) {
        ciudadLetra[ciudad] = letra;
        ++letra;
    }

    /*// Imprimir asignacion de letras a ciudades
    cout << "\nAsignacion de letras a ciudades:\n";
    for (const auto &par : ciudadLetra) {
        cout << par.second << ":" << par.first << "\n";
    }*/

    // Construir el arbol general al inicio del programa
    leerGuardianes("guardianes.txt", arbol);
    arbolConstruido = true;

    // Construir el arbol de ranking al inicio del programa
    construirArbolRanking("guardianes.txt", arbolRanking);
    arbolRankingConstruido = true;

    int opcion;

    do {
    	cout << "\n\nTHE GUARDIANS BATTLE\n";
    	
    	cout << "\nPD:\n";
    	cout << "Ten en cuenta que cuando te pide una letra debes ponerla en MAYUSCULA\n";
    	
        cout << "\nMenu:\n";
        cout << "1. Ver lista de Candidatos\n";
        cout << "2. Conocer al Guardian\n";
        cout << "3. Conocer el Reino\n";
        cout << "4. Presenciar Batalla\n";
        cout << "0. Salir\n";

        cout << "Ingrese la opcion: ";
        cin >> opcion;

        switch (opcion) {
            case 1:
                // Imprimir Matriz de Adyacencia con letras
                //imprimirMatrizAdyacenciaConLetras(ciudadesUnicas, grafo, ciudadLetra);
                 if (!arbolRankingConstruido) {
                    construirArbolRanking("guardianes.txt", arbolRanking);
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
                break;

            case 2:
                
                verGuardian(arbol);
                break;

            case 3:
        
                conocerAlReino(grafo, ciudadesUnicas, ciudadLetra);
                break;

            case 4:
                //mostrarArbol(arbol);
                presenciarBatalla(arbol, grafo, ciudadLetra);
                break;
			
            case 0:
                cout << "Saliendo del programa.\n";
                break;

            default:
                cout << "Opcion no valida. Intente de nuevo.\n";
        }

    } while (opcion != 0);

    return 0;
}
