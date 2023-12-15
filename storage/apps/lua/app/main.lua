p = require('paxolib')

function show(text)
    print("show: " .. text)
    if (window_show == nil) then
        window_show = p.window("Mon application")
        local label = p.label(window_show, 0, 0, 320, 100)
        label:setFontSize(30)
        label:setText(text)
        label:onClick(function() run() end)
    end
    p.setWindow(window_show)
end

function run()
    --print(p.getWeb("http://example.com"))
    
    if (window_run == nil) then
    print("run: ")
        window_run = p.window("Mon application")
        local label = p.label(window_run, 0, 0, 320, 100)
        local image = p.image(window_run, "logo.png", 0, 100, 42, 42)
        label:setColor(p.COLOR_SUCCESS)
        label:setFontSize(30)

        label:setText("Click here")
        label:onClick(function() show("hello world") end)

        local button = p.button(window_run, 10, 150, 300, 100)
        button:setText("Click here")
    end

    p.setWindow(window_run)
end
