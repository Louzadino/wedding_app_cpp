#include "PessoaFisica.hpp"
#include <regex>
#include <iomanip>

namespace model {

PessoaFisica::PessoaFisica(const string& nome, const string& telefone, const string& endereco,
                           const string& cpf, const tm& dataNascimento, const Financeiro& financeiro,
                           const string& idPessoa)
    : Pessoa(nome, telefone, endereco, idPessoa), cpf(cpf), dataNascimento(dataNascimento), financeiro(financeiro) {
    regex regexCpf(R"(^(?:\d{3}\.\d{3}\.\d{3}-\d{2})$)");
    if (!regex_match(cpf, regexCpf)) {
        throw DataInconsistencyException("CPF inválido! Deve estar no formato 000.000.000-00.");
    }
}

string PessoaFisica::getCpf() const {
    return cpf;
}

tm PessoaFisica::getDataNascimento() const {
    return dataNascimento;
}

Financeiro PessoaFisica::getFinanceiro() const {
    return financeiro;
}

void PessoaFisica::setFinanceiro(const Financeiro& financeiro) {
    this->financeiro = financeiro;
}

ostream& operator<<(ostream& os, const PessoaFisica& pessoa) {
    os << "PessoaFisica{ID='" << pessoa.getIdPessoa()
       << "', Nome='" << pessoa.getNome()
       << "', Telefone='" << pessoa.getTelefone()
       << "', CPF='" << pessoa.getCpf()
       << "', DataNascimento=" 
       << put_time(&pessoa.dataNascimento, "%d/%m/%Y")
       << ", Endereço='" << pessoa.getEndereco()
       << "', Financeiro=" << pessoa.getFinanceiro() << "}";
    return os;
}
} // namespace model