
void SendToMQTT(){ 

   
    for(int i=0;i<NUMSENSORS;i++)
    {  
        char astr[25];
        char pstr[25];
        char tstr[25];
        char lstr[25];
        sprintf(astr,"S0Sensor/%d/Actual", i);
        sprintf(pstr,"S0Sensor/%d/Peak", i);
        sprintf(tstr,"S0Sensor/%d/Today", i);
        sprintf(lstr,"S0Sensor/%d/Total", i);
 
        sensors[i]->CalculateActuals();
        char astrval[16];
        char pstrval[16];
        char tstrval[16];
        char lstrval[16];

        ltoa(sensors[i]->ActualVal(), astrval, 10);
        ltoa(sensors[i]->PeakVal(), pstrval, 10);
        ltoa(sensors[i]->TodayVal(), tstrval, 10);
        ltoa(sensors[i]->TotalVal(), lstrval, 10);


         Process p;    // Create a process and call it "p"
        p.begin("/mnt/sda1/sendmqtt.py");  // Process that launch the "sendmqtt.py" command
        p.addParameter(astr); //topic
        p.addParameter(astrval); //value
        p.addParameter(pstr);
        p.addParameter(pstrval);
        p.addParameter(tstr);
        p.addParameter(tstrval);
        p.addParameter(lstr);
        p.addParameter(lstrval);
        p.run();    // Run the process and wait for its termination
    }
  
}
