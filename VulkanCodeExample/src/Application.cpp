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


using namespace hprl::FontManager;
using json = nlohmann::json;
using namespace std;

//DA FARE: SERIALIZATION (?)
//CACHE DELLE TEXTURE
//VERSIONE INCREMENTALE DELLA CREATE TEXTURE
//ASPECT RATIO NELLA CONVERSIONE A RGBA32BIT

int main() {

    SetConsoleOutputCP(65001);

    u8string in = u8R"({"lines":[{"content":{"ops":[{"attributes":{"underline":true,"strike":true,"italic":true,"size":"92px","color":"#e60000","bold":true},"insert":"Str"},{"attributes":{"underline":true,"strike":true,"italic":true,"size":"92px","color":"#9933ff","bold":true},"insert":"ike    "}]},"line_attributes":{}}],"dimensions":{"width":"1728px","height":"750px"}})";
    
    vector<float> myHeights{50, 60, 70, 100};

   
    FontFaceDescription f_normal{"c:/windows/fonts/arial.ttf", 0, "NORMAL"};
    FontFaceDescription f_bold{ "c:/windows/fonts/arialbd.ttf", 0, "BOLD"};
    FontFaceDescription f_italic{ "c:/windows/fonts/ariali.ttf", 0, "ITALIC" };
    FontFaceDescription f_bolditalic{ "c:/windows/fonts/arialbi.ttf", 0, "BOLDITALIC" };
    
    vector<FontFaceDescription> descr{};
 
    BasicFontDescription bas_descr{ f_normal, f_bold, f_italic, f_bolditalic, descr };
    TextManager font_fam = TextManager(bas_descr, 32, 95, myHeights);

    


    auto t1 = Clock::now();
    ///*font_fam.createTextTextureSingleFontFace('a', text, 50, myColor1);
    //font_fam.createTextTextureSingleFontFace('b', text, 60, myColor1);
    //font_fam.createTextTextureSingleFontFace('b', text, 70, myColor1);*/
    //font_fam.createTextTextureSingleFontFace("bold", text, 100, myColor1);
    font_fam.parse(in);
    font_fam.printToConsole(in);
    
    auto t2 = Clock::now();
   
    std::cout << "\n\nDelta time t2-t1: "
        << std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count()
        << " microseconds" << std::endl;

    return 0;



}