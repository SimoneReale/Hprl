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
//


//DA VEDERE LA CACHE CHE NON FUNZIONA

int main() {

    SetConsoleOutputCP(65001);

    u8string in = u8R"({"lines":[{"content":{"ops":[{"attributes":{"size":"30px","background":"#e60000"},"insert":"Sapone"}]},"line_attributes":{}}],"dimensions":{"width":"1383px","height":"578px"}})";
    
    vector<float> myHeights{25};

    FontFaceDescription f_normal{"c:/windows/fonts/arial.ttf", 0, "NORMAL"};
    FontFaceDescription f_bold{ "c:/windows/fonts/arialbd.ttf", 0, "BOLD"};
    FontFaceDescription f_italic{ "c:/windows/fonts/ariali.ttf", 0, "ITALIC" };
    FontFaceDescription f_bolditalic{ "c:/windows/fonts/arialbi.ttf", 0, "BOLDITALIC" };
    
    vector<FontFaceDescription> descr{};
 
    BasicFontDescription bas_descr{ f_normal, f_bold, f_italic, f_bolditalic, descr };
    TextManager font_fam = TextManager(bas_descr, 32, 95, myHeights);

    
    auto t1 = Clock::now();
    font_fam.parse(in);
    //font_fam.printToConsole(in);
    auto t2 = Clock::now();
   
    std::cout << "\n\nDelta time t2-t1: "
        << std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count()
        << " microseconds" << std::endl;



    return 0;



}