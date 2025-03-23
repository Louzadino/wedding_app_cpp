#include "PessoaRepository.hpp"

#include "util/CSVReader.hpp"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <locale>
#include <stdexcept>
#include <ctime>

using namespace std;

namespace repository {

PessoaRepository::PessoaRepository() {}

// Destrutor
PessoaRepository::~PessoaRepository() {
    for (auto& par : pessoas) {
        delete par.second;
    }
    pessoas.clear(); // Limpa o mapa após a deleção
    cpfs.clear();
    cnpjs.clear();
}

void PessoaRepository::adicionar(Pessoa* pessoa) {

    if (pessoa == nullptr)
        throw invalid_argument("A pessoa não pode ser nula.");

    if (pessoas.find(pessoa->getIdPessoa()) != pessoas.end()) 
        throw invalid_argument("Já existe uma pessoa com este ID no repositório.");

    PessoaFisica* pf = dynamic_cast<PessoaFisica*>(pessoa);
    
    if (pf) {
        // 🔹 Validação para Pessoa Física (CPF)
        if (auto* pf = dynamic_cast<PessoaFisica*>(pessoa)) {
            if (cpfs.find(pf->getCpf()) != cpfs.end() && cpfs[pf->getCpf()] != pf->getIdPessoa()) {
                throw invalid_argument("O CPF " + pf->getCpf() + " da Pessoa " + pf->getIdPessoa() + " é repetido.");
            }
            cpfs[pf->getCpf()] = pf->getIdPessoa(); // Adiciona ao mapa de CPFs
        }

        // 🔹 Validação para Pessoa Jurídica e Loja (CNPJ)
        if (auto* pj = dynamic_cast<PessoaJuridica*>(pessoa)) {
            if (cnpjs.find(pj->getCnpj()) != cnpjs.end() && cnpjs[pj->getCnpj()] != pj->getIdPessoa()) {
                throw invalid_argument("O CNPJ " + pj->getCnpj() + " da Pessoa " + pj->getIdPessoa() + " é repetido.");
            }
            cnpjs[pj->getCnpj()] = pj->getIdPessoa(); // Adiciona ao mapa de CNPJs
        }
    }

    pessoas[pessoa->getIdPessoa()] = pessoa;
}

void PessoaRepository::remover(Pessoa* pessoa) {
    if (pessoa == nullptr) {
        throw invalid_argument("A pessoa não pode ser nula.");
    }
    if (pessoas.find(pessoa->getIdPessoa()) == pessoas.end()) {
        throw invalid_argument("A pessoa não existe no repositório.");
    }

    // Remove dos mapas auxiliares de CPF/CNPJ
    if (auto* pf = dynamic_cast<PessoaFisica*>(pessoa)) {
        cpfs.erase(pf->getCpf());
    } else if (auto* pj = dynamic_cast<PessoaJuridica*>(pessoa)) {
        cnpjs.erase(pj->getCnpj());
    }

    pessoas.erase(pessoa->getIdPessoa());
}

vector<Pessoa*> PessoaRepository::listar() const {
    vector<Pessoa*> lista;
    for (const auto& item : pessoas) {
        lista.push_back(item.second);
    }
    return lista;
}

Pessoa* PessoaRepository::buscarPorId(const string& id) const {
    if (id.empty()) {
        throw invalid_argument("O ID não pode ser nulo ou vazio.");
    }
    auto it = pessoas.find(id);
    if (it != pessoas.end()) {
        return it->second;
    }
    return nullptr;
}

void PessoaRepository::carregarDados(const string& caminhoArquivo) {
    vector<vector<string>> linhas = CSVReader::lerCSV(caminhoArquivo);

    locale brasilianLocale("pt_BR.UTF-8");

    for (const auto& campos : linhas) {
        if (campos.size() < 5) {
            cout << "Linha inválida encontrada, ignorando: ";
            for (const auto& campo : campos) {
                cout << campo << ";";
            }
            cout << endl;
            continue;
        }

        string id = campos[0];

        if (pessoas.find(id) != pessoas.end()) {
            throw invalid_argument("ID repetido " + id + " na classe Pessoa.");
        }

        string tipo = campos[1];
        string nome = campos[2];
        string telefone = campos[3];
        string endereco = campos[4];

        if (tipo == "F") { // Pessoa Física
            if (campos.size() < 10) continue;

            string cpf = campos[5];

            // Converte a data usando o formato dd/MM/yyyy
            struct tm dataNascimento = {};
            istringstream dataStream(campos[6]);
            dataStream >> get_time(&dataNascimento, "%d/%m/%Y");
            if (dataStream.fail()) {
                throw invalid_argument("Erro ao converter data para pessoa com ID " + id);
            }

            istringstream dinheiroStream(campos[7]);
            dinheiroStream.imbue(brasilianLocale);
            double dinheiroPoupanca;
            dinheiroStream >> dinheiroPoupanca;

            istringstream salarioStream(campos[8]);
            salarioStream.imbue(brasilianLocale);
            double salarioLiquido;
            salarioStream >> salarioLiquido;

            istringstream gastosStream(campos[9]);
            gastosStream.imbue(brasilianLocale);
            double gastosMensais;
            gastosStream >> gastosMensais;

            Financeiro financeiro(dinheiroPoupanca, salarioLiquido, gastosMensais);

            PessoaFisica* pessoaFisica = new PessoaFisica(nome, telefone, endereco, cpf, dataNascimento, financeiro, id);

            if (cpfs.find(cpf) != cpfs.end() && cpfs[cpf] != id) {
                delete pessoaFisica;
                throw invalid_argument("O CPF " + cpf + " da Pessoa " + id + " é repetido.");
            }
            adicionar(pessoaFisica);
        } 
        else if (tipo == "J" || tipo == "L") { // Pessoa Jurídica ou Loja
            if (campos.size() < 6) continue;

            string cnpj = campos[5];

            if (tipo == "J") {
                PessoaJuridica* pessoaJuridica = new PessoaJuridica(nome, telefone, endereco, cnpj, id);
            
                // Usa .find() e .at() sem criar valor automaticamente
                auto it = cnpjs.find(cnpj);
                if (it != cnpjs.end() && it->second != id) {
                    delete pessoaJuridica;
                    throw invalid_argument("O CNPJ " + cnpj + " da Pessoa " + id + " é repetido.");
                }
            
                // Força o valor no mapa sem otimização
                cnpjs.insert({cnpj, id});
            
                adicionar(pessoaJuridica);
            } else if (tipo == "L") {
                Loja* loja = new Loja(nome, telefone, endereco, cnpj, id);
                adicionar(loja);
            } else {
                throw invalid_argument("Tipo de pessoa inválido encontrado: " + tipo);
            }
            
        }
            
    }
}

} // namespace repository
