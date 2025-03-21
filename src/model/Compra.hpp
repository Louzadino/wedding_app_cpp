#ifndef COMPRA_H
#define COMPRA_H

#include <string>
#include <stdexcept>
#include <regex>
#include <iostream>

using namespace std;

namespace model {

class Compra {

private:
    string idCompra;
    string idLoja;
    string idTarefa;
    string nomeProduto;
    int quantidade;
    double valorUnitario;
    int numParcelas;

public:
    /**
    * Construtor da classe Compra.
    *
    * @param idCompra      Identificador único da compra (32 dígitos).
    * @param idLoja        Identificador da loja.
    * @param idTarefa      Identificador da tarefa associada.
    * @param nomeProduto   Nome do produto comprado.
    * @param quantidade    Quantidade de itens comprados.
    * @param valorUnitario Valor unitário dos itens comprados.
    * @param numParcelas   Número de parcelas do pagamento.
    */
    Compra(const string& idCompra, const string& idLoja,
           const string& idTarefa, const string& nomeProduto,
           int quantidade, double valorUnitario, int numParcelas);

    // Getters
    string getIdCompra() const;
    string getIdLoja() const;
    string getIdTarefa() const;
    string getNomeProduto() const;
    int getQuantidade() const;
    double getValorUnitario() const;
    int getNumParcelas() const;

    // Métodos adicionais
    double valorTotal() const;
    double getValorParcela() const;

    // Sobrecarga do operador << para impressão
    friend ostream& operator<<(ostream& os, const Compra& compra);
};

}

#endif
