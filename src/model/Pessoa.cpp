#include "Pessoa.hpp"
#include <algorithm>

namespace model {

Pessoa::Pessoa(const string& nome, const string& telefone, const string& endereco, const string& idPessoa)
    : idPessoa(idPessoa), nome(nome), telefone(telefone), endereco(endereco) {
    if (idPessoa.empty() || idPessoa.size() != 32) {
        throw DataInconsistencyException("ID inválido! Deve conter exatamente 32 dígitos numéricos.");
    }
    if (nome.empty()) {
        throw DataInconsistencyException("Nome não pode ser vazio.");
    }
    if (telefone.empty()) {
        throw DataInconsistencyException("Telefone não pode ser vazio.");
    }
    if (endereco.empty()) {
        throw DataInconsistencyException("Endereço não pode ser nulo.");
    }
}

// Getters
string Pessoa::getIdPessoa() const {
    return idPessoa;
}

string Pessoa::getNome() const {
    return nome;
}

string Pessoa::getTelefone() const {
    return telefone;
}

string Pessoa::getEndereco() const {
    return endereco;
}

// Setters
void Pessoa::setNome(const string& nome) {
    if (nome.empty()) {
        throw DataInconsistencyException("Nome não pode ser vazio.");
    }
    this->nome = nome;
}

void Pessoa::setTelefone(const string& telefone) {
    if (telefone.empty()) {
        throw DataInconsistencyException("Telefone não pode ser vazio.");
    }
    this->telefone = telefone;
}

void Pessoa::setEndereco(const string& endereco) {
    if (endereco.empty()) {
        throw DataInconsistencyException("Endereço não pode ser nulo.");
    }
    this->endereco = endereco;
}

} // namespace model
