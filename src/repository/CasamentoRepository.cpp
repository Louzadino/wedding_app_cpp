#include "CasamentoRepository.hpp"
#include "util/CSVReader.hpp"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <locale>
#include <stdexcept>

using namespace std;

namespace repository {

// Construtor 
CasamentoRepository::CasamentoRepository() {}

// Destrutor
CasamentoRepository::~CasamentoRepository() {
    for (auto& par : casamentos) {
        if (par.second != nullptr) {
            delete par.second;
        }
    }
    casamentos.clear();
}


void CasamentoRepository::adicionar(Casamento* casamento) {
    if (casamento == nullptr) {
        throw invalid_argument("O casamento não pode ser nulo.");
    }
    if (casamentos.find(casamento->getIdCasamento()) != casamentos.end()) {
        throw invalid_argument("Já existe um casamento com este ID no repositório.");
    }
    casamentos[casamento->getIdCasamento()] = casamento;
}

void CasamentoRepository::remover(Casamento* casamento) {
    if (casamento == nullptr) {
        throw invalid_argument("O casamento não pode ser nulo.");
    }
    if (casamentos.find(casamento->getIdCasamento()) == casamentos.end()) {
        throw invalid_argument("O casamento não existe no repositório.");
    }
    casamentos.erase(casamento->getIdCasamento());
}

vector<Casamento*> CasamentoRepository::listar() const {
    vector<Casamento*> lista;
    for (const auto& item : casamentos) {
        lista.push_back(item.second);
    }
    return lista;
}

Casamento* CasamentoRepository::buscarPorId(const string& id) const {
    if (id.empty()) {
        throw invalid_argument("O ID não pode ser nulo ou vazio.");
    }
    auto it = casamentos.find(id);
    if (it != casamentos.end()) {
        return it->second;
    }
    return nullptr;
}

void CasamentoRepository::carregarDados(const string& caminhoArquivo, PessoaRepository& pessoaRepo, 
                                        FestaRepository& festaRepo, LarRepository& larRepo, 
                                        CasalRepository& casalRepo) {
    vector<vector<string>> linhas = CSVReader::lerCSV(caminhoArquivo);

    locale brasilianLocale("pt_BR.UTF-8");

    for (const auto& campos : linhas) {
        if (campos.size() < 6) {
            cout << "Linha inválida encontrada, ignorando: ";
            for (const auto& campo : campos) {
                cout << campo << ";";
            }
            cout << endl;
            continue;
        }

        string idCasamento = campos[0];

        if (casamentos.find(idCasamento) != casamentos.end()) {
            throw invalid_argument("ID repetido " + idCasamento + " na classe Casamento.");
        }

        string idPessoa1 = campos[1];
        string idPessoa2 = campos[2];

        struct tm data = {};
        istringstream dataStream(campos[3]);
        dataStream >> get_time(&data, "%d/%m/%Y");
        if (dataStream.fail()) {
            throw invalid_argument("Erro ao converter data para casamento com ID " + idCasamento);
        }

        string hora = campos[4];
        string local = campos[5];

        bool pessoa1Existe = pessoaRepo.buscarPorId(idPessoa1) != nullptr;
        bool pessoa2Existe = pessoaRepo.buscarPorId(idPessoa2) != nullptr;

        if (!pessoa1Existe && !pessoa2Existe) {
            throw invalid_argument("ID(s) de Pessoa " + idPessoa1 + " " + idPessoa2 +
                                   " não cadastrado no Casamento de ID " + idCasamento + ".");
        }
        if (!pessoa1Existe) {
            throw invalid_argument("ID de Pessoa " + idPessoa1 +
                                   " não cadastrado no Casamento de ID " + idCasamento + ".");
        }
        if (!pessoa2Existe) {
            throw invalid_argument("ID de Pessoa " + idPessoa2 +
                                   " não cadastrado no Casamento de ID " + idCasamento + ".");
        }

        model::Festa* festa = festaRepo.buscarPorId(idCasamento);

        Casal* casal = casalRepo.buscarPorIdPessoa(idPessoa1);
        if (casal == nullptr) {
            casal = new Casal(idPessoa1, idPessoa2, idCasamento, "");
            casalRepo.adicionar(casal);
        } else if (casal->getIdCasamento().empty()) {
            casal->setIdCasamento(idCasamento);
        }

        Casamento* casamento = new Casamento(idCasamento, casal, data, hora, local, festa);
        adicionar(casamento);
        IDs.push_back(idCasamento);
    }
}

void CasamentoRepository::recarregarFestas(FestaRepository& festaRepo) {
    for (auto& item : casamentos) {
        Casamento* casamento = item.second;
        model::Festa* festa = festaRepo.buscarPorId(casamento->getIdCasamento());
        if (festa != nullptr) {
            casamento->setFesta(festa);
        }
    }
}

map<string, Casamento*> CasamentoRepository::getCasamentos() const {
    return casamentos;
}

vector<string> CasamentoRepository::getIDs() const {
    return IDs;
}

}
