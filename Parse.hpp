//  C:/Tony/Dxf/Parse.hpp
#pragma once
#include <filesystem>
#include <fstream>
#include <limits>
#include "Util.hpp"

namespace ARQ
{
class Arquivo
{
private:
    std::ifstream mFonte;
    size_t mTotalCh = 0;
    size_t mTotalLn = 0;
    std::vector<size_t> mIndices = {};
    bool Ler(const std::filesystem::path &fonte);
public:
    Arquivo() = default;
    bool SetArquivo(const std::filesystem::path &fonte){return Ler(fonte);}
    size_t GetChTotal(){return mTotalCh;}
    size_t GetLnTotal(){return mTotalLn;}
    std::vector<size_t> GetIndices(){return mIndices;}
    std::vector<char> GetChars();
    std::vector<char> GetLinha(size_t n);
    std::vector<char> GetLinha(size_t n, char *tx);
    bool LnIgual(size_t m, size_t n);   //  ln(m) = ln(n)
    bool LnMenor(size_t m, size_t n);   //  ln(m) < ln(n)
    bool LnMaior(size_t m, size_t n);   //  ln(m) > ln(n)
};
} // namespace ARQ

//--EXEMPLO DE USO--
/*

void tParse()
{
    ARQ::Arquivo arq;
    arq.SetArquivo("C:/Tony/Soft/cpp/integrado/sistemaIntegrado/CPP/INSTANCIA/TMP/20260710152627.tmp");
    std::cout << "Sao " << arq.GetLnTotal() << " linhas, compostas por " << arq.GetChTotal() << " caracteres." << std::endl;
    std::cout << "As dez primeiras linhas sao:\n";
    for(size_t i = 0; i < 10; i++)
    {
        char txt[256] = {};
        arq.GetLinha(i, txt);
        std::cout << txt << std::endl;
    }
    std::cout << "...\n ..\n  .\ne dez ultimas:\n";
    size_t lnTotal = arq.GetLnTotal();
    for(size_t i = (lnTotal - 10); i < lnTotal; i++)
    {
        char txt[256] = {};
        arq.GetLinha(i, txt);
        std::cout << txt << std::endl;
    }
    std::vector<size_t> indices = {};
    indices.reserve(lnTotal);
    indices = arq.GetIndices();
    std::cout << ".\n..\n...\nOs dez primeiros indices de linhas sao:\n";
    for(size_t i = 0; i < 10; i++)std::cout << indices[i] << ' ';
    std::cout << "\n(os fins de linha no arquivo)\n...\n..\n.\n";
}

*/
//==============================================================================================================================================
