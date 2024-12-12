This is written by Japanese with ShiftJIS.

###############################################################################
                         E R I N A - L i b r a r y
###############################################################################


１．ご挨拶
　この度は、「ERISA-Library」をダウンロードくださいましてまことに有難うございます。


２．ERISA－Library について
　ERISA-Library は EntisGLS の ESL 層と ERISA 層からなるサブセットライブラリです。同梱のライブラリは、EntisGLS のソースからビルドしていますが、リンクの確認は行っていません。ERISA-Library 単体で利用する際に、ヘッダファイルの不足やリンクエラーなどがありましたらご報告ください。


３．バージョンについて
　ERISA-Library のバージョンは、混乱を防ぐため、元になったソースの EntisGLS のマイナーバージョン + 1.00 で表示しています。


４．ライセンスについて
　ライセンスは EntisGLS とは異なり、旧 ERINA-Library のライセンスとほぼ同じライセンスが有効です。詳細は license.txt をお読みください。
　因みに EntisGLS はシェアウェアですが、ERISA-Library はフリーウェアです。


５．お問い合わせ
　お問い合わせは、
	URL	: http://www.entis.gr.jp/eri/
	E-mail	: eri@entis.jp
までお気軽にどうぞ。


６．履歴
2005/05/23	version 1.07e
　ESLFileObject クラスに 64 ビットファイルサイズに対応でのバグ修正。

2005/05/17	version 1.07d
　ESLFileObject クラスの 64 ビットファイルサイズへの対応と、ERawFile、EMCFile、ERISAArchive クラスの 64 ビットファイルサイズ処理の実装。

2005/05/05	version 1.07c
　NOA 書庫最小展開ライブラリを付加。
　COMPACT_NOA_DECODER マクロを定義してコンパイルし、xerisa_decnoa.lib （デバッグバージョンは xerisa_db_decnoa.lib）とリンクしてください。
　使用可能な関数やクラスのドキュメンテーションは未整備。最低限度の関数とクラスしか使えないと理解してください。
　ERawFile、EMemoryFile、ERISAArchie（デコードのみ）は使用できます。

2004/10/23	version 1.06e
　ライセンス一部改定。
　ヒープ関係を一部改良。

2004/03/13	version 1.04d
　動画圧縮フォーマットを改良。

2004/03/06	version 1.04c
　動画圧縮フォーマットに動き補償を組み込み
　これに伴って画像圧縮・展開関数（ERISAEncoder, ERISADecoder クラス）の
一部に挙動の変化有り。（これ以上の高水準では同じように振舞う）
　圧縮の際に閾値が使われていなかったバグを修正。（汗）
　非可逆圧縮のプリセットパラメータを微調整
　ERI フォーマットに解像度タグを追加。

2004/01/22	version 1.03d
　とりあえずリリース。

