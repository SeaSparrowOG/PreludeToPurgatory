## Prelude To Purgatory
DLL companion for Prelude To Purgatory.

## Building
### Requirements:
- CMake
- VCPKG
- Visual Studio 2026 (with desktop C++ development)
  - A build configuration for Visual Studio 2022 is provided.
---
### Instructions:
**IMPORTANT:** Prelude to Purgatory requires the Creation Kit and Skyrim Special Edition to be installed in order to compile. A post-build event will re-compile all papyrus scripts, and merge everything into a BSA. If this isn't desirable, edit CMakeLists.txt to omit that step.
```
git clone https://github.com/SeaSparrowOG/PreludeToPurgatory
cd PreludeToPurgatory
git submodule update --recursive --init
cmake --preset vs-windows-vcpkg 
cmake --build Release --config Release
```
---
### Automatic deployment to MO2:
You can automatically deploy to MO2's mods folder by defining an Environment Variable named SKYRIM_MODS_FOLDER and pointing it to your MO2 mods folder. It will create a new mod with the appropriate name. After that, simply refresh MO2 and enable the mod.