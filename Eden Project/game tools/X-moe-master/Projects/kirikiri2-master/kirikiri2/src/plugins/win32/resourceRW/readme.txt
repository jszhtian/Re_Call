TITLE: リソース操作プラグイン(experimental)
AUTHOR: miahmie

●これはなに？

EXEやDLLファイルのリソースを操作するプラグインです。

※まだ十分に動作確認されていません！


本プラグインは simplebinder.hpp の使用サンプルです。
simplebinder.hpp については，../00_simplebilder のファイルを確認してください。


●使い方

manual.tjs参照


●コンパイル

vs2005
vs2008
等のプロジェクトを使用してください。

コンパイルには ../tp_stub.* および ../00_simplebilder フォルダ内の
ファイルも必要です。


●ファイルについて

readme.txt	このファイル
Main.cpp	プラグイン本体ソース
lang.inc	LANG_/SUBLANG_登録用マクロ
manual.tjs	擬似コードによるマニュアル
premake4.lua	premake4のプロジェクト生成用定義ファイル
vs20??/		各VS用プロジェクト
gmake/		gcc用プロジェクト


●その他注意事項

・とりあえず書き起こしただけなので，動作確認が不十分です。
・文字列でないリソースに対してResourceReader.readToTextなどしないでください
・MessageTableのリソースを読み書きする場合は自前のパーサ等を作る必要があります
　BinaryStream.dllなどを利用し，WinNT.hなどに定義されている
	MESSAGE_RESOURCE_DATA
	MESSAGE_RESOURCE_BLOCK
	MESSAGE_RESOURCE_ENTRY
　のtypedef/structのフォーマットを自前で読み書きしてください。
　参考；http://www.codeproject.com/Articles/14444/Enumerating-Message-Table-Contents
・アイコンは rtIcon,rtGroupIconリソースをを適切に設定する必要があります
　参考；http://www.codeproject.com/Articles/30644/Replacing-ICON-resources-in-EXE-and-DLL-files

●ライセンス

このプラグインのライセンスは吉里吉里２／吉里吉里Ｚに準拠してください。

