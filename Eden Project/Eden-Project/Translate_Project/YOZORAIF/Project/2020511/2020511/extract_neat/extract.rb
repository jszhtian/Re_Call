#encoding: utf-8
Dir.mkdir("extract") unless File.exist?("extract")
$ec_u16_u8 = Encoding::Converter.new("utf-16le", "utf-8")
def extract_one(arcfile)
    arc_dir = "extract/" + File.basename(arcfile, "arc")
    Dir.mkdir(arc_dir) unless File.exist?(arc_dir)
    f = File.open(arcfile, "rb")
    nof = f.sysread(4).unpack("l")[0]
    ffo = f.sysread(4).unpack("l")[0] + 8
    cur_pos = 8
    for i in 0...nof
        arcsize = f.sysread(4).unpack("l")[0]
        arcpos = f.sysread(4).unpack("l")[0] + ffo
        arcname = ""
        while (tmp = f.sysread(2)) != "\0\0"
            arcname += tmp
        end
        arcname = $ec_u16_u8.convert(arcname)
        tmp = f.pos
        f.pos = arcpos
        File.open(arc_dir + "/" + arcname,"wb"){|ff| ff.syswrite(f.sysread(arcsize))}
        f.pos = tmp
    end
    f.close
end

extract_one("../" + $*[0] + ".arc")