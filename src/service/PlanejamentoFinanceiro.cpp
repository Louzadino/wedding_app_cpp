#include "PlanejamentoFinanceiro.hpp"

#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <chrono>

using namespace std;
using namespace repository;
using namespace model;

namespace service {

struct NumPunctBR : numpunct<char> {
protected:
    char do_decimal_point() const override {
        return ','; // Define vírgula como separador decimal
    }
};

const char* FORMATADOR_DATA = "%m/%Y"; // Formato de data: mês/ano    const repository::CasalRepository& casais;

PlanejamentoFinanceiro::PlanejamentoFinanceiro(const repository::CasalRepository& casalRepo,
                           const repository::CasamentoRepository& casamentoRepo,
                           const repository::PessoaRepository& pessoaRepo,
                           const repository::TarefaRepository& tarefaRepo,
                           const repository::FestaRepository& festaRepo,
                           const repository::CompraRepository& compraRepo,
                           const repository::LarRepository& larRepo)
    : casais(casalRepo), casamentos(casamentoRepo), pessoas(pessoaRepo),
      tarefas(tarefaRepo), festas(festaRepo), compras(compraRepo), lares(larRepo) {}

void PlanejamentoFinanceiro::gerarPlanejamento(const string& filePath, const string& cpf1, const string& cpf2) {
    PessoaFisica* pessoa1 = buscarPessoaPorCpf(cpf1);
    PessoaFisica* pessoa2 = buscarPessoaPorCpf(cpf2);

    if (pessoa1 == nullptr || pessoa2 == nullptr) {
        escreverMensagem(filePath, "Casal com CPFs " + cpf1 + " e " + cpf2 + " não está cadastrado.");
        return;
    }

    Casal* casal = buscarCasal(pessoa1->getIdPessoa(), pessoa2->getIdPessoa());

    if (casal == nullptr) {
        escreverMensagem(filePath, "Casal com CPFs " + cpf1 + " e " + cpf2 + " não está cadastrado.");
        return;
    }

    map<string, double> saldoMensal = calcularSaldoMensalCasal(casal, pessoa1, pessoa2);

    auto dataInicial = encontrarPrimeiroGasto(casal);
    auto dataFinal = calcularDataFinal(casal, pessoa1->getIdPessoa(), pessoa2->getIdPessoa());

    if (saldoMensal.empty()) {
        escreverMensagem(filePath, "Casal com CPFs " + cpf1 + " e " + cpf2 + " não possui gastos cadastrados.");
        return;
    }

    escreverCSV(filePath, pessoa1, pessoa2, saldoMensal, dataInicial, dataFinal);
}

PessoaFisica* PlanejamentoFinanceiro::buscarPessoaPorCpf(const string& cpf) {
    for (auto& pessoa : pessoas.listar()) {
        PessoaFisica* pessoaFisica = dynamic_cast<PessoaFisica*>(pessoa);
        if (pessoaFisica != nullptr && pessoaFisica->getCpf() == cpf) {
            return pessoaFisica;
        }
    }
    return nullptr;
}

Casal* PlanejamentoFinanceiro::buscarCasal(const string& idPessoa1, const string& idPessoa2) {
    for (auto& casal : casais.listar()) {
        if ((casal->getIdPessoa1() == idPessoa1 && casal->getIdPessoa2() == idPessoa2) ||
            (casal->getIdPessoa1() == idPessoa2 && casal->getIdPessoa2() == idPessoa1)) {
            return casal;
        }
    }
    return nullptr;
}

void PlanejamentoFinanceiro::escreverMensagem(const string& filePath, const string& mensagem) {
    ofstream file(filePath, ios::app);
    if (file.is_open()) {
        file << mensagem << "\n";
    }
}
map<string, double> PlanejamentoFinanceiro::calcularSaldoMensalCasal(Casal* casal, PessoaFisica* pessoa1, PessoaFisica* pessoa2) {
    map<string, double> saldoMensal;
    tm primeiraData = encontrarPrimeiroGasto(casal);
    tm ultimaData = calcularDataFinal(casal, pessoa1->getIdPessoa(), pessoa2->getIdPessoa());

    if (primeiraData.tm_year == 0 || ultimaData.tm_year == 0) {
        return saldoMensal; // Retorna um mapa vazio
    }

    tm dataAtual = primeiraData;
    Financeiro financeiro1 = pessoa1->getFinanceiro();
    Financeiro financeiro2 = pessoa2->getFinanceiro();

    double saldoMensalCasal = financeiro1.getDinheiroPoupanca() + financeiro2.getDinheiroPoupanca();
    double salarioTotalCasalFixo = financeiro1.getSalarioLiquido() + financeiro2.getSalarioLiquido();
    double gastosMensaisCasalFixo = financeiro1.getGastosMensais() + financeiro2.getGastosMensais();

    while (true) {
        double gastosTotais = gastosMensaisCasalFixo + calcularGastosPorMes(casal->getIdCasamento(), dataAtual, 
                                                                            pessoa1->getIdPessoa(), pessoa2->getIdPessoa());
        double ganhosMes = calcularGanhosPorMes(dataAtual, pessoa1->getIdPessoa(), pessoa2->getIdPessoa());

        // Atualiza o saldo mensal do casal
        // Saldo = Saldo * 0.5% + Salário total do casal - Gastos totais + Ganhos do mês
        // 0.5% é a taxa de rendimento da poupança
        saldoMensalCasal += (saldoMensalCasal * (0.5 / 100)) + salarioTotalCasalFixo - gastosTotais + ganhosMes;

        // Se for dezembro, adicionar 13o salário
        if (dataAtual.tm_mon == 11) {
            saldoMensalCasal += financeiro1.getSalarioLiquido() + financeiro2.getSalarioLiquido();
        }

        // Formata a data como string
        ostringstream oss;
        oss << put_time(&dataAtual, FORMATADOR_DATA);
        string dataFormatada = oss.str();

        // Adiciona ao mapa
        saldoMensal[dataFormatada] = saldoMensalCasal;

        if (dataAtual.tm_year == ultimaData.tm_year && 
            dataAtual.tm_mon == ultimaData.tm_mon) {
            // Sai do loop apenas após uma iteração extra
            break;
        }

        adicionarMes(dataAtual);
    }

    return saldoMensal;
}

void adicionarMes(tm& data) {
    data.tm_mon += 1; // Adiciona 1 ao mês
    if (data.tm_mon > 11) { // Se ultrapassar dezembro
        data.tm_mon = 0;    // Volta para janeiro
        data.tm_year += 1;  // Incrementa o ano
    }
}

double PlanejamentoFinanceiro::calcularGanhosPorMes(tm data, const string& idPessoa1, const string& idPessoa2) {
    double totalGanhos = 0.0;
    
    for (Tarefa* tarefa : tarefas.listar()) {
        string idPrestador = tarefa->getIdPrestador();

        // Verifica se o prestador é um dos parceiros do casal
        if (idPrestador == idPessoa1 || idPrestador == idPessoa2) {
            // Verifica se a parcela da tarefa está sendo paga neste mês
            if (estaParcelaSendoPaga(tarefa->getDataInicio(), tarefa->getNumParcelas(), data)) {
                totalGanhos += tarefa->getValorParcela();
            }
        }
    }

    return totalGanhos;   
}

double PlanejamentoFinanceiro::calcularGastosPorMes(const string& idCasamento, tm data, const string& idPessoa1, const string& idPessoa2) {
    double totalGastos = 0.0;

    // Filtrar tarefas associadas ao casal e ao casamento
    for (Tarefa* tarefa : tarefas.listar()) {
        // Verifica se a tarefa pertence ao lar do casal
        Lar* larAssociado = lares.buscarPorId(tarefa->getIdLar());

        if (larAssociado != nullptr) {
            // Pega informações do lar associado à tarefa
            string idPessoa1Lar = larAssociado->getCasal()->getIdPessoa1();
            string idPessoa2Lar = larAssociado->getCasal()->getIdPessoa2();

            if ((idPessoa1Lar == idPessoa1 && idPessoa2Lar == idPessoa2) ||
                (idPessoa1Lar == idPessoa2 && idPessoa2Lar == idPessoa1)) {
                if (estaParcelaSendoPaga(tarefa->getDataInicio(), tarefa->getNumParcelas(), data)) {
                    totalGastos += tarefa->getValorParcela();
                }
            }
        }
    }

    // Filtrar compras associadas ao casal
    for (Compra* compra : compras.listar()) {
        // Verifica se a compra pertence ao lar do casal
        Tarefa* tarefaAssociada = tarefas.buscarPorId(compra->getIdTarefa());
        if (tarefaAssociada != nullptr) {
            Lar* larAssociado = lares.buscarPorId(tarefaAssociada->getIdLar());

            if (larAssociado != nullptr) {
                // Pega informações do lar associado à tarefa
                // Pega informações do lar associado à tarefa
                string idPessoa1Lar = larAssociado->getCasal()->getIdPessoa1();
                string idPessoa2Lar = larAssociado->getCasal()->getIdPessoa2();

                if ((idPessoa1Lar == idPessoa1 && idPessoa2Lar == idPessoa2) ||
                    (idPessoa1Lar == idPessoa2 && idPessoa2Lar == idPessoa1)) {
                    if (estaParcelaSendoPaga(tarefaAssociada->getDataInicio(), compra->getNumParcelas(), data)) {
                        totalGastos += compra->getValorParcela();
                    }
                }
            }
        }
    }

    // Filtrar festas associadas ao casamento
    for (Festa* festa : festas.listar()) {
        Casamento* casamentoAssociado = casamentos.buscarPorId(festa->getIdCasamento());
        if (casamentoAssociado != nullptr && idCasamento == casamentoAssociado->getIdCasamento()) {
            time_t parsedDate = parseDate(festa->getData(), FORMATADOR_DATA);
            tm* dataTm = localtime(&parsedDate);
            if (dataTm != nullptr && estaParcelaSendoPaga(*dataTm, festa->getNumParcelas(), data)) {
                totalGastos += festa->getValorParcela();
            }
        }
    }

    return totalGastos;
}

void PlanejamentoFinanceiro::escreverCSV(const string& filePath, PessoaFisica* pessoa1, PessoaFisica* pessoa2,
                                         map<string, double>& saldoMensal, tm dataInicio, tm dataFim) {
    locale localeBR(locale(), new NumPunctBR);                                  
    ofstream file(filePath, ios::app);
    file.imbue(localeBR); // Aplica o locale ao arquivo de saída

    if (file.is_open()) {
        file << "Nome 1;Nome 2;";

        // Cabeçalho com meses
        tm dataAtual = dataInicio;
        while (true) {
            char buffer[8];
            strftime(buffer, sizeof(buffer), "%m/%Y", &dataAtual);
            file << buffer << ";";
            if (dataAtual.tm_year == dataFim.tm_year && dataAtual.tm_mon == dataFim.tm_mon) break;
            dataAtual.tm_mon++;
        }
        file << "\n";

        file << pessoa1->getNome() << ";" << pessoa2->getNome() << ";";

        dataAtual = dataInicio;
        while (true) {
            char buffer[8];
            strftime(buffer, sizeof(buffer), "%m/%Y", &dataAtual);
            file << "R$ " << fixed << setprecision(2) << saldoMensal[buffer] << ";";
            if (dataAtual.tm_year == dataFim.tm_year && dataAtual.tm_mon == dataFim.tm_mon) break;
            dataAtual.tm_mon++;
        }
        file << "\n";
    }
}

bool PlanejamentoFinanceiro::estaParcelaSendoPaga(tm dataInicio, int numParcelas, tm dataConsulta) {
    for (int i = 0; i < numParcelas; i++) {
        dataInicio.tm_mon++;
        if (dataInicio.tm_year == dataConsulta.tm_year && dataInicio.tm_mon == dataConsulta.tm_mon) {
            return true;
        }
    }
    return false;
}

tm PlanejamentoFinanceiro::encontrarPrimeiroGasto(Casal* casal) {

    vector<tm> datas;
    string idPessoa1 = casal->getIdPessoa1();
    string idPessoa2 = casal->getIdPessoa2();

    // Buscar tarefas associadas ao casal
    for (Tarefa* tarefa : tarefas.listar()) {
        // Busca o lar da tarefa e verifica se pertence ao casal
        Lar* lar = lares.buscarPorId(tarefa->getIdLar());

        if (lar != nullptr) {
            string idPessoa1Lar = lar->getCasal()->getIdPessoa1();
            string idPessoa2Lar = lar->getCasal()->getIdPessoa2();

            if ((idPessoa1Lar == idPessoa1 && idPessoa2Lar == idPessoa2) ||
                (idPessoa1Lar == idPessoa2 && idPessoa2Lar == idPessoa1)) {
                datas.push_back(tarefa->getDataInicio());
            }
        }
    }

    // Buscar festas associadas ao casal
    for (Festa* festa : festas.listar()) {
        if (festa->getIdCasamento() == casal->getIdCasamento()) {
            time_t parsedDate = parseDate(festa->getData(), FORMATADOR_DATA);
            tm dataTm = localtime(parsedDate);

            datas.push_back(dataTm);
        }
    }

    // Buscar compras associadas a tarefas do casal
    for (Compra* compra : compraRepo.listar()) {
        Tarefa* tarefa = tarefaRepo.buscarPorId(compra->getIdTarefa());
        if (tarefa != nullptr) {
            Lar* lar = larRepo.buscarPorId(tarefa->getIdLar());
            if (lar != nullptr) {
                Casal* casalLar = &lar->getCasal();
                if ((casalLar->getIdPessoa1() == idPessoa1 && casalLar->getIdPessoa2() == idPessoa2) ||
                    (casalLar->getIdPessoa1() == idPessoa2 && casalLar->getIdPessoa2() == idPessoa1)) {
                    datas.push_back(tarefa->getDataInicio());
                }
            }
        }
    }

    // Retorna a menor data encontrada ou um valor nulo caso não haja dados
    return datas.empty() ? system_clock::time_point() : *min_element(datas.begin(), datas.end());
}

} // namespace service