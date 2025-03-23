#ifndef TRATAMENTO_EXCEPTIONS_H
#define TRATAMENTO_EXCEPTIONS_H

#include <stdexcept>
#include <string>
#include <fstream>
#include <iostream>

using namespace std;

namespace exception {

class TratamentoExceptions {
private:
    runtime_error* mensagemDadoInconsistente;
    ios_base::failure* mensagemIO;
    invalid_argument* mensagemParsing;

public:
    TratamentoExceptions(ios_base::failure* mensagemIO);
    TratamentoExceptions(runtime_error* mensagemDadoInconsistente);
    TratamentoExceptions(invalid_argument* mensagemParsing);
    ~TratamentoExceptions();

    void escreveDadosInconsistentesException(const string& diretorioBase);
};

} // namespace exception

#endif