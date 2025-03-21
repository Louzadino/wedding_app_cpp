#ifndef FINANCEIRO_H
#define FINANCEIRO_H

#include <stdexcept>
#include <string>
#include <sstream>
#include <iomanip>

#include "exception/DataInconsistencyException.hpp"

namespace model {

class Financeiro {

private:
    double dinheiroPoupanca;
    double salarioLiquido;
    double gastosMensais;

    // Método para validar valores negativos
    double validarValor(double valor, const std::string& campo) const;

public:
    /**
     * Construtor da classe Financeiro.
     *
     * @param dinheiroPoupanca Dinheiro guardado na poupança (não pode ser negativo).
     * @param salarioLiquido   Salário líquido mensal (não pode ser negativo).
     * @param gastosMensais    Gastos mensais da pessoa (não pode ser negativo).
     */
    Financeiro(double dinheiroPoupanca, double salarioLiquido, double gastosMensais);

    double getDinheiroPoupanca() const;
    double getSalarioLiquido() const;
    double getGastosMensais() const;

    void setDinheiroPoupanca(double dinheiroPoupanca);
    void setSalarioLiquido(double salarioLiquido);
    void setGastosMensais(double gastosMensais);

    std::string formatarValor(double valor) const;

    // Sobrecarga do operador <<
    friend std::ostream& operator<<(std::ostream& os, const Financeiro& financeiro);
};
}

#endif
