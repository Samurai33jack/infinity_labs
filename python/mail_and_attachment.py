import smtplib
from email.mime.multipart import MIMEMultipart
from email.mime.text import MIMEText
from email.mime.application import MIMEApplication

sender_password = "spgz eeex ashh vbsz"
smtp_server = 'smtp.gmail.com'
smtp_port = 465

message = MIMEMultipart()
message['Subject'] = "this is a email with attachment test"
message['From'] = "samurai.jack33@gmail.com"
message['To'] = "wasaf1404@gmail.com"
body_part = MIMEText("This is the body of the text message")
message.attach(body_part)

with open('words_list.txt', 'rb') as file:
    message.attach(MIMEApplication(file.read(), Name="words_list.txt"))

with smtplib.SMTP_SSL(smtp_server, smtp_port) as server:
    server.login("samurai.jack33@gmail.com", sender_password)
    server.sendmail("samurai.jack33@gmail.com", "wasaf1404@gmail.com", message.as_string())
