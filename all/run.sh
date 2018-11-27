bash c.sh
gcc server.c -o server -lpthread -lmysqlclient
gnome-terminal -t "client" -x bash "exec bash;"
sudo ./server


