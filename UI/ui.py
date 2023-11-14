from flask import Flask, render_template, jsonify

app = Flask(__name__)

# sample data change with the data from the server
datasets = [
    {
        "label": "Dataset 1",
        "data": [65, 59, 80, 81, 56],
        "borderColor": "rgba(75, 192, 192, 1)",
        "backgroundColor": "rgba(75, 192, 192, 0.2)"
    },
    {
        "label": "Dataset 2",
        "data": [28, 48, 40, 19, 86],
        "borderColor": "rgba(255, 99, 132, 1)",
        "backgroundColor": "rgba(255, 99, 132, 0.2)"
    },
    {
        "label": "Dataset 3",
        "data": [45, 55, 70, 50, 75],
        "borderColor": "rgba(255, 205, 86, 1)",
        "backgroundColor": "rgba(255, 205, 86, 0.2)"
    },
    {
        "label": "Dataset 4",
        "data": [30, 25, 60, 70, 40],
        "borderColor": "rgba(54, 162, 235, 1)",
        "backgroundColor": "rgba(54, 162, 235, 0.2)"
    }
]

@app.route('/')
def index():
    return render_template('index.html')

@app.route('/get_data/<int:index>')
def get_data(index):
    return jsonify(datasets[index])

if __name__ == '__main__':
    app.run(debug=True)
