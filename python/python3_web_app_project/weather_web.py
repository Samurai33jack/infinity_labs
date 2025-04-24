import urllib

import flask
import requests

app = flask.Flask(__name__)


@app.route("/", methods=['POST', 'GET'])
def form():
    return flask.render_template('form.html')


@app.route("/back", methods=['POST', 'GET'])
def back():
    return flask.render_template('form.html')


@app.route('/data/', methods=['POST', 'GET'])
def data():
    if flask.request.method == 'GET':
        return f"The URL /data is accessed directly. Try going to '/' to submit form"
    if flask.request.method == 'POST':
        form_data = flask.request.form
        city = form_data['City']
        full_url = 'http://api.openweathermap.org/data/2.5/forecast?q=' + city + ('&units=metric&appid'
                                                                                  '=e9b4d9f438febe993453dcd5bd75cbd6')

        json_data = requests.get(full_url).json()

        try:
            for item in json_data["list"]:
                if "12:00:00" in item["dt_txt"]:
                    item["dt_txt"] = item["dt_txt"].replace("12:00:00", "day")
                elif "00:00:00" in item["dt_txt"]:
                    item["dt_txt"] = item["dt_txt"].replace("00:00:00", "night")
                else:
                    item["dt_txt"] = None

            return flask.render_template('print_search.html', title='search results',
                                         data=json_data)
        except Exception:
            print("Something went wrong")
            return flask.render_template('form.html')


if __name__ == '__main__':
    app.run(debug=True)
