#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <stdint.h>
#include <hfx.h>
#include <hfx_rb.h>
#include <hfx_cmds.h>
#include <hfx_int.h>
#include <hfx_types.h>
#include <libdragon.h>

static resolution_t res = RESOLUTION_320x240;
static bitdepth_t bit = DEPTH_16_BPP;

static int done = 1;
static hfx_state *state;

static char pbuf[256];
float v1[] =
{
    0.0f, 0.0f, 0.0f,
    100.0f, 0.0f, 0.0f,
    100.0f / 2.0f, 100.0f, 0.0f,
};

uint8_t vc1[] =
{
    255.0f, 0.0f, 0.0f, 255.0f,
    0.0f, 255.0f, 0.0f, 255.0f,
    0.0f, 0.0f, 255.0f, 255.0f,
};

int main(void)
{
    /* enable interrupts (on the CPU) */
    init_interrupts();

    /* Initialize peripherals */
    display_init( res, bit, 2, GAMMA_NONE, ANTIALIAS_RESAMPLE );

    state = hfx_init();

    hfx_vertex_pointer(state, 3, HFX_FLOAT, 0, v1);
    hfx_color_pointer(state, 4, HFX_UNSIGNED_BYTE, 0, vc1);    

    hfx_load_identity(state);
    //hfx_rotate_f(state, 50, 0, 0, 1);
    hfx_translate_f(state, 50.0f, 50.0f, 0.0f);

    hfx_draw_arrays(state, HFX_TRIANGLES, 0, 3);

    hfx_clear_color_f(state, 0.0f, 0.0f, 0.0f, 1.0f);

    float angle = 0.0;

    while(1)
    {
        if(angle == 360)
            angle = 0;
        else
            angle += 1.0f;
        done = 1;

        // This angle causes graphical glitch
        //angle = 240.0f;

        // Queue the next frame up
        hfx_clear(state, HFX_COLOR_BUFFER_BIT|HFX_DEPTH_BUFFER_BIT);
        hfx_load_identity(state);
        hfx_translate_f(state, 100.0f, 100.0f, 0.0f);
        hfx_rotate_f(state, angle, 0, 0, 1);

        hfx_draw_arrays(state, HFX_TRIANGLES, 0, 3);

        hfx_wait_for_idle(state);

        sprintf(pbuf, "Done %f", angle);
        graphics_draw_text(state->display, 0, 100, pbuf);

        sprintf(pbuf, "Start 0x%08lx, end 0x%08lx", hfx_read_reg(HFX_VADDR_REG_RB_START), state->rb_end);
        graphics_draw_text(state->display, 0, 130, pbuf);

        uint32_t rdp_start = hfx_read_reg(0x04100000);
        uint32_t rdp_end = hfx_read_reg(0x04100004);
        uint32_t rdp_current = hfx_read_reg(0x04100008);

        sprintf(pbuf, "Start 0x%lx, end 0x%lx, current 0x%lx", rdp_start, rdp_end, rdp_current);
        graphics_draw_text(state->display, 0, 160, pbuf);

        hfx_swap_buffers(state);
    }
}