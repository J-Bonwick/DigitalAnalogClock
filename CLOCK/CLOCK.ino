#include <Time.h>
#include <TimeLib.h>
#include <DS3232RTC.h>

int clockA = 2;          // Set these to the pin numbers you have attached the clock wires
int clockB = 3;          // to.  Order is not important.

int tickPin = clockA;    // This keeps track of which clock pin should be fired next.

long localSecond = 21600;
long rtcSecond;
// Initialize the IO ports
void setup()
{
  pinMode(clockA, OUTPUT);
  pinMode(clockB, OUTPUT);

  digitalWrite(clockA, LOW);
  digitalWrite(clockB, LOW);

  Serial.begin(115200);
  setSyncProvider(RTC.get);   // the function to get the time from the RTC
  if (timeStatus() != timeSet)
    Serial.println("Unable to sync with the RTC");
  else
    Serial.println("RTC has set the system time");
}

void doTick(int dir) {
  // Energize the electromagnet in the correct direction.
  digitalWrite(tickPin, HIGH);
  //Depending on the delay the clock will move forward or backwards.
  if (dir == 1) {
    delay(22);// Move forward.
    localSecond++;
    if (localSecond >= 43200) {
      localSecond = 0;
    }
  }
  else {
    delay(10);//Move Backward.
    localSecond--;
    if (localSecond <= 0) {
      localSecond = 43200;
    }
  }
  digitalWrite(tickPin, LOW);
  if (tickPin == clockA)
  {
    tickPin = clockB;
  } else {
    tickPin = clockA;
  }
}


// Main loop
void loop()
{
  rtcSecond = ((long)hourFormat12() * 3600 + minute() * 60 + second());
  if (rtcSecond != localSecond) {
    if (rtcSecond > localSecond) {
      if (abs(rtcSecond - localSecond) > (localSecond + 43200 - rtcSecond)) {
        doTick(0);
        delay(100);
      }
      else {
        doTick(1);
        delay(100);
      }
    }
    else {
      if (abs(rtcSecond - localSecond) > (43200 + rtcSecond - localSecond)) {
        doTick(1);
        delay(100);
      }
      else {
        doTick(0);
        delay(100);
      }
    }
  }
  digitalClockDisplay();
}
void digitalClockDisplay(void)
{
  // digital clock display of the time
  Serial.print(hourFormat12());
  printDigits(minute());
  printDigits(second());
  Serial.print("-");
  Serial.print(localSecond);
  Serial.print(":");
  Serial.print(rtcSecond);
  Serial.println();
}
void printDigits(int digits)
{
  // utility function for digital clock display: prints preceding colon and leading 0
  Serial.print(':');
  if (digits < 10)
    Serial.print('0');
  Serial.print(digits);
}
