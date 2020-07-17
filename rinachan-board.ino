#include <FastLED.h>
#include "faces.h"

/* PIN */
#define SW1_PIN 5
#define SW2_PIN 4
#define SW3_PIN 3
#define SW4_PIN 2
#define DATA_PIN 10

/* FastLED */
#define RINA_PINK 0xff1493
#define NUM_LEDS 154
CRGB leds[NUM_LEDS];

/* 各種操作用フラグ */
int brightness = 30;            //default 30
int mode = 1;                   //1:random 2:manual 0:dokipipo
int random_timemode = 1;        //0:random 1:constant
int now_preset = 0;             //selected preset number
int now_preset_size = FACES;    //selected preset size
int now_manualmode_emote = 0;   //selected emotion in manualmode
int automode_preset = 0;        //selected preset in automode (only 0:dokipipo)
int automode_playing = 0;       //playing flag
int automode_waiting_emote = 0; //selected emotion in automode

void setup()
{
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
  FastLED.setBrightness(brightness);
  pinMode(SW1_PIN, INPUT_PULLUP);
  pinMode(SW2_PIN, INPUT_PULLUP);
  pinMode(SW3_PIN, INPUT_PULLUP);
  pinMode(SW4_PIN, INPUT_PULLUP);
  randomSeed(analogRead(0)); //乱数初期化
}

void loop()
{
  operate_flags_by_switch();
  delay(50);
  operate_emotion_by_flag();
  delay(50);
}

/* スイッチを読み取りフラグ操作 */
void operate_flags_by_switch()
{
  if (digitalRead(SW1_PIN) == LOW)
  {
    while (digitalRead(SW1_PIN) == LOW)
    {
      //長押ししている間何もしない
    };
    sw1();
  }

  if (digitalRead(SW2_PIN) == LOW)
  {
    while (digitalRead(SW2_PIN) == LOW)
    {
      //長押ししている間何もしない
    }
    sw2();
  }

  if (digitalRead(SW3_PIN) == LOW)
  {
    int t = 0; //長押し判定カウンタ
    while (digitalRead(SW3_PIN) == LOW)
    {
      t++;
      delay(5);
    }
    if (t < 50)
    {
      //0.25秒未満は単押し
      sw3(0);
    }
    else
    {
      //それ以上は長押し
      sw3(1);
    }
  }

  if (digitalRead(SW4_PIN) == LOW)
  {
    int t = 0; //長押し判定カウンタ
    while (digitalRead(SW4_PIN) == LOW)
    {
      t++;
      delay(5);
    }
    if (t < 50)
    {
      //0.25秒未満は単押し
      sw4(0);
    }
    else
    {
      //それ以上は長押し
      sw4(1);
    }
  }
}

/* フラグを読み取り表情操作 */
void operate_emotion_by_flag()
{
  /* mode 1 : ランダム randmode */
  if (mode == 1)
  {
    //選択中のプリセットからランダムに取得し表示
    set_face(now_preset, random(now_preset_size));

    //timemode = 1 : 一定間隔で表情切り替え
    if (random_timemode == 1)
    {
      int count = 0;
      while (1)
      {
        //表示中も操作を受け付ける
        operate_flags_by_switch();
        delay(10);
        count++;
        if (count > 100)
        {
          break;
        }
      }
    }

    //timemode = 0 : ランダム間隔で表情切り替え
    else if (random_timemode == 0)
    {
      int count = 0;
      int wait = random(80) + 80; //0.8~1.6sec
      while (1)
      {
        //表示中も操作を受け付ける
        operate_flags_by_switch();
        delay(10);
        count++;
        if (count > wait)
        {
          break;
        }
      }
    }
  }

  /* mode 2 : 手動 manualmode */
  else if (mode == 2)
  {
    //スイッチで前後 表情切り替え、プリセットも？
    set_face(now_preset, now_manualmode_emote);
  }

  /* mode 3 : プリセット再生 automode */
  else if (mode == 0)
  {
    //再生フラグが立つまで任意の表情で待機
    if (automode_playing == 0)
    {
      set_face(0, automode_waiting_emote);
    }
    //再生フラグが立ったらautomode_presetを再生
    else if (automode_playing == 1)
    {
      //*プリセット再生処理*//
      if (automode_preset == 0)
      {
        doki_pipo();
        automode_playing = 0;
      }
      else if (automode_preset == 1)
      {
        /*再生プリセット足すならここに分岐追加*/
      }
    }
  }
}

