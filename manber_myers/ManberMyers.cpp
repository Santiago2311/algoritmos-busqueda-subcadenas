#include <algorithm>
#include <bits/stdc++.h>
#include <chrono>
using namespace std;
using namespace std::chrono;

struct SubstrRank
{
public:
    int leftRank;
    int rightRank;
    int index;

    SubstrRank(int leftRank = 0, int rightRank = 0, int index = 0)
    {
        this->leftRank = leftRank;
        this->rightRank = rightRank;
        this->index = index;
    }
};

//Ahora se pasa el vector de enteros como referencia
void makeRanks(const vector<SubstrRank> &substr_rank, int n, vector<int>& rank)
{
    int r = 1;
    rank.resize(n, -1);
    rank[substr_rank[0].index] = r;

    for (int i = 1; i < n; i++)
    {
        if (substr_rank[i].leftRank != substr_rank[i - 1].leftRank || substr_rank[i].rightRank != substr_rank[i - 1].rightRank)
        {
            r++;
        }
        rank[substr_rank[i].index] = r;
    }
}

vector<int> suffixArray(const string &T)
{
    int n = T.size();
    vector<SubstrRank> substr_rank;
    substr_rank.reserve(n);

    for (int i = 0; i < n; i++)
    {
        int left = (int)T[i];
        int right = (i < n - 1) ? (int)T[i + 1] : 0;
        substr_rank.push_back(SubstrRank(left, right, i));
    }

    sort(substr_rank.begin(), substr_rank.end(), [](const SubstrRank &a, const SubstrRank &b)
         {
        if (a.leftRank == b.leftRank)
            return a.rightRank < b.rightRank;
        return a.leftRank < b.leftRank; });

    int l = 2;
    vector<int> rank;
    while (l < n)
    {
        makeRanks(substr_rank, n, rank);

        for (int i = 0; i < n; i++)
        {
            int nextIndex = substr_rank[i].index + l;
            int right = (nextIndex < n) ? rank[nextIndex] : 0;
            substr_rank[i] = SubstrRank(rank[substr_rank[i].index], right, substr_rank[i].index);
        }

        sort(substr_rank.begin(), substr_rank.end(), [](const SubstrRank &a, const SubstrRank &b)
             {
            if (a.leftRank == b.leftRank)
                return a.rightRank < b.rightRank;
            return a.leftRank < b.leftRank; });

        l *= 2;
    }

    vector<int> SA(n);
    for (int i = 0; i < n; i++)
        SA[i] = substr_rank[i].index;

    return SA;
}

//Encuentra todas las recurrencias de un patron
vector<int> searchPattern(const string &T, const string &pattern, const vector<int> &SA)
{
    vector<int> occurrences;
    int n = T.size();
    int m = pattern.size();
    
    if (m == 0 || m > n) return occurrences;
    
    int left = 0, right = n;
    while (left < right)
    {
        int mid = (left + right) / 2;
        string suffix = T.substr(SA[mid], min(m, n - SA[mid]));
        if (suffix < pattern)
            left = mid + 1;
        else
            right = mid;
    }
    int leftBound = left;
    
    left = 0;
    right = n;
    while (left < right)
    {
        int mid = (left + right) / 2;
        string suffix = T.substr(SA[mid], min(m, n - SA[mid]));
        if (suffix <= pattern)
            left = mid + 1;
        else
            right = mid;
    }
    int rightBound = right;
    
    for (int i = leftBound; i < rightBound; i++)
    {
        if (SA[i] + m <= n)
        {
            string suffix = T.substr(SA[i], m);
            if (suffix == pattern)
                occurrences.push_back(SA[i]);
        }
    }
    
    return occurrences;
}

//Funcion para leer el archivo
string readFile(string &nombre){
    ifstream file(nombre);
    if (!file.is_open()){
        cout << "No se pudo abrir el archivo" << endl;
        return "";
    }

    stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}


int main()
{
    string nombre = "../libros/homer.txt";
    string patron = "of";

    cout << nombre << endl;
    string T = readFile(nombre);
    int n = T.size();
    size_t memoria = n*12 + n*4 + n*4 + n;
    cout << "Memoria estimada: " << memoria / (1024.0 * 1024.0) << " MB" << endl;
    auto start = high_resolution_clock::now();
    vector<int> SA = suffixArray(T);
    auto end = high_resolution_clock::now();

    double constructionTime = duration_cast<milliseconds>(end - start).count();
    cout << "Tiempo de ejecucion: " << constructionTime << " ms" << endl;

    vector<int> posiciones = searchPattern(T, patron, SA);
    cout << "Ocurrencias encontradas del patron: " << posiciones.size() << endl;

    if (!posiciones.empty()){
        cout << "Primeras 10 posiciones: ";
        for (int i = 0; i < 10; i++){
            cout << posiciones[i] << " ";
        }
        cout << endl;
    }


    return 0;
}
