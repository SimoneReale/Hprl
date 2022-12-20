#pragma once
#define _CRT_SECURE_NO_WARNINGS
#define STB_TRUETYPE_IMPLEMENTATION 
#define  _SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING
#define _SILENCE_CXX20_CODECVT_FACETS_DEPRECATION_WARNING
#include "stb_truetype.h"
#include "json.hpp"
#include <iostream>
#include <stdio.h>
#include <vector>
#include <string>
#include <unordered_map>


#define DEFAULT_CHAR_HEIGHT_PX 18
#define DEFAULT_LEFT_MARGIN_PX 18
#define DEFAULT_UPPER_MARGIN   18
#define DEFAULT_TEXTURE_CACHE_CAPACITY 100
#define DEFAULT_DILATION_KERNEL_SIZE 3

typedef std::u32string  hprl_string;
typedef char32_t        hprl_char;
using json = nlohmann::json;

//dividere implementazione ed interfaccia anche da queste funzioni
inline unsigned int to_uint(char ch)
{
    return static_cast<unsigned int>(static_cast<unsigned char>(ch));
}
inline hprl_string to_utf32(std::string const& s)
{
    std::wstring_convert<std::codecvt<char32_t, char, std::mbstate_t>, char32_t > convert;
    auto asInt = convert.from_bytes(s);
    return hprl_string(reinterpret_cast<char32_t const*>(asInt.data()), asInt.length());
}

unsigned char* grayscale_dilation(unsigned char* in, unsigned int in_width, unsigned int in_height, int kernel_size) {

    
    int* kernel = new int[kernel_size * kernel_size];


    for (int i = 0; i < kernel_size * kernel_size; i++) {

        kernel[i] = 1;
    }


    unsigned char* out = new unsigned char[in_width * in_height];


    for (int i = 0; i < in_width * in_height; i++) {

        out[i] = 0;

    }

    int max;
    int offset = kernel_size / 2;
    //lascio stare i bordi
    for (int idxRow = offset; idxRow < in_height - offset; idxRow++) {
        for (int idxCol = offset; idxCol < in_width - offset; idxCol++) {
            max = 0;
            //slide del kernel
            for (int krnRow = -offset; krnRow < offset + 1; krnRow++) {
                for (int krnCol = -offset; krnCol < offset + 1; krnCol++) {
                    if (kernel[(offset + krnRow) * (kernel_size) + offset + krnCol] *
                        in[(idxRow + krnRow) * in_width + idxCol + krnCol] > max) {
                        max = in[(idxRow + krnRow) * in_width + idxCol + krnCol];
                    }
                }
            }

            out[idxRow * in_width + idxCol] = max;
        }

    }

        return out;

}

unsigned char* shear(unsigned char* in, unsigned int in_width, unsigned int in_height) {
    

    unsigned char* out = new unsigned char[in_width * in_height];

    for (int i = 0; i < in_width * in_height; i++) {

        out[i] = 0;

    }

    float shear_coefficient = 0.10;
    int new_x = 0;
    int new_y = 0;

    for (int x = 0; x < in_width; x++) {
        for (int y = 0; y < in_height; y++) {

            new_x = int(x + y * shear_coefficient);
            new_y = y;

            if (new_x * in_width + new_y < in_width * in_height + 1) {
            
                out[new_x * in_width + new_y] = in[x * in_width + y];
            
            }

        }
    }

    return out;

}


struct Texture {

    unsigned char* texture;
    unsigned int width;
    unsigned int height;
    unsigned long size_bytes;

    Texture(unsigned char* texture_ini, unsigned int width_ini, unsigned int height_ini) {

        texture = texture_ini;
        width = width_ini;
        height = height_ini;

        size_bytes = sizeof(this) + sizeof(unsigned char) * width * height * 4;

    }

    Texture() {
        texture = nullptr;
        width = 0;
        height = 0;
        size_bytes = 0;
    }

    };

class TextureSizeHandler {

    public:
        static void resize_texture(unsigned char* dest, int dwidth, int dheight, unsigned char* src, int swidth, int sheight)
    {
        if (dwidth == swidth && sheight == dheight)
            memcpy(dest, src, dwidth * dheight * 4);
        else if (dwidth > swidth || dheight > sheight)
            bilinear_interpolation(dest, dwidth, dheight, src, swidth, sheight);
        else
            shrink_average_method(dest, dwidth, dheight, src, swidth, sheight);
    }

