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

    u8string in = u8R"({"lines":[{"content":{"ops":[{"attributes":{"size":"25px","color":"#e60000"},"insert":"Lorem ipsum dolor sit amet, consectetur adipiscing elit. Pellentesque mollis dolor magna, quis posuere nunc commodo"}]},"line_attributes":{}},{"content":{"ops":[{"attributes":{"size":"25px","color":"#e60000"},"insert":"quis. Nunc non nibh et dolor fringilla efficitur ut sit amet ex. Integer tellus nunc, molestie quis ex a, varius fermentum nibh."}]},"line_attributes":{}},{"content":{"ops":[{"attributes":{"size":"25px","color":"#e60000"},"insert":"Proin eu leo mauris. Etiam finibus tellus placerat, ornare nisl sed, egestas ipsum. Praesent in auctor leo. Integer mattis "}]},"line_attributes":{}},{"content":{"ops":[{"attributes":{"size":"25px","color":"#e60000"},"insert":"lectus sit amet sapien placerat, sed pulvinar purus tempor. Integer malesuada magna id dignissim egestas. Ut purus sem, "}]},"line_attributes":{}},{"content":{"ops":[{"attributes":{"size":"25px","color":"#e60000"},"insert":"elementum sed quam quis, feugiat tempus turpis. Sed sit amet arcu vel mauris elementum sollicitudin. Suspendisse nec "}]},"line_attributes":{}},{"content":{"ops":[{"attributes":{"size":"25px","color":"#e60000"},"insert":"mauris sed tortor bibendum lobortis. Cras id malesuada sapien. Mauris nec tincidunt quam. Vivamus vestibulum faucibus"}]},"line_attributes":{}},{"content":{"ops":[{"attributes":{"size":"25px","color":"#e60000"},"insert":"bibendum. Phasellus varius egestas risus a iaculis. In nec mi et sem placerat vestibulum."}]},"line_attributes":{}}],"dimensions":{"width":"1383px","height":"578px"}})";
    
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