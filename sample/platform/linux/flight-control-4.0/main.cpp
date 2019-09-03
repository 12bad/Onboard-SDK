/*! @file flight-control-4.0/main.cpp
 *  @version 3.9
 *  @date JULY 24 2019
 *
 *  @brief
 *  main for Flight Control API usage in a Linux environment.
 *  Provides a number of helpful additions to core API calls,
 *  especially for position control, attitude control, takeoff,
 *  landing.
 *
 *  @Copyright (c) 2016-2019 DJI
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
#include <dji_linux_helpers.hpp>
#include <dji_vehicle.hpp>

using namespace DJI::OSDK;
using namespace DJI::OSDK::Telemetry;
void setSampleCallBack(ErrorCode::ErrCodeType retCode, UserData SampleLog) {
  DSTATUS("Set Parameter RetCode : %d", retCode);
  if (retCode == ErrorCode::UnifiedErrCode::kNoError) {
    DSTATUS("Set Parameter Pass : %s.", SampleLog);
  } else {
    DERROR("Set Parameter Error : %s. Error code : %d", SampleLog, retCode);
  }
}
template <typename ParamType>
void getSampleCallBack(ErrorCode::ErrCodeType retCode, ParamType param,
                       UserData SampleLog) {
  DSTATUS("Get Parameter retCode : %d", retCode);
  if (retCode == ErrorCode::UnifiedErrCode::kNoError) {
    DSTATUS("Get Parameter Pass : %s.", SampleLog);
    DSTATUS("Get Parameter Parameter is  : %d", param);
  } else {
    DERROR("Get Parameter Error : %s. Error code : %d", SampleLog, retCode);
  }
}

int main(int argc, char **argv) {
  LinuxSetup linuxEnvironment(argc, argv);
  Vehicle *vehicle = linuxEnvironment.getVehicle();
  if (vehicle == NULL) {
    std::cout << "Vehicle not initialized, exiting.\n";
    return -1;
  }
  // Display interactive prompt
  std::cout << "| [a] Set and Get rtk Enable          |" << std::endl;
  std::cout << "| [b] Set Go and Home Altitude        |" << std::endl;
  std::cout << "| [c] Set Go Home Point               |" << std::endl;
  std::cout << "| [d] Set Avoid Obstacle Switch       |" << std::endl;
  char inputChar;
  std::cin >> inputChar;

  switch (inputChar) {
    case 'a':
      FlightAssistant::rtkEnableData rtkEnable;
      ErrorCode::ErrCodeType ret;
      vehicle->flightController->flightAssistant->setRTKEnableAsync(
          FlightAssistant::rtkEnableData::RTK_ENABLE, setSampleCallBack,
          (UserData) "rtk enable");
      vehicle->flightController->flightAssistant->getRTKEnableAsync(
          getSampleCallBack, (UserData) "rtk enable");
      ret = vehicle->flightController->flightAssistant->getRtkEnableSync(
          rtkEnable, 1);
      DSTATUS("rtkEanble:%d, ret:%d", rtkEnable, ret);
      sleep(3);
      ret = vehicle->flightController->flightAssistant->setRtkEnableSync(
          FlightAssistant::rtkEnableData::RTK_DISABLE, 1);
      DSTATUS("ret:%d", ret);

      ret = vehicle->flightController->flightAssistant->getRtkEnableSync(
          rtkEnable, 1);
      DSTATUS("rtkEanble:%d, ret:%d", rtkEnable, ret);
      break;

    case 'b':
      FlightAssistant::goHomeAltitude altitude;
      vehicle->flightController->flightAssistant->setGoHomeAltitudeAsync(
          200, setSampleCallBack, (UserData) "go home altitude");
      vehicle->flightController->flightAssistant->getGoHomeAltitudeAsync(
          getSampleCallBack, (UserData) "go home altitude");
      ret = vehicle->flightController->flightAssistant->setGoHomeAltitudeSync(
          550, 1);
      DSTATUS("ret:%d", ret);
      ret = vehicle->flightController->flightAssistant->getGoHomeAltitudeSync(
          altitude, 1);
      DSTATUS("ret:%d, altitude:%d", ret, altitude);
      break;

    case 'c':
      FlightAssistant::SetHomepointData req;
      req.type = FlightAssistant::HomePointType::DJI_HOMEPOINT_SDK_SET_LOCAIION;
      req.healthy = 0;
      req.latitude = 22.583360483302915 * 3.1415926 / 180;
      req.longitude = 113.95725278010300 * 3.1415926 / 180;
      vehicle->flightController->flightAssistant->setHomePointAsync(
          req, setSampleCallBack, (UserData) "home point");
      ret =
          vehicle->flightController->flightAssistant->setHomePointSync(req, 1);
      DSTATUS("ret:%d", ret);
      break;

    case 'd':
      FlightAssistant::AvoidObstacleData data;
      data.activeAvoidFlag = 0;
      data.leftBrakeFlag = 0;
      data.backBrakeFlag = 0;
      data.rightBrakeFlag = 0;
      data.frontBrakeFLag = 0;
      vehicle->flightController->flightAssistant->setAvoidObstacleSwitchAsync(
          data, setSampleCallBack, (UserData) "avoid obstacle data");
      ret = vehicle->flightController->flightAssistant
                ->setAvoidObstacleSwitchSync(data, 1);
      DSTATUS("ret:%d", ret);
      break;
    default:
      break;
  }
}
