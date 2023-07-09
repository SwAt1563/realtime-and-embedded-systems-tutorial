
#include <Arduino_FreeRTOS.h>

#include <semphr.h>  // add the FreeRTOS functions for Semaphores (or Flags).

// Declare a mutex Semaphore Handle which we will use to manage 
// the Serial Port.
// It will be used to ensure only one Task is accessing this 
// resource at any time.
SemaphoreHandle_t xSerialSemaphore;

// define two Tasks for DigitalRead & AnalogRead
void TaskDigitalRead( void *pvParameters );
void TaskAnalogRead( void *pvParameters );




void TaskBlink1( void *pvParameters );



void setup() {
  // initialize serial communication at 9600 bits per second:

  Serial.begin(9600);

  xTaskCreate(TaskBlink1, "task1", 128, NULL, 1, NULL);


  if ( xSerialSemaphore == NULL )  
  {
    // Create a mutex semaphore we will use to manage the Serial Port
    xSerialSemaphore = xSemaphoreCreateMutex();

    if ( ( xSerialSemaphore ) != NULL )

      // Make the Serial Port available for use, by "Giving" 
      // the Semaphore.
      xSemaphoreGive( ( xSerialSemaphore ) );  
  }

  // Now set up two Tasks to run independently.
  xTaskCreate(
      TaskDigitalRead,
      "DigitalRead",  // A name just for humans
      128,  // This is the stack size
      NULL,
      2,  // Priority, with 1 being the highest, and 4 the lowest.
      NULL );

  xTaskCreate(
      TaskAnalogRead,
      "AnalogRead",
      128,  // Stack size
      NULL,
      1,
      NULL );
      
      


  vTaskStartScheduler();

}

void loop()
{
}

void TaskBlink1(void *pvParameters)  {
  pinMode(8, OUTPUT);

  while(1)
  {
    Serial.println("Task1");
    digitalWrite(8, HIGH);   

    vTaskDelay( 200 / portTICK_PERIOD_MS ); 
    digitalWrite(8, LOW);    
    vTaskDelay( 200 / portTICK_PERIOD_MS ); 
  }


}



void TaskDigitalRead( void *pvParameters )  // This is a Task.
{
  // DigitalReadSerial
  // Reads a digital input on pin 2, prints the result to the 
  // serial monitor

  // This example code is in the public domain.
  
  // digital pin 2 has a pushbutton attached to it. Give it a name:
  uint8_t pushButton = 2;

  // make the pushbutton's pin an input:
  pinMode(pushButton, INPUT);

  for (;;) // A Task shall never return or exit.
  {
    // read the input pin:
    int buttonState = digitalRead(pushButton);

    // See if we can obtain or "Take" the Serial Semaphore.
    // If the semaphore is not available, wait 5 ticks of the 
    // Scheduler to see if it becomes free.
    if ( xSemaphoreTake( xSerialSemaphore, ( TickType_t ) 5 ) == pdTRUE )
    {
     
     
      
      if(buttonState == HIGH){
        digitalWrite(9, HIGH);
        vTaskDelay(3000 / portTICK_PERIOD_MS);

      }else{
        digitalWrite(9, LOW);
      }


      // Now free or "Give" the Serial Port for others.
      xSemaphoreGive( xSerialSemaphore ); 
    }

    // one tick delay (15ms) in between reads for stability
    vTaskDelay(1);
  }
}

void TaskAnalogRead( void *pvParameters )
{
  for (;;)
  {
    // read the input on analog pin 0:
    int sensorValue = analogRead(A0);

    // See if we can obtain or "Take" the Serial Semaphore.
    // If the semaphore is not available, wait 5 ticks of the 
    // Scheduler to see if it becomes free.
    if ( xSemaphoreTake( xSerialSemaphore, ( TickType_t ) 5 ) == pdTRUE )
    {
     
      if(sensorValue > 0){
        digitalWrite(10, HIGH);
        vTaskDelay(3000 / portTICK_PERIOD_MS);

      }else{
        digitalWrite(10, LOW);
      }


      // Now free or "Give" the Serial Port for others.
      xSemaphoreGive( xSerialSemaphore ); 
    }

    // one tick delay (15ms) in between reads for stability
    vTaskDelay(1); 
  }
}
