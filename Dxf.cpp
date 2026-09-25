//  C:\Tony\DXF\Dxf.cpp
#include "Dxf.hpp"
#include <fstream>
#include <cstring>

#include <iostream>
void DXF::Dxf::Valida()
{
    Ler("C:/Tony/Projeto/Inscopia.dxf");
}

void DXF::Linha::getString(std::ifstream *file, char *tx)
{
    file->seekg(Posi, std::ios::beg);
    size_t i = 0;
    for(size_t j = Posi; j <= Posf; j++) tx[i++] = file->get();
    tx[--i] = 0;
}

bool DXF::Linha::igual_(std::ifstream *file, Linha &outra)
{
    if(Tipo != outra.Tipo) return false;
    if(Tipo == NULO || outra.Tipo == NULO) return false;
    char tx0[128], tx1[128];
    this->getString(file, tx0);
    outra.getString(file, tx1);
    if(Tipo == STR) return !strcmp(tx0, tx1);
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
    if(Tipo == STR) return (strcmp(tx0, tx1) > 0);
    if(Tipo == DBL) return (std::stod(tx0) > std::stod(tx1));
    return (std::stoll(tx0) > std::stoll(tx1));
}

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

void DXF::Dxf::Ler(const std::filesystem::path &nome)
{
    //--Abrir o arquivo--
    std::ifstream arq(nome, std::ios::in | std::ios::binary | std::ios::ate);
    if(!arq.is_open()) return;
    //--Criar a variável local de quantidades--
    std::vector<size_t> tamanhos;
    tamanhos.reserve(16);
    tamanhos.push_back((size_t)arq.tellg());
    if(tamanhos.at(0) <= 0) return;
    tamanhos.push_back(QuantLinhas(&arq));
    //--Apresentar primeiros tamanhos--
    std::cout << tamanhos.at(0) << " caracteres em " << tamanhos.at(1) << " linhas" << std::endl;
    //--Povoar linhas--
    std::vector<Linha> linhas = {};
    linhas.reserve(tamanhos.at(1));
    linhas = LerLinhas(&arq, tamanhos.at(1));
    //--Apresentar linhas brutas--
    /*
    for(auto ln : linhas)
    {
        char texto[64];
        ln.getString(&arq, texto);
        if(ln.Tipo == GRUPO)
            std::cout << std::stoi(texto) << '\t';
        else
            std::cout << texto << '\t' << (int)ln.Tipo << std::endl;
    }

    char texto[64], outrotx[64];
    linhas.at(1).getString(&arq, texto);
    linhas.at(3).getString(&arq, outrotx);
    std::cout << "Linha 1, \'" << texto << "\', eh ";
    if(linhas.at(1).igual_(&arq, linhas.at(3)))
        std::cout << "igual a ";
    else
        std::cout << "diferente de ";
    std::cout << "linha 3, \'" << outrotx << "\'." << std::endl;
    linhas.at(31).getString(&arq, outrotx);
    std::cout << "Linha 1, \'" << texto << "\', eh ";
    if(linhas.at(1).igual_(&arq, linhas.at(31)))
        std::cout << "igual a ";
    else
        std::cout << "diferente de ";
    std::cout << "linha 31, \'" << outrotx << "\'." << std::endl;
    */
    //--Fazer dicionário de strings--
    //  --Adquirir a quantidade de linhas tipo STR
    tamanhos.push_back(0);
    for(auto ln : linhas) if(ln.Tipo == STR) tamanhos.at(2)++;
    std::cout << "Temos " << tamanhos.at(2) << " strings!" << std::endl;
    //  --Adquirir as linhas tipo STR
    std::vector<size_t> iStrs;
    iStrs.reserve(tamanhos.at(2));
    for(size_t i = 0; i < linhas.size(); i++)
        if(linhas.at(i).Tipo == STR) iStrs.push_back(i);
    for(size_t i = 0; i < iStrs.size(); i++)
    {
        char tx[64];
        linhas.at(iStrs.at(i)).getString(&arq, tx);
        std::cout << iStrs.at(i) << " .. \'" << tx << '\'' << std::endl;
    }
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