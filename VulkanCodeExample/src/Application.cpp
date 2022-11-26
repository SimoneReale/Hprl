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

    const char8_t* in = u8R"({"lines":[{"content":{"ops":[{"insert":"L'elfo Cannarsa disse: lo prendo nell'ano nelle foreste"},{"attributes":{"color":"#e60000","bold":true},"insert":"Cannarsa"},{"insert":" disse: \"lo prendo nell'ano nelle "},{"attributes":{"italic":true},"insert":"foreste!"},{"insert":"\""}]},"line_attributes":{}}]})";
    json j3 = json::parse(in);

    string stringhetta = j3["lines"][0]["content"]["ops"][0]["insert"];

    std::vector<hprl_string> text{to_utf32(stringhetta), U"ti voglio male:", U"Non θ domenica", U"θωςΰ£", U"LYVWvwy"};

    std::vector<float> myHeights{50, 60, 70, 100};

    FontColor myColor1 = FontColor::colorConverter("#9933ff", 1);
    FontColor myColor2{ 1.0f, 1.0f, 1.0f, 1.0f };

    FontFaceDescription f_descr1{"c:/windows/fonts/arial.ttf", 0, 'a'};
    FontFaceDescription f_descr2{ "c:/windows/fonts/arialbd.ttf", 0, 'b' };
    
    std::vector<FontFaceDescription> descr;
    descr.push_back(f_descr1);
    descr.push_back(f_descr2);

    FontFamily font_fam = FontFamily(descr, 32, 95, myHeights);


    auto t1 = Clock::now();
    /*font_fam.createTextTextureSingleFontFace('a', text, 50, myColor1);
    font_fam.createTextTextureSingleFontFace('b', text, 60, myColor1);
    font_fam.createTextTextureSingleFontFace('b', text, 70, myColor1);*/
    font_fam.createTextTextureSingleFontFace('a', text, 100, myColor1);
    auto t2 = Clock::now();
   
    std::cout << "\n\nDelta time t2-t1: "
        << std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count()
        << " microseconds" << std::endl;


    ///*std::vector<float> myHeights{};

    //auto t1 = Clock::now();
    //FontFace myFont = FontFace("c:/windows/fonts/arial.ttf", 32, 95, myHeights, 0);
    //HprlBakedFont myBakedFont = myFont.bakeFont(60, 32, 95, 1);
    //auto t2 = Clock::now();*/


    //std::cout << "\n\nDelta time t2-t1: "
    //    << std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count()
    //    << " microseconds" << std::endl;

    

    


    return 0;



}