#include "Financeiro.hpp"
#include "exception/DataInconsistencyException.hpp"

using namespace std;
using namespace model;
using namespace exception;

namespace model {

Financeiro::Financeiro(double dinheiroPoupanca, double salarioLiquido, double gastosMensais) 
    : dinheiroPoupanca(validarValor(dinheiroPoupanca, "Dinheiro na poupança")),
      salarioLiquido(validarValor(salarioLiquido, "Salário líquido")),
      gastosMensais(validarValor(gastosMensais, "Gastos mensais")) {}

double Financeiro::validarValor(double valor, const string& campo) const {
    if (valor < 0) {
        throw DataInconsistencyException(campo + " não pode ser negativo.");
    }
    return valor;
}

double Financeiro::getDinheiroPoupanca() const {
    return dinheiroPoupanca;
}

double Financeiro::getSalarioLiquido() const {
    return salarioLiquido;
}

double Financeiro::getGastosMensais() const {
    return gastosMensais;
}

void Financeiro::setDinheiroPoupanca(double dinheiroPoupanca) {
    this->dinheiroPoupanca = validarValor(dinheiroPoupanca, "Dinheiro na poupança");
}

void Financeiro::setSalarioLiquido(double salarioLiquido) {
    this->salarioLiquido = validarValor(salarioLiquido, "Salário líquido");
}

void Financeiro::setGastosMensais(double gastosMensais) {
    this->gastosMensais = validarValor(gastosMensais, "Gastos mensais");
}

// Formatar valor para moeda brasileira (R$)
string Financeiro::formatarValor(double valor) const {
    ostringstream stream;
    stream << fixed << setprecision(2) << "R$ " << valor;
    return stream.str();
}

// Sobrecarga do operador <<
ostream& operator<<(ostream& os, const Financeiro& financeiro) {
    os << "Financeiro { Dinheiro Poupança = " << financeiro.formatarValor(financeiro.dinheiroPoupanca) 
       << ", Salário Líquido = " << financeiro.formatarValor(financeiro.salarioLiquido)
       << ", Gastos Mensais = " << financeiro.formatarValor(financeiro.gastosMensais)
       << ", Saldo Mensal = " << financeiro.formatarValor(financeiro.salarioLiquido - financeiro.gastosMensais) 
       << " }";
    return os;
}

} // namespace model