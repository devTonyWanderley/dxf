//  C:\Tony\DXF\main.cpp
#include <iostream>
#include "Dxf.hpp"

//--Lidar com dicionário de char[]--
std::vector<char> linguicao = {'C', 'A', 'S', 'A', 'B', 'E', 'C', 'O', 'A', 'N', 'T', 'A', 'Z', 'E', 'N', 'O', 'B', 'I', 'O', 'M', 'O', 'U', 'S',
    'E', 'C', 'O', 'L', 'H', 'E', 'R', 'F', 'I', 'T', 'A', 'C', 'A', 'S', 'A', 'Z', 'E', 'N', 'O', 'B', 'I', 'O', 'A', 'N', 'T', 'A', };
std::vector<size_t> iLingua = {0, 4, 8, 12, 19, 24, 30, 34, 38, 45, 49};

void imprime(std::vector<char> &texto, size_t posi, size_t posf)
{
    std::cout << posi << " - " << posf << "\t\'";
    for(auto i = posi; i < posf; i++) std::cout << texto.at(i);
    std::cout << "\'\n";
}

std::vector<DXF::Fresta> iLinguaPraIFrestas(std::vector<size_t> &li)
{
    std::vector<DXF::Fresta> r;
    if(!li.size()) return r;
    r.reserve(li.size() - 1);
    auto ita = li.begin();
    for(auto it = li.begin(); it != li.end(); it++)
    {
        if(it != li.begin())
        {
            DXF::Fresta f;
            f.posi = *ita;
            f.posf = *it;
            r.push_back(f);
        }
        ita = it;
    }
    return r;
}

int main()
{
    DXF::Dxf teste;
    teste.Valida();
    //std::cout << linguicao.size() << std::endl;
    //std::vector<size_t>::iterator iti{}, itf{};
    //for(itf = iLingua.begin(); itf != iLingua.end(); itf++)
    //{
    //    if(itf != iLingua.begin()) imprime(linguicao, *iti, *itf);
    //    iti = itf;
    //}
    //std::vector<DXF::Fresta> fILingua = iLinguaPraIFrestas(iLingua);
    //for(auto f : fILingua)std::cout << f.posi << ' ' << f.posf << std::endl;

    //DXF::viewChar a;
    //a.setFonte(linguicao);
    //a.setOculo(24, 30);
    //char texto[20];
    //a.getTexto(texto);
    //std::cout << texto << std::endl;
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