import { View, Text, Button } from 'react-native';
import { ScrollView, StyleSheet } from 'react-native';

import { useState, useEffect, useRef } from 'react';

import { TouchableOpacity } from 'react-native';


/*
1. 
npm install react_native_mqtt // expo 
npm install react_native_mqtt --legacy-peer-deps

2. 
npm install @react-native-async-storage/async-storage // expo? 
npm install @react-native-async-storage/async-storage --legacy-peer-deps

Issues : 
2a. ReferenceError: Property 'Paho' doesn't exist // REBUILT!!

2b. Android Bundled 13447ms node_modules\expo-router\entry.js // MAKE SURE THIS MESSAGE APPEAR
*/
import init from 'react_native_mqtt';
import AsyncStorage from '@react-native-async-storage/async-storage';



//---------------------------------------------------------------------------// 
export function RoundedButton ({ onPress = ()=>{}, message = 'Press me' })
{
  return (
    <TouchableOpacity style={styles.button} onPress={onPress}>
      <Text style={styles.text}>{message}</Text>
    </TouchableOpacity>
  );
};

const styles = StyleSheet.create({
  button: {
    backgroundColor: '#007bff',
    paddingVertical: 10,
    paddingHorizontal: 20,
    borderRadius: 25, // makes it rounded
    alignItems: 'center',
    marginVertical: 3,
    marginHorizontal: 5,  
  },
  text: {
    color: '#fff',
    fontSize: 16,
  },
});

//---------------------------------------------------------------------------// 
//---------------------------------------------------------------------------// 
export default function TestA() 
{
    return (
        <Text>
            <CurrentTime /> 
            { "\n" } 
            <MQTTClient />
        </Text>
    );
};
    

//---------------------------------------------------------------------------// 
//---------------------------------------------------------------------------// 
export function CurrentTime () 
{
    //let data = useSpicyData(); 

    const [time, setTime] = useState(new Date().toLocaleTimeString());

    useEffect(() => {
    const interval = setInterval(() => 
    {
        let t = new Date().toLocaleTimeString(); 
        setTime( t );
        //data.setTime( t ); 
    }, 1000);
    return () => clearInterval(interval);
    }, []);

    return (
        <View> 
            <Text>
                <Text style={{ color: 'red' }}>Current Time: </Text>
                <Text style={{ color: 'blue' }}>'{time}'</Text>
            </Text>
        </View>
    );
    //return ( <Text> </Text> );
};
  
//---------------------------------------------------------------------------// 
//---------------------------------------------------------------------------// 
// Init MQTT storage backend
const options = {
    host: 'test.mosquitto.org',
    path: '/mqtt',
    port: 8081,
};


init({
    size: 10000,
    storageBackend: AsyncStorage,
    defaultExpires: 1000 * 3600 * 24,
    enableCache: true,
    sync: {},
});


export const MQTTClient = () => 
{
        const clientRef = useRef(null);

        const [logs, setLogs] = useState([]);
        const [connected, setConnected] = useState(false);
 
        const [xData, setXData] = useState([]);
        const [yData, setYData] = useState([]);

        //let data = useSpicyData(); 

        const appendLog = (msg) => {
          setLogs((prev) => [...prev, msg]);
        };
    
        const connectClient = () => {
          if (connected) {
            appendLog('⚠️ Already connected');
            return;
          }
    
          const client = new Paho.MQTT.Client(options.host, options.port, options.path);
          clientRef.current = client;
    
          const onConnect = () => {
            appendLog('✅ Connected');
            setConnected(true);
            client.subscribe('v1/devices/me/telemetry');
            appendLog('📡 Subscribed to topic: v1/devices/me/telemetry');
          };
    
          const onConnectionLost = (responseObject) => {
            if (responseObject.errorCode !== 0) {
              appendLog('❌ Connection lost: ' + responseObject.errorMessage);
            }
            setConnected(false);
          };
    

          const onMessageArrived = (message) => {
            appendLog('📩 Message: ' + message.payloadString);
      
            try {
              const parsedArray = JSON.parse(message.payloadString); // Expects an array
              const newX = parsedArray.map(item => parseFloat(item.x));
              const newY = parsedArray.map(item => parseFloat(item.y));
              const newT = parsedArray.map(item => parseFloat(item.t));

              setXData((prev) => [...prev, ...newX]);
              setYData((prev) => [...prev, ...newY]);
//data.updateData(newT, newY); 

            } catch (err) {
              appendLog('❌ JSON parsing error');
            }
          };          
    
          client.onConnectionLost = onConnectionLost;
          client.onMessageArrived = onMessageArrived;
    
          client.connect({
            onSuccess: onConnect,
            useSSL: true,
          });
        };
    
        const disconnectClient = () => {
          const client = clientRef.current;
          if (client && client.isConnected()) {
            client.disconnect();
            appendLog('🔌 Disconnected manually');
            setConnected(false);
          }
        };
    
        useEffect(() => {
          return () => {
            const client = clientRef.current;
            if (client && client.isConnected()) {
              client.disconnect();
              appendLog('🔌 MQTT client disconnected on unmount');
            }
          };
        }, []);
    
        return (
          <View style={styles_1.container}>
            
            <Text style={styles_1.title}>MQTT Client Running...</Text>
    
            <ScrollView style={styles_1.logBox}>
              {[...logs].reverse().map((log, idx) => (
                <Text key={idx} style={styles_1.logText}>• {log}</Text>
              ))}
            </ScrollView>

{/*
            <Button title="Connect" onPress={connectClient} />
            <Button title="Disconnect" onPress={disconnectClient} /> 
            <RoundedButton message={"RoundedButton"}/> 
*/}
            <RoundedButton message={"Connect"} onPress={connectClient} /> 
            <RoundedButton message={"Disconnect"} onPress={disconnectClient} /> 

          </View>
        );
};
    

const styles_1 = StyleSheet.create({
    container: { padding: 16 },
    title: { fontSize: 16, fontWeight: 'bold', marginBottom: 10 },
    logBox: { maxHeight: 200, backgroundColor: '#eee', padding: 10, borderRadius: 6 },
    logText: { fontSize: 14, color: '#333', marginBottom: 4 },
});
  

//---------------------------------------------------------------------------// 
//---------------------------------------------------------------------------// 