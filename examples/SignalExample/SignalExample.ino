#include <TN_SignalState.h>

TN_Signal btnA;

/* 2番ピンにプッシュボダンスイッチなどを接続していると仮定 */
const uint8_t SWITCH_PIN = 2;

void setup() {
  Serial.begin(115200);
  pinMode(SWITCH_PIN, INPUT);

  /* 長押し判定時間やダブルクリックの制限時間の設定方法 */
  btnA.set_long_press_time(300);
  btnA.set_second_press_time(150);
  //btnA.set_chatter_time(0);

  Serial.println(btnA.long_press_time());
  Serial.println(btnA.second_press_time());

}

void loop() {

  /* プッシュボタンが押されているときにHIGH */
  bool is_high = digitalRead(SWITCH_PIN);
  // Serial.println(is_high);

  /* High or Low をbooleanで渡して状態を遷移させます */
  TnSignalState btnA_state = btnA.GiveSignal(is_high);

  /* 状態を取得する例 */
  switch (btnA_state) {
    case TnSignalState::kIdring:        /* Serial.println("Idring"); */ break;
    case TnSignalState::kFirstPress:   Serial.print("\nFirst Press"); break;
    case TnSignalState::kFirstHold:    /* Serial.print("\nFirst Hold"); */  break;
    case TnSignalState::kFirstRelease: Serial.print("\nFirst Release"); break;
    case TnSignalState::kSecondIdring:  /* Serial.print("\nSecond Idring");*/ break;
    case TnSignalState::kSecondPress:   Serial.print("\nSecond Press"); break;
    case TnSignalState::kSecondHold:    /* Serial.print("\nSecond Hold"); */  break;
    case TnSignalState::kSecondRelease: Serial.print("\nSecond Release"); break;
    case TnSignalState::kThirdIdring:  /* Serial.print("\nThird Idring"); */ break;
    case TnSignalState::kLongPress:     Serial.print("\nLong Press"); break;
    case TnSignalState::kLongHold:      /* Serial.print("\nLong Hold"); */  break;
    case TnSignalState::kLongRelease:   Serial.print("\nLong Release"); break;
    case TnSignalState::kLowPassFiltering:   /* Serial.print("\nLow Pass Filtering"); */ break;
    default: Serial.print("\nUNKNOWN"); break;
  }

  /* 状態のインデックス番号を取得する例 */
  uint8_t state_index = static_cast<uint8_t>(btnA_state);
  // Serial.println(state_index);

}
