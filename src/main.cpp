#include <iostream>
#include <locale>
#include <ctime>

#include "model/Pessoa.hpp"
#include "model/PessoaFisica.hpp"
#include "model/PessoaJuridica.hpp"
#include "model/Loja.hpp"
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
        Loja l1 = Loja("Loja 1", "1234-1234", "Rua 1", "12.123.123/1234-22", "01234567890123456789012345678901");

        // Printa os dados da pessoa
        cout << l1 << endl;

        // Teste com ID inválido
        Loja l2 = Loja("Loja 2", "12.123.123/1234-1", "Rua 2", "12.123.123/1234-11", "1");
        
        cout << "Erro: deveria ter lançado exceção!" << endl;

    } catch (const DataInconsistencyException& e) {
        cerr << "Exceção capturada: " << e.what() << endl;
    } catch (const std::runtime_error& e) {
        cerr << "Exceção capturada: " << e.what() << endl;
    }

    return 0;
}
