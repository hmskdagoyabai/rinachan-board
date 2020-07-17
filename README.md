# フラグ説明

``` C
/* flags for emotion control */
int brightness = 30;          //default 30
int mode = 1;                 //1:random 2:manual 0:dokipipo
int random_timemode = 1;      //0:random 1:constant
int now_preset = 0;           //selected preset number
int now_preset_size = FACES;  //selected preset size
int now_manualmode_emote = 0; //selected emotion in manualmode
int automode_preset = 0;      //selected preset in automode (only 0:dokipipo)
int automode_play = 0;        //play start flag
int automode_wait = 0;        //selected emotion in automode
```

## brightness

デフォルトの明るさを指定。SW4で明るさの調整が可能。

## mode

使用中のモード番号を保持するフラグ。
以下の３モードが実装されており、SW4の長押しでモード切り替えを行う。

### ランダムモード

### マニュアルモード

### ドキピポモード（オートモード）

## random_timemode

ランダムモード時の表情切り替え時間設定を保持するフラグ。

### 等間隔モード

等間隔で表情が切り替わるモード。

### ランダム間隔モード

オート感が減るかな？と思って実装した、多少ランダムな間隔で表情が切り替わるモード。いまいち。

## now_preset

使用中の表情プリセット番号を保持するフラグ。
デフォルトは0(全表情)

## now_preset_size

使用中の表情プリセットの要素数を保持するフラグ。

## now_manualmode_emote

マニュアルモードで表示中の表情番号を保持するフラグ。

## automode_preset

オートモードで選択している再生プリセットを保持するフラグ。
死に機能。

## automode_play

オートモードの再生開始フラグ。
死に機能。

## automode_wait

オートモードの再生待ち中に表示しておく表情番号を保持するフラグ。
死に機能。

# 操作説明
