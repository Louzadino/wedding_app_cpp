#include <iostream>
#include <locale>

#include "model/Pessoa.hpp"
#include "exception/DataInconsistencyException.hpp"

using namespace std;
using namespace model;
using namespace exception;

int main() {

    try {

        // Configura o locale padrão do sistema
        //std::locale::global(std::locale(""));
       // std::cout.imbue(std::locale());

        // Teste com dados válidos
        Pessoa p1("João Silva", "99999-9999", "Rua A, 123", "12345678901234567890123456789012");
        cout << "Pessoa criada com sucesso: " << p1.getNome() << endl;

        // Teste com ID inválido
        Pessoa p2("Maria Souza", "88888-8888", "Rua B, 456", "123"); 
        cout << "Erro: deveria ter lançado exceção!" << endl;

    } catch (const DataInconsistencyException& e) {
        cerr << "Exceção capturada: " << e.what() << endl;
    } catch (const std::runtime_error& e) {
        cerr << "Exceção capturada: " << e.what() << endl;
    }

    return 0;
}
