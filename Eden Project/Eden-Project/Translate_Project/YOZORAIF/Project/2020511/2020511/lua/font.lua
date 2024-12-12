-- 
function getDefaultFont()
    return "Tahoma"
end
initFontInformation_ori = initFontInformation
function initFontInformation()
    initFontInformation_ori()
    fontinfo = cclass.CLegacyFontInfo:create()
    fontinfo:remove("system", "Txt01")
    -- default
    -- fontinfo:add("system", "Txt01", "FOT-ロダン Pro DB", 16, 1, 0, 0xFFFFFFFF, 0x001948FF, 1)
    fontinfo:add("system", "Txt01", 
    -- 字体
    "楷体",  -- 注 楷体无法显示 ♪ ・
    -- 字号
    16, 
    -- 羽化像素?
    0,
    -- ?
    0,
    -- messageColor: normal
    0xFFFFFFFF,
    -- messageColor: normalEdge
    0x001948FF,
    -- ?
    1)
end
-- 颜色此处可修改
g_message1Color.normal = 0xFFFFFFFF
g_message1Color.normalEdge = 0x001948FF

