# DIY FC Multitap

## overview

ファミコンのマルチタップの自作です。マルチタップは「2人用」と「4人用」があります。

4人用はArduinoで動きます。「ホリ4プレイヤーズアダプター」の互換品を目指して作りました。ラッチの後、外部コントローラP1/P2、外部コントローラP3/P4、ID1/ID2の順場に転送します。通信1回あたり8bit×3組=24bitです。「FC MULTITAP TEST」と「ダウンタウン熱血行進曲」と「Micro Mages」で動作を確認しています。「ダウンタウン熱血行進曲」は通信方法が特殊です。このプログラムでは起動時に通信テストを行って、CLK2が先に来た場合は専用の処理に分岐させてます。

2人用はマイコン不要で作れます。「Micro Mages」「びっくり熱血新記録」「クレイジー・クライマー」で確認しました。

## files

FCMULTITAP.ino : プログラムです。Arduino IDEで実行します。

FC_TAP_SFC.COMP : 基板CADのデータ(CADLUS X用)

自作の動作テストプログラム「FC MULTITAP TEST」です。ファミコンで動きます。
https://github.com/nicotakuya/FC_MULTITAP_TEST

## Parts

・U1: ATmega168/328(Arduinoを書き込む)   http://akizukidenshi.com/catalog/g/gI-03142/

・X1: 16MHzセラミック発振子   http://akizukidenshi.com/catalog/g/gP-09576/

・CN1: 6pinピンヘッダ 

・CN2: ファミコン拡張端子用コネクタ  延長ケーブルを流用

・CN3/4/5/6: Dsub15pinオス型  http://akizukidenshi.com/catalog/g/gC-00025/

・R1: 10k ohm 抵抗

・R2: 3.3k ohm 抵抗

・C1/2/3: 0.1μF コンデンサ

・LED1: LED 

・Fuse: リセッタブルヒューズ  http://akizukidenshi.com/catalog/g/gP-12911/

## images

![fcmultitap](https://user-images.githubusercontent.com/5597377/131260214-092b5fe4-1d2f-44fa-83aa-7dd78e52f2f9.png)

## Movie

https://www.youtube.com/watch?v=1-oePBSOBHg
