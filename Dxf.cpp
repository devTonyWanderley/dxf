#include "Dxf.hpp"
#include <cstring>

#include <iostream>

bool DXF::Dxf::LerArquivo(const std::filesystem::path &fonte)
{
    //--Ler o arquivo--
    ARQ::Arquivo arq;
    if(!arq.SetArquivo(fonte)) return false;
    //======================================

    //--Obter a quantidade de linhas por tipo--
    size_t m = arq.GetLnTotal();
    size_t ttDbl, ttI16, ttI32, ttI64, ttStr, ttHnd, ttBoo, ttBin;
    ttDbl = ttI16 = ttI32 = ttI64 = ttStr = ttHnd = ttBoo = ttBin = 0;
    for(size_t i = 0, j = 0; i < m; i++, j++)
    {
        std::vector<char> tx = arq.GetLinha(i);
        if(tx.size() == 1 && tx[0] < 32) j++;
        if(tx.size() == 1 && tx[0] < 32) continue;
        if(!(j % 2)) continue;
        char str[16];
        size_t k = 0, l = 0;
        for(; k < tx.size(); k++) if(tx[k] >= '0' && tx[k] <= '9') str[l++] = tx[k];
        str[l] = 0;
        std::cout << str;
        if(!strlen(str)) continue;
        int v = std::stoi(str);
        if((v == 5 || v == 105) || (v > 319 && v < 371)) ttHnd++;
        else if((v < 10) || (v == 100 || v == 102) || (v > 299 && v < 310) || (v > 429 && v < 440) ||
                 (v > 469 && v < 480) || (v > 999 && v < 1004)) ttStr++;
        else if((v > 9 && v < 60) || (v > 109 && v < 150) || (v > 209 && v < 240) ||
                 (v > 459 && v < 470) || (v > 1009 && v < 1060)) ttDbl++;
        else if((v > 59 && v < 80) || (v > 269 && v < 290) || (v > 369 && v < 390)) ttI16++;
        else if((v > 89 && v < 100) || (v > 439 && v < 460) || (v > 1059 && v < 1072)) ttI32++;
        else if(v > 159 && v < 170) ttI64++;
        else if(v > 289 && v < 300) ttBoo++;
        else if((v > 309 && v < 320) || v == 1004) ttBin++;
    }
    std::cout << ttDbl << '\t' << ttI16 << '\t' << ttI32 << '\t' << ttI64 << '\t'
              << ttStr << '\t' << ttHnd << '\t' << ttBoo << '\t' << ttBin << std::endl;
    //======================================
    return true;
}