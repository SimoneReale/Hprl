#define IMAGE_WRITER_IMPLEMENTATION
#include "hprl.h"
#include <iostream>
#include <stdio.h>
#include<vector>
#include <string>
#include <chrono>
#include<memory>
#include <codecvt>
#include <cstdio>
#include <windows.h>

#pragma execution_character_set( "utf-8" )

//#define STB_IMAGE_WRITE_IMPLEMENTATION
//#include "stb_image_write.h"
typedef std::chrono::high_resolution_clock Clock;



using json = nlohmann::json;

//DA FARE: SERIALIZATION (?)
//CACHE DELLE TEXTURE
//VERSIONE INCREMENTALE DELLA CREATE TEXTURE
//ASPECT RATIO NELLA CONVERSIONE A RGBA32BIT
//


//DA VEDERE LA CACHE CHE NON FUNZIONA

int main() {

    SetConsoleOutputCP(65001);

    
    std::u8string in_big = u8R"({"lines":[{"content":{"ops":[{"attributes":{"color":"#e60000","size":"30px"},"insert":"Questo è un "},{"attributes":{"color":"#e60000","size":"30px","italic":true},"insert":"gran"},{"attributes":{"color":"#e60000","size":"30px"},"insert":" "},{"attributes":{"color":"#e60000","size":"30px","bold":true},"insert":"test"}]},"line_attributes":{}},{"content":{"ops":[{"attributes":{"color":"#e60000","size":"30px"},"insert":"proviamo "},{"attributes":{"color":"#e60000","size":"30px","underline":true},"insert":"tutto"}]},"line_attributes":{"align":"center"}},{"content":{"ops":[{"attributes":{"color":"#e60000","size":"30px","underline":true},"insert":"ma "},{"attributes":{"underline":true,"color":"#e60000","size":"30px","strike":true},"insert":"davvero"},{"attributes":{"color":"#e60000","size":"30px","underline":true},"insert":" "},{"attributes":{"underline":true,"color":"#9933ff","size":"30px","italic":true},"insert":"tutto"}]},"line_attributes":{"align":"right"}},{"content":{"ops":[{"attributes":{"underline":true,"color":"#9933ff","size":"30px","italic":true},"insert":"Funzionalità interessanti"}]},"line_attributes":{"align":"center","list":"ordered"}},{"content":{"ops":[{"attributes":{"underline":true,"color":"#9933ff","size":"30px","italic":true},"insert":"Meno interessanti"}]},"line_attributes":{"align":"center","list":"ordered"}},{"content":{"ops":[{"attributes":{"underline":true,"color":"#e60000","size":"30px","italic":true},"insert":"Ottime"}]},"line_attributes":{"align":"center","list":"bullet"}},{"content":{"ops":[{"attributes":{"underline":true,"color":"#e60000","size":"30px","italic":true},"insert":"Meno ottime"}]},"line_attributes":{"align":"center","list":"bullet"}}],"dimensions":{"width":"1728px","height":"750px"}})";

    std::vector<float> myHeights{};

    FontFaceDescription *f_normal = new FontFaceDescription("c:/windows/fonts/arial.ttf", 0, "NORMAL" );
    FontFaceDescription *f_bold = new FontFaceDescription("c:/windows/fonts/arialbd.ttf", 0, "BOLD");
    FontFaceDescription *f_italic = new FontFaceDescription("c:/windows/fonts/ariali.ttf", 0, "ITALIC");
    FontFaceDescription *f_bolditalic = new FontFaceDescription("c:/windows/fonts/arialbi.ttf", 0, "BOLDITALIC");

    std::vector<FontFaceDescription> descr{};

    BasicFontDescription bas_descr = BasicFontDescription(f_normal, f_bold, f_italic, f_bolditalic, descr);
    TextManager font_fam = TextManager(bas_descr, 32, 95, myHeights, 80);

    Texture text;
    std::size_t hash = font_fam.parse(in_big, text);
   
    auto t1 = Clock::now();

    ImageWriter::createPng("test_con_lru_cache.png", text);

    auto t2 = Clock::now();

    std::cout << "\n\nDelta time t2-t1: "
        << std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count()
        << " microseconds" << std::endl;\


    return 0;



}