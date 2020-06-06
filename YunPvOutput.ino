float previous = -1;

// This function updates all registered sensors to pvoutput
// The sensors are listed in the 'S' array
void SendToPvOutput(BaseSensor** S)
{
  
  // create a total for each variable that can be used in pvoutput
  // !! The index in this array starts at 0 while the pvoutput vars start at 1
  float v[12]; // data sum
  bool b[12]; // data present flags
  // start with 0
  for(byte n = 0; n < 12; n++)
  { 
    v[n] = 0;
    b[n] = false;
  }
  
  
  long sid = S[0]->SID;

  for(byte i = 0; i<NUMSENSORS; i++) // scan through the sensor array
  {
    byte type = S[i]->Type;
    float actual = S[i]->Actual / S[i]->Factor;
    float peak = S[i]->Peak / S[i]->Factor;
    float today = S[i]->Today / S[i]->Factor;

    switch(type)
    {
      // temperature
      case 5:   v[type-1] += actual;
                b[type-1] = true;
                break;
      //voltage
      case 6:   v[type-1] += today;
                b[type-1] = true;
                break;
      //ferraris or P1
      case 24:  // total consumption is production + net consumption
                v[2] = v[0] + today;
                
                if(v[1] == 0) // no production, use data from type 24 directly
                {
                    v[3] = actual;
                }
                else
                {
                    // actual power is energy since previous upload divided by number of uploads per hour
                    // using this method because actual values of production and consumption sensors have different sampling rates, causing actual to be unreliable.
                    if(previous >=0 && previous < v[2]) 
                    {
                      v[3] = (v[2] - previous) * 60 / UPDATEINTERVAL;
                    }
                }
                previous = v[2];
                b[2] = true;
                b[3] = true;
                break;
      // other sensors (including type 0). Log Peak and total
      default:  v[type-1] += peak;
                v[type-2] += today;
                b[type-1] = true;
                b[type-2] = true;
   }

    if(i == NUMSENSORS-1 || S[i+1]->SID != sid)
    {
      if(sid > 0) // only upload if the sid is valid
      {   
          
          //The order is important, the parameters are provided in this order and interpreted by the
          //python script as parameters in this order!

          Process p;
          p.begin("/mnt/sda1/pvoutput.py");
          
          sprintf(webData, "%ld",sid);
          p.addParameter(webData);
          p.addParameter(PVOUTPUT_API_KEY);
          sprintf(webData, "%04d%02d%02d", year(),month(),day());
          p.addParameter(webData);
          sprintf(webData, "%02d:%02d", hour(),minute());
          p.addParameter(webData);

          for(int i = 0; i < 12; i++)
          {

            //alleen v1 en v2:
            long x = (v[0]*1000)/1000;
            sprintf(webData,"%ld",x);
            p.addParameter(webData);
            x = (v[1]*1000)/1000;
            sprintf(webData,"%ld",x);
            p.addParameter(webData);
          }

          p.run();
    
  
      }
      // reset the counters for the next round
      for(byte n = 0; n < 12; n++)
      {
        v[n] = 0;
        b[n] = false;
      }
      if(i < NUMSENSORS) sid = S[i+1]->SID;
    }
  }
}
