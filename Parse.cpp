#include "Parse.hpp"

bool ARQ::Arquivo::Ler(const std::filesystem::path &nome)
{
    //--Abrir o arquivo--
    mFonte.open(nome, std::ios::in | std::ios::binary | std::ios::ate);
    if(!mFonte.is_open()) return false;
    //=======================================================================================

    //--Adquirir quantidade de caracteres--
    mTotalCh = (size_t)mFonte.tellg() - 1;
    if(!mTotalCh) return false;
    //=======================================================================================

    //--Adquirir quantidade de linhas--
    mFonte.clear();
    mFonte.seekg(0, std::ios::beg);
    char c, d = 32;
    mTotalLn = 0;
    for(size_t i = 0; i < mTotalCh; i++)
    {
        c = d;
        d = mFonte.get();
        if((d == 10 || d == 13) && (c != 10 && c != 13)) mTotalLn++;
    }
    //=======================================================================================

    //--Adquirir índices--
    mFonte.clear();
    mFonte.seekg(0, std::ios::beg);
    mIndices.reserve(mTotalLn);
    d = 32;
    for(size_t i = 0; i < mTotalCh; i++)
    {
        c = d;
        d = mFonte.get();
        if((d == 10 || d == 13) && (c != 10 && c != 13) && (i > 0)) mIndices.push_back(i);
    }
    //=======================================================================================
    return true;
}

std::vector<char> ARQ::Arquivo::GetChars()
{
    std::vector<char> r = {};
    if(!mTotalCh) return r;
    r.reserve(mTotalCh);
    mFonte.clear();
    mFonte.seekg(0, std::ios::beg);
    for(size_t i = 0; i < mTotalCh; i++) r.push_back(mFonte.get());
    return r;
}

std::vector<char> ARQ::Arquivo::GetLinha(size_t n)
{
    std::vector<char> r = {};
    if(!mTotalLn) return r;
    if(n >= mIndices.size()) n = mIndices.size() - 1;
    size_t posi, posf;
    posf = mIndices[n];
    if(n == 0) posi = 0;
    else posi = mIndices[n - 1];
    mFonte.clear();
    mFonte.seekg(posi, std::ios::beg);
    for(size_t i = posi; i < posf; i++) //  **acredito que é irrelevante reservar espaço pro r**
    {
        char c = mFonte.get();
        if(c != 10 && c != 13) r.push_back(c);
    }
    return r;
}

std::vector<char> ARQ::Arquivo::GetLinha(size_t n, char *tx)
{
    tx[0] = 0;
    std::vector<char> r = GetLinha(n);
    size_t i = 0;
    for(; i < r.size(); i++) tx[i] = r[i];
    tx[i] = 0;
    return r;
}