#include "CSVReader.hpp"
#include <fstream>
#include <sstream>

using namespace std;

namespace util {

vector<vector<string>> CSVReader::lerCSV(const string& filePath) {
    vector<vector<string>> linhas;
    ifstream file(filePath);
    if (!file.is_open()) {
        throw runtime_error("Erro ao abrir o arquivo CSV: " + filePath);
    }

    string linha;
    while (getline(file, linha)) {
        vector<string> campos;
        stringstream ss(linha);
        string campo;

        while (getline(ss, campo, ';')) {
            campos.push_back(campo);
        }
        linhas.push_back(campos);
    }

    file.close();
    return linhas;
}

} //namespace util