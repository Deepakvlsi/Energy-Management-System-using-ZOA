# Energy-Management-System-using-ZOA
This study employs the Zebra Optimization Algorithm (ZOA) for dynamic power allocation in manufacturing plants, optimizing energy efficiency and renewable utilization. It minimizes losses, balances supply-demand and enhances sustainability.

# 1. Introduction
In modern industrial manufacturing plants, energy management plays a crucial role in ensuring efficient operations and cost optimization. With the integration of multiple renewable energy sources such as solar, windand hydro, a central power station is established to manage and distribute power effectively. However, the dynamic nature of energy generation and fluctuating power demands across various units necessitate an intelligent allocation strategy. To ensure that the available power meets demand optimally, advanced optimization techniques must be employed. This study explores the use of the Zebra Optimization Algorithm (ZOA) for efficient power splitting in a manufacturing plant's central power station. In cases where there is a power deficit, the system automatically supplements the shortfall by drawing additional energy from solar power, ensuring that the overall energy supply remains balanced and the plant operates continuously without interruption.

# 2. Overview
# Problem Statement
Manufacturing plants rely on multiple energy sources, including renewable energy, which introduces variability in power generation. The central power station must distribute this energy efficiently among different units based on real-time demand. Traditional load allocation methods often fail to optimize power distribution, leading to inefficiencies such as overloading, underutilization, and energy wastage. To address this challenge, an intelligent and adaptive power allocation strategy is required to ensure:
•	Optimal utilization of available renewable energy sources.
•	Dynamic power distribution based on real-time demand.
•	Minimization of energy losses and maximization of efficiency.
•	A balance between supply and demand without overloading any unit.
# Proposed Solution
To achieve efficient power distribution, this study proposes the implementation of the Zebra Optimization Algorithm (ZOA). Inspired by the social behavior and movement patterns of zebras, ZOA is a metaheuristic optimization technique that efficiently searches for optimal solutions in complex problem spaces. In this context, ZOA will be utilized to:
•	Allocate power dynamically to different units based on real-time demand variations.
•	Optimize energy distribution to minimize power losses and enhance efficiency.
•	Adaptively balance supply and demand, ensuring uninterrupted manufacturing operations.
•	Improve the overall sustainability of the manufacturing plant by maximizing renewable energy utilization.
By leveraging ZOA, the central power station can enhance operational efficiency, reduce energy waste, and achieve an intelligent energy management system that supports sustainable industrial practices.

# Block diagram
![image](https://github.com/user-attachments/assets/433d6f5c-e437-4a21-a26c-6efd36b1e347)



# Key Features
1.	Demand-Based Power Allocation
2.	Multi-Source Renewable Energy Integration
3.	Energy Efficiency Optimization
4.	Scalability and Flexibility
5.	Predictive Analytics for Demand Forecasting
6.	User-Friendly Monitoring and Control Interface
   
# 3. Components Required (Bill of Materials)
<img width="257" alt="image" src="https://github.com/user-attachments/assets/b2d939fa-5885-43cf-a57f-2eb4e28f7717" />

Total Estimated Budget: Rs. 1280

# 4. Pin Connection Table
<img width="247" alt="image" src="https://github.com/user-attachments/assets/2509297d-b1d7-427f-a61e-2ce6d87501f2" />


# 5. Working Code

```c
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
```
# 6. Explanation of Formulas and Code
 

![image](https://github.com/user-attachments/assets/c35326f0-97c2-4b6f-a811-61fc5ea929d0)


Energy Imbalance Calculation
For each unit, the imbalance is determined by:
 ![image](https://github.com/user-attachments/assets/03cfd4c2-5859-4e56-8409-9d889fd21a8e)

where:
•	Zi is the energy supply (“Zebra” value).
•	Fi is the energy demand (“Food” value).
A positive Δiindicates a surplus; a negative value indicates a deficit.
Total Surplus and Deficit
The system computes the overall surplus and deficit by summing the net energies:
 ![image](https://github.com/user-attachments/assets/8cb95510-d59f-4409-81f6-073b7dc23dfe)

For example, in the given code:
•	Machining has a surplus of 40 (i.e., 190−150).
•	Other units have deficits, leading to a total deficit of 50.
External Energy Injection
If the overall supply is less than the demand:
Required External Energy=Total Deficit−Total Surplus
The simulation applies this injection by setting each unit's adjusted supply equal to its demand. This ensures that:
 ![image](https://github.com/user-attachments/assets/1062475f-0d4e-4684-aa54-6f8493cd2e25)

for all units, achieving a balanced system.

# LED Status Update
The function updateLEDs() checks the balance:
•	Red LED: Activated if the overall system is in deficit (Total Surplus<Total Deficit)

•	White LED: Activated if the system is balanced (Total Surplus≈Total Deficit)

•	Green LED: Could be used to indicate surplus (Total Surplus<Total Deficit).

•	Yellow LED: Indicated Solar Power being used.

After the external injection, the system is balanced, and the white LED is turned on.

# 7. Conclusion
Efficient energy management is crucial for modern industrial manufacturing plants, where multiple renewable energy sources power various departments such as machining, welding, and assembly. The variability in energy generation and fluctuating demand necessitates an intelligent and adaptive power allocation strategy. By leveraging the Zebra Optimization Algorithm (ZOA), the central power station can dynamically distribute power based on real-time requirements, ensuring optimal utilization of renewable energy, minimizing energy losses, and enhancing overall efficiency. Implementing ZOA not only improves operational sustainability but also contributes to cost-effective and reliable energy management in industrial settings.
# 8.Demo Video

[Watch the demo video 1](https://youtu.be/6-wVCGnphaM )


[Watch the demo video 2](https://youtu.be/fnti-PApTPE)

# 9.Acknowledgement 

We would like to express our sincere gratitude to everyone who contributed to the successful completion of this project. Special thanks to Mr. Sathya Vignesh, Mr.V. Achyuthand Mr. Deepak Kumar Dasfor their dedication and collaborative efforts in developing the Energy Management System using the Zebra Optimization Algorithm for this hackathon. Their hard work, innovative thinking, and commitment to solving real-world industrial challenges have been instrumental in sha
