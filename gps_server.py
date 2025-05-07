from flask import Flask, jsonify
import serial
import threading

app = Flask(__name__)
gps_data = {"lat": 0.0, "lng": 0.0}

def read_serial():
    global gps_data
    ser = serial.Serial('COM3', 9600)  # Change to your COM port
    while True:
        line = ser.readline().decode('utf-8').strip()
        if ',' in line:
            try:
                lat, lng = map(float, line.split(','))
                gps_data = {"lat": lat, "lng": lng}
            except:
                pass

@app.route('/location')
def location():
    return jsonify(gps_data)

if __name__ == '__main__':
    threading.Thread(target=read_serial, daemon=True).start()
    app.run(debug=False)
