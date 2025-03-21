#ifndef ENDERECO_H
#define ENDERECO_H

#include <string>
#include <stdexcept>
#include <iostream>

using namespace std;

namespace model {

class Endereco {

private:
    string rua;
    int numero;
    string complemento;

public:
    /**
     * Construtor da classe Endereco
     * 
     * @param rua           Nome da rua (não pode ser vazio).
     * @param numero        Número da casa (não pode ser vazio).
     * @param complemento   complemento do endereço (opcional).
     */
    Endereco(const string& rua, int numero, const string& complemento);

    // Getters
    string getRua() const;
    int getNumero() const;
    string getComplemento() const;

    // Setters
    void setRua(const string& rua);
    void setNumero(int numero);
    void setComplemento(const string& complemento);

    // Sobrecarga do operador << para imprimir o endereço
    friend ostream& operator<<(ostream& os, const Endereco& endereco);
};

}

#endif
