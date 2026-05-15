# Nuklear + SDL3 Renderer – Dokumentation

Basierend auf `demo/sdl3_renderer/main.c` aus dem [Nuklear-Repository](https://github.com/Immediate-Mode-UI/Nuklear).

---

## Was ist Nuklear?

Nuklear ist eine **Immediate-Mode GUI-Bibliothek** in ANSI C – eine einzige Header-Datei (`nuklear.h`). Das bedeutet: Es gibt keinen gespeicherten Widget-State in der Bibliothek. Du beschreibst die GUI **jeden Frame neu** und Nuklear rendert sie. State wie "ist die Checkbox an?" liegt bei **dir** – in deinen eigenen Variablen.

> **Faustregel:** Kein `nk_button_create()` und später `nk_button_draw()`. Stattdessen: `if (nk_button_label(ctx, "Klick mich")) { /* wurde geklickt */ }` – alles in einem Aufruf, jeden Frame.

---

## SDL3 Main Callbacks – das neue Einstiegsmodell

Statt klassischem `main()` mit eigener Event-Loop nutzt dieses Demo das **SDL3 Main Callbacks**-System:

```c
#define SDL_MAIN_USE_CALLBACKS
#include <SDL3/SDL_main.h>
```

SDL3 ruft dann automatisch vier Funktionen auf, die du implementierst:

| Funktion | Wann aufgerufen | Zweck |
|---|---|---|
| `SDL_AppInit()` | Einmalig beim Start | Fenster, Renderer, Nuklear initialisieren |
| `SDL_AppEvent()` | Pro SDL-Event | Events an Nuklear weiterleiten |
| `SDL_AppIterate()` | Jeden Frame | GUI definieren & rendern |
| `SDL_AppQuit()` | Beim Beenden | Aufräumen |

Der Rückgabewert steuert den App-Flow: `SDL_APP_CONTINUE` läuft weiter, `SDL_APP_SUCCESS` beendet sauber, `SDL_APP_FAILURE` bricht mit Fehler ab.

---

## App-State: das `nk_sdl_app`-Struct

```c
struct nk_sdl_app {
    SDL_Window   *window;    // Das SDL-Fenster
    SDL_Renderer *renderer;  // SDL Hardware-Renderer
    struct nk_context *ctx;  // Nuklear-Kontext (das Herzstück)
    struct nk_colorf bg;     // Hintergrundfarbe (RGBA float)
    enum nk_anti_aliasing AA; // Anti-Aliasing an/aus
};
```

Dieser Struct wird als `appstate` zwischen den Callbacks herumgereicht – SDL verwaltet den Pointer, du castest ihn in den Callbacks:

```c
struct nk_sdl_app* app = (struct nk_sdl_app*)appstate;
```

---

## Phase 1: Initialisierung (`SDL_AppInit`)

### 1.1 SDL starten und Fenster erstellen

```c
SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS);

SDL_CreateWindowAndRenderer(
    "Nuklear: SDL3 Renderer",
    WINDOW_WIDTH, WINDOW_HEIGHT,
    SDL_WINDOW_RESIZABLE,
    &app->window,
    &app->renderer
);

SDL_SetRenderVSync(app->renderer, 1); // VSync an
```

### 1.2 HiDPI / Display Scale

Auf Retina- oder HiDPI-Displays wird der Renderer skaliert, damit die GUI nicht winzig wirkt:

```c
const float scale = SDL_GetWindowDisplayScale(app->window);
SDL_SetRenderScale(app->renderer, scale, scale);
float font_scale = scale; // Merken für Font-Baking
```

> **Wichtig:** Wenn sich der Scale ändert (z.B. Fenster auf anderen Monitor ziehen), feuert SDL `SDL_EVENT_WINDOW_DISPLAY_SCALE_CHANGED`. Das Demo loggt nur eine Warnung – in einer echten App müsstest du hier den Font neu backen.

### 1.3 Nuklear initialisieren

```c
struct nk_context *ctx = nk_sdl_init(app->window, app->renderer, nk_sdl_allocator());
```

`nk_sdl_allocator()` ist ein SDL-nativer Allocator (nutzt `SDL_malloc`/`SDL_free`). Besser als `NK_INCLUDE_DEFAULT_ALLOCATOR`, weil portabler.

### 1.4 Font-Baking

Nuklear rendert Fonts selbst – dafür müssen sie einmalig in eine Textur "gebacken" werden:

```c
struct nk_font_atlas *atlas;
struct nk_font_config config = nk_font_config(0);
struct nk_font *font;

atlas = nk_sdl_font_stash_begin(ctx);          // Atlas vorbereiten
font = nk_font_atlas_add_default(atlas, 13 * font_scale, &config); // Default-Font
// Alternativ eigene TTF:
// font = nk_font_atlas_add_from_file(atlas, "MeinFont.ttf", 16 * font_scale, &config);
nk_sdl_font_stash_end(ctx);                    // Textur hochladen

// HiDPI-Hack: Font-Höhe zurückskalieren, damit Layouts korrekt berechnet werden
font->handle.height /= font_scale;

nk_style_set_font(ctx, &font->handle);         // Font aktivieren
```

**Ablauf zusammengefasst:** `stash_begin` → Fonts hinzufügen → `stash_end` → Höhe korrigieren → `set_font`

> **Alternative:** `nk_sdl_style_set_debug_font(ctx)` setzt einen simplen 8×8-Pixelfont – kein Baking nötig, aber sehr klein und unscharf bei Skalierung.

### 1.5 Input-Initialisierung

```c
nk_input_begin(ctx); // Erste Input-Sammlung starten
```

Dieser Aufruf muss **vor** dem ersten Event-Loop stehen. In der normalen Loop läuft er am **Ende** von `SDL_AppIterate`.

---

## Phase 2: Event-Handling (`SDL_AppEvent`)

```c
SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event) {
    struct nk_sdl_app* app = (struct nk_sdl_app*)appstate;

    switch (event->type) {
        case SDL_EVENT_QUIT:
            return SDL_APP_SUCCESS;
        case SDL_EVENT_KEY_DOWN:
            if (event->key.key == SDLK_Q && event->key.mod & SDL_KMOD_CTRL)
                return SDL_APP_SUCCESS;
            break;
    }

    // WICHTIG: Koordinaten auf Renderer-Space umrechnen
    SDL_ConvertEventToRenderCoordinates(app->renderer, event);

    // Event an Nuklear übergeben
    nk_sdl_handle_event(app->ctx, event);

    return SDL_APP_CONTINUE;
}
```

**Kritischer Punkt:** `SDL_ConvertEventToRenderCoordinates()` muss **vor** `nk_sdl_handle_event()` aufgerufen werden, weil der Renderer mit Custom Scale läuft. Ohne diese Konvertierung stimmen Mausklick-Positionen nicht mit dem überein, was auf dem Bildschirm zu sehen ist.

---

## Phase 3: Frame-Loop (`SDL_AppIterate`)

Das ist das Herzstück – hier passiert alles jeden Frame:

```c
SDL_AppResult SDL_AppIterate(void *appstate) {
    struct nk_sdl_app* app = (struct nk_sdl_app*)appstate;
    struct nk_context* ctx = app->ctx;

    // 1) Input-Sammlung abschließen
    nk_input_end(ctx);

    // 2) GUI definieren (Immediate Mode!)
    if (nk_begin(ctx, "Demo", nk_rect(50, 50, 230, 250),
        NK_WINDOW_BORDER | NK_WINDOW_MOVABLE | NK_WINDOW_SCALABLE |
        NK_WINDOW_MINIMIZABLE | NK_WINDOW_TITLE))
    {
        // ... Widgets hier ...
    }
    nk_end(ctx);

    // 3) SDL Renderer leeren
    SDL_SetRenderDrawColorFloat(app->renderer,
        app->bg.r, app->bg.g, app->bg.b, app->bg.a);
    SDL_RenderClear(app->renderer);

    // 4) Nuklear rendern
    nk_sdl_render(ctx, app->AA);

    // 5) Text-Input-State aktualisieren (für Textfelder)
    nk_sdl_update_TextInput(ctx);

    // 6) Frame anzeigen
    SDL_RenderPresent(app->renderer);

    // 7) Neue Input-Sammlung starten
    nk_input_begin(ctx);

    return SDL_APP_CONTINUE;
}
```

### Die Frame-Reihenfolge im Überblick

```
nk_input_end()          ← Input der letzten Frame abschließen
  ↓
GUI definieren           ← nk_begin / Widgets / nk_end
  ↓
SDL_RenderClear()        ← Bildschirm leeren
  ↓
nk_sdl_render()          ← Nuklear zeichnet in SDL-Renderer
  ↓
nk_sdl_update_TextInput() ← SDL Keyboard-Input für Textfelder sync
  ↓
SDL_RenderPresent()      ← Frame anzeigen
  ↓
nk_input_begin()         ← Nächsten Input-Zyklus starten
```

---

## Widget-Übersicht aus dem Demo

### Fenster öffnen/schließen

```c
if (nk_begin(ctx, "Fenstername", nk_rect(x, y, breite, hoehe), flags)) {
    // Widgets hier
}
nk_end(ctx); // IMMER aufrufen, auch wenn nk_begin false zurückgab
```

**Wichtige Flags:**

| Flag | Bedeutung |
|---|---|
| `NK_WINDOW_BORDER` | Rahmen |
| `NK_WINDOW_MOVABLE` | Verschiebbar |
| `NK_WINDOW_SCALABLE` | Größe änderbar |
| `NK_WINDOW_MINIMIZABLE` | Minimierbar |
| `NK_WINDOW_TITLE` | Titelleiste |

### Layout-Rows

Vor jedem Widget-Block muss ein Layout definiert werden:

```c
// Statisch: 1 Widget, 80px breit, 30px hoch
nk_layout_row_static(ctx, 30, 80, 1);

// Dynamisch: 2 Widgets, teilen sich die Breite gleichmäßig
nk_layout_row_dynamic(ctx, 30, 2);
```

### Button

```c
if (nk_button_label(ctx, "button")) {
    SDL_Log("button pressed"); // Wird ausgeführt wenn geklickt
}
```

### Radio-Buttons (Options)

```c
enum { EASY, HARD };
static int op = EASY;

nk_layout_row_dynamic(ctx, 30, 2);
if (nk_option_label(ctx, "easy", op == EASY)) op = EASY;
if (nk_option_label(ctx, "hard", op == HARD)) op = HARD;
```

Der zweite Parameter ist der "aktiv"-Zustand – du vergleichst selbst.

### Integer-Property (Slider mit Zahleneingabe)

```c
static int property = 20;
nk_layout_row_dynamic(ctx, 25, 1);
nk_property_int(ctx, "Compression:", 0, &property, 1000, 1, 1);
//                    Label         Min  Wert*      Max  Step PixelPerStep
```

### Label

```c
nk_layout_row_dynamic(ctx, 20, 1);
nk_label(ctx, "background:", NK_TEXT_LEFT);
// Alignment: NK_TEXT_LEFT, NK_TEXT_CENTERED, NK_TEXT_RIGHT
```

### Combo-Box mit Color-Picker

```c
if (nk_combo_begin_color(ctx, nk_rgb_cf(app->bg), nk_vec2(widget_width, 400))) {
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

`nk_rgb_cf()` konvertiert `nk_colorf` → `nk_color` für die Anzeige im Combo-Button.  
`nk_widget_width(ctx)` gibt die aktuelle Widget-Breite zurück – nützlich für responsive Dropdowns.

---

## Phase 4: Aufräumen (`SDL_AppQuit`)

```c
void SDL_AppQuit(void *appstate, SDL_AppResult result) {
    struct nk_sdl_app* app = (struct nk_sdl_app*)appstate;
    if (app) {
        nk_input_end(app->ctx);       // Offenen Input-Block schließen
        nk_sdl_shutdown(app->ctx);    // Nuklear + Atlas + Texturen freigeben
        SDL_DestroyRenderer(app->renderer);
        SDL_DestroyWindow(app->window);
        SDL_free(app);
    }
}
```

---

## Defines: Was muss/kann ich setzen?

### Pflicht (für sdl3_renderer)

```c
#define NK_INCLUDE_COMMAND_USERDATA       // Für Renderer-Erweiterungen
#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT   // Vertex-basiertes Rendering
```

### Optional aber empfohlen

```c
#define NK_INCLUDE_FONT_BAKING    // TTF-Font-Unterstützung
#define NK_INCLUDE_DEFAULT_FONT   // Eingebauter Fallback-Font
#define NK_INCLUDE_STANDARD_VARARGS
#define NK_INCLUDE_STANDARD_IO
```

### SDL-Integration (statt eigener Implementierungen)

```c
// Typen von SDL übernehmen statt NK_INCLUDE_FIXED_TYPES
#define NK_INT8  Sint8
#define NK_UINT8 Uint8
// ... (siehe main.c für alle)

// SDL-Funktionen nutzen statt libc
#define NK_ASSERT(c)      SDL_assert(c)
#define NK_MEMSET(d,c,l)  SDL_memset(d,c,l)
#define NK_MEMCPY(d,s,l)  SDL_memcpy(d,s,l)
#define NK_SIN(f)         SDL_sinf(f)
#define NK_COS(f)         SDL_cosf(f)
```

### Implementierung aktivieren (einmalig, in genau einer .c-Datei)

```c
#define NK_IMPLEMENTATION
#include "nuklear.h"

#define NK_SDL3_RENDERER_IMPLEMENTATION
#include "nuklear_sdl3_renderer.h"
```

> **Achtung:** Diese beiden `_IMPLEMENTATION`-Defines dürfen nur in **einer** Translation Unit gesetzt werden. In anderen .c-Dateien die Header ohne das Define inkludieren.

---

## Häufige Stolperfallen

| Problem | Ursache | Lösung |
|---|---|---|
| Mausklicks treffen falsche Stelle | `SDL_ConvertEventToRenderCoordinates` fehlt | Vor `nk_sdl_handle_event` aufrufen |
| `nk_end` vergessen | Nuklear-State korrupt | **Immer** `nk_end` nach `nk_begin`, auch wenn `nk_begin` false zurückgab |
| Font zu klein auf HiDPI | `font->handle.height` nicht angepasst | Nach `stash_end`: `font->handle.height /= font_scale` |
| Textfelder funktionieren nicht | `nk_sdl_update_TextInput` fehlt | Nach `nk_sdl_render` aufrufen |
| `NK_IMPLEMENTATION` mehrfach | Linker-Fehler (duplicate symbols) | Nur in einer `.c`-Datei definieren |
| Input-Zyklus falsch | `nk_input_begin`/`end` falsch verschachtelt | Ende von Iterate: `begin`; Anfang von Iterate: `end` |

---

## Beispiel-Struktur für eigene Projekte

```
mein_projekt/
├── nuklear.h                    # Nuklear Single-Header
├── nuklear_sdl3_renderer.h      # SDL3-Backend
└── main.c
    ├── Defines (NK_IMPLEMENTATION etc.)
    ├── #include "nuklear.h"
    ├── #include "nuklear_sdl3_renderer.h"
    ├── struct MyApp { ... }     // Eigener App-State
    ├── SDL_AppInit()            // Init
    ├── SDL_AppEvent()           // Events
    ├── SDL_AppIterate()         // Frame: GUI + Render
    └── SDL_AppQuit()            // Cleanup
```

---

*Dokumentation erstellt auf Basis von `demo/sdl3_renderer/main.c` – Nuklear Repository (Immediate-Mode-UI/Nuklear)*
