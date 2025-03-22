#ifndef ESTATISTICAS_CASAIS_SERVICE_H
#define ESTATISTICAS_CASAIS_SERVICE_H

#include <string>
#include <vector>
#include <set>
#include "repository/CasamentoRepository.hpp"
#include "repository/CasalRepository.hpp"
#include "repository/PessoaRepository.hpp"
#include "repository/TarefaRepository.hpp"
#include "repository/FestaRepository.hpp"
#include "repository/CompraRepository.hpp"
#include "repository/LarRepository.hpp"

using namespace std;

namespace service {

// Declaração antecipada das classes de repositório para evitar dependências cíclicas
class CasalRepository;
class CasamentoRepository;
class PessoaRepository;
class TarefaRepository;
class FestaRepository;
class CompraRepository;
class LarRepository;

class EstatisticaCasal {
public:
    EstatisticaCasal(const string& nome1, const string& nome2, double totalGasto, int festasConvidados);

    string getNome1() const;
    string getNome2() const;
    double getTotalGasto() const;
    int getFestasConvidados() const;

private:
    string nome1;
    string nome2;
    double totalGasto;
    int festasConvidados;
};

class EstatisticasCasaisService {
public:
    EstatisticasCasaisService(const repository::CasalRepository& casalRepo, 
                              const repository::CasamentoRepository& casamentoRepo, 
                              const repository::PessoaRepository& pessoaRepo,
                              const repository::TarefaRepository& tarefaRepo, 
                              const repository::FestaRepository& festaRepo, 
                              const repository::CompraRepository& compraRepo,
                              const repository::LarRepository& larRepo);

    void gerarEstatisticas(const string& filePath);

private:
    const repository::CasalRepository& casais;
    const repository::CasamentoRepository& casamentos;
    const repository::PessoaRepository& pessoas;
    const repository::TarefaRepository& tarefas;
    const repository::FestaRepository& festas;
    const repository::CompraRepository& compras;
    const repository::LarRepository& lares;

    void adicionarEstatisticaCasal(const string& idPessoa1, const string& idPessoa2, 
                                   vector<EstatisticaCasal>& estatisticasLista, 
                                   set<string>& casaisProcessados);

    double calcularGastosTarefas(const string& idPessoa1, const string& idPessoa2);
    double calcularGastosFestas(const string& idPessoa1, const string& idPessoa2);
    double calcularGastosCompras(const string& idPessoa1, const string& idPessoa2);
};

} // namespace service

#endif
