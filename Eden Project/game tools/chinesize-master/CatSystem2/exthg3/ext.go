package main

import (
	"bytes"
	"compress/zlib"
	"encoding/binary"
	"encoding/json"
	"flag"
	"fmt"
	"image"
	"image/color"
	"image/png"
	"io"
	"io/ioutil"
	"log"
	"os"
	"path/filepath"
	"strings"
	"math/bits"
	"encoding/base64"

	"github.com/regomne/bstream"
)

// HG3Header hg3 file header
type HG3Header struct {
	Magic      [4]byte
	HeaderSize uint32
	Info       uint32
}

// ImageBlockInfo hg3 consists of some blocks
type ImageBlockInfo struct {
	NextOffset uint32
	ImageID    uint32
}

// SectionInfo block consists of some sections
type SectionInfo struct {
	Tag        [8]byte
	NextOffset uint32
	SecLength  uint32
}

// SecStdinfo stdinfo section
type SecStdinfo struct {
	Width        uint32
	Height       uint32
	BitDepth     uint32
	OffsetX      int32
	OffsetY      int32
	CanvasWidth  uint32
	CanvasHeight uint32
	Unk1         uint32
	Unk2         uint32
	Unk3         uint32
}

// SecImg img000XX section
type SecImg struct {
	Unk1        uint32
	Height      uint32
	DataCompLen uint32
	DataUncLen  uint32
	CtrlCompLen uint32
	CtrlUncLen  uint32
}

type secRecord struct {
	Tag  string
	Info interface{}
	Data interface{}
}

type blockRecord struct {
	ImageID  uint32
	Sections []secRecord
}

type hg3Record struct {
	ImageBaseName string
	Records       []blockRecord
}

func unpackVal(in byte) (out byte) {
	out = in >> 1
	if (in & 1) != 0 {
		out ^= 0xff
	}
	return
}

var gTbl [4][256]uint32
var gTblInited = false

// code by pmm
func removeAlphaMultiply(r, g, b ,a uint32)(or, og, ob, oa byte){
	if(a == 0xFF){
		return uint8(r), uint8(g), uint8(b), uint8(a)
	}
	if(a == 0){
		return 0, 0, 0, 0
	}
	tr := (r * 0xFFFF) / a
	tg := (g * 0xFFFF) / a
	tb := (b * 0xFFFF) / a
	return uint8(tr >> 8), uint8(tg >> 8), uint8(tb >> 8), uint8(a)
}

func getDibFromImage(img image.Image, depthBytes int) (dib []byte, err error) {
	bounds := img.Bounds()
	width, height := bounds.Max.X, bounds.Max.Y
	dibSize := width * depthBytes * height
	if dibSize < 1024 {
		dibSize = 1024
	}
	dib = make([]byte, dibSize)
	stride := width * depthBytes

	if depthBytes == 4 {
		//fmt.Printf("Start copy\n")
		for y := 0; y < height; y++ {
			line := dib[y*stride:]
			for x := 0; x < width; x++ {
				c := img.At(x, height-y-1)
				
				//nc := color.NRGBAModel.Convert(c)
				// 你以为我是没有预乘的值，RGBA()：我帮你乘好。
				// 我真是谢谢您嘞（cnm）。
				r, g, b, a := removeAlphaMultiply(c.RGBA())
				line[x*4] = byte(b)
				line[x*4+1] = byte(g)
				line[x*4+2] = byte(r)
				line[x*4+3] = byte(a)
				
			}
		}
		//fmt.Printf("End copy\n")
	} else if depthBytes == 3 {
		for y := 0; y < height; y++ {
			line := dib[y*stride:]
			for x := 0; x < width; x++ {
				c := img.At(x, height-y-1)
				///*
				nc := color.NRGBAModel.Convert(c)
				r, g, b, _ := nc.RGBA()
				line[x*3] = byte(b >> 8)
				line[x*3+1] = byte(g >> 8)
				line[x*3+2] = byte(r >> 8)
				//*/
			}
		}
	} else {
		err = fmt.Errorf("only support depth bytes 4 or 3, given %d", depthBytes)
	}
	return
}

