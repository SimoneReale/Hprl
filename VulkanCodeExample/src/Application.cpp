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

    u8string in = u8R"({"lines":[{"content":{"ops":[{"insert":"La riga a "},{"attributes":{"bold":true},"insert":"si"},{"attributes":{"size":"25px","bold":true},"insert":"nistra"}]},"line_attributes":{}},{"content":{"ops":[{"attributes":{"size":"28px"},"insert":"Ri"},{"attributes":{"size":"10px"},"insert":"ga"},{"attributes":{"size":"28px"},"insert":" al "},{"attributes":{"color":"#e60000","size":"31px"},"insert":"cen"},{"attributes":{"color":"#e60000","size":"28px"},"insert":"tro"}]},"line_attributes":{"align":"center"}},{"content":{"ops":[{"insert":"Riga a "},{"attributes":{"underline":true},"insert":"destra"}]},"line_attributes":{"align":"right"}},{"content":{"ops":[{"insert":"Lista "},{"attributes":{"strike":true},"insert":"puntata"}]},"line_attributes":{"list":"ordered"}},{"content":{"ops":[{"insert":"Lista "},{"attributes":{"italic":true},"insert":"bullet"}]},"line_attributes":{"align":"center","list":"bullet"}}],"dimensions":{"width":"1728px","height":"750px"}})";
    json j3 = json::parse(in);

    string stringhetta = j3["lines"][0]["content"]["ops"][0]["insert"];

    vector<hprl_string> text{to_utf32(stringhetta), U"ti voglio male:", U"Non � domenica", U"����", U"LYVWvwy"};

    vector<float> myHeights{50, 60, 70, 100};

    FontColor myColor1 = FontColor::colorConverter("#9933ff", 1);
    FontColor myColor2{ 1.0f, 1.0f, 1.0f, 1.0f };

    FontFaceDescription f_descr1{"c:/windows/fonts/arial.ttf", 0, "NORMAL"};
    FontFaceDescription f_descr2{ "c:/windows/fonts/arialbd.ttf", 0, "BOLD"};
    
    vector<FontFaceDescription> descr;
    descr.push_back(f_descr1);
    descr.push_back(f_descr2);
    TextManager font_fam = TextManager(descr, 32, 95, myHeights);


    font_fam.parse(in);

    font_fam.printToConsole(in);


    //auto t1 = Clock::now();
    ///*font_fam.createTextTextureSingleFontFace('a', text, 50, myColor1);
    //font_fam.createTextTextureSingleFontFace('b', text, 60, myColor1);
    //font_fam.createTextTextureSingleFontFace('b', text, 70, myColor1);*/
    //font_fam.createTextTextureSingleFontFace("BOLD", text, 100, myColor1);
    //auto t2 = Clock::now();
   
    //std::cout << "\n\nDelta time t2-t1: "
    //    << std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count()
    //    << " microseconds" << std::endl;



   /* std::size_t h1 = std::hash<std::string>{}(R"({"lines": [{"content":{"ops": [{"insert":"NONNa � Pnuozz�"}, {"attributes":{"color":"#e60000","bold" : true},"insert" : "Cannarsa"}, {"insert":" disse: \"lo prendo nell'ano nelle "
}, { "attributes":{"italic":true},"insert" : "foreste!" }, { "insert":"\"" }]}, "line_attributes":{}}]}"))");

    cout << h1 << endl;*/



    return 0;



}