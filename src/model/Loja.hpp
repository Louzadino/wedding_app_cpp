#ifndef LOJA_H
#define LOJA_H

#include <string>
#include "PessoaJuridica.hpp"

using namespace std;

namespace model {

/**
 * Classe que representa uma loja no sistema.
 * A loja é sempre uma pessoa jurídica.
 */
class Loja : public PessoaJuridica {
public:
    /**
     * Construtor da classe Loja.
     *
     * @param nome      Nome da loja.
     * @param telefone  Telefone de contato da loja.
     * @param endereco  Endereço da loja.
     * @param cnpj      CNPJ da loja (14 dígitos numéricos).
     * @param idPessoa  Identificador da loja (mesmo que PessoaJuridica).
     */    
    Loja(const string& nome, const string& telefone, const string& endereco,
         const string& cnpj, const string& idPessoa);

    // Sobrecarga do operador <<
    friend ostream& operator<<(ostream& os, const Loja& loja);
};

} // namespace model

#endif