    private:
        static void shrink_average_method(unsigned char* dest, int dwidth, int dheight, unsigned char* src, int swidth, int sheight)
    {
        #define clamp(x,low,high) (x) < (low) ? (low) : (x) > (high) ? (high) : (x)

        int x, y;
        int i, ii;
        float red, green, blue, alpha;
        float xfrag, yfrag, xfrag2, yfrag2;
        float xt, yt, dx, dy;
        int xi, yi;


        dx = ((float)swidth) / dwidth;
        dy = ((float)sheight) / dheight;

        for (yt = 0, y = 0; y < dheight; y++, yt += dy)
        {
            yfrag = ceil(yt) - yt;
            if (yfrag == 0)
                yfrag = 1;
            yfrag2 = yt + dy - (float)floor(yt + dy);
            if (yfrag2 == 0 && dy != 1.0f)
                yfrag2 = 1;

            for (xt = 0, x = 0; x < dwidth; x++, xt += dx)
            {
                xi = (int)xt;
                yi = (int)yt;
                xfrag = (float)ceil(xt) - xt;
                if (xfrag == 0)
                    xfrag = 1;
                xfrag2 = xt + dx - (float)floor(xt + dx);
                if (xfrag2 == 0 && dx != 1.0f)
                    xfrag2 = 1;
                red = xfrag * yfrag * src[(yi * swidth + xi) * 4];
                green = xfrag * yfrag * src[(yi * swidth + xi) * 4 + 1];
                blue = xfrag * yfrag * src[(yi * swidth + xi) * 4 + 2];
                alpha = xfrag * yfrag * src[(yi * swidth + xi) * 4 + 3];

                for (i = 0; xi + i + 1 < xt + dx - 1; i++)
                {
                    red += yfrag * src[(yi * swidth + xi + i + 1) * 4];
                    green += yfrag * src[(yi * swidth + xi + i + 1) * 4 + 1];
                    blue += yfrag * src[(yi * swidth + xi + i + 1) * 4 + 2];
                    alpha += yfrag * src[(yi * swidth + xi + i + 1) * 4 + 3];
                }

                red += xfrag2 * yfrag * src[(yi * swidth + xi + i + 1) * 4];
                green += xfrag2 * yfrag * src[(yi * swidth + xi + i + 1) * 4 + 1];
                blue += xfrag2 * yfrag * src[(yi * swidth + xi + i + 1) * 4 + 2];
                alpha += xfrag2 * yfrag * src[(yi * swidth + xi + i + 1) * 4 + 3];


                for (i = 0; yi + i + 1 < yt + dy - 1 && yi + i + 1 < sheight; i++)
                {
                    red += xfrag * src[((yi + i + 1) * swidth + xi) * 4];
                    green += xfrag * src[((yi + i + 1) * swidth + xi) * 4 + 1];
                    blue += xfrag * src[((yi + i + 1) * swidth + xi) * 4 + 2];
                    alpha += xfrag * src[((yi + i + 1) * swidth + xi) * 4 + 3];

                    for (ii = 0; xi + ii + 1 < xt + dx - 1 && xi + ii + 1 < swidth; ii++)
                    {
                        red += src[((yi + i + 1) * swidth + xi + ii + 1) * 4];
                        green += src[((yi + i + 1) * swidth + xi + ii + 1) * 4 + 1];
                        blue += src[((yi + i + 1) * swidth + xi + ii + 1) * 4 + 2];
                        alpha += src[((yi + i + 1) * swidth + xi + ii + 1) * 4 + 3];
                    }

                    if (yi + i + 1 < sheight && xi + ii + 1 < swidth)
                    {
                        red += xfrag2 * src[((yi + i + 1) * swidth + xi + ii + 1) * 4];
                        green += xfrag2 * src[((yi + i + 1) * swidth + xi + ii + 1) * 4 + 1];
                        blue += xfrag2 * src[((yi + i + 1) * swidth + xi + ii + 1) * 4 + 2];
                        alpha += xfrag2 * src[((yi + i + 1) * swidth + xi + ii + 1) * 4 + 3];
                    }
                }

                if (yi + i + 1 < sheight)
                {
                    red += xfrag * yfrag2 * src[((yi + i + 1) * swidth + xi) * 4];
                    green += xfrag * yfrag2 * src[((yi + i + 1) * swidth + xi) * 4 + 1];
                    blue += xfrag * yfrag2 * src[((yi + i + 1) * swidth + xi) * 4 + 2];
                    alpha += xfrag * yfrag2 * src[((yi + i + 1) * swidth + xi) * 4 + 3];

                    for (ii = 0; xi + ii + 1 < xt + dx - 1 && xi + ii + 1 < swidth; ii++)
                    {
                        red += yfrag2 * src[((yi + i + 1) * swidth + xi + ii + 1) * 4];
                        green += yfrag2 * src[((yi + i + 1) * swidth + xi + ii + 1) * 4 + 1];
                        blue += yfrag2 * src[((yi + i + 1) * swidth + xi + ii + 1) * 4 + 2];
                        alpha += yfrag2 * src[((yi + i + 1) * swidth + xi + ii + 1) * 4 + 3];
                    }
                }

                if (yi + i + 1 < sheight && xi + ii + 1 < swidth)
                {
                    red += xfrag2 * yfrag2 * src[((yi + i + 1) * swidth + xi + ii + 1) * 4];
                    green += xfrag2 * yfrag2 * src[((yi + i + 1) * swidth + xi + ii + 1) * 4 + 1];
                    blue += xfrag2 * yfrag2 * src[((yi + i + 1) * swidth + xi + ii + 1) * 4 + 2];
                    alpha += xfrag2 * yfrag2 * src[((yi + i + 1) * swidth + xi + ii + 1) * 4 + 3];
                }


                red /= dx * dy;
                green /= dx * dy;
                blue /= dx * dy;
                alpha /= dx * dy;

                red = clamp(red, 0, 255);
                green = clamp(green, 0, 255);
                blue = clamp(blue, 0, 255);
                alpha = clamp(alpha, 0, 255);

                dest[(y * dwidth + x) * 4] = (unsigned char)red;
                dest[(y * dwidth + x) * 4 + 1] = (unsigned char)green;
                dest[(y * dwidth + x) * 4 + 2] = (unsigned char)blue;
                dest[(y * dwidth + x) * 4 + 3] = (unsigned char)alpha;
            }
        }


    }
        static void bilinear_interpolation(unsigned char* dest, int dwidth, int dheight, unsigned char* src, int swidth, int sheight)
    {
        float a, b;
        float red, green, blue, alpha;
        float dx, dy;
        float rx, ry;
        int x, y;
        int index0, index1, index2, index3;

        dx = ((float)swidth) / dwidth;
        dy = ((float)sheight) / dheight;
        for (y = 0, ry = 0; y < dheight - 1; y++, ry += dy)
        {
            b = ry - (int)ry;
            for (x = 0, rx = 0; x < dwidth - 1; x++, rx += dx)
            {
                a = rx - (int)rx;
                index0 = (int)ry * swidth + (int)rx;
                index1 = index0 + 1;
                index2 = index0 + swidth;
                index3 = index0 + swidth + 1;

                red = src[index0 * 4] * (1.0f - a) * (1.0f - b);
                green = src[index0 * 4 + 1] * (1.0f - a) * (1.0f - b);
                blue = src[index0 * 4 + 2] * (1.0f - a) * (1.0f - b);
                alpha = src[index0 * 4 + 3] * (1.0f - a) * (1.0f - b);
                red += src[index1 * 4] * (a) * (1.0f - b);
                green += src[index1 * 4 + 1] * (a) * (1.0f - b);
                blue += src[index1 * 4 + 2] * (a) * (1.0f - b);
                alpha += src[index1 * 4 + 3] * (a) * (1.0f - b);
                red += src[index2 * 4] * (1.0f - a) * (b);
                green += src[index2 * 4 + 1] * (1.0f - a) * (b);
                blue += src[index2 * 4 + 2] * (1.0f - a) * (b);
                alpha += src[index2 * 4 + 3] * (1.0f - a) * (b);
                red += src[index3 * 4] * (a) * (b);
                green += src[index3 * 4 + 1] * (a) * (b);
                blue += src[index3 * 4 + 2] * (a) * (b);
                alpha += src[index3 * 4 + 3] * (a) * (b);

                red = red < 0 ? 0 : red > 255 ? 255 : red;
                green = green < 0 ? 0 : green > 255 ? 255 : green;
                blue = blue < 0 ? 0 : blue > 255 ? 255 : blue;
                alpha = alpha < 0 ? 0 : alpha > 255 ? 255 : alpha;

                dest[(y * dwidth + x) * 4] = (unsigned char)red;
                dest[(y * dwidth + x) * 4 + 1] = (unsigned char)green;
                dest[(y * dwidth + x) * 4 + 2] = (unsigned char)blue;
                dest[(y * dwidth + x) * 4 + 3] = (unsigned char)alpha;
            }
            index0 = (int)ry * swidth + (int)rx;
            index1 = index0;
            index2 = index0 + swidth;
            index3 = index0 + swidth;

            red = src[index0 * 4] * (1.0f - a) * (1.0f - b);
            green = src[index0 * 4 + 1] * (1.0f - a) * (1.0f - b);
            blue = src[index0 * 4 + 2] * (1.0f - a) * (1.0f - b);
            alpha = src[index0 * 4 + 3] * (1.0f - a) * (1.0f - b);
            red += src[index1 * 4] * (a) * (1.0f - b);
            green += src[index1 * 4 + 1] * (a) * (1.0f - b);
            blue += src[index1 * 4 + 2] * (a) * (1.0f - b);
            alpha += src[index1 * 4 + 3] * (a) * (1.0f - b);
            red += src[index2 * 4] * (1.0f - a) * (b);
            green += src[index2 * 4 + 1] * (1.0f - a) * (b);
            blue += src[index2 * 4 + 2] * (1.0f - a) * (b);
            alpha += src[index2 * 4 + 3] * (1.0f - a) * (b);
            red += src[index3 * 4] * (a) * (b);
            green += src[index3 * 4 + 1] * (a) * (b);
            blue += src[index3 * 4 + 2] * (a) * (b);
            alpha += src[index3 * 4 + 3] * (a) * (b);

            red = red < 0 ? 0 : red > 255 ? 255 : red;
            green = green < 0 ? 0 : green > 255 ? 255 : green;
            blue = blue < 0 ? 0 : blue > 255 ? 255 : blue;

            alpha = alpha < 0 ? 0 : alpha > 255 ? 255 : alpha;
            dest[(y * dwidth + x) * 4] = (unsigned char)red;
            dest[(y * dwidth + x) * 4 + 1] = (unsigned char)green;
            dest[(y * dwidth + x) * 4 + 2] = (unsigned char)blue;
            dest[(y * dwidth + x) * 4 + 3] = (unsigned char)alpha;
        }
        index0 = (int)ry * swidth + (int)rx;
        index1 = index0;
        index2 = index0 + swidth;
        index3 = index0 + swidth;

        for (x = 0, rx = 0; x < dwidth - 1; x++, rx += dx)
        {
            a = rx - (int)rx;
            index0 = (int)ry * swidth + (int)rx;
            index1 = index0 + 1;
            index2 = index0;
            index3 = index0;

            red = src[index0 * 4] * (1.0f - a) * (1.0f - b);
            green = src[index0 * 4 + 1] * (1.0f - a) * (1.0f - b);
            blue = src[index0 * 4 + 2] * (1.0f - a) * (1.0f - b);
            alpha = src[index0 * 4 + 3] * (1.0f - a) * (1.0f - b);
            red += src[index1 * 4] * (a) * (1.0f - b);
            green += src[index1 * 4 + 1] * (a) * (1.0f - b);
            blue += src[index1 * 4 + 2] * (a) * (1.0f - b);
            alpha += src[index1 * 4 + 3] * (a) * (1.0f - b);
            red += src[index2 * 4] * (1.0f - a) * (b);
            green += src[index2 * 4 + 1] * (1.0f - a) * (b);
            blue += src[index2 * 4 + 2] * (1.0f - a) * (b);
            alpha += src[index2 * 4 + 3] * (1.0f - a) * (b);
            red += src[index3 * 4] * (a) * (b);
            green += src[index3 * 4 + 1] * (a) * (b);
            blue += src[index3 * 4 + 2] * (a) * (b);
            alpha += src[index3 * 4 + 3] * (a) * (b);

            red = red < 0 ? 0 : red > 255 ? 255 : red;
            green = green < 0 ? 0 : green > 255 ? 255 : green;
            blue = blue < 0 ? 0 : blue > 255 ? 255 : blue;
            alpha = alpha < 0 ? 0 : alpha > 255 ? 255 : alpha;

            dest[(y * dwidth + x) * 4] = (unsigned char)red;
            dest[(y * dwidth + x) * 4 + 1] = (unsigned char)green;
            dest[(y * dwidth + x) * 4 + 2] = (unsigned char)blue;
            dest[(y * dwidth + x) * 4 + 3] = (unsigned char)alpha;
        }

        dest[(y * dwidth + x) * 4] = src[((sheight - 1) * swidth + swidth - 1) * 4];
        dest[(y * dwidth + x) * 4 + 1] = src[((sheight - 1) * swidth + swidth - 1) * 4 + 1];
        dest[(y * dwidth + x) * 4 + 2] = src[((sheight - 1) * swidth + swidth - 1) * 4 + 2];
        dest[(y * dwidth + x) * 4 + 3] = src[((sheight - 1) * swidth + swidth - 1) * 4 + 3];
    }

};

