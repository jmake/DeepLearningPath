import { BleManager } from 'react-native-ble-plx';

import React, { useEffect, useState } from 'react';

import { PermissionsAndroid, Platform, FlatList, StyleSheet } from 'react-native';
import { View, Button, Text, Pressable } from 'react-native';
import { TouchableOpacity } from 'react-native';


//---------------------------------------------------------------------------// 
//---------------------------------------------------------------------------// 
let currentDevice = null;

const manager = new BleManager();


//---------------------------------------------------------------------------// 
const requestPermissions = async () => 
{
  if (Platform.OS === 'android') {
    await PermissionsAndroid.requestMultiple([
      PermissionsAndroid.PERMISSIONS.ACCESS_FINE_LOCATION,
      PermissionsAndroid.PERMISSIONS.BLUETOOTH_SCAN,
      PermissionsAndroid.PERMISSIONS.BLUETOOTH_CONNECT,
    ]);
  }
};

const IMU_UUID = "19b10020-e8f2-537e-4f6c-d104768a1214"; 


//---------------------------------------------------------------------------// 
function imu_update(base64Value) 
{
  const buffer = new Uint8Array(atob(base64Value).split("").map(char => char.charCodeAt(0)));

  const floatArray = new Float32Array(buffer.buffer);
  console.log( Array.from(floatArray) ); 
}


async function characteristic_general(chars) 
{
  if(!currentDevice) return; 
  
  console.log(`[characteristic_general] '${chars.uuid}'`);
  if (chars.uuid !== IMU_UUID) return;

  chars.monitor((error, char) => 
  {
    if (error) 
    {
      console.warn(`[characteristic_general] error : '${error}'`);
      return;
    }

    if (char?.value) 
    {
      console.log(`[characteristic_general] '${chars.uuid}' '${char.value}' `);
      imu_update(char.value); 
    }
    else
    {
      console.log(`[characteristic_general] '${chars.uuid}' 'NO FOUND!' `);
    }
  });
}


//---------------------------------------------------------------------------// 
async function connectToDevice(device) 
{
  try 
  {
    console.log('[connectToDevice] Disconnecting from all connected devices...');

    if(currentDevice) 
    {
      console.log(`🔌 Disconnecting: '${currentDevice.name}' `);
      await manager.cancelDeviceConnection(currentDevice.id);
      currentDevice = null;      
    }

    console.log(`🔌 Connecting to device: ${device.name}`);

    const connectedDevice = await manager.connectToDevice(device.id);
    console.log(`+ '${connectedDevice.name}' connected!`);

    currentDevice = connectedDevice; 

    await connectedDevice.discoverAllServicesAndCharacteristics();
    const services = await connectedDevice.services();

    for (const service of services) 
    {
      console.log(`  [${connectedDevice.name}] Service: ${service.uuid}`);

      const characteristics = await service.characteristics();
      for (const characteristic of characteristics) 
      {
        console.log(`    [${connectedDevice.name}] Characteristic: ${characteristic.uuid}`);
        characteristic_general(characteristic); 
      }
    }

    return connectedDevice;
  } 
  catch (error) 
  { 
    console.log(`[connectToDevice] Connection error: ${error}`);
  }
}


