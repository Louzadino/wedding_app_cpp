#ifndef COMPRA_REPOSITORY_H
#define COMPRA_REPOSITORY_H

#include <string>
#include <map>
#include <vector>
#include "model/Compra.hpp"
#include "TarefaRepository.hpp"
#include "PessoaRepository.hpp"

using namespace std;

namespace repository {

class CompraRepository {

private:
    std::map<std::string, Compra*> compras;

public:
    CompraRepository();
    ~CompraRepository();

    void adicionar(Compra* compra);
    void remover(Compra* compra);
    std::vector<Compra*> listar() const;
    Compra* buscarPorId(const std::string& id) const;
    void carregarDados(const std::string& caminhoArquivo, TarefaRepository& tarefaRepo, PessoaRepository& pessoaRepo);
};

}

#endif
