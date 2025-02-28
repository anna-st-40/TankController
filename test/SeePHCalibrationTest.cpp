#include <Arduino.h>
#include <ArduinoUnitTests.h>

#include "Keypad_TC.h"
#include "LiquidCrystal_TC.h"
#include "PHProbe.h"
#include "Serial_TC.h"
#include "TankController.h"
#include "UIState/SeePHCalibration.h"

unittest(testOutput) {
  // Set up
  TankController* tc = TankController::instance();
  LiquidCrystal_TC* display = LiquidCrystal_TC::instance();
  PHProbe* pHProbe = PHProbe::instance();

  assertEqual("MainMenu", tc->stateName());
  SeePHCalibration* state = new SeePHCalibration();
  tc->setNextState(state, true);
  assertEqual("SeePHCalibration", tc->stateName());
  assertFalse(tc->isInCalibration());

  // Test the output
  tc->loop();
  assertEqual("PH Calibration  ", display->getLines().at(0));
  assertEqual("Requesting...   ", display->getLines().at(1));
  pHProbe->setCalibration(2);
  pHProbe->setPhSlope();
  assertEqual("PH Calibra: 2 pt", display->getLines().at(0));
  assertEqual("99.7,100.3,-0.89", display->getLines().at(1));
  // Return to mainMenu
  Keypad_TC::instance()->_getPuppet()->push_back('D');
  tc->loop();
  assertEqual("MainMenu", tc->stateName());
}

unittest(testTimeout) {
  // Set up
  TankController* tc = TankController::instance();
  LiquidCrystal_TC* display = LiquidCrystal_TC::instance();
  PHProbe* pHProbe = PHProbe::instance();

  assertEqual("MainMenu", tc->stateName());
  SeePHCalibration* state = new SeePHCalibration(true);
  tc->setNextState(state, true);
  assertEqual("SeePHCalibration", tc->stateName());

  pHProbe->setCalibration(2);
  delay(55000);
  tc->loop();
  assertTrue(tc->isInCalibration());
  assertEqual("SeePHCalibration", tc->stateName());
  delay(5000);
  tc->loop();  // Set next state
  tc->loop();  // Loop again to switch states
  assertEqual("MainMenu", tc->stateName());
  assertFalse(tc->isInCalibration());
}

unittest_main()
