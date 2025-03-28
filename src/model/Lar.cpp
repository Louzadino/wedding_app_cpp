#include "Lar.hpp"
#include "exception/DataInconsistencyException.hpp"
#include <algorithm>

using namespace std;
using namespace exception;

namespace model {

// Construtor
Lar::Lar(const string& idLar, Casal *casal, const Endereco& endereco)
    : idLar(idLar), casal(casal), endereco(endereco) {
    validarId(idLar);
}

// Validação do ID
void Lar::validarId(const string& id) const {
    if (id.empty() || id.length() != 32 || !all_of(id.begin(), id.end(), ::isdigit)) {
        throw DataInconsistencyException("O ID do lar deve conter exatamente 32 dígitos numéricos.");
    }
}

// Getters
string Lar::getIdLar() const {
    return idLar;
}

Casal* Lar::getCasal() const {
    return casal;
}

Endereco Lar::getEndereco() const {
    return endereco;
}

// Setters
void Lar::setEndereco(const Endereco& endereco) {
    this->endereco = endereco;
}

ostream& operator<<(ostream& os, const Lar& lar) {
    os << "ID: " << lar.idLar << endl;
    os << "Casal: " << lar.casal->getIdPessoa1() << " e " << lar.casal->getIdPessoa2() << endl;
    os << "Endereço: " << lar.endereco << endl;
    return os;
}

} // namespace model