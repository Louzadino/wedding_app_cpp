#ifndef TAREFA_REPOSITORY_H
#define TAREFA_REPOSITORY_H

#include <string>
#include <map>
#include <vector>
#include <stdexcept>
#include "model/Tarefa.hpp"
#include "LarRepository.hpp"
#include "PessoaRepository.hpp"

using namespace std;

namespace repository {

class TarefaRepository {

private:
    map<string, Tarefa*> tarefas;

public:
    TarefaRepository();
    ~TarefaRepository();

    void adicionar(Tarefa* tarefa);
    void remover(Tarefa* tarefa);
    vector<Tarefa*> listar() const;
    Tarefa* buscarPorId(const string& id) const;
    void carregarDados(const string& caminhoArquivo, LarRepository& larRepo, PessoaRepository& pessoaRepo);
};

}

#endif
