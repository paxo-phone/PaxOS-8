box = 0;

function run()
    win = Gui("Window")
    label = Gui("Label", 10, 150, 100, 100, win)
    box = Gui("Box", 10, 10, 100, 100, win)

    setText(label, "hello world")
    setColor(box, COLOR_SUCCESS)
end

function pressed()
    setColor(box, COLOR_ERROR)
end