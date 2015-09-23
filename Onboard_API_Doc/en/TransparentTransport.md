# Data Transparent Transmission

## Intro

The purpose of this document is to give an overview of the 'Data Transparent Transmission' between an Onboard Device and an Mobile Device. The rest of this document is organized as followed:
* Problem Motivation
* Usage Scenarios
* Sample Codes for implementation

Please be aware that the current upstream bandwidth (Mobile to Onboard Device) is around _1KB/s_ while the downstream bandwidth (Onboard to Mobile Device) is around _8KB/s_.

### Problem Motivation

DJI provides two types of APIs for developers to create their own applications: Mobile API and Onboard API. Mobile API allows developers to monitor and control the UAV from a mobile device running iOS or Android with the remote controller connected. Onboard API allows developers to monitor and control the UAV from any system directly connected to the UAV through a serial port (the UART interface).

Mobile API can be used without any other devices and allows developers to monitor the flight status easily. However, this configuration has some obvious limitations such as: a relatively low computing power, limited wireless bandwidth and unacceptable time latency for real-time or complex control.

Onboard API is implemented through the Onboard Device which is mounted on the UAV. Communication with the UAV is done directly through their serial ports. It provides sufficient computing power and stability for developers to run complex and demanding applications. Since the Onboard Device is mounted on the UAV, developers are not able to monitor the flight status from their programs. If program crashes, developers will have to manually control the UAV with the remote controller.

Data Transparent Transmission was developed to combine the benefits of these two APIs by establishing a connection between the Mobile Device and the Onboard Device. With this method, developers are able to send data from their mobile devices to the Onboard Devices to control over the program running on the Onboard Device. The Onboard Device can also send data to the mobile device for flight status monitoring and other functions.

In short, Data Transparent Transmission serves as a linkage between Mobile API and Onboard API, granting developers a greater flexibility in creating their applications.

![streamFrame](Images/streamFrame.png)

## USAGE - Onboard to Mobile

### Onboard Device to UAV

The Onboard SDK OPEN protocol between an Onboard Device and an UAV is introduced in another document. This section explains it in more detail.

The communication protocol is described as below:

    CMD set: 0x00
    CMD ID: 0xFE

|Data Type|Offset|Size|Description|
|---------|------|----|-----------|
|CMD Val|0|0~100|Data needs to be sent to Mobile Device|
|Ack Val|0|2| Return code 0: Success|

~~~c
char cmd_buf[10];
cmd_buf[0] = 0x00;
cmd_buf[1] = 0xFE;
memcpy(&cd_buf[2], "Hello!", 7);
Linklayer_Send(SESSION_MODE3,
                cmd_buf,
                9,
                0,
                200,
                3,
                0
);
~~~

### UAV to Mobile Device

This section use the Android system as an example. Users can enter the DJI-SDK-DEMO application to monitor the UAV status through relevant functions and use the Data Transparent Transmission function on the Controller State page.

**Note: For now, ONLY the DJI M100 flight platform supports this functionality.**

![SDK1](Images/SDKDemoMain.png)
![SDK2](Images/SDKDemoRelative.png)
![SDK3](Images/SDKDemoTTI.png)

The relative sample codes are shown as below:

- iOS

~~~cSharp
//Setting Delegation
inspireMC.mcDelegate = self;
  
//The legation function is called when receiving data
(void)mainController:(DJIMainController*)mc didReceivedDataFromExternalDevice:(NSData*)data {
//Here is the receiving data
NSLog(@"%@",data);
}
~~~
  
- Android

~~~java
//Receiving the data callback interface sent from UAV
DJIMainControllerExternalDeviceRecvDataCallBack mExtDevReceiveDataCallBack = null;
  
//Instantiate callback interface
mExtDevReceiveDataCallBack = new DJIMainControllerExternalDeviceRecvDataCallBack() {
@override
public void onResult(byte[] data) {
  //Here is the receiving data
}
};
  
//Setting callback interface
DJIDrone.getDjiMC().setExternalDeviceRecvDataCallBack(mExtDevReceiveDataCallBack);
~~~

## USAGE - Mobile to Onboard

### Mobile Device to UAV

The relevant sample code are shown as below:

#### iOS

  - Initialization
  
~~~cSharp
//Create DJI Drone object according to relative UAV type.
DJIDrone* drone = [DJIDrone droneWithType:DJIDrone_Inspire];
//Obtain Main controller object from DJI Drone object.
DJIInspireMainController* inspireMC = (DJIInspireMainController*)drone.mainController;
//Start data connection.
[drone connectToDrone];
~~~
  
  - Sending data.
  
~~~cSharp
  //Please note that data size should be no larger than 100 bytes.
  NSData* data = [NSData dataWithByte:"..."];
  //Sending data to peripheral and check the sending status through callback function.
  [inspireMC sendDataToExternalDevice:data withResult:(^(DJIError* error)){
    if(error.errorCode == ERR_Successed){
      //Data sent successfully.
    }
    else if(error.errorCode == ERR_InvalidParam) {
      //Data size is null or larger than 100 bytes.
    }
    else {
      //Data sent failed
    }
  }];
~~~
  
#### Android

~~~java
  //Data needs to be sent, please note that data size should be no larger than 100 bytes.
  byte[] data = {0};
  //Sending data to UAV
  DJIDrone.getDjiMC().sendDataToExternalDevice(data,new DJIExecuteResultCallback(){
    @override
    public void onResult(DJIError result) {
      //result is the callback status after sending data
      //1. result == DJIError.ERR_PARAM_IILEGAL,  Data size is null or larger than 100 bytes.
      //2. result == DJIError.ERR_TIMEOUT,        Data sent failed.
      //3. result == DJIError.RESULT_OK,          Data sent successfully.
    }
  });
~~~

### UAV to Onboard Device

Use this function with the same method mentioned previously. The communication protocol is described as below:

    CMD Set: 0x02
    CMD ID: 0x02

|Data Type|Offset|Size|Description|
|---------|------|----|-----------|
|Request Data|0|1~100|User defined data|
|Return Data|0|0|No return data|
