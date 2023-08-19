box = 0;
state = false;

function run()
    win = Gui("Window")
    label = Gui("Label", 10, 150, 100, 100, win)
    
    
    upButton = Gui("Label", 50, 280, 40, 40, win)
    setText(upButton, "^")
    onClick(upButton, "moveUp")
    
    downButton = Gui("Label", 50, 320, 40, 40, win)
    setText(downButton, "V")
    onClick(downButton, "moveDown")
    
    rightButton = Gui("Label", 90, 300, 40, 40, win)
    setText(rightButton, ">")
    onClick(rightButton, "moveRight")
    
    leftButton = Gui("Label", 10, 300, 40, 40, win)
    setText(leftButton, "<")
    onClick(leftButton, "moveLeft")
    
    zoomInButton = Gui("Label", 130, 300, 40, 40, win)
    setText(zoomInButton, "+")
    onClick(zoomInButton, "zoomIn")
    
    zoomOutButton = Gui("Label", 170, 300, 40, 40, win)
    setText(zoomOutButton, "-")
    onClick(zoomOutButton, "zoomOut")
    
    box = Gui("Box", 10, 10, 100, 100, win)

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