//---------------------------------------------------------------------------// 
export function UIMain(props) 
{
  const styles = StyleSheet.create({
    container: { flex: 1, padding: 20, paddingTop: 50 },
    device: { marginVertical: 8, fontSize: 16, color: 'blue' },
  });

  const [connected, setConnected] = useState(false);
  const [connecting, setConnecting] = useState(false);
  const [connectedDevice, setConnectedDevice] = useState(null);

  const handleDevicePress = async (device) => {
    setConnecting(true);
    setConnectedDevice(device); // Set early for title during connecting

    const result = await connectToDevice(device);
    console.log(`[UIMain] '${result.name}' connected!`);

    if (result) {
      setConnected(true);
      setConnectedDevice(result);
    }

    setConnecting(false);
  };

  useEffect(() => {
    setConnected(false);
    setConnectedDevice(null);
    setConnecting(false);
  }, [props.devices]);

  const getScanButtonTitle = () => {
    if (props.isScanning) return "Scanning...";
    if (connecting && connectedDevice) return `${connectedDevice.name} connecting...`;
    if (connected && connectedDevice) return `${connectedDevice.name} (Disconnect)`;
    return "Scan for BLE Devices";
  };

  return (
    <View style={styles.container}>
      <CurrentTime disabled={props.isScanning} />

      <RoundedButton 
        title={getScanButtonTitle()} 
        onPress={props.startScan} 
        disabled={props.isScanning || connecting} 
      />

      <FlatList
        data={props.devices}
        keyExtractor={(item) => item.id}
        renderItem={({ item }) => (
          <RoundedButton 
            color="green"
            title={`${item.name} (${item.id})`} 
            onPress={() => handleDevicePress(item)} 
            disabled={connected || connecting || props.isScanning} 
          />
        )}
      />
    </View>
  );
}

/*
export function UIMain (props) 
{
  const styles = StyleSheet.create({
    container: { flex: 1, padding: 20, paddingTop: 50 },
    device: { marginVertical: 8, fontSize: 16, color: 'blue' },
  });

  const [connected, setConnected] = useState(false);
  const [connecting, setConnecting] = useState(false);

  const handleDevicePress = async (device) => 
  {
    setConnecting(true); 

    const connectedDevice = await connectToDevice(device);
    console.log(`[UIMain] '${connectedDevice.name}' connected! `);

    if (connectedDevice) setConnected(true);
    setConnecting(false); 
  };  
  
  useEffect(() => 
  {
    setConnected(false);
    setConnecting(false);
  }, [props.devices]);
  
  return (
    <View style={styles.container}>
      <CurrentTime disabled={props.isScanning} />

      <RoundedButton 
        title="Scan for BLE Devices" 
        onPress={props.startScan} 
        disabled={props.isScanning} 
      />

      <FlatList
        data={props.devices}
        keyExtractor={(item) => item.id}
        renderItem={({ item }) => 
        (
          <RoundedButton 
            color={"green"}
            title={ `${item.name} (${item.id})` } 
            onPress={ () => handleDevicePress(item) } 
            disabled={ connected || connecting || props.isScanning } 
          />  
        )}
      />
    </View>
  );
}
*/

//---------------------------------------------------------------------------// 
const App = () => 
{
  console.log("[BLE] Running...");

  const [devices, setDevices] = useState([]);
  const [isScanning, setIsScanning] = useState(false); 

  useEffect(() => 
  {
    requestPermissions();
    //return () => manager.destroy();
    return () => manager.stopDeviceScan();
  }, []);

  const startScan = async () => 
  {
    setDevices([]);
    setIsScanning(true); 

    if( currentDevice ) 
    {
      console.log(`[BLE] Disconnecting: '${currentDevice.name}' `);
      await manager.cancelDeviceConnection(currentDevice.id);
      currentDevice = null;      
      setIsScanning(true);
    }
    else
    {
      console.log(`[BLE] 'currentDevice' no found!`);
    }
    
    manager.startDeviceScan(null, null, (error, device) => 
    {
      if (error) 
      {
        setIsScanning(false);
        console.warn(error);
        return;
      }

      if (device?.name) 
      {
        console.log(`[BLE] Found device: '${device.name}'`);

        setDevices((prev) => {
          if (prev.find((d) => d.id === device.id)) return prev;
          return [...prev, device];
        });
      }
    });

    setTimeout( async () => 
    {
      setIsScanning(false); 
      manager.stopDeviceScan(); 
      console.log(`[BLE] Scanning end!`); 
      return stop;  
    }, 5000);

  };

  return (
    <UIMain 
      devices={devices} 
      startScan={startScan} 
      isScanning={isScanning} 
    />
  ); 
};

