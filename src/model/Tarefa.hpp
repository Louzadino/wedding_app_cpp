#ifndef TAREFA_H
#define TAREFA_H

#include "Compra.hpp"
#include <string>
#include <stdexcept>
#include <iostream>
#include <ctime>

using namespace std;

namespace model {
    class Tarefa;
    class Compra;
}

/**
 * Classe que representa uma tarefa associada à organização de um lar.
 */
class Tarefa {
private:
    string idTarefa;
    string idLar;
    string idPrestador;
    tm dataInicio;
    int prazoEntrega;
    double valorPrestador;
    int numParcelas;
    Compra* compra; // Pode ser nullptr

    tm adicionarDias(const tm& data, int dias) const;

public:
    /**
     * Construtor da classe Tarefa.
     *
     * @param idTarefa       Identificador único da tarefa (32 dígitos).
     * @param idLar          Identificador do lar associado (pode ser null).
     * @param idPrestador    Identificador do prestador de serviço.
     * @param dataInicio     Data de início da tarefa.
     * @param prazoEntrega   Número de dias para conclusão.
     * @param valorPrestador Valor cobrado pelo prestador.
     * @param numParcelas    Número de parcelas do pagamento.
     * @param compra         Compra de materiais associada à tarefa (pode ser null).
     */    
    Tarefa(const string& idTarefa, const string& idLar, const string& idPrestador,
           const tm& dataInicio, int prazoEntrega, double valorPrestador, int numParcelas, Compra* compra);

    // Destrutor
    ~Tarefa();

    // Getters
    string getIdTarefa() const;
    string getIdLar() const;
    string getIdPrestador() const;
    tm getDataInicio() const;
    int getPrazoEntrega() const;
    tm getDataEntrega() const;
    double getValorPrestador() const;
    int getNumParcelas() const;
    Compra* getCompra() const;

    // Setters
    void setCompra(Compra* compra);

    double getValorParcela() const;

    // Sobrecarga do operador <<
    friend ostream& operator<<(ostream& os, const Tarefa& tarefa);
};

#endif
