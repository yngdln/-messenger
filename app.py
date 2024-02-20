from datetime import datetime as dt, timedelta as td
import time
from flask import Flask, request
app=Flask(__name__)

@app.route("/status")
def status():
    return{
        "status": True,
        "name": "MyMessenger",
        "time":dt.now().strftime("%Y %m %d"),
    }

db=[
    {
        "msg":"gtr",
        "name":"ggrrb",
        "date":time.time(),
    },
    {
        "msg":"sky",
        "name":"rrr",
        "date":time.time(),
    },
    {
        "msg":"mark",
        "name":"bbbb",
        "date":time.time(),
    }
]
@app.route("/send", methods=['POST'])
def send_message():
    data = request.json
    text = data["text"]
    name=data["name"]
    mes={
        "msg":text,
        "name":name,
        "date":time.time()
    }
    db.append(mes)
    return {"status": "OK!"}
@app.route("/send", methods=['GET'])
def show():
    text = "text"
    name="name"
    return{"msg":text,
        "name":name,
        "date":time.time()}
@app.route("/show", methods=["GET"])
def print_db():
    return{"database":db}
def get_messages(after):
    result = []
    for item in db:
        if item["date"] > after:
            result.append(item)
    return result



app.run()