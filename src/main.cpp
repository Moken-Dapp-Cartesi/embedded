#include <ArduinoJson.h>
#include <HTTPClient.h>
#include <NfcAdapter.h>
#include <PN532.h>
#include <PN532_I2C.h>
#include <WiFi.h>
#include <Wire.h>

using namespace std;

// Local network
const char* ssid = "xxxxxx";
const char* password = "xxxxxx";
const char* endpoint = "http://localhost:4000/graphql";

PN532_I2C pn532_i2c(Wire);
NfcAdapter nfc = NfcAdapter(pn532_i2c);

class GraphQL {
public:
	static String takeOwnerAddress() {
		HTTPClient http;

		http.begin(endpoint);

		http.addHeader("Accept-Encoding", "gzip, deflate, br");
		http.addHeader("Content-Type", "application/json");
		http.addHeader("Accept", "application/json");
		http.addHeader("Connection", "keep-alive");
		http.addHeader("DNT", "1");

		StaticJsonDocument<200> doc;

		// Get last notice / last owner address
		doc["query"] = "query GetNotices($cursor: String) {notices(last : 1, after: $cursor) {nodes { id payload }}}";

		String requestBody;
		serializeJson(doc, requestBody);

		int httpResponseCode = http.POST(requestBody);

		if (httpResponseCode > 0) {
			Serial.print("HTTP Response code: ");
			Serial.println(httpResponseCode);

			String response = http.getString();
			http.end();

			StaticJsonDocument<200> doc;

			deserializeJson(doc, response);

			String address = doc["data"]["notices"]["nodes"][0]["payload"].as<String>();

			Serial.println(address);

			return address;
		} else {
			Serial.print("Error code: ");
			Serial.println(httpResponseCode);
			http.end();
		}
	}
};

void canPass() { digitalWrite(23, HIGH); }

void cannotPass() { digitalWrite(23, LOW); }

void setup() {
	Serial.begin(115200);
	// Start PN532
	nfc.begin();

	// Try to connect to WiFi network and wait for connection
	WiFi.begin(ssid, password);

	pinMode(23, OUTPUT);

	digitalWrite(23, LOW);

	while (WiFi.status() != WL_CONNECTED) {
		delay(1000);
		Serial.println("Connecting to WiFi...");
	}

	Serial.println("WiFi connected!");
}

void loop() {
	if (WiFi.status() == WL_CONNECTED) {
		// Get Notice
		String owner_address = GraphQL::takeOwnerAddress();

		Serial.println("\nScan a NFC tag\n");
		if (nfc.tagPresent()) {
			NfcTag tag = nfc.read();
			NdefMessage message = tag.getNdefMessage();
			tag.print();

			NdefRecord record = message.getRecord(0);

			if (owner_address == record.getId()) {
				canPass();
			} else {
				cannotPass();
			}
		}
	}
	delay(5000);
	cannotPass();
}
