#include "CasalRepository.hpp"
#include <stdexcept>
#include <algorithm>

using namespace std;

namespace repository {

// Construtor
CasalRepository::CasalRepository() {}

// Destrutor
CasalRepository::~CasalRepository() {
    
    for (auto& casal : casais) {
        delete casal;
    }
    casais.clear();
}

void CasalRepository::adicionar(Casal* casal) {
    if (casal == nullptr) {
        throw invalid_argument("O casal não pode ser nulo (para o repositório).");
    }
    casais.push_back(casal);
}

void CasalRepository::remover(Casal* casal) {
    if (casal == nullptr) {
        throw invalid_argument("O casal não pode ser nulo (para o repositório).");
    }
    auto it = find(casais.begin(), casais.end(), casal);
    if (it != casais.end()) {
        casais.erase(it);
    }
}

vector<Casal*> CasalRepository::listar() const {
    return casais;
}

void CasalRepository::carregarCasais(CasamentoRepository& casamentoRepo, LarRepository& larRepo) {
    // Iteração sobre os casamentos
    for (Casamento* casamento : casamentoRepo.listar()) {
        Casal* casal = casamento->getCasal();

        if (casal != nullptr) {
            adicionar(casal);
        } else {
            throw invalid_argument("Casal não encontrado para o casamento de ID " + casamento->getIdCasamento());
        }
    }

    // Iteração sobre os lares
    for (Lar* lar : larRepo.listar()) {
        Casal* casal = lar->getCasal();

        if (casal != nullptr) {
            auto it = find(casais.begin(), casais.end(), casal);
            if (it == casais.end()) {
                adicionar(casal);
            } else {
                throw invalid_argument("Casal já adicionado ao repositório.");
            }
        } else {
            throw invalid_argument("Casal não encontrado para o lar de ID " + lar->getIdLar());
        }
    }
}

Casal* CasalRepository::buscarPorIdPessoa(const string& idPessoa) const {
    for (Casal* casal : casais) {
        if (casal->getIdPessoa1() == idPessoa || casal->getIdPessoa2() == idPessoa) {
            return casal;
        }
    }
    return nullptr;
}

Casal* CasalRepository::buscarPorIdLar(const string& idLar) const {
    for (Casal* casal : casais) {
        if (casal->getIdLar() == idLar) {
            return casal;
        }
    }
    return nullptr;
}

Casal* CasalRepository::buscarPorIdCasamento(const string& idCasamento) const {
    for (Casal* casal : casais) {
        if (casal->getIdCasamento() == idCasamento) {
            return casal;
        }
    }
    return nullptr;
}

}
