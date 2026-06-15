#include <nuklear.h>
#include "demo-widget.hpp"

void DemoWidget::Iterate(struct nk_context *const ctx)
{
    if (nk_begin(ctx, "Demo", nk_rect(50, 50, 230, 250),
                 NK_WINDOW_BORDER | NK_WINDOW_MOVABLE | NK_WINDOW_SCALABLE |
                     NK_WINDOW_MINIMIZABLE | NK_WINDOW_TITLE))
    {
        // Show the name of the slider
        nk_layout_row_dynamic(ctx, 32, 1); // 32 pixels tall, one item in the row
        nk_label(ctx, "Slider", NK_TEXT_ALIGN_LEFT | NK_TEXT_ALIGN_BOTTOM);

        // Create the slider

        int min = 0;
        int max = 20;
        int step = 1;
        nk_layout_row_dynamic(ctx, 30, 1);
        nk_slider_int(ctx, min, &_sliderValue, max, step);
    }
    nk_end(ctx);
}