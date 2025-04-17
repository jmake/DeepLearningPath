import * as React from 'react';
import { useState } from 'react'; 

// npm install react-native-paper 
import { MD3DarkTheme as DarkTheme } from 'react-native-paper';
import { Provider as PaperProvider, Text, Appbar } from 'react-native-paper';
import { SafeAreaView, StyleSheet, View, ScrollView, Dimensions } from 'react-native';

import TestA from "./SpicyMQTT";
import { CurrentTime, MQTTClient } from "./SpicyMQTT";
import { RoundedButton } from "./SpicyMQTT";

import spicyBLE from "./SpicyBLE";


const ViewPage1a = ({style}) => 
{
  return (
    <View>
      <Text style={style}>
        <RoundedButton message={"Message"} />
        <CurrentTime /> 
        { "\n" } 
        <MQTTClient />
      </Text>
    </View>
    );
};

const ViewPage2a = ({style}) => 
{
  return (
    <View>
        <TestA style={style}/> 
    </View>
  );
};
  

const marginVertical = 4; 
const screenHeight = Dimensions.get('window').height;

const childrenViews1 = [
  <ViewPage1a style={{ color: '#fff', fontSize: 20 }} />, 
  <Text style={{ color: '#fff', fontSize: 20 }}>Page 1a</Text>,
  <ViewPage2a style={{ color: '#fff', fontSize: 20 }} />, 
  <Text style={{ color: '#fff', fontSize: 20 }}>Page 1c</Text>,
];

const childrenViews2 = [
  <Text style={{ color: '#fff', fontSize: 20 }}>Page 2a</Text>,
  <Text style={{ color: '#fff', fontSize: 20 }}>Page 2b</Text>,
  <Text style={{ color: '#fff', fontSize: 20 }}>Page 2c</Text>,
];

const childrenViews3 = [
  <Text style={{ color: '#fff', fontSize: 20 }}>Page 3a</Text>,
  <Text style={{ color: '#fff', fontSize: 20 }}>Page 3b</Text>,
  <Text style={{ color: '#fff', fontSize: 20 }}>Page 3c</Text>,
];


const CreateHorizontalViewItem = ({ availableHeight, childrenViews}) => {
  const screenWidth = Dimensions.get('window').width;

  const styles = StyleSheet.create({
    container: {
      flexDirection: 'row',
    },
  });


  return (
    <ScrollView
      horizontal
      pagingEnabled
      snapToInterval={screenWidth}
      decelerationRate="fast"
      snapToAlignment="start"
      showsHorizontalScrollIndicator={false}
    >
      <View style={styles.container}>
        <HorizontalPageItem
          childrenViews={childrenViews}
          screenWidth={screenWidth}
          availableHeight={availableHeight}
        />
      </View>
    </ScrollView>
  );
};


const HorizontalPageItem = ({ childrenViews, screenWidth, availableHeight }) => {
  const styles = StyleSheet.create({
    item: {
      width: screenWidth,
      borderRadius: 16,
      height: availableHeight,
      justifyContent: 'center',
      alignItems: 'center',
      backgroundColor: '#333',
      marginVertical: marginVertical,
    },
  });

  return (
    <>
      {childrenViews.map((childView, index) => (
        <View key={index} style={styles.item}>
          {childView}
        </View>
      ))}
    </>
  );
};



const CenterContent = ({ availableHeight }) => {
  const styles = StyleSheet.create({
    center: {padding: 0},
  });
  return (
    <ScrollView
      contentContainerStyle={styles.center}
      pagingEnabled
      snapToInterval={availableHeight}
      decelerationRate="fast"
      snapToAlignment="start"
      showsVerticalScrollIndicator={false}
      nestedScrollEnabled={true}
    >
      <CreateHorizontalViewItem availableHeight={availableHeight} childrenViews={childrenViews1}/>
      <CreateHorizontalViewItem availableHeight={availableHeight} childrenViews={childrenViews2}/>
      <CreateHorizontalViewItem availableHeight={availableHeight} childrenViews={childrenViews3}/>
    </ScrollView>
  );
};


const Header = ({ onLayout }) => 
{
  const styles = StyleSheet.create({
    wrapper: {
      borderBottomLeftRadius: 16,
      borderBottomRightRadius: 16,
      overflow: 'hidden',
    },
    header: {
      backgroundColor: '#1E1E1E',
      height: 50
    },
  });

  return (
    <View style={styles.wrapper} onLayout={onLayout}>
      <Appbar.Header style={styles.header}>
        <Appbar.Content title="Header Area" />
      </Appbar.Header>
    </View>
  );
};

const Footer = ({ onLayout }) => {
  const styles = StyleSheet.create({
    wrapper: {
      borderTopLeftRadius: 16,
      borderTopRightRadius: 16,
      overflow: 'hidden',
    },
    footer: {
      padding: 8,
      backgroundColor: '#1E1E1E',
      alignItems: 'center',
    },
  });

  return (
    <View style={styles.wrapper} onLayout={onLayout}>
      <View style={styles.footer}>
        <Text variant="bodyMedium">Footer Area</Text>
      </View>
    </View>
  );
};


export default function Main() 
{
  const [headerHeight, setHeaderHeight] = React.useState(0);
  const [footerHeight, setFooterHeight] = React.useState(0);

  const onHeaderLayout = (event) => {
    const { height } = event.nativeEvent.layout;
    setHeaderHeight(height);
  };

  const onFooterLayout = (event) => {
    const { height } = event.nativeEvent.layout;
    setFooterHeight(height);
  };

  const availableHeight = screenHeight - headerHeight - footerHeight - 2*marginVertical; 

  return (
    <PaperProvider theme={theme}>
      <SafeAreaView style={styles.container}>
        <Header onLayout={onHeaderLayout} />
        <CenterContent availableHeight={availableHeight} />
        <Footer onLayout={onFooterLayout} />
      </SafeAreaView>
    </PaperProvider>
  );
}

const theme = {
  ...DarkTheme,
  colors: {
    ...DarkTheme.colors,
    primary: '#BB86FC',
  },
};

const styles = StyleSheet.create({
  container: {
    flex: 1,
    backgroundColor: DarkTheme.colors.background,
  },
});