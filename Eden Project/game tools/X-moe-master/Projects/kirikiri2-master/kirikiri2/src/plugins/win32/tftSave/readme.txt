Title: レンダリング済みフォントデータ保存プラグイン
Author: miahmie

●これは何？

レンダリング済みフォントデータ(*.tft)を作成するためのプラグインです。
吉里吉里標準の作成ツールが既に存在しますが，
このプラグインを利用すると，Windowsのフォントにとどまらず，
より柔軟なデータを作成できるようになります。


●使い方

manual.tjs を参照してください。


●tftSave.dllによるレンダリング済みフォント作成ツール

サンプルとして，このプラグインを使用したツールを実装しました。

吉里吉里標準ツールのようにフォントが持つ全文字ではなく，
限定された文字に対してレンダリング済みフォントファイルを作成できます。
これにより，一部のフォントのライセンス問題がクリアされます。

対象の文字セットは，あらかじめ定義された組み込みのセットを選択するか，
テキストをパースして使用文字をリストアップする方法で決定されます。

使用方法は，ヘルプボタンを押して表示されるテキストを参照してください。

実行には以下のプラグインが必要です。

・tftSave.dll（このプラグイン）
・fstat.dll
・win32dialog.dll
・csvParser.dll

また，追加スクリプトとして win32dialog.tjs も必要です。

bin/win32/pluginに上記プラグインを置き，
copyfiles.batを実行すると，data以下に必要なファイルがコピーされます。
うまくいかない場合は手動でコピーしてください。

data/以下をリリーサーでEXE込みでパックすると，
ツール単体として使用できるようになります。


●ライセンス

このプラグインのライセンスは吉里吉里本体に準拠してください。

