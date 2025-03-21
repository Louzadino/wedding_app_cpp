#include <iostream>
#include <locale>
#include <ctime>

#include "model/Pessoa.hpp"
#include "model/PessoaFisica.hpp"
#include "model/Financeiro.hpp"
#include "util/DateUtils.hpp"
#include "exception/DataInconsistencyException.hpp"

using namespace std;
using namespace model;
using namespace exception;

int main() {

    try {

        // Configura o locale padrão do sistema
        // std::locale::global(std::locale(""));
        // std::cout.imbue(std::locale());

        // Teste com dados válidos
        PessoaFisica p1("Fulano", "99999-9990", "Rua B, 222", "123.456.789-01", 
                        {0, 0, 0, 1, 0, 100}, Financeiro(1000.0, 2000.0, 1500.0), "12345678901234567890123456789012");
        cout << "Pessoa criada com sucesso: " << p1.getNome() << endl;

        // Printa os dados da pessoa
        cout << p1 << endl;

        // Teste com ID inválido
        PessoaFisica p2("Fulano", "99999-9990", "Rua B, 222", 
                        "123", {0, 0, 0, 1, 0, 100}, Financeiro(1000.0, 2000.0, 1500.0), "12345678901234567890123456789012");
        cout << "Erro: deveria ter lançado exceção!" << endl;

    } catch (const DataInconsistencyException& e) {
        cerr << "Exceção capturada: " << e.what() << endl;
    } catch (const std::runtime_error& e) {
        cerr << "Exceção capturada: " << e.what() << endl;
    }

    return 0;
}
