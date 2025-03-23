#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <stdexcept>

#include "repository/CasamentoRepository.hpp"
#include "repository/CompraRepository.hpp"
#include "repository/FestaRepository.hpp"
#include "repository/LarRepository.hpp"
#include "repository/PessoaRepository.hpp"
#include "repository/TarefaRepository.hpp"
#include "repository/CasalRepository.hpp"

#include "service/EstatisticasPrestadoresService.hpp"
#include "service/EstatisticasCasaisService.hpp"
#include "service/PlanejamentoFinanceiro.hpp"

#include "exception/DataInconsistencyException.hpp"
#include "exception/TratamentoExceptions.hpp"

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

        // Gerar estatísticas dos casais
        service::EstatisticasCasaisService estatisticasCasais(casalRepo, casamentoRepo, pessoaRepo, tarefaRepo, festaRepo, compraRepo, larRepo);
        estatisticasCasais.gerarEstatisticas(caminhoArquivoRelatorio3);

        // Gerar estatísticas de prestadores de serviço
        service::EstatisticasPrestadoresService estatisticasPrestadores(pessoaRepo, tarefaRepo, compraRepo);
        estatisticasPrestadores.gerarRelatorioPrestadores(caminhoArquivoRelatorio2);

        // Gerar planejamento financeiro do casal
        service::PlanejamentoFinanceiro planejamento(casalRepo, casamentoRepo, pessoaRepo, tarefaRepo, festaRepo, compraRepo, larRepo);


        // Criar ou limpar arquivo CSV
        ofstream writer(caminhoArquivoRelatorio1);
        if (!writer.is_open()) {
            throw runtime_error("Erro ao abrir arquivo de saída do relatório.");
        }
        writer.close();

        // Ler os CPFs do arquivo de entrada e gerar o planejamento
        string linha;
        while (getline(cin, linha)) {
            // Procura o índice do primeiro e do último caractere na string que não seja um espaço (ou tabulação)
            linha.erase(0, linha.find_first_not_of(" \t")); // Remove espaços em branco do início
            linha.erase(linha.find_last_not_of(" \t") + 1); // Remove espaços em branco do final

            // Se a linha estiver vazia, encerra o loop (não há mais CPFs para processar)
            if (linha.empty()) {
                break;
            }

            // Separar os CPFs da linha
            stringstream ss(linha); // Cria um stream de string a partir da linha
            vector<string> cpfs;
            string cpf;

            while (getline(ss, cpf, ',')) {
                // Procura o índice do primeiro e do último caractere na string que não seja um espaço (ou tabulação)
                cpf.erase(0, cpf.find_first_not_of(" \t")); // Remove espaços antes do CPF
                cpf.erase(cpf.find_last_not_of(" \t") + 1); // Remove espaços depois do CPF
                cpfs.push_back(cpf);
            }

            if (cpfs.size() == 2) {
                planejamento.gerarPlanejamento(caminhoArquivoRelatorio1, cpfs[0], cpfs[1]);
            }
        }
    } catch (const DataInconsistencyException& e) {
        exception::TratamentoExceptions tratamentoExceptions(new DataInconsistencyException(e.what()));
        tratamentoExceptions.escreveDadosInconsistentesException(caminhoArquivoEntrada);
    }

    return 0;
}
