#define HPRL_IMAGE_WRITER_IMPLEMENTATION
#define HPRL_IMPLEMENTATION
#define HPRL_DEFAULT_FONT
#include "hprl.h"
#include <chrono>
#include <cstdio>
#include <windows.h>

#pragma execution_character_set( "utf-8" )

typedef std::chrono::high_resolution_clock Clock;


//DA FARE: SERIALIZATION (?)
//CACHE DELLE TEXTURE
//VERSIONE INCREMENTALE DELLA CREATE TEXTURE
//ASPECT RATIO NELLA CONVERSIONE A RGBA32BIT
//Implementare font di default (base64)
// Casi d'uso:
// fps che varia e quanto è impattante sugli fps
// chat: qualità del testo con ulteriori aggiunte 
// testo variabile su una scena, display a scorrimento su di una porta
//debug information (unity): fps posizione della camera timestamp
// 


/////////////////////////////////////
//CREARE TEMPLATE PER GLI ATTRIBUTI
//UN VETTORE DI ATTRIBUTI BANALMENTE
/////////////////////////////////////


int main() {

    SetConsoleOutputCP(65001);

    std::u8string in_big = u8R"({"lines":[{"content":{"ops":[{"attributes":{"size":"40px","bold":true},"insert":"Giovanni"},{"attributes":{"size":"40px"},"insert":" è "},{"attributes":{"underline":true,"size":"40px","color":"#e60000","italic":true},"insert":"stronzo"}]},"line_attributes":{}},{"content":{"ops":[{"attributes":{"size":"40px","color":"#9933ff","strike":true},"insert":"Giulia"},{"attributes":{"size":"40px"},"insert":" "},{"attributes":{"italic":true,"size":"40px","bold":true},"insert":"anche"}]},"line_attributes":{"align":"center"}}],"dimensions":{"width":"400px","height":"400px"}})";
    std::vector<float> myHeights{40};
    FontFamilyManager tm = FontFamilyManager(myHeights, 80);

    TextCreator x{ 1000, 1000 };

    x
      << 30 << Color{"#e60000"}
      << "Questo è italic" << italic << lst_ord << el 
      << "Questo è bold underlined" << bold << underline << 40 << Color{ "#e600e6" } << lst_bul << el
      << "Questa è una lista" << align_c << Color{ "#e600e6" } << lst_ord;

    Texture text;
    std::size_t hash = tm.parseFromEditor(in_big, text);
  
    auto t1 = Clock::now();

    ImageWriter::createPng("test_DEFAULT_FONT.png", text);

    auto t2 = Clock::now();


    /*FontFaceDescription *f_normal = new FontFaceDescription("c:/windows/fonts/arial.ttf", 0, "NORMAL" );
    FontFaceDescription *f_bold = new FontFaceDescription("c:/windows/fonts/arialbd.ttf", 0, "BOLD");
    FontFaceDescription* f_italic = new FontFaceDescription("c:/windows/fonts/ariali.ttf", 0, "ITALIC");
    FontFaceDescription *f_bolditalic = new FontFaceDescription("c:/windows/fonts/arialbi.ttf", 0, "BOLDITALIC");

    std::vector<FontFaceDescription> descr{};

    FontFamilyInitializer bas_descr = FontFamilyInitializer(f_normal, f_bold, f_italic, f_bolditalic, descr);
    FontFamilyManager tm = FontFamilyManager(bas_descr, 32, 95, myHeights, 80);*/


    std::cout << "\n\nDelta time t2-t1: "
        << std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count()
        << " microseconds" << std::endl;\


    return 0;



}