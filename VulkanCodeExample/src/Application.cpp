#define HPRL_IMAGE_WRITER_IMPLEMENTATION
#define HPRL_IMPLEMENTATION
#define HPRL_DEFAULT_FONT
#include "hprl.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
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
//SCRIVERE NELLA TESI DELLA PEG GRAMMAR
/////////////////////////////////////


int main() {

    SetConsoleOutputCP(65001);

    hprl_editor_string in_big = u8R"({"lines":[{"content":{"ops":[{"insert":"This is a test."}]},"line_attributes":{}},{"content":{"ops":[{"insert":"The font size is 18 px:"}]},"line_attributes":{}},{"content":{"ops":[{"attributes":{"size":"18px","color":"#e60000"},"insert":"Lorem ipsum dolor sit amet, consectetur adipiscing elit. "}]},"line_attributes":{}},{"content":{"ops":[{"attributes":{"size":"18px","color":"#e60000"},"insert":"Praesent "},{"attributes":{"size":"18px","color":"#e60000","bold":true},"insert":"accumsan"},{"attributes":{"size":"18px","color":"#e60000"},"insert":" sit amet ipsum nec pulvinar. "}]},"line_attributes":{}},{"content":{"ops":[{"attributes":{"size":"18px","color":"#e60000"},"insert":"Nunc malesuada, "},{"attributes":{"size":"18px","color":"#e60000","italic":true},"insert":"mi ut auctor faucibus"},{"attributes":{"size":"18px","color":"#e60000"},"insert":", neque dolor maximus est."}]},"line_attributes":{}},{"content":{"ops":[{"attributes":{"size":"18px","color":"#e60000"},"insert":"Nulla facilisi. "},{"attributes":{"size":"18px","color":"#0066cc","underline":true},"insert":"Donec"},{"attributes":{"size":"18px","color":"#e60000"},"insert":"."}]},"line_attributes":{}}],"dimensions":{"width":"800px","height":"500px"}})";

    FontFamilyManager fff = FontFamilyManager();
    int id_default;
    FontFamily tm_default = fff.addDefaultFontFamily(id_default, hprl_hgts_px({ 40 }), 80);
    
    FontFaceDescription *f_normal = new FontFaceDescription("c:/sr/against.ttf", 0, "NORMAL" );
    FontFaceDescription *f_bold = new FontFaceDescription("c:/windows/fonts/arialbd.ttf", 0, "BOLD");
    FontFaceDescription *f_italic = new FontFaceDescription("c:/windows/fonts/ariali.ttf", 0, "ITALIC");
    FontFaceDescription *f_bolditalic = new FontFaceDescription("c:/windows/fonts/arialbi.ttf", 0, "BOLDITALIC");
    std::vector<FontFaceDescription> descr{};
    FontFamilyInitializer bas_descr = FontFamilyInitializer(f_normal, f_bold, f_italic, f_bolditalic, descr);

    int id_complex;
    FontFamily tm_complex = fff.addComplexFontFamily(id_complex, bas_descr, 32, 95, hprl_hgts_px({ 40 }), 80);


    FontFaceDescription* f_normal2 = new FontFaceDescription("c:/windows/fonts/arial.ttf", 0, "NORMAL");
    FontFaceDescription* f_bold2 = new FontFaceDescription("c:/windows/fonts/arialbd.ttf", 0, "BOLD");
    FontFaceDescription* f_italic2 = new FontFaceDescription("c:/windows/fonts/ariali.ttf", 0, "ITALIC");
    FontFaceDescription* f_bolditalic2 = new FontFaceDescription("c:/windows/fonts/arialbi.ttf", 0, "BOLDITALIC");
    std::vector<FontFaceDescription> descr2{};
    FontFamilyInitializer bas_descr2 = FontFamilyInitializer(f_normal2, f_bold2, f_italic2, f_bolditalic2, descr2);


    int id_complex2;
    FontFamily tm_complex2 = fff.addComplexFontFamily(id_complex2, bas_descr2, 32, 95, hprl_hgts_px({ 40 }), 80);


    Texture text_parse;
    tm_complex2.parseFromEditor(in_big, text_parse, 0, 0, 0);




    FixedTextCreator x{ 1280, 720, 100, 150, 200};
    x
        << 30 << Color{ "#e60000" }
        << "There is a 200 px upper margin" << el
        << "There is a 100 px left margin" << el
        << "There is a 150 px right margin" << el
        << "This is a centered red ordered list" << align_c << lst_ord << el
        << Color{ "#000000" } << "This is a bold black ordered list aligned on the left" << bold << lst_ord << el
        << Color{ "#eeee00" } << "This is a yellow ordered list aligned on the right" << align_r << lst_ord << el
        << Color{ "#ee00ee" } << "This is a bold italic purple ordered list aligned on the left" << bold << italic << el;
        
        

    Texture text_created;
    tm_complex2.getCreatorTexture(x, text_created);



    /*LiveTextEditor new_te = LiveTextEditor(fff,id_complex2, 1280, 720, 30, 0, 0, 0);
    Texture tex_ed;

    new_te.ManageColor("#eeeeee");
    new_te.ManageBold(true);
    new_te.appendFragment("Lorem ipsum dolor sit amet,", tex_ed);
    new_te.addNewLine();
    new_te.appendFragment("consectetur adipiscing elit.", tex_ed);
    new_te.addNewLine();
    new_te.appendFragment("Nam feugiat ut nisi vel molestie.", tex_ed);
    new_te.addNewLine();
    new_te.appendFragment("Class aptent taciti sociosqu ad,", tex_ed);
    new_te.addNewLine();
    new_te.appendFragment("litora torquent per conubia nostra", tex_ed);
    new_te.addNewLine();
    new_te.appendFragment("per inceptos himenaeos.", tex_ed);
    new_te.addNewLine();
    new_te.appendFragment("Curabitur scelerisque rutrum blandit.", tex_ed);
    new_te.addNewLine();
    new_te.appendFragment("Nunc at ipsum orci.", tex_ed);
    new_te.addNewLine();
    new_te.appendFragment("Quisque egestas lorem eget molestie varius.", tex_ed);
    new_te.addNewLine();
    new_te.appendFragment("Vestibulum eu mauris nunc.", tex_ed);
    new_te.addNewLine();
    new_te.appendFragment("Imperdiet libero in.", tex_ed);*/



    //new_te.ManageColor("#e00000");
    //int fd1 = new_te.appendFragment("Sapone1", tex_ed);
    //int fd2 = new_te.appendFragment("Sapone2", tex_ed);
    //new_te.appendFragment("Sapone3", tex_ed);
    //new_te.ManageColor("#00ee00");
    //new_te.appendFragment("Sapone4", tex_ed);
    //new_te.changeSizePx(160);
    //int fd4 = new_te.appendFragment("Sapone5", tex_ed);
    //new_te.changeSizePx(20);
    //new_te.appendFragment("Sapone6", tex_ed);
    //new_te.changeSizePx(50);
    //int fd3 = new_te.appendFragment("Sapone7", tex_ed);
    //int fd5 = new_te.appendFragment("Sapone8", tex_ed);
    //new_te.appendFragment("Sapone9", tex_ed);
    //new_te.ManageColor("#e00000");
    //new_te.changeFontFamily(id_default);
    //new_te.changeSizePx(100);
    //new_te.appendFragment("Sapone10", tex_ed);
    ////new_te.deleteFragmentLayoutAdjustment(fd3);
    //new_te.ManageColor("#00eeee");
    ////new_te.substituteFragment(fd1, "Sapone20", tex_ed);
    //new_te.ManageColor("#00ee00");
    //new_te.substituteFragment(fd2, "Sapone21", tex_ed);
    //new_te.changeFontFamily(id_complex);
    //new_te.changeSizePx(60);
    //new_te.appendFragment("Sapon11", tex_ed);
    //new_te.changeSizePx(130);
    //int fd6 = new_te.appendFragment("Sapone12", tex_ed);
    //new_te.deleteFragment(fd6, tex_ed);
    

    

    /*new_te.internalDelete(fd1);
    new_te.internalDelete(fd2);
    new_te.internalDelete(fd3);*/

    
    //new_te.printStructure();



    //Texture tex_new = Texture(data, width, height);

    

    //ImageWriter::createPng("test_from_editor_online.png", text_parse);
    ImageWriter::createPng("test_from_creator_default.png", text_created);
    //ImageWriter::createPng("lorem_ipsum.png", text_created);
    
    //ImageWriter::createPng("test_new_lorem_normal.png", new_te.getSingleChan(), 1280, 720);
    //ImageWriter::createPng("test_new_lorem_artificial_bold.png", grayscale_dilation(new_te.getSingleChan(), 1280, 720, 3), 1280, 720);
    //ImageWriter::createJpg("test.jpg", tex_ed, 100);


    /*Texture text_parse;
    tm.parseFromEditor(in_big, text_parse);*/





    //std::cout << "\nDelta time t2-t1: "
    //    << std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count()
    //    << " microseconds" << std::endl;\


    return 0;



}