#ifndef userdefs
#define userdefs

#define PVOUTPUT_API_KEY "YOUR PVOUTPUT API KEY HERE"
//***************************************************************** // ***** Do not use the read only key *****
#define UPDATEINTERVAL 5
//*****************************************************************
#define TIME_OFFSET 0                                               // Change to 1 or 2 when the uploader hase some missing data in Pvoutput
//*****************************************************************
#define NUMSENSORS 1                                                // Total use of sensors 
//***************************************************************** // ***** Arduino Don't use pin 4 and 7 *****
// Where the: 2 = pin, 1000 = pulse S0 counter, xxxxx = SID         // When you have a 800 pulse S0 counter, this this value to 800                                                   
S0Sensor  S1(2,1000,xxxxx,2,1);                                     // Pin 2 = First SID  Solar
//S0Sensor  S2(3,1000,xxxxx,10,1);                                   // Pin 3 = Second SID Solar
//S0Sensor  S3(5,1000,xxxxx,4,1);                                   // Pin 5 = Third SID  Usage [ Change to 4 for upload to usage  in Pvoutput ] 
//*****************************************************************
BaseSensor* sensors[] = {&S1};                                     // {&S1,&S2,&S3,};
#endif
