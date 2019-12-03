/** @file dji_vehicle.cpp
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

#include "dji_vehicle.hpp"
#include <new>

using namespace DJI;
using namespace DJI::OSDK;

Vehicle::Vehicle(const char* device,
		 uint32_t baudRate)
  : linker(NULL)
  , legacyLinker(NULL)
  , camera(NULL)
  , gimbal(NULL)
  , control(NULL)
  , broadcast(NULL)
{
  if (!device )
  {
    DERROR("Illegal serial device handle!\n");
  }

  this->device          = device;
  this->baudRate        = baudRate;

  ackErrorCode.data = OpenProtocolCMD::ErrorCode::CommonACK::NO_RESPONSE_ERROR;
}

bool
Vehicle::init()
{
  /*
   * @note Initialize communication layer
   */
  if (!initLinker())
  {
    DERROR("Failed to initialize Linker!\n");
    return 1;
  }

  if (!initLegacyLinker())
  {
    DERROR("Failed to initialize LegacyLinker!\n");
    return 1;
  }

  /*
   * Initialize broadcast if supported
   */
  if (!initBroadcast())
  {
    DERROR("Failed to initialize Broadcast!\n");
    return 1;
  }

  /*
   * @note Initialize Movement Control,
   * @note it will be replaced by FlightActions and FlightController in the future.
   */
  if (!initControl())
  {
    DERROR("Failed to initialize Control!\n");
    return 1;
  }

  /*
   * @note Initialize external components
   * like Camera and MFIO
   */
  if (!initCamera())
  {
    DERROR("Failed to initialize Camera!\n");
    return 1;
  }

  if(!initGimbal())
  {
    DERROR("Failed to initialize Gimbal!\n");
    return 1;
  }

  return true;
}

int
Vehicle::functionalSetUp()
{
  if (!initVersion())
  {
    return true;
  }
  else if (this->getFwVersion() < extendedVersionBase &&
           this->getFwVersion() != Version::M100_31 && !(this->isLegacyM600()))
  {
    DERROR("Upgrade firmware using Assistant software!\n");
    return true;
  }

  return false;
}

Vehicle::~Vehicle()
{
  delete this->linker;
}


bool
Vehicle::initVersion()
{
#if STM32
  //! Non blocking call for STM32 as it does not support multi-thread
  getDroneVersion();
  DJI_TASK_SLEEP_MS(2000);
  if(this->getFwVersion() > 0)
  {
    return true;
  }
#else
  ACK::DroneVersion rc = getDroneVersion(wait_timeout);
  if (!ACK::getError(rc.ack))
  {
    return true;
  }
#endif
  return false;
}

bool
Vehicle::initLinker() {
  if(this->linker)
  {
    DDEBUG("vehicle-linker already initalized!");
    return true;
  }

  this->linker = new (std::nothrow) Linker();
  if (this->linker == 0)
  {
    DERROR("Failed to allocate memory for LegacyLinker!\n");
    return false;
  }

  if (!linker->init())
  {
    DERROR("Failed to initialize Protocol Layer!\n");
    return false;
  }

  return true;
}

bool
Vehicle::initLegacyLinker(){
  if(this->legacyLinker)
  {
    DDEBUG("vehicle->legacyLinker already initalized!");
    return true;
  }

  this->legacyLinker = new (std::nothrow) LegacyLinker(this);
  if (this->legacyLinker == 0)
  {
    DERROR("Failed to allocate memory for LegacyLinker!\n");
    return false;
  }

  return true;
}

bool
Vehicle::initControl()
{
  if(this->control)
  {
    DDEBUG("vehicle->control already initalized!");
    return true;
  }

  if (isCmdSetSupported(OpenProtocolCMD::CMDSet::control))
  {
    this->control = new (std::nothrow) Control(this);
    if (this->control == 0)
    {
      DERROR("Failed to allocate memory for Control!\n");
      return false;
    }
  }
  else
  {
    DSTATUS("Control functionalities are not supported on this platform!\n");
  }

  return true;
}

bool
Vehicle::initBroadcast()
{
  if(this->broadcast)
  {
    DDEBUG("vehicle->broadcast already initalized!");
    return true;
  }

  if (isCmdSetSupported(OpenProtocolCMD::CMDSet::broadcast))
  {
    this->broadcast = new (std::nothrow) DataBroadcast(this);
    if (this->broadcast == 0)
    {
      DERROR("Failed to allocate memory for Broadcast!\n");
      return false;
    }
  }
  else
  {
    DSTATUS("Telemetry broadcast is not supported on this platform!\n");
  }

  return true;
}

