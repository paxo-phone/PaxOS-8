box = 0;
state = false;

function run()
    win = Gui(WINDOW_TYPE)
    label = Gui(BUTTON_TYPE, 10, 150, 100, 100, win)
    
    
    upButton = Gui(BUTTON_TYPE, 50, 280, 40, 40, win)
    setText(upButton, "^")
    onClick(upButton, "moveUp")
    
    downButton = Gui(BUTTON_TYPE, 50, 320, 40, 40, win)
    setText(downButton, "V")
    onClick(downButton, "moveDown")
    
    rightButton = Gui(BUTTON_TYPE, 90, 300, 40, 40, win)
    setText(rightButton, ">")
    onClick(rightButton, "moveRight")
    
    leftButton = Gui(BUTTON_TYPE, 10, 300, 40, 40, win)
    setText(leftButton, "<")
    onClick(leftButton, "moveLeft")
    
    zoomInButton = Gui(BUTTON_TYPE, 130, 300, 40, 40, win)
    setText(zoomInButton, "+")
    onClick(zoomInButton, "zoomIn")
    
    zoomOutButton = Gui(BUTTON_TYPE, 170, 300, 40, 40, win)
    setText(zoomOutButton, "-")
    onClick(zoomOutButton, "zoomOut")
    
    box = Gui(BOX_TYPE, 10, 10, 100, 100, win)

    setText(label, "hello world")
    setColor(box, COLOR_SUCCESS)
    onClick(box, "pressed")
end

function pressed()
    if(state == false) then
        setColor(box, COLOR_ERROR)
        state = true
    else
        setColor(box, COLOR_SUCCESS)
        state = false
    end
end

function moveUp()
    setY(box, getY(box) - 10)
end

function moveDown()
    setY(box, getY(box) + 10)
end

function moveRight()
    setX(box, getX(box) + 10)
end

function moveLeft()
    setX(box, getX(box) - 10)
end

function zoomIn()
    setWidth(box, getWidth(box) * 2)
    setHeight(box, getHeight(box) * 2)
end

function zoomOut()
    setWidth(box, getWidth(box) / 2)
    setHeight(box, getHeight(box) / 2)
end
