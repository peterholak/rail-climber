#include "platform/graphics.h"
#include <SDL_ttf.h>
#include <SDL_video.h>
#include <SDL_image.h>
#include <string>
#include <list>
#include <vector>
#include <cinttypes>
#include "Logging.h"

using namespace std;

namespace RailClimberPlatform {

    unsigned int loadTexture(const string &filename, int *retW, int *retH, bool hRepeat, bool vRepeat) {
        GLuint texture;
        int w, h;

        SDL_Surface *surface = IMG_Load((dataDir + filename).c_str());

        if (!surface) {
            Log::error("Failed to load texture from '%s'", filename.c_str());
            return 0;
        }

        if (!surface->format->Amask) {
            Log::debug("Image '%s' doesn't have an alpha channel, setting opaque", filename.c_str());
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

    RCLFontType loadFont(const string &filename, int ptsize) {
        return TTF_OpenFont((dataDir + filename).c_str(), ptsize);
    }

    void unloadFont(RCLFontType font) {
        TTF_CloseFont(font);
    }

    void textMetrics(RCLFontType font, const string &text, int *retW, int *retH) {
        int tmpW, tmpH;
        if (retW == nullptr) {
            retW = &tmpW;
        }
        if (retH == nullptr) {
            retH = &tmpH;
        }

        TTF_SizeUTF8(font, text.c_str(), retW, retH);
    }

    unsigned int textureFromText(RCLFontType font, uint32_t colorRgba, const string &text, int *retW, int *retH) {
        GLuint texture;

        SDL_Color clr;
        clr.r = uint8_t((colorRgba & 0xFF000000) >> 24);
        clr.g = uint8_t((colorRgba & 0x00FF0000) >> 16);
        clr.b = uint8_t((colorRgba & 0x0000FF00) >> 8);
        clr.a = uint8_t(colorRgba & 0x000000FF);
        SDL_Surface *surface = TTF_RenderUTF8_Blended(font, text.c_str(), clr);

        if (clr.a != 0xFF && surface->format->BytesPerPixel == 4) {
            float alphaMultiplier = clr.a / 255.0f;
            for (int i=0; i<surface->w*surface->h; i++) {
                uint32_t *pixelPointer = ((uint32_t*)(surface->pixels)) + i;
                uint8_t originalAlpha = uint8_t((*pixelPointer & surface->format->Amask) >> surface->format->Ashift);
                uint8_t newAlpha = uint8_t(originalAlpha * alphaMultiplier);
                (*pixelPointer) = (*pixelPointer & ~surface->format->Amask) | (newAlpha << surface->format->Ashift);
            }
        }

        GLenum textureFormat = surface->format->Rmask == 0xFF ? GL_RGBA : GL_BGRA;

        *retW = surface->w;
        *retH = surface->h;

        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, surface->w, surface->h, 0, textureFormat, GL_UNSIGNED_BYTE, surface->pixels);

        SDL_FreeSurface(surface);
        return texture;
    }

    unsigned int multiLineText(RCLFontType font, uint32_t colorRgba, const string &text, int w, int *retH, MultiLineJustify justify) {
        //first, break the text into words
        list<pair<string, char> > words;
        vector<int> lineWidth;

        int len = text.length();
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
        clr.r = uint8_t((colorRgba & 0xFF000000) >> 24);
        clr.g = uint8_t((colorRgba & 0x00FF0000) >> 16);
        clr.b = uint8_t((colorRgba & 0x0000FF00) >> 8);
        clr.a = uint8_t(colorRgba & 0x000000FF);
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
