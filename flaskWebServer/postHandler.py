import json

import app
import topicName
import utility


def on_post(form):
    if form is not None:
        formDict = form.to_dict()
        jsonDict = reformat_onPost_Json(formDict)
        update_data_onPost(formDict)
        payload = json.dumps(jsonDict)
        if formDict.get("form") == "switchInflux":
            app.publish(topicName.influx_db, payload)
        if formDict.get("form") == "switchMYSQL":
            app.publish(topicName.mysql_db, payload)
        elif formDict.get("form") == "reading":
            app.publish(topicName.header + "/onPost", payload)
        elif formDict.get("form") == "actuator":
            app.publish(topicName.actuator_state_set, payload)


def reformat_onPost_Json(formDict):
    print(formDict)
    dict = {"mac_address": formDict.get('mac_address')}
    if formDict.get("form") == "switchInflux":
        dict["active"] = True if formDict.get('switchInflux') == "true" else False
    if formDict.get("form") == "switchMYSQL":
        dict["active"] = True if formDict.get('switchMYSQL') == "true" else False
    elif formDict.get("form") == "reading":
        dict["sensors"] = [{
            "name": formDict.get('sensor_name'),
            "readings": [{
                "name": formDict.get('reading_name'),
                "threshold": formDict.get('threshold'),
            }],
        }]
    elif formDict.get("form") == "actuator":
        if formDict.get('state') == "True":
            state = True
        else:
            state = False
        dict["actuator"] = {
            "name": formDict.get('actuator_name'),
            "state": state
        }
    return dict


def update_data_onPost(formDict):
    device = utility.getDevice(app.devices, formDict.get("mac_address"))
    if formDict.get("form") == "switchInflux":
        device.switchInflux = True if formDict.get('switchInflux') == "true" else False
    if formDict.get("form") == "switchMYSQL":
        device.switchMYSQL = True if formDict.get('switchMYSQL') == "true" else False
    elif formDict.get("form") == "reading":
        sensor = device.getSensor(formDict.get('sensor_name'))
        reading = sensor.getReading(formDict.get('reading_name'))
        reading.threshold = formDict.get('threshold')
    elif formDict.get("form") == "reading":
        actuator = device.getActuator(formDict.get('actuator_name'))
        actuator.state = formDict.get('state')
