#include "../../platform/graphics.h"
#include "../../platform/core.h"
#include "RailClimber.h"
#include <FMedia.h>
#include <FBase.h>
#include <list>

using namespace std;
using namespace Tizen::Base;

namespace RailClimberPlatform {

unsigned int loadTexture(const char *filename, int *retW, int *retH, bool hRepeat, bool vRepeat) {
	GLuint texture;
	int w, h;

	char dataPath[1024];
	const wchar_t *wData = g_app->GetAppDataPath().GetPointer();
	wcstombs(dataPath, wData, 1024);
	string s = string(dataPath) + "/";
	s += filename;

	Tizen::Media::Image *img = new Tizen::Media::Image();
	img->Construct();
	Tizen::Graphics::Bitmap *bmp = new Tizen::Graphics::Bitmap();
	ByteBuffer *bb = NULL;
	/*if (sourceFormat == GL_RGB)
		bb = img->DecodeToBufferN(s.c_str(), Tizen::Graphics::BITMAP_PIXEL_FORMAT_R8G8B8A8, w, h);
	else //assuming GL_RGBA*/
	bb = img->DecodeToBufferN(s.c_str(), Tizen::Graphics::BITMAP_PIXEL_FORMAT_R8G8B8A8, w, h);
	Tizen::Graphics::Dimension dim(w, h);
	bmp->Construct(*bb, dim, Tizen::Graphics::BITMAP_PIXEL_FORMAT_R8G8B8A8, Tizen::Graphics::BUFFER_SCALING_NONE);
	delete img;
	delete bb;

	Tizen::Graphics::BufferInfo bufInfo;
	bmp->Lock(bufInfo);
	w = bufInfo.width;
	h = bufInfo.height;
	*retW = w;
	*retH = h;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, hRepeat ? GL_REPEAT : GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, vRepeat ? GL_REPEAT : GL_CLAMP_TO_EDGE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, bufInfo.pPixels);
	bmp->Unlock();
	delete bmp;

	return texture;
}


RCLFontType loadFont(const char *filename, int ptsize) {
    RCLFontType font = new Tizen::Graphics::Font();

    char dataPath[1024];
    const wchar_t *wData = g_app->GetAppDataPath().GetPointer();
    wcstombs(dataPath, wData, 1024);
    string str = string(dataPath) + "/" + filename;

    font->Construct(str.c_str(), Tizen::Graphics::FONT_STYLE_PLAIN, ptsize);
    return font;
}

void unloadFont(RCLFontType font) {
    delete font;
}

unsigned int textureFromText(RCLFontType font, int color, const char *text, int *retW, int *retH) {
    GLuint texture;

    Tizen::Graphics::Dimension dim;
    int add = 0;
    int i = 0;
    while (text[i] != 0) {
    	if (text[i] > 0x7F)
    		add++;
    	i++;
    }
    font->GetTextExtent(text, i - add/2, dim);

    if (dim.width == 0)
    	dim.width = 1;

    dim.height = font->GetMaxHeight();

    *retW = dim.width;
    *retH = dim.height;

    Tizen::Graphics::Canvas c;
    c.Construct(Tizen::Graphics::Rectangle(0, 0, dim.width, dim.height));
    c.SetFont(*font);
	//swap red and blue
	color = (color & 0xFF00FF00) | ((color & 0xFF0000) >> 16) | ((color & 0xFF) << 16);
    c.SetForegroundColor(Tizen::Graphics::Color(color, false));
    c.DrawText(Tizen::Graphics::Point(0, 0), text);

    Tizen::Graphics::Bitmap bmp;
    bmp.Construct(c, Tizen::Graphics::Rectangle(0, 0, dim.width, dim.height));

    Tizen::Graphics::BufferInfo info;
    bmp.Lock(info);

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, info.width, info.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, info.pPixels);

    bmp.Unlock();
    return texture;
}

unsigned int multiLineText(RCLFontType font, int color, const char *text, int w, int *retH, MultiLineJustify justify) {
	list<pair<string, char> > words;
	int mfh = font->GetMaxHeight();

	if (w == 0)
		w = 1;

	int len = strlen(text);
	char word[128];
	char lastchar = 0;
	int pos = 0;
	for (int i=0; i<=len; i++) {
		if (text[i] == '\n' && lastchar == '\r')
			continue;
		word[pos] = text[i];
		if (text[i] == ' ' || text[i] == '\n' || text[i] == '\r' || text[i] == 0) {
			word[pos] = 0;
			if (pos == 0) {
				word[0] = ' ';
				word[1] = 0;
			}
			words.push_back(make_pair(word, text[i]));
			pos = 0;
		}else pos++;
		lastchar = text[i];
	}

	int tw, th, spacew;
	Tizen::Graphics::Dimension dim;
	font->GetTextExtent(" ", 1, dim);
	spacew = dim.width;
	th = mfh;

	int curX = 0, lines = 1;
	for (list<pair<string, char> >::iterator it = words.begin(); it != words.end(); ++it) {
		string &s = it->first;
		char &sep = it->second;
		int add = 0;
		unsigned int i = 0;
		while (i < s.length()) {
		    if (s[i] > 0x7F)
		    	add++;
		    i++;
		}
		font->GetTextExtent(s.c_str(), i-add/2, dim);
		tw = dim.width;
		th = mfh;
		if (curX + tw + spacew > w) {
			curX = 0;
			lines++;
		}
		curX += tw + spacew;
		if (sep != ' ' && sep != 0) {
			curX = 0;
			lines++;
		}
	}

	curX = 0;
	int curY = 0;

	Tizen::Graphics::Canvas c;

	c.Construct(Tizen::Graphics::Rectangle(0, 0, w, lines*mfh ));

	c.SetFont(*font);
	//swap red and blue
	color = (color & 0xFF00FF00) | ((color & 0xFF0000) >> 16) | ((color & 0xFF) << 16);
	c.SetForegroundColor(Tizen::Graphics::Color(color, false));
	for (list<pair<string, char> >::iterator it = words.begin(); it != words.end(); ++it) {
		string &s = it->first;
		char &sep = it->second;
		int add = 0;
		unsigned int i = 0;
		while (i < s.length()) {
			if (s[i] > 0x7F)
				add++;
			i++;
		}
		font->GetTextExtent(s.c_str(), i-add/2, dim);
		dim.height = mfh;
		if (curX + dim.width + spacew > w) {
			curX = 0;
			curY += dim.height;
		}
		c.DrawText(Tizen::Graphics::Point(curX, curY), s.c_str());
		curX += dim.width + spacew;
		if (sep != ' ' && sep != 0) {
			curX = 0;
			curY += dim.height;
		}
	}
	Tizen::Graphics::Bitmap bmp;

	bmp.Construct(c, Tizen::Graphics::Rectangle(0, 0, w, lines*mfh));

    Tizen::Graphics::BufferInfo info;
    bmp.Lock(info);


    GLuint texture;
    if (retH != NULL)
    	*retH = info.height;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, info.width, info.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, info.pPixels);

    bmp.Unlock();

    return texture;
}

void textMetrics(RCLFontType font, const char *text, int *retW, int *retH) {

}

}
