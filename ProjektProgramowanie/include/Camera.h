#pragma once

class Camera
{
public:
    Camera();

    void update(float target_x, float target_y, int window_w, int window_h, int map_w, int map_h);

    float view_x_get() const;
    float view_y_get() const;

private:
    float view_x;
    float view_y;
};
   
