#/bin/bash


while [ true ]
do
    n=`rand`
    mod_n=$(($n%10))

    mock_temp=$((28+$mod_n))
    mock_humid=$((15+$mod_n))

    echo "Mock Temp: $mock_temp"
    echo "Mock Humid: $mock_humid"

    mosquitto_pub -h localhost -u admin -P $pass -p 1883 -t esp/1/temp -m $mock_temp
    mosquitto_pub -h localhost -u admin -P $pass -p 1883 -t esp/1/humid -m $mock_humid

    sleep 1

    n=`rand`
    mod_n=$(($n%10))

    mock_temp=$((28+$mod_n))
    mock_humid=$((15+$mod_n))

    echo "Mock Temp: $mock_temp"
    echo "Mock Humid: $mock_humid"

    mosquitto_pub -h localhost -u admin -P $pass -p 1883 -t esp/2/temp -m $mock_temp
    mosquitto_pub -h localhost -u admin -P $pass -p 1883 -t esp/2/humid -m $mock_humid

    sleep 1

    n=`rand`
    mod_n=$(($n%10))

    mock_temp=$((28+$mod_n))
    mock_humid=$((15+$mod_n))

    echo "Mock Temp: $mock_temp"
    echo "Mock Humid: $mock_humid"

    mosquitto_pub -h localhost -u admin -P $pass -p 1883 -t esp/3/temp -m $mock_temp
    mosquitto_pub -h localhost -u admin -P $pass -p 1883 -t esp/3/humid -m $mock_humid

    sleep 1

    n=`rand`
    mod_n=$(($n%10))

    mock_temp=$((28+$mod_n))
    mock_humid=$((15+$mod_n))

    echo "Mock Temp: $mock_temp"
    echo "Mock Humid: $mock_humid"

    mosquitto_pub -h localhost -u admin -P $pass -p 1883 -t esp/4/temp -m $mock_temp
    mosquitto_pub -h localhost -u admin -P $pass -p 1883 -t esp/4/humid -m $mock_humid

    sleep 3
done