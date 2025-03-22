#include "Festa.hpp"
#include <iomanip>

using namespace std;

const char* FORMATADOR_DATA = "%d/%m/%Y"; // Formato de data: dia/mês/ano

namespace model {

// Construtor
Festa::Festa(const string& idFesta, const string& idCasamento, const string& endereco,
             double valorFesta, int numParcelas, const tm& data, const string& hora,
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

tm Festa::getData() const {
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
    os << "ID da festa: " << festa.idFesta << endl
       << "ID do casamento: " << festa.idCasamento << endl
       << "Endereço: " << festa.endereco << endl
       << "Valor da festa: R$ " << fixed << setprecision(2) << festa.valorFesta << endl
       << "Número de parcelas: " << festa.numParcelas << endl
       << "Valor da parcela: R$ " << fixed << setprecision(2) << festa.getValorParcela() << endl
       << "Hora: " << festa.hora << endl
       << "Convidados:" << endl;
    for (const auto& convidado : festa.convidados) {
        os << "  - " << convidado << endl;
    }
    return os;
}

} // namespace model