struct RasterizedGlyph {

    unsigned char* bitmap;
    int x0, y0, x1, y1;
    int advance;
    int lsb;

};

struct FontInstance {

    float scale_factor;
    int max_h_bbox_font;
    int max_w_bbox_font;
    std::unordered_map<char32_t, RasterizedGlyph> glyphs;

};

struct FontBoundingBox {

    int x0, y0, x1, y1;

};

struct FontColor {

    float r, g, b, alpha;
    static struct FontColor colorConverter(std::string hexValue, float alpha)
    {
        if (hexValue.at(0) == '#') {
            hexValue.erase(0, 1);
        }

        int hex = stoul(hexValue, nullptr, 16);
        struct FontColor rgbaColor;
        rgbaColor.r = ((hex >> 16) & 0xFF) / 255.0;
        rgbaColor.g = ((hex >> 8) & 0xFF) / 255.0;
        rgbaColor.b = ((hex) & 0xFF) / 255.0;
        rgbaColor.alpha = alpha;

        return rgbaColor;
    }


};

struct FontFaceDescription {

    unsigned char* font_data;
    int offset;
    std::string font_face_prop;

    FontFaceDescription(std::string font_path_ini, int offset_ini, std::string font_face_prop_ini) {
        font_data = readFontData(font_path_ini);
        offset = offset_ini;
        font_face_prop = font_face_prop_ini;
    }
    FontFaceDescription(unsigned char* font_data_ini, int offset_ini, std::string font_face_prop_ini) {
        font_data = font_data_ini;
        offset = offset_ini;
        font_face_prop = font_face_prop_ini;
    }

