Title: Squirrel Plugin
Author: �킽�Ȃׂ���

������͂ȂɁH

Squirrel (http://squirrel-lang.org/) �̋g���g���o�C���h�ł��B

���g�p���@

Squirrel �̌Ăяo���@�\�� Scripts �Ɋg������܂�

------------------------------------------------------------------------------
Scripts.execSQ("local a=10; return a;");	// Squirrel �X�N���v�g�̎��s
Scripts.execStorageSQ("filename.nut");		// Squirrel �X�N���v�g�t�@�C���̎��s
------------------------------------------------------------------------------

�ETJS2 �̃O���[�o����Ԃ� Squirrel ������ "::krkr" �ŎQ�Ƃł��܂��B

  �v���~�e�B�u�l�̎Q�ƂƁA�I�u�W�F�N�g�ɑ΂���
�@get/set/call ����������Ă��܂��B
  ��call �̓N���X����̃C���X�^���X�̐������\�ł��B

�ESquirrel �̃O���[�o����Ԃ� TJS2 ������ "sqglobal" �ŎQ�Ƃł��܂��B

�@�v���~�e�B�u�l�̎Q�ƂƁA
  �I�u�W�F�N�g�ɑ΂��� PropGet/PropSet/FuncCall/CreateNew
  ����������Ă��܂��B�Ȃ� incontextof �͈Ӗ��������܂���B

���R���p�C��

�v���O�C���̃R���p�C���ɂ͈ȉ��̃��C�u�������K�v�ɂȂ�܂��B
�t�H���_�ɓW�J���Ă��������B���t�H���_���� sqplus �Ƀ��l�[�����Ă�������

�Esqplus (Squirrel �� C++ �o�C���h)

�@http://wiki.squirrel-lang.org/default.aspx/SquirrelWiki/SqPlus.html

�@�J������ SQUIRREL2_1_1_sqplus25.zip ���g���č�Ƃ��Ă��܂��B

�����C�Z���X

Squirrel �� ������ zlib/libpng�X�^�C�����C�Z���X�ł��B

Copyright (c) 2003-2007 Alberto Demichelis

This software is provided 'as-is', without any 
express or implied warranty. In no event will the 
authors be held liable for any damages arising from 
the use of this software.

Permission is granted to anyone to use this software 
for any purpose, including commercial applications, 
and to alter it and redistribute it freely, subject 
to the following restrictions:

		1. The origin of this software must not be 
		misrepresented; you must not claim that 
		you wrote the original software. If you 
		use this software in a product, an 
		acknowledgment in the product 
		documentation would be appreciated but is 
		not required.

		2. Altered source versions must be plainly 
		marked as such, and must not be 
		misrepresented as being the original 
		software.

		3. This notice may not be removed or 
		altered from any source distribution.
-----------------------------------------------------
END OF COPYRIGHT

���̃v���O�C�����̂̃��C�Z���X�͋g���g���{�̂ɏ������Ă��������B
