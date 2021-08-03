![My First Vulkan Engine - Written in Cpp](./resources/readme-title.png)

![Seperator](./resources/readme-seperator.png)

<p align="center">
  <b> 
My First Vulkan Engine (MFVE) is an open source game and rendering engine written in modern C++
  </b>
</p>

<p align="center">
  <b> 
    <a href="https://github.com/WSWhitehouse/my-first-vulkan-engine/issues"><img alt="GitHub issues" src="https://img.shields.io/github/issues/WSWhitehouse/my-first-vulkan-engine?style=flat-square"></a>
    <a href="https://github.com/WSWhitehouse/my-first-vulkan-engine"><img alt="GitHub license" src="https://img.shields.io/github/license/WSWhitehouse/my-first-vulkan-engine?style=flat-square"></a>
  </b>
</p>

![Seperator](./resources/readme-seperator.png)

# Getting Started
### Platform Support 
  - `Linux`
  - `Windows x64`

The development of this engine is mainly focused on `Linux` so expect missing/incomplete features and bugs on other platforms.

### Prerequisites 
- Download and install the `VulkanSDK` - available on the [LunarG Website](https://vulkan.lunarg.com/sdk/home)
- Download `Premake5` - available here: [https://premake.github.io/](https://premake.github.io/)

### Cloning The Repo
- Clone the repo recursively to initialise the submodules
  - HTTPS: `git clone --recursive https://github.com/WSWhitehouse/my-first-vulkan-engine`
  - SSH: `git clone --recursive git@github.com:WSWhitehouse/my-first-vulkan-engine.git`
  

- Initialise the submodules if the repository was not cloned recursively 
  - `git submodule update --init`
  
### Customising Your Application

Your application is automatically generated through Premake. 
In order to change and customise it, you can edit some values from within the premake lua files.

1. Open the `premake5.lua` file in the root of this repository.
2. Edit the variables in the first section of the file.

<p align="center">
  <b><i> Don't edit anything below the "Workspace" comment or any other premake / lua files unless you know what you're doing </i></b>
</p>

### Running Premake5
1. Open the terminal/cmd in the root of the project directory.
2. Run the premake5 command for the required IDE, for example `premake5 vs2019` which will generate Visual Studio 2019 project files

![Seperator](./resources/readme-seperator.png)

# About MFVE
 about ...

![Seperator](./resources/readme-seperator.png)

# License
This project is under the MIT License. Check out the [License file](https://github.com/WSWhitehouse/my-first-vulkan-engine/blob/main/LICENSE.md) for more information.

![Seperator](./resources/readme-seperator.png)
