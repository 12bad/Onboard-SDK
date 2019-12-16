/** @file dji_vehicle.hpp
 *  @version 3.3
 *  @date April 2017
 *
 *  @brief
 *  Vehicle API for DJI onboardSDK library
 *
 *  @Copyright (c) 2017 DJI
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 */

#ifndef OSDK_CORE_INC_DJI_VEHICLE_H_
#define OSDK_CORE_INC_DJI_VEHICLE_H_

#include <cstdint>
#include "dji_status.hpp"
#include "dji_ack.hpp"
#include "dji_type.hpp"
#include "dji_vehicle_callback.hpp"
#include "dji_version.hpp"
#include "dji_linker.hpp"
#include "dji_legacy_linker.hpp"
#include "dji_log.hpp"
#include "dji_broadcast.hpp"
#include "dji_gimbal.hpp"
#include "dji_camera.hpp"
#include "dji_control.hpp"
#include "dji_subscription.hpp"
#include "dji_mobile_communication.hpp"
#include "dji_mobile_device.hpp"
#include "dji_payload_device.hpp"
#include "dji_virtual_rc.hpp"
#include "dji_hardware_sync.hpp"
#include "dji_mfio.hpp"
#include "dji_mission_manager.hpp"
#include "dji_camera_manager.hpp"
#include "dji_gimbal_manager.hpp"
#include "dji_flight_controller.hpp"
#include "dji_psdk_manager.hpp"

namespace DJI
{
namespace OSDK
{



/*! @brief A top-level encapsulation of a DJI drone/FC connected to your OES.
 *
 * @details This class instantiates objects for all features your drone/FC
 * supports.
 * Create a Vechile object in your code and you will have access to the entire
 * DJI OSDK API.
 *
 */
class Vehicle
{
public:
#pragma pack(1)
  typedef struct ActivateData
  {
    uint32_t ID;
    uint32_t reserved;
    uint32_t version;
    uint8_t  iosID[32]; //! @note useless
    char*    encKey;
  } ActivateData; // pack(1)
#pragma pack()

public:
  Vehicle(Linker* linker);
  ~Vehicle();

  Linker*              linker;
  LegacyLinker*        legacyLinker;
  DataSubscription*    subscribe;
  DataBroadcast*       broadcast;
  Control*             control;
  Camera*              camera;
  Gimbal*              gimbal;
  MFIO*                mfio;
  MobileCommunication* moc;
  MobileDevice*        mobileDevice;
  MissionManager*      missionManager;
  HardwareSync*        hardSync;
  // Supported only on Matrice 100
  VirtualRC*           virtualRC;
  PayloadDevice*       payloadDevice;
  CameraManager*       cameraManager;
  FlightController*    flightController;
  PSDKManager*         psdkManager;
  GimbalManager*       gimbalManager;

  int functionalSetUp();
  ////////// Blocking calls ///////////

  /**
   * @brief
   * Send activation request to your flight controller
   * to check if: \n a) your application registered in your developer
   * account \n b) API Control enabled in the Assistant software\n\n
   * Proceed to programming if activation successful.
   */
  void activate(ActivateData* data, VehicleCallBack callback = 0,
                UserData userData = 0);

  /**
  * @remark
  * Blocks until ACK frame arrives or timeout occurs
  *
  * @brief
  * Send activation control to your flight controller to check if: \n a)
  * your application registered in your developer
  * account \n b) API Control enabled in the Assistant software\n\n
  * Proceed to programming if activation successful.
  *
  * @return ACK from flight controller
  *
  * @todo
  * Implement high resolution timer to catch ACK timeout
  */
  ACK::ErrorCode activate(ActivateData* data, uint32_t timeoutMs);

