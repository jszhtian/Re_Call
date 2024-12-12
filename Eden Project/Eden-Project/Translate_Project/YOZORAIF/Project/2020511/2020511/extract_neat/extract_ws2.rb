#encoding: utf-8
require_relative "rorl"
$extract_dir = "extract/" + $*[0]
$extract_dir = File.expand_path($extract_dir)
$target_dir = $extract_dir + "/oritxts"
Dir.mkdir($target_dir) unless File.exist?($target_dir)
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
def extract_ws2_one(ws2file)
    ws2filesize = File.size(ws2file)
    ws2data = File.open(ws2file, "rb"){|f|f.sysread(ws2filesize)}
    RORL.ror(ws2data, ws2filesize)
    oritxtfile = $target_dir + "/" + File.basename(ws2file, ".ws2") + ".txt"
    fori = File.open(oritxtfile, "wb")
    fp = 0
    while(fp = ws2data.index("char\0", fp))        
        charname = read_charname(ws2data, fp)[0]
        if charname != ""
            fori.syswrite(charname)
            fori.syswrite("\n")
        end
        fp += 5
        fori.syswrite(read_text(ws2data, fp)[0])
        fori.syswrite("\n")
    end
    fori.close
    if(File.size(oritxtfile) == 0)
        File.delete(oritxtfile)
    end
end
Dir.foreach($extract_dir) do |file|
    file = File.expand_path(file, $extract_dir)
    next unless file[-4,4].downcase == ".ws2"
    extract_ws2_one file
end