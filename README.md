# libcrobj
UI Abstraction and Rotation Core for Cronos

---

## 🚀 Overview

- 🧩 libcrobj is a lightweight helper library built on top of LVGL, providing consistent rotation and layout behavior for UI objects within the Cronos platform.

- LVGL does not offer unified rotation or rotation-aware layout handling. libcrobj fills this gap with a small abstraction layer that standardizes rotation logic, sizing rules, and object alignment.

- Applications currently still call LVGL APIs directly. libcrobj focuses on practical rotation/layout handling. A fuller abstraction that hides LVGL is planned for the future but not implemented yet.

---
## ✅ Current Features

- 🔄 Rotation logic: Unified handling for screen and object rotation, supplementing missing LVGL features.

- 📐 Layout & alignment helpers: Consistent positioning/sizing rules independent of LVGL quirks.

- 🧰 Common UI utilities: Shared helpers (used by terminal-ui, …) to avoid duplicated logic.

- 🗂️ Menu widget (stable): First widget built on the rotation-aware layout model.

- ⌨️ Keyboard widget (in progress): Ported from legacy terminal-ui and being adapted to the new rotation + layout logic.

---
## 🔮 Future Directions

Where libcrobj may expand as Cronos evolves:

- 📦 Reduced direct LVGL usage: Gradual encapsulation of more LVGL calls within the library.

- 📱 Additional widgets: Phone-like components (panels, popups, navigation bars).

- ⌨️ Improved on-screen keyboard: Rotation-aware, scalable layout with multiple modes.

- 🔌 Optional hardware hooks:
    - Vibration feedback
    - Backlight/brightness control