    private:
        unsigned char* readFontData(std::string font_path) {

        FILE* fp = fopen(font_path.c_str(), "rb");

        if (fp == NULL) {

            throw std::runtime_error("Failed to load font file");

        }

        long sz;
        fseek(fp, 0, SEEK_END);
        sz = ftell(fp);
        fseek(fp, 0, SEEK_SET);
        unsigned char* font_data = new unsigned char[sz];
        fread(font_data, 1, sz, fp);
        fclose(fp);

        return font_data;

    }

};

struct BasicFontDescription {

    FontFaceDescription* normal;
    FontFaceDescription* bold;
    FontFaceDescription* italic;
    FontFaceDescription* bolditalic;
    std::vector<FontFaceDescription> other_ff_descr;

    BasicFontDescription(FontFaceDescription* normal_ini, FontFaceDescription* bold_ini, FontFaceDescription* italic_ini, FontFaceDescription* bolditalic_ini, std::vector<FontFaceDescription> other_ff_descr_ini) {
                
        if (normal_ini == nullptr) {
            throw std::runtime_error("NORMAL FONT CANT BE NULLPTR");
        }

        normal = normal_ini;

        if (bold_ini != nullptr) {
            bold = bold_ini;
        }
        else { bold = normal_ini; }

        if (italic_ini != nullptr) {
            italic = italic_ini;
        }
        else { italic = normal_ini; }

        if (bolditalic_ini != nullptr) {
            bolditalic = bolditalic_ini;
        }
        else { bolditalic = normal_ini; }
                
    }
           
};

struct HprlTextFragment {


    HprlTextFragment(json& text_frag) {

        content = to_utf32(text_frag["insert"]);
        content_utf8 = text_frag["insert"];
        text_attributes = text_frag["attributes"];

        if (text_attributes.contains("size")) {

            std::string size_s = text_attributes["size"];
            fragment_height_px = std::stoul(size_s.substr(0, size_s.size() - 2).c_str());
        }
        else {

            fragment_height_px = DEFAULT_CHAR_HEIGHT_PX;

        }

    }
    
    HprlTextFragment(std::string content_utf8_ini, unsigned int fragment_height_px_ini) {
        content_utf8 = content_utf8_ini;
        content = to_utf32(content_utf8);
        fragment_height_px = fragment_height_px_ini;

    }



    static HprlTextFragment createTextFragment(std::string content_ini) {

        return HprlTextFragment(content_ini, DEFAULT_CHAR_HEIGHT_PX);

    }


    HprlTextFragment& addSizePxToFragment(unsigned int size_px) {

        text_attributes["size"] = std::to_string(size_px).append("px");
        fragment_height_px = size_px;
        return *this;

    }

    HprlTextFragment& addColorToFragment(std::string hex_value) {

        text_attributes["color"] = hex_value;
        return *this;

    }

    HprlTextFragment& addUnderlineToFragment() {

        text_attributes["underline"] = true;
        return *this;
    }

    HprlTextFragment& addStrikeToFragment() {

        text_attributes["strike"] = true;
        return *this;
    }

    HprlTextFragment& addBoldnessToFragment() {

        text_attributes["bold"] = true;
        return *this;
    }

    HprlTextFragment& addItalicToFragment(HprlTextFragment& frag) {

        text_attributes["italic"] = true;
        return *this;
    }


    hprl_string content;
    json text_attributes;
    std::string content_utf8;
    unsigned int fragment_height_px;

};

struct HprlLine {



    HprlLine(json& line_ini, unsigned short& ordered_list_count) {

        max_char_height_line_px = 0;
        line_attributes = line_ini["line_attributes"];




        if (line_attributes.contains("list")) {

            std::string type_of_list = line_attributes["list"];


            if (type_of_list.compare(std::string("ordered")) == 0) {


                std::string list_ordered = std::to_string(ordered_list_count).append(".");

                text_fragments.push_back(HprlTextFragment(list_ordered, DEFAULT_CHAR_HEIGHT_PX));

                ordered_list_count++;
            }

            if (type_of_list.compare(std::string("bullet")) == 0) {

                ordered_list_count = 1;
                text_fragments.push_back(HprlTextFragment("-", DEFAULT_CHAR_HEIGHT_PX));

            }



        }
        else {

            ordered_list_count = 1;

        }

        for (json& x : line_ini["content"]["ops"]) {

            text_fragments.push_back(HprlTextFragment(x));

        }

        for (auto& x : text_fragments) {

            if (x.fragment_height_px > max_char_height_line_px) {

                max_char_height_line_px = x.fragment_height_px;

            }

        }




    }

