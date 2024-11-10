import paho.mqtt.client as mqtt
import firebase_admin
from firebase_admin import credentials, firestore
import json  # To handle JSON data

# Initialize Firestore
cred = credentials.Certificate("/home/kavish/Downloads/sit210-project-a83fd-firebase-adminsdk-jm86y-8fe123c70e.json")
firebase_admin.initialize_app(cred)
db = firestore.client()

# Callback function for when a message is received
def on_message(client, userdata, msg):
    topic = msg.topic
    payload = msg.payload.decode()
    print(f"Received `{payload}` from `{topic}` topic")

    try:
        # Parse the received JSON data
        data = json.loads(payload)

        # Store data in Firestore
        db.collection('sensorData').add({
            'temperature': data['temperature'],
            'humidity': data['humidity'],
            'timestamp': firestore.SERVER_TIMESTAMP
        })
        print("Data saved to Firestore.")

    except json.JSONDecodeError:
        print("Failed to decode JSON")
    except KeyError:
        print("Invalid data format received")

# MQTT setup
mqtt_client = mqtt.Client()
mqtt_client.on_message = on_message
mqtt_client.connect("broker.emqx.io", 1883, 60)
print("Connected successfully to MQTT broker.")
mqtt_client.subscribe("SIT210/dht22")  # Subscribe to sensor topic
mqtt_client.loop_start()

# Keep the script running
try:
    while True:
        pass
except KeyboardInterrupt:
    mqtt_client.loop_stop()
    mqtt_client.disconnect()
