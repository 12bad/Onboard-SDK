/*! @file flight_sample.hpp
 *  @version 3.9
 *  @date August 05 2019
 *
 *  @brief
 *  Flight Control API usage in a Linux environment.
 *  Provides a number of helpful additions to core API calls,
 *  especially for go home ,landing, set rtk and avoid obstacle switch.
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

#ifndef DJIOSDK_FLIGHT_SAMPLE_HPP
#define DJIOSDK_FLIGHT_SAMPLE_HPP

#include <dji_vehicle.hpp>

/*! @brief Sample to setup subscription
  *
  *  @param vehicle Vehicle pointer
  *  @param pkgIndex subscription package index
  *  @param freq subscription frequency
  *  @param topicList topic list array and see TopicName enum
  *  @param topicSize topic list array size
  *  @param timeout timeout
  *  @return result:true:success, false:fail
  */
bool setUpSubscription(DJI::OSDK::Vehicle* vehicle, int pkgIndex, int freq,
                       Telemetry::TopicName topicList[], uint8_t topicSize,
                       int timeout = 1);

/*! @brief Sample to teardown subscription
  *
  *  @param vehicle Vehicle pointer
  *  @param pkgIndex subscription package index
  *  @param timeout timeout
  *  @return result:true:success, false:fail
  */
bool teardownSubscription(Vehicle* vehicle, const int pkgIndex,
                          int timeout = 1);

/*! @brief Sample to get current home point and status
  *
  *  @param vehicle Vehicle pointer
  *  @param homePointSetStatus see Telemetry::HomePointStatus
  *  @param homePointInfo,see HomePointData struct define
  *  @param responseTimeout blocking timeout
  *  @return result true:success, false:fail
  */
bool getHomePoint(Vehicle* vehicle,
                  Telemetry::HomePointStatus& homePointSetStatus,
                  Telemetry::HomePointData& homePointInfo, int responseTimeout);

/*! @brief Sample check flight action started or not
  *
  *  @param vehicle Vehicle pointer
  *  @param mode see the DisplayMode enum
  *  @return result:true:success, false:fail
  */
bool checkActionStarted(Vehicle* vehicle, uint8_t mode);

/*! @brief Sample set go home altitude
  *
  *  @param vehicle Vehicle pointer
  *  @param altitude go home altitude request
  *  @param timeout timeout
  *  @return result:true:success, false:fail
  */
ErrorCode::ErrorCodeType setGoHomeAltitude(
    Vehicle* vehicle, FlightModule::GoHomeAltitude altitude,
    int timeout = 1);

/*! @brief Sample to set current aircraft position as an new home point
 *
 *  @param vehicle Vehicle pointer
 *  @param timeout timeout
 *  @return result:true:success, false:fail
 */
ErrorCode::ErrorCodeType setNewHomePoint(Vehicle* vehicle, int timeout = 1);

/*! @brief Sample to open avoid obstacle
 *
 *  @param vehicle Vehicle pointer
 *  @param timeout timeout
 *  @return result:true:success, false:fail
 */
ErrorCode::ErrorCodeType openAvoidObstacle(Vehicle* vehicle, int timeout = 1);

/*! @brief Sample to close avoid obstacle
 *
 *  @param vehicle Vehicle pointer
 *  @param timeout timeout
 *  @return result:true:success, false:fail
 */
ErrorCode::ErrorCodeType closeAvoidObstacle(Vehicle* vehicle, int timeout = 1);

/*! @brief Sample to open RTK switch
 *
 *  @param vehicle Vehicle pointer
 *  @param timeout timeout
 *  @return result:true:success, false:fail
 */
ErrorCode::ErrorCodeType openRtkSwtich(Vehicle* vehicle, int timeout = 1);

/*! @brief Sample to close  RTK switch
 *
 *  @param vehicle Vehicle pointer
 *  @param timeout timeout
 *  @return result:true:success, false:fail
 */
ErrorCode::ErrorCodeType closeRtkSwtich(Vehicle* vehicle, int timeout = 1);

/*! @brief Sample to go home, landing and force landing avoid ground
 *
 *  @param vehicle Vehicle pointer
 *  @param timeout timeout
 *  @return result:true:success, false:fail
 */
bool goHomeAndForceLanding(Vehicle* vehicle, int timeout = 1);

#endif  // DJIOSDK_FLIGHT_SAMPLE_HPP
