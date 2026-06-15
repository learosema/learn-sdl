#include <nuklear.h>
#include "demo-widget.hpp"

void DemoWidget::Iterate(struct nk_context * const ctx) 
{
    if (nk_begin(ctx, "Demo", nk_rect(50, 50, 230, 250),
    NK_WINDOW_BORDER | NK_WINDOW_MOVABLE | NK_WINDOW_SCALABLE |
    NK_WINDOW_MINIMIZABLE | NK_WINDOW_TITLE))
    {
        // ... widgets go here ...
    }
    nk_end(ctx);    
}