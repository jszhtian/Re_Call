@loadplugin module="wuvorbis.dll"
@iscript
function stopAllVoices()
{
	// 2 �` 6 �̂��ׂĂ̌��ʉ����~����
	for(var i = 0; i <= 2; i++) kag.se[i].stop();
}
function playVoice(buf, storage)
{
	// ���ʉ��o�b�t�@ buf �ɂ� storage ���Đ�����
	// KAG ���X�L�b�v�������̏ꍇ�͏������s��Ȃ�
	if(!kag.skipMode)
	{
		stopAllVoices();
		kag.se[buf].play(%[ storage : storage ]);
	}
}
function createHistoryActionExp(buf, storage)
{
	// ���b�Z�[�W�������N���b�N�����Ƃ��Ɏ��s���� TJS ���𐶐�����
	return "stopAllVoices(), kag.se[" + buf  +"].play(%[ storage : '" + storage + "' ])";
}
@endscript
@macro name=pv
@hact exp="&createHistoryActionExp(mp.b, mp.s)"
@eval exp="playVoice(mp.b, mp.s)"
@endmacro
@macro name=waitvoices
@ws buf=0
@ws buf=1
@ws buf=2
@endmacro
@macro name=sv
@endhact
@waitvoices cond="kag.autoMode"
@eval exp="stopAllVoices()"
@endmacro
*start|�ŏ�
@cm
[pv b=0 s=musicbox]�ق�[l][sv][r]
[pv b=1 s=musicbox]�ق���[l][sv][r]
[pv b=2 s=musicbox]�ق�����[p][sv]
*start2|��
@cm
[pv b=0 s=musicbox.ogg]�ق�[l][sv][r]
[pv b=1 s=musicbox.ogg]�ق���[l][sv][r]
[pv b=2 s=musicbox.ogg]�ق�����[p][sv]
[s]

*test
�ق���
[s]

