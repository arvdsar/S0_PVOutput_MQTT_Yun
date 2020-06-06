

long ReadSD(int id) {

  //open the file for reading:
  char path[30];
  long content;
  sprintf(path, "/mnt/sda1/store/%0d.txt", id);

  File myFile = FileSystem.open(path, FILE_READ);
  Console.print("Read File: ");
  Console.println(path);
  
  if (myFile) {
    
    // read from the file until there's nothing else in it:
    myFile.read((byte*)&content, sizeof(long)); // read 4 bytes

      myFile.close();
    }
  else {
    // if the file didn't open, print an error:
    Console.println("error opening for reading");
  }
 Console.print("data: ");
 Console.println(content);

return(content);
}




long WriteSD(int id, long getal) {
 char path[30];
    sprintf(path, "/mnt/sda1/store/%0d.txt", id);
    Console.print("Write File: ");
    Console.println(path);
    
    File dataFile = FileSystem.open(path, FILE_WRITE);
  // if the file is available, write to it:
  if (dataFile) {
    dataFile.write((byte*)&getal, sizeof(long)); // write 4 bytes
    dataFile.close();
    
  }
  // if the file isn't open, pop up an error:
  else {
    Console.println("error opening file for writing");
  }
}
