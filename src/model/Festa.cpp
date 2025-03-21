#include "Festa.hpp"

using namespace std;

namespace model {
    class Festa;
}

// Construtor
Festa::Festa(const string& idFesta, const string& idCasamento, const string& endereco,
             double valorFesta, int numParcelas, const string& data, const string& hora,
             const vector<string>& convidados)
    : idFesta(idFesta), idCasamento(idCasamento), endereco(endereco),
      valorFesta(validarValor(valorFesta, "Valor da festa")), numParcelas(numParcelas),
      data(data), hora(hora), convidados(convidados) {

    if (idFesta.empty() || !regex_match(idFesta, regex("\\d{32}"))) {
        throw invalid_argument("O ID da festa deve conter exatamente 32 dígitos numéricos.");
    }
    if (idCasamento.empty() || !regex_match(idCasamento, regex("\\d{32}"))) {
        throw invalid_argument("O ID do casamento deve conter exatamente 32 dígitos numéricos.");
    }
    if (endereco.empty()) {
        throw invalid_argument("O endereço não pode ser nulo.");
    }
    if (!regex_match(hora, regex("([01]\\d|2[0-3]):[0-5]\\d"))) {
        throw invalid_argument("A hora da festa deve estar no formato HH:mm (00:00 a 23:59).");
    }
    if (numParcelas <= 0) {
        throw invalid_argument("O número de parcelas deve ser maior que zero.");
    }
}

// Getters
string Festa::getIdFesta() const {
    return idFesta;
}

string Festa::getIdCasamento() const {
    return idCasamento;
}

string Festa::getEndereco() const {
    return endereco;
}

double Festa::getValorFesta() const {
    return valorFesta;
}

int Festa::getNumParcelas() const {
    return numParcelas;
}

double Festa::getValorParcela() const {
    return valorFesta / numParcelas;
}

string Festa::getData() const {
    return data;
}

string Festa::getHora() const {
    return hora;
}

vector<string> Festa::getConvidados() const {
    return convidados;
}

// Setters
void Festa::setEndereco(const string& endereco) {
    if (endereco.empty()) {
        throw invalid_argument("O endereço não pode ser nulo.");
    }
    this->endereco = endereco;
}

void Festa::setValorFesta(double valorFesta) {
    this->valorFesta = validarValor(valorFesta, "Valor da festa");
}

void Festa::setData(const string& data) {
    if (data.empty()) {
        throw invalid_argument("A data da festa não pode ser nula.");
    }
    this->data = data;
}

void Festa::setHora(const string& hora) {
    if (!regex_match(hora, regex("([01]\\d|2[0-3]):[0-5]\\d"))) {
        throw invalid_argument("A hora da festa deve estar no formato HH:mm (00:00 a 23:59).");
    }
    this->hora = hora;
}

// Métodos adicionais
void Festa::adicionarConvidado(const string& convidado) {
    if (convidado.empty()) {
        throw invalid_argument("O nome do convidado não pode ser vazio.");
    }
    convidados.push_back(convidado);
}

void Festa::removerConvidado(const string& convidado) {
    if (convidado.empty()) {
        throw invalid_argument("O nome do convidado não pode ser vazio.");
    }
    auto it = find(convidados.begin(), convidados.end(), convidado);
    if (it != convidados.end()) {
        convidados.erase(it);
    } else {
        throw invalid_argument("Convidado não encontrado na lista.");
    }
}

// Validação de valor
double Festa::validarValor(double valor, const string& campo) {
    if (valor < 0) {
        throw invalid_argument(campo + " não pode ser negativo.");
    }
    return valor;
}

// Sobrecarga do operador <<
ostream& operator<<(ostream& os, const Festa& festa) {
    os << "Festa{ID='" << festa.idFesta << "', CasamentoID='" << festa.idCasamento
       << "', Convidados=" << festa.convidados.size()
       << ", Data='" << festa.data
       << "', Hora='" << festa.hora
       << "', Endereço='" << festa.endereco
       << "', Valor=R$ " << festa.valorFesta
       << ", Parcelas=" << festa.numParcelas << "}";

    return os;
}
