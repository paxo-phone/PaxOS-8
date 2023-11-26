
p = require('paxolib')
x = 0

function update()
    print('Updating')
end

function run()

    local window = p.window("Mon application")
    local label = p.label(window, 0, 0, 200, 100)
    label:setColor(p.COLOR_SUCCESS)
    

    p.setInterval(function ()
        label:setText(tostring(p.monotonic()))
    end, 10)

    p.setWindow(window)
end
