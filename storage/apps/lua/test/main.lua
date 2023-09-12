p = require('paxolib')
state = false;

function run()
   local window = p.window("Mon application")
   local label  = p.button(window, 10, 150, 100, 100)
   local box    = p.box(window, 10, 10, 100, 100)
	
    p.setWindow(window)
	
    upButton = p.button(window, 50, 280, 40, 40)
    upButton:setText("^")
    upButton:onClick(function () box:setY(box:getY() - 10) end)
    
    downButton = p.button(window, 50, 320, 40, 40)
    downButton:setText("V")
    downButton:onClick(function () box:setY(box:getY() + 10) end)
    
    rightButton = p.button(window, 90, 300, 40, 40)
    rightButton:setText(">")
    rightButton:onClick(function () box:setX(box:getX() + 10) end)
    
    leftButton = p.button(window, 10, 300, 40, 40)
    leftButton:setText("<")
    leftButton:onClick(function () box:setX(box:getX() - 10) end)
    
    zoomInButton = p.button(window, 130, 300, 40, 40)
    zoomInButton:setText("+")
    zoomInButton:onClick(function ()
			box:setWidth(box:getWidth() * 2)
			box:setHeight(box:getHeight() * 2)
		end)
    
    zoomOutButton = p.button(window, 170, 300, 40, 40)
    zoomOutButton:setText("-")
    zoomOutButton:onClick(function ()
			box:setWidth(box:getWidth() / 2)
			box:setHeight(box:getHeight() / 2)
		end)

    label:setText(p.readFile("conf.txt"));
    label:setColor(p.COLOR_SUCCESS);
    box:setColor(p.COLOR_SUCCESS)
    box:onClick(function()
    		if(state == false) then
			box:setColor(p.COLOR_BLACK)
			state = true
		else
			box:setColor(p.COLOR_SUCCESS)
			state = false
		end
	end)
end