/* 選んだ表情に光らせる */
void set_face(int preset_no, int face_no)
{
  //対象の表情の要素数と表情配列を取得
  int size = presets_sizelist[preset_no][face_no];
  int *emotion = presets[preset_no][face_no];

  //一旦全部OFF
  set_black();
  //その後表情配列を読みON
  for (int i = 0; i < size; i++)
  {
    leds[emotion[i]] = RINA_PINK;
  }
  FastLED.show();
}

/* 全off */
void set_black()
{
  for (int i = 0; i < NUM_LEDS; i++)
  {
    leds[i] = 0x000000;
  }
}

/* SW1の挙動 */
void sw1()
{
  //randmode時はプリセットを進める
  if (mode == 1)
  {
    now_preset = (now_preset + 1) % PRESETS;
    now_preset_size = presets_sizes[now_preset];
    now_manualmode_emote = 0;
  }

  //manualmode時は表情を一つすすめる
  else if (mode == 2)
  {
    now_manualmode_emote = (now_manualmode_emote + 1) % now_preset_size;
  }

  //automode時は待機表情を一つすすめる
  else if (mode == 0)
  {
    automode_waiting_emote = (automode_waiting_emote + 1) % FACES;
  }
}

/* SW2の挙動 */
void sw2()
{
  //randmode時はプリセットを戻る
  if (mode == 1)
  {
    now_preset--;
    if (now_preset < 0)
    {
      now_preset = PRESETS - 1;
    }
    now_preset_size = presets_sizes[now_preset];
    now_manualmode_emote = 0;
  }
  //manualmode時は表情を一つ戻す
  else if (mode == 2)
  {
    now_manualmode_emote--;
    if (now_manualmode_emote < 0)
    {
      now_manualmode_emote = now_preset_size - 1;
    }
  }
  //automode時は待機表情をランダムにセット
  else if (mode == 0)
  {
    automode_waiting_emote = random(FACES);
  }
}

/* SW3の挙動 */
void sw3(bool is_long)
{
  //randmode時は間隔を切り替える
  if (mode == 1)
  {
    random_timemode = !random_timemode;
  }
  //manualmode時
  else if (mode == 2)
  {
    //短押し時はプリセットをひとつ進める
    if (!is_long)
    {
      now_preset = (now_preset + 1) % PRESETS;
      now_preset_size = presets_sizes[now_preset];
      now_manualmode_emote = 0;
    }
    //長押し時はプリセットをallに
    else if (is_long)
    {
      now_preset = 0;
      now_preset_size = presets_sizes[now_preset];
      now_manualmode_emote = 0;
    }
  }
  //automode時は再生フラグ切り替え
  else if (mode == 0)
  {
    //誤操作防止のため長押し時のみ反応
    if (is_long)
    {
      automode_playing = 1;
    }
  }
}

/* SW4の挙動 */
void sw4(bool is_long)
{
  /*単押しした時 明るさ+20, max100*/
  if (!is_long)
  {
    brightness = (brightness + 20) % 100;
    FastLED.setBrightness(brightness);
  }
  /*長押しした時 modechange*/
  else if (is_long)
  {
    //モード切り替え
    mode = ((mode + 1) % 3);
    changed();
  }
}

/* ドキピポ（仮） */
void doki_pipo()
{
  //とりあえず適当な中身
  set_face(0, 1);
  delay(1000);
  set_face(0, 2);
  delay(800);
  set_face(0, 3);
  delay(800);
  set_face(0, 4);
  delay(500);
  set_face(0, 2);
  delay(800);
  set_face(0, 3);
}

/* モードチェンジインジケータ */
void changed()
{
  if (mode == 0)
  {
    leds[0] = CRGB(255, 0, 0);
  }
  else if (mode == 1)
  {
    leds[0] = CRGB(0, 255, 0);
  }
  else if (mode == 2)
  {
    leds[0] = CRGB(0, 0, 255);
  }

  FastLED.show();
  delay(200);
}
