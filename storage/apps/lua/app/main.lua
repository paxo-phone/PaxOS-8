
p = require('paxolib')

function run()

    local window = p.window("Mon application")
    local label  = p.button(window, 10, 150, 100, 100)
	label:setText("Hello, world !")
end