  /*! @brief A callback function for activate non-blocking calls
   *  @param receivedFrame: RecvContainer populated by the protocolLayer
   *  @return NULL
   */
  static void activateCallback(Vehicle* vehiclePtr, RecvContainer recvFrame,
                               UserData userData = 0);
  /*! @brief A callback function for get drone version non-blocking calls
   *  @param receivedFrame: RecvContainer populated by the protocolLayer
   *  @return NULL
   */
  static void getDroneVersionCallback(Vehicle*      vehiclePtr,
                                      RecvContainer recvFrame,
                                      UserData      userData = 0);
  /**
   * @brief
   * Send get version control to the vehicle.
   *
   * @return type ACK::DroneVersion containing:
   * ACKErrorCode: data (ack value)
   * VersionData:  hardware version
   * VersionData:  firmware version
   * VersionData:  hardware serial number
   * VersionData:  CRC
   * VersionData:  version name
   */
  ACK::DroneVersion getDroneVersion(uint32_t timeoutMs);
  //@{
  /**
   * Get aircraft version.
   *
   * @note
   * You can query your flight controller prior to activation.
   */
  void getDroneVersion(VehicleCallBack callback = 0, UserData userData = 0);

  //////////// Getters/Setters //////////

  /**
   * Get Activation information
   */
  ActivateData getAccountData() const;

  /*
   * Activation Control
   */
  void setAccountData(const ActivateData& value);

  /**
   * Set SDK version.
   */
  void setVersion(const Version::FirmWare& value);

private:
  static HeartBeatPack  heartBeatPack;
  static uint8_t        fcLostConnectCount;
  const static uint8_t  kMaxFCLostConnectCount = 5;
  const static uint8_t  kOSDKSendId;
  const static uint32_t kHeartBeatPackSendTimeInterval;

public:
  Version::FirmWare getFwVersion() const;
  char*             getHwVersion() const;
  char*             getHwSerialNum() const;
  bool              isLegacyM600();
  bool              isM100();
  bool              isM210V2();

private:
  Version::VersionData versionData;
  ActivateData         accountData;

private:
  bool is_activated = false;
  bool encrypt = false;

public:

  // User space ACK types
  ACK::ErrorCode     ackErrorCode;
  ACK::DroneVersion  droneVersionACK;

public:
  bool init();

  bool initVersion();

public:
  static bool parseDroneVersionInfo(Version::VersionData& versionData,
                                    uint8_t*              ackPtr);

private:
  const int            wait_timeout   = 1000;
  const int            GIMBAL_MOUNTED = 1;
  static const uint8_t NUM_CMD_SET    = 9;
  CMD_SETSupportMatrix cmd_setSupportMatrix[NUM_CMD_SET];

public:
  void setEncryption(bool encryptSetting);
  bool getEncryption();
  bool getActivationStatus();

  /*!
   * @brief Initialize main read thread to support UART communication
   * @return fasle if error, true if success
   */
  bool initLegacyLinker();
  bool initSubscriber();
  bool initBroadcast();
  bool initControl();
  bool initCamera();
  bool initGimbal();
  bool initMFIO();
  bool initMOC();
  bool initMobileDevice();
  bool initPayloadDevice();
  bool initMissionManager();
  bool initHardSync();
  bool initVirtualRC();
  bool initCameraManager();
  bool initFlightController();
  bool initPSDKManager();
  bool initGimbalManager();
  bool initOSDKHeartBeatThread();
private:
  void setActivationStatus(bool is_activated);
  void initCMD_SetSupportMatrix();
  bool isCmdSetSupported(const uint8_t cmdSet);

  void sendBuriedDataPkgToFC(void);
  uint8_t stm32Flag = NOT_STM32;

  static void fcLostConnectCallBack(void);
  static uint8_t sendHeartbeatToFCFunc(Linker * linker);
  T_OsdkTaskHandle sendHeartbeatToFCHandle;
  static void *sendHeartbeatToFCTask(void *arg);
public:
  void setStm32Flag(uint8_t & flag);
};
}
}
#endif /* OSDK_CORE_INC_DJI_VEHICLE_H_ */