    std::vector<HprlTextFragment> text_fragments;
    json line_attributes;
    unsigned int max_char_height_line_px;



};

struct HprlText {

public:

    HprlText(unsigned int width_px_ini, unsigned int height_px_ini, json& lines) {

        width_px = width_px_ini;
        height_px = height_px_ini;
        left_margin_px = DEFAULT_LEFT_MARGIN_PX;
        upper_margin_px = DEFAULT_UPPER_MARGIN;

        for (json& x : lines) {

            lines_vec.push_back(HprlLine(x, ordered_list_counter));

        }

    }
    HprlText(unsigned int width_px_ini, unsigned int height_px_ini, unsigned int left_margin_px_ini, unsigned int upper_margin_px_ini, std::vector<HprlLine> lines_vec_ini) {

        width_px = width_px_ini;
        height_px = height_px_ini;
        left_margin_px = left_margin_px_ini;
        upper_margin_px = upper_margin_px_ini;
        lines_vec = lines_vec_ini;

    }


    unsigned int width_px;
    unsigned int height_px;
    unsigned int left_margin_px;
    unsigned int upper_margin_px;
    unsigned short ordered_list_counter = 1;
    std::vector<HprlLine> lines_vec;

};

class FontFace {


public:

    int ascent;
    int descent;
    int line_gap;
    FontBoundingBox font_bbox;

    FontFace(unsigned char* font_data_ini, char32_t first_char, char32_t char_count, std::vector<float> heights_char_pixel, int offset);

    FontFace() = delete;



    RasterizedGlyph& getGlyph(char32_t requested_char, float height_char_pixel);
    float getScaleFactor(float height_char_pixel);

    FontFace& makeArtificialBold() {

        std::cout << "Sono nel make artifcial" << std::endl;

        std::cout << "Size: " << font_instances_collection.size() << std::endl;

        for (auto& f : font_instances_collection) {

            FontInstance f_ins = f.second;

            for (auto& r : f_ins.glyphs) {

                RasterizedGlyph r_glyph = r.second;

                unsigned int bitmap_width = r_glyph.x1 - r_glyph.x0;
                unsigned int bitmap_height = r_glyph.y1 - r_glyph.y0;

                r_glyph.bitmap = grayscale_dilation(r_glyph.bitmap, bitmap_width, bitmap_height, DEFAULT_DILATION_KERNEL_SIZE);

                

                r_glyph.advance = int(r_glyph.advance * 1.10);

            }

        }

        return *this;


    }



private:

    unsigned char* font_data;
    stbtt_fontinfo font_info;
    char32_t first_char_ini;
    char32_t char_count_ini;


    std::map<float, FontInstance> font_instances_collection;


    void initializeFontFace(char32_t first_char, char32_t char_count, std::vector<float>& heights_char_pixel);


};

class LRUTextureCache {

  public:

    LRUTextureCache(int capacity)
    {
        cap = capacity;
        head->next = tail;
        tail->prev = head;
    }
    LRUTextureCache() {

        cap = DEFAULT_TEXTURE_CACHE_CAPACITY;
    }

    bool at(std::size_t key, Texture& texture)
    {
        if (m.find(key) != m.end())
        {
            node* res = m[key];
            m.erase(key);
            Texture ans = res->value;
            deletenode(res);
            addnode(res);
            m[key] = head->next;
            texture = ans;
            return true;
        }

        return false;
    }
    void emplace(std::size_t key, Texture value)
    {
        if (m.find(key) != m.end())
        {
            node* exist = m[key];
            m.erase(key);
            deletenode(exist);
        }

        if (m.size() == cap)
        {
            m.erase(tail->prev->key);
            deletenode(tail->prev);
        }
        addnode(new node(key, value));
        m[key] = head->next;
    }
    bool contains(std::size_t key) {
        if (m.find(key) != m.end())
        {
            return true;
        }
        
        return false;
    }

  private:

    class node
    {
    public:
        std::size_t key;
        Texture value;
        node* prev;
        node* next;

        node(std::size_t key_ini, Texture value_ini)
        {
            key = key_ini;
            value = value_ini;
        }
    };
    node* head = new node(0, Texture());
    node* tail = new node(0, Texture());
    int cap;
    std::map<std::size_t, node*> m;

    void addnode(node* temp)
    {
        node* dummy = head->next;
        head->next = temp;
        temp->prev = head;
        temp->next = dummy;
        dummy->prev = temp;
    }
    void deletenode(node* temp)
    {
        node* delnext = temp->next;
        node* delprev = temp->prev;
        delnext->prev = delprev;
        delprev->next = delnext;
    }

};

class TextManager {

public:
            
    TextManager(BasicFontDescription& basic_ff_descr, char32_t first_char, char32_t char_count, std::vector<float>& heights_char_pixel, unsigned int cache_capacity);

    TextManager() = delete;


    std::size_t parse(std::u8string input, Texture& texture);

    bool getTextureFromCache(std::size_t hash, Texture& dst_texture);

    std::size_t getHashFromInputString(std::u8string input);

    unsigned char* createTexture(HprlText& text);



private:

    std::unordered_map<std::string, FontFace> basic_font_faces_map;
    std::unordered_map<std::string, FontFace> other_font_faces_map;
    LRUTextureCache texture_cache;


    void printTextFragment(HprlTextFragment& frag, unsigned char* work_buffer, unsigned char* single_chan_buf, int tex_w, int tex_h, int& xpos, int& baseline);

    int getLineLength(HprlLine& line);

    int getFragmentLength(HprlTextFragment& frag);

};



#ifdef HPRL_IMPLEMENTATION

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
////
//// IMPLEMENTATION
////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////
//
// Font Face
//
//////////////////////////////////////////////////////////////////////////////

