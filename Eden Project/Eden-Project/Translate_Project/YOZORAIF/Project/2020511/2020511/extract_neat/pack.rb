#  encoding : utf-8
$ec_u8_u16 = Encoding::Converter.new("utf-8", "utf-16le")
def pack_arc(arc_dir)
    arcfile = File.open(File.basename(arc_dir) + ".arc", "wb")
    num_of_ele = 0
    pos_of_first_ele = 0
    total_size = 0
    arcfile.syswrite("\0\0\0\0\0\0\0\0")
    Dir.foreach(arc_dir) do |file|
        orifile = File.expand_path(file, arc_dir)
        next if File.directory? orifile
        repfile = $rep_dir + "/" + file
        file = File.exist?(repfile) ? repfile : orifile
        num_of_ele += 1
        arcfile.syswrite([File.size(file)].pack("l"))
        arcfile.syswrite([total_size].pack("l"))
        total_size += File.size(file)
        name = File.basename(file)
        name = $ec_u8_u16.convert(name).force_encoding("ascii-8bit") + "\x00\x00".force_encoding("ascii-8bit")
        arcfile.syswrite(name)
        pos_of_first_ele += name.bytesize + 8
    end
    arcfile.pos = 0
    arcfile.syswrite([num_of_ele].pack("l"))
    arcfile.syswrite([pos_of_first_ele].pack("l"))
    arcfile.pos = pos_of_first_ele + 8
    Dir.foreach(arc_dir) do |file|
        orifile = File.expand_path(file, arc_dir)
        next if File.directory? orifile
        repfile = $rep_dir + "/" + file
        file = File.exist?(repfile) ? repfile : orifile
        arcfile.syswrite(File.open(file, "rb"){|f|f.sysread(File.size(file))})
    end
    arcfile.close
end
$pack_dir = "extract/" + $*[0]
$pack_dir = File.expand_path($pack_dir)
$rep_dir = $pack_dir + "/rep"
pack_arc($pack_dir)



