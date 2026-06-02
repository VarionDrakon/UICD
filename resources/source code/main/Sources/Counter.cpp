// #include "../Headers/Headers.h"

// #define SENSOR_FIRST_PINOUT 2
// #define SENSOR_SECOND_PINOUT 3

// #define AUTO_STATE_IDLE       0
// #define AUTO_STATE_S1_FIRST   1
// #define AUTO_STATE_S2_FIRST   2
// #define AUTO_STATE_COUNTED    3

// volatile uint8_t autoState = AUTO_STATE_IDLE;

// void sensorsInitialize() {
//   pinMode(SENSOR_FIRST_PINOUT, INPUT_PULLUP);
//   pinMode(SENSOR_SECOND_PINOUT, INPUT_PULLUP);

//   attachInterrupt(digitalPinToInterrupt(SENSOR_FIRST_PINOUT), counterSensorFirst, FALLING);
//   attachInterrupt(digitalPinToInterrupt(SENSOR_SECOND_PINOUT), counterSensorSecondary, FALLING);
// }

// void counterSensorFirst() {
//   if (autoState == AUTO_STATE_S1_FIRST) {
//     autoState = AUTO_STATE_IDLE;
//     return;
//   }
//   if (autoState == AUTO_STATE_IDLE) {
//     if (digitalRead(SENSOR_SECOND_PINOUT) == HIGH) {
//       autoState = AUTO_STATE_S1_FIRST;
//     }
//     return;
//   }
//   else if (autoState == AUTO_STATE_S2_FIRST) {
//     if (digitalRead(SENSOR_SECOND_PINOUT) == LOW) {
//         autoState = AUTO_STATE_COUNTED;
//         counterSensorHandleBackward();
//     }
//   }
// }

// void counterSensorSecondary() {
//   if (autoState == AUTO_STATE_S2_FIRST) {
//     autoState = AUTO_STATE_IDLE;
//     return;
//   }
//   if (autoState == AUTO_STATE_IDLE) {
//     if (digitalRead(SENSOR_FIRST_PINOUT) == HIGH) {
//       autoState = AUTO_STATE_S2_FIRST;
//     }
//     return;
//   }
//   else if (autoState == AUTO_STATE_S1_FIRST) {
//     if (digitalRead(SENSOR_FIRST_PINOUT) == LOW) {
//       autoState = AUTO_STATE_COUNTED;
//       counterSensorHandleForward();
//     }
//   }
// }

// void counterSensorHandleForward() {
//   totalizerDirectValueAdd();
//   totalizerCommonValueAdd();
//   autoState = AUTO_STATE_IDLE;
// }

// void counterSensorHandleBackward() {
//   totalizerReverseValueAdd();
//   totalizerCommonValueAdd();
//   autoState = AUTO_STATE_IDLE;
// }

// Code with helping ai:

#include "../Headers/Headers.h"

#define SENSOR_FIRST_PINOUT  2
#define SENSOR_SECOND_PINOUT 3

#define AUTO_STATE_IDLE      0
#define AUTO_STATE_S1_FIRST  1
#define AUTO_STATE_S2_FIRST  2
#define AUTO_STATE_COUNTED   3

#define MANUAL_STATE_IDLE    0
#define MANUAL_STATE_WAIT_S2 1
#define MANUAL_STATE_WAIT_S1 2
#define MANUAL_STATE_WRONG   3

volatile uint8_t manualState = MANUAL_STATE_IDLE;

volatile uint8_t autoState = AUTO_STATE_IDLE;
volatile uint8_t counterMode = COUNTER_MODE_AUTO;
volatile uint8_t counterManualDirection = COUNTER_DIR_FORWARD;

void counterSensorFirst() {
    if (autoState == AUTO_STATE_S1_FIRST) {
        autoState = AUTO_STATE_IDLE;
        return;
    }
    if (autoState == AUTO_STATE_IDLE) {
        if (digitalRead(SENSOR_SECOND_PINOUT) == HIGH) {
            autoState = AUTO_STATE_S1_FIRST;
        }
        return;
    }
    else if (autoState == AUTO_STATE_S2_FIRST) {
        if (digitalRead(SENSOR_SECOND_PINOUT) == LOW) {
            autoState = AUTO_STATE_COUNTED;
            counterSensorHandleBackward();
        }
    }
}