func diffImage(dib []byte, width, height, depthBytes uint32) []byte {
	stride := width * depthBytes
	for y := height - 1; y >= 1; y-- {
		line := dib[y*stride:]
		prev := dib[(y-1)*stride:]
		for x := 0; x < int(stride); x++ {
			line[x] -= prev[x]
		}
	}

	for x := stride; x >= depthBytes; x-- {
		dib[x] -= dib[x-depthBytes]
	}
	return dib
}

func packVal(in byte) (out uint32) {
	if in&0x80 == 0 {
		return uint32(in << 1)
	}
	return uint32((in << 1) ^ 0xff)
}

func queryTable(val uint32) (v1, v2, v3, v4 byte) {
	v4 = byte((val & 3) | (val>>6)&0xc | (val>>12)&0x30 | (val>>18)&0xc0)
	v3 = byte(((val >> 2) & 3) | (val>>8)&0xc | (val>>14)&0x30 | (val>>20)&0xc0)
	v2 = byte(((val >> 4) & 3) | (val>>10)&0xc | (val>>16)&0x30 | (val>>22)&0xc0)
	v1 = byte(((val >> 6) & 3) | (val>>12)&0xc | (val>>18)&0x30 | (val>>24)&0xc0)
	return
}

func rgbaCvt(dib []byte, width, height, depthBytes uint32) (out []byte) {
	out = make([]byte, len(dib))
	secLen := width * height * depthBytes / 4
	sec1 := out
	sec2 := out[secLen:]
	sec3 := out[secLen*2:]
	sec4 := out[secLen*3:]
	p := dib
	for i := 0; i < int(secLen); i++ {
		val := packVal(p[0]) | (packVal(p[1]) << 8) | (packVal(p[2]) << 16) | (packVal(p[3]) << 24)
		v1, v2, v3, v4 := queryTable(val)
		sec1[i] = v1
		sec2[i] = v2
		sec3[i] = v3
		sec4[i] = v4
		p = p[4:]
	}
	return
}

func writeBitLen(stm *bstream.BStream, l uint32) {
	if l == 0 {
		panic("error bit len")
	}
	bitCnt := bits.Len32(l)
	for i := bitCnt - 1; i != 0; i-- {
		stm.WriteBit(false)
	}
	l <<= uint(32 - bitCnt)
	for ; bitCnt != 0; bitCnt-- {
		stm.WriteBit(l&0x80000000 != 0)
		l <<= 1
	}
}

func doRLE(dib []byte) (content []byte, ctrl []byte) {
	stm := bstream.NewBStreamWriter(200)
	stm.WriteBit(true)
	writeBitLen(stm, uint32(len(dib)))
	writeBitLen(stm, uint32(len(dib)))
	content = dib
	ctrl = stm.Bytes()
	return
}

func doZlib(in []byte) (out []byte, err error) {
	var bf bytes.Buffer
	w := zlib.NewWriter(&bf)
	_, err = w.Write(in)
	if err != nil {
		return
	}
	w.Close()
	out = bf.Bytes()
	return
}

func readImageToBuff(stdInfo *SecStdinfo, imgInfo *SecImg, fname string) (
	newImgInfo SecImg, out []byte, err error) {
	fs, err := os.Open(fname)
	if err != nil {
		return
	}
	img, _, err := image.Decode(fs)
	fs.Close()
	if err != nil {
		err = fmt.Errorf("can't decode image: %v", err)
		return
	}

	if int(stdInfo.Width) != img.Bounds().Max.X ||
		int(stdInfo.Height) != img.Bounds().Max.Y {
		err = fmt.Errorf("size error, should be: %d*%d", stdInfo.Width, stdInfo.Height)
		return
	}

	dib, err := getDibFromImage(img, int(stdInfo.BitDepth/8))
	if err != nil {
		return
	}

	dib = diffImage(dib, stdInfo.Width, stdInfo.Height, stdInfo.BitDepth/8)
	dib = rgbaCvt(dib, stdInfo.Width, stdInfo.Height, stdInfo.BitDepth/8)
	content, ctrl := doRLE(dib)
	newImgInfo.DataUncLen = uint32(len(content))
	newImgInfo.CtrlUncLen = uint32(len(ctrl))
	content, err = doZlib(content)
	if err != nil {
		err = fmt.Errorf("compress error: %v", err)
		return
	}
	ctrl, err = doZlib(ctrl)
	if err != nil {
		err = fmt.Errorf("compress error: %v", err)
		return
	}
	newImgInfo.DataCompLen = uint32(len(content))
	newImgInfo.CtrlCompLen = uint32(len(ctrl))
	newImgInfo.Height = imgInfo.Height
	newImgInfo.Unk1 = imgInfo.Unk1
	out = append(content, ctrl...)
	return
}

