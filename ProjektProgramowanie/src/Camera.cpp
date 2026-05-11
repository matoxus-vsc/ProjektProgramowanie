#include "Camera.h"

Camera::Camera()
    : view_x(0.0f), view_y(0.0f)
{
}

void Camera::update(float target_x, float target_y, int window_w, int window_h, int map_w, int map_h)
{
    view_x = target_x - (window_w / 2.0f);
    view_y = target_y - (window_h / 2.0f);

    if (view_x < 0) view_x = 0;
    if (view_y < 0) view_y = 0;
    if (view_x > map_w - window_w) view_x = map_w - window_w;
    if (view_y > map_h - window_h) view_y = map_h - window_h;
}

float Camera::view_x_get() const
{
    return view_x;
}

float Camera::view_y_get() const
{
    return view_y;
}
