#include "TarefaRepository.hpp"
#include "util/CSVReader.hpp"
#include "exception/DataInconsistencyException.hpp"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <locale>
#include <stdexcept>

using namespace std;
using namespace exception;

namespace repository {

TarefaRepository::TarefaRepository() {}

// 🔹 Destrutor (Libera memória das tarefas e compras associadas)
TarefaRepository::~TarefaRepository() {
    for (auto& par : tarefas) {
        if (par.second != nullptr) {  // Verifica se a tarefa existe
            if (par.second->getCompra() != nullptr) { 
                delete par.second->getCompra(); // Libera a compra associada à tarefa
                par.second->setCompra(nullptr); // Remove a referência da compra da tarefa
            }
            delete par.second; // Libera a própria tarefa
        }
    }
    tarefas.clear(); // Remove todos os elementos do mapa
}

void TarefaRepository::adicionar(Tarefa* tarefa) {
    if (tarefa == nullptr) {
        throw DataInconsistencyException("A tarefa não pode ser nula.");
    }
    if (tarefas.find(tarefa->getIdTarefa()) != tarefas.end()) {
        throw DataInconsistencyException("Já existe uma tarefa com este ID no repositório.");
    }
    tarefas[tarefa->getIdTarefa()] = tarefa;
}

void TarefaRepository::remover(Tarefa* tarefa) {
    if (tarefa == nullptr) {
        throw DataInconsistencyException("A tarefa não pode ser nula.");
    }
    if (tarefas.find(tarefa->getIdTarefa()) == tarefas.end()) {
        throw DataInconsistencyException("A tarefa não existe no repositório.");
    }
    tarefas.erase(tarefa->getIdTarefa());
}

vector<Tarefa*> TarefaRepository::listar() const {
    vector<Tarefa*> lista;
    for (const auto& item : tarefas) {
        lista.push_back(item.second);
    }
    return lista;
}

Tarefa* TarefaRepository::buscarPorId(const string& id) const {
    if (id.empty()) {
        throw DataInconsistencyException("O ID não pode ser nulo ou vazio.");
    }
    auto it = tarefas.find(id);
    if (it != tarefas.end()) {
        return it->second;
    }
    return nullptr;
}

void TarefaRepository::carregarDados(const string& caminhoArquivo, LarRepository& larRepo, PessoaRepository& pessoaRepo) {
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

        string idTarefa = campos[0];

        // Verifica se o ID já existe no repositório
        if (tarefas.find(idTarefa) != tarefas.end()) {
            throw DataInconsistencyException("ID repetido " + idTarefa + " na classe Tarefa.");
        }

        string idLar = campos[1];
        string idPrestador = campos[2];
        
        // Converte a data usando o formato dd/MM/yyyy
        struct tm dataInicio = {};
        istringstream ss(campos[3]);
        ss >> get_time(&dataInicio, "%d/%m/%Y");
        if (ss.fail()) {
            throw DataInconsistencyException("Erro ao converter data para tarefa com ID " + idTarefa);
        }

        int prazoEntrega = stoi(campos[4]);
        
        // Converte valor usando o locale brasileiro
        istringstream valorStream(campos[5]);
        valorStream.imbue(brasilianLocale);
        double valorPrestador;
        valorStream >> valorPrestador;
        if (valorStream.fail()) {
            throw DataInconsistencyException("Erro ao converter valor para tarefa com ID " + idTarefa);
        }

        int numParcelas = stoi(campos[6]);

        // 🔹 Validação: Verifica se o ID do Lar existe
        Lar* lar = larRepo.buscarPorId(idLar);
        if (lar == nullptr) {
            throw DataInconsistencyException("ID(s) de Lar " + idLar + " não cadastrado na Tarefa de ID " + idTarefa + ".");
        }

        // 🔹 Validação: Verifica se o ID do Prestador de Serviço existe
        Pessoa* prestador = pessoaRepo.buscarPorId(idPrestador);
        if (prestador == nullptr) {
            throw DataInconsistencyException("ID(s) de Prestador de Serviço " + idPrestador + " não cadastrado na Tarefa de ID " + idTarefa + ".");
        }

        // Cria e adiciona a nova tarefa ao repositório
        Tarefa* tarefa = new Tarefa(idTarefa, idLar, idPrestador, dataInicio, prazoEntrega, valorPrestador, numParcelas, nullptr);
        adicionar(tarefa);
    }
}

} // namespace repository