void counterSensorSecondary() {
    if (autoState == AUTO_STATE_S2_FIRST) {
        autoState = AUTO_STATE_IDLE;
        return;
    }
    if (autoState == AUTO_STATE_IDLE) {
        if (digitalRead(SENSOR_FIRST_PINOUT) == HIGH) {
            autoState = AUTO_STATE_S2_FIRST;
        }
        return;
    }
    else if (autoState == AUTO_STATE_S1_FIRST) {
        if (digitalRead(SENSOR_FIRST_PINOUT) == LOW) {
            autoState = AUTO_STATE_COUNTED;
            counterSensorHandleForward();
        }
    }
}

void counterSensorFirstManual() {
    if (counterManualDirection == COUNTER_DIR_FORWARD)
    {
        switch (manualState) {
            case MANUAL_STATE_IDLE:
                manualState = MANUAL_STATE_WRONG;
                break;

            case MANUAL_STATE_WAIT_S2:
                counterSensorHandleManual();
                manualState = MANUAL_STATE_IDLE;
                break;

            case MANUAL_STATE_WRONG:
                manualState = MANUAL_STATE_IDLE;
                break;

            default:
                manualState = MANUAL_STATE_IDLE;
                break;
        }
    }
    else
    {
        switch (manualState) {
            case MANUAL_STATE_IDLE:
                manualState = MANUAL_STATE_WAIT_S1;
                break;

            case MANUAL_STATE_WAIT_S1:
                break;

            case MANUAL_STATE_WRONG:
                manualState = MANUAL_STATE_IDLE;
                break;

            default:
                manualState = MANUAL_STATE_IDLE;
                break;
        }
    }
}

void counterSensorSecondaryManual() {
    if (counterManualDirection == COUNTER_DIR_FORWARD)
    {
        switch (manualState) {
            case MANUAL_STATE_IDLE:
                manualState = MANUAL_STATE_WAIT_S2;
                break;

            case MANUAL_STATE_WAIT_S2:
                break;

            case MANUAL_STATE_WRONG:
                manualState = MANUAL_STATE_IDLE;
                break;

            default:
                manualState = MANUAL_STATE_IDLE;
                break;
        }
    }
    else
    {
        switch (manualState) {
            case MANUAL_STATE_IDLE:
                manualState = MANUAL_STATE_WRONG;
                break;

            case MANUAL_STATE_WAIT_S1:
                counterSensorHandleManual();
                manualState = MANUAL_STATE_IDLE;
                break;

            case MANUAL_STATE_WRONG:
                manualState = MANUAL_STATE_IDLE;
                break;

            default:
                manualState = MANUAL_STATE_IDLE;
                break;
        }
    }
}

void counterSensorHandleForward() {
    totalizerDirectValueAdd();
    totalizerCommonValueAdd();
    autoState = AUTO_STATE_IDLE;
}

void counterSensorHandleBackward() {
    totalizerReverseValueAdd();
    totalizerCommonValueAdd();
    autoState = AUTO_STATE_IDLE;
}

void counterSensorHandleManual() {
    if (counterManualDirection == COUNTER_DIR_FORWARD) {
        totalizerDirectValueAdd();
    } else {
        totalizerReverseValueAdd();
    }
    totalizerCommonValueAdd();
    autoState = AUTO_STATE_IDLE;
}

void sensorsInitialize() {
    pinMode(SENSOR_FIRST_PINOUT,  INPUT_PULLUP);
    pinMode(SENSOR_SECOND_PINOUT, INPUT_PULLUP);
    counterModeApply();
}

void sensorsAttachAuto() {
    attachInterrupt(digitalPinToInterrupt(SENSOR_FIRST_PINOUT), counterSensorFirst, FALLING);
    attachInterrupt(digitalPinToInterrupt(SENSOR_SECOND_PINOUT), counterSensorSecondary, FALLING);
}

void sensorsDetachAll() {
    detachInterrupt(digitalPinToInterrupt(SENSOR_FIRST_PINOUT));
    detachInterrupt(digitalPinToInterrupt(SENSOR_SECOND_PINOUT));
}

void counterModeApply() {
    sensorsDetachAll();

    autoState = AUTO_STATE_IDLE;
    manualState = MANUAL_STATE_IDLE;

    if (counterMode == COUNTER_MODE_AUTO) {
        sensorsAttachAuto();
    } 
    else {
        attachInterrupt(digitalPinToInterrupt(SENSOR_FIRST_PINOUT), counterSensorFirstManual, FALLING);
        attachInterrupt(digitalPinToInterrupt(SENSOR_SECOND_PINOUT), counterSensorSecondaryManual, FALLING);
    }
}