bool
Vehicle::initCamera()
{
  if(this->camera)
  {
    DDEBUG("vehicle->camera already initalized!");
    return true;
  }

  this->camera = new (std::nothrow) Camera(this);

  if (this->camera == 0)
  {
    DERROR("Failed to allocate memory for Camera!\n");
    return false;
  }
  return true;
}

bool
Vehicle::initGimbal()
{
  if(this->gimbal)
  {
    DDEBUG("vehicle->gimbal already initalized!");
    return true;
  }

  this->gimbal = new (std::nothrow) Gimbal(this);

  if (this->gimbal == 0)
  {
    DERROR("Failed to allocate memory for Gimbal!\n");
    return false;
  }

  return true;
}

bool
Vehicle::parseDroneVersionInfo(Version::VersionData& versionData,
                               uint8_t*              ackPtr)
{

  Version::VersionData versionStruct;

  //! Note down our starting point as a sanity check
  uint8_t* startPtr = ackPtr;
  //! 2b ACK.
  versionStruct.version_ack = ackPtr[0] + (ackPtr[1] << 8);
  ackPtr += 2;

  //! Next, we might have CRC or ID; Put them into a variable that we will parse
  //! later. Find next \0
  uint8_t crc_id[16] = {};
  int     i          = 0;
  while (*ackPtr != '\0')
  {
    crc_id[i] = *ackPtr;
    i++;
    ackPtr++;
    if (ackPtr - startPtr > 18)
    {
      return false;
    }
  }
  //! Fill in the termination character
  crc_id[i] = *ackPtr;
  ackPtr++;

  //! Now we're at the name. First, let's fill up the name field.
  memcpy(versionStruct.version_name, ackPtr, 32);

  //! Now, we start parsing the name. Let's find the second space character.
  while (*ackPtr != ' ')
  {
    ackPtr++;
    if (ackPtr - startPtr > 64)
    {
      return false;
    }
  } //! Found first space ("SDK-v1.x")
  ackPtr++;

  while (*ackPtr != ' ')
  {
    ackPtr++;
    if (ackPtr - startPtr > 64)
    {
      return false;
    }
  } //! Found second space ("BETA")
  ackPtr++;

  //! Next is the HW version
  int j = 0;
  while (*ackPtr != '-')
  {
    versionStruct.hwVersion[j] = *ackPtr;
    ackPtr++;
    j++;
    if (ackPtr - startPtr > 64)
    {
      return false;
    }
  }
  //! Fill in the termination character
  versionStruct.hwVersion[j] = '\0';
  ackPtr++;

  //! Finally, we come to the FW version. We don't know if each clause is 2 or 3
  //! digits long.
  int ver1 = 0, ver2 = 0, ver3 = 0, ver4 = 0;

  while (*ackPtr != '.')
  {
    ver1 = (*ackPtr - 48) + 10 * ver1;
    ackPtr++;
    if (ackPtr - startPtr > 64)
    {
      return false;
    }
  }
  ackPtr++;
  while (*ackPtr != '.')
  {
    ver2 = (*ackPtr - 48) + 10 * ver2;
    ackPtr++;
    if (ackPtr - startPtr > 64)
    {
      return false;
    }
  }
  ackPtr++;
  while (*ackPtr != '.')
  {
    ver3 = (*ackPtr - 48) + 10 * ver3;
    ackPtr++;
    if (ackPtr - startPtr > 64)
    {
      return false;
    }
  }
  ackPtr++;
  while (*ackPtr != '\0')
  {
    ver4 = (*ackPtr - 48) + 10 * ver4;
    ackPtr++;
    if (ackPtr - startPtr > 64)
    {
      return false;
    }
  }

  versionStruct.fwVersion = Version::FW(ver1, ver2, ver3, ver4);

  //! Special cases
  //! M100:
  if (strcmp(versionStruct.hwVersion, Version::M100) == 0)
  {
    //! Bug in M100 does not report the right FW.
    ver3                    = 10 * ver3;
    versionStruct.fwVersion = Version::FW(ver1, ver2, ver3, ver4);
  }
  //! M600/A3 FW 3.2.10
  if (versionStruct.fwVersion == Version::FW(3, 2, 10, 0))
  {
    //! Bug in M600 does not report the right FW.
    ver3                    = 10 * ver3;
    versionStruct.fwVersion = Version::FW(ver1, ver2, ver3, ver4);
  }

  //! Now, we can parse the CRC and ID based on FW version. If it's older than
  //! 3.2 then it'll have a CRC, else not.
  if (versionStruct.fwVersion < Version::FW(3, 2, 0, 0))
  {
    versionStruct.version_crc =
      crc_id[0] + (crc_id[1] << 8) + (crc_id[2] << 16) + (crc_id[3] << 24);
    uint8_t* id_ptr = &crc_id[4];

    int i = 0;
    while (*id_ptr != '\0')
    {
      versionStruct.hw_serial_num[i] = *id_ptr;
      i++;
      id_ptr++;
      if (id_ptr - &crc_id[4] > 12)
      {
        return false; //! Not catastrophic error
      }
    }
    //! Fill in the termination character
    versionStruct.hw_serial_num[i] = *id_ptr;
  }
  else
  {
    versionStruct.version_crc = 0;
    uint8_t* id_ptr           = &crc_id[0];

    int i = 0;
    while (*id_ptr != '\0')
    {
      versionStruct.hw_serial_num[i] = *id_ptr;
      i++;
      id_ptr++;
      if (id_ptr - &crc_id[0] > 16)
      {
        return false; //! Not catastrophic error
      }
    }
    //! Fill in the termination character
    versionStruct.hw_serial_num[i] = *id_ptr;
  }

  //! Finally, we print stuff out.

  if (versionStruct.fwVersion > Version::FW(3, 1, 0, 0))
  {
    DSTATUS("Device Serial No. = %.16s\n", versionStruct.hw_serial_num);
  }
  DSTATUS("Hardware = %.12s\n", versionStruct.hwVersion);
  DSTATUS("Firmware = %d.%d.%d.%d\n", ver1, ver2, ver3, ver4);
  if (versionStruct.fwVersion < Version::FW(3, 2, 0, 0))
  {
    DSTATUS("Version CRC = 0x%X\n", versionStruct.version_crc);
  }

  versionData = versionStruct;
  return true;
}

