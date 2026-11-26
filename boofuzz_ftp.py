from boofuzz import (Session, Target,
    TCPSocketConnection, s_get, s_initialize,
    s_static, s_string)

session = Session(
    target=Target(connection=TCPSocketConnection(
        "127.0.0.1", 2121)),
    check_data_received_each_request=True,
)

s_initialize("user")
s_static("USER ")
s_string("anonymous", name="username")
s_static("\r\n")

session.connect(s_get("user"))
session.fuzz()
