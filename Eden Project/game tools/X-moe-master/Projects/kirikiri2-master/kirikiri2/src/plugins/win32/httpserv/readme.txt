Title: httpserv プラグイン
Author: miahmie

●これは何か？

シンプルな HTTP server プラグインです。

以下のような制限があります。

・ポートを開けるのは 127.0.0.1 のローカルループバックアドレスのみ
（アドレスは http://127.0.0.1〜となります。
  http://localhost〜でも開けなくもありませんが、
  IEだと「イントラネット設定がオフ云々」の通知が出るので注意してください）

・ライブラリ(POCO)が提供しているような細かい制御はできない
（cookie等の機能も提供されているが繋がってないので未対応です。
　また、終了時にコネクションがつながったままだとエラーが発生するので
　keepAliveは常にオフとなります）

・アーカイブ内ファイルのレスポンスは一括でメモリに読み込んで転送するので
  大きなファイルを返すのは厳しい

・色々とテスト不足のため公開するものに組み込むのはお勧めできない
（内製ツールなどでご使用ください）


●使い方

manual.tjs を参照してください。


●コンパイル

コンパイル・リンクには POCO (http://pocoproject.org/）が必要です。
詳細は poco/00-compile-memo.txt を参照してください。


●ライセンス

このプラグインのライセンスは吉里吉里本体に準拠してください。
POCO 自体は Boost Software License 準拠となっています。