bool
Vehicle::activate(ActivateData* data, uint32_t timeoutMs)
{
  T_CmdInfo cmdInfo = {0};
  T_CmdInfo ackInfo = {0};
  uint8_t cbData[1024];
  uint16_t *activateData;

  if(this->functionalSetUp() != 0)
  {
    DERROR("Unable to initialize some vehicle components!");
    return false;
  }

  data->version        = versionData.fwVersion;
  accountData          = *data;
  accountData.reserved = 2;

  for (int i             = 0; i < 32; ++i)
    accountData.iosID[i] = '0'; //! @note for ios verification
  DSTATUS("version 0x%X\n", versionData.fwVersion);
  DDEBUG("%.32s", accountData.iosID);

  cmdInfo.cmdSet = OpenProtocolCMD::CMDSet::Activation::activate[0];
  cmdInfo.cmdId  = OpenProtocolCMD::CMDSet::Activation::activate[1];
  cmdInfo.dataLen = sizeof(accountData) - sizeof(char*);
  cmdInfo.needAck = OSDK_COMMAND_NEED_ACK_FINISH_ACK;
  cmdInfo.packetType = OSDK_COMMAND_PACKET_TYPE_REQUEST;
  cmdInfo.addr = GEN_ADDR(0, ADDR_SDK_COMMAND_INDEX);
  cmdInfo.channelId = 0;

  if(timeoutMs < 3)
  {
    timeoutMs = 3;
  }

  linker->sendSync(&cmdInfo,(uint8_t *)&accountData, &ackInfo, cbData, timeoutMs/3, 3);
  activateData = (uint16_t *)cbData;

  if (*activateData == OpenProtocolCMD::ErrorCode::ActivationACK::SUCCESS &&
      accountData.encKey)
  {
    DSTATUS("Activation successful\n");
    linker->setKey(accountData.encKey);
    setActivationStatus(true);

  }
  else
  {
    DERROR("Failed to activate please retry SET 0x%X ID 0x%X\n",
           cmdInfo.cmdSet, cmdInfo.cmdId);

    if(*activateData == ErrorCode::ActivationACK::NEW_DEVICE_ERROR )
    {
      DERROR("Solutions for NEW_DEVICE_ERROR:\n"
               "\t* Double-check your app_id and app_key in UserConfig.txt. "
               "Does it match with your DJI developer account?\n"
               "\t* If this is a new device, you need to activate it through the App or DJI Assistant 2 with Internet\n"
               "\tFor different aircraft, the App and the version of DJI Assistant 2 might be different\n"
               "\tFor A3, N3, M600/Pro and M100, please use DJI GO App\n"
               "\tFor M210 V1, please use DJI GO 4 App or DJI Pilot App\n"
               "\tFor M210 V2, please use DJI Pilot App\n"
               "\tFor DJI Assistant 2, it's available on the 'Download' tab of the product page\n"
               "\t* If this device is previously activated with another app_id and app_key, "
               "you will need to re-activate it again.\n"
               "\t* A new device needs to be activated twice to fix the NEW_DEVICE_ERROR, "
               "so please try it twice.\n");
    }
    return false;
  }

  return true;
}

