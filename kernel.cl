#include "vecs.h"

kernel void voronoi(global Vec3f *pixels, global const Vec3f *points, 
                    const int n_points, const Vec3d mouse_pos)
{
    size_t I = get_global_id(0);
    size_t x_ = I % WIDTH;
    size_t y_ = (I - x_) / WIDTH;
    float x = (float)x_;
    float y = (float)y_;

    float m_dist = (x - points[0].x) * (x - points[0].x) + 
                   (y - points[0].y) * (y - points[0].y);
    int m_i = 0;
    for (int i = 1; i < n_points; ++i)
    {
        float dx = x - points[i].x;
        float dy = y - points[i].y;
        float d2 = dx * dx + dy * dy;
        if (d2 <= m_dist)
        {
            m_dist = d2;
            m_i = i;
        }
    }

    float dx = x - mouse_pos.x;
    float dy = y - mouse_pos.y;
    float d2 = dx * dx + dy * dy;
    if (d2 < m_dist)
        pixels[I] = Vec3fFrom(interp(x, 0, WIDTH, 0, 1),
                              interp(x - y, -HEIGHT, WIDTH + HEIGHT, 0, 1),
                              interp(y, 0, HEIGHT, 0, 1));
    else
        pixels[I] = Vec3fFrom(interp(points[m_i].x, 0, WIDTH, 0, 1), 
                              interp(points[m_i].x + points[m_i].y, 0, HEIGHT + WIDTH, 0, 1), 
                              interp(points[m_i].x * points[m_i].y, 0, HEIGHT * WIDTH, 0, 1));
    if (d2 <= 15)
            pixels[I] = Vec3fFromScalar(0.0);


    for (int i = 0; i < n_points; ++i)
    {
        float dx = x - points[i].x;
        float dy = y - points[i].y;
        float d2 = dx * dx + dy * dy;
        if (d2 <= 15)
            pixels[I] = Vec3fFromScalar(0.0);
    }
}