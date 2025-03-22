#ifndef FESTA_H
#define FESTA_H

#include <string>
#include <vector>
#include <stdexcept>
#include <regex>
#include <iostream>
#include <algorithm>
#include <ctime>

#include "util/DateUtils.hpp"

using namespace std;

namespace model {

class Festa {

class DateUtils;

private:
    string idFesta;
    string idCasamento;
    string endereco;
    double valorFesta;
    int numParcelas;
    tm data;
    string hora;
    vector<string> convidados;

    // Método para validar valores
    double validarValor(double valor, const string& campo);

public:
    /**
     * Construtor da classe Festa.
     * 
     * @param idFesta     Identificador único da festa (32 dígitos numéricos).
     * @param idCasamento Identificador do casamento ao qual a festa pertence.
     * @param endereco    Endereço da festa.
     * @param valorFesta  Valor total gasto na organização da festa.
     * @param numParcelas Número de parcelas em que o valor da festa foi dividido.
     * @param data        Data da festa (dd/mm/aaaa).
     * @param hora        Horário que a festa começa (hh:mm).
     * @param convidados  Lista de nomes dos convidados.
     */
    Festa(const string& idFesta, const string& idCasamento, const string& endereco,
          double valorFesta, int numParcelas, const tm& data, const string& hora,
          const vector<string>& convidados);

    // Getters
    string getIdFesta() const;
    string getIdCasamento() const;
    string getEndereco() const;
    double getValorFesta() const;
    int getNumParcelas() const;
    double getValorParcela() const;
    tm getData() const;
    string getHora() const;
    vector<string> getConvidados() const;

    // Setters
    void setEndereco(const string& endereco);
    void setValorFesta(double valorFesta);
    void setHora(const string& hora);

    // Métodos adicionais
    void adicionarConvidado(const string& convidado);
    void removerConvidado(const string& convidado);

    // Sobrecarga do operador <<
    friend ostream& operator<<(ostream& os, const Festa& festa);
};

}

#endif
