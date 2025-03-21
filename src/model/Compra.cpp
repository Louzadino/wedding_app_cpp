#include "Compra.hpp"

using namespace std;

namespace model {
    class Compra;
}

Compra::Compra(const string& idCompra, const string& idLoja,
               const string& idTarefa, const string& nomeProduto,
               int quantidade, double valorUnitario, int numParcelas)
    : idCompra(idCompra), idLoja(idLoja), idTarefa(idTarefa),
      nomeProduto(nomeProduto), quantidade(quantidade),
      valorUnitario(valorUnitario), numParcelas(numParcelas) {

    // Validações
    if (idCompra.empty() || !regex_match(idCompra, regex("\\d{32}"))) {
        throw invalid_argument("O ID da compra deve ter exatamente 32 dígitos numéricos.");
    }
    if (idLoja.empty() || !regex_match(idLoja, regex("\\d{32}"))) {
        throw invalid_argument("O ID da loja deve ter exatamente 32 dígitos numéricos.");
    }
    if (idTarefa.empty() || !regex_match(idTarefa, regex("\\d{32}"))) {
        throw invalid_argument("O ID da tarefa deve ter exatamente 32 dígitos numéricos.");
    }
    if (nomeProduto.empty()) {
        throw invalid_argument("O nome do produto não pode ser vazio.");
    }
    if (quantidade <= 0) {
        throw invalid_argument("A quantidade de itens comprados deve ser maior que zero.");
    }
    if (valorUnitario <= 0) {
        throw invalid_argument("O valor unitário dos itens comprados deve ser maior que zero.");
    }
    if (numParcelas <= 0) {
        throw invalid_argument("O número de parcelas deve ser maior que zero.");
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
    os << "Compra{"
       << "ID='" << compra.idCompra << "', "
       << "ID Loja='" << compra.idLoja << "', "
       << "ID Tarefa='" << compra.idTarefa << "', "
       << "Produto='" << compra.nomeProduto << "', "
       << "Quantidade=" << compra.quantidade << ", "
       << "Valor Unitário=" << compra.valorUnitario << ", "
       << "Parcelas=" << compra.numParcelas
       << "}";
    return os;
}
