#include "Wait.h"

#include "MainMenu.h"
#include "model/TC_util.h"

/**
 * @brief Construct a new Wait:: Wait object
 *
 * @param tc
 * @param msDelay
 * @param nextState
 */
Wait::Wait(uint16_t msDelay, UIState *nextState) : UIState() {
  endTime = millis() + msDelay;
  this->nextState = nextState;
  if (this->nextState == nullptr) {
    this->nextState = (UIState *)new MainMenu();
  }
}

/**
 * @brief Destroy the Wait:: Wait object
 *
 */
Wait::~Wait() {
  if (nextState) {
    delete nextState;
    nextState = nullptr;
  }
}

bool Wait::isInCalibration() {
  return nextState ? nextState->isInCalibration() : false;
}

void Wait::loop() {
  if (endTime <= millis()) {
    this->setNextState(nextState);
    nextState = nullptr;  // otherwise ~Wait() will destroy the active state
  }
}
