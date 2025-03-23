#ifndef TRATAMENTO_EXCEPTIONS_H
#define TRATAMENTO_EXCEPTIONS_H

#include <stdexcept>
#include <string>
#include <fstream>
#include <iostream>

#include "exception/DataInconsistencyException.hpp"

using namespace std;

namespace exception {

/**
 * Classe responsável por tratar as exceções lançadas durante a execução do programa.
 */
class TratamentoExceptions {
private:
    DataInconsistencyException* mensagemParsing;

public:
    TratamentoExceptions(DataInconsistencyException* mensagemParsing);
    ~TratamentoExceptions();

    void escreveDadosInconsistentesException(const string& diretorioBase);
};

} // namespace exception

#endif