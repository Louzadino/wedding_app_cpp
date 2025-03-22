#include "Casamento.hpp"
#include <iostream>
#include <stdexcept>
#include <regex>
#include <iomanip>
#include "Festa.hpp"

using namespace std;

namespace model {

// Construtor para casamento sem festa
Casamento::Casamento(const string& idCasamento, Casal* casal,
                     const tm& dataCasamento, const string& horaCasamento,
                     const string& localCerimonia)
    : Casamento(idCasamento, casal, dataCasamento, horaCasamento, localCerimonia, nullptr) {}

// Construtor para casamento com ou sem festa
Casamento::Casamento(const string& idCasamento, Casal* casal,
                     const tm& dataCasamento, const string& horaCasamento,
                     const string& localCerimonia, Festa* festa)
    : idCasamento(idCasamento), casal(casal), dataCasamento(dataCasamento),
      horaCasamento(horaCasamento), localCerimonia(localCerimonia), festa(festa) {
    if (idCasamento.empty() || !regex_match(idCasamento, regex("\\d{32}"))) {
        throw invalid_argument("O ID do casamento deve conter exatamente 32 dígitos numéricos.");
    }
    if (horaCasamento.empty() || !regex_match(horaCasamento, regex("([01]\\d|2[0-3]):[0-5]\\d"))) {
        throw invalid_argument("A hora do casamento deve estar no formato HH:mm (00:00 a 23:59).");
    }
    if (localCerimonia.empty()) {
        throw invalid_argument("O local da cerimônia não pode ser vazio.");
    }
}

// Getters
string Casamento::getIdCasamento() const {
    return idCasamento;
}

Casal* Casamento::getCasal() const {
    return casal;
}

tm Casamento::getDataCasamento() const {
    return dataCasamento;
}

string Casamento::getHoraCasamento() const {
    return horaCasamento;
}

string Casamento::getLocalCerimonia() const {
    return localCerimonia;
}

Festa* Casamento::getFesta() const {
    return festa;
}

// Setters
void Casamento::setDataCasamento(const tm& dataCasamento) {
    this->dataCasamento = dataCasamento;
}

void Casamento::setHoraCasamento(const string& horaCasamento) {
    if (horaCasamento.empty() || !regex_match(horaCasamento, regex("([01]\\d|2[0-3]):[0-5]\\d"))) {
        throw invalid_argument("A hora do casamento deve estar no formato HH:mm (00:00 a 23:59).");
    }
    this->horaCasamento = horaCasamento;
}

void Casamento::setLocalCerimonia(const string& localCerimonia) {
    if (localCerimonia.empty()) {
        throw invalid_argument("O local da cerimônia não pode ser vazio.");
    }
    this->localCerimonia = localCerimonia;
}

void Casamento::setFesta(Festa* festa) {
    this->festa = festa;
}

// Sobrecarga do operador << para imprimir detalhes do casamento
ostream& operator<<(ostream& os, const Casamento& casamento) {
    os << "ID do casamento: " << casamento.idCasamento << endl;
    os << "Casal: " << *casamento.casal << endl;
    os << "Data do casamento: " << put_time(&casamento.dataCasamento, "%d/%m/%Y") << endl;
    os << "Hora do casamento: " << casamento.horaCasamento << endl;
    os << "Local da cerimônia: " << casamento.localCerimonia << endl;
    if (casamento.festa != nullptr) {
        os << "Festa: " << *casamento.festa << endl;
    }
    return os;
}

}
