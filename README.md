# CronosEngine
[![License](https://img.shields.io/github/license/lucho1/CronosEngine?color=red&label=License&style=plastic)](https://github.com/lucho1/CronosEngine/blob/master/LICENSE)
[![Version](https://img.shields.io/github/v/tag/lucho1/CronosEngine?color=blue&label=Version&style=plastic)](https://github.com/lucho1/CronosEngine/releases)
![Last Commit](https://img.shields.io/github/last-commit/lucho1/CronosEngine?color=blue&label=Last%20Commit&style=plastic)

![Code Size](https://img.shields.io/github/languages/code-size/lucho1/CronosEngine?color=green&label=Code%20Size&style=plastic)
![Repo Size](https://img.shields.io/github/repo-size/lucho1/CronosEngine?color=green&label=Repo%20Size&style=plastic)
![GH Top Language](https://img.shields.io/github/languages/top/lucho1/CronosEngine?color=green&style=plastic)
![GH Total Languages](https://img.shields.io/github/languages/count/lucho1/CronosEngine?color=green&style=plastic)

![Total Downloads](https://img.shields.io/github/downloads/lucho1/CronosEngine/total?color=green&label=Total%20Downloads&style=plastic)
![Watchers](https://img.shields.io/github/watchers/lucho1/CronosEngine?style=social)
![Forks](https://img.shields.io/github/forks/lucho1/CronosEngine?style=social)

Cronos Engine is a 3D Game Engine based on OpenGL and made by 2 students (Lucho Suaya and Roger Leon) of the Degree in Videogames Design and Development of Universitat Politecnica de Catalunya for the Videogames Engines subject of 3rd course.

![](https://raw.githubusercontent.com/lucho1/CronosEngine/Cronos_development/CronosEngine/Cronos/res/Icons/Cronos_Engine_Logo.png)

We used [SDL](https://www.libsdl.org/), [SDL_Mixer](https://www.libsdl.org/projects/SDL_mixer/), STL, [OpenGL4](https://www.opengl.org/) with [GLAD](https://glad.dav1d.de/), [ImGui](https://github.com/ocornut/imgui), [Premake](https://premake.github.io/), [GLM](https://glm.g-truc.net/0.9.9/index.html), [GPUDetect](https://github.com/GameTechDev/gpudetect), [MMRG](https://www.flipcode.com/archives/Presenting_A_Memory_Manager.shtml), [ParShapes](https://prideout.net/shapes), [Assimp](http://www.assimp.org/), [Devil](http://openil.sourceforge.net/) and [JSON for Modern C++](https://nlohmann.github.io/json/).

## Innovating Features
* Extensive Filesystem with good UX Functionalities
* Asset Panel
  * Asset Browser (Searcher)
  * Workflow in and out
  * Renaming of Assets
  * Creation/Deletion of Assets and Folders
  * Names of Assets are 10 chars max but the full name appears when the mouse is on top
  * Icons with textures
  * Drag & Drop
  * Textures Changing
  * (Internal) File copy into project when dragging it and dropping it
  * Game Object Hierarchy (with no huge parenting for now)
  * Game Object Selection

* Started the implementation of node System
* Many different types of primitives creation (check it out!)
* Shaders usage with Vertex Arrays and a good abstraction for buffers and textures classes
* Engine Icon

* Namespaced Engine code
* OpenGL Automatic Error Handler and logger (check out Errors.h!) and with possibility of calling GLCall() for every OGL Function to get possible errors with OGL functions/methods (through glGetError()) with extensive information
* Extensive System Info Read (on GPU, CPU, Memory and Software!)

* Cronos Engine will detect if you have a dedicated NVIDIA or AMD Graphics Card and will use it automatically!
* Usage of Precompiled Header to imporve build performance
* Engine's Project built with a Premake file and Project.bat generator to automatize build process

## Installation
Just download a release and run the executable! Inside 'res' folder you'll find all the resources used by the engine (textures, models loaded, configuration,...).
**Note:** Be careful when modifying things on config file, it will probably modify engine's configuration. And please, don't delete the dll files, they are needed for the engine to work.

## Usage
**Camera Controls**
* Right Click:			Camera Look Around
* Right Click + WASD:		Move camera and look around (using camera's forward vector)
* Alt + Left Click:		Orbit object (or center if none selected)
* Alt + Left Click + WASD:	Orbit object and move around it (or center if not selected)
* Shift while Moving:		Duplicate camera moving speed.
* Right Click + T, G:		Move camera up and down.
* Mouse Wheel:			Zoom in/out.
* F:					Focus camera at object (or center if not selected)

## Changelog
v0.1
* Many different types of primitives creation
* Models Load, including textures.
* 3D Renderer
* Game Objects and Components Hierarchy
* Shaders usage with Vertex Arrays and a good abstraction for buffers and textures classes
* Camera Movement
* Engine's basic GUI
  * Implemented Asset Panel
    * Asset Browser (Searcher)
    * Workflow in and out
    * Renaming of Assets
    * Creation/Deletion of Assets and Folders
    * Names of Assets are 10 chars max but the full name appears when the mouse is on top
    * File Copy when adding them at project (internally)
    * Drag & Drop
    * Textures Changing
  * Full Configuration menus
  * UI Panels
    * Assetbrowser Panel
    * Inspector Panel (Under construction)
    * Hierarchy (Under Construction)
    * Configuration Panel
* Filesystem Implemented
* Game Objects & Hierarchy
* Hardware's Log Window - Extensive System Info Read (on GPU, CPU, Memory and Software, using GPUDetect library to get CPU/GPU information)
* Performance Window to track useful info (Memory, FPS, ...)
* Detection of a dedicated NVIDIA or AMD Graphics Card - automatic usage
* Implementation of MMRG to track memory leaks and usage (for internal purposes by now)
* Usage of Precompiled Header to imporve build performance
* Engine's Project built with a Premake file and a Generate.bat file.
* OpenGL Automatic Error Handler and logger (check out Errors.h!) and with possibility of calling GLCall() for every OGL Function to get possible errors with OGL functions/methods (through glGetError()) with extensive information
* Engine Icon
* Other external libraries implementation

## Credits
Cronos Engine is developed by **LuchoSuaya**(@lucho1) and **Roger Leon** (@rleonborras) for Videogames Engines subject of UPC's Degree in Videogames Design and Development, taught by Marc Garrig  Garcia [@markitus18].

Take a look to our web:
Take a look to our Github Repository: https://www.github.com/lucho1/CronosEngine

## Contributing
1. Fork it!
2. Create your feature branch: `git checkout -b my-newfeature`
3. Commit your changes: `git commit -am 'Add some
feature'`
4. Push to the branch: `git push origin my-new-feature`
5. Submit a pull request and we might look into it. :D

## License
**Copyright (c) 2019 Lucho Suaya Leiro and Roger Leon Borras**
Licensed under the **[Apache License](LICENSE.txt)**

Cronos Engine is Licensed under Apache License (http://www.apache.org/licenses/), you'll find the details in the License file in Github repository or in Engine's folder.