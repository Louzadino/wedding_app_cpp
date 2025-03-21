#ifndef CASAL_REPOSITORY_H
#define CASAL_REPOSITORY_H

#include <string>
#include <vector>
#include "model/Casal.hpp"
#include "CasamentoRepository.hpp"
#include "LarRepository.hpp"

using namespace std;

namespace repository {

class LarRepository;
class CasamentoRepository;

class CasalRepository {

private:
    vector<Casal*> casais;

public:
    CasalRepository();
    ~CasalRepository();

    void adicionar(Casal* casal);
    void remover(Casal* casal);
    vector<Casal*> listar() const;
    void carregarCasais(CasamentoRepository& casamentoRepo, LarRepository& larRepo);

    Casal* buscarPorIdPessoa(const string& idPessoa) const;
    Casal* buscarPorIdLar(const string& idLar) const;
    Casal* buscarPorIdCasamento(const string& idCasamento) const;
};

}

#endif
