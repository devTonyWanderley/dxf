//  C:/Tony/Dxf/Parse.hpp
#pragma once
#include <filesystem>
#include <fstream>
#include <cstdlib>
#include <cstdint>
#include <limits>
#include <vector>

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
};
} // namespace ARQ