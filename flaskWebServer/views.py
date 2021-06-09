import json

from flask import Blueprint, request, render_template
import app
import postHandler


home_monitoring = Blueprint('homeMonitoring', __name__, template_folder='templates')


@home_monitoring.route("/")
def index():
    return render_template('index_templ.html', devices=app.devices, influx=app.influx)


@home_monitoring.route('/', methods=['GET', 'POST'])
def refresh():
    if request.method == "POST":
        postHandler.on_post(request.form)
        return render_template('index_templ.html', devices=app.devices, influx=app.influx)
    return '', 204

