#ifndef FONT_MANAGER_HPP
#define FONT_MANAGER_HPP

#include <string>
#include <map>
#include <utility>

using std::string;
using std::map;
using std::pair;
struct _TTF_Font;
typedef struct _TTF_Font TTF_Font;


class FontManager
{
public:
    static TTF_Font* get_font(string uri, int size);

    static void empty_cache();

private:
    typedef map<pair<string,int>, TTF_Font*>
        FontCollection;
    static FontCollection _fonts;
};

#endif
