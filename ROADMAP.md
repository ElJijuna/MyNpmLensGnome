# NPM Lens GNOME App — ROADMAP

App GNOME nativa que envuelve la web app [NpmLens](https://eljijuna.github.io/MyNpmLens/) usando WebKitGTK.

---

## Stack tecnológico

| Componente | Tecnología |
|---|---|
| Lenguaje | C |
| UI Toolkit | GTK4 + libadwaita |
| WebView | WebKitGTK 6.0 |
| Empaquetado | Flatpak |
| Build system | Meson |
| App ID | `io.github.eljijuna.NpmLens` |

---

## Decisiones de diseño

- **HeaderBar nativa con AdwToolbarView** — la barra de título y los controles de ventana son nativos de GNOME (AdwHeaderBar), el contenido vive en el WebView.
- **JS ↔ C bridge via WebKit Message Handlers** — el HTML puede invocar acciones nativas (cerrar app, minimizar, etc.) usando `window.webkit.messageHandlers.app.postMessage("close")`.
- **Eventos C → JS via `dispatchEvent`** — el botón Add nativo despacha `new CustomEvent('gnome:open-dialog-addpackage')` al `window` del WebView.
- **GResource para assets** — el icono se embebe en el binario como GResource para funcionar sin instalar.

---

## Fase 1 — Esqueleto del proyecto

**Objetivo:** estructura de archivos lista para desarrollo.

- [x] Definir estructura de directorios (`src/`, `data/`)
- [x] `meson.build` raíz con dependencias GTK4, libadwaita, webkitgtk-6.0
- [x] `io.github.eljijuna.NpmLens.json` — manifest de Flatpak
- [x] `.desktop` entry (lanzador de app)
- [x] Icono de aplicación (SVG del proyecto web)
- [x] `io.github.eljijuna.NpmLens.metainfo.xml` (AppStream)

Resultado: `meson setup build && meson compile -C build` compila sin errores.

---

## Fase 2 — Ventana con WebView y HeaderBar nativa

**Objetivo:** app que arranca y carga la URL con barra de título nativa.

- [x] `main.c` — punto de entrada, `AdwApplication` + `g_application_run()`
- [x] `window.c` / `window.h` — `AdwApplicationWindow` con `AdwToolbarView`
- [x] `AdwHeaderBar` con botón "+ Add" (`suggested-action`)
- [x] Cargar `https://eljijuna.github.io/MyNpmLens/` al iniciar
- [x] Tamaño por defecto (1000×700) y mínimo (600×400)
- [x] GResource: icono embebido en el binario (visible sin instalar)

Resultado: `./build/src/npmlens` abre la app con barra nativa y el icono correcto.

---

## Fase 3 — JS ↔ C bridge (Message Handlers + eventos)

**Objetivo:** comunicación bidireccional entre C y el WebView.

- [x] Registrar `WebKitUserContentManager` con handler `"app"`
- [x] Botón Add envía `CustomEvent('gnome:open-dialog-addpackage')` al WebView via JS
- [ ] Conectar señal `script-message-received::app` → callback en C
- [ ] Implementar acciones manejadas desde JS:
  - `"close"` → `g_application_quit()`
  - `"minimize"` → `gtk_window_minimize()`
  - `"maximize"` → `gtk_window_maximize()` / `gtk_window_unmaximize()`

Resultado: botón Add abre el diálogo en la web app; HTML puede cerrar la ventana nativa.

---

## Fase 4 — Ajustes WebKit y UX

**Objetivo:** comportamiento correcto del WebView en contexto desktop.

- [x] `WebKitSettings`: JavaScript habilitado, hardware acceleration
- [ ] Interceptar navegación: URLs externas → `xdg-open` (browser del sistema)
- [ ] User-Agent personalizado (`NpmLens/1.0 WebKitGTK`)
- [ ] Persistencia de cookies/sesión entre reinicios (`WebKitCookieManager`)
- [ ] Zoom: `Ctrl++` / `Ctrl+-` / `Ctrl+0`
- [ ] `Ctrl+Q` como atajo de salida desde C
- [ ] Manejo de error de red: inyectar página de error HTML con botón de reintentar

---

## Fase 5 — Empaquetado Flatpak

**Objetivo:** distribuir la app como Flatpak.

- [ ] Manifest `io.github.eljijuna.NpmLens.json` completo
  - Runtime: `org.gnome.Platform` 48
  - SDK: `org.gnome.Sdk` 48
- [ ] Permisos mínimos (`--share=network`, `--socket=wayland`, `--socket=fallback-x11`)
- [ ] Build local: `flatpak-builder build-dir manifest.json`
- [ ] Instalar y probar: `flatpak-builder --install --user`
- [ ] Script `make-release.sh` para generar `.flatpak` bundle

Resultado: `flatpak install NpmLens.flatpak` funciona desde cero.

---

## Fase 6 — Publicación (Flathub) *(opcional)*

- [ ] Cumplir requisitos de Flathub (AppStream completo, icono, permisos mínimos)
- [ ] PR al repositorio [flathub/flathub](https://github.com/flathub/flathub)
- [ ] CI con `flatpak-github-actions`

---

## Estructura de directorios objetivo

```
NpmLensGnome/
├── src/
│   ├── main.c
│   ├── window.c
│   └── window.h
├── data/
│   ├── icons/
│   │   └── io.github.eljijuna.NpmLens.svg
│   ├── io.github.eljijuna.NpmLens.desktop
│   ├── io.github.eljijuna.NpmLens.metainfo.xml
│   └── io.github.eljijuna.NpmLens.gresource.xml
├── io.github.eljijuna.NpmLens.json   # Flatpak manifest
├── meson.build
├── ROADMAP.md
└── LICENSE
```

---

## Orden de implementación

```
Fase 1 → Fase 2 → Fase 3 → Fase 4 → Fase 5 → Fase 6
```

Cada fase produce algo funcional antes de pasar a la siguiente.
