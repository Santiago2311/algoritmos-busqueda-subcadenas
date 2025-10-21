#include <bits/stdc++.h>
using namespace std;

//Definición de la función getBuckets: calcula los límites (inicio y fin) de cada símbolo en el arreglo T.
//Se utiliza map para almacenar los rangos (buckets) de cada símbolo.
map<int, pair<int,int>> getBuckets(vector<int>& T) {    //Parámetro por referencia para evitar copias innecesarias.
    map<int,int> count;                                 //Mapa para contar ocurrencias de cada símbolo.
    map<int,pair<int,int>> buckets;                     //Mapa que guardará los rangos de inicio y fin.

    //Conteo de frecuencias de cada carácter.
    for (int c : T)
        count[c] = count[c] + 1;

    int start = 0;
    //Generación de los límites de cada bucket.
    for (auto &p : count) {
        int c = p.first;
        buckets[c] = {start, start + count[c]};
        start += count[c];
    }
    return buckets;
}

//Implementación del algoritmo SA-IS para construir el arreglo de sufijos.
vector<int> sais(vector<int>& T) {
    int n = T.size();
    vector<bool> t(n);       //Vector que identifica los tipos de posición: 'S' o 'L'.

    //Último carácter de tipo S.
    t[n-1] = true;  // true = 'S', false = 'L'

    // Clasificación de cada posición
    for (int i = n - 1; i > 0; i--) {
        if (T[i-1] == T[i])
            t[i-1] = t[i];
        else
            t[i-1] = (T[i-1] < T[i]);  // true si S, false si L
    }

    map<int,pair<int,int>> buckets = getBuckets(T);  //Obtención de los buckets.
    map<int,int> count;                              //Mapa para contar inserciones dentro de cada bucket.
    vector<int> SA(n, -1);                           //Inicialización del arreglo de sufijos con -1.
    map<int,int> LMS;                                //Mapa que guarda las posiciones de los LMS (Leftmost S-type substrings).
    int end = -1;                                    //Marca del final del último segmento LMS.

    //Identificación de las posiciones LMS y colocación inicial en el arreglo SA.
    for (int i = n - 1; i > 0; i--) {
        if (t[i] && !t[i-1]) {
            int revoffset = ++count[T[i]];
            SA[buckets[T[i]].second - revoffset] = i;
            if (end != -1)
                LMS[i] = end;
            end = i;
        }
    }

    LMS[n - 1] = n;  //El último carácter también se considera LMS.
    count.clear();

    //Colocación de los sufijos tipo L hacia la izquierda.
    for (int i = 0; i < n; i++) {
        if (SA[i] >= 0 && SA[i] - 1 >= 0 && !t[SA[i] - 1]) {
            int symbol = T[SA[i] - 1];
            int offset = count[symbol];
            SA[buckets[symbol].first + offset] = SA[i] - 1;
            count[symbol] = offset + 1;
        }
    }

    //Colocación de los sufijos tipo S hacia la derecha.
    count.clear();
    for (int i = n - 1; i > 0; i--) {
        if (SA[i] > 0 && t[SA[i] - 1]) {
            int symbol = T[SA[i] - 1];
            int revoffset = ++count[symbol];
            SA[buckets[symbol].second - revoffset] = SA[i] - 1;
        }
    }

    //Renombrado de subcadenas LMS (creación del array de nombres).
    vector<int> namesp(n, -1);
    int name = 0;
    int prev = -1;
    for (int i = 0; i < n; i++) {
        if (SA[i] >= 0 && t[SA[i]] && SA[i] - 1 >= 0 && !t[SA[i] - 1]) {
            if (prev != -1) {
                int start1 = SA[prev];
                int end1 = LMS[SA[prev]];
                int start2 = SA[i];
                int end2 = LMS[SA[i]];
                
                if (end1 > start1 && end2 > start2 && end1 <= n && end2 <= n) {
                    vector<int> sub1(T.begin() + start1, T.begin() + end1);
                    vector<int> sub2(T.begin() + start2, T.begin() + end2);
                    if (sub1 != sub2)
                        name++;
                } else {
                    name++;
                }
            }
            prev = i;
            namesp[SA[i]] = name;
        }
    }
    //Extracción de nombres y sus índices asociados.
    vector<int> names;
    vector<int> SApIdx;
    for (int i = 0; i < n; i++) {
        if (namesp[i] != -1) {
            names.push_back(namesp[i]);
            SApIdx.push_back(i);
        }
    }

    //Recursividad en los nombres si existen duplicados.
    if (name < (int)names.size() - 1)
        names = sais(names);

    reverse(names.begin(), names.end());  //Reinserción de los nombres.

    //Reconstrucción final del SA a partir de los nombres ordenados.
    SA.assign(n, -1);
    count.clear();
    for (int i = 0; i < (int)names.size(); i++) {
        int pos = SApIdx[names[i]];
        int revoffset = ++count[T[pos]];
        SA[buckets[T[pos]].second - revoffset] = pos;
    }

    //Segunda pasada: inserción de sufijos tipo L.
    count.clear();
    for (int i = 0; i < n; i++) {
        if (SA[i] >= 0 && SA[i] - 1 >= 0 && !t[SA[i] - 1]) {
            int symbol = T[SA[i] - 1];
            int offset = count[symbol];
            SA[buckets[symbol].first + offset] = SA[i] - 1;
            count[symbol] = offset + 1;
        }
    }

    //Tercera pasada: inserción de sufijos tipo S.
    count.clear();
    for (int i = n - 1; i > 0; i--) {
        if (SA[i] > 0 && t[SA[i] - 1]) {
            int symbol = T[SA[i] - 1];
            int revoffset = ++count[symbol];
            SA[buckets[symbol].second - revoffset] = SA[i] - 1;
        }
    }

    return SA;
}

