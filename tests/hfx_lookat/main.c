#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <stdint.h>
#include <math.h>
#include <hfx.h>
#include <hfx_rb.h>
#include <hfx_cmds.h>
#include <hfx_int.h>
#include <hfx_types.h>
#include <libdragon.h>

static resolution_t res = RESOLUTION_320x240;
static bitdepth_t bit = DEPTH_16_BPP;

static hfx_state *state;


float cube_verts[] = 
{
    // Top
    -1.0f, 1.0f, -1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f, 1.0f, -1.0f,

    -1.0f, 1.0f, -1.0f,
    -1.0f, 1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,

    // Front
    -1.0f, -1.0f, -1.0f,
    1.0f, 1.0f, -1.0f,
    -1.0f, 1.0f, -1.0f,

    -1.0f, -1.0f, -1.0f,
    1.0f, -1.0f, -1.0f,
    1.0f, 1.0f, -1.0f,

    // Right
    1.0f, -1.0f, -1.0f,
    1.0f, 1.0f, -1.0f,
    1.0f, 1.0f, 1.0f,

    1.0f, -1.0f, -1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f, -1.0f, 1.0f,

    // Left
    -1.0f, -1.0f, -1.0f,
    -1.0f, 1.0f, -1.0f,
    -1.0f, -1.0f, 1.0f,

    -1.0f, -1.0f, 1.0f,
    -1.0f, 1.0f, -1.0f,
    -1.0f, 1.0f, 1.0f,

    // Back
    -1.0f, -1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    -1.0f, 1.0f, 1.0f,

    -1.0f, -1.0f, 1.0f,
    1.0f, -1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,

    // Bottom
    -1.0f, -1.0f, -1.0f,
    1.0f, -1.0f, 1.0f,
    1.0f, -1.0f, -1.0f,

    -1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f, 1.0f,
    1.0f, -1.0f, 1.0f,

};


uint8_t cube_colors[] =
{
    // Top
    255, 0, 0, 255,
    255, 0, 0, 255,
    255, 0, 0, 255,

    255, 0, 0, 255,
    255, 0, 0, 255,
    255, 0, 0, 255,

    // Front
    0, 255, 0, 255,
    0, 255, 0, 255,
    0, 255, 0, 255,

    0, 255, 0, 255,
    0, 255, 0, 255,
    0, 255, 0, 255,

    // Right
    0, 0, 255, 255,
    0, 0, 255, 255,
    0, 0, 255, 255,

    0, 0, 255, 255,
    0, 0, 255, 255,
    0, 0, 255, 255,

    // Left
    0, 0, 255, 255,
    0, 0, 255, 255,
    0, 0, 255, 255,

    0, 0, 255, 255,
    0, 0, 255, 255,
    0, 0, 255, 255,

    // Back
    255, 100, 0, 255,
    255, 100, 0, 255,
    255, 100, 0, 255,

    255, 100, 0, 255,
    255, 100, 0, 255,
    255, 100, 0, 255,

    // Bottom
    0, 255, 255, 255,
    0, 255, 255, 255,
    0, 255, 255, 255,

    0, 255, 255, 255,
    0, 255, 255, 255,
    0, 255, 255, 255,
};


void ang2f4(float ang_x, float ang_y, float* result)
{
    result[0] = cosf(ang_x)*cosf(ang_y);
    result[1] = sinf(ang_x);
    result[2] = cosf(ang_x)*sinf(ang_y);
    result[3] = 1;
};

int main(void)
{

    /* Initialize peripherals */
    display_init( res, bit, 2, GAMMA_NONE, ANTIALIAS_RESAMPLE );
    controller_init();

    state = hfx_init();

    hfx_enable(state, HFX_COLOR_ARRAY);
    hfx_enable(state, HFX_DEPTH_TEST);

    hfx_vertex_pointer(state, 3, HFX_FLOAT, 3*sizeof(float), cube_verts);
    hfx_color_pointer(state, 4, HFX_UNSIGNED_BYTE, 4*sizeof(char), cube_colors);  
    hfx_clear_color_f(state, 0.0f, 0.0f, 0.0f, 1.0f);

    float angle = 10.0f;

    float h_pi = 3.14f / 2.0f;
    float camera_ang_x = -h_pi/2;
    float camera_ang_y = 0;
    float camera_pos[4] = {-2,2,0,0};
    float camera_dir[4] = {0,0,0,0};

    float cube2_verts[108];
    for(int i=0; i<108; ++i)
        cube2_verts[i] = cube_verts[i] * 4;

    while(1)
    {
        controller_scan();

        struct controller_data keys_pressed = get_keys_pressed();

        camera_ang_x += (float)(keys_pressed.c[0].y / 255.0f) * 0.1f;
        camera_ang_y += (float)(keys_pressed.c[0].x / 255.0f) * 0.1f;
        if(camera_ang_x < -h_pi) camera_ang_x = -h_pi;
        if(camera_ang_x > h_pi) camera_ang_x = h_pi;
        ang2f4(camera_ang_x, camera_ang_y, camera_dir);


        if(keys_pressed.c[0].A)
        {
            camera_pos[0] += 0.1f * camera_dir[0];
            camera_pos[1] += 0.1f * camera_dir[1];
            camera_pos[2] += 0.1f * camera_dir[2];
        }
        if(keys_pressed.c[0].B)
        {
            camera_pos[0] -= 0.1f * camera_dir[0];
            camera_pos[1] -= 0.1f * camera_dir[1];
            camera_pos[2] -= 0.1f * camera_dir[2];
        }

        angle += 1.0f;
        // Queue the next frame up
        hfx_clear(state, HFX_COLOR_BUFFER_BIT|HFX_DEPTH_BUFFER_BIT);
        hfx_load_identity(state);
        hfx_persp_f(state, 120.0f, 320.0f/240.0f, 0.1f, 100.0f);
        hfx_lookat_f(state,    camera_pos[0],camera_pos[1],camera_pos[2],
                                camera_pos[0] + camera_dir[0],camera_pos[1] + camera_dir[1],camera_pos[2] + camera_dir[2],
                                0,1,0);

        //hfx_rotate_f(state, angle, 0.0f, 1.0f, 0.0f);
        hfx_vertex_pointer(state, 3, HFX_FLOAT, 3*sizeof(float), cube_verts);
        hfx_draw_arrays(state, HFX_TRIANGLES, 0, 12);

       // hfx_scale_f(state, 10.0f, 4.0f, 10.0f);
        hfx_vertex_pointer(state, 3, HFX_FLOAT, 3*sizeof(float), cube2_verts);
        hfx_draw_arrays(state, HFX_TRIANGLES, 0, 12);

        hfx_swap_buffers(state);
    }
}
