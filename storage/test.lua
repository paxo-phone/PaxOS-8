box = 0;
state = false;

function run()
    win = Gui("Window")
    label = Gui("Label", 10, 150, 100, 100, win)
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