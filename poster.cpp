#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <map>
#include <dirent.h>  
#include <cstring>   

using namespace std;

// Definir los valores de referencia (umbrales)
map<string, double> umbrales = {
    {"Glucosa", 100.0},         // Valor normal de glucosa en mg/dl
    {"Colesterol", 200.0},      // Valor normal de colesterol total en mg/dl
    {"Triglicéridos", 150.0},   // Valor normal de triglicéridos en mg/dl
    {"Creatinina", 1.0}         // Valor normal de creatinina en mg/dl
};

// Estructura para almacenar los datos de cada persona (por DNI)
struct DatosBioquimicos {
    map<string, double> parametros;
};

// Función para leer el archivo y almacenar los datos
void leerArchivo(const string& dni, map<string, DatosBioquimicos>& registros) {
    string archivo = dni + ".txt";  // Se espera que el archivo esté en formato "DNI.txt"
    ifstream file(archivo);

    if (!file.is_open()) {
        cout << "No se pudo abrir el archivo para el DNI: " << dni << endl;
        return;
    }

    DatosBioquimicos datos;
    string linea;
    while (getline(file, linea)) {
        stringstream ss(linea);
        string parametro;
        double valor;
        ss >> parametro >> valor;
        datos.parametros[parametro] = valor;
    }

    registros[dni] = datos;  // Guardamos los datos en el mapa, asociado al DNI
    file.close();
}

// Función para verificar los valores elevados de un registro
void verificarValoresElevados(const DatosBioquimicos& datos) {
    bool encontrados = false;
    for (const auto& par : datos.parametros) {
        if (umbrales.find(par.first) != umbrales.end() && par.second > umbrales.at(par.first)) {
            cout << "¡Alerta! El valor de " << par.first << " es elevado: " << par.second << endl;
            encontrados = true;
        }
    }

    if (!encontrados) {
        cout << "Todos los valores están dentro del rango normal." << endl;
    }
}

// Función para listar todos los archivos .txt en un directorio
void listarArchivos(const string& directorio, map<string, DatosBioquimicos>& registros) {
    DIR* dir;
    struct dirent* entry;

    // Abrir el directorio
    dir = opendir(directorio.c_str());
    if (dir == nullptr) {
        cout << "No se pudo abrir el directorio: " << directorio << endl;
        return;
    }

    // Leer los archivos del directorio
    while ((entry = readdir(dir)) != nullptr) {
        // Solo procesar archivos que tengan extensión .txt
        if (strstr(entry->d_name, ".txt") != nullptr) {
            string dni = entry->d_name;
            dni = dni.substr(0, dni.find(".txt"));  // Extraer el DNI del nombre del archivo
            leerArchivo(dni, registros);  // Leer y almacenar los datos del archivo
        }
    }

    closedir(dir);  // Cerrar el directorio
}

int main() {
    map<string, DatosBioquimicos> registros;  // Mapa para almacenar los registros por DNI

    // Directorio donde se encuentran los archivos .txt
    string directorio = "./";  // Cambia esto si los archivos están en otro directorio

    // Leer todos los archivos .txt en el directorio
    listarArchivos(directorio, registros);

    // Solicitar al usuario el DNI para buscar
    string dniBuscado;
    cout << "Introduce el DNI para buscar los datos bioquimicos: ";
    cin >> dniBuscado;

    // Verificar si el DNI existe en los registros
    auto it = registros.find(dniBuscado);
    if (it != registros.end()) {
        cout << "Datos bioquimicos para el DNI: " << dniBuscado << endl;
        verificarValoresElevados(it->second);  // Verificar los valores elevados
    } else {
        cout << "No se encontraron datos para el DNI: " << dniBuscado << endl;
    }

    return 0;
}

