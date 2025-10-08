// volatile bool back = false;             //Triggered sensor 1
// volatile bool forv = false;             //Triggered sensor 2
// volatile bool isIntrTrg_1 = false;      //Triggered sensor 1
// volatile bool isIntrTrg_2 = false;      //Triggered sensor 2
// volatile uint32_t lstIntrTim_1 = 0;     //Timer for sensor 1
// volatile uint32_t lstIntrTim_2 = 0;     //Timer for sensor 2
// const unsigned long debounceDelay = 0; // Debounce delay. OLD varian - if (millis() - secDbn >= 100 && digitalRead(3))

// void frtIntr() {
//   unsigned long currentMillis = millis();
//   if (currentMillis - lstIntrTim_1 >= debounceDelay) {
//     isIntrTrg_1 = true;
//     lstIntrTim_1 = currentMillis;
//     if (isIntrTrg_2) {
//       noInterrupts();
//       forv = true;
//       interrupts();
//     }
//   }
// }

// void scdIntr() {
//   unsigned long currentMillis = millis();
//   if (currentMillis - lstIntrTim_2 >= debounceDelay) {
//     isIntrTrg_2 = true;
//     lstIntrTim_2 = currentMillis;
//     if (isIntrTrg_1) {
//       noInterrupts();
//       back = true;
//       interrupts();
//     }
//   }
// }