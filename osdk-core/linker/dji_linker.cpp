/** @file dji_linker.cpp
 *  @version 3.3
 *  @date April 2017
 *
 *  @brief
 *  Vehicle API for DJI onboardSDK library
 *
 *  @Copyright (c) 2019 DJI
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

#include "dji_linker.hpp"
#include <new>

using namespace DJI;
using namespace DJI::OSDK;

Linker::Linker(uint8_t defaultSenderId)
:senderId(defaultSenderId)
{
  uartPlugged = false;
  usbPlugged = false;
}

Linker::~Linker()
{
  deinit();
}

bool
Linker::init()
{
  E_OsdkStat errCode;

  errCode = OsdkCore_RootTaskInit();
  if (errCode != OSDK_STAT_OK) {
      DERROR("Root Task Init Error!, Error Code : %d\n", errCode);
      return false;
  }

  errCode = OsdkChannel_InitInstance();
  if (errCode != OSDK_STAT_OK) {
      DERROR("Channel Task Init Error!, Error Code : %d\n", errCode);
      return false;
  }

  errCode = OsdkCommand_InitInstance();
  if (errCode != OSDK_STAT_OK) {
      DERROR("Command Task Init Error!", errCode);
      return false;
  }

  return true;
}

bool
Linker::deinit()
{
  OsdkCommand_DeInit(OsdkCommand_GetInstance());
  OsdkCore_RootTaskDeInit();
  OsdkChannel_DeinitInstance();

  return true;
}

bool Linker::isUartPlugged() {
  return uartPlugged;
}

bool Linker::isUSBPlugged() {
  return usbPlugged;
}

bool
Linker::addUartChannel(const char *device, uint32_t baudrate,
                       E_ChannelIDType id)
{
  E_OsdkStat errCode;
  errCode = OsdkChannel_InitUartChannel(device, baudrate, id);
  bool ret = ((errCode == OSDK_STAT_OK) ? true : false);
  if (id == FC_UART_CHANNEL_ID) uartPlugged = ret;
  if (id == USB_ACM_CHANNEL_ID) usbPlugged = ret;

  return ret;
}

#ifdef __linux__
bool
Linker::addUSBBulkChannel(uint16_t pid, uint16_t vid, uint16_t num, uint16_t epIn,
                          uint16_t epOut, E_ChannelIDType id)
{
  E_OsdkStat errCode;
  errCode = OsdkChannel_InitUSBBulkChannel(pid, vid, num, epIn, epOut, id);
  return (errCode == OSDK_STAT_OK) ? true : false;
}

#endif

void
Linker::sendAsync(T_CmdInfo *cmdInfo, const uint8_t *cmdData,
                  Command_SendCallback func, void *userData,
                  uint32_t timeOut, uint16_t retryTimes)
{
  return OsdkCommand_SendAsync(OsdkCommand_GetInstance(), cmdInfo, cmdData,
                               func, userData, timeOut, retryTimes);
}

E_OsdkStat
Linker::sendSync(T_CmdInfo *cmdInfo, const uint8_t *cmdData,
                 T_CmdInfo *ackInfo, uint8_t *ackData,
                 uint32_t timeOut, uint16_t retryTimes)
{
  return OsdkCommand_SendSync(OsdkCommand_GetInstance(), cmdInfo, cmdData, 
                              ackInfo, ackData, timeOut, retryTimes);
}

void
Linker::setKey(const char *key)
{
  OsdkCommand_SetKey(key);
}


void
Linker::setSenderId(uint8_t senderId) {
  this->senderId = senderId;
}

uint8_t
Linker::getLocalSenderId() {
  return senderId;
}

E_OsdkStat
Linker::send(T_CmdInfo *cmdInfo, const uint8_t *cmdData)
{
  return OsdkCommand_Send(cmdInfo, cmdData);
}

E_OsdkStat
Linker::sendAck(const T_CmdInfo *ackInfo, const uint8_t *ackData, uint16_t ackDataLen)
{
  return OsdkCommand_SendAckData(ackInfo, ackData, ackDataLen);
}

bool
Linker::registerCmdHandler(T_RecvCmdHandle *recvCmdHandle)
{
  E_OsdkStat errCode;
  errCode = OsdkCommand_RegRecvCmdHandler(OsdkCommand_GetInstance(), recvCmdHandle);
  return (errCode == OSDK_STAT_OK) ? true : false;
}

bool
Linker::createLiveViewTask()
{
  E_OsdkStat errCode;
  errCode = OsdkCommand_CreateLiveViewTask();
  return (errCode == OSDK_STAT_OK) ? true : false;
}

bool
Linker::destroyLiveViewTask()
{
  E_OsdkStat errCode;
  errCode = OsdkCommand_DestroyLiveViewTask();
  return (errCode == OSDK_STAT_OK) ? true : false;
}

bool
Linker::createAdvancedSensingTask()
{
  E_OsdkStat errCode;
  errCode = OsdkCommand_CreateAdvancedSensingTask();
  return (errCode == OSDK_STAT_OK) ? true : false;
}

bool
Linker::destroyAdvancedSensingTask()
{
  E_OsdkStat errCode;
  errCode = OsdkCommand_DestroyAdvancedSensingTask();
  return (errCode == OSDK_STAT_OK) ? true : false;
}

