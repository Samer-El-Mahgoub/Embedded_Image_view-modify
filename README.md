# Embedded_Image_view-modify
# 🖼️ Dynamic Resource Patcher (Win32 API)

## 📌 Overview
This repository contains a low-level C project demonstrating **Dynamic Resource Patching** within compiled Windows executables (`.exe`). It allows for the injection and replacement of embedded BMP images directly into the PE (Portable Executable) structure of a standalone binary at runtime. This is achieved using the native Win32 API, completely eliminating the need for external image files, source code modifications, or recompilation.

## 🚀 Features
- **True Standalone Execution:** The target application does not rely on external file paths. Images are safely embedded and loaded directly from its own resource section.
- **Dynamic Binary Manipulation:** Utilizes Windows APIs (`BeginUpdateResource`, `UpdateResource`, `EndUpdateResource`) to open, patch, and save executables dynamically.
- **Zero-Dependency Patching:** The modifier tool operates entirely through command-line arguments, altering the target binary directly in memory.

## 🛠️ Tech Stack
- **Language:** C
- **OS Interfacing:** Windows API (Win32)
- **Compiler/Toolchain:** GCC (MinGW) & `windres`

## ⚙️ Repository Structure
- `show_image.c`: The target application. A standalone viewer that loads an embedded image from its own binary using `MAKEINTRESOURCE(101)`.
- `image_modifier.c`: The patching tool. It takes a new `.bmp` image, strips its headers in memory, and safely injects the raw data into `show_image.exe`.
- `resource.rc`: The resource script enforcing the `LANG_NEUTRAL` configuration and linking the initial asset.

## 💻 Compilation & Usage

### 1. Build the Target Program
Compile the resource file and link it to the main viewer:
```bash
windres resource.rc -o resource.o
gcc show_image.c resource.o -o show_image.exe -mwindows
