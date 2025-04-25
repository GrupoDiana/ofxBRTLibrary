# ofxBRTLibrary  
*Addon for OpenFrameworks to integrate the Binaural Rendering Toolbox (BRT)*

## Introduction  
**ofxBRTLibrary** is an OpenFrameworks addon that provides a simple interface to the [Binaural Rendering Toolbox (BRT)](https://github.com/GrupoDiana/BRTLibrary), a powerful set of libraries and tools for binaural spatialisation and psychoacoustic experimentation. This addon allows OpenFrameworks users to integrate real-time spatial audio rendering into their interactive applications, installations, or VR/AR projects.

This addon is developed in the context of the [SONICOM project](https://www.sonicom.eu/), and builds on the technology of the [3D Tune-In Toolkit](https://github.com/3DTune-In/3dti_AudioToolkit).

The included example demonstrates real-time spatialisation of a white noise source located at 45 degrees azimuth with respect to the listener.

For more complex use cases and experiments, refer to our extended example repository: [BRTLibrary_Examples](https://github.com/GrupoDiana/BRTLibrary_Examples).

## License  
This addon is distributed under the terms of the **GNU General Public License v3.0 or later (GPLv3+)**, in line with the main BRT Library.  
Please see `license.md` for full license details.

## Installation  
1. Clone or download this repository.  
2. Place the `ofxBRTLibrary` folder inside your `openFrameworks/addons/` directory.  
3. Open the included example using the OpenFrameworks Project Generator, generate the project files, and compile. It should work out of the box.

## Dependencies  
- The **BRT Library** is included as a **git submodule** within this addon.  
- In turn the **BRT library** includes the Eigen library with submodule. 
- Make sure to initialize submodules after cloning the repository:
  

```bash
git submodule update --init --recursive
```

## Compatibility  

- **Tested with**: OpenFrameworks 0.12.x  

### Platforms  

- **Addon tested on**: Windows  
- **BRT Library tested on**: Windows, macOS, Android  
- **BRT Library expected to work on**: iOS  

## Known Issues  

- The addon requires manual setup of the development environment to ensure proper linking with the BRT static libraries. Follow the example project structure for guidance.

## Version History  

### Version 0.1 (April 2025)  

- First public release  
- Initial support for BRT spatial audio rendering in OpenFrameworks  
- iOS example rendering a dynamic white noise source at fixed azimuth  
- Compatible with **BRT Library version 2.2.0 and later**