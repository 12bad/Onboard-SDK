/** @file dji_waypoint_v2.hpp
 *  @version 4.0
 *  @date April 2019
 *
 *  @brief Implementation of GPS Waypoint Missions for DJI OSDK
 *
 *  @Copyright (c) 2016-2020 DJI
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

#ifndef DJI_WAYPOINT_MISSION_V2_HPP
#define DJI_WAYPOINT_MISSION_V2_HPP

#include <vector>
#include "dji_mission_base.hpp"
#include "dji_waypoint_v2_action.hpp"

namespace DJI
{
namespace OSDK
{

  class Linker;
  /*! @brief APIs for GPS Waypoint Missions
   *
   *  @details This class inherits from MissionBase and can be used with
   *  MissionManager.
   */
  class WaypointMissionV2 //: public MissionBase
  {
  public:
    const uint16_t MAX_WAYPOINT_NUM_SIGNAL_PUSH = 260;

    enum DJIWaypointV2MissionState{
      /**
       *  The state of the operator is unknown. It is the initial state when the operator
       *  is just created.
       */
        DJIWaypointV2MissionStateUnWaypointActionActuatorknown = -1,

      /**
       *  The connection between the mobile device, remote controller and aircraft is
       *  broken.
       */
        DJIWaypointV2MissionStateDisconnected,

      /**
       *  The connection between the mobile device, remote controller and aircraft is
       *  built-up. The operator is synchronizing the state from the aircraft.
       */
        DJIWaypointV2MissionStateRecovering,

      /**
       *  The connected product does not support waypoint mission 2.0.
       */
        DJIWaypointV2MissionStateNotSupported,

      /**
       *  The aircraft is ready to upload a mission.
       */
        DJIWaypointV2MissionStateReadyToUpload,

      /**
       *  The uploading is started successfully. Detail information for each waypoint is
       *  being uploaded one by one.
       */
        DJIWaypointV2MissionStateUploading,

      /**
       *  Waypoint mission is uploaded completely and the aircraft is ready to start the
       *  execution.
       */
        DJIWaypointV2MissionStateReadyToExecute,

      /**
       *  The execution is started successfully.
       */
        DJIWaypointV2MissionStateExecuting,

      /**
       *  Waypoint mission is paused successfully. User can call
       *  ``DJIWaypointV2MissionOperator_interruptMission`` to continue the execution.
       */
        DJIWaypointV2MissionStateInterrupted,
    };

    typedef uint8_t  RetCodeType;
    typedef uint32_t WaypointV2CommonAck;
    typedef uint16_t GlobalCruiseSpeed;

    #pragma pack(1)
    typedef struct UploadMissionRawAck
    {
      uint32_t result;
      uint16_t startIndex;
      uint16_t endIndex;
    }UploadMissionRawAck;

    typedef struct UploadActionsRawAck
    {
      uint32_t result;
      uint16_t errorActionId;
    }UploadActionSRawAck;

    typedef struct GetRemainRamAck
    {
      uint16_t totalMemory;
      uint16_t remainMemory;
    }getRemainRamAck;

    typedef struct GetWaypontStartEndIndexAck
    {
      uint32_t result;
      uint16_t startIndex;
      uint16_t endIndex;
    }GetWaypontStartEndIndexAck;

    typedef struct DownloadMissionRsp
    {
      uint16_t startIndex;
      uint16_t endIndex;
    }DownloadMissionRsp;

    typedef struct DownloadMissionAck
    {
      uint32_t result;
      uint16_t startIndex;
      uint16_t endIndex;
    }DownloadMissionAck;

    typedef struct MissionStateCommanData
    {

      uint16_t curWaypointIndex;
      uint8_t  state;
      uint16_t velocity;
      uint8_t  config;
    }MissionStateCommanData;

    typedef struct MissionStatePushAck
    {
      uint8_t commonDataVersion = 1;
      uint16_t commonDataLen;
      MissionStateCommanData data;
//      uint8_t uniqueDataVersion ;
//      uint16_t uniqueDataLength;
    }MissionStatePushAck;

    typedef union Eventdata
    {
      /*ID:0x01*/
      uint8_t interruptReason;

      /*ID:0x02*/
      uint8_t RecoverProcess;

      /*ID:0x03*/
      uint8_t finishReason;

      /*ID:0x10*/
      uint16_t waypointIndex;

      /*ID:0x11*/
       struct MissionExecEvent{
        uint8_t CurrentMissionExecNum;
        uint8_t finishedAllExecNum:1;
        uint8_t reserved:7;
      }MissionExecEvent;

      /*ID:0x12*/
      uint8_t avoidState;

      /*ID:0x20*/
       struct MissionValidityEvent {
        uint8_t misValidityFlag;
        float32_t estimateRunTime;
      }MissionValidityEvent;

      /*ID:0x30*/
       struct ActionExecEvent{
        uint16_t actionId;
        uint8_t preActuatorState;
        uint8_t curActuatorState;
        uint32_t result;
      };
    }Eventdata;

    typedef struct MissionEventPushAck
    {
      uint8_t event ;
      uint16_t FCTimestamp;
      Eventdata data;
    }MissionEventPushAck;

    #pragma pack()


    WaypointMissionV2(Vehicle* vehiclePtr);

    ~WaypointMissionV2();

    ErrorCode::ErrorCodeType init(WayPointV2InitSettings* Info, int timeout);

    ErrorCode::ErrorCodeType startV2(int timeout);
    /*! @brief
     *
     *  stop the waypt mission
     *
     *  @param register a callback function for error code
     */
    ErrorCode::ErrorCodeType stopV2(int timeout);
    /*! @brief
     *
     *  pause the waypt mission
     *
     *  @param register a callback function for error code
     */
    ErrorCode::ErrorCodeType pauseV2(int timeout);
    /*! @brief
     *
     *  resume the waypt mission
     *
     *  @param register a callback function for error code
     */
    ErrorCode::ErrorCodeType resumeV2(int timeout);

    ErrorCode::ErrorCodeType uploadMission(const std::vector<WaypointV2> &mission,int timeout);

    ErrorCode::ErrorCodeType downloadMission(std::vector<WaypointV2> &vector, int timeout);

    ErrorCode::ErrorCodeType getGlogalCruiseSpeed(GlobalCruiseSpeed &cruiseSpeed,int timeout);

    ErrorCode::ErrorCodeType setGlogalCruiseSpeed(const GlobalCruiseSpeed &cruiseSpeed,int timeout);

    ErrorCode::ErrorCodeType uploadActionV2(std::vector<DJIWaypointV2Action> &actions,int timeout);

    ErrorCode::ErrorCodeType downloadActionV2(int timeout);

    ErrorCode::ErrorCodeType getActionRemainMemory(GetRemainRamAck &remainRamAck, int timeout);

    ErrorCode::ErrorCodeType getWaypointIndexInList(GetWaypontStartEndIndexAck &startEndIndexAck, int timeout);

    void RegisterMissionStateCallback();

    void RegisterMissionEventCallback();

    /*! @brief
     *
     *  start the waypt mission
     *
     *  @param register a callback function for error code
     */
    void start(VehicleCallBack callback, UserData userData);
    /*! @brief
     *
     *  stop the waypt mission
     *
     *  @param register a callback function for error code
     */
    void stop(VehicleCallBack callback, UserData userData);
    /*! @brief
     *
     *  pause the waypt mission
     *
     *  @param register a callback function for error code
     */
    void pause(VehicleCallBack callback, UserData userData);
    /*! @brief
     *
     *  resume the waypt mission
     *
     *  @param register a callback function for error code
     */
    void resume(VehicleCallBack callback, UserData userData);


  //  /*! @brief
  //   *
  //   *  get current speed of the waypt mission
  //   *
  //   *  @param register a callback function for error code
  //   */
  //  void getCurrentSpeed(std::function<void(float cruise_speed, WaypointV2Interface::CommonErrorCode error_code)> callback);
  //  /*! @brief
  //   *
  //   *  set current speed of the waypt mission
  //   *
  //   *  @param register a callback function for error code
  //   */
  //  void setCurrentSpeed(float speed, CommonErrorCallback errorCallback);
    /*! @brief
     *
     *  upload a waypt mission with new data strucutre
     *
     *  @param mission settings
     *  @param register a callback function for error code
     */
    void uploadMissionV2(const std::vector<WaypointV2> &waypointV2Mission,
                         VehicleCallBack callback, UserData userData);

    /*! @brief
     *
     *  download a waypt mission with new data strucutre
     *
     *  @param mission setting data struct to be written
     *  @param register a callback function for error code
     */
    bool DownloadMissionV2(const std::vector<WaypointV2> &waypointV2Mission,
                           VehicleCallBack callback, UserData userData);



  //  /*! @brief
  //   *
  //   *  upload a action which work in parallel with waypt mission
  //   *
  //   *  @param action settings
  //   *  @param register a callback function for error code
  //   */
  //  void uploadActionV2(const std::vector<dji::waypointv2::WaypointActionConfig> &actions,
  //                      VehicleCallBack callback, UserData userData);
  //
  //  void downloadActionV2(const std::vector<dji::waypointv2::WaypointActionConfig> &actions,
  //                        VehicleCallBack callback, UserData userData);
    /*! @brief
     *
     *  get current mission state
     *
     */
    inline DJIWaypointV2MissionState getCurrentState() { return currentState; }
    /*! @brief
     *
     *  get previous mission state
     *
     */
    inline DJIWaypointV2MissionState getPrevState() { return prevState; }

//    /*! @brief
//     *
//     *  get current action state
//     *
//     */
//    inline DJIWaypointV2MissionState getCurrentActionState() { return waypointV2Interface.getCurrentActionState(); }
//    /*! @brief
//     *
//     *  get previous action state
//     *
//     */
//    inline DJIWaypointV2MissionState getPrevActionState() { return waypointV2Interface.getPrevActionState(); }

    void setPrevState(DJIWaypointV2MissionState state) {prevState = state; }

    void setCurrentState(DJIWaypointV2MissionState state) {currentState = state; }

  private:
    WayPointV2InitSettings info;
    //WayPointSettings*    index;
    Vehicle *vehicle;
    Linker *linker;
    DJIWaypointV2MissionState currentState;
    DJIWaypointV2MissionState prevState;
  };

} // namespace OSDK
} // namespace DJI

#endif // DJI_WAYPOINT_MISSION_V2_HPP
