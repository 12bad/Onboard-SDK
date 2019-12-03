/** @file dji_legacy_linker.cpp
 *  @version 4.0
 *  @date November 2019
 *
 *  @brief
 *  Legacy adapting in linker for OSDK 3.9
 *
 *  @Copyright (c) 2016-2017 DJI
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

#include "dji_legacy_linker.hpp"
#include "dji_vehicle.hpp"

#define MAX_PARAMETER_VALUE_LENGTH 8

#ifdef STM32
#include <stdio.h>
#endif

using namespace DJI;
using namespace DJI::OSDK;

LegacyLinker::LegacyLinker(Vehicle *vehicle)
    : vehicle(vehicle) {
}

LegacyLinker::~LegacyLinker() {
}

void LegacyLinker::send(const uint8_t cmd[], void *pdata, size_t len) {
  T_CmdInfo cmdInfo = {0};

  cmdInfo.cmdSet = cmd[0];
  cmdInfo.cmdId = cmd[1];
  cmdInfo.dataLen = len;
  cmdInfo.needAck = OSDK_COMMAND_NEED_ACK_NO_NEED;
  cmdInfo.packetType = OSDK_COMMAND_PACKET_TYPE_REQUEST;
  cmdInfo.addr = GEN_ADDR(0, ADDR_SDK_COMMAND_INDEX);
  cmdInfo.encType = vehicle->getEncryption();
  cmdInfo.channelId = 0;
  vehicle->linker->send(&cmdInfo, (uint8_t *) pdata);
}

typedef struct legacyAdaptingData {
  VehicleCallBack cb;
  UserData udata;
  Vehicle *vehicle;
} legacyAdaptingData;

void LegacyLinker::legacyAdaptingAsyncCB(const T_CmdInfo *cmdInfo,
                                         const uint8_t *cmdData,
                                         void *userData, E_OsdkStat cb_type) {
  if (cb_type == OSDK_STAT_OK) {
    if ((!cmdInfo) && (!userData) && (!((legacyAdaptingData *) (userData))->cb)
        && (!((legacyAdaptingData *) (userData))->vehicle)) {
      legacyAdaptingData para = *(legacyAdaptingData *) userData;

      RecvContainer recvFrame = recvFrameAdapting(cmdInfo, cmdData);
      para.cb(para.vehicle, recvFrame, para.udata);
    } else {
      DERROR("Parameter invalid.");
    }
  } else if (cb_type == OSDK_STAT_ERR_TIMEOUT) {
    DERROR("wait for callback time out.");
  } else {
    DERROR("wait for callback error.");
  }

  free(userData);
}

void LegacyLinker::sendAsync(const uint8_t cmd[], void *pdata, size_t len,
                             int timeout, int retry_time,
                             VehicleCallBack callback, UserData userData) {
  T_CmdInfo cmdInfo = {0};

  cmdInfo.cmdSet = cmd[0];
  cmdInfo.cmdId = cmd[1];
  cmdInfo.dataLen = len;
  cmdInfo.needAck = OSDK_COMMAND_NEED_ACK_FINISH_ACK;
  cmdInfo.packetType = OSDK_COMMAND_PACKET_TYPE_REQUEST;
  cmdInfo.addr = GEN_ADDR(0, ADDR_SDK_COMMAND_INDEX);
  cmdInfo.encType = vehicle->getEncryption();
  cmdInfo.channelId = 0;
  legacyAdaptingData
      *udata = (legacyAdaptingData *) malloc(sizeof(legacyAdaptingData));
  *udata = {callback, userData, vehicle};

  vehicle->linker->sendAsync(&cmdInfo, (uint8_t *) pdata, legacyAdaptingAsyncCB,
                             udata, timeout, retry_time);
}

void *LegacyLinker::decodeAck(E_OsdkStat ret, T_CmdInfo *ackInfo,
                              uint8_t *ackData)
{

  void* pACK;
  /*  if (ret != OSDK_STAT_OK){
    ackErrorCode.info = recvFrame.recvInfo;
    ackErrorCode.data = recvFrame.recvData.ack;
    pACK = static_cast<void*>(&this->ackErrorCode);
  }
  if (!ackInfo) {

  }
*/

  uint cmd[2] = {ackInfo->cmdSet, ackInfo->cmdId};
  RecvContainer recvFrame = recvFrameAdapting(ackInfo, ackData);
  if (recvFrame.recvInfo.cmd_set == OpenProtocolCMD::CMDSet::mission)
  {
    if (memcmp(cmd, OpenProtocolCMD::CMDSet::Mission::waypointAddPoint,
               sizeof(cmd)) == 0)
    {
      waypointAddPointACK.ack.info = recvFrame.recvInfo;
      waypointAddPointACK.ack.data = recvFrame.recvData.wpAddPointACK.ack;
      waypointAddPointACK.index    = recvFrame.recvData.wpAddPointACK.index;
      pACK = static_cast<void*>(&this->waypointAddPointACK);
    }
    else if (memcmp(cmd, OpenProtocolCMD::CMDSet::Mission::waypointDownload,
                    sizeof(cmd)) == 0)
    {
      waypointInitACK.ack.info = recvFrame.recvInfo;
      waypointInitACK.ack.data = recvFrame.recvData.wpInitACK.ack;
      waypointInitACK.data     = recvFrame.recvData.wpInitACK.data;
      pACK = static_cast<void*>(&this->waypointInitACK);
    }
    else if (memcmp(cmd, OpenProtocolCMD::CMDSet::Mission::waypointIndexDownload,
                    sizeof(cmd)) == 0)
    {
      waypointIndexACK.ack.info = recvFrame.recvInfo;
      waypointIndexACK.ack.data = recvFrame.recvData.wpIndexACK.ack;
      waypointIndexACK.data     = recvFrame.recvData.wpIndexACK.data;
      pACK = static_cast<void*>(&this->waypointIndexACK);
    }
    else if (memcmp(cmd, OpenProtocolCMD::CMDSet::Mission::hotpointStart,
                    sizeof(cmd)) == 0)
    {
      hotpointStartACK.ack.info  = recvFrame.recvInfo;
      hotpointStartACK.ack.data  = recvFrame.recvData.hpStartACK.ack;
      hotpointStartACK.maxRadius = recvFrame.recvData.hpStartACK.maxRadius;
      pACK = static_cast<void*>(&this->hotpointStartACK);
    }
    else if (memcmp(cmd, OpenProtocolCMD::CMDSet::Mission::hotpointDownload,
                    sizeof(cmd)) == 0)
    {
      hotpointReadACK.ack.info = recvFrame.recvInfo;
      hotpointReadACK.ack.data = recvFrame.recvData.hpReadACK.ack;
      hotpointReadACK.data     = recvFrame.recvData.hpReadACK.data;
      pACK = static_cast<void*>(&this->hotpointReadACK);
    }
    else if (cmd[0] == OpenProtocolCMD::CMDSet::mission
        && OpenProtocolCMD::CMDSet::Mission::waypointInitV2[1] <= cmd[1]
        &&  cmd[1] <= OpenProtocolCMD::CMDSet::Mission::waypointGetMinMaxActionIDV2[1])
    {
      /*
      wayPoint2CommonRspACK.info      = recvFrame.recvInfo;
      wayPoint2CommonRspACK.info.buf  = recvFrame.recvData.raw_ack_array;
      wayPoint2CommonRspACK.updated   = true;
      pACK = static_cast<void*>(&this->wayPoint2CommonRspACK);
     */
      DERROR("TODO : to be implemented.");
    }
    else
    {
      ackErrorCode.info = recvFrame.recvInfo;
      ackErrorCode.data = recvFrame.recvData.missionACK;
      pACK = static_cast<void*>(&this->ackErrorCode);
    }
  }
  else if (memcmp(cmd, OpenProtocolCMD::CMDSet::Activation::getVersion,
                  sizeof(cmd)) == 0)
  {
    size_t arrLength = sizeof(recvFrame.recvData.versionACK);
    for (int i = 0; i < arrLength; i++)
    {
      //! Interim stage: version data will be parsed before returned to user
      this->rawVersionACK[i] = recvFrame.recvData.versionACK[i];
      pACK = static_cast<void*>(&this->rawVersionACK);
    }
    droneVersionACK.ack.info = recvFrame.recvInfo;
  }
  else if (recvFrame.recvInfo.cmd_set == OpenProtocolCMD::CMDSet::subscribe)
  {
    ackErrorCode.info = recvFrame.recvInfo;
    ackErrorCode.data = recvFrame.recvData.subscribeACK;
    pACK = static_cast<void*>(&this->ackErrorCode);
  }
  else if (recvFrame.recvInfo.cmd_set == OpenProtocolCMD::CMDSet::control)
  {
    if (memcmp(cmd, OpenProtocolCMD::CMDSet::Control::extendedFunction,
               sizeof(cmd)) == 0) {
      /*
      extendedFunctionRspAck.info = recvFrame.recvInfo;
      extendedFunctionRspAck.info.buf = recvFrame.recvData.raw_ack_array;
      extendedFunctionRspAck.updated = true;
      pACK = static_cast<void*>(&this->extendedFunctionRspAck);
      */
      DERROR("TODO : to be implemented.");
    }
    else if(memcmp(cmd, OpenProtocolCMD::CMDSet::Control::parameterRead, sizeof(cmd))==0 ||
        memcmp(cmd, OpenProtocolCMD::CMDSet::Control::parameterWrite, sizeof(cmd))==0)
    {
      paramAck.info            = recvFrame.recvInfo;
      paramAck.data.retCode    = recvFrame.recvData.paramAckData.retCode;
      paramAck.data.hashValue  = recvFrame.recvData.paramAckData.hashValue;
      memcpy(paramAck.data.paramValue, recvFrame.recvData.paramAckData.paramValue, MAX_PARAMETER_VALUE_LENGTH);
      paramAck.updated         = true;
      pACK = static_cast<void*>(&this->paramAck);
    }
    else
    {
      ackErrorCode.info = recvFrame.recvInfo;
      ackErrorCode.data = recvFrame.recvData.commandACK;
      pACK = static_cast<void*>(&this->ackErrorCode);
    }
  }
  else if (memcmp(cmd, OpenProtocolCMD::CMDSet::MFIO::init, sizeof(cmd)) == 0)
  {
    ackErrorCode.info = recvFrame.recvInfo;
    ackErrorCode.data = recvFrame.recvData.mfioACK;
    pACK = static_cast<void*>(&this->ackErrorCode);
  }
  else if (memcmp(cmd, OpenProtocolCMD::CMDSet::MFIO::get, sizeof(cmd)) == 0)
  {
    mfioGetACK.ack.info = recvFrame.recvInfo;
    mfioGetACK.ack.data = recvFrame.recvData.mfioGetACK.result;
    mfioGetACK.value    = recvFrame.recvData.mfioGetACK.value;
    pACK = static_cast<void*>(&this->mfioGetACK);
  }
  else if (memcmp(cmd, OpenProtocolCMD::CMDSet::Intelligent::setAvoidObstacle, sizeof(cmd)) == 0)
  {
    /*! data mean's the setting's data ref in AvoidObstacleData struct*/
    ackErrorCode.info = recvFrame.recvInfo;
    ackErrorCode.data = recvFrame.recvData.commandACK;
    pACK = static_cast<void*>(&this->ackErrorCode);
  }
  else
  {
    ackErrorCode.info = recvFrame.recvInfo;
    ackErrorCode.data = recvFrame.recvData.ack;
    pACK = static_cast<void*>(&this->ackErrorCode);
  }

  return pACK;
}

