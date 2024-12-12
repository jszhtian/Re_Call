#encoding: utf-8
require_relative "rorl"
$pack_dir = "extract/" + $*[0]
$pack_dir = File.expand_path($pack_dir)
$target_dir = $pack_dir + "/rep"
$txt_dir = $pack_dir + "/reptxts"
Dir.mkdir($target_dir) unless File.exist?($target_dir)
$ec_u8_u16 = Encoding::Converter.new("utf-8", "utf-16le", Encoding::Converter::UNIVERSAL_NEWLINE_DECORATOR)
$ec_u16_u8 = Encoding::Converter.new("utf-16le", "utf-8")
$slf = "%LF".force_encoding("ascii-8bit")
$sk = "%K".force_encoding("ascii-8bit")
$sp = "%P".force_encoding("ascii-8bit")
def read_text(data,fp)
    np = data.index("\0", fp)
    [data[fp...np], np]
end
def read_charname(data,fp)
    if data[fp - 8, 2] == "\x15\x00"
        return ["", fp - 7]
    end
    np = data.rindex("%LF", fp)
    [read_text(data, np)[0], np]
end
def pack_ws2_one(ws2file)
    txtfile = $txt_dir + "/" + File.basename(ws2file, ".ws2") + ".txt"
    return if !File.exist?(txtfile)
    ws2filesize = File.size(ws2file)
    ws2data = File.open(ws2file, "rb"){|f|f.sysread(ws2filesize)}
    RORL.ror(ws2data, ws2filesize)
    txtdata = File.open(txtfile, "rb"){|f|f.sysread(File.size(txtfile))}
    if txtdata[0,3] == "\xEF\xBB\xBF".force_encoding("ascii-8bit")
        txtdata = txtdata[3..txtdata.bytesize]
    end
    txtdata = $ec_u8_u16.convert(txtdata).force_encoding("ascii-8bit")
    txtdata = txtdata.split("\x0A\x00").map{|s|$ec_u16_u8.convert(s).force_encoding("ascii-8bit")}
    ptxt = lp = fp = 0
    repdata = "".force_encoding("ascii-8bit")
    while(fp = ws2data.index("char\0", lp))        
        charname, cp = read_charname(ws2data, fp)
        repdata += ws2data[lp...cp]
        if charname != ""
            repdata += $slf
            repdata += txtdata[ptxt]
            ptxt += 1
        end
        repdata += "\0".force_encoding("ascii-8bit")
        fp += 5
        repdata += ws2data[fp - 11, 11]
        repdata += txtdata[ptxt]
        ptxt += 1
        repdata += $sk
        repdata += $sp
        lp = read_text(ws2data, fp)[1]
    end
    repdata += ws2data[lp...ws2data.bytesize]
    RORL.rol(repdata, repdata.bytesize)
    File.open($target_dir + "/" + File.basename(ws2file), "wb"){|f|f.syswrite repdata}
end
Dir.foreach($pack_dir) do |file|
    file = File.expand_path(file, $pack_dir)
    next unless file[-4,4].downcase == ".ws2"
    pack_ws2_one file
end