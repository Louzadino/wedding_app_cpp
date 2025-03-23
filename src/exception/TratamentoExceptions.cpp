#include "TratamentoExceptions.hpp"

#include <iostream>
#include <fstream>
#include <filesystem>

using namespace std;
using namespace exception;

// 🔹 Construtor para erro de entrada e saída
TratamentoExceptions::TratamentoExceptions(ios_base::failure* mensagemIO)
    : mensagemIO(mensagemIO), mensagemDadoInconsistente(nullptr), mensagemParsing(nullptr) {}

// 🔹 Construtor para erro de dados inconsistentes
TratamentoExceptions::TratamentoExceptions(runtime_error* mensagemDadoInconsistente)
    : mensagemDadoInconsistente(mensagemDadoInconsistente), mensagemIO(nullptr), mensagemParsing(nullptr) {}

// 🔹 Construtor para erro de parsing
TratamentoExceptions::TratamentoExceptions(invalid_argument* mensagemParsing)
    : mensagemParsing(mensagemParsing), mensagemDadoInconsistente(nullptr), mensagemIO(nullptr) {}

// 🔹 Destrutor
TratamentoExceptions::~TratamentoExceptions() {
    delete mensagemDadoInconsistente;
    delete mensagemIO;
    delete mensagemParsing;
}

// 🔹 Escreve os arquivos de saída com base no diretório recebido
void TratamentoExceptions::escreveDadosInconsistentesException(const string& diretorioBase) {
    // Caminhos dos arquivos com base no diretório recebido
    string caminhoArquivoRelatorio1 = diretorioBase + "/saida/1-planejamento.csv";
    string caminhoArquivoRelatorio2 = diretorioBase + "/saida/2-estatisticas-prestadores.csv";
    string caminhoArquivoRelatorio3 = diretorioBase + "/saida/3-estatisticas-casais.csv";

    try {
        ofstream arquivoRelatorio1(caminhoArquivoRelatorio1);
        arquivoRelatorio1.close();
    } catch (const ios_base::failure& ex) {
        cout << "Erro ao escrever no arquivo 1-planejamento.csv: " << ex.what() << endl;
    }

    try {
        ofstream arquivoRelatorio2(caminhoArquivoRelatorio2);
        arquivoRelatorio2.close();
    } catch (const ios_base::failure& ex) {
        cout << "Erro ao escrever no arquivo 2-estatisticas-prestadores.csv: " << ex.what() << endl;
    }

    try {
        ofstream arquivoRelatorio3(caminhoArquivoRelatorio3);
        arquivoRelatorio3.close();
    } catch (const ios_base::failure& ex) {
        cout << "Erro ao escrever no arquivo 3-estatisticas-casais.csv: " << ex.what() << endl;
    }

    // 🔹 Exibe a mensagem da exceção
    if (mensagemDadoInconsistente != nullptr) {
        cout << mensagemDadoInconsistente->what() << endl;
    } else if (mensagemIO != nullptr) {
        cout << mensagemIO->what() << endl;
    } else if (mensagemParsing != nullptr) {
        cout << mensagemParsing->what() << endl;
    }
}