export default App;


//---------------------------------------------------------------------------// 
//---------------------------------------------------------------------------// 
export function RoundedButton ( {color='#007bff', ...props} )
{
  const styles = StyleSheet.create({
    button: {
      alignItems: 'center',
      borderRadius: 15, 
      marginVertical: 3,
      paddingVertical: 10,
      marginHorizontal: 5,  
      paddingHorizontal: 20,
      backgroundColor: color,
    },
    text: {
      color: '#fff',
      fontSize: 16,
    },
    disabledButton: {
      opacity: 0.6,
      backgroundColor: '#888',
    },
  });
  
  return (
    <TouchableOpacity 
      style={[styles.button, props.disabled && styles.disabledButton]}
      onPress={props.onPress} 
      disabled={props.disabled} 
    >
      <Text style={styles.text}>{props.title}</Text>
    </TouchableOpacity>
  );
};


//---------------------------------------------------------------------------// 
//---------------------------------------------------------------------------// 
export function CurrentTime ( props ) 
{
    //let data = useSpicyData(); 

    const [time, setTime] = useState(new Date().toLocaleTimeString());

    useEffect(() => {
    const interval = setInterval(() => 
    {
        let t = new Date().toLocaleTimeString(); 
        setTime( t );
        //data.setTime( t ); 
        //if(!props.disabled) 
        console.log(`[CurrentTime] time:${t}`);
    }, 1000);
    return () => clearInterval(interval);
    }, []);

    return (
        <View> 
            <Text>
                <Text style={{ color: 'red' }}>Current Time: </Text>
                <Text style={{ color: 'white' }}>'{time}'</Text>
            </Text>
        </View>
    );
    //return ( <Text> </Text> );
};



//---------------------------------------------------------------------------// 
//---------------------------------------------------------------------------// 

/*
async function connectToDevice(device) 
{
  try {
    console.log(`🔌 Connecting to device: ${device.name}`);

    const connectedDevice = await manager.connectToDevice(device.id);
    console.log('✅ Connected:', connectedDevice.name);

    await connectedDevice.discoverAllServicesAndCharacteristics();
    const services = await connectedDevice.services();

    for (const service of services) {
      console.log(`🔧 Service: ${service.uuid}`);

      const characteristics = await service.characteristics();

      for (const characteristic of characteristics) {
        console.log(`📍 Characteristic: ${characteristic.uuid}`);
      }
    }

    return connectedDevice;
  } catch (error) {
    console.warn('❌ Connection error:', error);
  }
}
*/


/*
const App = () => 
{
  console.log("App Running..."); 

  const [devices, setDevices] = useState([]);

  useEffect(() => {
    requestPermissions();
    return () => manager.destroy();
  }, []);


  const startScan = () => 
  {
    setDevices([]);
    manager.startDeviceScan(null, null, (error, device) => {
      if (error) {
        console.warn(error);
        return;
      }

      if (device?.name) 
      {
        console.log(`Found device: '${device.name}' ` ); 

        setDevices((prev) => 
        {
          if (prev.find((d) => d.id === device.id)) return prev;
          return [...prev, device];
        });
      }
    });

    setTimeout(() => manager.stopDeviceScan(), 10000);
  };

  return (
    <View style={styles.container}>
      <CurrentTime />
      <Button title="Scan for BLE Devices..." onPress={startScan} />
      <FlatList
        data={devices}
        keyExtractor={(item) => item.id}
        renderItem={({ item }) => (
          <Text style={styles.device}>{item.name} ({item.id})</Text>
        )}
      />
    </View>
  );
};


const styles = StyleSheet.create({
  container: { flex: 1, padding: 20, paddingTop: 50 },
  device: { marginVertical: 8, fontSize: 16 },
});

export default App;
*/