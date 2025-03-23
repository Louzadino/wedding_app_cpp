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

        // Avança para o próximo mês
        dataAtual = adicionarMeses(dataAtual, 1);
    }

    return saldoMensal;
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
            if (estaParcelaSendoPaga(festa->getData(), festa->getNumParcelas(), data)) {
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
            file << buffer;

            if (dataAtual.tm_year == dataFim.tm_year && dataAtual.tm_mon == dataFim.tm_mon) break;
            file << ";"; // Adiciona ponto e vírgula se não for a última coluna
            dataAtual = adicionarMeses(dataAtual, 1);
        }
        file << "\n";

        file << pessoa1->getNome() << ";" << pessoa2->getNome() << ";";

        dataAtual = dataInicio;
        while (true) {
            char buffer[8];
            strftime(buffer, sizeof(buffer), "%m/%Y", &dataAtual);
            file << "R$ " << fixed << setprecision(2) << saldoMensal[buffer] << ";";

            if (dataAtual.tm_year == dataFim.tm_year && dataAtual.tm_mon == dataFim.tm_mon) break;

            dataAtual = adicionarMeses(dataAtual, 1);
        }
        file << "\n";
    }
}

bool PlanejamentoFinanceiro::estaParcelaSendoPaga(tm dataInicio, int numParcelas, tm dataConsulta) {
    for (int i = 0; i < numParcelas; i++) {
        if (dataInicio.tm_year == dataConsulta.tm_year && dataInicio.tm_mon == dataConsulta.tm_mon) {
            return true;
        }
        dataInicio = adicionarMeses(dataInicio, 1);
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
            datas.push_back(festa->getData());
        }
    }

    // Buscar compras associadas a tarefas do casal
    for (Compra* compra : compras.listar()) {
        // Verifica se a compra pertence ao lar do casal
        string idTarefa = compra->getIdTarefa();
        string idLarAssociadaTarefa = tarefas.buscarPorId(idTarefa)->getIdLar();


        if ((lares.buscarPorId(idLarAssociadaTarefa)->getCasal()->getIdPessoa1() == idPessoa1) &&
            (lares.buscarPorId(idLarAssociadaTarefa)->getCasal()->getIdPessoa2() == idPessoa2)) {
            datas.push_back(tarefas.buscarPorId(compra->getIdTarefa())->getDataInicio());
        }
    }

    // Retorna a menor data encontrada ou um valor nulo caso não haja dados
    tm menorData = encontrarMenorData(datas);
    if (menorData.tm_year == -1) {
        return {};
    }

    return menorData;
}

tm PlanejamentoFinanceiro::calcularDataFinal(Casal* casal, const string& idPessoa1, const string& idPessoa2) {
        // Inicializa a data final
        tm dataFinal = {};
        dataFinal.tm_year = 0;  // Ano 1900 (tm_year é o número de anos desde 1900)
        dataFinal.tm_mon = 0;   // Janeiro (baseado em zero)
        dataFinal.tm_mday = 1;  // Dia 1

        // Última data considerando parcelas das TAREFAS
        tm ultimaDataTarefa = {};
        for (const auto& tarefa : tarefas.listar()) {
            Lar* lar = lares.buscarPorId(tarefa->getIdLar());
            if (lar && ((lar->getCasal()->getIdPessoa1() == idPessoa1 && lar->getCasal()->getIdPessoa2() == idPessoa2) ||
                        (lar->getCasal()->getIdPessoa1() == idPessoa2 && lar->getCasal()->getIdPessoa2() == idPessoa1))) {
                
                // Adiciona o número de parcelas - 1 meses à data de início da tarefa
                tm novaData = adicionarMeses(tarefa->getDataInicio(), tarefa->getNumParcelas() - 1);
                if (mktime(&novaData) > mktime(&ultimaDataTarefa)) {
                    ultimaDataTarefa = novaData;
                }
            }
        }

        // Última data considerando parcelas das FESTAS
        tm ultimaDataFesta = {};
        for (const auto& festa : festas.listar()) {
            if (festa->getIdCasamento() == casal->getIdCasamento()) {
                // Adiciona o número de parcelas - 1 meses à data da festa
                tm novaData = adicionarMeses(festa->getData(), festa->getNumParcelas() - 1);
                if (mktime(&novaData) > mktime(&ultimaDataFesta)) {
                    ultimaDataFesta = novaData;
                }
            }
        }

        // Última data considerando parcelas das COMPRAS
        tm ultimaDataCompra = {};
        for (const auto& compra : compras.listar()) {
            string idTarefa = compra->getIdTarefa();
            Tarefa* tarefa = tarefas.buscarPorId(idTarefa);
            if (tarefa) {
                Lar* lar = lares.buscarPorId(tarefa->getIdLar());
                if (lar && ((lar->getCasal()->getIdPessoa1() == idPessoa1 && lar->getCasal()->getIdPessoa2() == idPessoa2) ||
                            (lar->getCasal()->getIdPessoa1() == idPessoa2 && lar->getCasal()->getIdPessoa2() == idPessoa1))) {
                    // Adiciona o número de parcelas - 1 meses à data de início da tarefa
                    tm novaData = adicionarMeses(tarefa->getDataInicio(), compra->getNumParcelas() - 1);
                    if (mktime(&novaData) > mktime(&ultimaDataCompra)) {
                        ultimaDataCompra = novaData;
                    }          
                }
            }
        }

        // Determina a maior data entre todas as despesas do casal
        vector<tm> datas = {ultimaDataTarefa, ultimaDataFesta, ultimaDataCompra};
        dataFinal = encontrarMaiorData(datas);

        if (dataFinal.tm_year == -1) {
            return {};
        }

        // Se não houver despesas, retorna a data atual + 1 mês como fallback
        if (dataFinal.tm_year == 0 && dataFinal.tm_mon == 0 && dataFinal.tm_mday == 0) {
            // Se não houver despesas, retorna a data atual + 1 mês como fallback
            time_t now = time(nullptr); // Obtém a data atual
            tm dataAtual = *localtime(&now); // Converte para tm
            return adicionarMeses(dataAtual, 1); // Adiciona 1 mês
        }

        return dataFinal;
}

tm PlanejamentoFinanceiro::adicionarMeses(std::tm data, int meses) {
    // Adiciona os meses ao campo tm_mon
    data.tm_mon += meses;

    // Ajusta o ano e o mês caso tm_mon ultrapasse os limites (0-11)
    while (data.tm_mon > 11) {
        data.tm_mon -= 12;
        data.tm_year += 1;
    }
    while (data.tm_mon < 0) {
        data.tm_mon += 12;
        data.tm_year -= 1;
    }

    return data;
}

tm PlanejamentoFinanceiro::encontrarMenorData(const vector<tm>& datas) {
    if (datas.empty()) {
        tm dataVazia = {};
        dataVazia.tm_year = -1; // Indica que não há data válida
        return dataVazia;
    }

    return *min_element(datas.begin(), datas.end(), [](const tm& a, const tm& b) {
        return mktime(const_cast<tm*>(&a)) < mktime(const_cast<tm*>(&b));
    });
}

tm PlanejamentoFinanceiro::encontrarMaiorData(const vector<tm>& datas) {
    if (datas.empty()) {
        tm dataVazia = {};
        dataVazia.tm_year = -1; // Indica que não há data válida
        return dataVazia;
    }

    return *max_element(datas.begin(), datas.end(), [](const tm& a, const tm& b) {
        return mktime(const_cast<tm*>(&a)) < mktime(const_cast<tm*>(&b));
    });
}

} // namespace service