#include "EstatisticasPrestadoresService.hpp"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <algorithm>

using namespace std;
using namespace repository;
using namespace model;

namespace service {

struct NumPunctBR : std::numpunct<char> {
protected:
    char do_decimal_point() const override {
        return ','; // Define vírgula como separador decimal
    }
};

EstatisticasPrestadoresService::EstatisticasPrestadoresService(const repository::PessoaRepository& pessoaRepo, 
                                                               const repository::TarefaRepository& tarefaRepo, 
                                                               const repository::CompraRepository& compraRepo)
    : pessoas(pessoaRepo), tarefas(tarefaRepo), compras(compraRepo) {}

void EstatisticasPrestadoresService::gerarRelatorioPrestadores(const string& caminhoSaida) {
    map<string, double> valoresRecebidos;
    map<string, string> tiposPrestadores;

    // Processa os pagamentos de tarefas (prestadores de serviço)
    for (const auto& tarefa : tarefas.listar()) {
        string idPrestador = tarefa->getIdPrestador();
        double valor = tarefa->getValorPrestador();
        valoresRecebidos[idPrestador] += valor;
    }

    // Processa os pagamentos de compras (lojas)
    for (const auto& compra : compras.listar()) {
        string idLoja = compra->getIdLoja();
        double valor = compra->valorTotal();
        valoresRecebidos[idLoja] += valor;
    }

    // Determina os tipos de prestadores
    for (const auto& pessoa : pessoas.listar()) {
        string id = pessoa->getIdPessoa();

        if (dynamic_cast<PessoaFisica*>(pessoa) != nullptr) {
            tiposPrestadores[id] = "PF";
        } 
        else if (dynamic_cast<Loja*>(pessoa) != nullptr) {
            tiposPrestadores[id] = "Loja";
        } 
        else if (dynamic_cast<PessoaJuridica*>(pessoa) != nullptr) {
            tiposPrestadores[id] = "PJ";
        }

        if (dynamic_cast<PessoaFisica*>(pessoa) == nullptr) {
            // Garante que todos os prestadores estejam no mapa, mesmo que com R$ 0,00
            if (valoresRecebidos.find(id) == valoresRecebidos.end()) {
                valoresRecebidos[id] = 0.0;
            }
        }
    }

    // Converte o mapa para um vetor ordenado
    vector<pair<string, double>> listaOrdenada(valoresRecebidos.begin(), valoresRecebidos.end());

    sort(listaOrdenada.begin(), listaOrdenada.end(), [&](const pair<string, double>& e1, const pair<string, double>& e2) {
        string tipo1 = tiposPrestadores[e1.first];
        string tipo2 = tiposPrestadores[e2.first];

        // Mapeia os tipos para valores numéricos
        int peso1 = (tipo1 == "PF") ? 1 : (tipo1 == "PJ") ? 2 : 3;
        int peso2 = (tipo2 == "PF") ? 1 : (tipo2 == "PJ") ? 2 : 3;

        // Ordena pelo peso (PF primeiro, depois PJ, depois Loja)
        if (peso1 != peso2) {
            return peso1 < peso2;
        }

        // Ordena por valor recebido (decrescente)
        if (e1.second != e2.second) {
            return e1.second > e2.second;
        }

        // Ordena por nome (alfabeticamente)
        string nome1 = pessoas.buscarPorId(e1.first)->getNome();
        string nome2 = pessoas.buscarPorId(e2.first)->getNome();
        return nome1 < nome2;
    });

    // Configura o locale para o formato brasileiro
    locale localeBR(locale(), new NumPunctBR);
    ofstream file(caminhoSaida);
    file.imbue(localeBR); // Aplica o locale ao arquivo de saída
    
    if (file.is_open()) {
        for (const auto& entry : listaOrdenada) {
            string id = entry.first;
            string tipo = tiposPrestadores[id];
            string nome = pessoas.buscarPorId(id)->getNome();
            double totalRecebido = entry.second;

            file << tipo << ";" << nome << ";R$ " 
                 << fixed << setprecision(2) << totalRecebido << "\n";
        }
    } else {
        cerr << "Erro ao gerar relatório de prestadores!" << endl;
    }
}

} // namespace service