void* LegacyLinker::sendSync(const uint8_t cmd[], void *pdata,
                                      size_t len, int timeout, int retry_time) {
  T_CmdInfo cmdInfo = {0};
  T_CmdInfo ackInfo = {0};
  uint8_t ackData[1024];

  /*! request cmd info */
  cmdInfo.cmdSet = cmd[0];
  cmdInfo.cmdId = cmd[1];
  cmdInfo.dataLen = len;
  cmdInfo.needAck = OSDK_COMMAND_NEED_ACK_FINISH_ACK;
  cmdInfo.packetType = OSDK_COMMAND_PACKET_TYPE_REQUEST;
  cmdInfo.addr = GEN_ADDR(0, ADDR_SDK_COMMAND_INDEX);
  cmdInfo.encType = vehicle->getEncryption();
  cmdInfo.channelId = 0;

  /*! default ack info value */
  ackInfo.cmdSet = cmdInfo.cmdSet;
  ackInfo.cmdId = cmdInfo.cmdId;

  E_OsdkStat ret =
      vehicle->linker->sendSync(&cmdInfo, (uint8_t *) pdata, &ackInfo, ackData,
                                timeout, retry_time);

  return decodeAck(ret, &ackInfo, ackData);
}

RecvContainer LegacyLinker::recvFrameAdapting(const T_CmdInfo *cmdInfo, const uint8_t *cmdData)
{
  RecvContainer recvFrame = {0};

  recvFrame.dispatchInfo.isAck = true;
  recvFrame.recvInfo.cmd_set = cmdInfo->cmdSet;
  recvFrame.recvInfo.cmd_id = cmdInfo->cmdId;
  memcpy(recvFrame.recvData.raw_ack_array, cmdData, cmdInfo->dataLen);
  recvFrame.dispatchInfo.isCallback = true;
  recvFrame.dispatchInfo.callbackID = 0; //only valid in before OSDK 4.0
  recvFrame.recvInfo.buf = (uint8_t *) cmdData;
  recvFrame.recvInfo.seqNumber = cmdInfo->seqNum;
  recvFrame.recvInfo.len = cmdInfo->dataLen + OpenProtocol::PackageMin;
  return recvFrame;
}

