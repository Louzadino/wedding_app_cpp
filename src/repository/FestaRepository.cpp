#include "FestaRepository.hpp"
#include "util/CSVReader.hpp"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <locale>
#include <stdexcept>

using namespace std;

namespace repository {

// Construtor
FestaRepository::FestaRepository() {}

// Destrutor
FestaRepository::~FestaRepository() {
    for (const auto& item : festas) {
        item.second->getConvidados().clear();
        delete item.second;
    }
    festas.clear();
}

void FestaRepository::adicionar(Festa* festa) {
    if (festa == nullptr) {
        throw invalid_argument("A festa não pode ser nula.");
    }
    if (festas.find(festa->getIdFesta()) != festas.end()) {
        throw invalid_argument("Já existe uma festa com este ID no repositório.");
    }
    if (casamentosComFesta.find(festa->getIdCasamento()) != casamentosComFesta.end()) {
        throw invalid_argument("O casamento com ID " + festa->getIdCasamento() + " já possui uma festa cadastrada.");
    }

    festas[festa->getIdFesta()] = festa;
    casamentosComFesta.insert(festa->getIdCasamento());
}

void FestaRepository::remover(Festa* festa) {
    if (festa == nullptr) {
        throw invalid_argument("A festa não pode ser nula.");
    }
    if (festas.find(festa->getIdFesta()) == festas.end()) {
        throw invalid_argument("A festa não existe no repositório.");
    }

    festas.erase(festa->getIdFesta());
    casamentosComFesta.erase(festa->getIdCasamento());
}

vector<Festa*> FestaRepository::listar() const {
    vector<Festa*> lista;
    for (const auto& item : festas) {
        lista.push_back(item.second);
    }
    return lista;
}

Festa* FestaRepository::buscarPorId(const string& id) const {
    if (id.empty()) {
        throw invalid_argument("O ID não pode ser nulo ou vazio.");
    }
    auto it = festas.find(id);
    if (it != festas.end()) {
        return it->second;
    }
    return nullptr;
}

void FestaRepository::carregarDados(const string& caminhoArquivo, CasamentoRepository& casamentoRepo, PessoaRepository& pessoaRepo) {
    vector<vector<string>> linhas = CSVReader::lerCSV(caminhoArquivo);

    locale brasilianLocale("pt_BR.UTF-8");

    for (const auto& campos : linhas) {
        if (campos.size() < 8) {
            cout << "Linha inválida encontrada, ignorando: ";
            for (const auto& campo : campos) {
                cout << campo << ";";
            }
            cout << endl;
            continue;
        }

        string idFesta = campos[0];
        string idCasamento = campos[1];

        // Verifica se o casamento já tem uma festa cadastrada
        if (casamentosComFesta.find(idCasamento) != casamentosComFesta.end()) {
            continue;
        }

        // Verifica se o ID da festa já existe
        if (festas.find(idFesta) != festas.end()) {
            throw invalid_argument("ID repetido " + idFesta + " na classe Festa.");
        }

        string local = campos[2];

        // Converte a data usando o formato dd/MM/yyyy
        struct tm data = {};
        istringstream dataStream(campos[3]);
        dataStream >> get_time(&data, "%d/%m/%Y");
        if (dataStream.fail()) {
            throw invalid_argument("Erro ao converter data para festa com ID " + idFesta);
        }

        string hora = campos[4];

        // Converte o valor usando o locale brasileiro
        istringstream valorStream(campos[5]);
        valorStream.imbue(brasilianLocale);
        double valorFesta;
        valorStream >> valorFesta;
        if (valorStream.fail()) {
            throw invalid_argument("Erro ao converter valor para festa com ID " + idFesta);
        }

        int numParcelas = stoi(campos[6]);
        int numConvidados = stoi(campos[7]);

        // 🔹 Validação: Verifica se o ID do casamento existe
        Casamento* casamento = casamentoRepo.buscarPorId(idCasamento);
        if (casamento == nullptr) {
            throw invalid_argument("ID de Casamento " + idCasamento + " não cadastrado na festa de ID " + idFesta + ".");
        }

        // Obtém os nomes dos donos da festa
        PessoaFisica* dono1 = dynamic_cast<PessoaFisica*>(pessoaRepo.buscarPorId(casamento->getCasal()->getIdPessoa1()));
        PessoaFisica* dono2 = dynamic_cast<PessoaFisica*>(pessoaRepo.buscarPorId(casamento->getCasal()->getIdPessoa2()));

        if (dono1 == nullptr || dono2 == nullptr) {
            throw invalid_argument("Os donos da festa com ID " + idFesta + " não foram encontrados.");
        }

        string nomeDono1 = dono1->getNome();
        string nomeDono2 = dono2->getNome();

        // Lista de convidados (removendo os donos da festa)
        vector<string> convidados;
        if (numConvidados > 0) {
            for (size_t i = 8; i < campos.size(); i++) {
                string convidado = campos[i];
                if (convidado != nomeDono1 && convidado != nomeDono2) {
                    convidados.push_back(convidado);
                }
            }
        }

        // Converte a data para string no formato dd/MM/yyyy
        ostringstream dataFormatada;
        dataFormatada << put_time(&data, "%d/%m/%Y");

        // Cria a festa com a data formatada como string
        Festa* festa = new Festa(idFesta, idCasamento, local, valorFesta, numParcelas, dataFormatada.str(), hora, convidados);
        adicionar(festa);
    }
}

} // namespace repository
