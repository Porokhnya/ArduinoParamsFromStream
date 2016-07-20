//-------------------------------------------------------------------------------------------
#include "CommandsParser.h"
//-------------------------------------------------------------------------------------------
CommandsParser cParser(Serial);
//-------------------------------------------------------------------------------------------
void paramsHandler(const char* param, const char* value)
{
  Serial.print("Received: param=");
  Serial.print(param);
  Serial.print("; value=");
  Serial.println(value);
}
//-------------------------------------------------------------------------------------------
void setup() 
{
  Serial.begin(9600);
  cParser.begin(paramsHandler);

}
//-------------------------------------------------------------------------------------------
void loop() 
{
  cParser.update();

}
//-------------------------------------------------------------------------------------------
