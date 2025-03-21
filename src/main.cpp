#include <iostream>
#include <locale>
#include <ctime>

#include "model/Pessoa.hpp"
#include "model/PessoaFisica.hpp"
#include "model/PessoaJuridica.hpp"
#include "model/Loja.hpp"
#include "model/Casal.hpp"
#include "model/Lar.hpp"
#include "model/Financeiro.hpp"
#include "model/Endereco.hpp"
#include "model/Casamento.hpp"
#include "model/Festa.hpp"
#include "model/Compra.hpp"
#include "model/Tarefa.hpp"
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

        Casal c1 = Casal("01234567890123456789012345678901", 
                        "01234567890123456789012345678902", 
                        "01234567890123456789012345678903", 
                        "01234567890123456789012345678904");

        Endereco end1 = Endereco("Rua 1", 1, "Casa 1");

        // Teste com dados válidos
        Lar l1 = Lar("01234567890123456789012345678905", c1, end1);

        // Printa os dados da pessoa
        cout << l1 << endl;

    } catch (const DataInconsistencyException& e) {
        cerr << "Exceção capturada: " << e.what() << endl;
    } catch (const std::runtime_error& e) {
        cerr << "Exceção capturada: " << e.what() << endl;
    }

    return 0;
}