func genBlockWithImage(block *blockRecord, blockID int, pathPrefix string) (out []byte, err error) {
	if block.Sections[0].Tag != "stdinfo" {
		err = fmt.Errorf("section 0 must be stdinfo")
		return
	}
	if len(block.Sections) < 2 {
		err = fmt.Errorf("need at least 2 seciton")
		return
	}
	var stdInfo SecStdinfo
	temp, _ := json.Marshal(block.Sections[0].Info)
	err = json.Unmarshal(temp, &stdInfo)
	if err != nil {
		err = fmt.Errorf("section 0 format error: %v", err)
		return
	}
	var secInfo SectionInfo
	copy(secInfo.Tag[:], []byte("stdinfo\x00"))
	secInfo.SecLength = uint32(binary.Size(stdInfo))
	secInfo.NextOffset = secInfo.SecLength + uint32(binary.Size(secInfo))

	var writer bytes.Buffer
	binary.Write(&writer, binary.LittleEndian, &secInfo)
	binary.Write(&writer, binary.LittleEndian, &stdInfo)
	for idx, sec := range block.Sections[1:] {
		secInfo = SectionInfo{}
		if sec.Tag[0:4] == "img0" && sec.Tag[0:7] != "img_jpg" {
			copy(secInfo.Tag[:], []byte(sec.Tag))
			var imgInfo SecImg
			temp, _ := json.Marshal(sec.Info)
			err = json.Unmarshal(temp, &imgInfo)
			if err != nil {
				err = fmt.Errorf("section %d info error: %v", idx+1, err)
				return
			}
			imgName := fmt.Sprintf(`%s_%d_%03d.png`, pathPrefix, blockID, idx)
			newImgInfo, imgbf, err1 := readImageToBuff(&stdInfo, &imgInfo, imgName)
			if err1 != nil {
				err = fmt.Errorf("reading pic: %s error: %v", imgName, err1)
				return
			}
			secInfo.SecLength = uint32(binary.Size(newImgInfo) + len(imgbf))
			if idx+1 != len(block.Sections)-1 {
				secInfo.NextOffset = secInfo.SecLength + uint32(binary.Size(secInfo))
			}
			binary.Write(&writer, binary.LittleEndian, &secInfo)
			binary.Write(&writer, binary.LittleEndian, &newImgInfo)
			writer.Write(imgbf)
		} else {
			copy(secInfo.Tag[:], []byte(sec.Tag))
			dataString, _ := sec.Data.(string)
			data, err1 := base64.StdEncoding.DecodeString(dataString)
			if err1 != nil {
				err = fmt.Errorf("section %d data format error", idx)
				return
			}
			secInfo.SecLength = uint32(len(data))
			if idx+1 != len(block.Sections)-1 {
				secInfo.NextOffset = secInfo.SecLength + uint32(binary.Size(secInfo))
			}
			binary.Write(&writer, binary.LittleEndian, &secInfo)
			writer.Write(data)
		}
	}
	out = writer.Bytes()
	return
}

func writeBlocks(w io.Writer, blocks []blockRecord, pathPrefix string) (err error) {
	var padding [12]byte
	for idx, block := range blocks {
		var buff []byte
		buff, err = genBlockWithImage(&block, idx, pathPrefix)
		if err != nil {
			err = fmt.Errorf("generate block %d err: %v", idx, err)
			return
		}
		alignedLen := len(buff)
		if alignedLen%8 != 0 {
			alignedLen = alignedLen&(^7) + 8
		}
		if idx == 0 {
			alignedLen += 4
		}
		var blockInfo ImageBlockInfo
		blockInfo.ImageID = block.ImageID
		if idx+1 != len(blocks) {
			blockInfo.NextOffset = uint32(alignedLen) + 8
		}
		binary.Write(w, binary.LittleEndian, &blockInfo)
		w.Write(buff)
		if alignedLen != len(buff) {
			w.Write(padding[0 : alignedLen-len(buff)])
		}
	}
	return
}

