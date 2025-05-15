/*************************************************************************
Title:    Iowa Scaled Engineering MSS-XCADE Double Track Single Crossover Example
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
SignalMast signalMastC;
SignalMast signalMastD;

#define LOOP_UPDATE_TIME_MS 50
#define I2C_SDA  36
#define I2C_SCL  37

void setup() 
{
  Serial.begin(115200);
  Serial.println("Startup");

  // Start the I2C/Wire interface.  This is how the host processor talks to everything on the xcade board.
  //  Also must be started before setting up the WireMux object - described below - because the mux is also
  //  configured over I2C
  Wire.setPins(I2C_SDA, I2C_SCL);
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

  // Signal A & D will be single-headed signals, since there's only one path through the interlocking for those
  // Signal B & C will be double-headed signals that can display both a normal or a diverging route
  signalMastA.addSignalHeads(&xcade.signals.A1);
  signalMastB.addSignalHeads(&xcade.signals.B1, &xcade.signals.B2);
  signalMastC.addSignalHeads(&xcade.signals.C1, &xcade.signals.C2);
  signalMastD.addSignalHeads(&xcade.signals.D1);

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

  // This is our example track layout - a single crossover between double track

  //     A1 |--O          OO--| C1/C2
  //	    BD=S1   IR=S2    BD=S3
  //	A ----------- | ------------- C
  //                 /  TO=GPIO1
  //                / 
  //      TO=GPIO2 / 
  //	B ----------- | ------------- D
  //  B1/B2 |--OO         O--| D1
  //	    BD=S4   IR=S5    BD=S6

  //  Sensor S1 - The DCC current detector for block A
  //  Sensor S2 - The IR detector at the block boundary between A and C (in the turnout)
  //  Sensor S3 - The DCC current detector for block C
  //  Sensor S4 - The DCC current detector for block B
  //  Sensor S5 - The IR detector at the block boundary between B and D (in the turnout)
  //  Sensor S6 - The DCC current detector for block D
  //  GPIO1     - turnout A/C position (low = normal, high = reverse)
  //  GPIO2     - turnout B/D position (low = normal, high = reverse)


  // First, read the input state from the hardware
  xcade.updateInputs();

  // Read sensors
  bool turnoutACThrown = xcade.gpio.digitalRead(1);
  bool turnoutBDThrown = xcade.gpio.digitalRead(2);
  bool blockAOccupancy = xcade.gpio.digitalRead(SENSOR_1_PIN);
  bool blockBOccupancy = xcade.gpio.digitalRead(SENSOR_4_PIN);
  bool blockCOccupancy = xcade.gpio.digitalRead(SENSOR_3_PIN);
  bool blockDOccupancy = xcade.gpio.digitalRead(SENSOR_6_PIN);
  bool irACOccupancy = xcade.gpio.digitalRead(SENSOR_2_PIN);
  bool irBDOccupancy = xcade.gpio.digitalRead(SENSOR_5_PIN);

	if (!turnoutACThrown && !turnoutBDThrown)
	{
		// If both turnouts are normal (input low), then just cascade through the appropriate signals
    xcade.mssPortA.setLocalOccupancy(blockAOccupancy || irACOccupancy);
    xcade.mssPortC.setLocalOccupancy(blockCOccupancy || irACOccupancy);

    xcade.mssPortB.setLocalOccupancy(blockBOccupancy || irBDOccupancy);
    xcade.mssPortD.setLocalOccupancy(blockDOccupancy || irBDOccupancy);

    // Cascade MSS bus from A->C and C->A
	  xcade.mssPortA.cascadeFromPort(xcade.mssPortC);
	  xcade.mssPortC.cascadeFromPort(xcade.mssPortA);

    // Cascade MSS bus from B->D and D->B
	  xcade.mssPortB.cascadeFromPort(xcade.mssPortD);
	  xcade.mssPortD.cascadeFromPort(xcade.mssPortB);

    signalMastA.setIndication(xcade.mssPortC);
    signalMastB.setIndication(xcade.mssPortD);
    signalMastC.setIndication(xcade.mssPortA);
    signalMastD.setIndication(xcade.mssPortB);

	} else if (turnoutACThrown && turnoutBDThrown) {

  //     A1 |--O          OO--| C1/C2
  //	    BD=S1   IR=S2    BD=S3
  //	A ----------- | ------------- C
  //                 /  TO=GPIO1
  //                / 
  //      TO=GPIO2 / 
  //	B ----------- | ------------- D
  //  B1/B2 |--OO         O--| D1
  //	    BD=S4   IR=S5    BD=S6

		// Turnout is lined against A / D, set them to stop.  
    //  They also don't get the IR sensor for occupancy
    xcade.mssPortA.setLocalOccupancy(blockAOccupancy);
    xcade.mssPortD.setLocalOccupancy(blockDOccupancy);

    // B and C are connected via the crossover and either IR sensor may be tripped
    //  at the boundary to hold down occupancy in both
    xcade.mssPortC.setLocalOccupancy(blockCOccupancy || irACOccupancy || irBDOccupancy);
    xcade.mssPortB.setLocalOccupancy(blockBOccupancy || irACOccupancy || irBDOccupancy);

    // Send stop indications out on A and D
		xcade.mssPortA.cascadeFromIndication(INDICATION_STOP);
		xcade.mssPortD.cascadeFromIndication(INDICATION_STOP);

    // Cascade MSS bus from B->C and C->B AND set the diverging indication out
	  xcade.mssPortB.cascadeFromPort(xcade.mssPortC, true);
	  xcade.mssPortC.cascadeFromPort(xcade.mssPortB, true);

    signalMastA.setIndication(INDICATION_STOP);
    signalMastD.setIndication(INDICATION_STOP);

    signalMastB.setIndication(xcade.mssPortC);
    signalMastC.setIndication(xcade.mssPortB);

	} else if (turnoutACThrown && !turnoutBDThrown) {
    // AC is thrown, but BD is still lined through

		// If both turnouts are normal (input low), then just cascade through the appropriate signals
    xcade.mssPortA.setLocalOccupancy(blockAOccupancy);
    xcade.mssPortC.setLocalOccupancy(blockCOccupancy || irACOccupancy);

    xcade.mssPortB.setLocalOccupancy(blockBOccupancy || irBDOccupancy);
    xcade.mssPortD.setLocalOccupancy(blockDOccupancy || irBDOccupancy);

    // Send stop indications out on A and C, and send diverging on C
		xcade.mssPortA.cascadeFromIndication(INDICATION_STOP);
		xcade.mssPortC.cascadeFromIndication(INDICATION_STOP, true);

    // Cascade MSS bus from B->D and D->B
	  xcade.mssPortB.cascadeFromPort(xcade.mssPortD);
	  xcade.mssPortD.cascadeFromPort(xcade.mssPortB);

    signalMastA.setIndication(INDICATION_STOP);
    signalMastC.setIndication(INDICATION_STOP);

    signalMastB.setIndication(xcade.mssPortD);
    signalMastD.setIndication(xcade.mssPortB);

	} else if (!turnoutACThrown && turnoutBDThrown) {
    // BD is thrown, but AC is still lined through

    //     A1 |--O          OO--| C1/C2
    //	    BD=S1   IR=S2    BD=S3
    //	A ----------- | ------------- C
    //                 /  TO=GPIO1
    //                / 
    //      TO=GPIO2 /
    //	B ----------- | ------------- D
    //  B1/B2 |--OO         O--| D1
    //	    BD=S4   IR=S5    BD=S6

		// If both turnouts are normal (input low), then just cascade through the appropriate signals
    xcade.mssPortA.setLocalOccupancy(blockAOccupancy || irACOccupancy);
    xcade.mssPortC.setLocalOccupancy(blockCOccupancy || irACOccupancy);

    xcade.mssPortB.setLocalOccupancy(blockBOccupancy || irBDOccupancy);
    xcade.mssPortD.setLocalOccupancy(blockDOccupancy);

    // Send stop indications out on A and C, and send diverging on C
		xcade.mssPortB.cascadeFromIndication(INDICATION_STOP, true);
		xcade.mssPortD.cascadeFromIndication(INDICATION_STOP);

    // Cascade MSS bus from B->D and D->B
	  xcade.mssPortA.cascadeFromPort(xcade.mssPortC);
	  xcade.mssPortC.cascadeFromPort(xcade.mssPortA);

    signalMastA.setIndication(xcade.mssPortC);
    signalMastC.setIndication(xcade.mssPortA);

    signalMastB.setIndication(INDICATION_STOP);
    signalMastD.setIndication(INDICATION_STOP);
  }

  // Now that all state is computed, send the outputs to the hardware
  xcade.updateOutputs();
}
