# FreeRTOS Windows Demo
This is a windows demo based on [FreeRTOS Demo](https://github.com/FreeRTOS/FreeRTOS/tree/main/FreeRTOS/Demo/Posix_GCC) for executing FreeRTOS simple simulation

# Execute
```bash
pio run -t exec
```

# Steps To Reproduce
1. Download Latest FreeRTOS Kernel Release from https://github.com/FreeRTOS/FreeRTOS-Kernel/releases
2. Extract Archive to `lib` folder
3. Copy from this repo the `.gitignore` from `lib/FreeRTOS-Kernel` to your project or delete unused ports in `portable` - Everything except `portable/MSVC-MingW`, `portable/ThirdParty/GCC/Posix`, `portable/Common` and `portable/MemMang`
4. Copy `library.json` from this repo to your `lib/FreeRTOS-Kernel` modify as needed
5. Add `build_flags = -I src -lwinmm` to your `native` section in `platformio.ini`
6. Copy `FreeRTOSConfig.h` to your `src` folder, this is a lean configuration that can run with minimum requirements

