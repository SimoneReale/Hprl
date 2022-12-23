#define HPRL_IMAGE_WRITER_IMPLEMENTATION
#define HPRL_IMPLEMENTATION
#include "hprl.h"
#include <chrono>
#include <cstdio>
#include <windows.h>
#include "default_font.h"

#pragma execution_character_set( "utf-8" )

//#define STB_IMAGE_WRITE_IMPLEMENTATION
//#include "stb_image_write.h"
typedef std::chrono::high_resolution_clock Clock;



using json = nlohmann::json;

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

    
    std::u8string in_big = u8R"({"lines":[{"content":{"ops":[{"attributes":{"size":"60px","color":"#e60000"},"insert":"ARTIFICIAL FUNZIONA"},{"attributes":{"size":"60px"},"insert":" "},{"attributes":{"size":"60px","color":"#9933ff"},"insert":"NORMAL"}]},"line_attributes":{}},{"content":{"ops":[{"attributes":{"size":"60px","color":"#e60000","bold":true},"insert":"ARTIFICIAL BOLD "},{"attributes":{"size":"60px","color":"#9933ff"},"insert":"YES"}]},"line_attributes":{}},{"content":{"ops":[{"attributes":{"size":"60px","color":"#e60000","italic":true},"insert":"ARTIFICIAL BOLD"},{"attributes":{"size":"60px","color":"#e60000"},"insert":" "},{"attributes":{"size":"60px","color":"#9933ff"},"insert":"NOT"}]},"line_attributes":{}}],"dimensions":{"width":"1728px","height":"750px"}})";

    std::vector<float> myHeights{20};

    FontFaceDescription *f_normal = new FontFaceDescription("c:/windows/fonts/arial.ttf", 0, "NORMAL" );
    FontFaceDescription *f_bold = new FontFaceDescription("c:/windows/fonts/arialbd.ttf", 0, "BOLD");
    FontFaceDescription* f_italic = new FontFaceDescription("c:/windows/fonts/ariali.ttf", 0, "ITALIC");
    FontFaceDescription *f_bolditalic = new FontFaceDescription("c:/windows/fonts/arialbi.ttf", 0, "BOLDITALIC");

    std::vector<FontFaceDescription> descr{};

    FontFamilyInitializer bas_descr = FontFamilyInitializer(f_normal, f_bold, f_italic, f_bolditalic, descr);
    FontFamilyManager tm = FontFamilyManager(bas_descr, 32, 95, myHeights, 80);

    

    /*Creator x{ 1000, 1000 };

    x
      << 30 << Color{"#e60000"}
      << "Questo è italic" << italic << lst_ord << el 
      << "Questo è bold underlined" << bold << underline << 40 << Color{ "#e600e6" } << lst_bul << el
      << "Questa è una lista" << align_c << Color{ "#e600e6" } << lst_ord;*/


    Texture text;
    std::size_t hash = tm.parseFromEditor(in_big, text);
  
    auto t1 = Clock::now();

    ImageWriter::createPng("test_con_modifiche.png", text);

    auto t2 = Clock::now();

    std::cout << "\n\nDelta time t2-t1: "
        << std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count()
        << " microseconds" << std::endl;\


    return 0;



}