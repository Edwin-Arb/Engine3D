# Engine3D

A small 3D engine written in C++ with OpenGL.

**[English](#english) · [Русский](#русский)**

---

## English

Engine3D is a starting point for a 3D engine built on modern OpenGL.
It opens a window with an OpenGL 3.3 Core context and clears it each frame —
the foundation to build a renderer on top of.

### Tech stack

| Component | Purpose |
|-----------|---------|
| [GLFW](https://www.glfw.org/) | Window, OpenGL context and input |
| [GLAD 2](https://github.com/Dav1dde/glad) | OpenGL function loader |
| [GLM](https://github.com/g-truc/glm) | Math (vectors, matrices) |
| CMake | Build system |

All dependencies are vendored under `ThirdParty/`, so no system-wide installation is required.

### Requirements

- A C++17 compiler (developed with **Visual Studio 2022 / MSVC**)
- **CMake ≥ 3.20**
- **Python 3** with the **Jinja2** package — GLAD generates its loader at configure time:
  ```
  pip install jinja2
  ```
- A GPU and driver supporting **OpenGL 3.3 Core**

### Build & run

**In an IDE (Rider / CLion / Visual Studio):**
Open the project folder — the IDE detects `CMakeLists.txt`, configures the project,
then run the `Engine3D` target.

**From the command line:**
```bash
cmake -S . -B build
cmake --build build --config Debug
./build/bin/Debug/Engine3D      # Windows
```

`ESC` closes the window.

### Project structure

```
Engine3D/
├─ CMakeLists.txt      # build script (links GLFW + GLAD + GLM)
├─ Engine3D/
│  └─ Engine3D.cpp     # entry point
├─ ThirdParty/         # vendored dependencies
│  ├─ glfw/            # built from source
│  ├─ glad/            # loader generated at configure time
│  └─ glm/             # header-only
└─ .gitignore
```

---

## Русский

Engine3D — заготовка 3D-движка на современном OpenGL.
Открывает окно с контекстом OpenGL 3.3 Core и очищает его каждый кадр —
основа, поверх которой строится рендерер.

### Технологии

| Компонент | Назначение |
|-----------|------------|
| [GLFW](https://www.glfw.org/) | Окно, контекст OpenGL и ввод |
| [GLAD 2](https://github.com/Dav1dde/glad) | Загрузчик функций OpenGL |
| [GLM](https://github.com/g-truc/glm) | Математика (векторы, матрицы) |
| CMake | Система сборки |

Все зависимости лежат в `ThirdParty/`, поэтому ничего ставить в систему не нужно.

### Требования

- Компилятор C++17 (разрабатывалось на **Visual Studio 2022 / MSVC**)
- **CMake ≥ 3.20**
- **Python 3** с пакетом **Jinja2** — GLAD генерирует загрузчик на этапе конфигурации:
  ```
  pip install jinja2
  ```
- Видеокарта и драйвер с поддержкой **OpenGL 3.3 Core**

### Сборка и запуск

**В IDE (Rider / CLion / Visual Studio):**
Открой папку проекта — IDE сама найдёт `CMakeLists.txt`, сконфигурирует проект,
после чего запусти цель `Engine3D`.

**Из командной строки:**
```bash
cmake -S . -B build
cmake --build build --config Debug
./build/bin/Debug/Engine3D      # Windows
```

`ESC` закрывает окно.

### Структура проекта

```
Engine3D/
├─ CMakeLists.txt      # скрипт сборки (линкует GLFW + GLAD + GLM)
├─ Engine3D/
│  └─ Engine3D.cpp     # точка входа
├─ ThirdParty/         # встроенные зависимости
│  ├─ glfw/            # собирается из исходников
│  ├─ glad/            # загрузчик генерируется при конфигурации
│  └─ glm/             # header-only
└─ .gitignore
```
