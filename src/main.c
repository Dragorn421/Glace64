// SPDX-License-Identifier: CC0-1.0

#include <stdio.h>
#include <stdint.h>

#include <libdragon.h>

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/gl_integration.h>

#include "include/glprimitives.h"

void callback(float radius, int n_segments, int iy, int itheta)
{
    glColor3f((float)iy / n_segments, (float)itheta / n_segments, 1.0f);
}

int main()
{
    debug_init(DEBUG_FEATURE_ALL);
    debugf("hello world!\n");
    controller_init();
    rdpq_init();
    gl_init();
    display_init(RESOLUTION_320x240, DEPTH_16_BPP, 3, GAMMA_CORRECT, ANTIALIAS_RESAMPLE_FETCH_NEEDED);
    surface_t zbuffer = surface_alloc(FMT_RGBA16, display_get_width(), display_get_height());
    float radius = 2.0f;
    int n_segments = 5;
    float rot_pitch = 0.0f;
    float rot_yaw = 0.0f;
    uint32_t ticks_last = TICKS_READ();
    while (true)
    {
        surface_t *disp = display_get();
        rdpq_attach(disp, &zbuffer);
        gl_context_begin();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluPerspective(80.0f, 4.0f / 3.0f, 0.1f, 10.0f);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        gluLookAt(0, 0, 5,
                  0, 0, 0,
                  0, 1, 0);
        glRotatef(rot_pitch, 1, 0, 0);
        glRotatef(rot_yaw, 0, 1, 0);
        glprim_sphere(radius, n_segments, callback);
        gl_context_end();
        rdpq_detach_show();
        controller_scan();
        struct controller_data down = get_keys_down();
        struct controller_data pressed = get_keys_pressed();
        uint32_t ticks_now = TICKS_READ();
        uint32_t ticks_elapsed = TICKS_DISTANCE(ticks_last, ticks_now);
        ticks_last = ticks_now;
        float seconds_elapsed = (float)ticks_elapsed / TICKS_PER_SECOND;
        float x = pressed.c[0].x / 127.0f / 0.7f;
        float y = pressed.c[0].y / 127.0f / 0.7f;
        float rotspeed_yaw = 360; // degrees per second
        float rotspeed_pitch = 360;
        debugf("%f %f\n", x, y);
        rot_yaw += x * seconds_elapsed * rotspeed_yaw;
        rot_pitch += y * seconds_elapsed * rotspeed_pitch;
        if (down.c[0].C_up)
            n_segments++;
        if (down.c[0].C_down)
            n_segments--;
    }
}
