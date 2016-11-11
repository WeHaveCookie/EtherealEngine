# Ethereal Engine

## Presentation
Ethereal Engine is a very basic "Game Engine" mainly based on the SFML. It's used Moodycamel, Rapidjson, and ImGui. We can find link in the External section
It's currently in development and much work remains to be done.

## How to use
Fork this repo for get all external lib needed by Ethereal Engine 
https://github.com/WeHaveCookie/External

Run premake.bat for generate .sln for visual studio.
Tips : Sometime, regenerate solution by premake removes create option for PCH. Just rigth-click on stdafx.cpp and select Create /Yc in Precompiled Headers option.


## Changelog
### Features
- [x] Entity management
- [x] Music and sound management
- [x] Loading thread
- [x] Save thread
- [x] Persistent management
- [x] Input management (8 pads max)
- [x] Gui management for ImGui display
- [x] File management
- [x] Command management
- [x] Physic management
- [ ] Camera management
- [ ] IA management
- [ ] Level management
- [ ] Menu management
- [ ] Gui management for game
- [ ] Event management

### Tools
- [x] Entity creator
- [x] Entity viewer (include animtion viewer)
- [x] Music and Sound creator
- [x] Music and Sound viewer
- [x] Input binder
- [x] Input viewer
- [ ] Level editor
- [ ] Level viewer
- [ ] Animation editor



## External
Ethereal Engine uses these following external repositories:

ImGui : https://github.com/ocornut/imgui

Moodycamel : https://github.com/cameron314/concurrentqueue

Rapidjson : https://github.com/miloyip/rapidjson

SFML : https://github.com/SFML/SFML/

## License

Ethereal Engine is licensed under the MIT License, see LICENSE for more information.
