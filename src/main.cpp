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

#include "repository/CasamentoRepository.hpp"
#include "repository/CompraRepository.hpp"
#include "repository/FestaRepository.hpp"
#include "repository/LarRepository.hpp"
#include "repository/PessoaRepository.hpp"
#include "repository/TarefaRepository.hpp"
#include "repository/CasalRepository.hpp"

#include "util/DateUtils.hpp"
#include "util/CSVReader.hpp"

#include "exception/DataInconsistencyException.hpp"

using namespace std;

int main(int argc, char* argv[]) {

    if (argc < 2) {
        cerr << "Erro: Caminho do arquivo de entrada não especificado." << endl;
        return 1;
    }

    string caminhoArquivoEntrada = argv[1];

    // Definição dos caminhos dos arquivos CSV
    string caminhoArquivoPessoas = caminhoArquivoEntrada + "/pessoas.csv";
    string caminhoArquivoCasamento = caminhoArquivoEntrada + "/casamentos.csv";
    string caminhoArquivoCompra = caminhoArquivoEntrada + "/compras.csv";
    string caminhoArquivoFesta = caminhoArquivoEntrada + "/festas.csv";
    string caminhoArquivoLares = caminhoArquivoEntrada + "/lares.csv";
    string caminhoArquivoTarefa = caminhoArquivoEntrada + "/tarefas.csv";

    // Inicialização dos repositórios
    repository::PessoaRepository pessoaRepo;
    repository::LarRepository larRepo;
    repository::TarefaRepository tarefaRepo;
    repository::CompraRepository compraRepo;
    repository::CasamentoRepository casamentoRepo;
    repository::FestaRepository festaRepo;
    repository::CasalRepository casalRepo;

    try {
        // Configura o locale padrão do sistema
        // std::locale::global(std::locale(""));
        // std::cout.imbue(std::locale());

        // Carregar dados dos CSVs
        pessoaRepo.carregarDados(caminhoArquivoPessoas);
        larRepo.carregarDados(caminhoArquivoLares, pessoaRepo, casalRepo);
        tarefaRepo.carregarDados(caminhoArquivoTarefa, larRepo, pessoaRepo);
        compraRepo.carregarDados(caminhoArquivoCompra, tarefaRepo, pessoaRepo);
        //casamentoRepo.carregarDados(caminhoArquivoCasamento, pessoaRepo, festaRepo, larRepo, casalRepo);
        //festaRepo.carregarDados(caminhoArquivoFesta, casamentoRepo, pessoaRepo);
        //casamentoRepo.recarregarFestas(festaRepo); // Recarregar festas após carregar os casamentos

    } catch (const DataInconsistencyException& e) {
        cerr << "Exceção capturada: " << e.what() << endl;
    } catch (const std::runtime_error& e) {
        cerr << "Exceção capturada: " << e.what() << endl;
    }

    // Destrutores
    pessoaRepo.~PessoaRepository();
    larRepo.~LarRepository();
    tarefaRepo.~TarefaRepository();

    return 0;
}
