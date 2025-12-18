### Setup

#### Install Broker

```
sudo apt install mosquitto

```

#### Init system on Linux

```
$ sudo systemctl enable mosquitto
$ sudo systemctl start mosquitto
$ sudo systemctl status mosquitto

```

#### Setup for Global connection

check
```
$ sudo ss -tlnp | grep 1883

```

edit conf
```
$ sudo nano /etc/mosquitto/mosquitto.conf

listener 1883 0.0.0.0
allow_anonymous true

```

restart

```
sudo systemctl restart mosquitto

```


#### Testing

pub 

```
$ mosquitto_pub -h 3.16.151.209 -t sensor/data -m '{"temperature":35.5,"tds":204,"ph":6.5}'

```

sub

```
$ mosquitto_sub -h 3.16.151.209 -t sensor/data

```
