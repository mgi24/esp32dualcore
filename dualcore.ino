//ESP32 dual core usage example by MMV Project
//Free to distribute, do not forget the credit!
//Pastikan sudah install Library ESP32!!!

// Deklarasi fungsi task
void run_on_core0(void *parameter);
void run_on_core1(void *parameter);

//pengganti semaphor
int core0_go=0;
int core1_go=0;

void setup() {
  // Inisialisasi Serial Monitor
  Serial.begin(115200);

  // Buat task run_on_core0 yang berjalan di core 0
  xTaskCreatePinnedToCore(
    run_on_core0,    // Fungsi task
    "Task1",         // Nama task
    10000,           // Ukuran stack task
    NULL,            // Parameter yang akan diteruskan ke task
    1,               // Prioritas task
    NULL,            // Handle task
    0);              // Core yang akan menjalankan task

  // Buat task run_on_core1 yang berjalan di core 1
  xTaskCreatePinnedToCore(
    run_on_core1,    // Fungsi task
    "Task2",         // Nama task
    10000,           // Ukuran stack task
    NULL,            // Parameter yang akan diteruskan ke task
    2,               // Prioritas task
    NULL,            // Handle task
    1);              // Core yang akan menjalankan task
  delay(1000);
  //mulai memberikan core 1 mulai dlu loopnya!
  core1_go=1;
}

void loop() {
  // Tidak ada yang dilakukan di loop utama
  delay(10000);
}

void run_on_core0(void *parameter) {
  Serial.print("TASK 1 using core ");  Serial.print(xPortGetCoreID());
  Serial.print(", priority = "); Serial.println(uxTaskPriorityGet(NULL));
  while (true) {
    //TULIS LOOP CORE 0 DISINI
    //Serial.println("jalan di core 0");


    //ini untuk task yang harus nunggu resource
    if (core0_go==1){
      core0_go=0;
      Serial.println("TASK 1 Sedang Berjalan");
      delay(1000);
      core1_go=1;//menyerahkan core2
    }
    //jangan hapus vTaskDelay, klo dihapus bakal timeout dan crash!!!
    vTaskDelay(portTICK_PERIOD_MS); // Delay 1 detik
  }
}

void run_on_core1(void *parameter) {
  Serial.print("TASK 2 using core ");  Serial.print(xPortGetCoreID());
  Serial.print(", priority = "); Serial.println(uxTaskPriorityGet(NULL));
  while (true) {
    //TULIS LOOP CORE 1 DISINI
    //Serial.println("jalan di core 1");

    //ini untuk task yang harus nunggu resource
    if(core1_go==1){
      core1_go=0;
      Serial.println("Task 2 SEDANG BERJALAN");
      delay(1000);
      core0_go=1;
    }
    
    //jangan hapus vTaskDelay, klo dihapus bakal timeout dan crash!!!
    vTaskDelay(portTICK_PERIOD_MS); // Delay 1 detik
  }
}
