
opendir DH, "ps/reduced";
@files = readdir(DH);
closedir DH;

@mode_list = (
	"ltPsNormal",
	"ltPsAdditive",
	"ltPsSubtractive",
	"ltPsMultiplicative",
	"ltPsScreen",
	"ltPsOverlay",
	"ltPsHardLight",
	"ltPsSoftLight",
	"ltPsColorDodge",
	"ltPsColorDodge5",
	"ltPsColorBurn",
	"ltPsLighten",
	"ltPsDarken",
	"ltPsDifference",
	"ltPsDifference5",
	"ltPsExclusion"
);


open FH , ">index.html" or die;

print FH <<EOF;
<body><html>
<p><img src="grad_base.png"> �̏�� <img src="grad_fore.png"> ��l�X�ȃ��[�h�ŏd�ˍ��킹�����ł��B</p>
<p>A�͑S�s�N�Z���̕s�����x�� 100% �ɂ��ă��C���̕s�����x 100% �ŏd�ˍ��킹�����AB�͑S�s�N�Z���̕s�����x�� 100% �ɂ��ă��C���̕s�����x�� 50% �ɂ��ďd�ˍ��킹�����AC�͑S�s�N�Z���̕s�����x�� 50% �ɂ��ă��C���̕s�����x�� 100% �ɂ��ďd�ˍ��킹�����̂ł��B</p>
<p>�摜�́A���Z���ʂ�������₷���悤��16�F�Ɍ��F���Ă���܂��B</p>

<table>
	<tr>
		<td>
		</td>
		<td>
			A
		</td>
		<td>
			B
		</td>
		<td>
			C
		</td>
	</tr>
EOF

foreach $mode (@mode_list)
{
	print FH "<tr><td style=\"background:#ffd0ff; color:black;\">PhotoShop<br>$mode</td>";
	print FH "<td style=\"background:#ffd0ff; color:black;\">";
	print FH "<img src=\"ps/reduced/$mode.png\">";
	print FH "</td>";
	print FH "<td style=\"background:#ffd0ff; color:black;\">";
	print FH "<img src=\"ps/reduced/${mode}_o.png\">";
	print FH "</td>";
	print FH "<td style=\"background:#ffd0ff; color:black;\">";
	print FH "<img src=\"ps/reduced/${mode}_h.png\">";
	print FH "</td>";
	print FH "</tr>\n";

	print FH "<tr><td style=\"background:#d0ffff; color:black;\">�g���g��<br>$mode</td>";
	print FH "<td style=\"background:#d0ffff; color:black;\">";
	print FH "<img src=\"kirikiri/reduced/$mode.png\">";
	print FH "</td>";
	print FH "<td style=\"background:#d0ffff; color:black;\">";
	print FH "<img src=\"kirikiri/reduced/${mode}_o.png\">";
	print FH "</td>";
	print FH "<td style=\"background:#d0ffff; color:black;\">";
	print FH "<img src=\"kirikiri/reduced/${mode}_h.png\">";
	print FH "</td>";
	print FH "</tr>\n";


}

print FH <<EOF;
</table>
</html></body>
EOF



open FH , ">index2.html" or die;

print FH <<EOF;
<body><html>
<p>A �� <img src="grad_base.png"> �̏�� <img src="grad_fore.png"> ��s�����x 50% �ŏd�ˍ��킹���摜�ł��B</p>
<p>B �� <img src="grad_fore.png"> �̏�� <img src="grad_base.png"> ��s�����x 50% �ŏd�ˍ��킹���摜�ł��B</p>

<table>
	<tr>
		<td>
		</td>
		<td>
			A
		</td>
		<td>
			B
		</td>
	</tr>
EOF

foreach $mode (@mode_list)
{
	print FH "<tr><td style=\"background:#d0ffff; color:black;\">�g���g��<br>$mode</td>";
	print FH "<td style=\"background:#d0ffff; color:black;\">";
	print FH "<img src=\"kirikiri/reduced/${mode}_o.png\">";
	print FH "</td>";
	print FH "<td style=\"background:#d0ffff; color:black;\">";
	print FH "<img src=\"kirikiri/reduced/${mode}_c.png\">";
	print FH "</td>";
	print FH "</tr>\n";


}

print FH <<EOF;
</table>
</html></body>
EOF

