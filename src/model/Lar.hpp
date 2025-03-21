#ifndef LAR_H
#define LAR_H

#include <stdexcept>
#include <string>
#include <sstream>
#include "Casal.hpp"
#include "Endereco.hpp"
#include "exception/DataInconsistencyException.hpp"

using namespace std;

namespace model {

class Lar {

private:
    string idLar;
    Casal casal;
    Endereco endereco;

    // Método para validar ID
    void validarId(const string& id) const;

public:
    /**
     * Construtor da classe Lar.
     * 
     * @param idLar      Identificador único do lar (32 dígitos numéricos).
     * @param casal      Casal que mora no lar.
     * @param endereco   Endereço do lar.
     */
    Lar(const string& idLar, const Casal& casal, const Endereco& endereco);

    // Getters
    string getIdLar() const;
    Casal getCasal() const;
    Endereco getEndereco() const;

    // Setters
    void setEndereco(const Endereco& endereco);

    // Sobrecarga do operador <<
    friend ostream& operator<<(ostream& os, const Lar& lar);
};

}

#endif
