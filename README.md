<div align=center>
  <img src="resources/images/splash.png">
</div>
<p align="center">
  A to-do list software tailored for software engineers, inspired by the Revelation album of G.E.M.
</p>

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
git clone --recursive https://github.com/Codingdogzxg/Revelation.git
```

### Edit CMakePresets.json

```json
"configurePresets": [
    {
        "binaryDir": "E:/GithubRepos/Revelation/build", // your build folder
        "cacheVariables": {
            "REVELATION_OUTPUT_DIR": "E:/GithubRepos/Revelation/binary", // your binary folder
            "CMAKE_PREFIX_PATH": "D:/ProgramFiles/Qt/6.6.3/msvc2019_64/lib/cmake" // your qt cmake path
    	}
    }
]
```

### Build
```bash
mkdir build
cd build
cmake ../SourceCode/ --preset windows
```

### Deploy executable 

```bash
# using qt msvc command line tool
# cd to binary folder where `Revelation.exe` is located
windeployqt Revelation.exe
```

### Copy resource files

Copy files from `./resources` to the binary folder where `Revelation.exe` is located.

### Open solution and set 'Revelation' as default start up project

# Reference
+ [mowangshuying/CppQtFluentUi888: FluentUI For Cpp Qt.(github.com)](https://github.com/mowangshuying/CppQtFluentUi888)
