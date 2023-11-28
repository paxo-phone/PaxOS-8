p = require('paxolib')

function run()
    local window = p.window("Mon application")
    local label = p.label(window, 0, 0, 320, 100)
    label:setColor(p.COLOR_SUCCESS)
    label:setFontSize(30)

    local h = p.getTime()

    for i,v in ipairs(h) do
        print(tostring(v))
    end
    
    p.setInterval(function ()
        label:setText(tostring(p.monotonic()))
        if (label:isFocused()) then
            print("pressed")
        end
    end, 10)

    p.setWindow(window)
end
