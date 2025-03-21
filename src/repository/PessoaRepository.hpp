#ifndef PESSOA_REPOSITORY_H
#define PESSOA_REPOSITORY_H

#include <string>
#include <map>
#include <vector>
#include "model/Pessoa.hpp"
#include "model/PessoaFisica.hpp"
#include "model/PessoaJuridica.hpp"
#include "model/Loja.hpp"

using namespace std;

namespace repository {

class PessoaRepository {

private:
    map<string, Pessoa*> pessoas;
    map<string, string> cpfs;
    map<string, string> cnpjs;

public:
    PessoaRepository();
    ~PessoaRepository();

    void adicionar(Pessoa* pessoa);
    void remover(Pessoa* pessoa);
    vector<Pessoa*> listar() const;
    Pessoa* buscarPorId(const string& id) const;
    void carregarDadosDoCSV(const string& caminhoArquivo);
};

}

#endif
