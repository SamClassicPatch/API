# Classics Patch API

This is a collection of headers that act as a public frontend for [Serious Sam Classics Patch](https://github.com/SamClassicPatch).

If you need to develop a plugin for the patch or any external module that interacts with the patch in any way, you have to use this API.

Every time a new Classics Patch update is released, `lib/classicscore.lib` is updated to match the [latest release](https://github.com/SamClassicPatch/SuperProject/releases).

> [!IMPORTANT]
> The static library is always based on `ClassicsCore.dll` for **The Second Encounter v1.07** (`Release_TSE107` configuration) due to being the most full-fledged version. This means that not all API functionality might be available depending on the build configuration. Though, every single API function is still implemented properly for all official builds to prevent linking errors. Keep that in mind when linking against third-party builds.

> [!TIP]
> If you need a static library for a specific build, you can manually generate it from `ClassicsCore.dll` using command line tools from any Visual Studio. Search up the instructions online on how to do it.

## API design

This API is largely inspired by [Steamworks API](https://partner.steamgames.com/), where functions are divided into so-called "interfaces".

### C API
The key difference from Steamworks API though is that this API is more "flat", with most functions using plain C linkage and calling conventions instead of residing inside C++ interface classes.

That's because as long as Classics Patch is loaded in memory, these functions may be called at any time without the need to check which interface is available.

### C++ API (optional)
This API also has a virtual C++ interface as an extra that simply wraps every C method into a virtual function under a specific class with a special accessor method.

This is mostly useful when accessing the API through the game shell instead of directly linking the core library. For example, when you want to optionally interact with the patch through your mod but you're not certain whether your mod is being played through the patch or not.

> [!WARNING]
In very rare circumstances when Classics Patch hasn't been initialized yet, the interface accessors return `NULL` and the main virtual interface **cannot** be retrieved through the game shell!

## Usage

This API requires a C++ compiler because of usage of classes, virtual functions, templates and so on.

1. Clone this repository somewhere near your project or add it as a submodule.
2. Include `include/classicspatch_api.h` header in the desired source file to get access to the entire API.
3. Configure the project to link against `lib/classicscore.lib` library.
> [!TIP]
> MSVC compilers allow you to specify the `#pragma comment(lib, "lib/classicscore.lib")` preprocessor directive in any source file.

## License

**Classics Patch API** is licensed under the MIT license (see LICENSE file).

> [!NOTE]
> The rest of [Serious Sam Classics Patch](https://github.com/SamClassicPatch) is licensed under GNU GPL v2 in order to be perfectly compatible with [Serious Engine 1](https://github.com/Croteam-official/Serious-Engine), which might not be compatible with many other licenses. This API is written without direct usage of Serious Engine, which allows it to have its own, more permissible license.
