#define BLYNK_PRINT Serial // Blynk kütüphanesi debug (hata ayıklama) çıktılarının Serial monitörde görüntülenmesi için tanımlama

#define BLYNK_TEMPLATE_ID "" // Blynk şablon kimliği
#define BLYNK_TEMPLATE_NAME "Sıcaklık ve Nem Sensör" // Blynk şablon adı
#define BLYNK_AUTH_TOKEN "" // Blynk kimlik doğrulama anahtarı

#include <WiFi.h> // WiFi kütüphanesini ekler
#include <BlynkSimpleEsp32.h> // ESP32 için Blynk kütüphanesini ekler
#include <DHT.h> // DHT sensör kütüphanesini ekler

char auth[] = BLYNK_AUTH_TOKEN; // Blynk kimlik doğrulama anahtarını değişkene atar

char ssid[] = "";  // WiFi adı
char pass[] = "";  // WiFi şifresi

BlynkTimer timer; // Blynk zamanlayıcısı oluşturur

#define DHTPIN 5 // DHT sensörünün veri pini, NODE MCU'da D2 pinine bağlı
#define DHTTYPE DHT22 // DHT sensör tipi DHT22 olarak tanımlanır
DHT dht(DHTPIN, DHTTYPE); // DHT sensör nesnesi oluşturulur

// Sensör verilerini okuyan ve Blynk sunucusuna gönderen fonksiyon
void sendSensor()
{
  float h = dht.readHumidity(); // Nem değerini okur
  float t = dht.readTemperature(); // Sıcaklık değerini okur (Fahrenheit için dht.readTemperature(true) kullanılır)

  if (isnan(h) || isnan(t)) { // Eğer okuma başarısız olursa
    Serial.println("Failed to read from DHT sensor!"); // Hata mesajı yazdırılır
    return; // Fonksiyon sonlandırılır
  }
  
  // Değerleri Blynk sanal pinlerine gönderir
  Blynk.virtualWrite(V0, t); // Sıcaklık değeri V0 pinine yazılır
  Blynk.virtualWrite(V1, h); // Nem değeri V1 pinine yazılır
  
  // Seri monitöre sıcaklık ve nem değerleri yazdırılır
  Serial.print("Temperature : ");
  Serial.print(t);
  Serial.print("    Humidity : ");
  Serial.println(h);
}

void setup()
{   
  Serial.begin(115200); // Seri iletişimi başlatır
  
  Blynk.begin(auth, ssid, pass); // Blynk bağlantısını başlatır
  dht.begin(); // DHT sensörünü başlatır
  timer.setInterval(100L, sendSensor); // Her 100ms'de bir sendSensor fonksiyonunu çağırır
}

void loop()
{
  Blynk.run(); // Blynk işlemlerini çalıştırır
  timer.run(); // Zamanlayıcı işlemlerini çalıştırır
}