FontFace::FontFace(unsigned char* font_data_ini, char32_t first_char, char32_t char_count, std::vector<float> heights_char_pixel, int offset) {

    this->font_data = font_data_ini;
    this->first_char_ini = first_char;
    this->char_count_ini = char_count;
    stbtt_InitFont(&font_info, font_data, offset);
    stbtt_GetFontVMetrics(&font_info, &ascent, &descent, &line_gap);
    initializeFontFace(first_char, char_count, heights_char_pixel);

}
RasterizedGlyph& FontFace::getGlyph(char32_t requested_char, float height_char_pixel) {

    //std::cout << "Font instance collection size: " << font_instances_collection.size() << "\n" << std::endl;

    if (!font_instances_collection.contains(height_char_pixel)) {

        //std::cout << "Height does not exist\n" << std::endl;

        std::vector h{ height_char_pixel };
        initializeFontFace(requested_char, 1, h);

    }
    else {

        float scale_factor = font_instances_collection[height_char_pixel].scale_factor;
        if (!font_instances_collection.at(height_char_pixel).glyphs.contains(requested_char)) {

            /*std::cout << "Cache miss " << char(to_uint(requested_char)) << std::endl;
            std::cout << "Num of glyphs: " << font_instances_collection.at(height_char_pixel).glyphs.size() << std::endl;
            std::cout << "Font instance collection size: " << font_instances_collection.size() << "\n" << std::endl;*/
            // not found
            int x0, x1, y0, y1;
            stbtt_GetCodepointBitmapBox(&font_info, requested_char, scale_factor, scale_factor, &x0, &y0, &x1, &y1);

            int x_length = x1 - x0;
            int y_length = y1 - y0;
            int advance = 0, lsb = 0;

            unsigned char* temp_bitmap = new unsigned char[x_length * y_length];
            stbtt_MakeCodepointBitmap(&font_info, temp_bitmap, x_length, y_length, x_length, scale_factor, scale_factor, requested_char);
            stbtt_GetCodepointHMetrics(&font_info, requested_char, &advance, &lsb);
            RasterizedGlyph new_glyph = { temp_bitmap, x0, y0, x1, y1, advance, lsb };

            font_instances_collection.at(height_char_pixel).glyphs[requested_char] = new_glyph;

        }
        else {

            /*std::cout << "Cache hit: " << char(to_uint(requested_char))  << std::endl;
            std::cout << "Num of glyphs: " << font_instances_collection.at(height_char_pixel).glyphs.size() << std::endl;
            std::cout << "Font instance collection size: " << font_instances_collection.size() << "\n" << std::endl;*/
        }

    }


    return font_instances_collection.at(height_char_pixel).glyphs[requested_char];

}
float FontFace::getScaleFactor(float height_char_pixel) {

    if (font_instances_collection.contains(height_char_pixel)) {

        return font_instances_collection[height_char_pixel].scale_factor;
    }
    else {

        std::vector h{ height_char_pixel };
        initializeFontFace(32, 1, h);


        return font_instances_collection[height_char_pixel].scale_factor;

    }
}
void FontFace::initializeFontFace(char32_t first_char, char32_t char_count, std::vector<float>& heights_char_pixel) {


    for (float h : heights_char_pixel) {


        FontInstance font_instance;

        float scale_factor = stbtt_ScaleForPixelHeight(&font_info, h);
        int max_h_bbox_font = int((font_bbox.y1 - font_bbox.y0) * scale_factor);
        int max_w_bbox_font = int((font_bbox.x1 - font_bbox.x0) * scale_factor);


        for (int i = first_char; i < first_char + char_count; i++) {

            int x0, x1, y0, y1;
            stbtt_GetCodepointBitmapBox(&font_info, i, scale_factor, scale_factor, &x0, &y0, &x1, &y1);

            int x_length = x1 - x0;
            int y_length = y1 - y0;
            int advance = 0, lsb = 0;

            unsigned char* temp_bitmap = new unsigned char[x_length * y_length];
            stbtt_MakeCodepointBitmap(&font_info, temp_bitmap, x_length, y_length, x_length, scale_factor, scale_factor, i);
            stbtt_GetCodepointHMetrics(&font_info, i, &advance, &lsb);

            font_instance.glyphs[i] = { temp_bitmap, x0, y0, x1, y1, advance, lsb };


        }

        font_instance.scale_factor = scale_factor;
        font_instance.max_h_bbox_font = max_h_bbox_font;
        font_instance.max_w_bbox_font = max_w_bbox_font;
        font_instances_collection[h] = font_instance;


    }
}

//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
//
// Text Manager
//
//////////////////////////////////////////////////////////////////////////////

TextManager::TextManager(BasicFontDescription& basic_ff_descr, char32_t first_char, char32_t char_count, std::vector<float>& heights_char_pixel, unsigned int cache_capacity) {

    if (cache_capacity > 0) {
        texture_cache = LRUTextureCache(cache_capacity);
    }
    else {
        //default capacity
        texture_cache = LRUTextureCache();
    }


    FontFace bold_face = FontFace(basic_ff_descr.normal->font_data, first_char, char_count, heights_char_pixel, basic_ff_descr.normal->offset);
    std::cout << "Sto nel text manager" << std::endl;

    bold_face.makeArtificialBold();

    basic_font_faces_map.emplace("normal", FontFace(basic_ff_descr.normal->font_data, first_char, char_count, heights_char_pixel, basic_ff_descr.normal->offset));
    //basic_font_faces_map.emplace("bold", FontFace(basic_ff_descr.bold->font_data, first_char, char_count, heights_char_pixel, basic_ff_descr.bold->offset));
    basic_font_faces_map.emplace("bold", bold_face);
    basic_font_faces_map.emplace("italic", FontFace(basic_ff_descr.italic->font_data, first_char, char_count, heights_char_pixel, basic_ff_descr.italic->offset));
    basic_font_faces_map.emplace("bolditalic", FontFace(basic_ff_descr.bolditalic->font_data, first_char, char_count, heights_char_pixel, basic_ff_descr.bolditalic->offset));


    for (FontFaceDescription& f : basic_ff_descr.other_ff_descr) {

        other_font_faces_map.emplace(f.font_face_prop, FontFace(f.font_data, first_char, char_count, heights_char_pixel, f.offset));

    }

}

