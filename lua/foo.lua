thing = { name = "n", size = 2 }

function AddStuff (a, b)
  print("[LUA] AddStuff ("..a..", "..b..") called")
  return a + b
end

function DoAThing (a, b)
  print("[LUA] DoAThing ("..a..", "..b..") called")
  c = host_function (a + 10, b * 3)
  return c
end
