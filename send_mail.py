import smtplib, ssl

SERVER = "in-v3.mailjet.com"

FROM = "vsbaranov84@gmail.com"
TO = ["vsbaranov83@gmail.com"] # must be a list

SUBJECT = "Somebody in your appartment!"

TEXT = "This message was sent with Python's smtplib."

# Prepare actual message

message = """\
From: %s
To: %s
Subject: %s

%s
""" % (FROM, ", ".join(TO), SUBJECT, TEXT)

# Send the mail
context = ssl.create_default_context()
context = ssl.SSLContext(ssl.PROTOCOL_TLS)
#context.options |= ssl.OP_NO_SSLv2
#context.options |= ssl.OP_NO_SSLv3
#context.options |= ssl.OP_NO_TLSv1
#context.options |= ssl.OP_NO_TLSv1_1
#context.options |= ssl.OP_NO_COMPRESSION
#context.set_ciphers(DEFAULT_CIPHERS)
server = smtplib.SMTP(SERVER, 587)
server.ehlo()
server.starttls(context=context)
server.ehlo()
server.esmtp_features['auth'] = 'LOGIN PLAIN'
server.login('24ed635ed6e3e35437ce2e33415ea422', "16748719e3322b46a173b5e6791e7f85")
server.sendmail(FROM, TO, message)
server.quit()
