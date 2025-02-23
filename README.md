# System-Monitor-by-ESP8266-and-Libre-Hardware-Monitor
System Monitor by ESP8266 and Libre Hardware Monitor

from flask import Flask, jsonify
import requests

app = Flask(__name__)

# Libre Hardware Monitor server URL
LIBRE_HW_MONITOR_URL = "http://192.168.0.19:8085/data.json"

def fetch_and_shorten_data():
    try:
        # Fetch the JSON data from Libre Hardware Monitor
        response = requests.get(LIBRE_HW_MONITOR_URL)
        data = response.json()

        # Initialize shortened data structure
        shortened_data = {
            "CPU": {
                "Name": None,
                "Temperature": None,
                "Load": None
            },
            "GPU": {
                "Name": None,
                "Temperature": None,
                "Load": None
            }
        }

        # Extract CPU and GPU data
        for hardware in data.get("Children", []):
            for device in hardware.get("Children", []):
                # Extract CPU data
                if "Intel Core" in device.get("Text", ""):
                    shortened_data["CPU"]["Name"] = device.get("Text")
                    for sensor in device.get("Children", []):
                        if sensor.get("Text") == "Temperatures":
                            for temp_sensor in sensor.get("Children", []):
                                if "Core" in temp_sensor.get("Text", ""):
                                    shortened_data["CPU"]["Temperature"] = temp_sensor.get("Value")
                        elif sensor.get("Text") == "Load":
                            for load_sensor in sensor.get("Children", []):
                                if "CPU Total" in load_sensor.get("Text", ""):
                                    shortened_data["CPU"]["Load"] = load_sensor.get("Value")

                # Extract GPU data
                if "NVIDIA" in device.get("Text", ""):
                    shortened_data["GPU"]["Name"] = device.get("Text")
                    for sensor in device.get("Children", []):
                        if sensor.get("Text") == "Temperatures":
                            for temp_sensor in sensor.get("Children", []):
                                if "GPU Core" in temp_sensor.get("Text", ""):
                                    shortened_data["GPU"]["Temperature"] = temp_sensor.get("Value")
                        elif sensor.get("Text") == "Load":
                            for load_sensor in sensor.get("Children", []):
                                if "GPU Core" in load_sensor.get("Text", ""):
                                    shortened_data["GPU"]["Load"] = load_sensor.get("Value")

        return shortened_data
    except Exception as e:
        return {"error": str(e)}

@app.route('/data.json')
def serve_shortened_data():
    shortened_data = fetch_and_shorten_data()
    return jsonify(shortened_data)

if __name__ == '__main__':
    app.run(host='0.0.0.0', port=5000)



