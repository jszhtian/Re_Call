Title: layerExSave plugin
Author: わたなべごう

●これはなに？

Layer/Window クラスにTGL5/PNG 形式での保存メソッドを追加するプラグインです。

●PNG保存の制限

PNG保存はlibpngに依存しない実装になっているので、バグがある可能性があります。
また、フォーマットは、常に下記のようになります。

・32bitRGBA固定（透明度あり）
・フィルタなし、
・インターレースなし

フィルタ処理がないので、圧縮率は libpng の実装のものより劣ります。

 ※圧縮率が重要になるような画像は、別の方法での保存を推奨します。

吉里吉里標準のBMP保存だと大きすぎて困るような場面などで使用してください。


タグ情報（offs_*, reso_*, vpag_*）もサポートされますが動作未確認です。

タグ情報辞書に comp_lv を渡すと圧縮率を変更できます。
（0〜9まで：この情報はPNGのチャンクとしては保存されません）


●使い方

manual.tjs 参照

●ライセンス

このプラグインのライセンスは吉里吉里本体に準拠してください。
