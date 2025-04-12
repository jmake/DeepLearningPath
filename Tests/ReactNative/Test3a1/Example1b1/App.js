import React, { useEffect, useState } from 'react';
import {
  ScrollView,
  StatusBar,
  StyleSheet,
  Text,
  useColorScheme,
  View,
} from 'react-native';

import {
  Colors,
  DebugInstructions,
  Header,
  LearnMoreLinks,
  ReloadInstructions,
} from 'react-native/Libraries/NewAppScreen';

import init from 'react_native_mqtt';
import { Input, Button } from '@rneui/base';
import AsyncStorage from '@react-native-async-storage/async-storage';

// Init MQTT storage backend
init({
  size: 10000,
  storageBackend: AsyncStorage,
  defaultExpires: 1000 * 3600 * 24,
  enableCache: true,
  sync: {},
});

const options = {
  host: 'test.mosquitto.org',
  port: 8081,
  path: '/mqtt',
};

const styles_1 = StyleSheet.create({
  container: { padding: 16 },
  title: { fontSize: 16, fontWeight: 'bold', marginBottom: 10 },
  logBox: { maxHeight: 200, backgroundColor: '#eee', padding: 10, borderRadius: 6 },
  logText: { fontSize: 14, color: '#333', marginBottom: 4 },
});

const CurrentTime = () => {
  const [time, setTime] = useState(new Date().toLocaleTimeString());

  useEffect(() => {
    const interval = setInterval(() => {
      setTime(new Date().toLocaleTimeString());
    }, 1000);
    return () => clearInterval(interval);
  }, []);

  return <Text>Current Time: {time}</Text>;
};

const MQTTClient = () => {
  const [logs, setLogs] = useState([]);

  const appendLog = (msg) => {
    setLogs((prev) => [...prev, msg]);
  };

  useEffect(() => {
    const client = new Paho.MQTT.Client(options.host, options.port, options.path);

    const onConnect = () => {
      appendLog('✅ Connected');
      client.subscribe('v1/devices/me/telemetry');
      appendLog('📡 Subscribed to topic: v1/devices/me/telemetry');
    };

    const onConnectionLost = (responseObject) => {
      if (responseObject.errorCode !== 0) {
        appendLog('❌ Connection lost: ' + responseObject.errorMessage);
      }
    };

    const onMessageArrived = (message) => {
      appendLog('📩 Message: ' + message.payloadString);
    };

    client.onConnectionLost = onConnectionLost;
    client.onMessageArrived = onMessageArrived;

    client.connect({
      onSuccess: onConnect,
      useSSL: true,
    });

    return () => {
      if (client.isConnected()) {
        client.disconnect();
        appendLog('🔌 MQTT client disconnected');
      }
    };
  }, []);

  return (
    <View style={styles_1.container}>
      <Text style={styles_1.title}>MQTT Client Running...</Text>
      <ScrollView style={styles_1.logBox}>
        {logs.map((log, idx) => (
          <Text key={idx} style={styles_1.logText}>• {log}</Text>
        ))}
      </ScrollView>
    </View>
  );
};

const Section = ({ children, title }) => {
  const isDarkMode = useColorScheme() === 'dark';
  return (
    <View style={styles.sectionContainer}>
      <Text
        style={[
          styles.sectionTitle,
          { color: isDarkMode ? Colors.white : Colors.black },
        ]}>
        {title}
      </Text>
      <Text
        style={[
          styles.sectionDescription,
          { color: isDarkMode ? Colors.light : Colors.dark },
        ]}>
        {children}
      </Text>
    </View>
  );
};

const App = () => {
  const isDarkMode = useColorScheme() === 'dark';

  const backgroundStyle = {
    backgroundColor: isDarkMode ? Colors.darker : Colors.lighter,
  };

  const safePadding = '5%';

  return (
    <View style={backgroundStyle}>
      <StatusBar
        barStyle={isDarkMode ? 'light-content' : 'dark-content'}
        backgroundColor={backgroundStyle.backgroundColor}
      />
      <ScrollView style={backgroundStyle}>
        <View style={{ paddingRight: safePadding }}>
          <Header />
        </View>
        <View
          style={{
            backgroundColor: isDarkMode ? Colors.black : Colors.white,
            paddingHorizontal: safePadding,
            paddingBottom: safePadding,
          }}>
          <Section title="Step One">
            <CurrentTime />{"\n"}
            <MQTTClient />{"\n"}
            Edit <Text style={styles.highlight}>App.js</Text> ...{"\n"}
          </Section>

          <Section title="See Your Changes">
            <ReloadInstructions />
          </Section>
          <Section title="Debug">
            <DebugInstructions />
          </Section>
          <Section title="Learn More">
            Read the docs to discover what to do next:
          </Section>
          <LearnMoreLinks />
        </View>
      </ScrollView>
    </View>
  );
};

const styles = StyleSheet.create({
  sectionContainer: {
    marginTop: 32,
    paddingHorizontal: 24,
  },
  sectionTitle: {
    fontSize: 24,
    fontWeight: '600',
  },
  sectionDescription: {
    marginTop: 8,
    fontSize: 18,
    fontWeight: '400',
  },
  highlight: {
    fontWeight: '700',
  },
});

export default App;