string leerArchivo(string& nombre){
    ifstream archivo("../libros/" + nombre);
    if (!archivo.is_open()) {
        cerr<<"Error al abrir el archivo: " << nombre << endl;
        exit(1);
    }
    return string((istreambuf_iterator<char>(archivo)), istreambuf_iterator<char>());
}

long getMemoryUsage() {
    long rss = 0;
    ifstream stat_stream("/proc/self/status", ios_base::in);
    string line;
    while (getline(stat_stream, line)) {
        if (line.compare(0, 6, "VmRSS:") == 0) {
            istringstream iss(line);
            string key;
            long value;
            string unit;
            iss >> key >> value >> unit;
            rss = value;
            break;
        }
    }
    return rss;
}

int main() {
    auto start = chrono::high_resolution_clock::now(); //Temporizador para tiempo de ejecución
    long mem_start = getMemoryUsage(); //Inicio de cálculo de memoria
    string nombreArchivo = "dracula.txt"; //Nombre del archivo
    string s = leerArchivo(nombreArchivo);

    vector<int> T;
    for (char c : s)
        T.push_back((int)c);  //Conversión de cada carácter a su valor ASCII.

    vector<int> SA = sais(T); //Cálculo del arreglo de sufijos.

    long mem_peak = getMemoryUsage(); //Memoria al final de la ejecución

    for (int x : SA)
        cout<<x<<" ";
    cout<<endl;

    auto end = chrono::high_resolution_clock::now(); //Finalizar temporizador
    auto duration_us = chrono::duration_cast<chrono::microseconds>(end - start).count();
    
    cout<<"Libro utilizado: "<<nombreArchivo<<endl;
    cout<<"Tamaño del texto: "<<T.size()<<" caracteres"<<endl;
    cout<<"Tiempo transcurrido: "<<duration_us / 1000.0<<" ms"<<endl;
    cout<<"Memoria utilizada: "<<(mem_peak - mem_start) / 1024.0<<" MB"<<endl;

    
    return 0;
}
