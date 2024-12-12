require_relative "rorl"
def fix(f, pos, val)
    f.pos = pos
    tmp = [val].pack("l")
    RORL.rol(tmp, 4)
    f.syswrite(tmp)
end
fn = "extract/Rio1/rep/yozora_IF_com_000.ws2"
File.open(fn, "rb+") do |f|
    s = File.size(fn)
    fix(f, s - 0xC4, s - 0x2CEE + 0x2C40)
    fix(f, s - 0xC0, s - 0x2CEE + 0x2C49)
    fix(f, s - 0xB2, s - 0x2CEE + 0x2C49)
end
fn = "extract/Rio1/rep/yozora_IF_com_009d.ws2"
File.open(fn, "rb+") do |f|
    data = f.sysread(File.size(fn))
    RORL.ror(data, File.size(fn))
    cc = "\x00\x00\x80\x3F\x00\x00\x00\x00\xDC\x81".force_encoding("ascii-8bit")
    pp = data.index(cc)
    if pp
        fix(f, pp + 8, pp - 0x81B9 + 0x81DC)
        fix(f, pp + 8 + 0x10, pp - 0x81B9 + 0x81DC)
    end
    cc = "\x00\x00\x80\x3F\x00\x00\x00\x00\x96\xC7".force_encoding("ascii-8bit")
    pp = data.index(cc)
    if pp
        fix(f, pp + 8, pp - 0xC773 + 0xC796)
        fix(f, pp + 8 + 0x10, pp - 0xC773 + 0xC796)
    end
end
fn = "extract/Rio1/rep/yozora_IF_com_009e.ws2"
File.open(fn, "rb+") do |f|
    data = f.sysread(File.size(fn))
    RORL.ror(data, File.size(fn))
    cc = "\x00\x00\x80\x3F\x00\x00\x00\x00\x3E\x80".force_encoding("ascii-8bit")
    pp = data.index(cc)
    if pp
        fix(f, pp + 8, pp - 0x801B + 0x803F)
        fix(f, pp + 8 + 0x10, pp - 0x801B + 0x803F)
    end
    cc = "\x00\x00\x80\x3F\x00\x00\x00\x00\x2F\x14\x01\x00".force_encoding("ascii-8bit")
    pp = data.index(cc)
    if pp
        fix(f, pp + 8, pp - 0x1140C + 0x1142F)
        fix(f, pp + 8 + 0x10, pp - 0x1140C + 0x1142F)
    end
end
fn = "extract/Rio1/rep/yozora_IF_com_010.ws2"
File.open(fn, "rb+") do |f|
    data = f.sysread(File.size(fn))
    RORL.ror(data, File.size(fn))
    cc = "\x00\x00\x80\x3F\x00\x00\x00\x00\x2B\x9E".force_encoding("ascii-8bit")
    pp = data.index(cc)
    if pp
        fix(f, pp + 8, pp - 0x9E08 + 0x9E2B)
        fix(f, pp + 8 + 0x10, pp - 0x9E08 + 0x9E2B)
    end
end
fn = "extract/Rio1/rep/yozora_IF_com_010a.ws2"
File.open(fn, "rb+") do |f|
    data = f.sysread(File.size(fn))
    RORL.ror(data, File.size(fn))
    cc = "\x00\x00\x80\x3F\x00\x00\x00\x00\x04\x59".force_encoding("ascii-8bit")
    pp = data.index(cc)
    if pp
        fix(f, pp + 8, pp - 0x58E1 + 0x5904)
        fix(f, pp + 8 + 0x10, pp - 0x58E1 + 0x5904)
    end
end
fn = "extract/Rio1/rep/yozora_IF_com_010b.ws2"
File.open(fn, "rb+") do |f|
    data = f.sysread(File.size(fn))
    RORL.ror(data, File.size(fn))
    cc = "\x00\x00\x80\x3F\x00\x00\x00\x00\x4F\x53".force_encoding("ascii-8bit")
    pp = data.index(cc)
    if pp
        fix(f, pp + 8, pp - 0x532C + 0x534F)
        fix(f, pp + 8 + 0x10, pp - 0x532C + 0x534F)
    end
end
fn = "extract/Rio1/rep/yozora_IF_com_010c.ws2"
File.open(fn, "rb+") do |f|
    s = File.size(fn)
    fix(f, s - 0xC7, s - 0x40C4 + 0x4013)
    fix(f, s - 0xC3, s - 0x40C4 + 0x401C)
    fix(f, s - 0xB5, s - 0x40C4 + 0x401C)
end
fn = "extract/Rio1/rep/yozora_IF_com_011.ws2"
File.open(fn, "rb+") do |f|
    s = File.size(fn)
    fix(f, s - 0x115, s - 0x3B98 + 0x3A8C)
    fix(f, s - 0x110, s - 0x3B98 + 0x3A97)
    fix(f, s - 0x18 , s - 0x3B98 + 0x3B88)
end