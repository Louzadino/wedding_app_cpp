#ifndef PLANEJAMENTO_FINANCEIRO_H
#define PLANEJAMENTO_FINANCEIRO_H

#include <string>
#include <map>
#include <vector>
#include <ctime>
#include "model/Casal.hpp"
#include "model/Casamento.hpp"
#include "model/Compra.hpp"
#include "model/Festa.hpp"
#include "model/Financeiro.hpp"
#include "model/Lar.hpp"
#include "model/PessoaFisica.hpp"
#include "model/Tarefa.hpp"
#include "repository/CasalRepository.hpp"
#include "repository/CasamentoRepository.hpp"
#include "repository/CompraRepository.hpp"
#include "repository/FestaRepository.hpp"
#include "repository/LarRepository.hpp"
#include "repository/PessoaRepository.hpp"
#include "repository/TarefaRepository.hpp"

using namespace std;

namespace service {

// Declaração antecipada das classes para evitar dependências cíclicas
class CasalRepository;
class CasamentoRepository;
class PessoaRepository;
class TarefaRepository;
class FestaRepository;
class CompraRepository;
class LarRepository;

class PlanejamentoFinanceiro {
public:
    PlanejamentoFinanceiro(const repository::CasalRepository& casalRepo,
                           const repository::CasamentoRepository& casamentoRepo,
                           const repository::PessoaRepository& pessoaRepo,
                           const repository::TarefaRepository& tarefaRepo,
                           const repository::FestaRepository& festaRepo,
                           const repository::CompraRepository& compraRepo,
                           const repository::LarRepository& larRepo);

    void gerarPlanejamento(const string& filePath, const string& cpf1, const string& cpf2);

private:
    const repository::CasalRepository& casais;
    const repository::CasamentoRepository& casamentos;
    const repository::PessoaRepository& pessoas;
    const repository::TarefaRepository& tarefas;
    const repository::FestaRepository& festas;
    const repository::CompraRepository& compras;
    const repository::LarRepository& lares;

    void escreverCSV(const string& filePath, PessoaFisica* pessoa1, PessoaFisica* pessoa2,
                     map<string, double>& saldoMensal, tm dataInicio, tm dataFim);

    PessoaFisica* buscarPessoaPorCpf(const string& cpf);
    Casal* buscarCasal(const string& idPessoa1, const string& idPessoa2);
    void escreverMensagem(const string& filePath, const string& mensagem);

    map<string, double> calcularSaldoMensalCasal(Casal* casal, PessoaFisica* pessoa1, PessoaFisica* pessoa2);

    double calcularGanhosPorMes(tm data, const string& idPessoa1, const string& idPessoa2);
    double calcularGastosPorMes(const string& idCasamento, tm data, const string& idPessoa1, const string& idPessoa2);

    bool estaParcelaSendoPaga(tm dataInicio, int numParcelas, tm dataConsulta);

    tm encontrarPrimeiroGasto(Casal* casal);
    tm calcularDataFinal(Casal* casal, const string& idPessoa1, const string& idPessoa2);
};

} // namespace service

#endif