std::size_t TextManager::parse(std::u8string input, Texture& texture) {

    std::size_t h1 = std::hash<std::u8string>{}(input);

    if (!texture_cache.contains(h1)) {

        json text_descr = json::parse(input);

        json lines = text_descr["lines"];
        json dimensions = text_descr["dimensions"];

        std::string width_s = dimensions["width"];
        std::string height_s = dimensions["height"];

        if (width_s.size() <= 2 || height_s.size() <= 2) {

            throw std::runtime_error("BAD DIMENSIONS IN PARSED STRING");

        }


        //strip px
        unsigned int width = std::stoul(width_s.substr(0, width_s.size() - 2).c_str());
        unsigned int height = std::stoul(height_s.substr(0, height_s.size() - 2).c_str());
        HprlText newText = HprlText(width, height, lines);
        texture_cache.emplace(h1, Texture{ createTexture(newText) , width, height });
}

    texture_cache.at(h1, texture);

    return h1;

    }

bool TextManager::getTextureFromCache(std::size_t hash, Texture& dst_texture) {

    if (texture_cache.contains(hash)) {

        texture_cache.at(hash, dst_texture);
        return true;
    }
    else {
        return false;
    }


}

std::size_t TextManager::getHashFromInputString(std::u8string input) {

    return std::hash<std::u8string>{}(input);
}

unsigned char* TextManager::createTexture(HprlText& text) {


    int tex_w = text.width_px, tex_h = text.height_px;
    unsigned char* work_buffer = new unsigned char[tex_h * tex_w * 4];
    unsigned char* single_channel_buff = new unsigned char[tex_h * tex_w];
    int center_x_texture = ceil(tex_w / 2);

    for (int i = 0; i < tex_w * tex_h; i++) {

        work_buffer[i * 4] = 0;
        work_buffer[i * 4 + 1] = 0;
        work_buffer[i * 4 + 2] = 0;
        work_buffer[i * 4 + 3] = 0;

    }

    for (int i = 0; i < tex_w * tex_h; i++) {

        single_channel_buff[i] = 1;

    }

    int ascent = basic_font_faces_map.at("normal").ascent;
    int descent = basic_font_faces_map.at("normal").descent;
    int line_gap = basic_font_faces_map.at("normal").line_gap;

    float max_height_ini = text.lines_vec[0].max_char_height_line_px;
    int baseline = (int)(basic_font_faces_map.at("normal").ascent * basic_font_faces_map.at("normal").getScaleFactor(max_height_ini));



    for (int i = 0; i < text.lines_vec.size(); i++) {


        json line_attributes = text.lines_vec[i].line_attributes;
        int xpos = 0;



        if (line_attributes.contains("align")) {

            std::string alignment = line_attributes["align"];



            if (alignment.compare(std::string("center")) == 0) {

                xpos = center_x_texture - ceil(getLineLength(text.lines_vec[i]) / 2);
            }

            if (alignment.compare(std::string("right")) == 0) {

                xpos = tex_w - getLineLength(text.lines_vec[i]);
            }


        }



        for (HprlTextFragment frag : text.lines_vec[i].text_fragments) {

            printTextFragment(frag, work_buffer, single_channel_buff, tex_w, tex_h, xpos, baseline);
        }

        if (i < text.lines_vec.size() - 1) {
            int max_height = text.lines_vec[i + 1].max_char_height_line_px;
            baseline += (ascent - descent + line_gap) * basic_font_faces_map.at("normal").getScaleFactor(max_height);
        }
    }

    return work_buffer;

}

