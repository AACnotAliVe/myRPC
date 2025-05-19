Place service here!
/lib/systemd/system/myRPC-server.service - final


ADD
/usr/local/bin/myRPC-server -c /etc/myRPC/myRPC.conf



USE
/etc/myRPC/users.conf , /etc/myRPC/myRPC.conf
