#include "hprl.h"
#include <iostream>
#include <stdio.h>
#include<vector>
#include <string>
#include <chrono>
#include <peglib.h>
#include<memory>

//#define STB_IMAGE_WRITE_IMPLEMENTATION
//#include "stb_image_write.h"
typedef std::chrono::high_resolution_clock Clock;


using namespace hprl::FontManager;
using namespace peg;

//DA FARE: SERIALIZATION (?)
//CACHE DELLE TEXTURE
//VERSIONE INCREMENTALE DELLA CREATE TEXTURE
//ASPECT RATIO NELLA CONVERSIONE A RGBA32BIT

int main() {

    ////hprl stringona = U"Lorem ipsum dolor sit amet, consectetur adipiscing elit. Nunc vel enim arcu. Nunc ornare, turpis nec molestie facilisis, nisl libero scelerisque lacus, viverra porttitor enim ante eu diam. Integer in magna nibh. Pellentesque vehicula, dolor a vehicula sagittis, est libero pretium purus, sed elementum tellus dui a odio. Fusce ac ullamcorper enim. Nullam eu justo vel dolor viverra viverra. Curabitur odio justo, gravida non est et, dignissim tincidunt orci.Sed sagittis nibh nec felis vestibulum, quis malesuada diam sagittis.Mauris fringilla ipsum ac libero aliquet euismod.Integer tincidunt urna vestibulum suscipit posuere.Maecenas fringilla ultricies dignissim.Donec vestibulum lacus eu ligula molestie, in pharetra arcu accumsan.Duis elementum mauris non libero dapibus, sit amet porta enim tempor.Aenean eu quam quis nisl egestas lacinia.Nulla erat nisi, viverra et leo nec, porta tincidunt odio.Sed porttitor libero vitae bibendum vehicula.Pellentesque feugiat orci sed augue lectus.";
    std::vector<hprl_string> text{U"Questo è un test breve", U"su più righe d'interesse:", U"TjFyyyY", U"èùòà£", U"LYVWvwy"};

    std::vector<float> myHeights{50, 60, 70, 100};

    

    FontColor myColor1{ 1.0f, 1.0f, 0.0f, 1.0f };
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
    font_fam.createTextTextureSingleFontFace('b', text, 100, myColor1);
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



    /*parser parser(R"(

    Root <- Span_element*
    Span_element <- Space* '<span'i Span_attribute* '>' Space* (Element / Text)*  Space* '</span'i'>' Space*

    Span_attribute <- Space* (Color_attribute / Size_attribute / Align_attribute)  Space*
    Color_attribute <- 'color'i Space*  '=' Space* <[0-9]{0,3}  Space* ',' Space* [0-9]{0,3}  Space* ','  Space* [0-9]{0,3}>
    Size_attribute <- 'size'i Space* '=' Space* <[0-9]{0,4}>
    Align_attribute <- 'align'i Space* '=' Space* <Align_mode>
    Align_mode <- 'center'i / 'left'i / 'right'i


    Element  <-  Bold_element / Italic_element / Simple_element / List
    Bold_element <- Space* '<b'i'>' Space* (Element / Text)* Space* '</b'i'>' Space*
    Italic_element <- Space* '<i'i'>' Space* (Element / Text)* Space* '</i'i'>' Space*
    Simple_element <- '<br>'i


    List <- Space* '<list'i List_type_attribute '>' Space* (List_element)*  Space* '</list'i'>' Space*
    List_element <- Space* '<li>'i Space* (Element / Text)* Space* '</li>' Space*
    List_type_attribute <- Space * 'type'i Space* '=' Space* <List_type> Space*
    List_type <- 'ordered'i / 'unordered'i


    Text <- [^'</']+
    ~Space <- [\r\n \t\u000C]
  )");



    parser.enable_ast();

    std::shared_ptr<peg::Ast> ast;
    if (parser.parse(
        "<span color = 100, 100, 0 align = center size = 10><list type = ordered >"
        "<li>Elem 1 </li>"
        "<li>Elem 2 </li>"
        "<li>"
        "<list type = unordered>"
        "<li><i> Elem nested 1.1 </i> </li>"
        "</list>"
        "</li>"
        "</list>"
        "</span>"
        "<span align = left>Il mio titolo </span>", ast)) {
        std::cout << peg::ast_to_s(ast);
    }
    else {

        std::cout << "Errore" << std::endl;

    }*/



    return 0;



}