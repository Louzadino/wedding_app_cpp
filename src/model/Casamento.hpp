#ifndef CASAMENTO_H
#define CASAMENTO_H

#include <string>
#include <ctime>
#include "Casal.hpp"
#include "Festa.hpp"
#include "exception/DataInconsistencyException.hpp"
#include "util/DateUtils.hpp"

using namespace std;

namespace model {

/**
 * Classe que representa um casamento.
 */
class Casamento {

private:
    string idCasamento;
    Casal* casal;
    tm dataCasamento{};
    string horaCasamento;
    string localCerimonia;
    Festa* festa;

public:
    /**
     * Construtor para casamento sem festa.
     *
     * @param idCasamento    Identificador único do casamento (32 dígitos numéricos).
     * @param casal          Casal que está se casando.    
     * @param dataCasamento  Data do casamento.
     * @param horaCasamento  Hora do casamento (hh:mm).
     * @param localCerimonia Local da cerimônia.
     */
    Casamento(const string& idCasamento, Casal* casal,
              const tm& dataCasamento, const string& horaCasamento,
              const string& localCerimonia);

    // Construtor para casamento com ou sem festa
    Casamento(const string& idCasamento, Casal* casal,
              const tm& dataCasamento, const string& horaCasamento,
              const string& localCerimonia, Festa* festa);

    // Getters
    string getIdCasamento() const;
    Casal* getCasal() const;
    tm getDataCasamento() const;
    string getHoraCasamento() const;
    string getLocalCerimonia() const;
    Festa* getFesta() const;

    // Setters
    void setDataCasamento(const tm& dataCasamento);
    void setHoraCasamento(const string& horaCasamento);
    void setLocalCerimonia(const string& localCerimonia);
    void setFesta(Festa* festa);

    // Sobrecarga de operador << para impressão
    friend ostream& operator<<(ostream& os, const Casamento& casamento);
};

} // namespace model

#endif
