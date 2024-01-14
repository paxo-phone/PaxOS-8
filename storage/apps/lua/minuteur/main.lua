function run()
  myTable = storage:listDir("")

  -- Print each element in the table
  print("--------------------------------")
  for i, value in ipairs(myTable) do
      print(i, value)
  end
  print("--------------------------------")
  storage:mkdir("test2")
end