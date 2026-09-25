//  C:\Tony\DXF\main.cpp
#include <iostream>
#include "Dxf.hpp"
#include <queue>

// teste de swap de elementos
void troca()
{
    struct indices
    {
        std::size_t pivot, n;
    };
    //std::queue<indices> guia;
    std::vector<int> v = {2, 9, 5, 4, 6, 0, 7, 3, 8, 1};
    indices iJob;
    iJob.pivot = 0;
    iJob.n = v.size();
    for(size_t i = iJob.pivot + 1; i < iJob.n; i++)
    {
        if(v[i] < v[iJob.pivot])
        {
            std::swap(v[i], v[iJob.pivot]);
            iJob.pivot++;
            if(i != iJob.pivot) std::swap(v[i], v[iJob.pivot]);
        }
    }
    for(auto vv : v) std::cout << ' ' << vv;
    std::cout << std::endl;
}

void ordenaTrecho(std::vector<char> &lst, size_t &pvt, size_t n)
{
    for(size_t i = (pvt + 1); i < n; i++)
    {
        if(lst[i] < lst[pvt])
        {
            std::swap(lst[i], lst[pvt]);
            pvt++;
            if(pvt != i) std::swap(lst[i], lst[pvt]);
        }
    }
}

void chama(std::vector<char> &lst, size_t &pvt, size_t n)
{
    std::cout << '[' << pvt << ',' << n << "]:";
    ordenaTrecho(lst, pvt, n);
    for(auto v : lst) std::cout << ' ' << v;
    std::cout << '\t' << pvt << std::endl;
}

void ordena()
{}

