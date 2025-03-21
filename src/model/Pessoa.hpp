#ifndef PESSOA_H
#define PESSOA_H

#include <string>
#include <stdexcept>
#include <iostream>
#include "exception/DataInconsistencyException.hpp"

using namespace std;
using namespace exception;

namespace model {

/**
 * Classe que representa uma pessoa genérica no sistema.
 */
class Pessoa {
private:
    string idPessoa;
    string nome;
    string telefone;
    string endereco;

public:

    /**
     * Construtor da classe Pessoa.
     * 
     * @param nome     Nome da pessoa (não pode ser vazio).
     * @param telefone Telefone da pessoa (não pode ser vazio).
     * @param endereco Endereço da pessoa (não pode ser nulo).
     * @param idPessoa Identificador único da pessoa (32 dígitos).
     */
    Pessoa(const string& nome, const string& telefone, const string& endereco, const string& idPessoa);

    // Destrutor virtual
    virtual ~Pessoa() = default;

    string getIdPessoa() const;
    string getNome() const;
    string getTelefone() const;
    string getEndereco() const;

    void setNome(const string& nome);
    void setTelefone(const string& telefone);
    void setEndereco(const string& endereco);

    /**
     * Sobrecarga do operador de inserção em stream para a classe Pessoa.
     */
    friend ostream& operator<<(ostream& os, const Pessoa& pessoa);
};

}

#endif