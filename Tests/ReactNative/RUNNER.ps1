clear

$originalPath = $env:Path

$env:JAVA_HOME = "D:\z2025_1\Android\Studio\jbr"
$env:ANDROID_HOME = "D:\z2025_1\Android\SDK\"

## adb.exe 
## java.exe 
## emulator.exe 
$env:Path += ";$env:JAVA_HOME\bin"
$env:Path += ";$env:ANDROID_HOME\emulator"
$env:Path += ";$env:ANDROID_HOME\platform-tools"

adb --version
java --version 
emulator -list-avds

###& "D:\z2025_1\Downloads\Expo.Orbit-2.0.2-x64.Setup.exe"
#
### npx @react-native-community/cli@latest init MyAppX ??
### cd MyAppX
### npx react-native start  ?

## npx react-native start --reset-cache 
npx react-native run-android

$env:Path = $originalPath

ls .\android\app\build\outputs\apk\debug\app-debug.apk

## cd android
## .\gradlew assembleDebug

<#

https://github.com/react-native-community/cli

https://reactnative.dev/docs/getting-started-without-a-framework

https://microsoft.github.io/react-native-windows/docs/getting-started

https://dev.to/aneeqakhan/step-by-step-guide-running-your-first-react-native-android-app-in-2023-1hh6

#>

<#
EXPO : 

npm install create-expo-app

npx create-expo-app@latest MyApp3 

cd MyApp3

npm run web 



#>

<#
https://www.emqx.com/en/blog/how-to-use-mqtt-in-react-native

npm install react_native_mqtt  

npm install @rneui/base @rneui/themed
#>