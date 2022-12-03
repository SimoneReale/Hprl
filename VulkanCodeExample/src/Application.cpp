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

    u8string in = u8R"({"lines":[{"content":{"ops":[{"attributes":{"size":"100px"},"insert":"Saponazzo"}]},"line_attributes":{}},{"content":{"ops":[{"attributes":{"color":"#e60000","size":"100px"},"insert":"Saponazzo"}]},"line_attributes":{}},{"content":{"ops":[{"attributes":{"color":"#008a00","size":"35px"},"insert":"Sapo"},{"attributes":{"size":"35px"},"insert":"ne"},{"attributes":{"color":"#008a00","size":"35px"},"insert":"tta"}]},"line_attributes":{"align":"right"}}],"dimensions":{"width":"1728px","height":"750px"}})";
    json j3 = json::parse(in);

    string stringhetta = j3["lines"][0]["content"]["ops"][0]["insert"];

    vector<hprl_string> text{to_utf32(stringhetta), U"ti voglio male:", U"Non è domenica", U"èùòà£", U"LYVWvwy"};

    vector<float> myHeights{50, 60, 70, 100};

    FontColor myColor1 = FontColor::colorConverter("#9933ff", 1);
    FontColor myColor2{ 1.0f, 1.0f, 1.0f, 1.0f };

    FontFaceDescription f_descr1{"c:/windows/fonts/arial.ttf", 0, "NORMAL"};
    FontFaceDescription f_descr2{ "c:/windows/fonts/arialbd.ttf", 0, "BOLD"};
    
    vector<FontFaceDescription> descr;
    descr.push_back(f_descr1);
    descr.push_back(f_descr2);

   
    BasicFontDescription bas_descr{ f_descr1, f_descr2, f_descr1, f_descr2, descr };
    TextManager font_fam = TextManager(bas_descr, 32, 95, myHeights);


    font_fam.parse(in);

    font_fam.printToConsole(in);


    //auto t1 = Clock::now();
    ///*font_fam.createTextTextureSingleFontFace('a', text, 50, myColor1);
    //font_fam.createTextTextureSingleFontFace('b', text, 60, myColor1);
    //font_fam.createTextTextureSingleFontFace('b', text, 70, myColor1);*/
    //font_fam.createTextTextureSingleFontFace("bold", text, 100, myColor1);
    //auto t2 = Clock::now();
   
    //std::cout << "\n\nDelta time t2-t1: "
    //    << std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count()
    //    << " microseconds" << std::endl;



   /* std::size_t h1 = std::hash<std::string>{}(R"({"lines": [{"content":{"ops": [{"insert":"NONNa è Pnuozzò"}, {"attributes":{"color":"#e60000","bold" : true},"insert" : "Cannarsa"}, {"insert":" disse: \"lo prendo nell'ano nelle "
}, { "attributes":{"italic":true},"insert" : "foreste!" }, { "insert":"\"" }]}, "line_attributes":{}}]}"))");

    cout << h1 << endl;*/



    return 0;



}