func packImagesToHG3(imagePath string, destName string) bool {
	desc, err := ioutil.ReadFile(filepath.Join(imagePath, "info.json"))
	if err != nil {
		fmt.Println("reading json error:", err)
		return false
	}
	var fileRecord hg3Record
	err = json.Unmarshal(desc, &fileRecord)
	if err != nil {
		fmt.Println("json parse error:", err)
		return false
	}
	idx := strings.Index(fileRecord.ImageBaseName, ".")
	prefix := imagePath
	if idx != -1 {
		prefix = filepath.Join(prefix, fileRecord.ImageBaseName[0:idx])
	} else {
		prefix = filepath.Join(prefix, fileRecord.ImageBaseName)
	}

	fs, err := os.Create(destName)
	if err != nil {
		fmt.Println("create hg3 error:", err)
		return false
	}
	defer fs.Close()
	header := HG3Header{[4]byte{'H', 'G', '-', '3'}, 12, 0x300}
	binary.Write(fs, binary.LittleEndian, &header)
	err = writeBlocks(fs, fileRecord.Records, prefix)
	if err != nil {
		fmt.Println("writing block error:", err)
		return false
	}
	return true
}


func getFilterTable() (tbl [][256]uint32) {
	if !gTblInited {
		for i := uint32(0); i < 256; i++ {
			val := i & 0xc0
			val <<= 6
			val |= i & 0x30
			val <<= 6
			val |= i & 0xc
			val <<= 6
			val |= i & 3
			gTbl[3][i] = val
			gTbl[2][i] = val << 2
			gTbl[1][i] = val << 4
			gTbl[0][i] = val << 6
		}
		gTblInited = true
	}
	return gTbl[:]
}

func unDeltaFilter(buff []byte, width, height, depthBytes uint32) (out []byte) {
	if depthBytes != 4 && depthBytes != 3 {
		log.Fatalf("depth bytes error:%d", depthBytes)
	}
	table := getFilterTable()
	secLen := len(buff) / 4
	out = make([]byte, len(buff))
	for i := 0; i < secLen; i++ {
		val := table[0][buff[i]]
		val |= table[1][buff[i+secLen]]
		val |= table[2][buff[i+secLen*2]]
		val |= table[3][buff[i+secLen*3]]

		out[i*4] = unpackVal(byte(val))
		out[i*4+1] = unpackVal(byte(val >> 8))
		out[i*4+2] = unpackVal(byte(val >> 16))
		out[i*4+3] = unpackVal(byte(val >> 24))
	}

	stride := width * depthBytes

	for x := uint32(depthBytes); x < stride; x++ {
		out[x] += out[x-depthBytes]
	}

	for y := uint32(1); y < height; y++ {
		line := out[y*stride:]
		prev := out[(y-1)*stride:]
		for x := uint32(0); x < stride; x++ {
			line[x] += prev[x]
		}
	}
	return
}

func unRLE(buff []byte, ctrl []byte) (out []byte) {
	stm := bstream.NewBStreamReader(ctrl)
	getLen := func() int {
		digits := 0
		val := 0
		for {
			b, err := stm.ReadBit()
			if b {
				break
			}
			if err != nil {
				panic("error")
			}
			digits++
		}
		val = 1 << uint32(digits)
		for digits--; digits >= 0; digits-- {
			b, _ := stm.ReadBit()
			if b {
				val |= (1 << uint32(digits))
			}
		}
		return val
	}

	copyFlag, _ := stm.ReadBit()
	outLen := getLen()
	out = make([]byte, outLen)
	n := 0
	bufPos := 0
	for i := 0; i < outLen; i += n {
		n = getLen()
		if copyFlag {
			copy(out[i:i+n], buff[bufPos:bufPos+n])
			bufPos += n
		}
		copyFlag = !copyFlag
	}
	return
}

func unzlib(reader io.Reader, comprLen, uncomprLen uint32) (out []byte, err error) {
	compData := make([]byte, comprLen)
	_, err = reader.Read(compData)
	if err != nil {
		return
	}
	byReader := bytes.NewReader(compData)
	zReader, _ := zlib.NewReader(byReader)
	out, err = ioutil.ReadAll(zReader)
	if err != nil {
		return
	}
	zReader.Close()
	if int(uncomprLen) != len(out) {
		err = fmt.Errorf("uncompress len error")
	}
	return
}

