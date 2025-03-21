#include "Loja.hpp"
#include <sstream>

using namespace std;

namespace model {

// Construtor
Loja::Loja(const string& nome, const string& telefone, const string& endereco,
           const string& cnpj, const string& idPessoa)
    : PessoaJuridica(nome, telefone, endereco, cnpj, idPessoa) {}

// Sobrecarga do operador <<
ostream& operator<<(ostream& os, const Loja& loja) {
    os << "Loja{ID='" << loja.getIdPessoa()
       << "', Nome='" << loja.getNome()
       << "', Telefone='" << loja.getTelefone()
       << "', CNPJ='" << loja.getCnpj()
       << "', Endereço='" << loja.getEndereco()
       << "'}";
    return os;
}

} // namespace model