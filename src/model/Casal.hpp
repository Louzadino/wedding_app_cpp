#ifndef CASAL_H
#define CASAL_H

#include <string>

using namespace std;

namespace model {

class Casal {

private:
    string idPessoa1;
    string idPessoa2;
    string idCasamento;
    string idLar;

public:
    /**
     * Construtor da classe Casal.
     * 
     * @param idPessoa1   ID da primeira pessoa do casal.
     * @param idPessoa2   ID da segunda pessoa do casal.
     * @param idCasamento ID do casamento (pode ser null se não forem casados).
     * @param idLar       ID do lar (pode ser null se não morarem juntos).
     */
    Casal(const string& idPessoa1, const string& idPessoa2, 
          const string& idCasamento = "", const string& idLar = "");

    // Getters
    string getIdPessoa1() const;
    string getIdPessoa2() const;
    string getIdCasamento() const;
    string getIdLar() const;

    // Setters
    void setIdCasamento(const string& idCasamento);
    void setIdLar(const string& idLar);

        /**
     * Verifica se o casal possui um casamento registrado.
     * 
     * @return true se o casal tem um casamento, false caso contrário.
     */
    bool temCasamento() const;

    /**
     * Verifica se o casal possui um lar registrado.
     * 
     * @return true se o casal tem um lar, false caso contrário.
     */
    bool temLar() const;

    // Sobrecarga do operador << para impressão
    friend ostream& operator<<(ostream& os, const Casal& casal);
};

}

#endif
