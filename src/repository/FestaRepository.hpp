#ifndef FESTA_REPOSITORY_H
#define FESTA_REPOSITORY_H

#include <string>
#include <map>
#include <set>
#include <vector>
#include "model/Festa.hpp"
#include "CasamentoRepository.hpp"
#include "PessoaRepository.hpp"

using namespace std;

namespace repository {

class CasamentoRepository;

class FestaRepository {

private:
    map<string, Festa*> festas;
    set<string> casamentosComFesta;

public:
    FestaRepository();
    ~FestaRepository();

    void adicionar(Festa* festa);
    void remover(Festa* festa);
    vector<Festa*> listar() const;
    Festa* buscarPorId(const string& id) const;
    void carregarDados(const string& caminhoArquivo, CasamentoRepository& casamentoRepo, PessoaRepository& pessoaRepo);
};

} // namespace repository

#endif
