//************************************************************
// this is a simple example that uses the painlessMesh library
//
// 1. sends a silly message to every node on the mesh at a random time between 1 and 5 seconds
// 2. prints anything it receives to Serial.print
//
//
//************************************************************
#include "painlessMesh.h"
//--------------------------------------------------------------------------------
#define   MESH_PREFIX     "whateverYouLike"
#define   MESH_PASSWORD   "somethingSneaky"
#define   MESH_PORT       5555
//--------------------------------------------------------------------------------
int incomingByte = 0;  // для хранения полученного байта
bool led_flag = false;
//--------------------------------------------------------------------------------
Scheduler userScheduler; // to control your personal task
painlessMesh  mesh;
//--------------------------------------------------------------------------------
// User stub
void sendMessage() ; // Prototype so PlatformIO doesn't complain

void led_ON(void);
void led_OFF(void);
//--------------------------------------------------------------------------------
Task taskSendMessage( TASK_SECOND * 1 , TASK_FOREVER, &sendMessage );
//--------------------------------------------------------------------------------
void led_ON(void)
{
    digitalWrite(LED_BUILTIN, LOW);
}
//--------------------------------------------------------------------------------
void led_OFF(void)
{
    digitalWrite(LED_BUILTIN, HIGH);
}
//--------------------------------------------------------------------------------
void sendMessage()
{
    String msg;
    if(led_flag)
    {
        msg = "led_ON";
    }
    else
    {
        msg = "led_OFF";
    }

    mesh.sendBroadcast( msg );
    taskSendMessage.setInterval( random( TASK_SECOND * 1, TASK_SECOND * 5 ));
}
//--------------------------------------------------------------------------------
// Needed for painless library
void receivedCallback( uint32_t from, String &msg )
{
    //Serial.printf("startHere: Received from %u msg=%s\n", from, msg.c_str());
    if(msg == "led_ON")
    {
        Serial.println("led_ON");
        led_flag = true;
        led_ON();
    }

    if(msg == "led_OFF")
    {
        Serial.println("led_OFF");
        led_flag = false;
        led_OFF();
    }
}
//--------------------------------------------------------------------------------
void newConnectionCallback(uint32_t nodeId)
{
    //Serial.printf("--> startHere: New Connection, nodeId = %u\n", nodeId);
}
//--------------------------------------------------------------------------------
void changedConnectionCallback()
{
    //Serial.printf("Changed connections %s\n", mesh.subConnectionJson().c_str());
}
//--------------------------------------------------------------------------------
void nodeTimeAdjustedCallback(int32_t offset)
{
    //Serial.printf("Adjusted time %u. Offset = %d\n", mesh.getNodeTime(), offset);
}
//--------------------------------------------------------------------------------
void setup()
{
    Serial.begin(115200);

    //mesh.setDebugMsgTypes( ERROR | MESH_STATUS | CONNECTION | SYNC | COMMUNICATION | GENERAL | MSG_TYPES | REMOTE ); // all types on
    mesh.setDebugMsgTypes( ERROR | STARTUP );  // set before init() so that you can see startup messages

    mesh.init( MESH_PREFIX, MESH_PASSWORD, &userScheduler, MESH_PORT );
    mesh.onReceive(&receivedCallback);
    mesh.onNewConnection(&newConnectionCallback);
    mesh.onChangedConnections(&changedConnectionCallback);
    mesh.onNodeTimeAdjusted(&nodeTimeAdjustedCallback);

    userScheduler.addTask( taskSendMessage );
    taskSendMessage.enable();

    pinMode(LED_BUILTIN, OUTPUT);
    led_OFF();
}
//--------------------------------------------------------------------------------
void loop()
{
    if (Serial.available() > 0)
    {
        incomingByte = Serial.read();
        if(incomingByte == '1') 
        {
          led_flag = true;
          led_ON();
        }
        if(incomingByte == '0')
        {
          led_flag = false;
          led_OFF();
        }

        String value = led_flag ? "true" : "false";
        Serial.println("### led_flag is " + value);

        sendMessage();
    }

    userScheduler.execute(); // it will run mesh scheduler as well
    mesh.update();
}
//--------------------------------------------------------------------------------
