#include "Compra.hpp"
#include "exception/DataInconsistencyException.hpp"

using namespace std;
using namespace exception;

namespace model {

Compra::Compra(const string& idCompra, const string& idLoja,
               const string& idTarefa, const string& nomeProduto,
               int quantidade, double valorUnitario, int numParcelas)
    : idCompra(idCompra), idLoja(idLoja), idTarefa(idTarefa),
      nomeProduto(nomeProduto), quantidade(quantidade),
      valorUnitario(valorUnitario), numParcelas(numParcelas) {

    // Validações
    if (idCompra.empty() || !regex_match(idCompra, regex("\\d{32}"))) {
        throw DataInconsistencyException("O ID da compra deve ter exatamente 32 dígitos numéricos.");
    }
    if (idLoja.empty() || !regex_match(idLoja, regex("\\d{32}"))) {
        throw DataInconsistencyException("O ID da loja deve ter exatamente 32 dígitos numéricos.");
    }
    if (idTarefa.empty() || !regex_match(idTarefa, regex("\\d{32}"))) {
        throw DataInconsistencyException("O ID da tarefa deve ter exatamente 32 dígitos numéricos.");
    }
    if (nomeProduto.empty()) {
        throw DataInconsistencyException("O nome do produto não pode ser vazio.");
    }
    if (quantidade <= 0) {
        throw DataInconsistencyException("A quantidade de itens comprados deve ser maior que zero.");
    }
    if (valorUnitario <= 0) {
        throw DataInconsistencyException("O valor unitário dos itens comprados deve ser maior que zero.");
    }
    if (numParcelas <= 0) {
        throw DataInconsistencyException("O número de parcelas deve ser maior que zero.");
    }
}

// Getters
string Compra::getIdCompra() const {
    return idCompra;
}

string Compra::getIdLoja() const {
    return idLoja;
}

string Compra::getIdTarefa() const {
    return idTarefa;
}

string Compra::getNomeProduto() const {
    return nomeProduto;
}

int Compra::getQuantidade() const {
    return quantidade;
}

double Compra::getValorUnitario() const {
    return valorUnitario;
}

int Compra::getNumParcelas() const {
    return numParcelas;
}

// Métodos adicionais
double Compra::valorTotal() const {
    return quantidade * valorUnitario;
}

double Compra::getValorParcela() const {
    return valorTotal() / numParcelas;
}

// Sobrecarga do operador << para impressão
ostream& operator<<(ostream& os, const Compra& compra) {
    os << "ID da compra: " << compra.idCompra << endl
       << "ID da loja: " << compra.idLoja << endl
       << "ID da tarefa: " << compra.idTarefa << endl
       << "Nome do produto: " << compra.nomeProduto << endl
       << "Quantidade: " << compra.quantidade << endl
       << "Valor unitário: " << compra.valorUnitario << endl
       << "Número de parcelas: " << compra.numParcelas << endl
       << "Valor total: " << compra.valorTotal() << endl
       << "Valor da parcela: " << compra.getValorParcela() << endl;
    return os;
}

} // namespace model