void TextManager::printTextFragment(HprlTextFragment& frag, unsigned char* work_buffer, unsigned char* single_chan_buf, int tex_w, int tex_h, int& xpos, int& baseline) {

    json attributes = frag.text_attributes;
    unsigned int height = frag.fragment_height_px;
    FontColor color;
    FontColor color_background;

    int initial_xpos = xpos;
    int strike_height = baseline - int(height / 3);
    int underline_height = baseline + int(height / 10);
    int thickness_of_line = ceil(height / 10);



    if (attributes.contains("color")) {

        color = FontColor::colorConverter(attributes["color"], 1.0f);

    }
    else {

        color = FontColor{ 0.0f, 0.0f, 0.0f, 1.0f };
    }





    FontFace work_font_face = basic_font_faces_map.at("normal");

    if (!attributes.contains("bold") && !attributes.contains("italic")) {

        work_font_face = basic_font_faces_map.at("normal");



    }


    if (attributes.contains("bold") && attributes.contains("italic")) {

        work_font_face = basic_font_faces_map.at("bolditalic");

    }

    if (attributes.contains("bold") && !attributes.contains("italic")) {


        work_font_face = basic_font_faces_map.at("bold");



    }

    if (!attributes.contains("bold") && attributes.contains("italic")) {

        work_font_face = basic_font_faces_map.at("italic");

    }



    int ch = 0;
    hprl_string text = frag.content;

    float scale_factor = work_font_face.getScaleFactor(height);


    while (to_uint(text[ch])) {
        unsigned int c = to_uint(text[ch]);

        RasterizedGlyph& glyph = work_font_face.getGlyph(c, height);

        int advance = glyph.advance;
        int lsb = glyph.lsb;
        int x0 = glyph.x0, y0 = glyph.y0, x1 = glyph.x1, y1 = glyph.y1;

        unsigned char* temp_buffer = glyph.bitmap;



        for (int j = y0; j < y1; j++) {
            for (int i = x0; i < x1; i++) {

                if (single_chan_buf[(baseline + j) * tex_w + ((int)xpos + i)] != 0 && temp_buffer[(j - y0) * (x1 - x0) + (i - x0)] == 0) {
                }
                else {

                    single_chan_buf[(baseline + j) * tex_w + ((int)xpos + i)] = temp_buffer[(j - y0) * (x1 - x0) + (i - x0)];
                }
            }
        }

        for (int j = y0; j < y1; j++) {
            for (int i = x0; i < x1; i++) {

                if (single_chan_buf[(baseline + j) * tex_w + ((int)xpos + i)] == 0) {


                    //red
                    work_buffer[(baseline + j) * tex_w * 4 + ((int)xpos + i) * 4 + 0] = 255;
                    //green
                    work_buffer[(baseline + j) * tex_w * 4 + ((int)xpos + i) * 4 + 1] = 255;
                    //blue
                    work_buffer[(baseline + j) * tex_w * 4 + ((int)xpos + i) * 4 + 2] = 255;
                    //alpha
                    work_buffer[(baseline + j) * tex_w * 4 + ((int)xpos + i) * 4 + 3] = 255;


                }


                if (single_chan_buf[(baseline + j) * tex_w + ((int)xpos + i)] > 0 && single_chan_buf[(baseline + j) * tex_w + ((int)xpos + i)] < 255) {
                    //red
                    work_buffer[(baseline + j) * tex_w * 4 + ((int)xpos + i) * 4 + 0] = unsigned char(color.r * single_chan_buf[(baseline + j) * tex_w + ((int)xpos + i)]);
                    //green
                    work_buffer[(baseline + j) * tex_w * 4 + ((int)xpos + i) * 4 + 1] = unsigned char(color.g * single_chan_buf[(baseline + j) * tex_w + ((int)xpos + i)]);
                    //blue
                    work_buffer[(baseline + j) * tex_w * 4 + ((int)xpos + i) * 4 + 2] = unsigned char(color.b * single_chan_buf[(baseline + j) * tex_w + ((int)xpos + i)]);
                    //alpha
                    work_buffer[(baseline + j) * tex_w * 4 + ((int)xpos + i) * 4 + 3] = single_chan_buf[(baseline + j) * tex_w + ((int)xpos + i)];
                }

                else {

                    //red
                    work_buffer[(baseline + j) * tex_w * 4 + ((int)xpos + i) * 4 + 0] = unsigned char(color.r * single_chan_buf[(baseline + j) * tex_w + ((int)xpos + i)]);
                    //green
                    work_buffer[(baseline + j) * tex_w * 4 + ((int)xpos + i) * 4 + 1] = unsigned char(color.g * single_chan_buf[(baseline + j) * tex_w + ((int)xpos + i)]);
                    //blue
                    work_buffer[(baseline + j) * tex_w * 4 + ((int)xpos + i) * 4 + 2] = unsigned char(color.b * single_chan_buf[(baseline + j) * tex_w + ((int)xpos + i)]);
                    //alpha
                    work_buffer[(baseline + j) * tex_w * 4 + ((int)xpos + i) * 4 + 3] = unsigned char(color.alpha * 255);

                }


            }

        }

        xpos += (advance * scale_factor);

        ++ch;
    }

    if (attributes.contains("strike")) {
        for (int y = strike_height; y < strike_height + thickness_of_line; y++) {
            for (int x = initial_xpos; x < xpos; x++) {

                work_buffer[y * tex_w * 4 + x * 4 + 0] = unsigned char(color.r * 255);
                work_buffer[y * tex_w * 4 + x * 4 + 1] = unsigned char(color.g * 255);
                work_buffer[y * tex_w * 4 + x * 4 + 2] = unsigned char(color.b * 255);
                work_buffer[y * tex_w * 4 + x * 4 + 3] = unsigned char(color.alpha * 255);

            }
        }
    }


    if (attributes.contains("underline")) {
        for (int y = underline_height; y < underline_height + thickness_of_line; y++) {
            for (int x = initial_xpos; x < xpos; x++) {

                work_buffer[y * tex_w * 4 + x * 4 + 0] = unsigned char(color.r * 255);
                work_buffer[y * tex_w * 4 + x * 4 + 1] = unsigned char(color.g * 255);
                work_buffer[y * tex_w * 4 + x * 4 + 2] = unsigned char(color.b * 255);
                work_buffer[y * tex_w * 4 + x * 4 + 3] = unsigned char(color.alpha * 255);

            }
        }
    }


}

int TextManager::getLineLength(HprlLine& line) {

    int total_length = 0;

    for (HprlTextFragment frag : line.text_fragments) {

        total_length += getFragmentLength(frag);

    }

    return total_length;



}

int TextManager::getFragmentLength(HprlTextFragment& frag) {

    json attributes = frag.text_attributes;
    hprl_string content = frag.content;
    unsigned int height_px = frag.fragment_height_px;

    FontFace work_font_face = basic_font_faces_map.at("normal");

    if (!attributes.contains("bold") && attributes.contains("italic")) {

        work_font_face = basic_font_faces_map.at("italic");

    }


    if (attributes.contains("bold") && attributes.contains("italic")) {

        work_font_face = basic_font_faces_map.at("bolditalic");

    }

    if (attributes.contains("bold") && !attributes.contains("italic")) {


        work_font_face = basic_font_faces_map.at("bold");


    }

    int ch = 0;
    float xpos = 0;


    while (content[ch]) {

        RasterizedGlyph& glyph = work_font_face.getGlyph(content[ch], height_px);
        int advance = glyph.advance;
        xpos += (advance * work_font_face.getScaleFactor(height_px));
        ++ch;
    }

    return ceil(xpos);

}

//////////////////////////////////////////////////////////////////////////////

#endif // !HPRL_IMPLEMANTATION


#ifdef HPRL_IMAGE_WRITER_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
        namespace ImageWriter {

            int createPng(char const* filename, Texture texture) {
                return stbi_write_png(filename, texture.width, texture.height, 4, texture.texture, texture.width * 4);
            }

            int createBmp(char const* filename, Texture texture) {

                return stbi_write_bmp(filename, texture.width, texture.height, 4, texture.texture);
            }

            //quality between 1 and 100
            int createJpg(char const* filename, Texture texture, int quality) {
                return stbi_write_jpg(filename, texture.width, texture.height, 4, texture.texture, quality);
            }


        }
#endif


    