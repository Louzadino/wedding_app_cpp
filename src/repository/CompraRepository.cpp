#include "CompraRepository.hpp"
#include "util/CSVReader.hpp"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <locale>
#include <stdexcept>

using namespace std;

namespace repository {

// Construtor
CompraRepository::CompraRepository() {}

// Destrutor
CompraRepository::~CompraRepository() {
    // Percorre todas as compras e desaloca a memória
    for (auto& par : compras) {
        delete par.second;  // Libera a memória de cada objeto Compra*
    }
    
    compras.clear(); // Agora limpa o mapa corretamente

    // Verifica se ainda há compras (debug)
    if (!compras.empty()) {
        cerr << "⚠️ Aviso: O repositório de compras ainda contém itens após desalocação!" << endl;
    }
}


void CompraRepository::adicionar(Compra* compra) {
    if (compra == nullptr) {
        throw invalid_argument("A compra não pode ser nula.");
    }
    if (compras.find(compra->getIdCompra()) != compras.end()) {
        throw invalid_argument("Já existe uma compra com este ID no repositório.");
    }
    compras[compra->getIdCompra()] = compra;
}

void CompraRepository::remover(Compra* compra) {
    if (compra == nullptr) {
        throw invalid_argument("A compra não pode ser nula.");
    }
    if (compras.find(compra->getIdCompra()) == compras.end()) {
        throw invalid_argument("A compra não existe no repositório.");
    }
    compras.erase(compra->getIdCompra());
}

vector<Compra*> CompraRepository::listar() const {
    vector<Compra*> lista;
    for (const auto& item : compras) {
        lista.push_back(item.second);
    }
    return lista;
}

Compra* CompraRepository::buscarPorId(const string& id) const {
    if (id.empty()) {
        throw invalid_argument("O ID não pode ser nulo ou vazio.");
    }
    auto it = compras.find(id);
    if (it != compras.end()) {
        return it->second;
    }
    return nullptr;
}

void CompraRepository::carregarDados(const string& caminhoArquivo, TarefaRepository& tarefaRepo, PessoaRepository& pessoaRepo) {
    vector<vector<string>> linhas = CSVReader::lerCSV(caminhoArquivo);

    locale brasilianLocale("pt_BR.UTF-8");

    for (const auto& campos : linhas) {
        if (campos.size() < 7) {
            cout << "Linha inválida encontrada, ignorando: ";
            for (const auto& campo : campos) {
                cout << campo << ";";
            }
            cout << endl;
            continue;
        }

        string idCompra = campos[0];

        // Verifica se o ID já existe no repositório
        if (compras.find(idCompra) != compras.end()) {
            throw invalid_argument("ID repetido " + idCompra + " na classe Compra.");
        }

        string idTarefa = campos[1];
        string idLoja = campos[2];
        string nomeProduto = campos[3];
        int qtdProduto = stoi(campos[4]);

        // Converte o valor usando o locale brasileiro
        istringstream valorStream(campos[5]);
        valorStream.imbue(brasilianLocale);
        double valorUnitario;
        valorStream >> valorUnitario;
        if (valorStream.fail()) {
            throw invalid_argument("Erro ao converter valor unitário para compra com ID " + idCompra);
        }

        int numParcelas = stoi(campos[6]);

        // 🔹 Validação: Verifica se o ID da Tarefa existe
        model::Tarefa* tarefa = tarefaRepo.buscarPorId(idTarefa);
        if (tarefa == nullptr) {
            throw invalid_argument("ID(s) de Tarefa " + idTarefa + " não cadastrado na compra de ID " + idCompra + ".");
        }

        // 🔹 Validação: Verifica se a Loja existe e se é de fato uma Loja
        Pessoa* pessoa = pessoaRepo.buscarPorId(idLoja);
        if (pessoa == nullptr) {
            throw invalid_argument("ID(s) de Loja " + idLoja + " não cadastrado na compra de ID " + idCompra + ".");
        }

        // Verifica se é uma Pessoa Jurídica e Loja
        PessoaJuridica* pessoaJuridica = dynamic_cast<PessoaJuridica*>(pessoa);
        Loja* loja = dynamic_cast<Loja*>(pessoaJuridica);
        if (pessoaJuridica != nullptr && loja == nullptr) {
            throw invalid_argument("ID " + idLoja + " da compra de ID " + idCompra + " não se refere a uma Loja, mas a uma PJ.");
        }

        // Cria uma nova compra e adiciona ao repositório
        Compra* compra = new Compra(idCompra, idLoja, idTarefa, nomeProduto, qtdProduto, valorUnitario, numParcelas);
        adicionar(compra);
    }
}

} // namespace repository