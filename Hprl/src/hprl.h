#pragma once
#define _CRT_SECURE_NO_WARNINGS
#define STB_TRUETYPE_IMPLEMENTATION 
#define STB_IMAGE_WRITE_IMPLEMENTATION
#define  _SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING
#define _SILENCE_CXX20_CODECVT_FACETS_DEPRECATION_WARNING
#include "stb_truetype.h"
#include "stb_image_write.h"
#include "json.hpp"
#include <iostream>
#include <stdio.h>
#include<vector>
#include <string>
#include<map>
#include <unordered_map>


#define DEFAULT_CHAR_HEIGHT_PX 18

typedef std::u32string  hprl_string;
typedef char32_t        hprl_char;

using json = nlohmann::json;


inline unsigned int to_uint(char ch)
{
    
    return static_cast<unsigned int>(static_cast<unsigned char>(ch));
}


hprl_string to_utf32(std::string const& s)
{
    std::wstring_convert<std::codecvt<char32_t, char, std::mbstate_t>, char32_t > convert;
    auto asInt = convert.from_bytes(s);
    return hprl_string(reinterpret_cast<char32_t const*>(asInt.data()), asInt.length());
}



namespace hprl {

    namespace FontManager {


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
            std::map<char32_t, RasterizedGlyph> glyphs;

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

            const char* font_path;
            const int offset;
            const std::string font_face_prop;

        };

        struct HprlPackedchar {

            unsigned short x0, y0, x1, y1; // coordinates of bbox in bitmap
            float xoff, yoff, xadvance;
            float xoff2, yoff2;

            HprlPackedchar(stbtt_packedchar p_char) {

                x0 = p_char.x0;
                x1 = p_char.x1;
                y0 = p_char.y0;
                y1 = p_char.y1;
                xoff = p_char.xoff;
                yoff = p_char.yoff;
                xadvance = p_char.xadvance;
                xoff2 = p_char.xoff2;
                yoff2 = p_char.yoff2;

            }


        } ;

        struct HprlBakedFont {
            int   atlas_width;
            int   atlas_height;
            int   stride_in_bytes;
            int   padding;
            int   skip_missing;
            unsigned int   h_oversample, v_oversample;

            unsigned char* atlas_texture;
            std::vector<HprlPackedchar> char_data;
        

            HprlBakedFont(stbtt_pack_context p_con) {
                atlas_width = p_con.width;
                atlas_height = p_con.height;
                stride_in_bytes = p_con.stride_in_bytes;
                padding = p_con.padding;
                skip_missing = p_con.skip_missing;
                h_oversample = p_con.h_oversample,
                v_oversample = p_con.v_oversample;
                atlas_texture = p_con.pixels;
            }


        };

        unsigned char* convertSingle8_into_RGBA(unsigned char* single_ch_8bit_bitmap, int tex_w, int tex_h, FontColor& color) {

            unsigned char* new_buff = new unsigned char[tex_w * tex_h * 4];


            
            for (int i = 0; i < tex_h; i++) {
                for (int j = 0; j < tex_w;  j++) {


                    if (single_ch_8bit_bitmap[i * tex_w + j] == 0) {

                        //red
                        new_buff[i * tex_w * 4 + j * 4 + 0] = 0;
                        //green
                        new_buff[i * tex_w * 4 + j * 4 + 1] = 0;
                        //blue
                        new_buff[i * tex_w * 4 + j * 4 + 2] = 0;
                        //alpha
                        new_buff[i * tex_w * 4 + j * 4 + 3] = 0;


                    }

                    else {

                        //red
                        new_buff[i * tex_w * 4 + j * 4 + 0] = unsigned char(color.r * single_ch_8bit_bitmap[i * tex_w + j]);
                        //green
                        new_buff[i * tex_w * 4 + j * 4 + 1] = unsigned char(color.g * single_ch_8bit_bitmap[i * tex_w + j]);
                        //blue
                        new_buff[i * tex_w * 4 + j * 4 + 2] = unsigned char(color.b * single_ch_8bit_bitmap[i * tex_w + j]);
                        //alpha
                        new_buff[i * tex_w * 4 + j * 4 + 3] = unsigned char(color.alpha * 255);

                    }


                }

            }

         
            stbi_write_png("test.png", tex_w, tex_h, 4, new_buff, tex_w * 4);
            

            return new_buff;



        }