ACK::DroneVersion
Vehicle::getDroneVersion(uint32_t timeoutMs)
{
  T_CmdInfo cmdInfo = {0};
  T_CmdInfo ackInfo = {0};
  uint8_t data[1024];
  uint8_t  cmd_data = 0;

  versionData.version_ack =
    OpenProtocolCMD::ErrorCode::CommonACK::NO_RESPONSE_ERROR;
  versionData.version_crc     = 0x0;
  versionData.version_name[0] = 0;

  cmdInfo.cmdSet = OpenProtocolCMD::CMDSet::Activation::getVersion[0];
  cmdInfo.cmdId  = OpenProtocolCMD::CMDSet::Activation::getVersion[1];
  cmdInfo.dataLen = 1;
  cmdInfo.needAck = OSDK_COMMAND_NEED_ACK_FINISH_ACK;
  cmdInfo.packetType = OSDK_COMMAND_PACKET_TYPE_REQUEST;
  cmdInfo.addr = GEN_ADDR(0, ADDR_SDK_COMMAND_INDEX);

  if(timeoutMs < 3)
  {
    timeoutMs = 3;
  }

  linker->sendSync(&cmdInfo, &cmd_data, &ackInfo, data, timeoutMs/3, 3);

  // Parse received data
  if (!parseDroneVersionInfo(this->versionData, data))
  {
    DERROR("Drone version not obtained! Please do not proceed.\n"
             "Possible reasons:\n"
             "\tSerial port connection:\n"
             "\t\t* SDK is not enabled, please check DJI Assistant2 -> SDK -> [v] Enable API Control.\n"
             "\t\t* Baudrate is not correct, please double-check from DJI Assistant2 -> SDK -> baudrate.\n"
             "\t\t* TX and RX pins are inverted.\n"
             "\t\t* Serial port is occupied by another program.\n"
             "\t\t* Permission required. Please do 'sudo usermod -a -G dialout $USER' "
             "(you do not need to replace $USER with your username). Then logout and login again\n");

    droneVersionACK.ack.data = this->versionData.version_ack;
    //! Set fwVersion to 0 so we can catch the error.
    this->versionData.fwVersion = 0;
  }
  else
  {
    //! Construct final ACK to return to user
    droneVersionACK.ack.data         = this->versionData.version_ack;

    // We can prompt for droneVersion without prior activation
    if(ACK::getError(droneVersionACK.ack) &&
       droneVersionACK.ack.data == OpenProtocolCMD::ErrorCode::CommonACK::NO_AUTHORIZATION_ERROR)
    {
      droneVersionACK.ack.data = ACK::SUCCESS;
    }

    droneVersionACK.data.version_ack = this->versionData.version_ack;
    droneVersionACK.data.version_crc = this->versionData.version_crc;
    droneVersionACK.data.fwVersion   = this->versionData.fwVersion;

    strncpy(droneVersionACK.data.version_name, this->versionData.version_name,
            sizeof(this->versionData.version_name));
    droneVersionACK.data.version_name[sizeof(this->versionData.version_name) - 1] =
      '\0';

    strncpy(droneVersionACK.data.hwVersion, this->versionData.hwVersion,
            sizeof(this->versionData.hwVersion));
    droneVersionACK.data.hwVersion[sizeof(this->versionData.hwVersion) - 1] = '\0';

    strncpy(droneVersionACK.data.hw_serial_num, this->versionData.hw_serial_num,
            sizeof(this->versionData.hw_serial_num));
    droneVersionACK.data.hw_serial_num[sizeof(this->versionData.hw_serial_num) - 1] =
      '\0';
  }
  return droneVersionACK;
}

Vehicle::ActivateData
Vehicle::getAccountData() const
{
  return accountData;
}

void
Vehicle::setAccountData(const ActivateData& value)
{
  accountData = value;
}

/*****************************Set State Data**************************************/

void
Vehicle::setVersion(const Version::FirmWare& value)
{
  versionData.fwVersion = value;
}


