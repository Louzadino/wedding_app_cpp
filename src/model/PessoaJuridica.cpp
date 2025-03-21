#include "PessoaJuridica.hpp"
#include <regex>

using namespace std;

namespace model {

// Construtor
PessoaJuridica::PessoaJuridica(const string& nome, const string& telefone, const string& endereco,
                               const string& cnpj, const string& idPessoa)
    : Pessoa(nome, telefone, endereco, idPessoa) {

    regex regexCnpj(R"(^\d{2}\.\d{3}\.\d{3}/\d{4}-\d{2}$)");
    if (!regex_match(cnpj, regexCnpj)) {
        throw DataInconsistencyException("CNPJ inválido! Deve estar no formato 00.000.000/0000-00.");
    }

    this->cnpj = cnpj;
}

// Getter
string PessoaJuridica::getCnpj() const {
    return cnpj;
}

// Sobrecarga do operador <<
ostream& operator<<(ostream& os, const PessoaJuridica& pessoa) {
    os << "PessoaJuridica{ID='" << pessoa.getIdPessoa()
       << "', Nome='" << pessoa.getNome()
       << "', Telefone='" << pessoa.getTelefone()
       << "', CNPJ='" << pessoa.getCnpj()
       << "', Endereço='" << pessoa.getEndereco()
       << "'}";
    return os;
}

} // namespace model