E_OsdkStat LegacyLinker::legacyAdaptingRegisterCB(
    struct _CommandHandle *cmdHandle,
    const T_CmdInfo *cmdInfo,
    const uint8_t *cmdData, void *userData) {
  legacyAdaptingData *legacyData = (legacyAdaptingData *)userData;
  if (cmdInfo && legacyData && legacyData->cb && legacyData->vehicle) {
    legacyAdaptingData para = *(legacyAdaptingData *) userData;

    RecvContainer recvFrame = recvFrameAdapting(cmdInfo, cmdData);
    para.cb(para.vehicle, recvFrame, para.udata);
    return OSDK_STAT_OK;
  } else {
    DERROR("Parameter invalid.");
    return OSDK_STAT_ERR_PARAM;
  }
}

bool LegacyLinker::registerCMDCallback(uint8_t cmdSet, uint8_t cmdID,
                                       VehicleCallBack &callback,
                                       UserData &userData) {
  T_RecvCmdHandle
      *recvCmdHandle = (T_RecvCmdHandle *) malloc(sizeof(T_RecvCmdHandle));
  T_RecvCmdItem *item = (T_RecvCmdItem *) malloc(sizeof(T_RecvCmdItem));
  legacyAdaptingData
      *cbParam = (legacyAdaptingData *) malloc(sizeof(legacyAdaptingData));
  *cbParam = {callback, userData, vehicle};

  item->cmdSet = cmdSet;
  item->cmdId = cmdID;
  item->pFunc = legacyAdaptingRegisterCB;
  item->userData = cbParam;
  item->mask = MASK_HOST_XXXXXX_SET_ID;

  recvCmdHandle->cmdCount = 1;
  recvCmdHandle->protoType = PROTOCOL_SDK;
  recvCmdHandle->cmdList = item;
  if (vehicle->linker->registerCmdHandler(recvCmdHandle)) {

    return true;
  } else {
    free(recvCmdHandle->cmdList->userData);
    free(recvCmdHandle->cmdList);
    free(recvCmdHandle);

    return false;
  }
}
