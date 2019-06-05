#include <TN_SignalState.h>

TN_Switch btnA;

/* pushbutton connected to this pin. */
const uint8_t SWITCH_PIN = 2;

void setup() {
  Serial.begin(115200);
  pinMode(SWITCH_PIN, INPUT);

  /* If you want to change the time threshold, please set in this way. */
  btnA.set_long_press_time(300);
  btnA.set_double_press_time(150);

  Serial.println(btnA.long_press_time());
  Serial.println(btnA.double_press_time());

}

void loop() {

  /* It goes HIGH when pushbutton is pressed. */
  bool is_high = digitalRead(SWITCH_PIN);
  // Serial.println(is_high);
  
  /* Give a signal to proceed the state transition. */
  TnSignalState btnA_state = btnA.GiveSignal(is_high);

  /* Get switch state. */
  switch(btnA_state){
    case TnSignalState::kIdring:        /* Serial.println("Idring"); */ break;
    case TnSignalState::kSinglePress:   Serial.print("\nSingle Press"); break;
    case TnSignalState::kSingleHold:    /* Serial.print("\nSingle Hold"); */  break;
    case TnSignalState::kSingleRelease: Serial.print("\nSingle Release"); break;
    case TnSignalState::kDoubleIdring:  /* Serial.print("\nDouble Idring");*/ break;
    case TnSignalState::kDoublePress:   Serial.print("\nDouble Press"); break;
    case TnSignalState::kDoubleHold:    /* Serial.print("\nDouble Hold"); */  break;
    case TnSignalState::kDoubleRelease: Serial.print("\nDouble Release"); break;
    case TnSignalState::kLongPress:     Serial.print("\nLong Press"); break;
    case TnSignalState::kLongHold:      /* Serial.print("\nLong Hold"); */  break;
    case TnSignalState::kLongRelease:   Serial.print("\nLong Release"); break;
    case TnSignalState::kLowPassFiltering:   /* Serial.print("\nLow Pass Filtering"); */ break;
    default: Serial.print("\nUNKNOWN"); break;
  }

  /* Get enum index value. */
  uint8_t state_index = static_cast<uint8_t>(btnA_state);
  // Serial.println(state_index);

}


