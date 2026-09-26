//  C:\Tony\DXF\Dxf.cpp
#include "Dxf.hpp"
#include <fstream>
#include <cstring>
#include <stack>

//--PROVISÓRIO PRA MONITORAMENTO--  =========================================================
#include <iostream>
void DXF::Dxf::Valida()
{
    Ler("C:/Tony/Projeto/Inscopia.dxf");
}
//===========================================================================================

//--LINHA-- =================================================================================
void DXF::Linha::getString(std::ifstream *file, char *tx)
{
    file->clear();
    file->seekg(Posi, std::ios::beg);
    size_t i = 0;
    for(size_t j = Posi; j <= Posf; j++)
    {
        if (i >= 127) break;
        tx[i++] = file->get();
    }
    tx[--i] = 0;
    while (i > 0 && (tx[i - 1] == '\r' || tx[i - 1] == '\n'))
    {
        i--;
        tx[i] = '\0';
    }
}

bool DXF::Linha::igual_(std::ifstream *file, Linha &outra)
{
    if(Tipo != outra.Tipo) return false;
    if(Tipo == NULO || outra.Tipo == NULO) return false;
    char tx0[128], tx1[128];
    this->getString(file, tx0);
    outra.getString(file, tx1);
    //f(Tipo == STR) return !strcmp(tx0, tx1);   //  substituir essa linha
    if(Tipo == STR) {
        char* p0 = tx0;
        char* p1 = tx1;
        while(*p0 != '\0' && *p0 <= 32) p0++;
        while(*p1 != '\0' && *p1 <= 32) p1++;
        return (strcmp(p0, p1) == 0);
    }
    if(Tipo == DBL) return (std::stod(tx0) == std::stod(tx1));
    return (std::stoll(tx0) == std::stoll(tx1));
}

bool DXF::Linha::maior_(std::ifstream *file, Linha &outra)
{
    if(Tipo != outra.Tipo) return false;
    if(Tipo == NULO || outra.Tipo == NULO) return false;
    char tx0[128], tx1[128];
    this->getString(file, tx0);
    outra.getString(file, tx1);
    if(Tipo == STR) {
        char* p0 = tx0;
        char* p1 = tx1;
        while(*p0 != '\0' && *p0 <= 32) p0++;
        while(*p1 != '\0' && *p1 <= 32) p1++;
        return (strcmp(p0, p1) > 0);
    }
    if(Tipo == DBL) return (std::stod(tx0) > std::stod(tx1));
    return (std::stoll(tx0) > std::stoll(tx1));
}

DXF::Linha& DXF::Linha::operator =(const Linha outra)
{
    Posi = outra.Posi;
    Posf = outra.Posf;
    Tipo = outra.Tipo;
    return *this;
}
//===========================================================================================

//--DXF--   =================================================================================

size_t DXF::Dxf::QuantLinhas(std::ifstream* file)
{
    file->clear();
    file->seekg(0, std::ios::end);
    size_t fim = (size_t)file->tellg();
    file->clear();
    file->seekg(0, std::ios::beg);
    size_t r = 0;
    char c, d = 0;
    for(size_t i = 0; i < fim; i++)
    {
        c = d;
        d = file->get();
        if((d == 10 || d == 13) && (c != 10 && c != 13)) r++;
    }
    return r;
}

DXF::TpLn DXF::Dxf::getTipo(std::ifstream *file, size_t posi, size_t posf)
{
    file->clear();
    file->seekg(posi, std::ios::beg);
    char tx[64];
    size_t i = 0;
    for(size_t j = posi; j <= posf; j++) tx[i++] = file->get();
    tx[--i] = 0;
    int k = std::stoi(tx);
    if((k >= 10 && k <= 59) || (k >= 110 && k <= 149) || (k >= 210 && k <= 239) || (k >= 460 && k <= 469) ||
        (k >= 1010 && k <= 1059)) return DBL;
    else if((k >= 60 && k <= 79) || (k >= 270 && k <= 289) || (k >= 370 && k <= 389)) return NN16;
    else if((k >= 90 && k <= 99) || (k >= 440 && k <= 459) || (k >= 1060 && k <= 1071)) return NN32;
    else if(k >= 160 && k <= 179) return NN64;
    return STR;
}

std::vector<DXF::Linha> DXF::Dxf::LerLinhas(std::ifstream *file, size_t n)
{
    file->clear();
    file->seekg(0, std::ios::end);
    size_t fim = (size_t)file->tellg();
    file->clear();
    file->seekg(0, std::ios::beg);
    std::vector<Linha> r = {};
    r.reserve(n);
    Linha ln;
    char c, d = 10;
    TpLn proxTipo;
    size_t posi, conta = 0;
    for(size_t i = 0; i < fim; i++)
    {
        c = d;
        d = file->get();
        if((c == 10 || c == 13) && (d != 10 && d != 13))    //  início
            posi = i;
        else if((c != 10 && c != 13) && (d == 10 || d == 13))   //  fim
        {
            if(((i - posi) == 1) && (c < 32)) continue;
            ln.Posi = posi;
            ln.Posf = i;
            if(conta % 2) ln.Tipo = proxTipo;
            else
            {
                proxTipo = getTipo(file, ln.Posi, ln.Posf);
                ln.Tipo = GRUPO;
            }
            r.push_back(ln);
            conta++;
        }
    }
    return r;
}

