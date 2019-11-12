/** @file dji_linker.hpp
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

#ifndef OSDK_DJI_CORE_INC_LINKER_H_
#define OSDK_DJI_CORE_INC_LINKER_H_

#include <stdint.h>
#include "osdk_command_instance.h"
#include "osdk_channel_instance.h"
#include "dji_log.hpp"
#include "osdk_root_task.h"

namespace DJI
{
namespace OSDK
{

class Linker
{
public:
  Linker();
  ~Linker();

bool init();

bool addUartChannel(const char *device, uint32_t baudrate);

bool addUdpChannel(const char *addr, uint16_t port);

void setKey(const char *key);

void sendAsync(T_CmdInfo *cmdInfo, const uint8_t *cmdData,
                  Command_SendCallback func, void *userData,
                  uint32_t timeOut, uint16_t retryTimes);

bool sendSync(T_CmdInfo *cmdInfo, const uint8_t *cmdData,
                 T_CmdInfo *ackInfo, uint8_t *ackData,
                 uint32_t timeOut, uint16_t retryTimes);

bool send(T_CmdInfo *cmdInfo, const uint8_t *cmdData);

bool registerCmdHandler(T_RecvCmdHandle *recvCmdHandle);

};
}
}



#endif
