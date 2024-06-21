# Revelation
A to-do list software tailored for software engineers.
# Build Steps:
## [Windows]
### Requirements
- Visual
 Studio 17 2022 with C++ desktop development tools installed.
- VCPKG
- Qt 6.3.3
- CMAKE

### Install VCPKG
```bash
git clone https://github.com/microsoft/vcpkg.git
cd vcpkg
./bootstrap-vcpkg.bat
```

### Set Environment Variables For VCPKG (set only once)
```bash
:: VCPKG_DOWNLOADS vcpkg download cache folder
:: VCPKG_DEFAULT_BINARY_CACHE vcpkg binary cache folder
setx VCPKG_ROOT "path_to_your_vcpkg_root"
setx VCPKG_DOWNLOADS "path_to_an_empty_folder1"
setx VCPKG_DEFAULT_BINARY_CACHE "path_to_an_empty_folder2"
```

### Clone Repository

```bash
git clone https://github.com/Codingdogzxg/Revelation.git
```

### Edit CMakePresets.json

### Build
```bash
mkdir build
cd build
cmake ../SourceCode/ --presets windows
```
