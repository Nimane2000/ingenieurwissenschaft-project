# IoT compose

## Quick started
deploy the stack into your docker swarm, this stack includes node-red, influxdb, mosqutti server, grafana which mean you can expect all the things work after deploying.
```sh
$ docker stack deploy -c iot-stack.yml iot-stack
```
After deploying, influxdb will run on port `8086`, mosquitto on port `1883`, node-red on port `1880`, and grafana on port `3000`

# Docker
Docker is an open platform for developing, shipping, and running applications. Docker enables you to separate your applications from your infrastructure so you can deliver software quickly. With Docker, you can manage your infrastructure in the same ways you manage your applications. By taking advantage of Docker's methodologies for shipping, testing, and deploying code, you can significantly reduce the delay between writing code and running it in production.  

For installed instruction please have a look at [here](https://docs.docker.com/desktop/install/linux-install/)


# Docker Compose
Docker Compose is a tool that helps you define and share multi-container applications. With Compose, you can create a YAML file to define the services and with a single command, you can spin everything up or tear it all down.

You can try using the command below
```sh
$ docker-compose up
```

# Docker swarm
Swarm mode is an advanced feature for managing a cluster of Docker daemons.
Use Swarm mode if you intend to use Swarm as a production runtime environment

If your PC haven't initial docker swarm node yet, please initial docker swarm before deploying stack
```sh
$ docker swarm init
```

# Deploying 
to deploy your (docker) stack into your docker swarm, please try the command below.
```sh
$ docker stack deploy -c iot-stack.yml iot-stack
```

# Mosquitto 
## [Setting up Authentication in Mosquitto MQTT Broker](https://medium.com/@eranda/setting-up-authentication-on-mosquitto-mqtt-broker-de5df2e29afc)
Mosqutto requires password for authentication, please provide the file below with your username and password.

Create `password.txt` file
```txt
user1:pass1
user2:pass2
```
Encryp password
```sh
$ mosquitto_passwd -U ./password.txt
```

Edit configuration file `/etc/mosquitto/mosquitto.conf`
```conf
password_file /etc/mosquitto/passwd
allow_anonymous false
```

# InfluxDB and Grafana
Edit file `default.env` or add `.env` file in your work directory with the modified variable below
```
INFLUXDB_USERNAME=<edit>
INFLUXDB_PASSWORD=<edit>

GRAFANA_USERNAME=<edit>
GRAFANA_PASSWORD=<edit>
```