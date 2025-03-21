#include "TarefaRepository.hpp"
#include "util/CSVReader.hpp"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <locale>
#include <stdexcept>

using namespace std;

namespace repository {

TarefaRepository::TarefaRepository() {}

TarefaRepository::~TarefaRepository() {
    for (const auto& item : tarefas) {
        delete item.second;
    }
}

void TarefaRepository::adicionar(Tarefa* tarefa) {
    if (tarefa == nullptr) {
        throw invalid_argument("A tarefa não pode ser nula.");
    }
    if (tarefas.find(tarefa->getIdTarefa()) != tarefas.end()) {
        throw invalid_argument("Já existe uma tarefa com este ID no repositório.");
    }
    tarefas[tarefa->getIdTarefa()] = tarefa;
}

void TarefaRepository::remover(Tarefa* tarefa) {
    if (tarefa == nullptr) {
        throw invalid_argument("A tarefa não pode ser nula.");
    }
    if (tarefas.find(tarefa->getIdTarefa()) == tarefas.end()) {
        throw invalid_argument("A tarefa não existe no repositório.");
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
        throw invalid_argument("O ID não pode ser nulo ou vazio.");
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
            throw invalid_argument("ID repetido " + idTarefa + " na classe Tarefa.");
        }

        string idLar = campos[1];
        string idPrestador = campos[2];
        
        // Converte a data usando o formato dd/MM/yyyy
        struct tm dataInicio = {};
        istringstream ss(campos[3]);
        ss >> get_time(&dataInicio, "%d/%m/%Y");
        if (ss.fail()) {
            throw invalid_argument("Erro ao converter data para tarefa com ID " + idTarefa);
        }

        int prazoEntrega = stoi(campos[4]);
        
        // Converte valor usando o locale brasileiro
        istringstream valorStream(campos[5]);
        valorStream.imbue(brasilianLocale);
        double valorPrestador;
        valorStream >> valorPrestador;
        if (valorStream.fail()) {
            throw invalid_argument("Erro ao converter valor para tarefa com ID " + idTarefa);
        }

        int numParcelas = stoi(campos[6]);

        // 🔹 Validação: Verifica se o ID do Lar existe
        Lar* lar = larRepo.buscarPorId(idLar);
        if (lar == nullptr) {
            throw invalid_argument("ID(s) de Lar " + idLar + " não cadastrado na Tarefa de ID " + idTarefa + ".");
        }

        // 🔹 Validação: Verifica se o ID do Prestador de Serviço existe
        Pessoa* prestador = pessoaRepo.buscarPorId(idPrestador);
        if (prestador == nullptr) {
            throw invalid_argument("ID(s) de Prestador de Serviço " + idPrestador + " não cadastrado na Tarefa de ID " + idTarefa + ".");
        }

        // Cria e adiciona a nova tarefa ao repositório
        Tarefa* tarefa = new Tarefa(idTarefa, idLar, idPrestador, dataInicio, prazoEntrega, valorPrestador, numParcelas, nullptr);
        adicionar(tarefa);
    }
}

} // namespace repository