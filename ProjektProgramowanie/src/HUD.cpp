#include "HUD.h"

#include "object_init.h"
#include <sstream>
HUD::HUD()
{
    //ctor
}

HUD::~HUD()
{
    //dtor
}

void HUD::fps_render(Uint64 &rend_frame)
{
    double fps = 1000.0 / static_cast<double>(rend_frame);
    std::stringstream fps_t;
    fps_t.str( "" );
    fps_t << "FPS:" << (int)fps;

    fps_text.text_load(fps_t.str().c_str(), t1.font_default_get(), t1.font_default_color_get());
}
