function cli()
    print("clicked")
end

function printTable(tbl)
    for key, value in pairs(tbl) do
        print(key, value)
    end
end

function run()
    window = gui:window()
    label = gui:label(window, 35, 130, 250, 40)
    label:setText("Steve Jobs")
    label:setHorizontalAlignment(CENTER_ALIGNMENT);

    file = storage:file("conf.txt", false)
    file:open()
    label:setText(file:readAll())
    file:close()

    storage:mkdir("conf")
    storage:mkdir("todel")
    storage:rmDir("deleteDir")

    local id = time:setTimeout(function() print ("LUAAAAA") end, 5000)
    time:removeTimeout(id)

    print("Contenu de la table:")
    printTable(time:get("y,s,h,d,mo,y"))
    print("end")

    img = gui:image(window, "launcher.png", 0, 0, 100, 100)
    label:onClick(function() print('ok') img:setX(img:getX() + 10) end)
    gui:setWindow(window)

    if(true) then
        local http = network:HttpClient()
        print(http:get("http://example.com"))
    end

end