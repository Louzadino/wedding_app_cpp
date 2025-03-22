#include "Casal.hpp"
#include <iostream>

using namespace std;

namespace model {

Casal::Casal(const string& idPessoa1, const string& idPessoa2,
             const string& idCasamento, const string& idLar)
    : idPessoa1(idPessoa1), idPessoa2(idPessoa2),
      idCasamento(idCasamento), idLar(idLar) {}

// Getters
string Casal::getIdPessoa1() const {
    return idPessoa1;
}

string Casal::getIdPessoa2() const {
    return idPessoa2;
}

string Casal::getIdCasamento() const {
    return idCasamento;
}

string Casal::getIdLar() const {
    return idLar;
}

// Setters
void Casal::setIdCasamento(const string& idCasamento) {
    this->idCasamento = idCasamento;
}

void Casal::setIdLar(const string& idLar) {
    this->idLar = idLar;
}

// Verifica se o casal possui um casamento registrado
bool Casal::temCasamento() const {
    return !idCasamento.empty();
}

// Verifica se o casal possui um lar registrado
bool Casal::temLar() const {
    return !idLar.empty();
}

// Sobrecarga do operador << para impressão
ostream& operator<<(ostream& os, const Casal& casal) {
    os << "Casal: " << casal.idPessoa1 << " e " << casal.idPessoa2 << endl;
    os << "Casamento: " << casal.idCasamento << endl;
    os << "Lar: " << casal.idLar << endl;
    return os;
}

}