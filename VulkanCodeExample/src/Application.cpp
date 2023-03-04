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

    hprl_editor_string in_big = u8R"({"lines":[{"content":{"ops":[{"attributes":{"size":"40px","bold":true},"insert":"Giovanni"},{"attributes":{"size":"40px"},"insert":" è "},{"attributes":{"underline":true,"size":"40px","color":"#e60000","italic":true},"insert":"simpatico"}]},"line_attributes":{}},{"content":{"ops":[{"attributes":{"size":"40px","color":"#9933ff","strike":true},"insert":"Giulia"},{"attributes":{"size":"40px"},"insert":" "},{"attributes":{"italic":true,"size":"40px","bold":true},"insert":"anche"}]},"line_attributes":{"align":"center"}}],"dimensions":{"width":"400px","height":"400px"}})";

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


    FixedTextCreator x{ 1280, 1490, 200, 100, 0};
    x
        << 40 << Color{"#ee00ee"} << "Saponetto" << bold << underline << " bitalic " << italic << Color{"#00ee00"} << "c" << strike << "drusilla" << underline << strike << el;



    //TextEditor te = TextEditor(fff,id_complex2, 1280, 1490, 80, 200, 0, 300);
    //Texture tex_ed;

    //te.ManageColor("#000000");
    //FragmentDescriptor fd5 = te.appendFragment("La prima riga normale", tex_ed);
    //te.addNewLine();
    //te.ManageColor("#ee0000");
    //te.appendFragment("Il razzo va a:", tex_ed);
    //te.addNewLine();
    //FragmentDescriptor fd = te.appendFragment("100 ", tex_ed);
    //te.appendFragment("km/h", tex_ed);
    //FragmentDescriptor fd3 = te.appendFragment(" fast", tex_ed);
    //te.appendFragment("pupula", tex_ed);
    //auto t1 = Clock::now();
    //te.ManageColor("#00ee00");
    //auto t2 = Clock::now();
    //te.changeFontFamily(id_complex);
    //te.addNewLine();
    //te.appendFragment("The altitude is: ", tex_ed);
    //te.ManageColor("#ee0000");
    //te.appendFragment("èàù", tex_ed);
    //te.ManageColor("#00ee00");
    //te.addNewLine();
    //te.appendFragment("Oxygen level: ", tex_ed);
    //te.ManageColor("#ee0000");
    //FragmentDescriptor fd2 = te.appendFragment("critic", tex_ed);
    ////FragmentDescriptor fd4 = te.printFragmentAt(788, 684, "Diletta leotta", tex_ed);
    //te.substituteFragment(fd5, "Olio calido", tex_ed);
    //te.substituteFragment(fd3, "Seppiolina seppiolona ", tex_ed);


    NewTextEditor new_te = NewTextEditor(fff,id_complex2, 1280, 1490, 50, 200, 200, 500);
    Texture tex_ed;

    new_te.ManageColor("#e00000");
    int fd1 = new_te.appendFragment("Sapone1", tex_ed);
    int fd2 = new_te.appendFragment("Sapone2", tex_ed);
    new_te.appendFragment("Sapone3", tex_ed);
    new_te.ManageColor("#00ee00");
    new_te.appendFragment("Sapone4", tex_ed);
    new_te.changeSizePx(160);
    int fd4 = new_te.appendFragment("Sapone5", tex_ed);
    new_te.changeSizePx(20);
    new_te.appendFragment("Sapone6", tex_ed);
    new_te.changeSizePx(80);
    int fd3 = new_te.appendFragment("Sapone7", tex_ed);
    int fd5 = new_te.appendFragment("Sapone8", tex_ed);
    new_te.appendFragment("Sapone9", tex_ed);
    new_te.ManageColor("#e00000");
    new_te.changeFontFamily(id_default);
    new_te.changeSizePx(100);
    new_te.appendFragment("Sapone10", tex_ed);
    new_te.deleteFragmentLayoutAdjustment(fd3);
    new_te.ManageColor("#00eeee");
    //new_te.substituteFragment(fd1, "Sapone20", tex_ed);
    new_te.ManageColor("#00ee00");
    //new_te.substituteFragment(fd3, "Sapone21", tex_ed);
    new_te.changeFontFamily(id_complex);
    new_te.changeSizePx(60);
    new_te.appendFragment("Sapon11", tex_ed);
    new_te.changeSizePx(130);
    new_te.appendFragment("Sapone12", tex_ed);
    

    

    /*new_te.internalDelete(fd1);
    new_te.internalDelete(fd2);
    new_te.internalDelete(fd3);*/

    
    new_te.printStructure();

    //te.printMap();

    //te.printLines();
    
    //int width;
    //int height;
    //int n;
    //unsigned char* data = stbi_load("C:/sr/lp.png", &width, &height, &n, 0);

    //std::cout << "\nEcco " << n << " w: " << width << " h: " << height << std::endl;

    //unsigned char alpha_new = 0;
    //int index = 0;
    //for (int j = 0; j < height; j++) {
    //    for (int i = 0; i < width; i++) {

    //        index = j * width * 4 + i * 4 + 0;

    //        //alpha_new = tex_ed.texture[index + 3] + data[index + 3] * (1 - tex_ed.texture[index + 3]);

    //        //if (alpha_new != 0) {

    //        //    //red
    //        //    new_image[index] = (data[index] * data[index + 3] + tex_ed.texture[index] * tex_ed.texture[index + 3] * (1 - data[index + 3])) / alpha_new;
    //        //    //green
    //        //    new_image[index + 1] = (data[index + 1] * data[index + 3] + tex_ed.texture[index + 1] * tex_ed.texture[index + 3] * (1 - data[index + 3])) / alpha_new;
    //        //    //blue
    //        //    new_image[index + 2] = (data[index + 2] * data[index + 3] + tex_ed.texture[index + 2] * tex_ed.texture[index + 3] * (1 - data[index + 3])) / alpha_new;
    //        //    //alpha
    //        //    new_image[index + 3] = alpha_new;

    //        //}
    //        //else {

    //        //    //red
    //        //    new_image[index] = 0;
    //        //    //green
    //        //    new_image[index + 1] = 0;
    //        //    //blue
    //        //    new_image[index + 2] = 0;
    //        //    //alpha
    //        //    new_image[index + 3] = 0;
    //        //}



    //        if (tex_ed.texture[index + 3] != 0) {

    //            data[index] = tex_ed.texture[index];
    //            data[index + 1] = tex_ed.texture[index + 1];
    //            data[index + 2] = tex_ed.texture[index + 2];
    //            data[index + 3] = tex_ed.texture[index + 3];

    //        }

    //    }
    //}
    //



    //Texture tex_new = Texture(data, width, height);

    

    //ImageWriter::createPng("test_from_editor.png", text_parse);
    //ImageWriter::createPng("test_from_creator_default.png", text_created_default);
    //ImageWriter::createPng("test_from_creator_complex.png", text_created_complex);
    ImageWriter::createPng("test_new_editor_dilate.png", tex_ed);
    //ImageWriter::createJpg("test.jpg", tex_ed, 100);


    /*Texture text_parse;
    tm.parseFromEditor(in_big, text_parse);*/





    //std::cout << "\nDelta time t2-t1: "
    //    << std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count()
    //    << " microseconds" << std::endl;\


    return 0;



}