#include "emscripten_graphics.h"
#include <SDL_video.h>
#include <SDL_opengles2.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <list>
#include <vector>
#include <string>
#include "Logging.h"

using namespace std;

namespace RailClimberPlatform {

    /**
     * Create a 32-bit RGBA texture from a png file
     * @param retW, retH where the size of the image in pixels will be stored, or NULL
     * @param hRepeat, vRepeat whether the texture repeats horizontally/vertically (sets clamp mode)
     * @return OpenGL texture id
     */
    unsigned int loadTexture(const char *filename, int *retW, int *retH, bool hRepeat, bool vRepeat) {
        string dataDir = "../../data/";

        GLuint texture;
        int w, h;

        SDL_Surface *surface = IMG_Load((dataDir + filename).c_str());

        if (!surface->format->Amask) {
            Log::debug("Image '%s' doesn't have an alpha channel, setting opaque", filename);
            SDL_Surface *rgba = SDL_CreateRGBSurface(SDL_SWSURFACE, surface->w, surface->h, 32, 0xFF, 0xFF00, 0xFF0000, 0xFF000000);
            SDL_BlitSurface(surface, &surface->clip_rect, rgba, &rgba->clip_rect);
            SDL_FreeSurface(surface);
            surface = rgba;
        }

        w = surface->w;
        h = surface->h;
        *retW = w;
        *retH = h;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, hRepeat ? GL_REPEAT : GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, vRepeat ? GL_REPEAT : GL_CLAMP_TO_EDGE);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, surface->pixels);

        SDL_FreeSurface(surface);
        return texture;
    }

    enum MultiLineJustify {
        Left, Center
    };

    RCLFontType loadFont(const char *filename, int ptsize) {
        string dataDir = "../../data/";
        Log::debug("Loading font from '%s'", (dataDir + filename).c_str());
        RCLFontType font = TTF_OpenFont((dataDir + filename).c_str(), ptsize);
        if (font != nullptr) {
            Log::debug("Success");
        }else{
            Log::error("Failed to load font.");
        }
        return font;
    }

    void unloadFont(RCLFontType font) {
        TTF_CloseFont(font);
    }

    unsigned int textureFromText(RCLFontType font, int color, const char *text, int *retW, int *retH) {
        GLuint texture;

        SDL_Color clr;
        clr.r = color & 0xFF;
        clr.g = (color & 0xFF00) >> 8;
        clr.b = (color & 0xFF0000) >> 16;
        clr.a = 0xFF;
        SDL_Surface *surface = SDL_CreateRGBSurface(SDL_SWSURFACE, 32, 16, 32, 0, 0, 0, 0); //TTF_RenderUTF8_Blended(font, text, clr);

        *retW = surface->w;
        *retH = surface->h;

        Log::debug("Rendered text (%d %d): '%s'", surface->w, surface->h, text);

        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, surface->w, surface->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, surface->pixels);

        SDL_FreeSurface(surface);
        return texture;
    }

    void textMetrics(RCLFontType font, const char *text, int *retW, int *retH) {
        int tmpW, tmpH;
        if (retW == nullptr) {
            retW = &tmpW;
        }
        if (retH == nullptr) {
            retH = &tmpH;
        }
        TTF_SizeUTF8(font, text, retW, retH);
    }

    unsigned int multiLineText(RCLFontType font, int color, const char *text, int w, int *retH, MultiLineJustify justify) {
        //first, break the text into words
        list<pair<string, char> > words;
        vector<int> lineWidth;

        int len = strlen(text);
        char word[128];
        char lastchar = 0;
        int pos = 0;
        for (int i = 0; i <= len; i++) {
            if (text[i] == '\n' && lastchar == '\r') {
                continue;
            }
            word[pos] = text[i];
            if (text[i] == ' ' || text[i] == '\n' || text[i] == '\r' || text[i] == 0) {
                word[pos] = 0;
                if (pos == 0) {
                    word[0] = ' ';
                    word[1] = 0;
                }
                words.push_back(make_pair(word, text[i]));
                pos = 0;
            } else {
                pos++;
            }
            lastchar = text[i];
        }

        //next, calculate the number of lines required
        int tw, th, spacew;
        TTF_SizeUTF8(font, " ", &spacew, &th);
        int curX = 0, lines = 1;
        for (list<pair<string, char> >::iterator it = words.begin(); it != words.end(); ++it) {
            string &s = it->first;
            char &sep = it->second;
            TTF_SizeUTF8(font, s.c_str(), &tw, &th);
            if (curX + tw + spacew > w) {
                lineWidth.push_back(curX);
                curX = 0;
                lines++;
            }
            curX += tw + spacew;
            if (sep != ' ' && sep != 0) {
                lineWidth.push_back(curX);
                curX = 0;
                lines++;
            }
        }
        lineWidth.push_back(curX);

        //last, create the surface and the final texture
        curX = 0;
        int curY = 0;
        SDL_Color clr;
        clr.r = color & 0xFF;
        clr.g = (color & 0xFF00) >> 8;
        clr.b = (color & 0xFF0000) >> 16;
        clr.a = 0xFF;
        SDL_Surface *bmp = SDL_CreateRGBSurface(SDL_SWSURFACE, w, lines * TTF_FontHeight(font), 32, 0xFF, 0xFF00, 0xFF0000, 0xFF000000);
        int line = 0;
        for (list<pair<string, char> >::iterator it = words.begin(); it != words.end(); ++it) {
            string &s = it->first;
            char &sep = it->second;
            SDL_Surface *tmp = TTF_RenderUTF8_Blended(font, s.c_str(), clr);
            SDL_SetSurfaceAlphaMod(tmp, 0);
            if (curX + tmp->w + spacew > w) {
                line++;
                curX = 0;
                curY += TTF_FontHeight(font);
            }
            SDL_Rect dstrect;
            dstrect.x = curX + (justify == Center ? ((w - lineWidth[line]) / 2) : 0);
            dstrect.y = curY;
            dstrect.w = tmp->w;
            dstrect.h = tmp->h;
            SDL_BlitSurface(tmp, &tmp->clip_rect, bmp, &dstrect);
            curX += tmp->w + spacew;
            if (sep != ' ' && sep != 0) {
                line++;
                curX = 0;
                curY += TTF_FontHeight(font);
            }
            SDL_FreeSurface(tmp);
        }

        GLuint texture;
        *retH = bmp->h;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, bmp->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, bmp->pixels);

        SDL_FreeSurface(bmp);
        return texture;
    }

}
