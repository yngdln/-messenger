from PyQt5 import QtCore, QtGui, QtWidgets
import requests
import datetime
import rsa
import time
import array

class Ui_MainWindow(object):
    def setupUi(self, MainWindow):
        MainWindow.setObjectName("MainWindow")
        MainWindow.resize(800, 600)
        self.centralwidget = QtWidgets.QWidget(MainWindow)
        self.centralwidget.setObjectName("centralwidget")
        self.PushButton = QtWidgets.QPushButton(self.centralwidget)
        self.PushButton.setGeometry(QtCore.QRect(620, 520, 120, 50))
        self.PushButton.setObjectName("PushButton")
        self.textEdit = QtWidgets.QTextEdit(self.centralwidget)
        self.textEdit.setGeometry(QtCore.QRect(20, 520, 600, 50))
        self.textEdit.setObjectName("textEdit")
        self.textEdit_2 = QtWidgets.QTextEdit(self.centralwidget)
        self.textEdit_2.setGeometry(QtCore.QRect(550, 30, 190, 40))
        self.textEdit_2.setObjectName("textEdit_2")
        self.textBrowser = QtWidgets.QTextBrowser(self.centralwidget)
        self.textBrowser.setGeometry(QtCore.QRect(20, 90, 720, 410))
        self.textBrowser.setPlainText("")
        self.textBrowser.setObjectName("textBrowser")
        MainWindow.setCentralWidget(self.centralwidget)

        self.retranslateUi(MainWindow)
        QtCore.QMetaObject.connectSlotsByName(MainWindow)

    def retranslateUi(self, MainWindow):
        _translate = QtCore.QCoreApplication.translate
        MainWindow.setWindowTitle(_translate("MainWindow", "MainWindow"))
        self.PushButton.setText(_translate("MainWindow", "Send"))

class MessengerApp(QtWidgets.QMainWindow, Ui_MainWindow):
    def __init__(self, host="http://127.0.0.1:5000"):
        super().__init__()
        self.host=host
        self.setupUi(self)
        self.PushButton.pressed.connect(self.send_messages)
        self.timer= QtCore.QTimer()
        self.timer.timeout.connect(self.get_messages)
        self.timer.start(1000)
        self.after_val=0
        self.friend_public_key = list()
        self.mypublickey = None
        self.myprivatekey = None
        self.count_arr=array.array('i',[0])


    def serch_key(self):
        self.connect(self.send_messages)


    def is_hex(s,s_1):
        try:
            int(s_1,16)
            return True
        except ValueError:
            return False


    def str_(s,s_1):
        return s_1


    def send_key(self):
        (pubkey, privkey) = rsa.newkeys(512)
        self.myprivatekey=privkey
        self.mypublickey=pubkey
        try:
            r=requests.get(self.host+"/count")
        except requests.exceptions.ConnectionError:
            return   
        response=r.json()
        for item in response["messages"]:
            count=str(int(str(item.get('msg'))[0])+1)+"_"
        try:
            r=requests.post(
                    self.host+"/count",
                    json=count
                )
        except requests.exceptions.ConnectionError:
            return

        name="_pub_key_"+count[0]
        text=str(pubkey)
        try:
            r=requests.post(
                self.host+"/send",
                json={"text":text, "name": name}
            )
        except requests.exceptions.ConnectionError:
            return
        self.textBrowser.append(count) 
    

    def get_messages(self):
        try:
            r=requests.get(
                self.host+"/get",
                params={"after": self.after_val}
                )
        except requests.exceptions.ConnectionError:
            return
        response=r.json()
        if len(response["messages"])>0:
            self.after_val=response["messages"][-1]["date"]
            for item in response["messages"]:
                name=item["name"]
                if(self.is_hex(str(item.get("msg"))) and str(item.get("name"))!=self.textEdit_2.toPlainText()):
                    
                    mes=bytes.fromhex(str(item.get("msg")))
                    try:
                        decrypt_message = rsa.decrypt(mes, self.myprivatekey)
                        
                    except rsa.DecryptionError:
                        decrypt_message=None
                    if(decrypt_message!=None):
                        text=str(decrypt_message)[2:-1]
                        dt = datetime.datetime.fromtimestamp(item["date"]).strftime("%Y,%m,%d %H:%M:%S")
                        self.textBrowser.append(dt+" "+name)
                        self.textBrowser.append(text)
                        self.textBrowser.append(" ")
                s_name = "".join([char for char in name if not char.isdigit()])
                try:
                    num=int("".join([char for char in name if char.isdecimal()]))
                except ValueError:
                    num=0
                l=len(self.friend_public_key)
                if(s_name=="_pub_key_"):
                    self.count_arr.append(num)
                    text=item.get("msg")
                    s=str(text)
                    n=s[10:-8]
                    self.friend_public_key.append(n)
                        
                
         
    def send_messages(self):
        name=self.textEdit_2.toPlainText()
        text=self.textEdit.toPlainText()
        dt = datetime.datetime.fromtimestamp(time.time()).strftime("%Y,%m,%d %H:%M:%S")
        for i in range(len(self.friend_public_key)):
            key=self.mypublickey
            key.n=int(self.friend_public_key[i])
            crypto_message = rsa.encrypt(text.encode('utf-8'), key).hex()
            text_crypto=str(crypto_message)
            try:
                r=requests.post(
                    self.host+"/send",
                    json={"text":text_crypto, "name": name}
                )
            except requests.exceptions.ConnectionError:
                return
        self.textBrowser.append(dt+" "+name)
        self.textBrowser.append(text)
        self.textBrowser.append(" ")
        self.textEdit.clear()
    
    
    
    



if __name__ == "__main__":
    app = QtWidgets.QApplication([]) 
    window = MessengerApp(host="http://192.168.1.47:5000")
    window.send_key()
    window.show()
    app.exec()