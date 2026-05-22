#include <iostream>
#include <thread>
#include <vector>
#include <mutex>
#include <algorithm>
using namespace std;

mutex mtx;
vector<pair<string,int>> resultado; // El vector global local contiene parejas de cada palabra y su conteo respectivo.

// Buscar una palabra dentro del vector de frecuencias
int buscarPalabra(vector<pair<string,int>>& lista,string palabra){
    // relizar la busqueda
    for (int i = 0; i < lista.size(); i ++){
        if(lista[i].first == palabra){
            return i;
        }
    }
    return -1; // regresar -1 si no se encuentra una palabra
}

// Cada thread cuenta uan parte del texto

void contarPalabras(vector<string>& palabras, int inicio, int fin) {
    vector<pair<string, int>> local;

    for (int i = inicio; i < fin; i++) {
        int pos = buscarPalabra(local, palabras[i]);

        if (pos == -1) {
            local.push_back({palabras[i], 1}); // establecer una palabra nueva 
        } else {
            local[pos].second++; // si se encuentra una palabra de nuevo se auamneta su conteo actual
        }
    }

    // Combinar el resultado local con el resultado global
    mtx.lock();

    for (auto par : local) {
        int pos = buscarPalabra(resultado, par.first);

        if (pos == -1) {
            resultado.push_back(par);
        } else {
            resultado[pos].second += par.second; // agregar el conteo de la pareja local al global
        }
    }

    mtx.unlock();
}




int main(){
    vector<string> palabras_griegas = {
        "ho", "aner", "blepei", "ton", "doulon",
        "ho", "aner", "pherei", "ton", "doulon",
        "ho", "aner", "blepei", "en", "to", "agro"
    };
    
    int numThreads = 2;
    int mitad = palabras_griegas.size() / numThreads;

    thread t1(contarPalabras,ref(palabras_griegas), 0, mitad);
    thread t2(contarPalabras, ref(palabras_griegas), mitad, palabras_griegas.size());

    t1.join();
    t2.join();

    // lambda fucntion para sortear el conteo

    sort(resultado.begin(),resultado.end(),
        [](pair<string, int> a, pair<string,int> b){
            return a.second > b.second; // prioritizar la palabra con en conteo mayor 
        });

    cout << "Frecuencia de palabras:\n";
    for (auto par: resultado){
        cout << par.first << ": " << par.second << endl;
    }

    return 0;
}