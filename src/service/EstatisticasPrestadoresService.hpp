#ifndef ESTATISTICAS_PRESTADORES_SERVICE_H
#define ESTATISTICAS_PRESTADORES_SERVICE_H

#include <string>
#include <map>
#include <vector>
#include "repository/PessoaRepository.hpp"
#include "repository/TarefaRepository.hpp"
#include "repository/CompraRepository.hpp"

namespace service {

// Declaração antecipada das classes
class PessoaRepository;
class TarefaRepository;
class CompraRepository;

class EstatisticasPrestadoresService {

private:
    const repository::PessoaRepository& pessoas;
    const repository::TarefaRepository& tarefas;
    const repository::CompraRepository& compras;

public:
    EstatisticasPrestadoresService(const repository::PessoaRepository& pessoaRepo, 
                                   const repository::TarefaRepository& tarefaRepo, 
                                   const repository::CompraRepository& compraRepo);

    void gerarRelatorioPrestadores(const std::string& caminhoSaida);
};

}

#endif
