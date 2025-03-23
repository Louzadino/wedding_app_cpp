#include "EstatisticasCasaisService.hpp"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <algorithm>

using namespace std;
using namespace model;
using namespace repository;
using namespace service;

namespace service {

struct NumPunctBR : numpunct<char> {
protected:
    char do_decimal_point() const override {
        return ','; // Define vírgula como separador decimal
    }
};

// Construtor de EstatisticaCasal
EstatisticaCasal::EstatisticaCasal(const string& nome1, const string& nome2, double totalGasto, int festasConvidados)
    : nome1(nome1), nome2(nome2), totalGasto(totalGasto), festasConvidados(festasConvidados) {}

string EstatisticaCasal::getNome1() const { return nome1; }
string EstatisticaCasal::getNome2() const { return nome2; }
double EstatisticaCasal::getTotalGasto() const { return totalGasto; }
int EstatisticaCasal::getFestasConvidados() const { return festasConvidados; }

// Construtor de EstatisticasCasaisService
EstatisticasCasaisService::EstatisticasCasaisService(const repository::CasalRepository& casalRepo, 
                                                     const repository::CasamentoRepository& casamentoRepo,
                                                     const repository::PessoaRepository& pessoaRepo, 
                                                     const repository::TarefaRepository& tarefaRepo,
                                                     const repository::FestaRepository& festaRepo, 
                                                     const repository::CompraRepository& compraRepo,
                                                     const repository::LarRepository& larRepo)
    : casais(casalRepo), casamentos(casamentoRepo), pessoas(pessoaRepo), tarefas(tarefaRepo),
      festas(festaRepo), compras(compraRepo), lares(larRepo) {}

void EstatisticasCasaisService::gerarEstatisticas(const string& filePath) {
    vector<EstatisticaCasal> estatisticasLista;
    set<string> casaisProcessados;

    // 🔹 Busca casais em CasalRepository, garantindo que não estejam duplicados
    for (const auto& casal : casais.listar()) {
        adicionarEstatisticaCasal(casal->getIdPessoa1(), casal->getIdPessoa2(), estatisticasLista, casaisProcessados);
    }

    // 🔹 Ordenação: primeiro pelo total gasto (decrescente), depois pelo nome1 (alfabético)
    sort(estatisticasLista.begin(), estatisticasLista.end(), [](const EstatisticaCasal& a, const EstatisticaCasal& b) {
        if (a.getTotalGasto() != b.getTotalGasto()) {
            return a.getTotalGasto() > b.getTotalGasto();
        }
        return a.getNome1() < b.getNome1();
    });

    // 🔹 Escrevendo os dados no CSV
    locale localeBR(locale(), new NumPunctBR);
    ofstream file(filePath);
    file.imbue(localeBR); // Aplica o locale ao arquivo de saída

    if (file.is_open()) {
        for (const auto& estatistica : estatisticasLista) {
            file << estatistica.getNome1() << ";"
                 << estatistica.getNome2() << ";"
                 << "R$ " << fixed << setprecision(2) << estatistica.getTotalGasto() << ";"
                 << estatistica.getFestasConvidados() << "\n";
        }
    } else {
        cerr << "Erro ao gerar o relatório de casais!" << endl;
    }
}

void EstatisticasCasaisService::adicionarEstatisticaCasal(const string& idPessoa1, const string& idPessoa2, 
                                                          vector<EstatisticaCasal>& estatisticasLista, 
                                                          set<string>& casaisProcessados) {
    string chaveCasal = idPessoa1 + "-" + idPessoa2;

    // Evita processar o mesmo casal mais de uma vez
    if (casaisProcessados.find(chaveCasal) != casaisProcessados.end()) {
        return;
    }
    casaisProcessados.insert(chaveCasal);

    PessoaFisica* pessoa1 = dynamic_cast<PessoaFisica*>(pessoas.buscarPorId(idPessoa1));
    PessoaFisica* pessoa2 = dynamic_cast<PessoaFisica*>(pessoas.buscarPorId(idPessoa2));

    if (!pessoa1 || !pessoa2) return;

    // Ordena os nomes para garantir que a ordem de impressão seja sempre a mesma
    string nome1 = (pessoa1->getNome() < pessoa2->getNome()) ? pessoa1->getNome() : pessoa2->getNome();
    string nome2 = (pessoa1->getNome() < pessoa2->getNome()) ? pessoa2->getNome() : pessoa1->getNome();

    double totalGasto = calcularGastosTarefas(idPessoa1, idPessoa2) +
                        calcularGastosFestas(idPessoa1, idPessoa2) +
                        calcularGastosCompras(idPessoa1, idPessoa2);

    // Verifica quantas festas o casal foi convidado
    int festasConvidados = 0;
    for (const auto& festa : festas.listar()) {
        vector<string> convidados = festa->getConvidados();

        if (find(convidados.begin(), convidados.end(), nome1) != convidados.end() &&
            find(convidados.begin(), convidados.end(), nome2) != convidados.end()) {
            festasConvidados++;
        }
    }

    estatisticasLista.emplace_back(nome1, nome2, totalGasto, festasConvidados);
}

double EstatisticasCasaisService::calcularGastosTarefas(const string& idPessoa1, const string& idPessoa2) {
    double total = 0.0;
    for (const auto& tarefa : tarefas.listar()) {
        Lar* lar = lares.buscarPorId(tarefa->getIdLar());
        if (lar && ((lar->getCasal()->getIdPessoa1() == idPessoa1 && lar->getCasal()->getIdPessoa2() == idPessoa2) ||
                    (lar->getCasal()->getIdPessoa1() == idPessoa2 && lar->getCasal()->getIdPessoa2() == idPessoa1))) {
            total += tarefa->getValorPrestador();
        }
    }
    return total;
}

double EstatisticasCasaisService::calcularGastosFestas(const string& idPessoa1, const string& idPessoa2) {
    double total = 0.0;
    for (const auto& festa : festas.listar()) {
        Casamento* casamento = casamentos.buscarPorId(festa->getIdCasamento());
        if (casamento &&
            casamento->getCasal()->getIdPessoa1() == idPessoa1 &&
            casamento->getCasal()->getIdPessoa2() == idPessoa2) {
            total += festa->getValorFesta();
        }
    }
    return total;
}

double EstatisticasCasaisService::calcularGastosCompras(const string& idPessoa1, const string& idPessoa2) {
    double total = 0.0;
    for (const auto& compra : compras.listar()) {
        Lar* lar = lares.buscarPorId(tarefas.buscarPorId(compra->getIdTarefa())->getIdLar());
        if (lar && lar->getCasal()->getIdPessoa1() == idPessoa1 &&
                   lar->getCasal()->getIdPessoa2() == idPessoa2) {
            total += compra->getQuantidade() * compra->getValorUnitario();
        }
    }
    return total;
}

} // namespace service