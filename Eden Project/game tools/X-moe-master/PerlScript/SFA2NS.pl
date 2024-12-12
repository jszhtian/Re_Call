#!/usr/bin/perl
use File::Basename;

$bname = 'tb06g';
my @scr_dir = glob "./scr_SFA/$bname.txt";
open OUT,">./scr_ons/$bname.txt";
foreach $scr_name(@scr_dir)
{
$scr_name =~ s#^.*/(\w+)\.txt#$1#;
open IN,"<./scr_SFA/$scr_name.txt"; 

$dkt=0; #Unknown command count

$jk=0; #if用到的块编号
@js=(); #储存块编号的栈
undef %jh; 

$swt=0; #多重选择用到的块编号
$sws=0; #当前多重选择第几项

while (<IN>){
$flag=0;
$fwc=0; #first word id ;
    s/#/;/g;
    unless(/^\s*;/ or /^\s*$/){ #是注释行则跳过
        if(s/^(s*(?:\[|[\x80-\xfe]+).*)(;*.*)$/$1\\$2/){ #是文本行
            s/\?/？/g if $flag==0;  #修复半角问号
        }
        else{ #是指令行
            $flag ++ if s/(%\w+)\s*=\s*(%\w+|\d+)/mov $1,$2/;
            $flag ++ if s/(%\w+)\s*\+=\s*(%\w+|\d+)/mov $1,$1+$2/;
            $flag ++ if s/::START/*_$scr_name/;
            $flag ++ if s/\^ret/return/;
            $flag ++ if s/::END/;sub_end/;
            $flag ++ if s/\^gsb\((\w+)\)/gosub *_$1/;
            
            $flag ++ if s#btnset\((\d+),\s*("\w+"),\s*(\d+),\s*(\d+)\)#btnset $1,$2,$3,$4#;
            $flag ++ if s#btnset\((\d+),("\w+"),\s*(\d+),\s*(\d+),\s*(".*"),\s*(\d+),\s*(\d+)\)#btnset2 $1,$2,$3,$4,$5,$6,$7#;
            $flag ++ if s#btnon\(ALL\)#;btnrec#;
            
            $flag ++ if s#bgon\("(\w+)"\)#bgon "$1"#;
            $flag ++ if s#bgoff#bgoff#;
            $flag ++ if s#cgon\("(\w+)"\)#cgon "$1"#;
            
            $flag ++ if s#filter\(\d+,\s*("\w+"),\s*(\d+),\s*(\d+)\s*\)#filteron $1,$2,$3#;
            $flag ++ if s#filter\(\d+\)#filteroff#;
            
            $flag ++ if m/^\s*wout/;
            if(/wout\(\s*(\d+)\s*\)/)
            {
                if($1<=2000){$_="wout\r\n";}
                else {$_="woutlong\r\n";}
                $flag ++;
            }
            $flag ++ if s#ud\(0\)#ud_0#;
            $flag ++ if s#ud\(1,\s*(\d+)\)#ud_1 $1#; #淡入淡出效果 例：ud(1, 200)
            $flag ++ if s#ud\(2,\s*"(\w+)",\s*(\d+)\)#ud_2 "$1",$2#; #遮罩效果 例：ud(2, "k_rht00", 2000)
            $flag ++ if s#ud\(38,\s*(\d+)\)#ud_3 $1#;  #Noise out效果
            
            $flag ++ if s#wndoff#hidewnd#;
            
            $flag ++ if s/bgmon\(("\w+")\)/bgmon $1/;
            $flag ++ if s/bgmon\(("\w+"),\s*\d\)/bgm1 $1/;
            $flag ++ if s/bgmoff.*/bgmstop/;
            
            $flag ++ if s/mono\(\s*1\s*\)/monocro #ffe8c8/;
            $flag ++ if s/mono\(\s*4\s*\)/nega 1/;
            $flag ++ if s/mono\(\s*-1\s*\)/monocrooff/;
                
            $flag ++ if s/chon\(\s*(\d+),\s*("\w+"),\s*CENTER\s*\)/chon_c $1,$2/;
            $flag ++ if s/chon\(\s*(\d+),\s*("\w+"),\s*NON\s*\)/chon $1,$2/;
            $flag ++ if s/chon\(\s*(\d+),\s*("\w+"),\s*(\d+),\s*(\d+)\s*\)/chon_d $1,$2,$3,$4/;
            $flag ++ if s/chon\(\s*(\d+),\s*("\w+"),\s*(\d+)\s*\)/chon_d $1,$2,$3,0/;
            $flag ++ if s/chon\(\s*(\d+),\s*("\w+"),\s*CENTER,\s*(\d+)\s*\)/chon_d $1,$2,512,$3/;
            $flag ++ if s/choff\(\s*(-?\d+)\)/choff $1/;
            $flag ++ if s#efex\((\d+),\s*\d+,\s*(\d+),\s*(\d+)\)#efex $1,$2,$3\r\nefexon $1,200#;
            $flag ++ if s#efex\((\d+),\s*12,\s*(\d+),\s*(\d+),\s*("\w+"),\s*(\d+),\s*(\d+)\s*\)#efex2 $1,$2,$3,$4,$5,$6\r\nefexon $1,200#;
            $flag ++ if s#efex\(-1,\s*(\d+),\s*(\d+)\s*\)#;efexon $1,$2#; #由于实现上的困难，改成依次移动
            $flag ++ if s#ef\(\s*(-?\d+),\s*\d+,\s*(\d+),\s*(\d+),\s*(\d+)\)#ef $1,$2,$3,$4#;
 
            $flag ++ if s/cvon\(\s*("\w+")\s*\)/cvon $1/; 
            $flag ++ if s/cv\(""\)/cvoff/;
            $flag ++ if s/seon\(\s*("\w+")\s*\)/seon $1/;
            $flag ++ if s/seon\(\s*("\w+")\s*,\s*\d+\s*,\s*\d+\s*\)/seon $1/; #无论原来播放几遍，都只能做到播一遍。。。
            $flag ++ if s/seoff\(.*\)/seoff/;
            $flag ++ if s/envon\(("\w+")\s*\)/envon $1/;
            $flag ++ if s/envoff\(.*\)/envoff/;
            
            $flag ++ if s/saveenable/saveon/;
            $flag ++ if s/loadenable\r\n//;
            $flag ++ if s#wait\((\d+)\)#wait $1#;
            $flag ++ if s/clrbacklog/lookbackflush/;
            $flag ++ if s/pause/click/;
            
            $flag ++ if s/global\(\s*%(\w+),\s*1\s*\)/mov \%$1,1/;
            $flag ++ if s/global\(\s*%(\w+),\s*-1\s*\)/mov \%reg,\%$1/;
            $flag ++ if s/backtitle/return/;
            $flag ++ if s/^:(\w+)/*$1/; #跳转标签
            
            if(/float\(\s*(\d+)\s*\)/){
                $flag ++;
                $_ = "gosub *save\n" if $1==1;
                $_ = "gosub *load\n" if $1==2;
                $_ = "gosub *cgmode\n" if $1==3;
                $_ = "gosub *hswitch\n" if $1==4;
                $_ = "gosub *bgmmode\n" if $1==5;
                $_ = "gosub *qsave\n" if $1==7;
                $_ = "gosub *qload\n" if $1==8;
                $_ = "gosub *config\n" if $1==9;
            }
            if(/^\s*\^/){
                $flag++;
                $flagj=1;
                $flagj ++ if s/\^jp\(\s*(\w+)\s*\)/goto *$1/; #跳转至标签
                if(/\^if\((.+)\)/){ #if
                    $jk++; #jk为if用到的块编号
                    push @js,$jk; #js为储存块编号的栈
                    #%jh为块编号当前次数
                    $jn=$jh{$jk}=1;
                    $_ = "if $1 goto *jump_${scr_name}_${jk}_${jn}_s\ngoto *jump_${scr_name}_${jk}_${jn}_e\n*jump_${scr_name}_${jk}_${jn}_s\n";
                }
                elsif(/\^else/){ #else
                    $jm=$js[-1];
                    $jn=$jh{$jm};
                    $_ = "goto *jump_${scr_name}_${jm}_end\n*jump_${scr_name}_${jm}_${jn}_e\n";
                    $jh{$jm}++;
                    $jn=$jh{$jm};
                    $_ = $_ . "*jump_${scr_name}_${jm}_${jn}_s\n";
                }
                elsif(/\^elif\((.+)\)/){ #elseif
                    $jm=$js[-1];
                    $jn=$jh{$jm};
                    $_ = "goto *jump_${scr_name}_${jm}_end\n*jump_${scr_name}_${jm}_${jn}_e\n";
                    $jh{$jm}++;
                    $jn=$jh{$jm};
                    $_ = $_ . "if $1 goto *jump_${scr_name}_${jm}_${jn}_s\ngoto *jump_${scr_name}_${jm}_${jn}_e\n*jump_${scr_name}_${jm}_${jn}_s\n";
                }
                elsif(/\^endi/){ #endif
                    $jm=pop @js;
                    $jn=$jh{$jm};
                    $_ = "*jump_${scr_name}_${jm}_${jn}_e\n*jump_${scr_name}_${jm}_end\n";
                }
                elsif(s/\^sw\((%\w+)\)/mov %swtmp,$1/){ #switch
                    $swt++;
                    $sws=1;
                }
                elsif(/\^case\((\d+)\)/){ #case
                    if($sws!=1){
                        $_ = "goto *sw_${scr_name}_${swt}_end\n*sw_${scr_name}_${swt}_${sws}_e\n";
                    }else{
                        $_ = "";
                    }
                    $sws++;
                    $_ = $_ . "if %swtmp=$1 goto *sw_${scr_name}_${swt}_${sws}_s\ngoto *sw_${scr_name}_${swt}_${sws}_e\n*sw_${scr_name}_${swt}_${sws}_s\n"
                }
                elsif(/\^df/){ #default
                    $_ = "goto *sw_${scr_name}_${swt}_end\n*sw_${scr_name}_${swt}_${sws}_e\n";
                    $sws++;
                    $_ = $_ . "*sw_${scr_name}_${swt}_${sws}_s\n";
                }
                elsif(/\^ends/){ #end switch
                    $_ = "*sw_${scr_name}_${swt}_${sws}_e\n*sw_${scr_name}_${swt}_end\n";
                }
                elsif($flagj==1){
                    $flagj=0;
                }
                $flag=0 if $flagj==0;
            }
            $flag ++ if s/^(\s*memo\("\s*"\))/;<ignore>$1/; #忽略的指令
            $flag ++ if s/^(\s*anmnum\(.*\))/;<ignore>$1/;
            $flag ++ if s/^(\s*fall\(.*\))/;<ignore>$1/;
            $flag ++ if s/^(\s*raster\(.*\))/;<ignore>$1/;
            $flag ++ if s/^(\s*ch\()/;<ignore>$1/;
            if($flag==0){
                $dkt++ if s/^(\s*[!-:<-~].*$)/;<unknown>$1/;
            }
         }
    }
#   s///;    
    print OUT;
}
close IN;
print "$scr_name: $dkt lines unknown\r\n";
}
close OUT;