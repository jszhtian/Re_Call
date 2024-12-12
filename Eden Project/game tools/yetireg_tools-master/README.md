# About

A collection of three scripts for processing scripts from PSP and Xbox 360 versions of a particular "Yeti" and "Regista" game engine.
You may recognize it by the presence of a ``sn.bin`` file and several ``.afs`` archives.


## extract_snbin.py

Uncompress a sn.bin archive into a .ccpak container (arbitrary extension), and splits it into `*.opcodescript` components. The compression algorithm is a widespread LZ77 implementation with N=4096, F=18 (12/4).

`/**************************************************************
 LZSS.C -- A Data Compression Program
***************************************************************
    4/6/1989 Haruhiko Okumura
    Use, distribute, and modify this program freely.
    Please send me your improved versions.
        PC-VAN      SCIENCE
        NIFTY-Serve PAF01022
        CompuServe  74050,1022

**************************************************************/
`

## extjis_psp-cc.py

Parses *.opcodescript into UTF-8 *.txt resources.


## Batch files

A couple of command prompt batch files (`*.cmd`) and PowerShell scripts (`*.ps1`) are provided for convenience.

Drop a copy `sn.bin` into the script directory, and run either of these batch sets.

Note: you may need to edit those files to match your Python 3 and txt2po locations.