        unsigned char* readFontData(const char* font_path) {

            FILE* fp = fopen(font_path, "rb");

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

        enum ATTRIBUTES{
            BOLD,
            ITALIC,
            STRIKE,
            UNDERLINE,
            COLOR
        };

        enum LINE_ATTRIBUTES {

            ORDERED_LIST,
            UNORDERED_LIST,
            ALIGN
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

            hprl_string content;
            json text_attributes;
            std::string content_utf8;
            unsigned int fragment_height_px;


            void printToConsoleFragment() {

                std::cout << "Fragment content: " << content_utf8 << std::endl;
                std::cout <<"Attributes: " << text_attributes.dump() << std::endl;
            }

        };

        struct HprlLine {



            HprlLine(json& line_ini) {

                max_char_height_line_px = 0;
                line_attributes = line_ini["line_attributes"];

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

            void printToConsoleLine() {

                std::cout << "\n******************" << std::endl;
                for (auto x : text_fragments) {
                    x.printToConsoleFragment();
                }

                std::cout << "Line attributes: " << line_attributes.dump() << std::endl;
                std::cout << "Max height line: " << max_char_height_line_px << std::endl;
                std::cout << "\n******************" << std::endl;

            }


        };

        class HprlMultiLineText {

            public:
            
                HprlMultiLineText(unsigned int ini_width_px, unsigned int ini_height_px, json& lines) {

                    width_px = ini_width_px;
                    height_px = ini_height_px;

                    for (json& x : lines) {

                        lines_vec.push_back(HprlLine(x));

                    }


                }


                void printToConsoleText() {
                    for (auto x : lines_vec) {
                        x.printToConsoleLine();
                    }
            }

            private:

                unsigned int width_px;
                unsigned int height_px;
                std::vector<HprlLine> lines_vec;

               

        };

        //perchè height è in float?
        class FontFace {

           
        public:

            FontFace(const char* font_path, char32_t first_char, char32_t char_count, std::vector<float> heights_char_pixel, int offset) {

                this->font_path = font_path;
                this->font_data = readFontData(font_path);
                this->first_char_ini = first_char;
                this->char_count_ini = char_count;
                stbtt_InitFont(&font_info, font_data, offset);
                stbtt_GetFontVMetrics(&font_info, &ascent, &descent, &line_gap);
                stbtt_GetFontBoundingBox(&font_info, &font_bbox.x0, &font_bbox.y0, &font_bbox.x1, &font_bbox.y1);
                initializeFontFace(first_char, char_count, heights_char_pixel);

            }
            
            FontFace() = delete;

            unsigned char* createTextTextureSingleFontFace(std::vector<hprl_string>& lines, float height_char_pixel, FontColor& color) {
                
                if (font_instances_collection.find(height_char_pixel) == font_instances_collection.end()) {
                
                    std::vector h{ height_char_pixel };
                    initializeFontFace(first_char_ini, char_count_ini, h);
                
                }
                
                int tex_w = 0, tex_h = 0;

                getTextureDimensions(lines, tex_w, tex_h, height_char_pixel);

                float scale_factor = font_instances_collection[height_char_pixel].scale_factor;

                std::map<char32_t, RasterizedGlyph>& glyphs = font_instances_collection[height_char_pixel].glyphs;

                unsigned char* work_buffer = new unsigned char[tex_h * tex_w];

                for (int i = 0; i < tex_w * tex_h; i++) {

                    work_buffer[i] = 0;

                }

                int baseline = (int)(ascent * scale_factor);



                for (hprl_string text : lines) {

                    int ch = 0;
                    //int xoff, yoff = 0;
                    float xpos = 0, ypos = 0; // leave a little padding in case the character extends left


                    while (to_uint(text[ch])) {
                        unsigned int c = to_uint(text[ch]);
                        //è inefficiente controllare ogni volta però prende soltanto O(log n) essendo un albero rosso e nero
                        if (glyphs.find(c) == glyphs.end()) {

                            // not found
                            int x0, x1, y0, y1;
                            stbtt_GetCodepointBitmapBox(&font_info, c, scale_factor, scale_factor, &x0, &y0, &x1, &y1);

                            int x_length = x1 - x0;
                            int y_length = y1 - y0;
                            int advance = 0, lsb = 0;

                            unsigned char* temp_bitmap = new unsigned char[x_length * y_length];
                            stbtt_MakeCodepointBitmap(&font_info, temp_bitmap, x_length, y_length, x_length, scale_factor, scale_factor, c);
                            stbtt_GetCodepointHMetrics(&font_info, c, &advance, &lsb);
                            RasterizedGlyph new_glyph = { temp_bitmap, x0, y0, x1, y1, advance, lsb };

                            glyphs[c] = new_glyph;

                        }


                        int advance = glyphs[c].advance;
                        int lsb = glyphs[c].lsb;
                        int x0 = glyphs[c].x0, y0 = font_instances_collection[height_char_pixel].glyphs[c].y0, x1 = glyphs[c].x1, y1 = glyphs[c].y1;
                        unsigned char* temp_buffer = glyphs[c].bitmap;



                        for (int j = y0; j < y1; j++) {
                            for (int i = x0; i < x1; i++) {
                                if (work_buffer[(baseline + j) * tex_w + ((int)xpos + i)] != 0 && temp_buffer[(j - y0) * (x1 - x0) + (i - x0)] == 0) {

                                }
                                else {
                                    work_buffer[(baseline + j) * tex_w + ((int)xpos + i)] = temp_buffer[(j - y0) * (x1 - x0) + (i - x0)];
                                }
                            }
                        }

                        xpos += (advance * scale_factor);
                        if (text[ch + 1])
                            xpos += scale_factor * stbtt_GetCodepointKernAdvance(&font_info, c, text[ch + 1]);
                        ++ch;
                    }


                    baseline += (ascent - descent + line_gap) * scale_factor;

                }


                /*for (int j = 0; j < tex_h; ++j) {
                    for (int i = 0; i < tex_w; ++i)
                        putchar(" .:ioVM@"[work_buffer[j * tex_w + i] >> 5]);
                        putchar('\n');
                 }*/

                

                return convertSingle8_into_RGBA(work_buffer, tex_w, tex_h, color);

                //stbi_write_png("aruanna.png", tex_w, tex_h, 1, work_buffer, tex_w);


            }

            void printSingleCharConsole(char32_t codepoint, float height_char_pixel) {


                std::map<char32_t, RasterizedGlyph>& glyphs = font_instances_collection[height_char_pixel].glyphs;

                int x_length = glyphs[codepoint].x1 - glyphs[codepoint].x0;
                int y_length = glyphs[codepoint].y1 - glyphs[codepoint].y0;

                for (int k = 0; k < y_length; ++k) {
                    for (int f = 0; f < x_length; ++f)
                        putchar(" .:ioVM@"[glyphs[codepoint].bitmap[k * x_length + f] >> 5]);
                    putchar('\n');
                }

                std::cout << "\n\n\n";
            }

            void giveFaceInformation() {
                std::cout << font_path << std::endl;
                std::cout << "Ascent: " << ascent << std::endl;
                std::cout << "Descent: " << descent << std::endl;
                std::cout << "Line gap: " << line_gap << std::endl;
                std::cout << "Font Instances size: " << font_instances_collection.size() << std::endl;
            }

            HprlBakedFont bakeFont(float height_char_pixel, char32_t first_char, char32_t char_count, int padding) {


                float scale_factor = stbtt_ScaleForPixelHeight(&font_info, height_char_pixel);

                int atlas_width = 0, atlas_height = 0;
                calculateAtlasDimensions(atlas_width, atlas_height, first_char, char_count, scale_factor);

                unsigned char* atlas_texture = new unsigned char[atlas_width * atlas_height];

                stbtt_pack_context p_context;
                stbtt_packedchar* char_data = new stbtt_packedchar[char_count];


                ////proporzionale all'altezza del font 
                //int padding = int(height_char_pixel / 10) + 1;


                if (!stbtt_PackBegin(&p_context, atlas_texture, atlas_width, atlas_height, 0, padding, nullptr))
                    throw std::runtime_error("Failed to initialize packing context");

                //stbtt_PackSetOversampling(&p_context, 1, 1);


                if (!stbtt_PackFontRange(&p_context, font_data, 0, height_char_pixel, first_char, char_count, char_data))
                    throw std::runtime_error("Failed to pack font");

                stbtt_PackEnd(&p_context);
                
                HprlBakedFont baked_font = HprlBakedFont(p_context);

                for (int i = 0; i < char_count; i++) {

                    baked_font.char_data.push_back(HprlPackedchar(char_data[i]));

                }

                return baked_font;


            }


        private:

            const char* font_path;
            unsigned char* font_data;
            stbtt_fontinfo font_info;
            char32_t first_char_ini;
            char32_t char_count_ini;

            int ascent;
            int descent;
            int line_gap;
            FontBoundingBox font_bbox;

            std::map<float, FontInstance> font_instances_collection;
            


            void initializeFontFace(char32_t first_char, char32_t char_count, std::vector<float>& heights_char_pixel) {


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

            //da rivedere l'aggiunta di un padding proporzionale
            void getTextureDimensions(std::vector<hprl_string>& lines, int& tex_w, int& tex_h, float height_char_pixel) {

                
                tex_h = font_instances_collection[height_char_pixel].max_h_bbox_font * lines.size();

                int max_line_length = 0;

                float scale_factor = font_instances_collection[height_char_pixel].scale_factor;

                for (hprl_string line : lines) {

                    int ch = 0;
                    float xpos = 0, ypos = 0;


                    while (line[ch]) {

                        int advance, lsb;
                        stbtt_GetCodepointHMetrics(&font_info, line[ch], &advance, &lsb);

                        xpos += (advance * scale_factor);

                        if (line[ch + 1])
                            xpos += scale_factor * stbtt_GetCodepointKernAdvance(&font_info, line[ch], line[ch + 1]);
                        ++ch;
                    }

                    int line_length = ceil(xpos);


                    if (line_length > max_line_length) {

                        max_line_length = line_length;

                    };


                }

                tex_w = max_line_length;

            }

            void calculateAtlasDimensions(int& atlas_width, int& atlas_height, char32_t first_char, char32_t char_count, float scale_factor) {

                int ix0, iy0, ix1, iy1;
                ix0 = iy0 = ix1 = iy1 = 0;

                float max_dist_x, max_dist_y;
                max_dist_x = max_dist_y = 0;

                for (int i = 0; i < char_count; i++) {

                    stbtt_GetCodepointBitmapBox(&font_info, first_char + i, scale_factor, scale_factor, &ix0, &iy0, &ix1, &iy1);
                    if (abs(ix1 - ix0) > max_dist_x) { max_dist_x = abs(ix1 - ix0); }
                    if (abs(iy1 - iy0) > max_dist_y) { max_dist_y = abs(iy1 - iy0); }

                }

                float sqrt_max_total_area = sqrt(max_dist_x * max_dist_y * char_count);
                atlas_width = int(sqrt_max_total_area * (max_dist_x / max_dist_y));
                atlas_height = int(sqrt_max_total_area * (max_dist_y / max_dist_x));

            }


        };

        class TextManager {

            public:

                TextManager(std::vector<FontFaceDescription>& ff_descr, char32_t first_char, char32_t char_count, std::vector<float>& heights_char_pixel) {

                    for (FontFaceDescription& f : ff_descr) {

                        font_faces_map.emplace(f.font_face_prop, FontFace(f.font_path, first_char, char_count, heights_char_pixel, f.offset));

                    }


                }

                TextManager() = delete;

                void printFaceInformation(std::string face_id) {

                    if (font_faces_map.contains(face_id)) {
                        font_faces_map.at(face_id).giveFaceInformation();
                    }

                }

                unsigned char* createTextTextureSingleFontFace(std::string face_id, std::vector<hprl_string>& lines, float height_char_pixel, FontColor& color) {

                    if (font_faces_map.contains(face_id)) {

                        return font_faces_map.at(face_id).createTextTextureSingleFontFace(lines, height_char_pixel, color);
                    }

                    else return nullptr;

                }
                


                void parse(std::u8string input) {


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

                    std::size_t h1 = std::hash<std::u8string>{}(input);

                    cache_multi_line.emplace(h1, HprlMultiLineText(width, height, lines));
                   

                }

                void printToConsole(std::u8string in) {

                    std::size_t h1 = std::hash<std::u8string>{}(in);

                    if (cache_multi_line.contains(h1)) {
                        cache_multi_line.at(h1).printToConsoleText();
                    }
                    else {

                        std::cout << "No such string in map" << std::endl;
                    }

                }



            private:


                std::unordered_map<std::string, FontFace> font_faces_map;
                std::unordered_map<std::size_t, HprlMultiLineText> cache_multi_line;

        };



    }
}