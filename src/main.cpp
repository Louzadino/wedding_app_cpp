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

#include "service/EstatisticasPrestadoresService.hpp"

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
        casamentoRepo.carregarDados(caminhoArquivoCasamento, pessoaRepo, festaRepo, larRepo, casalRepo);
        festaRepo.carregarDados(caminhoArquivoFesta, casamentoRepo, pessoaRepo);
        casamentoRepo.recarregarFestas(festaRepo); // Recarregar festas após carregar os casamentos

        // Saidas dos arquivos
        string caminhoArquivoRelatorio1 = caminhoArquivoEntrada + "/saida/1-planejamento.csv";
        string caminhoArquivoRelatorio2 = caminhoArquivoEntrada + "/saida/2-estatisticas-prestadores.csv";
        string caminhoArquivoRelatorio3 = caminhoArquivoEntrada + "/saida/3-estatisticas-casais.csv";

        // Gerar estatísticas de prestadores de serviço
        service::EstatisticasPrestadoresService estatisticasPrestadores(pessoaRepo, tarefaRepo, compraRepo);
        estatisticasPrestadores.gerarRelatorioPrestadores(caminhoArquivoRelatorio2);

        // Print dos dados para teste
        /*
        vector<Pessoa*> pessoas = pessoaRepo.listar();
        for (const auto& pessoa : pessoas) {
            cout << *pessoa << endl;
        }

        vector<Lar*> lares = larRepo.listar();
        for (const auto& lar : lares) {
            cout << *lar << endl;
        }

        vector<Tarefa*> tarefas = tarefaRepo.listar();
        for (const auto& tarefa : tarefas) {
            cout << *tarefa << endl;
        }

        vector<Compra*> compras = compraRepo.listar();
        for (const auto& compra : compras) {
            cout << *compra << endl;
        }

        vector<Casamento*> casamentos = casamentoRepo.listar();
        for (const auto& casamento : casamentos) {
            cout << *casamento << endl;
        }

        vector<Festa*> festas = festaRepo.listar();
        for (const auto& festa : festas) {
            cout << *festa << endl;
        }

        vector<Casal*> casais = casalRepo.listar();
        for (const auto& casal : casais) {
            cout << *casal << endl;
        }
        */

    } catch (const DataInconsistencyException& e) {
        cerr << "Exceção capturada: " << e.what() << endl;
    } catch (const std::runtime_error& e) {
        cerr << "Exceção capturada: " << e.what() << endl;
    }

    return 0;
}
