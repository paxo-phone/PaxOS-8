function cli()
    print("clicked")
end

function run()
    window = gui:window()
    label = gui:input(window, 0, 0, 320, 240)
    label:setText("coucou")
    label:setX(100)
    img = gui:image(window, "launcher.png", 0, 0, 320, 480)
    gui:setWindow(window)
end