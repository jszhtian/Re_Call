Title: binaryStream プラグイン
Author: miahmie

●これは何か？

バイナリファイルを扱うためのプラグインです。

※デバッグ不足のため，動作に問題がある可能性があります。


●使い方

manual.tjs を参照してください。


●コンパイルについて

付属のプロジェクトファイルを使用する場合，

..\tp_stub.*
..\ncbind
..\zlib

が必要です。また，DLLの出力先は

..\..\..\..\bin\win32\plugin

となります。
作業コピーを https://sv.kikyou.info/svn/kirikiri2/trunk/kirikiri2 から
チェックアウトしてある場合は問題ありません。


●サンプルスクリプトについて

XP3のアーカイブ内ファイル名一覧を自力で取得するサンプル
xp3index.tjs を参考にしてください。（要追加プラグインmemfile.dll）
# こういうのは吉里吉里本体からアーカイブのファイル名一覧を取得できるように
# 拡張するのが妥当かと思いますが，あくまでサンプルということで...


●ライセンス

このプラグインのライセンスは吉里吉里本体に準拠してください。

