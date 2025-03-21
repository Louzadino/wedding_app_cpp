#ifndef LAR_REPOSITORY_H
#define LAR_REPOSITORY_H

#include <string>
#include <map>
#include <vector>
#include "model/Lar.hpp"
#include "PessoaRepository.hpp"
#include "CasalRepository.hpp"

using namespace std;

namespace repository {

class CasalRepository;

class LarRepository {

private:
    map<string, Lar*> lares;

public:
    LarRepository();
    ~LarRepository();

    void adicionar(Lar* lar);
    void remover(Lar* lar);
    vector<Lar*> listar() const;
    Lar* buscarPorId(const string& id) const;
    void carregarDados(const string& caminhoArquivo, PessoaRepository& pessoaRepo, CasalRepository& casalRepo);
};

}

#endif
