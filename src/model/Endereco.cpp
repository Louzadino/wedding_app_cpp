#include "Endereco.hpp"

using namespace std;

namespace model {

// Construtor
Endereco::Endereco(const string& rua, int numero, const string& complemento)
    : rua(rua), numero(numero), complemento(complemento) {

    if (rua.empty()) {
        throw invalid_argument("Nome da rua não pode ser vazio.");
    }
    if (numero <= 0) {
        throw invalid_argument("Número da casa não pode ser menor ou igual a zero.");
    }
}

// Getters
string Endereco::getRua() const {
    return rua;
}

int Endereco::getNumero() const {
    return numero;
}

string Endereco::getComplemento() const {
    return complemento;
}

// Setters
void Endereco::setRua(const string& rua) {
    if (rua.empty()) {
        throw invalid_argument("Nome da rua não pode ser vazio.");
    }
    this->rua = rua;
}

void Endereco::setNumero(int numero) {
    if (numero <= 0) {
        throw invalid_argument("Número da casa não pode ser menor ou igual a zero.");
    }
    this->numero = numero;
}

void Endereco::setComplemento(const string& complemento) {
    if (complemento.empty()) {
        throw invalid_argument("Complemento não pode ser vazio.");
    }
    this->complemento = complemento;
}

// Sobrecarga do operador << para impressão
ostream& operator<<(ostream& os, const Endereco& endereco) {
    os << endereco.rua << ", " << endereco.numero;
    if (!endereco.complemento.empty()) {
        os << ", " << endereco.complemento;
    }
    return os;
}

} // namespace model