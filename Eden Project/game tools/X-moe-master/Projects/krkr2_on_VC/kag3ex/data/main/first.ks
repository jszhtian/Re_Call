@loadplugin module=wuvorbis.dll
@loadplugin module=extrans.dll
@loadplugin module=csvparser.dll
@call storage="world.ks"
@call storage="macro.ks"

; �I�����z�u�̈�̎w��
[selopt normal=select_normal over=select_over left=100 top=80 width=600 height=360]

[iscript]

// �S���������Ƃɂ���t���O XXX �ŏI�I�ɍ폜���邱��!!!
//tf.allseen = true;

// �N���A�t���O����ON XXX �ŏI�I�ɍ폜���邱��!!!
sf.clear = true;

// �o�[�W����
sf.software_version = "1.00";

// ���[�J�[
sf.software_maker = "�Ȃ�";

// �\�t�g��
sf.software_title = "�Ȃ��Q�[��";

// �N
sf.software_year = "2006";

// ���[�h�Ώۃy�[�W
sf.loadpage = 1 if sf.loadpage === void;

/**
 * ���ݒ菉��������
 */
function initialEnvironment()
{
    kag.noeffect     = false;
    kag.textspeed    = kag.chSpeeds.normal / (kag.chSpeeds.slow / 10);
    kag.autospeed    = kag.autoModePageWaits.medium / (kag.autoModePageWaits.slow / 10);
    kag.allskip      = false;
    kag.bgmvolume    = 80;
    kag.sevolume     = 100;
    kag.voicevolume  = 100;
    kag.setVoiceOn('voice0', true);
    kag.setVoiceOn('voice1', true);
    kag.setVoiceOn('voice2', true);
    kag.setVoiceOn('voice3', true);
    kag.setVoiceOn('voice4', true);
    kag.setVoiceOn('voice5', true);
}

// ����N�����͊�������������
if (sf.first_start != 'comp') {
    initialEnvironment();
    sf.first_start = 'comp';
}

[endscript]

[call storage="logo.ks" target="*logo"]
[jump storage=title.ks]
