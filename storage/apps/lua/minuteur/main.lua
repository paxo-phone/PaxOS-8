p = require('paxolib')

running = false
chrono = 0

function update()
  if running then
    label:setText(tostring(p.monotonic()-chrono))
  end
end

function run()
  local window = p.window("Mon application")

  label = p.label(window, 0, 50, 190, 100)
  local buttonRun = p.button(window, 10, 150, 300, 100)

  label:setText("0:00")
  print(tostring(p.CENTER_ALIGNMENT))
  label:setHorizontalAlignment(1)
  buttonRun:setText("Run")
  --print(tostring(p.COLOR_LIGHT))

  buttonRun:onClick(function()
    running = true
    chrono = p.monotonic()
  end)

  p.setInterval(function() update() end, 10)

  p.setWindow(window)
end