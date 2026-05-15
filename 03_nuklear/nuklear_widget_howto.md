# Nuklear Widget How-To

A quick visual reference for common Nuklear widgets. Each schematic shows the anatomy and the parameters you need to supply.

---

### Layout rows

Every widget needs a layout declaration first. This is one of the most common things to forget.

```
nk_layout_row_static(ctx, height, width, cols)
nk_layout_row_dynamic(ctx, height, cols)

Static — each widget gets a fixed pixel width:
┌────────────────────────────────────────────────────┐
│ [  widget  ]  · · · · · · (unused space) · · · · · │
│  ← 80 px →                                         │
└────────────────────────────────────────────────────┘

Dynamic — available width is divided equally:
┌────────────────────────────────────────────────────┐
│ [     widget 1     ] │ [     widget 2     ]         │
│ ←── 50% width ──→   │ ←── 50% width ──→           │
└────────────────────────────────────────────────────┘
```

```c
nk_layout_row_static(ctx, 30, 80, 1);   // 1 widget, 80 px wide, 30 px tall
nk_layout_row_dynamic(ctx, 30, 2);      // 2 widgets, equal width, 30 px tall
```

---

### Button

```
nk_button_label(ctx, label)  →  returns 1 (true) on click, 0 otherwise

┌──────────────┐
│    button    │  ← clicked → if-block runs once
└──────────────┘
```

```c
nk_layout_row_static(ctx, 30, 80, 1);
if (nk_button_label(ctx, "button")) {
    /* runs only on the frame the button is clicked */
}
```

> The `if` block is your click handler — no callbacks, no event IDs.

---

### Option (radio buttons)

```
nk_option_label(ctx, label, active)  →  returns 1 if clicked

State lives in YOUR variable:
  int op = EASY;   ← you own this

(•) easy    ← op == EASY is true  → filled circle
( ) hard    ← op == HARD is false → empty circle

On click: nk_option_label returns 1 → you reassign op
```

```c
enum { EASY, HARD };
static int op = EASY;

nk_layout_row_dynamic(ctx, 30, 2);
if (nk_option_label(ctx, "easy", op == EASY)) op = EASY;
if (nk_option_label(ctx, "hard", op == HARD)) op = HARD;
```

> The second argument is the active state — you compute it with a plain comparison.

---

### Property (int / float)

```
nk_property_int(ctx, label, min, &val, max, step, px_per_step)

┌──────────────┬───┬───────────────────┬───┐
│ Compression: │ − │        20         │ + │
└──────────────┴───┴───────────────────┴───┘
  ↑ label        ↑   ↑ &property          ↑
               min   value ptr           max

User can: click −/+  or  drag the center field
```

```c
static int property = 20;
nk_layout_row_dynamic(ctx, 25, 1);
nk_property_int(ctx, "Compression:", 0, &property, 1000, 1, 1);
```

> Float variants: `nk_property_float(&val)` writes via pointer, `nk_propertyf()` returns the value directly.

---

### Label

```
nk_label(ctx, text, alignment)

NK_TEXT_LEFT      │ Hello           │
NK_TEXT_CENTERED  │     Hello       │
NK_TEXT_RIGHT     │          Hello  │
```

```c
nk_layout_row_dynamic(ctx, 20, 1);
nk_label(ctx, "background:", NK_TEXT_LEFT);

// Colored variant:
nk_label_colored(ctx, "Warning", NK_TEXT_LEFT, nk_rgb(255, 160, 0));
```

---

### Combo box with color picker

```
nk_combo_begin_color(ctx, color, size)  →  returns 1 if open

Collapsed:
┌─────────────────────────────┬──────────┐
│                             │ ▼ color  │
└─────────────────────────────┴──────────┘

Expanded (dropdown):
┌─────────────────────────────┬──────────┐
│                             │ ▲ color  │
├─────────────────────────────┴──────────┤
│  [  color picker square  ] [hue bar]   │  ← nk_color_picker()
│  #R: [────────────── 0.10 ──────────]  │  ← nk_propertyf()
│  #G: [────────────── 0.18 ──────────]  │
│  #B: [────────────── 0.24 ──────────]  │
│  #A: [────────────── 1.00 ──────────]  │
└────────────────────────────────────────┘
         nk_combo_end(ctx)  ← always close
```

```c
if (nk_combo_begin_color(ctx, nk_rgb_cf(app->bg),
        nk_vec2(nk_widget_width(ctx), 400)))
{
    nk_layout_row_dynamic(ctx, 120, 1);
    app->bg = nk_color_picker(ctx, app->bg, NK_RGBA);

    nk_layout_row_dynamic(ctx, 25, 1);
    app->bg.r = nk_propertyf(ctx, "#R:", 0, app->bg.r, 1.0f, 0.01f, 0.005f);
    app->bg.g = nk_propertyf(ctx, "#G:", 0, app->bg.g, 1.0f, 0.01f, 0.005f);
    app->bg.b = nk_propertyf(ctx, "#B:", 0, app->bg.b, 1.0f, 0.01f, 0.005f);
    app->bg.a = nk_propertyf(ctx, "#A:", 0, app->bg.a, 1.0f, 0.01f, 0.005f);

    nk_combo_end(ctx);
}
```

> `nk_rgb_cf()` converts `nk_colorf` → `nk_color` for the button preview swatch.  
> `nk_widget_width(ctx)` matches the dropdown width to its parent widget automatically.

---

### Window anatomy

```
nk_begin(ctx, title, nk_rect(x, y, w, h), flags)

┌─ NK_WINDOW_BORDER ──────────────────────┐
│ Demo                              [−]   │ ← NK_WINDOW_TITLE + NK_WINDOW_MINIMIZABLE
├─────────────────────────────────────────┤
│                                         │
│   widgets go here                       │ ← NK_WINDOW_MOVABLE   (drag title bar)
│                                         │   NK_WINDOW_SCALABLE  (drag corner)
│                                      ◢  │
└─────────────────────────────────────────┘
         nk_end(ctx)  ← always call, even if nk_begin returned 0
```

```c
if (nk_begin(ctx, "Demo", nk_rect(50, 50, 230, 250),
    NK_WINDOW_BORDER | NK_WINDOW_MOVABLE | NK_WINDOW_SCALABLE |
    NK_WINDOW_MINIMIZABLE | NK_WINDOW_TITLE))
{
    // widgets here
}
nk_end(ctx);
```