size_t DXF::Dxf::PivotaStr(std::ifstream *file, std::vector<Linha> &lns, std::vector<size_t> &lst, Trecho tre)
{
    size_t p = tre.head, t = tre.tail, h = tre.head + 1;
    for(size_t i = h; i <= t; i++)
    {
        Linha lni = lns.at(lst.at(i)), lnp = lns.at(lst.at(p));
        if(lnp.maior_(file, lni))
        {
            std::swap(lst.at(i), lst.at(p));
            if(++p != i) std::swap(lst.at(i), lst.at(p));
        }
    }
    return p;
}

void DXF::Dxf::OrdenaStr(std::ifstream *file, std::vector<Linha> &lns, std::vector<size_t> &lst)
{
    std::stack<Trecho> pilha;
    Trecho tr;
    tr.head = 0;
    tr.tail = lst.size() - 1;
    pilha.push(tr);
    while(!pilha.empty())
    {
        size_t h = pilha.top().head, t = pilha.top().tail;
        size_t p = PivotaStr(file, lns, lst, pilha.top());
        pilha.pop();
        if(p > (h + 1))
        {
            tr.head = h;
            tr.tail = p - 1;
            pilha.push(tr);
        }
        if(t > (p - 1))
        {
            tr.head = p + 1;
            tr.tail = t;
            pilha.push(tr);
        }
    }
}

std::vector<size_t> DXF::Dxf::OrdenaStrExclusivo(std::ifstream *file, std::vector<Linha> &lns, std::vector<size_t> &lst)
{
    std::vector<size_t> r = {};
    r.reserve(lst.size());
    OrdenaStr(file, lns, lst);
    r.push_back(lst[0]);
    for(auto ie : lst)
    {
        char tx[128], txa[128];
        Linha e = lns[ie], e0 = lns[r[r.size() - 1]];
        e.getString(file, tx);
        e0.getString(file, txa);
        if(!std::strcmp(tx, txa)) continue;
        r.push_back(ie);
    }
    return r;
}

void DXF::Dxf::Ler(const std::filesystem::path &nome)
{
    //--Abrir o arquivo--
    std::ifstream arq(nome, std::ios::in | std::ios::binary | std::ios::ate);
    if(!arq.is_open()) return;
    //=======================================================================================

    //--Criar a variável local de quantidades--
    std::vector<size_t> tamanhos;
    tamanhos.reserve(16);
    tamanhos.push_back((size_t)arq.tellg());    //  tamanhos[0] <- número de caracteres
    if(tamanhos.at(0) <= 0) return;
    tamanhos.push_back(QuantLinhas(&arq));    //  tamanhos[1] <- número de strings
    //=======================================================================================

    //--Povoar linhas--
    std::vector<Linha> linhas = {};
    linhas.reserve(tamanhos.at(1));
    linhas = LerLinhas(&arq, tamanhos.at(1));
    //=======================================================================================

    //--Fazer dicionário de strings--
    //  --Adquirir a quantidade de linhas tipo STR--
    tamanhos.push_back(0);    //  tamanhos[2] <- número de dados a serem armazenados como strings
    for(auto ln : linhas) if(ln.Tipo == STR) tamanhos.at(2)++;
    //=======================================================================================

    //  --Adquirir as linhas tipo STR--
    std::vector<size_t> iStrs = {};
    iStrs.reserve(tamanhos.at(2));
    for(size_t i = 0; i < linhas.size(); i++) if(linhas.at(i).Tipo == STR) iStrs.push_back(i);
    //=======================================================================================

    //  --Ordenar e remover duplicatas de strings--
    std::vector<size_t> ordenado = {};
    ordenado = OrdenaStrExclusivo(&arq, linhas, iStrs);
    ordenado.shrink_to_fit();
    //=======================================================================================

    //  --Apresentar lista ordenada--
    for(size_t i = 0; i < ordenado.size(); i++)
    {
        char tx[64];
        linhas.at(ordenado.at(i)).getString(&arq, tx);
        std::cout << i << '\t' << iStrs[i] << '\t' << '\'' << tx << '\'' << std::endl;
    }
    //=======================================================================================

    //  --Adquirir o número de caracteres do buffer de char e carregar dicionário--
    tamanhos.push_back(ordenado.size());    //  tamanhos[3] <- número de strings no dicionário
    dicioStr.dict.reserve(tamanhos.at(3) + 1);
    dicioStr.dict.push_back(0);
    tamanhos.push_back(0);    //  tamanhos[4] <- número de caracteres no dicionário
    for(size_t qc : ordenado)
    {
        char tx[64];
        linhas.at(qc).getString(&arq, tx);
        tamanhos[4] += (size_t)std::strlen(tx);
        dicioStr.dict.push_back(tamanhos[4]);
    }
    dicioStr.buffer.reserve(tamanhos[4]);
    for(size_t qc : ordenado)
    {
        char tx[64];
        linhas.at(qc).getString(&arq, tx);
        for(size_t i = 0; i < std::strlen(tx); i++) dicioStr.buffer.push_back(tx[i]);
    }
    //=======================================================================================

    //  --Apresentar dicionário--
    for(size_t i = 1; i < dicioStr.dict.size(); i++)
    {
        char tx[64];
        dicioStr.getStr(i, tx);
        std::cout << '\'' << tx << '\'' << std::endl;
    }
    //=======================================================================================

    //  --Ordenar as linhas tipo STR--
    //{
    //  [0 - chrs no arquivo],
    //  [1 - linhas no arquivo],
    //  [2 - strings],
    //  [3 - chars de string],
    //  [3 - doubles],
    //  [4 - uint16],
    //  [5 - uint32],
    //  [6 - uint64],
    //  [8 - linhas nulas],
    //  [9 - linhas de grupo]
    //}
}
//===========================================================================================