func dumpObj(name string, buff []byte) {
	fs, err := os.Create(name)
	if err == nil {
		fs.Write(buff)
		fs.Close()
	}
}

func write32DibToPng(dib []byte, width, height int, out io.Writer) (err error) {
	if len(dib) < int(width*height*4) {
		err = fmt.Errorf("dib size error")
		return
	}
	canvas := image.NewNRGBA(image.Rect(0, 0, width, height))
	stride := width * 4
	for y := 0; y < height; y++ {
		line := dib[y*stride:]
		for x := 0; x < width; x++ {
			canvas.Set(x, height-y-1, color.NRGBA{
				B: line[x*4],
				G: line[x*4+1],
				R: line[x*4+2],
				A: line[x*4+3],
			})
		}
	}
	err = png.Encode(out, canvas)
	return
}

func write24DibToPng(dib []byte, width, height int, out io.Writer) (err error) {
	if len(dib) < int(width*height*3) {
		err = fmt.Errorf("dib size error")
		return
	}
	canvas := image.NewNRGBA(image.Rect(0, 0, width, height))
	stride := width * 3
	for y := 0; y < height; y++ {
		line := dib[y*stride:]
		for x := 0; x < width; x++ {
			canvas.Set(x, height-y-1, color.NRGBA{
				B: line[x*3],
				G: line[x*3+1],
				R: line[x*3+2],
				A: 255,
			})
		}
	}
	err = png.Encode(out, canvas)
	return
}

func extractAnImage(reader io.Reader, stdInfo *SecStdinfo, imgInfo *SecImg,
	out io.Writer) (err error) {
	if stdInfo.BitDepth != 32 && stdInfo.BitDepth != 24 {
		err = fmt.Errorf("only support 32/24 bit image, this is %d", stdInfo.BitDepth)
		return
	}
	if stdInfo.Height != imgInfo.Height {
		err = fmt.Errorf("height not fit")
		return
	}
	uncData, err := unzlib(reader, imgInfo.DataCompLen, imgInfo.DataUncLen)
	if err != nil {
		return
	}
	uncCtrl, err := unzlib(reader, imgInfo.CtrlCompLen, imgInfo.CtrlUncLen)
	if err != nil {
		return
	}
	dib1 := unRLE(uncData, uncCtrl)
	dib2 := unDeltaFilter(dib1, stdInfo.Width, stdInfo.Height, stdInfo.BitDepth/8)
	if stdInfo.BitDepth == 32 {
		err = write32DibToPng(dib2, int(stdInfo.Width), int(stdInfo.Height), out)
	} else {
		err = write24DibToPng(dib2, int(stdInfo.Width), int(stdInfo.Height), out)
	}
	return
}

func findStr(bf []byte) string {
	idx := bytes.IndexByte(bf, 0)
	if idx == -1 {
		return string(bf)
	}
	return string(bf[0:idx])
}

func extractABlock(reader io.Reader, blockID uint32, ID uint32, namePrefix string) (
	blockInfo []secRecord, err error) {
	var secInfo SectionInfo
	binary.Read(reader, binary.LittleEndian, &secInfo)
	if !bytes.Equal(secInfo.Tag[:], []byte("stdinfo\x00")) {
		err = fmt.Errorf("first section must be stdinfo")
		return
	}
	blockInfo = make([]secRecord, 0, 3)

	var stdInfo SecStdinfo
	err = binary.Read(reader, binary.LittleEndian, &stdInfo)
	if err != nil {
		return
	}
	blockInfo = append(blockInfo, secRecord{"stdinfo", stdInfo, nil})

	secImgIdx := 0
	for secInfo.NextOffset != 0 {
		err = binary.Read(reader, binary.LittleEndian, &secInfo)
		if err != nil {
			return
		}

		outName := fmt.Sprintf("%s_%d_%03d.png", namePrefix, blockID, secImgIdx)
		//outName2 := fmt.Sprintf("%s_%d_%03d.png", namePrefix, ID, secImgIdx)
		secImgIdx++
		if bytes.Equal(secInfo.Tag[0:4], []byte("img0")) &&
			!bytes.Equal(secInfo.Tag[0:7], []byte("img_jpg")) {
			var imgInfo SecImg
			err = binary.Read(reader, binary.LittleEndian, &imgInfo)
			if err != nil {
				return
			}
			var fs *os.File
			//fs, err = os.Create(outName2)
			fs, err = os.Create(outName)
			//fmt.Printf("ren %s %s\n",outName, outName2)
			if err != nil {
				return
			}
			defer fs.Close()
			err = extractAnImage(reader, &stdInfo, &imgInfo, fs)
			if err != nil {
				return
			}
			blockInfo = append(blockInfo, secRecord{findStr(secInfo.Tag[:]), imgInfo, nil})
		} else {
			data := make([]byte, secInfo.SecLength)
			_, err = reader.Read(data)
			if err != nil {
				return
			}
			blockInfo = append(blockInfo, secRecord{findStr(secInfo.Tag[:]), nil, data})
		}
	}
	return
}

