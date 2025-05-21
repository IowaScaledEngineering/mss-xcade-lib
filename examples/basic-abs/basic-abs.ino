/*************************************************************************
Title:    Iowa Scaled Engineering MSS-XCADE Basic ABS Example
Authors:  Michael Petersen <railfan@drgw.net>
          Nathan D. Holmes <maverick@drgw.net>
File:     mss-xcade-basic-example.ino
License:  GNU General Public License v3

LICENSE:
    Copyright (C) 2025 Michael Petersen & Nathan Holmes

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 3 of the License, or
    any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

*************************************************************************/

#include "Wire.h"
#include "mss-xcade.h"

WireMux wireMux;
XCade xcade;
SignalMast signalMastA;
SignalMast signalMastB;

#define LOOP_UPDATE_TIME_MS 50

void setup() 
{
  Serial.begin(115200);
  Serial.println("Startup");

  // Start the I2C/Wire interface.  This is how the host processor talks to everything on the xcade board.
  //  Also must be started before setting up the WireMux object - described below - because the mux is also
  //  configured over I2C
  Wire.setPins(XCADE_I2C_SDA, XCADE_I2C_SCL);
  Wire.setClock(100000);
  Wire.begin();

  // The master MSS-XCADE has an I2C/Wire switch on it to allow it to access up to three
  //  additional expansion boards.  Because this exists in sort of a "no man's land" between
  //  the host processor and the xcade peripherals that exist on each and every board, it
  //  becomes it's own object.  By passing that to the xcade, it can then switch the mux to talk
  //  to the correct board

  wireMux.begin(&Wire);

  // Start up the xcade board.  
  //  This is the master board, on the same PCB as the host processor.  If you had additional xcade boards, you would
  //  start them up afterwards using additional xcade objects and their port number, like xcade_expansion1.begin(&wireMux, 1);

  xcade.begin(&wireMux);

  // Configure the signal heads.  If you're already writing your own code, you probably already know what type of
  //  signals you plan to connect.  This is really only needed if you plan to do something other than use the default
  //  setting for 3-light heads and using the board common anode / cathode switch to drive the head polarity.

  // For setSignalHeadType(), the options are SIGNAL_HEAD_THREE_LIGHT (default) or SIGNAL_HEAD_SEARCHLIGHT
  // For setSignalHeadPolarity(), the options are SIGNAL_POL_BOARD_SENSING (default - obey the board switch) or 
  //  SIGNAL_POL_COMMON_ANODE or SIGNAL_POL_COMMON_CATHODE if you want to change any individual head.  Note that changing
  //  this define DOES NOT change the common pin on the signal output.

  // Set signal head B1 explicitly, even though this has no effect since I'm just setting it to the defaults.
  xcade.signals.B1.setSignalHeadType(SIGNAL_HEAD_THREE_LIGHT);
  xcade.signals.B1.setSignalHeadPolarity(SIGNAL_POL_BOARD_SENSING);

  // Create "signal masts" - this allows us to put multiple heads together and then feed in an indication, and let the internal
  //  signal rules drive the aspects based on a 1, 2, or 3-headed signal.  The rules can be overriden if you want different 
  //  aspects shown for a given condition.  However, that's largely an advanced use case, so we'll leave that alone for now.

  // Signal A will be a double-headed signal with A1 on top and A2 on bottom.  Signal B will be a single-headed signal.
  signalMastA.addSignalHeads(&xcade.signals.A1, &xcade.signals.A2);
  signalMastB.addSignalHeads(&xcade.signals.B1);

}

void loop() 
{
  uint32_t currentTime = millis();
  static uint32_t lastReadTime = 0;

	// Because debouncing needs some time between samples, don't go for a hideous update rate
  // 50mS or so between samples does nicely.  That gives a 200mS buffer for changes, which is more
  // than enough for propagation delay
	if (!(((uint32_t)currentTime - lastReadTime) > LOOP_UPDATE_TIME_MS))
    return;

  // Update the last time we ran through the loop to the current time
  lastReadTime = currentTime;

  // This is our example simple track layout - a basic ABS block boundary
  //  
  //                  O--| B1
  //	A ----------- | ------------- B
  //     A1/A2 |--OO
  //	    BD=S1   IR=S2    BD=S3
  //
  //  Sensor S1 - The DCC current detector for block A
  //  Sensor S2 - The IR detector at the block boundary
  //  Sensor S3 - The DCC current detector for block B
  //  Signal A1 - Upper head on signal seen by train leaving the A block for B
  //  Signal A2 - Lower head on signal seen by train leaving the A block for B
  //  Signal B1 - Only signal head on signal seen by train leaving the B block for A
  //  MSS Port A - MSS bus going to the left on the diagram
  //  MSS Port B - MSS bus going to the right on the diagram
  //

  // First, read the input state from the hardware
  xcade.updateInputs();

  // Read sensors
  //  Block A is occupied if either the current block detector on S1 or the IR sensor on S2 is tripped.
  //  Block B is occupied if either the current block detector on S3 or the IR sensor on S2 is tripped.
	bool blockAOccupancy = xcade.gpio.digitalRead(SENSOR_1_PIN) || xcade.gpio.digitalRead(SENSOR_2_PIN);
	bool blockBOccupancy = xcade.gpio.digitalRead(SENSOR_3_PIN) || xcade.gpio.digitalRead(SENSOR_2_PIN);

  // Set Occupancy
  //  Set local occupancy for each port based on the three sensors
  xcade.mssPortA.setLocalOccupancy(blockAOccupancy);
  xcade.mssPortB.setLocalOccupancy(blockBOccupancy);

  // Cascade MSS bus from A->B and B->A
	xcade.mssPortA.cascadeFromPort(xcade.mssPortB);
	xcade.mssPortB.cascadeFromPort(xcade.mssPortA);

  // Generate the signal indications based on each port status.  
  //  This must be done after setting local occupancy, because that can affect signal indications
  signalMastA.setIndication(xcade.mssPortB);
  signalMastB.setIndication(xcade.mssPortA);

  // Now that all state is computed, send the outputs to the hardware
  xcade.updateOutputs();
}
