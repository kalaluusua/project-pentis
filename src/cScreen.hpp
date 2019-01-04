#ifndef cSCREENS_INCLUDED
#define cSCREENS_INCLUDED

// Abstract class for screens
class cScreen {
    public :
    virtual int Run (sf::RenderWindow &App) = 0;
    virtual ~cScreen() = default;
};

#endif
