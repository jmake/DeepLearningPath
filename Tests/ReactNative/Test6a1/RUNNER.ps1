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

# -no-snapshot-load | -no-snapshot-save | -wipe-data
#emulator -avd Medium_Phone_API_36 -no-snapshot-save 

adb kill-server
adb start-server
adb devices

npx react-native run-android --device 1090018024064335
#npx react-native run-android --device 

#Read-Host "Remove USB..."
#emulator -avd Medium_Phone_API_36 -no-snapshot-load

exit 
# Expo go!
#npx expo start --clear --android 
# exit

###& "D:\z2025_1\Downloads\Expo.Orbit-2.0.2-x64.Setup.exe"

#npx @react-native-community/cli@latest init Example6b1  
#cd Example6b1

#npx react-native start --reset-cache 

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

<#

npx @react-native-community/cli@latest init Example3a1

## For react-native@>=0.78 and react@>=19, you need to use @shopify/react-native-skia@next
npm install @shopify/react-native-skia@next

yarn add @wuba/react-native-echarts echarts

#>

<#
adb tcpip 5555
Start-Sleep -Seconds 5

adb shell ip route

Read-Host "Remove USB... "

$ipRoute = adb shell ip route
if ($ipRoute -match 'src (\d{1,3}(\.\d{1,3}){3})') {
    $ip = $Matches[1]
    echo $ip 
    adb connect $ip
} else {
    Write-Host "IP address not found in route output."
}
#>

