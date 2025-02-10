/*
  ZOA Balancing with External Energy Injection and Solar Power Support
  Demonstrates different conditions:
    - Surplus (green LED)
    - Deficit (red LED)
    - Balanced (white LED)
    - Solar energy used (yellow LED)
*/

struct Unit {
  const char* name;
  float supply;
  float demand;
  float net;  // computed as supply - demand
};

const int NUM_UNITS = 4;
Unit units[NUM_UNITS] = {
{ "Machining",      150.0, 150.0, 0 },
{ "Compressed Air", 120.0, 120.0, 0 },
{ "Welding",        100.0, 100.0, 0 },
{ "Testing",        160.0, 160.0, 0 }
};

// LED pin definitions
const int greenLED  = 2; // Surplus
const int redLED    = 3; // Deficit
const int whiteLED  = 4; // Balanced
const int yellowLED = 5; // Solar injection in use

// Updates the LEDs based on overall surplus/deficit and whether solar injection is used.
// When solar injection is used, only yellow LED is on.
void updateLEDs(float totalSurplus, float totalDeficit, bool solarUsed) {
  // Turn off all LEDs
digitalWrite(greenLED, LOW);
digitalWrite(redLED, LOW);
digitalWrite(whiteLED, LOW);
digitalWrite(yellowLED, LOW);

  if (solarUsed) {
digitalWrite(yellowLED, HIGH);
  }
  else if (totalSurplus>totalDeficit) {
digitalWrite(greenLED, HIGH);
  }
  else if (totalSurplus<totalDeficit) {
digitalWrite(redLED, HIGH);
  }
  else {
digitalWrite(whiteLED, HIGH);
  }
}

// Utility function to print unit details and overall totals.
void printStatus() {
  float totalSurplus = 0.0, totalDeficit = 0.0;
Serial.println("----- Unit Data -----");
  for (int i = 0; i< NUM_UNITS; i++) {
    // Compute net for each unit
    units[i].net = units[i].supply - units[i].demand;
Serial.print(units[i].name);
Serial.print(" | Supply: ");
Serial.print(units[i].supply, 2);
Serial.print(" W, Demand: ");
Serial.print(units[i].demand, 2);
Serial.print(" W, Net: ");
Serial.println(units[i].net, 2);

    if (units[i].net > 0)
totalSurplus += units[i].net;
    else
totalDeficit += -units[i].net;
  }
Serial.print("Total Surplus: ");
Serial.println(totalSurplus, 2);
Serial.print("Total Deficit: ");
Serial.println(totalDeficit, 2);
Serial.println("---------------------");
}

// Scenario 1: Initially balanced; supply equals demand in every unit.
void scenarioBalanced() {
Serial.println("\n=== Scenario 1: Balanced Condition ===");
  // Set each unit's supply equal to its demand:
units[0].supply = 150.0;
units[1].supply = 120.0;
units[2].supply = 100.0;
units[3].supply = 160.0;

printStatus();
  // System balanced so white LED should glow.
updateLEDs(0, 0, false);
Serial.println("Balanced condition: White LED ON");
delay(3000);
}

// Scenario 2: Surplus condition.
void scenarioSurplus() {
Serial.println("\n=== Scenario 2: Surplus Condition ===");
  // Increase supply in one unit to simulate surplus.
units[0].supply = 200.0;  // surplus of 50 (200-150)
units[1].supply = 120.0;
units[2].supply = 100.0;
units[3].supply = 160.0;

printStatus();
  // With surplus, green LED should glow.
updateLEDs(50, 0, false);
Serial.println("Surplus condition: Green LED ON");
delay(3000);
}

// Scenario 3: Deficit condition followed by injection from stored surplus.
void scenarioDeficitInjection() {
Serial.println("\n=== Scenario 3: Deficit Condition (Before Injection) ===");
  // Set supply values to create a deficit:
  // Machining: 150 (balanced), Compressed Air: 100 (deficit of 20),
  // Welding: 80 (deficit of 20), Testing: 150 (deficit of 10) => Total deficit = 50.
units[0].supply = 150.0;
units[1].supply = 100.0;
units[2].supply = 80.0;
units[3].supply = 150.0;

printStatus();
  // Deficit condition: red LED should glow.
updateLEDs(0, 50, false);
Serial.println("Deficit condition: Red LED ON");
delay(3000);

  // Now, simulate using stored surplus energy to balance the deficit.
Serial.println("\n>>> Injecting stored surplus energy to balance the system...");
  for (int i = 0; i< NUM_UNITS; i++) {
    // Injection: Adjust supply to exactly match demand.
    units[i].supply = units[i].demand;
  }

printStatus();
  // System is now balanced.
updateLEDs(0, 0, false);
Serial.println("After injection: Balanced condition -> White LED ON");
delay(3000);
}

// Scenario 4: Extra deficit requiring renewable (solar) energy injection.
void scenarioSolarInjection() {
Serial.println("\n=== Scenario 4: Extra Deficit Requiring Solar Injection ===");
  // Set supply values to create a larger deficit:
  // For example, reduce supply for all units:
  // Machining: 140 (deficit of 10), Compressed Air: 110 (deficit of 10),
  // Welding: 90 (deficit of 10), Testing: 150 (deficit of 10) => Total deficit = 40.
units[0].supply = 140.0;
units[1].supply = 110.0;
units[2].supply = 90.0;
units[3].supply = 150.0;

printStatus();
  // Deficit condition: red LED should glow.
updateLEDs(0, 40, false);
Serial.println("Extra deficit condition: Red LED ON");
delay(3000);

  // Now, since no stored surplus is available, use solar energy.
Serial.println("\n>>> Using solar power to inject energy and balance the system...");

  // For a moment, indicate solar injection by turning on yellow LED.
digitalWrite(greenLED, LOW);
digitalWrite(redLED, LOW);
digitalWrite(whiteLED, LOW);
digitalWrite(yellowLED, HIGH);
Serial.println("Solar energy injection in progress: Yellow LED ON");
delay(2000);

  // After solar injection, adjust supplies to balance the system.
  for (int i = 0; i< NUM_UNITS; i++) {
    units[i].supply = units[i].demand;
  }

printStatus();
  // Turn off yellow and show balanced (white LED).
digitalWrite(yellowLED, LOW);
digitalWrite(whiteLED, HIGH);
Serial.println("After solar injection: Balanced condition -> White LED ON");
delay(3000);
}

void setup() {
Serial.begin(9600);

  // Initialize LED pins
pinMode(greenLED, OUTPUT);
pinMode(redLED, OUTPUT);
pinMode(whiteLED, OUTPUT);
pinMode(yellowLED, OUTPUT);

Serial.println("==== ZOA Balancing Process ====");

  // Run scenarios sequentially
scenarioBalanced();        // 1. Initially balanced (white)
scenarioSurplus();         // 2. Surplus condition (green)
scenarioDeficitInjection(); // 3. Deficit then injection (red then white)
scenarioSolarInjection();   // 4. Extra deficit then solar injection (red then yellow then white)

Serial.println("\n==== End of Process ====");
}

void loop() {
  // Do nothing after executing the scenarios.
}
