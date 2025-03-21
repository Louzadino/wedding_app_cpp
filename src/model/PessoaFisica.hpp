#ifndef PESSOAFISICA_H
#define PESSOAFISICA_H

#include "Pessoa.hpp"
#include "exception/DataInconsistencyException.hpp"
#include "util/DateUtils.hpp"
#include "model/Financeiro.hpp"
#include <string>
#include <stdexcept>
#include <regex>
#include <ctime>

using namespace std;
using namespace model;
using namespace exception;
using namespace util;

namespace model {

/**
 * Classe que representa uma pessoa física, herdando características de Pessoa.
 */
class PessoaFisica : public Pessoa {
private:
    string cpf;
    tm dataNascimento;
    Financeiro financeiro;

public:
    /**
     * Construtor da classe PessoaFisica.
     *
     * @param nome           Nome da pessoa.
     * @param telefone       Telefone da pessoa.
     * @param endereco       Endereço da pessoa.
     * @param cpf            CPF da pessoa (11 dígitos).
     * @param dataNascimento Data de nascimento da pessoa.
     * @param financeiro     Informações financeiras da pessoa.
     * @param idPessoa       Identificador único da pessoa.
     */
    PessoaFisica(const string& nome, const string& telefone, const string& endereco,
                 const string& cpf, const tm& dataNascimento, const Financeiro& financeiro,
                 const string& idPessoa);

    string getCpf() const;
    tm getDataNascimento() const;
    Financeiro getFinanceiro() const;

    void setFinanceiro(const Financeiro& financeiro);

    /**
     * Sobrecarga do operador de inserção em stream para a classe Pessoa.
     */    
    friend ostream& operator<<(ostream& os, const PessoaFisica& pessoa);
};
} // namespace model

#endif
