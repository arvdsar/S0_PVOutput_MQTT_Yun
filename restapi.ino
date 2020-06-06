
//used to set the todaycnt or total per sensor. Also used to save both todaycnt from all sensors.
// http://root:password@ipaddress/data/variable/value
//replace variable with: todaycnt0, todaycnt1, total0, total1 or just with save to save all todaycnt's
//replace value with the new value.
//the number behind todaycnt (0,1,2) indicates which sensor you are updating. This cannot be higher than the number of sensors you have.


void checkAPI(){
  char   total[10];
  char   todayCnt[10];
  char   save[2];
  char   api[15];
 for(byte i = 0; i < NUMSENSORS; i++){
    
    sprintf(api,"total%d",i);
    Bridge.get(api,total,10);
    long totalLong = atol(total);

    sprintf(api,"todaycnt%d",i);
    Bridge.get(api,todayCnt,10);
    long todayCntLong = atol(todayCnt);

    Bridge.get("save",save,2);
    int saveInt = atoi(save);

    if (totalLong !=0){
      sensors[i]->NewTotal(totalLong);
      sprintf(api,"total%d",i);
      Bridge.put(api,"0"); //reset storage
    }

    if (todayCntLong != 0){
      sensors[i]->Update(todayCntLong);
      sprintf(api,"todaycnt%d",i);
      Bridge.put(api,"0"); //reset storage
    }
    if (saveInt == 1){  //trigger saving todaycnt so you can reset the arduino (I guess we have small bug here. After saving the first sensor, it will reset save and not save the next one.
       sensors[i]->Save();
       Bridge.put("save","0"); //reset storage
    }
  }
}
