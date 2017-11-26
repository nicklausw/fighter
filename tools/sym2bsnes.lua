#!/usr/bin/env lua
-- bass-untech symbol files to bsnes-plus.
assert(#arg == 2)
InFile = assert(io.open(arg[1],"r"))
OutFile = assert(io.open(arg[2],"w"))

OutFile:write("[labels]\n")

while true do
  Line = InFile:read()
  if Line == nil then break end
  ValBank = Line:sub(3,4)
  ValRest = Line:sub(5,8)
  Var = Line:sub(10, #Line)
  OutFile:write(ValBank .. ":" .. ValRest .. " " .. Var .. "\n")
end

InFile:close()
OutFile:close()