# Gravy Renderer
This project is to develop a 3D renderer and tooling to maybe, and some day, be able to call it a 'Game Engine' and make a little game ou of it. At the moment the real purpose of this project is to gave me a reason to learn C++ and graphics programing.

There is no documentation for this at the moment and this project is only for my personal use but feel free to have fun with it if you want.

# Setting-up the building environnement

Please note that VSCode IDE is use to develop Poutine.

## Requirements
This project use `Cmake` to generate the project files.

- VCPKG (Packages manager preconfigured to download the following dependecies automaticaly)
    - OpenGL            (Open Graphics Library)
    - GLFW              (Window and input manager)
    - GLEW              (OpenGL wrapper)
    - GLM               (Math library)
    - ImGUI             (Graphical User interface)
    - spdlog            (Logger)
    - STB-IMAGE         (Image/texture loader)
    - Tiny OBJ Loader   (.obj model loader)
    - Tracy             (Profiler)

- Other manually added libraries :
    - SoLoud            (Portable c/c++ audio engine)

## Install VCPKG

### On Linux
You need to do a Git clone of the VCPKG repository in your "/home/$USER/.local/share/" directory:

```Console
git clone https://github.com/microsoft/vcpkg.git /home/$USER/.local/share/vcpkg
```

On Debian and Ubuntu derivatives:
```Console
sudo apt-get install curl zip unzip tar
```
On recent Red Hat and Fedora derivatives:
```Console
sudo dnf install curl zip unzip tar
```
On older Red Hat and Fedora derivatives:
```Console
sudo yum install curl zip unzip tar
```
On SUSE Linux and derivatives:
```Console
sudo zypper install curl zip unzip tar
```
On Arch Linux and derivatives:
```Console
sudo pacman -Syu base-devel git curl zip unzip tar cmake ninja
```
On Alpine:
```Console
apk add build-base cmake ninja zip unzip curl git
(and export VCPKG_FORCE_SYSTEM_BINARIES=1)
```

### On Windows 10/11
You need to do a Git clone of the VCPKG repository in your "C:/Users/%username%/" directory:

```Console
git clone https://github.com/microsoft/vcpkg.git /Users/$env:USERNAME/vcpkg
```