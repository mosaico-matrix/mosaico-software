# PixelForge software

--- 

## Requirements

### If you compile directly on Raspberry Pi
- CURL

### If you simulate the matrix on your computer
- CURL
- X11-dev
- SQLite3
---

## External libraries included as submodules
- hzeller/rpi-rgb-led-matrix
- chaiscript
- nlohmann/json
- mrtazz/restclient-cpp
- SqliteModernCpp/sqlite_modern_cpp

--- 
## Scripting for runners

### Chaiscript
Chaiscript is used for scripting the runners
- Cheetsheet: https://github.com/ChaiScript/ChaiScript/blob/develop/cheatsheet.md
- C++ chaiscript API: https://codedocs.xyz/ChaiScript/ChaiScript/index.html
- Chaiscript language reference: https://codedocs.xyz/ChaiScript/ChaiScript/LangGettingStarted.html

### Editor support
- VSCode: https://marketplace.visualstudio.com/items?itemName=codehz.chaiscript-syntax
- Sublime Text: https://github.com/ChaiScript/sublimetext-chaiscript?tab=readme-ov-file

### File structure
Every runner script file has a very simple structure:
```chaiscript

// Code that will be executed when the runner is loaded
global text = _createText();
text.setText("Test");
text.translate(24,25);

def loop()
{
    // This is the main loop of the runner
    // It will be called every frame
    text.setColor(RANDOM_COLOR());
    
    // Interacting directly with the canvas is only possible in loop()
    // If you try to do it outside of loop(), it will be lost as the canvas is cleared every frame
    _setPixel(10, 10, 255, 255, 255); 
}
```
### Scripting API
The runner can access a lot of functions and variables from the C++ code.
You can notice a C++ function call when it starts with an underscore `_` (e.g. `_createText()`)
#### Canvas
- `_setPixel(x, y, r, g, b)` - Set pixel at x, y to color r, g, b

---

## Database structure
The database is used to store various information about the runners installed on the matrix.
There are X tables in the database:
- `installed_runners` - Contains information about the runners installed on the matrix