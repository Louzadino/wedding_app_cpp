#include "Tarefa.hpp"
#include <iomanip>
#include <sstream>
#include <regex>

using namespace std;

namespace model {

// Função auxiliar para adicionar dias a uma data
tm Tarefa::adicionarDias(const tm& data, int dias) const {
    tm result = data;
    time_t time = mktime(&result);
    time += dias * 24 * 60 * 60;
    result = *localtime(&time);
    return result;
}

// Construtor
Tarefa::Tarefa(const string& idTarefa, const string& idLar, const string& idPrestador,
               const tm& dataInicio, int prazoEntrega, double valorPrestador, int numParcelas, model::Compra* compra)
    : dataInicio(dataInicio), prazoEntrega(prazoEntrega), valorPrestador(valorPrestador),
      numParcelas(numParcelas), compra(compra) {

    if (!regex_match(idTarefa, regex("\\d{32}"))) {
        throw invalid_argument("O ID da tarefa deve conter exatamente 32 dígitos numéricos.");
    }

    if (!idLar.empty() && !regex_match(idLar, regex("\\d{32}"))) {
        throw invalid_argument("O ID do lar deve conter exatamente 32 dígitos numéricos.");
    }

    if (!regex_match(idPrestador, regex("\\d{32}"))) {
        throw invalid_argument("O ID do prestador deve conter exatamente 32 dígitos numéricos.");
    }

    if (prazoEntrega <= 0) {
        throw invalid_argument("O prazo de entrega deve ser um número positivo.");
    }

    if (valorPrestador < 0) {
        throw invalid_argument("O valor do prestador não pode ser negativo.");
    }

    if (numParcelas <= 0) {
        throw invalid_argument("O número de parcelas deve ser maior que zero.");
    }

    this->idTarefa = idTarefa;
    this->idLar = idLar;
    this->idPrestador = idPrestador;
}

// Getters
string Tarefa::getIdTarefa() const {
    return idTarefa;
}

string Tarefa::getIdLar() const {
    return idLar.empty() ? "Nenhum" : idLar;
}

string Tarefa::getIdPrestador() const {
    return idPrestador;
}

tm Tarefa::getDataInicio() const {
    return dataInicio;
}

int Tarefa::getPrazoEntrega() const {
    return prazoEntrega;
}

tm Tarefa::getDataEntrega() const {
    return adicionarDias(dataInicio, prazoEntrega);
}

double Tarefa::getValorPrestador() const {
    return valorPrestador;
}

int Tarefa::getNumParcelas() const {
    return numParcelas;
}

model::Compra* Tarefa::getCompra() const {
    return compra;
}

// Setters
void Tarefa::setCompra(model::Compra* compra) {
    this->compra = compra;
}

double Tarefa::getValorParcela() const {
    return valorPrestador / numParcelas;
}

// Sobrecarga do operador <<
ostream& operator<<(ostream& os, const Tarefa& tarefa) {
    os << "ID da tarefa: " << tarefa.idTarefa << endl;
    os << "ID do lar: " << tarefa.idLar << endl;
    os << "ID do prestador: " << tarefa.idPrestador << endl;
    os << "Data de início: " << put_time(&tarefa.dataInicio, "%d/%m/%Y") << endl;
    os << "Prazo de entrega: " << tarefa.prazoEntrega << " dias" << endl;

    tm dataEntrega = tarefa.getDataEntrega();
    os << "Data de entrega: " << put_time(&dataEntrega, "%d/%m/%Y") << endl;    os << "Valor do prestador: R$ " << fixed << setprecision(2) << tarefa.valorPrestador << endl;
    os << "Número de parcelas: " << tarefa.numParcelas << endl;
    os << "Valor da parcela: R$ " << fixed << setprecision(2) << tarefa.getValorParcela() << endl;
    os << "Compra associada: " << (tarefa.compra == nullptr ? "Nenhuma" : tarefa.compra->getIdCompra()) << endl;
    return os;
}

} // namespace model