func extractHG3(fname, destDir string) bool {
	fs, err := os.Open(fname)
	if err != nil {
		fmt.Printf("open hg3 file error:%v\n", err)
		return false
	}
	defer fs.Close()
	if _, err = os.Stat(destDir); os.IsNotExist(err) {
		os.MkdirAll(destDir, os.ModePerm)
	}
	basename := filepath.Base(fname)
	idx := strings.Index(basename, ".")
	prefix := destDir
	if idx != -1 {
		prefix = filepath.Join(prefix, basename[0:idx])
	} else {
		prefix = filepath.Join(prefix, basename)
	}

	var fileHeader HG3Header
	binary.Read(fs, binary.LittleEndian, &fileHeader)
	if !bytes.Equal(fileHeader.Magic[:], []byte("HG-3")) ||
		fileHeader.HeaderSize != 12 {
		fmt.Println("not a hg3 file")
		return false
	}
	blocks := make([]blockRecord, 0)

	for {
		curOffset, _ := fs.Seek(0, 1)
		var blockInfo ImageBlockInfo
		err = binary.Read(fs, binary.LittleEndian, &blockInfo)
		if err != nil {
			fmt.Printf("read file error:%v\n", err)
			return false
		}
		var block []secRecord
		//block, err = extractABlock(fs, uint32(len(blocks)), prefix)
		block, err = extractABlock(fs, uint32(len(blocks)), uint32(blockInfo.ImageID), prefix)
		if err != nil {
			fmt.Printf("extract block id:%d error:%v\n", len(blocks), err)
			return false
		}

		blocks = append(blocks, blockRecord{blockInfo.ImageID, block})

		if blockInfo.NextOffset == 0 {
			break
		}
		fs.Seek(curOffset+int64(blockInfo.NextOffset), 0)
	}

	fileRecord := hg3Record{basename, blocks}
	fileInfo, err := json.MarshalIndent(fileRecord, "", "  ")
	if err != nil {
		fmt.Printf("json error:%v\n", err)
		return false
	}
	fsJson, err := os.Create(filepath.Join(destDir, "info.json"))
	if err != nil {
		fmt.Printf("can't create json file:%v\n", err)
		return false
	}
	defer fsJson.Close()
	fsJson.Write(fileInfo)
	return true
}

func main() {
	exitCode := 0
	defer func() {
		os.Exit(exitCode)
	}()
	isExtract := flag.Bool("e", false, "extract hg3")
	isPack := flag.Bool("p", false, "pack hg3")
	inputName := flag.String("i", "", "hg3 file name(with -e) or png image folder(with -p)")
	outputName := flag.String("o", "", "output name")
	flag.Parse()
	if (*isExtract && *isPack) || (!*isExtract && !*isPack) ||
		*inputName == "" {
		flag.Usage()
		return
	}

	output := *outputName
	if output == "" {
		if *isExtract {
			idx := strings.LastIndex(*inputName, ".")
			if idx != -1 && strings.LastIndexAny(*inputName, "\\/") < idx {
				output = (*inputName)[:idx]
			} else {
				fmt.Printf("please specify the output folder!\n")
				return
			}
		} else {
			output = *inputName + ".hg3"
		}
	}

	hasError := false
	if *isExtract {
		fmt.Printf("extract mode\n")
		hasError = extractHG3(*inputName, output)
		fmt.Printf("complete")
	} else {
		fmt.Printf("pack mode\n")
		hasError = packImagesToHG3(*inputName, output)
		fmt.Printf("complete")
	}
	if hasError {
		exitCode = 1
	}
}