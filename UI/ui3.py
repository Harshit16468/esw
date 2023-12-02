import json
import requests
import matplotlib.pyplot as plt
from flask import Flask, render_template, send_file

app = Flask(__name__)

# ThingSpeak configuration
CHANNEL_ID = 'YOUR_CHANNEL_ID'
READ_API_KEY = 'YOUR_READ_API_KEY'
THINGSPEAK_API_URL = f'https://api.thingspeak.com/channels/{CHANNEL_ID}/feeds.json'

# Function to fetch ThingSpeak data
def fetch_thingspeak_data():
    params = {'api_key': READ_API_KEY, 'results': 10}  # adjust 'results' as needed
    response = requests.get(THINGSPEAK_API_URL, params=params)
    data = json.loads(response.text)
    return data['feeds']

# Function to plot data using Matplotlib
def plot_graph(data):
    timestamps = [entry['created_at'] for entry in data]
    values = [float(entry['field1']) for entry in data]

    plt.plot(timestamps, values)
    plt.xlabel('Timestamp')
    plt.ylabel('Value')
    plt.title('ThingSpeak Data')
    plt.xticks(rotation=45)
    plt.tight_layout()

    # Save the plot as an image file
    plt.savefig('static/graph.png')
    plt.close()

# Flask route to render the HTML page with the graph
@app.route('/')
def index():
    data = fetch_thingspeak_data()
    plot_graph(data)
    return render_template('index2.html')

# Flask route to serve the graph image
@app.route('/graph')
def graph():
    return send_file('static/graph.png', mimetype='image/png')

if __name__ == '__main__':
    app.run(debug=True)
