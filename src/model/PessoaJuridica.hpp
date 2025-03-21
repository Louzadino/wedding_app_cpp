#ifndef PESSOAJURIDICA_H
#define PESSOAJURIDICA_H

#include "Pessoa.hpp"
#include "exception/DataInconsistencyException.hpp"
#include <string>
#include <stdexcept>
#include <regex>

using namespace std;

namespace model {

/**
 * Classe que representa uma pessoa jurídica no sistema.
 */
class PessoaJuridica : public Pessoa {
private:
    string cnpj;

public:
    /**
     * Construtor da classe PessoaJuridica.
     *
     * @param nome      Nome da empresa.
     * @param telefone  Telefone de contato.
     * @param endereco  Endereço da empresa.
     * @param cnpj      CNPJ da empresa (14 dígitos numéricos).
     * @param idPessoa  ID da pessoa jurídica.
     */
    PessoaJuridica(const string& nome, const string& telefone, const string& endereco,
                   const string& cnpj, const string& idPessoa);

    // Getter
    string getCnpj() const;

    // Sobrecarga do operador <<
    friend ostream& operator<<(ostream& os, const PessoaJuridica& pessoa);
};

} // namespace model

#endif
