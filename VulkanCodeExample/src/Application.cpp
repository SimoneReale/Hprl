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



int main() {

    SetConsoleOutputCP(65001);

    
    std::u8string in_big = u8R"({"lines":[{"content":{"ops":[{"attributes":{"size":"60px","color":"#e60000"},"insert":"ARTIFICIAL BOLD"},{"attributes":{"size":"60px"},"insert":" "},{"attributes":{"size":"60px","color":"#9933ff"},"insert":"NORMAL"}]},"line_attributes":{}},{"content":{"ops":[{"attributes":{"size":"60px","color":"#e60000","bold":true},"insert":"ARTIFICIAL BOLD "},{"attributes":{"size":"60px","color":"#9933ff"},"insert":"YES"}]},"line_attributes":{}},{"content":{"ops":[{"attributes":{"size":"60px","color":"#e60000","italic":true},"insert":"ARTIFICIAL BOLD"},{"attributes":{"size":"60px","color":"#e60000"},"insert":" "},{"attributes":{"size":"60px","color":"#9933ff"},"insert":"NOT"}]},"line_attributes":{}}],"dimensions":{"width":"1728px","height":"750px"}})";

    std::vector<float> myHeights{};

    FontFaceDescription *f_normal = new FontFaceDescription("c:/windows/fonts/arial.ttf", 0, "NORMAL" );
    FontFaceDescription *f_bold = new FontFaceDescription("c:/windows/fonts/arialbd.ttf", 0, "BOLD");
    //FontFaceDescription *f_italic = new FontFaceDescription("c:/windows/fonts/ariali.ttf", 0, "ITALIC");
    FontFaceDescription* f_italic = new FontFaceDescription("c:/windows/fonts/arialbd.ttf", 0, "ITALIC");
    FontFaceDescription *f_bolditalic = new FontFaceDescription("c:/windows/fonts/arialbi.ttf", 0, "BOLDITALIC");

    std::vector<FontFaceDescription> descr{};

    BasicFontDescription bas_descr = BasicFontDescription(f_normal, f_bold, f_italic, f_bolditalic, descr);
    TextManager tm = TextManager(bas_descr, 32, 95, myHeights, 80);


    /*HprlTextFragment x = HprlTextFragment::createTextFragment("Prova").addBoldnessToFragment().addSizePxToFragment(30);
    
    x.addUnderlineToFragment().addStrikeToFragment();*/


    Texture text;
    std::size_t hash = tm.parse(in_big, text);
   
    auto t1 = Clock::now();

    ImageWriter::createPng("test_ARTIFICIAL.png", text);

    auto t2 = Clock::now();

    std::cout << "\n\nDelta time t2-t1: "
        << std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count()
        << " microseconds" << std::endl;\


    return 0;



}