@wc time=20
[delay speed=user]�����\�����x : ���[�U�I��[l]
[delay speed=10]�����\�����x : 10[l]
[delay speed=100]�����\�����x : 100[l]
[nowait]�����\�����x : �m�[�E�F�C�g[endnowait][l]
�����\�����x : 100[l]
[delay speed=user]\
*start
5�b�҂B[l]
@wait time=5000
�҂����B[l]
6�b�J�E���g����B[l]
@resetwait
@wait mode=until time=1000
1 \
@wait mode=until time=2000
2 \
@wait mode=until time=3000
3 \
@wait mode=until time=4000
4 \
@wait mode=until time=5000
5 \
@wait mode=until time=6000
6 \
�҂����B[p]\
@ct
@jump target=*start
