#ifndef CASAMENTO_REPOSITORY_H
#define CASAMENTO_REPOSITORY_H

#include <string>
#include <map>
#include <vector>
#include "model/Festa.hpp"
#include "model/Casamento.hpp"
#include "PessoaRepository.hpp"
#include "FestaRepository.hpp"
#include "LarRepository.hpp"
#include "CasalRepository.hpp"

using namespace std;

namespace repository {

class CasalRepository;
class LarRepository;
class FestaRepository;

class CasamentoRepository {

private:
    map<string, Casamento*> casamentos;
    vector<string> IDs;

public:
    CasamentoRepository();
    ~CasamentoRepository();

    void adicionar(Casamento* casamento);
    void remover(Casamento* casamento);
    vector<Casamento*> listar() const;
    Casamento* buscarPorId(const string& id) const;
    void carregarDados(const string& caminhoArquivo, PessoaRepository& pessoaRepo, FestaRepository& festaRepo,
                       LarRepository& larRepo, CasalRepository& casalRepo);
    void recarregarFestas(FestaRepository& festaRepo);

    map<string, Casamento*> getCasamentos() const;
    vector<string> getIDs() const;
};

}

#endif