#include "LarRepository.hpp"
#include "util/CSVReader.hpp"
#include <iostream>
#include <sstream>
#include <stdexcept>

using namespace std;

namespace repository {

LarRepository::LarRepository() {}

LarRepository::~LarRepository() {
    for (auto& par : lares) {
        delete par.second;
    }
    lares.clear(); // Limpa o mapa após a deleção
}

void LarRepository::adicionar(Lar* lar) {
    if (lar == nullptr) {
        throw invalid_argument("O lar não pode ser nulo.");
    }
    if (lares.find(lar->getIdLar()) != lares.end()) {
        throw invalid_argument("Já existe um lar com este ID no repositório.");
    }
    lares[lar->getIdLar()] = lar;
}

void LarRepository::remover(Lar* lar) {
    if (lar == nullptr) {
        throw invalid_argument("O lar não pode ser nulo.");
    }
    if (lares.find(lar->getIdLar()) == lares.end()) {
        throw invalid_argument("O lar não existe no repositório.");
    }
    lares.erase(lar->getIdLar());
}

vector<Lar*> LarRepository::listar() const {
    vector<Lar*> lista;
    for (const auto& item : lares) {
        lista.push_back(item.second);
    }
    return lista;
}

Lar* LarRepository::buscarPorId(const string& id) const {
    if (id.empty()) {
        throw invalid_argument("O ID não pode ser nulo ou vazio.");
    }
    auto it = lares.find(id);
    if (it != lares.end()) {
        return it->second;
    }
    return nullptr;
}

void LarRepository::carregarDados(const string& caminhoArquivo, PessoaRepository& pessoaRepo, CasalRepository& casalRepo) {
    vector<vector<string>> linhas = CSVReader::lerCSV(caminhoArquivo);

    for (const auto& campos : linhas) {
        if (campos.size() < 6) {
            cout << "Linha inválida encontrada, ignorando: ";
            for (const auto& campo : campos) {
                cout << campo << ";";
            }
            cout << endl;
            continue;
        }

        // Informações dos IDs que envolvem um lar
        string idLar = campos[0];

        // Verifica se o ID já existe no repositório
        if (lares.find(idLar) != lares.end()) {
            throw invalid_argument("ID repetido " + idLar + " na classe Lar.");
        }

        string idPessoa1 = campos[1];
        string idPessoa2 = campos[2];

        // 🔹 Validação: verificar se as pessoas existem no `PessoaRepository`
        bool pessoa1Existe = pessoaRepo.buscarPorId(idPessoa1) != nullptr;
        bool pessoa2Existe = pessoaRepo.buscarPorId(idPessoa2) != nullptr;

        if (!pessoa1Existe && !pessoa2Existe) {
            throw invalid_argument(
                "ID(s) de Pessoa " + idPessoa1 + " " + idPessoa2 +
                " não cadastrado no Lar de ID " + idLar + "."
            );
        }
        if (!pessoa1Existe) {
            throw invalid_argument(
                "ID(s) de Pessoa " + idPessoa1 +
                " não cadastrado no Lar de ID " + idLar + "."
            );
        }
        if (!pessoa2Existe) {
            throw invalid_argument(
                "ID(s) de Pessoa " + idPessoa2 +
                " não cadastrado no Lar de ID " + idLar + "."
            );
        }

        // Endereço do lar
        string rua = campos[3];
        int num = stoi(campos[4]);
        string complemento = campos[5];
        Endereco endereco(rua, num, complemento);

        // 🔹 Busca ou cria o casal associado ao lar
        Casal* casal = casalRepo.buscarPorIdPessoa(idPessoa1);
        if (casal == nullptr) {
            casal = new Casal(idPessoa1, idPessoa2, "", idLar);
            casalRepo.adicionar(casal);
        }

        // Cria e adiciona o lar ao repositório
        Lar* lar = new Lar(idLar, casal, endereco);
        adicionar(lar);
    }
}
} // namespace repository