int main()
{
    //DXF::Dxf teste;
    //teste.Valida();
    //troca();
    //ordena();
    std::vector<char> v = {'V', '2', 'I', 'N', '9', 'C', 'Q', 'O', '5', 'J', '4', 'Z', 'T', 'G', '6', 'R', '1', 'D', 'A', 'L',
                           'M', '7', 'H', '3', 'E', 'S', 'F', 'P', 'U', 'B', 'Y', '8', '0', 'X', 'K'};
    std::cout << "Original:";
    for(size_t i = 0; i < v.size(); i++) std::cout << ' ' << v[i];
    std::cout << std::endl;
    size_t p = 0, q = v.size();
    chama(v, p, q); //  31

    q = p;
    p = 0;
    chama(v, p, q); //  2 31

    q = p;
    p = 0;
    chama(v, p, q); //  1 2 31
    //  ! q-p <= 2 !!!

    q = 31;
    p = 3;
    chama(v, p, q); //  9 1 2 31

    q = p;
    p = 3;
    chama(v, p, q); //  5 9 1 2 31

    q = p;
    p = 3;
    chama(v, p, q); //  4 5 9 1 2 31
    //  ! q-p <= 2 !!!

    q = 9;
    p = 6;
    chama(v, p, q); //  7 4 5 9 1 2 31

    q = 9;
    p = 7;
    chama(v, p, q); //  7 7 4 5 9 1 2 31

    q = 31;
    p = 10;
    chama(v, p, q); //  29 7 7 4 5 9 1 2 31

    q = p;
    p = 10;
    chama(v, p, q); //  16 29 7 7 4 5 9 1 2 31

    q = p;
    p = 10;
    chama(v, p, q); //  13 16 29 7 7 4 5 9 1 2 31

    q = p;
    p = 10;
    chama(v, p, q); //  10 13 16 29 7 7 4 5 9 1 2 31

    p = 11;
    chama(v, p, q); //  12 10 13 16 29 7 7 4 5 9 1 2 31

    q = 16;
    p = 13;
    chama(v, p, q); //  13 12 10 13 16 29 7 7 4 5 9 1 2 31

    p = 14;
    chama(v, p, q); //  15 13 12 10 13 16 29 7 7 4 5 9 1 2 31

    q = 29;
    p = 17;
    chama(v, p, q); //  22 15 13 12 10 13 16 29 7 7 4 5 9 1 2 31

    q = p;
    p = 17;
    chama(v, p, q); //  18 22 15 13 12 10 13 16 29 7 7 4 5 9 1 2 31

    q = 22;
    p = 19;
    chama(v, p, q); //  19 18 22 15 13 12 10 13 16 29 7 7 4 5 9 1 2 31

    q = 22;
    p = 20;
    chama(v, p, q); //  21 19 18 22 15 13 12 10 13 16 29 7 7 4 5 9 1 2 31

    q = 29;
    p = 23;
    chama(v, p, q); //  27 21 19 18 22 15 13 12 10 13 16 29 7 7 4 5 9 1 2 31

    q = p;
    p = 23;
    chama(v, p, q); //  25 27 21 19 18 22 15 13 12 10 13 16 29 7 7 4 5 9 1 2 31

    q = p;
    p = 24;
    chama(v, p, q); //  24 25 27 21 19 18 22 15 13 12 10 13 16 29 7 7 4 5 9 1 2 31

    q = v.size();
    p = 32;
    chama(v, p, q); //  33 24 25 27 21 19 18 22 15 13 12 10 13 16 29 7 7 4 5 9 1 2 31

    q = v.size();
    p = 34;
    chama(v, p, q); //  34 33 24 25 27 21 19 18 22 15 13 12 10 13 16 29 7 7 4 5 9 1 2 31

    q = v.size();
    p = 35;
    chama(v, p, q); //  35 34 33 24 25 27 21 19 18 22 15 13 12 10 13 16 29 7 7 4 5 9 1 2 31
    /*
    ordenaTrecho(v, p, q);  // [0, full]
    std::cout << "Ordenado:";
    for(size_t i = 0; i < v.size(); i++) std::cout << ' ' << v[i];
    std::cout << " [pivot: " << p << ']' << std::endl;
    //  sai 2

    q = p;
    p = 0;
    ordenaTrecho(v, p, q);  // [0, 2]
    std::cout << "Ordenado:";
    for(size_t i = 0; i < v.size(); i++) std::cout << ' ' << v[i];
    std::cout << " [pivot: " << p << ']' << std::endl;
    //  sai 1

    q = v.size();
    p = 3;
    ordenaTrecho(v, p, q);  // [3, full]
    std::cout << "Ordenado:";
    for(size_t i = 0; i < v.size(); i++) std::cout << ' ' << v[i];
    std::cout << " [pivot: " << p << ']' << std::endl;
    //  sai 9

    q = p;
    p = 3;
    ordenaTrecho(v, p, q);  // [3, 9]
    std::cout << "Ordenado:";
    for(size_t i = 0; i < v.size(); i++) std::cout << ' ' << v[i];
    std::cout << " [pivot: " << p << ']' << std::endl;
    //  sai 5

    q = p;
    p = 3;
    ordenaTrecho(v, p, q);  // [3, 5]
    std::cout << "Ordenado:";
    for(size_t i = 0; i < v.size(); i++) std::cout << ' ' << v[i];
    std::cout << " [pivot: " << p << ']' << std::endl;
    //  sai 4

    q = 9;
    p = 6;
    ordenaTrecho(v, p, q);  // [6, 9]
    std::cout << "Ordenado:";
    for(size_t i = 0; i < v.size(); i++) std::cout << ' ' << v[i];
    std::cout << " [pivot: " << p << ']' << std::endl;
    //  sai 7

    q = 9;
    p = 7;
    ordenaTrecho(v, p, q);  // [7, 9]
    std::cout << "Ordenado:";
    for(size_t i = 0; i < v.size(); i++) std::cout << ' ' << v[i];
    std::cout << " [pivot: " << p << ']' << std::endl;
    //  sai 7

    q = v.size();
    p = 10;
    ordenaTrecho(v, p, q);  // [10, full]
    std::cout << "Ordenado:";
    for(size_t i = 0; i < v.size(); i++) std::cout << ' ' << v[i];
    std::cout << " [pivot: " << p << ']' << std::endl;
    //  sai 16

    q = 16;
    p = 10;
    ordenaTrecho(v, p, q);  // [10, 16]
    std::cout << "Ordenado:";
    for(size_t i = 0; i < v.size(); i++) std::cout << ' ' << v[i];
    std::cout << " [pivot: " << p << ']' << std::endl;
    //  sai 12

    q = 12;
    p = 10;
    ordenaTrecho(v, p, q);  // [10, 12]
    std::cout << "Ordenado:";
    for(size_t i = 0; i < v.size(); i++) std::cout << ' ' << v[i];
    std::cout << " [pivot: " << p << ']' << std::endl;
    //  sai 10

    q = 16;
    p = 13;
    ordenaTrecho(v, p, q);  // [13, 16]
    std::cout << "Ordenado:";
    for(size_t i = 0; i < v.size(); i++) std::cout << ' ' << v[i];
    std::cout << " [pivot: " << p << ']' << std::endl;
    //  sai 14

    q = 16;
    p = 14;
    ordenaTrecho(v, p, q);  // [14, 16]
    std::cout << "Ordenado:";
    for(size_t i = 0; i < v.size(); i++) std::cout << ' ' << v[i];
    std::cout << " [pivot: " << p << ']' << std::endl;
    //  sai 14

    q = v.size();
    p = 17;
    ordenaTrecho(v, p, q);  // [17, full]
    std::cout << "Ordenado:";
    for(size_t i = 0; i < v.size(); i++) std::cout << ' ' << v[i];
    std::cout << " [pivot: " << p << ']' << std::endl;
    //  sai 21

    q = 21;
    p = 17;
    ordenaTrecho(v, p, q);  // [17, 21]
    std::cout << "Ordenado:";
    for(size_t i = 0; i < v.size(); i++) std::cout << ' ' << v[i];
    std::cout << " [pivot: " << p << ']' << std::endl;
    //  sai 17

    q = 21;
    p = 18;
    ordenaTrecho(v, p, q);  // [18, 21]
    std::cout << "Ordenado:";
    for(size_t i = 0; i < v.size(); i++) std::cout << ' ' << v[i];
    std::cout << " [pivot: " << p << ']' << std::endl;
    //  sai 18

    q = 21;
    p = 19;
    ordenaTrecho(v, p, q);  // [19, 21]
    std::cout << "Ordenado:";
    for(size_t i = 0; i < v.size(); i++) std::cout << ' ' << v[i];
    std::cout << " [pivot: " << p << ']' << std::endl;
    //  sai 19

    q = v.size();
    p = 22;
    ordenaTrecho(v, p, q);  // [22, full]
    std::cout << "Ordenado:";
    for(size_t i = 0; i < v.size(); i++) std::cout << ' ' << v[i];
    std::cout << " [pivot: " << p << ']' << std::endl;
    //  sai 24

    q = v.size();
    p = 24;
    ordenaTrecho(v, p, q);  // [24, full]
    std::cout << "Ordenado:";
    for(size_t i = 0; i < v.size(); i++) std::cout << ' ' << v[i];
    std::cout << " [pivot: " << p << ']' << std::endl;
    //  sai 24
    */
    return 0;
}