Version::FirmWare
Vehicle::getFwVersion() const
{
  return versionData.fwVersion;
}
char*
Vehicle::getHwVersion() const
{
  return (char*)versionData.hwVersion;
}
char*
Vehicle::getHwSerialNum() const
{
  return (char*)versionData.hw_serial_num;
}

uint8_t*
Vehicle::getRawVersionAck()
{
  return this->rawVersionACK;
}

void
Vehicle::setEncryption(bool encryptSetting)
{
  this->encrypt = encryptSetting;
}

bool
Vehicle::getEncryption()
{
  return this->encrypt;
}

void Vehicle::setActivationStatus(bool is_activated)
{
  this->is_activated = is_activated;
}

bool Vehicle::getActivationStatus()
{
  return this->is_activated;
}

bool
Vehicle::isLegacyM600()
{
  //! Check for the special M600 backwards compatibility
  if (versionData.fwVersion == Version::FW(3, 2, 15, 62))
  {
    if (strncmp(versionData.hwVersion, "PM820V3", 7) == 0)
    {
      return true;
    }
    else
    {
      return false;
    }
  }
  return false;
}

bool
Vehicle::isM100()
{
  //! Check for the M100 backwards compatibility
  if (versionData.fwVersion == Version::FW(3, 1, 10, 0))
  {
    if (strncmp(versionData.hwVersion, Version::M100, 4) == 0)
    {
      return true;
    }
    else
    {
      return false;
    }
  }
  return false;
}

bool
Vehicle::isM210V2()
{
  if (strncmp(versionData.hwVersion, Version::M210V2, 5) == 0)
  {
    return true;
  }
  return false;
}


void
Vehicle::initCMD_SetSupportMatrix()
{
  cmd_setSupportMatrix[0].cmdSet    = OpenProtocolCMD::CMDSet::activation;
  cmd_setSupportMatrix[0].fwVersion = mandatoryVersionBase;

  cmd_setSupportMatrix[1].cmdSet    = OpenProtocolCMD::CMDSet::control;
  cmd_setSupportMatrix[1].fwVersion = mandatoryVersionBase;

  cmd_setSupportMatrix[2].cmdSet    = OpenProtocolCMD::CMDSet::broadcast;
  cmd_setSupportMatrix[2].fwVersion = mandatoryVersionBase;

  cmd_setSupportMatrix[3].cmdSet    = OpenProtocolCMD::CMDSet::mission;
  cmd_setSupportMatrix[3].fwVersion = mandatoryVersionBase;

  cmd_setSupportMatrix[4].cmdSet    = OpenProtocolCMD::CMDSet::hardwareSync;
  cmd_setSupportMatrix[4].fwVersion = extendedVersionBase;

  // Not supported in extendedVersionBase
  cmd_setSupportMatrix[5].cmdSet    = OpenProtocolCMD::CMDSet::virtualRC;
  cmd_setSupportMatrix[5].fwVersion = mandatoryVersionBase;

  cmd_setSupportMatrix[7].cmdSet    = OpenProtocolCMD::CMDSet::mfio;
  cmd_setSupportMatrix[7].fwVersion = extendedVersionBase;

  cmd_setSupportMatrix[8].cmdSet    = OpenProtocolCMD::CMDSet::subscribe;
  cmd_setSupportMatrix[8].fwVersion = extendedVersionBase;
}

bool
Vehicle::isCmdSetSupported(const uint8_t cmdSet)
{
  for (int i = 0; i < NUM_CMD_SET; i++)
  {
    if (cmd_setSupportMatrix[i].cmdSet == cmdSet)
    {
      if (cmdSet == OpenProtocolCMD::CMDSet::virtualRC &&
          versionData.fwVersion != Version::M100_31)
      {
        return false;
      }
      else if (versionData.fwVersion == Version::M100_31)
      {
        // CMDs not supported in Matrice 100
        if (cmdSet == OpenProtocolCMD::CMDSet::hardwareSync ||
            cmdSet == OpenProtocolCMD::CMDSet::mfio ||
            cmdSet == OpenProtocolCMD::CMDSet::subscribe)
        {
          return false;
        }
      }
      else if (isLegacyM600())
      {
        // CMDs not supported in Matrice 600 old firmware
        if (cmdSet == OpenProtocolCMD::CMDSet::hardwareSync ||
            cmdSet == OpenProtocolCMD::CMDSet::mfio ||
            cmdSet == OpenProtocolCMD::CMDSet::subscribe)
        {
          return false;
        }
      }
    }
  }
  return true;
}