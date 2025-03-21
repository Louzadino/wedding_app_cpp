#include <iostream>
#include <locale>
#include <ctime>

#include "model/Pessoa.hpp"
#include "model/PessoaFisica.hpp"
#include "model/PessoaJuridica.hpp"
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
        PessoaJuridica p1 = PessoaJuridica("Empresa 1", "12.123.123/1234-22", "Rua 1", "00.000.000/0000-00", "01234567890123456789012345678901");

        // Printa os dados da pessoa
        cout << p1 << endl;

        // Teste com ID inválido
        PessoaJuridica p2 = PessoaJuridica("Empresa 2", "12.123.123/1234-22", "Rua 2", "00.000.000/0000-00", "2");
        
        cout << "Erro: deveria ter lançado exceção!" << endl;

    } catch (const DataInconsistencyException& e) {
        cerr << "Exceção capturada: " << e.what() << endl;
    } catch (const std::runtime_error& e) {
        cerr << "Exceção capturada: " << e.what() << endl;
    }

    return 0;
}