/*
 * # Códigos de Grupo DXF - Tipo de Dados por Intervalo

Este documento serve como referência rápida para o desenvolvimento de parsers e geradores de arquivos DXF
(Drawing Exchange Format).
Os códigos de grupo determinam o tipo de dado (formato) do valor que o sucede na linha seguinte.

## 📝 Strings (Textos e Identificadores)

| Intervalo de Códigos | Tipo de Dado | Descrição Comum |
| :--- | :--- | :--- |
| **0 a 9** | String | Nomes de entidades, camadas, blocos, estilos ou textos principais |
| **100** | String | Marcador de subclasse (limite de classe de dados) |
| **102** | String | Início/fim de grupos de aplicação ("{nome_aplicacao" ou "}") |
| **300 a 309** | String | Textos arbitrários (comentários, dados de terceiros ou IDs textuais) |
| **430 a 439** | String | Nomes de cores provenientes de catálogos (ex: PANTONE) |
| **470 a 479** | String | Identificadores de propriedades e estados do sistema |
| **1000 a 1003** | String | Dados estendidos de aplicação (XDATA) |

## 🔢 Números Decimais (Ponto Flutuante / Double)

| Intervalo de Códigos | Tipo de Dado | Descrição Comum |
| :--- | :--- | :--- |
| **10 a 59** | Double | Coordenadas geométricas iniciais (X/Y/Z), raios, distâncias e ângulos |
| **110 a 149** | Double | Coordenadas de vetores de direção, pontos de controle ou matrizes |
| **210 a 239** | Double | Vetores de direção de extrusão espacial (X/Y/Z) |
| **460 a 469** | Double | Valores de ponto flutuante de precisão dupla (escalas, opacidade) |
| **1010 a 1059** | Double | Valores de ponto flutuante em dados estendidos (XDATA) |

## 📐 Números Inteiros (Integer)

| Intervalo de Códigos | Tipo de Dado | Tamanho / Descrição |
| :--- | :--- | :--- |
| **60 a 79** | Inteiro de 16-bit | Flags de status, modos visíveis, contadores e booleanos |
| **90 a 99** | Inteiro de 32-bit | Inteiros longos (identificadores numéricos, contagens de vértices) |
| **160 a 169** | Inteiro de 64-bit | Grandes contadores de objetos ou timestamps do sistema |
| **270 a 289** | Inteiro de 16-bit | Flags curtas de configuração e estados de exibição |
| **370 a 379** | Inteiro de 16-bit | Índices de espessura de linha (Lineweight) |
| **380 a 389** | Inteiro de 16-bit | Métodos e flags de plotagem |
| **440 a 449** | Inteiro de 32-bit | Valores inteiros para transparência de cor |
| **450 a 459** | Inteiro de 32-bit | Atributos internos do sistema (renderização e hachuras) |
| **1060 a 1071** | Inteiro | Valores inteiros de 16 e 32 bits em dados estendidos (XDATA) |

## 🔑 Identificadores Hexadecimais (Handles / IDs de Ponteiro)

| Intervalo de Códigos | Tipo de Dado | Descrição Comum |
| :--- | :--- | :--- |
| **5** | String Hex | Handle único do objeto/entidade dentro do desenho |
| **105** | String Hex | Handle de tabela gráfica (idêntico ao código 5 em tabelas) |
| **320 a 329** | String Hex | Referências de ponteiros para handles de objetos proprietários |
| **330 a 339** | String Hex | ID de ponteiro (Soft pointer) para objetos contêineres/pais |
| **340 a 349** | String Hex | ID de ponteiro (Hard pointer) para objetos associados |
| **350 a 359** | String Hex | ID de referência direta (Soft owner) |
| **360 a 369** | String Hex | ID de referência direta (Hard owner) |

## 🧬 Binários e Booleanos

| Intervalo de Códigos | Tipo de Dado | Descrição Comum |
| :--- | :--- | :--- |
| **290 a 299** | Booleano | Flags de verdadeiro/falso (`0` ou `1`) |
| **310 a 319** | Binário (Hex) | Blocos de dados binários brutos (imagens, dados embutidos) |
| **1004** | Binário (Hex) | Dados binários em blocos estendidos (XDATA) |

--Em ordem--
| Intervalo de Códigos | Tipo de Dado | Descrição Comum |
| :--- | :--- | :--- |
| **0 a 4** | String | Nomes de entidades, camadas, blocos, estilos ou textos principais |
| **5** | String Hex | Handle único do objeto/entidade dentro do desenho |
| **6 a 9** | String | Nomes de entidades, camadas, blocos, estilos ou textos principais |
| **10 a 59** | Double | Coordenadas geométricas iniciais (X/Y/Z), raios, distâncias e ângulos |
| **60 a 79** | Inteiro de 16-bit | Flags de status, modos visíveis, contadores e booleanos |
| **90 a 99** | Inteiro de 32-bit | Inteiros longos (identificadores numéricos, contagens de vértices) |
| **100** | String | Marcador de subclasse (limite de classe de dados) |
| **102** | String | Início/fim de grupos de aplicação ("{nome_aplicacao" ou "}") |
| **105** | String Hex | Handle de tabela gráfica (idêntico ao código 5 em tabelas) |
| **110 a 149** | Double | Coordenadas de vetores de direção, pontos de controle ou matrizes |
| **160 a 169** | Inteiro de 64-bit | Grandes contadores de objetos ou timestamps do sistema |
| **210 a 239** | Double | Vetores de direção de extrusão espacial (X/Y/Z) |
| **270 a 289** | Inteiro de 16-bit | Flags curtas de configuração e estados de exibição |
| **290 a 299** | Booleano | Flags de verdadeiro/falso (`0` ou `1`) |
| **300 a 309** | String | Textos arbitrários (comentários, dados de terceiros ou IDs textuais) |
| **310 a 319** | Binário (Hex) | Blocos de dados binários brutos (imagens, dados embutidos) |
| **320 a 329** | String Hex | Referências de ponteiros para handles de objetos proprietários |
| **330 a 339** | String Hex | ID de ponteiro (Soft pointer) para objetos contêineres/pais |
| **340 a 349** | String Hex | ID de ponteiro (Hard pointer) para objetos associados |
| **350 a 359** | String Hex | ID de referência direta (Soft owner) |
| **360 a 369** | String Hex | ID de referência direta (Hard owner) |
| **370 a 379** | Inteiro de 16-bit | Índices de espessura de linha (Lineweight) |
| **380 a 389** | Inteiro de 16-bit | Métodos e flags de plotagem |
| **430 a 439** | String | Nomes de cores provenientes de catálogos (ex: PANTONE) |
| **440 a 449** | Inteiro de 32-bit | Valores inteiros para transparência de cor |
| **450 a 459** | Inteiro de 32-bit | Atributos internos do sistema (renderização e hachuras) |
| **460 a 469** | Double | Valores de ponto flutuante de precisão dupla (escalas, opacidade) |
| **470 a 479** | String | Identificadores de propriedades e estados do sistema |
| **1000 a 1003** | String | Dados estendidos de aplicação (XDATA) |
| **1004** | Binário (Hex) | Dados binários em blocos estendidos (XDATA) |
| **1010 a 1059** | Double | Valores de ponto flutuante em dados estendidos (XDATA) |
| **1060 a 1071** | Inteiro | Valores inteiros de 16 e 32 bits em dados estendidos (XDATA) |

## ⚠️ Lacunas e Códigos Especiais (Exceções da Especificação)

O padrão DXF possui intervalos vagos ou reservados que não seguem a distribuição contínua.
Ao construir o parser, trate-os seguindo estas regras:
* **80 a 89**: Intervalo reservado/não documentado. Deve ser ignorado no mapeamento padrão de tipos.
* **101**: String (Texto complementar de sistema).
* **103 e 104**: Códigos contextuais de controle (variam conforme o objeto, comumente associados a dados de renderização e iluminação).
* **106 a 109**: Intervalo reservado/não documentado.
* **1005 (XDATA)**: String Hexadecimal representando um Handle associado a dados estendidos.
* **1006 a 1009 (XDATA)**: Códigos reservados/não utilizados em dados estendidos (o padrão pula diretamente para as coordenadas no grupo 1010).

*/