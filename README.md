# 🧩 libcrobj — UI Abstraction and Rotation Core for Cronos

- libcrobj is a lightweight middleware library that provides a backend-
independent abstraction layer for managing UI objects, layout, and rotation
within the Cronos platform.

- It serves as an extended wrapper around LVGL, adding unified control over
screen and object rotation, layout management, and object alignment while
keeping UI logic fully decoupled from the underlying rendering backend.

- Designed with modularity and future scalability in mind, libcrobj enables
multiple applications to share the same UI foundation and allows easy backend
replacement (e.g., LVGL, Wayland) without changing higher-level logic.

