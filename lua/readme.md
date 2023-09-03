How to use lua:

the lua app is defined in a folder called with the name of the app:

app/
    conf.txt
    main.lua
    logo.png -> 42x42


at the startup of the lua app, the run() function is called:

```lua
function run()
    print("Running app")
end
```

to create an object, call the function Gui(type, x, y, w, h, parent)

```lua
obj = Gui(WINDOW_TYPE)
obj = Gui(BOX_TYPE, x, y, w, h, parent)
obj = Gui(BUTTON_TYPE, x, y, w, h, parent)
obj = Gui(LABEL_TYPE, x, y, w, h, parent)
obj = Gui(IMAGE_TYPE, x, y, w, h, parent, filename)
```

you can edit the gui objects using the following function

```lua
setText(obj, "text")
setX(obj, value)
setY(obj, value)
setWidth(obj, value)
setHeight(obj, value)
```

edit the color:

```lua
setColor(obj, COLOR)
```

you can choose between:
    COLOR_LIGHT
    COLOR_BLACK
    COLOR_PRIMARY
    COLOR_SUCCESS
    COLOR_WHITE
    COLOR_WHITE


On gui objects, you can add events:

```lua
function foo()
    print(pressed)
end

function run()
    onClick(obj, "foo")
end
```

You can also read and write files:
```lua
str = readFile("path")

writeFile("path", "hello world")
```

Note: the path is relative to the app folder