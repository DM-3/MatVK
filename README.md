# MatVK
Basic matrix math using Vulkan compute shaders

- [Requirements](#requirements)
- [Build](#build)
- [Run](#run)


## Requirements

- [Vulkan SDK 1.3+](https://vulkan.lunarg.com/)
- Vulkan 1.3+ compatible graphics card


## Build
Within the toplevel directory of the repository, create a build directory:
```
mkdir build && cd build
```

Then configure the cmake project:
```
cmake .. [<options>]
```
where ```options``` can be:
- ```-DBUILD_EXAMPLES=<ON|OFF>``` to enable/disable building examples (default: ```ON```)
- ```-DBUILD_TESTS=<ON|OFF>``` to enable/disable building tests (default: ```ON```)

Finally build with:
```
cmake --build .
```


## Run
From the toplevel directory of the repository,

...run examples with:
```
build/examples/<name>
```

...run tests with:
```
ctest --test-dir build/tests
```
