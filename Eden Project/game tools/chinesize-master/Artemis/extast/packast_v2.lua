#!/usr/bin/env lua5.3
local inspect = require('inspect')

local function mysplit(str, delimiter)
  local result = { }
  local from  = 1
  local delim_from, delim_to = string.find( str, delimiter, from  )
  while delim_from do
    table.insert( result, string.sub( str, from , delim_from-1 ) )
    from  = delim_to + 1
    delim_from, delim_to = string.find( str, delimiter, from  )
  end
  table.insert( result, string.sub( str, from  ) )
  return result
end

function parse_tag(s)
  if s:sub(1,1) == '/' then
    return {s}
  else
    if s:sub(1,11) ~= 'ruby text="' or s:sub(-1) ~= '"' then
      error('now only support "ruby" tag with "text" attr')
    end
    local obj = {}
    obj[1] = 'ruby'
    obj.text = s:sub(12,-2)
    return obj
  end
end

function pack_tag_object(s)
  local i = s:find(' ')
  if i == nil then
    return {s}
  end
  local obj = {s:sub(1, i - 1)}
  local kvs = mysplit(s:sub(i + 1), ' ')
  for i,v in ipairs(kvs) do
    i = v:find('=')
    if i == nil then
      error('error in parsing tag = ')
    end
    local real_val = v:sub(i+1)
    if real_val:sub(1,1) ~= '"' or real_val:sub(-1) ~= '"' then
      error('error in parsing tag "')
    end
    obj[v:sub(1,i-1)] = real_val:sub(2, -2)
  end
  return obj
end

function pack_dialog_text(text)
  local obj = {}
  local st = 1
  local i = 1
  while i <= #text do
    if text:sub(i,i) == '[' then
      if i ~= st then
        table.insert(obj, text:sub(st, i-1))
      end
      local pos = text:find('%]', i)
      if pos == nil then
        error("can't find ']' in: "..text)
      end
      table.insert(obj, pack_tag_object(text:sub(i+1, pos-1)))
      i = pos+1
      st = i
    else
      i = i+1
    end
  end
  if st <= #text then
    table.insert(obj, text:sub(st))
  end
  return obj
end

local ast_sel_idx = 1
local function next_sel_node(ast)
  while true do
    local n = ast[ast_sel_idx]
    if n == nil then
      return nil, nil
    end
    ast_sel_idx = ast_sel_idx + 1
    for i,v in ipairs(n) do
      if v[1] == 'select' and v.ja ~= nil then
        return n, i
      end
    end
  end
end

local function pack_text_to_ast(ast, ls)
  local ls_idx = 1
  local function next_line()
    local l = ls[ls_idx]
    if l == nil then
      error('line number too less')
    end
    ls_idx = ls_idx + 1
    return l
  end

  for i,v in ipairs(ast.text) do
    local sel = v.select
    if sel ~= nil then
      local sel_ls = {}
      for i = 1, #sel.ja do
        local l = next_line()
        sel.ja[i] = l
        table.insert(sel_ls, l)
      end
      local snode, i0 = next_sel_node(ast)
      for i = 1, #sel_ls do
        local v = snode[i0 - 1 + i]
        if v[1] == 'select' and v.ja ~= nil then
          v.ja = sel_ls[i]
        else
          print('error in select node:', inspect(snode))
          error('eeeee')
        end
      end
      local mwnode = snode[#sel_ls+1+i0-1]
      if mwnode ~= nil and mwnode.mw ~= nil then
        mwnode.mw = next_line()
      end
      goto cont
    end
    local name = v.name
    if name ~= nil then
      name.ja = next_line()
    end
    local ja = v.ja
    if #ja ~= 1 then
      print('error in ja field:', inspect(v))
    end
    ja[1] = pack_dialog_text(next_line())
    ::cont::
  end
end

local function read_ast_file(fname)
  local ast_func, err = loadfile(fname, 't')
  if ast_func == nil then
    print("err when loading", fname, ":", err)
    return nil
  end
  ast = nil
  ast_func()
  if ast == nil then
    print(fname, "'s format may be wrong: there is not 'ast' variable")
    return nil
  end
  return ast
end

local function read_txt_lines(fname)
  local fs = io.open(fname, 'rb')
  if fs==nil then
    print("can't open", fname)
    return nil
  end
  local fsize = fs:seek('end')
  fs:seek('set')
  local stm = fs:read(fsize)
  fs:close()
  local b1,b2,b3 = string.byte(stm, 1, 3)
  if (b1 == 0xfe and b2 ~= 0xff) or (b1 == 0xff and b2 == 0xfe) then
    print('only support utf-8 encoded text file:', fname)
    return nil
  end
  local s
  if b1==0xef and b2==0xbb and b3==0xbf then
    s = string.sub(stm,4)
  else
    s = stm
  end
  local ls = mysplit(s, '\r\n')
  return ls
end

local function packast(oriast, txt, newast)
  local ast = read_ast_file(oriast)
  if ast == nil then
    return
  end

  local nls = read_txt_lines(txt)
  if nls == nil then
    return
  end

  local ret, err = pcall(pack_text_to_ast, ast, nls)
  if not ret then
    print('error occured while packing', txt, ':', tostring(err))
    return
  end
  
  local fs = io.open(newast, 'wb')
  if fs == nil then
    print("can't create file:", newast)
    return
  end
  fs:write('ast = ')
  fs:write(inspect(ast))
  fs:close()
end

if arg[1] == nil or arg[2] == nil or arg[3] == nil then
  print('usage: extast.lua <astfile> <input_txt> <output_astfile>')
  return
end

packast(arg[1], arg[2], arg[3])