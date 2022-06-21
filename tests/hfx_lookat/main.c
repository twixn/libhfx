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

int main(void)
{

    /* Initialize peripherals */
    display_init( res, bit, 2, GAMMA_NONE, ANTIALIAS_RESAMPLE );

    state = hfx_init();

    hfx_enable(state, HFX_COLOR_ARRAY);
    hfx_enable(state, HFX_DEPTH_TEST);

    hfx_vertex_pointer(state, 3, HFX_FLOAT, 3*sizeof(float), cube_verts);
    hfx_color_pointer(state, 4, HFX_UNSIGNED_BYTE, 4*sizeof(char), cube_colors);  
    hfx_clear_color_f(state, 0.0f, 0.0f, 0.0f, 1.0f);

    float angle = 10.0f;

    while(1)
    {
        angle += 1.0f;
        // Queue the next frame up
        hfx_clear(state, HFX_COLOR_BUFFER_BIT|HFX_DEPTH_BUFFER_BIT);
        hfx_load_identity(state);
        hfx_persp_f(state, 120.0f, 320.0f/240.0f, 0.1f, 10.0f);
        hfx_lookat_f(state,    2,sin(angle/100)*1 + 1,2,
                                0,0,0,
                                0,1,0);

        hfx_rotate_f(state, angle, 0.0f, 1.0f, 0.0f);

        hfx_draw_arrays(state, HFX_TRIANGLES, 0, 12);

        hfx_swap_buffers(state);
    }
}
