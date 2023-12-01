from flask import Flask, request

app = Flask(__name__)

@app.route('/post_endpoint', methods=['POST'])
def handle_post_request():
    data = request.get_json()  # Assuming JSON data is sent
    print("Received data:", data)
    return "Data received successfully!"

if __name__ == '__main__':
    app.run(debug=True, port=8000)
