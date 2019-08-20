/*! @file camera_manager_async_sample.cpp
 *  @version 3.9
 *  @date July 29 2019
 *
 *  @brief
 *  Demonstrate how to use the asynchronous apis of camera manager.
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

#include "camera_manager_async_sample.hpp"

using namespace DJI::OSDK;
using namespace DJI::OSDK::Telemetry;

CameraManagerAsyncSample::CameraManagerAsyncSample(Vehicle *vehiclePtr)
    : vehicle(vehiclePtr) {}

CameraManagerAsyncSample::~CameraManagerAsyncSample() {}

void CameraManagerAsyncSample::getExposureModeCb(
    ErrorCode::ErrCodeType retCode, CameraModule::ExposureMode exposureModeGet,
    UserData userData) {
  AsyncSampleData *uData = (AsyncSampleData *)userData;

  DSTATUS("retCode : %d", retCode);
  if (!uData) {
    DERROR("User data is a null value.");
    return;
  }
  if (retCode == ErrorCode::UnifiedErrCode::kNoError) {
    DSTATUS("Get exposure mode = %d", exposureModeGet);
    if (uData->pm) {
      /*! compare the exposure mode set and get */
      if (*(CameraModule::ExposureMode *)uData->dataTarget == exposureModeGet) {
        DSTATUS("The exposure mode is already %d.", exposureModeGet);
        if (uData->userCallBack) {
          void (*cb)(ErrorCode::ErrCodeType, UserData);
          cb = (void (*)(ErrorCode::ErrCodeType, UserData))uData->userCallBack;
          cb(ErrorCode::UnifiedErrCode::kNoError, uData->userData);
        }
      } else {
        uData->pm->setExposureModeAsync(
            uData->index, *(CameraModule::ExposureMode *)uData->dataTarget,
            (void (*)(ErrorCode::ErrCodeType, UserData))uData->userCallBack,
            uData->userData);
      }
    }

  } else {
    DERROR("Get exposure mode error. Error code : %d", retCode);
    if (uData->userCallBack) {
      void (*cb)(ErrorCode::ErrCodeType, UserData);
      cb = (void (*)(ErrorCode::ErrCodeType, UserData))uData->userCallBack;
      /*! @TODO Temperarily using ret_code as errorcode, should be turned to be
       * a standrad one in the future */
      cb(retCode, uData->userData);
    }
  }
}

void CameraManagerAsyncSample::getISOCb(ErrorCode::ErrCodeType retCode,
                                   CameraModule::ISO isoGet,
                                   UserData userData) {
  AsyncSampleData *uData = (AsyncSampleData *)userData;

  DSTATUS("retCode : %d", retCode);
  if (!uData) {
    DERROR("User data is a null value.");
    return;
  }
  if (retCode == ErrorCode::UnifiedErrCode::kNoError) {
    DSTATUS("Get iso = %d", isoGet);
    if (uData->pm) {
      /*! compare the iso set and get */
      if (*(CameraModule::ISO *)uData->dataTarget == isoGet) {
        DSTATUS("The iso value is already %d.", isoGet);
        if (uData->userCallBack) {
          void (*cb)(ErrorCode::ErrCodeType, UserData);
          cb = (void (*)(ErrorCode::ErrCodeType, UserData))uData->userCallBack;
          cb(ErrorCode::UnifiedErrCode::kNoError, uData->userData);
        }
      } else {
        uData->pm->setISOAsync(
            uData->index, *(CameraModule::ISO *)uData->dataTarget,
            (void (*)(ErrorCode::ErrCodeType, UserData))uData->userCallBack,
            uData->userData);
      }
    }

  } else {
    DERROR("Get iso error. Error code : %d", retCode);
    if (uData->userCallBack) {
      void (*cb)(ErrorCode::ErrCodeType, UserData);
      cb = (void (*)(ErrorCode::ErrCodeType, UserData))uData->userCallBack;
      /*! @TODO Temperarily using ret_code as errorcode, should be turned to be
       * a standrad one in the future */
      cb(retCode, uData->userData);
    }
  }
}

void CameraManagerAsyncSample::getShutterSpeedCb(
    ErrorCode::ErrCodeType retCode, CameraModule::ShutterSpeed shutterSpeedGet,
    UserData userData) {
  AsyncSampleData *uData = (AsyncSampleData *)userData;

  DSTATUS("retCode : %d", retCode);
  if (!uData) {
    DERROR("User data is a null value.");
    return;
  }
  if (retCode == ErrorCode::UnifiedErrCode::kNoError) {
    DSTATUS("Get shutter speed = %d", shutterSpeedGet);
    if (uData->pm) {
      /*! compare the shutter speed set and get */
      if (*(CameraModule::ShutterSpeed *)uData->dataTarget == shutterSpeedGet) {
        DSTATUS("The shutter speed  value is already %d.", shutterSpeedGet);
        if (uData->userCallBack) {
          void (*cb)(ErrorCode::ErrCodeType, UserData);
          cb = (void (*)(ErrorCode::ErrCodeType, UserData))uData->userCallBack;
          cb(ErrorCode::UnifiedErrCode::kNoError, uData->userData);
        }
      } else {
        uData->pm->setShutterSpeedAsync(
            uData->index, *(CameraModule::ShutterSpeed *)uData->dataTarget,
            (void (*)(ErrorCode::ErrCodeType, UserData))uData->userCallBack,
            uData->userData);
      }
    }

  } else {
    DERROR("Get shutter speed error. Error code : %d", retCode);
    if (uData->userCallBack) {
      void (*cb)(ErrorCode::ErrCodeType, UserData);
      cb = (void (*)(ErrorCode::ErrCodeType, UserData))uData->userCallBack;
      /*! @TODO Temperarily using ret_code as errorcode, should be turned to be
       * a standrad one in the future */
      cb(retCode, uData->userData);
    }
  }
}

void CameraManagerAsyncSample::getApertureCb(ErrorCode::ErrCodeType retCode,
                                        CameraModule::Aperture apertureGet,
                                        UserData userData) {
  AsyncSampleData *uData = (AsyncSampleData *)userData;

  DSTATUS("retCode : %d", retCode);
  if (!uData) {
    DERROR("User data is a null value.");
    return;
  }
  if (retCode == ErrorCode::UnifiedErrCode::kNoError) {
    DSTATUS("Get aperture = %d", apertureGet);
    if (uData->pm) {
      /*! compare the aperture set and get */
      if (*(CameraModule::Aperture *)uData->dataTarget == apertureGet) {
        DSTATUS("The aperture value is already %d.", apertureGet);
        if (uData->userCallBack) {
          void (*cb)(ErrorCode::ErrCodeType, UserData);
          cb = (void (*)(ErrorCode::ErrCodeType, UserData))uData->userCallBack;
          cb(ErrorCode::UnifiedErrCode::kNoError, uData->userData);
        }
      } else {
        uData->pm->setApertureAsync(
            uData->index, *(CameraModule::Aperture *)uData->dataTarget,
            (void (*)(ErrorCode::ErrCodeType, UserData))uData->userCallBack,
            uData->userData);
      }
    }

  } else {
    DERROR("Get aperture error. Error code : %d", retCode);
    if (uData->userCallBack) {
      void (*cb)(ErrorCode::ErrCodeType, UserData);
      cb = (void (*)(ErrorCode::ErrCodeType, UserData))uData->userCallBack;
      /*! @TODO Temperarily using ret_code as errorcode, should be turned to be
       * a standrad one in the future */
      cb(retCode, uData->userData);
    }
  }
}

void CameraManagerAsyncSample::getEVCb(ErrorCode::ErrCodeType retCode,
                                  CameraModule::ExposureCompensation evGet,
                                  UserData userData) {
  AsyncSampleData *uData = (AsyncSampleData *)userData;

  DSTATUS("retCode : %d", retCode);
  if (!uData) {
    DERROR("User data is a null value.");
    return;
  }
  if (retCode == ErrorCode::UnifiedErrCode::kNoError) {
    DSTATUS("Get exposure compensation value = %d", evGet);
    if (uData->pm) {
      /*! compare the exposure compensation set and get */
      if (*(CameraModule::ExposureCompensation *)uData->dataTarget == evGet) {
        DSTATUS("The exposure compensation value is already %d.", evGet);
        if (uData->userCallBack) {
          void (*cb)(ErrorCode::ErrCodeType, UserData);
          cb = (void (*)(ErrorCode::ErrCodeType, UserData))uData->userCallBack;
          cb(ErrorCode::UnifiedErrCode::kNoError, uData->userData);
        }
      } else {
        uData->pm->setExposureCompensationAsync(
            uData->index,
            *(CameraModule::ExposureCompensation *)uData->dataTarget,
            (void (*)(ErrorCode::ErrCodeType, UserData))uData->userCallBack,
            uData->userData);
      }
    }

  } else {
    DERROR("Get exposure compensation value error. Error code : %d", retCode);
    if (uData->userCallBack) {
      void (*cb)(ErrorCode::ErrCodeType, UserData);
      cb = (void (*)(ErrorCode::ErrCodeType, UserData))uData->userCallBack;
      /*! @TODO Temperarily using ret_code as errorcode, should be turned to be
       * a standrad one in the future */
      cb(retCode, uData->userData);
    }
  }
}

void CameraManagerAsyncSample::setExposureModeAsyncSample(
    PayloadIndexType index, CameraModule::ExposureMode dataTarget,
    void (*UserCallBack)(ErrorCode::ErrCodeType retCode, UserData userData),
    UserData userData) {
  if (!vehicle || !vehicle->cameraManager) {
    DERROR("vehicle or cameraManager is a null value.");
    if (UserCallBack)
      UserCallBack(ErrorCode::UnifiedErrCode::kErrorInvalidParam, userData);
    return;
  }
  CameraManager *pm = vehicle->cameraManager;
  static AsyncSampleData uData;
  uData.index = index;
  uData.pm = pm;
  uData.userCallBack = (void *)UserCallBack;
  uData.userData = userData;
  memcpy(uData.dataTarget, &dataTarget, sizeof(dataTarget));

  /*! get the value from camera */
  DSTATUS("Get exposure mode now ...");
  pm->getExposureModeAsync(index, getExposureModeCb, &uData);
}

void CameraManagerAsyncSample::setISOAsyncSample(
    PayloadIndexType index, CameraModule::ISO dataTarget,
    void (*UserCallBack)(ErrorCode::ErrCodeType retCode, UserData userData),
    UserData userData) {
  if (!vehicle || !vehicle->cameraManager) {
    DERROR("vehicle or cameraManager is a null value.");
    if (UserCallBack)
      UserCallBack(ErrorCode::UnifiedErrCode::kErrorInvalidParam, userData);
    return;
  }
  CameraManager *pm = vehicle->cameraManager;
  static AsyncSampleData uData;
  uData.index = index;
  uData.pm = pm;
  uData.userCallBack = (void *)UserCallBack;
  uData.userData = userData;
  memcpy(uData.dataTarget, &dataTarget, sizeof(dataTarget));

  /*! get the value from camera */
  DSTATUS("Get iso now ...");
  pm->getISOAsync(index, getISOCb, &uData);
}

void CameraManagerAsyncSample::setShutterSpeedAsyncSample(
    PayloadIndexType index, CameraModule::ShutterSpeed dataTarget,
    void (*UserCallBack)(ErrorCode::ErrCodeType retCode, UserData userData),
    UserData userData) {
  if (!vehicle || !vehicle->cameraManager) {
    DERROR("vehicle or cameraManager is a null value.");
    if (UserCallBack)
      UserCallBack(ErrorCode::UnifiedErrCode::kErrorInvalidParam, userData);
    return;
  }
  CameraManager *pm = vehicle->cameraManager;
  static AsyncSampleData uData;
  uData.index = index;
  uData.pm = pm;
  uData.userCallBack = (void *)UserCallBack;
  uData.userData = userData;
  memcpy(uData.dataTarget, &dataTarget, sizeof(dataTarget));

  /*! get the value from camera */
  DSTATUS("Get shutter speed now ...");
  pm->getShutterSpeedAsync(index, getShutterSpeedCb, &uData);
}

void CameraManagerAsyncSample::setApertureAsyncSample(
    PayloadIndexType index, CameraModule::Aperture dataTarget,
    void (*UserCallBack)(ErrorCode::ErrCodeType retCode, UserData userData),
    UserData userData) {
  if (!vehicle || !vehicle->cameraManager) {
    DERROR("vehicle or cameraManager is a null value.");
    if (UserCallBack)
      UserCallBack(ErrorCode::UnifiedErrCode::kErrorInvalidParam, userData);
    return;
  }
  CameraManager *pm = vehicle->cameraManager;
  static AsyncSampleData uData;
  uData.index = index;
  uData.pm = pm;
  uData.userCallBack = (void *)UserCallBack;
  uData.userData = userData;
  memcpy(uData.dataTarget, &dataTarget, sizeof(dataTarget));

  /*! get the value from camera */
  DSTATUS("Get aperture now ...");
  pm->getApertureAsync(index, getApertureCb, &uData);
}

void CameraManagerAsyncSample::setEVAsyncSample(
    PayloadIndexType index, CameraModule::ExposureCompensation dataTarget,
    void (*UserCallBack)(ErrorCode::ErrCodeType retCode, UserData userData),
    UserData userData) {
  if (!vehicle || !vehicle->cameraManager) {
    DERROR("vehicle or cameraManager is a null value.");
    if (UserCallBack)
      UserCallBack(ErrorCode::UnifiedErrCode::kErrorInvalidParam, userData);
    return;
  }
  CameraManager *pm = vehicle->cameraManager;
  static AsyncSampleData uData;
  uData.index = index;
  uData.pm = pm;
  uData.userCallBack = (void *)UserCallBack;
  uData.userData = userData;
  memcpy(uData.dataTarget, &dataTarget, sizeof(dataTarget));

  /*! get the value from camera */
  DSTATUS("Get ev now ...");
  pm->getExposureCompensationAsync(index, getEVCb, &uData);
}

void CameraManagerAsyncSample::setFocusModeCb(ErrorCode::ErrCodeType retCode,
                                         UserData userData) {
  AsyncSampleData *uData = (AsyncSampleData *)userData;

  DSTATUS("retCode : %d", retCode);
  if (!uData) {
    DERROR("User data is a null value.");
    return;
  }
  if (retCode == ErrorCode::UnifiedErrCode::kNoError) {
    DSTATUS("Set focus mode as Auto successfully ");
    if (uData->pm) {
      /*! set focus point */
      uData->pm->setFocusTargetAsync(
          uData->index, *(CameraModule::TapFocusPosData *)uData->dataTarget,
          (void (*)(ErrorCode::ErrCodeType, UserData))uData->userCallBack,
          uData->userData);
    }

  } else {
    DERROR("Set focus mode error. Error code : %d", retCode);
    if (uData->userCallBack) {
      void (*cb)(ErrorCode::ErrCodeType, UserData);
      cb = (void (*)(ErrorCode::ErrCodeType, UserData))uData->userCallBack;
      /*! @TODO Temperarily using ret_code as errorcode, should be turned to be
       * a standrad one in the future */
      cb(retCode, uData->userData);
    }
  }
}

void CameraManagerAsyncSample::setFocusPointAsyncSample(
    PayloadIndexType index, float x, float y,
    void (*UserCallBack)(ErrorCode::ErrCodeType retCode, UserData userData),
    UserData userData) {
  if (!vehicle || !vehicle->cameraManager) {
    DERROR("vehicle or cameraManager is a null value.");
    if (UserCallBack)
      UserCallBack(ErrorCode::UnifiedErrCode::kErrorInvalidParam, userData);
    return;
  }
  CameraManager *pm = vehicle->cameraManager;
  static AsyncSampleData uData;
  uData.index = index;
  uData.pm = pm;
  uData.userCallBack = (void *)UserCallBack;
  uData.userData = userData;
  CameraModule::TapFocusPosData tapFocusPos{x, y};
  memcpy(uData.dataTarget, &tapFocusPos, sizeof(tapFocusPos));

  /*!< set camera focus mode to be CameraModule::FocusMode::AUTO */
  DSTATUS("Set focus mode = %d", CameraModule::FocusMode::AUTO);
  pm->setFocusModeAsync(index, CameraModule::FocusMode::AUTO, setFocusModeCb,
                        &uData);
}

void CameraManagerAsyncSample::setTapZoomEnableCb(ErrorCode::ErrCodeType retCode,
                                             UserData userData) {
  AsyncSampleData *uData = (AsyncSampleData *)userData;

  DSTATUS("retCode : %d", retCode);
  if (!uData) {
    DERROR("User data is a null value.");
    return;
  }
  if (retCode == ErrorCode::UnifiedErrCode::kNoError) {
    DSTATUS("Set tap zoom enable successfully ");
    if (uData->pm) {
      /*! set tap zoom point */
      uData->pm->tapZoomAtTargetAsync(
          uData->index, *(CameraModule::TapZoomPosData *)uData->dataTarget,
          (void (*)(ErrorCode::ErrCodeType, UserData))uData->userCallBack,
          uData->userData);
    }

  } else {
    DERROR("Tap zoom at enable error. Error code : %d", retCode);
    if (uData->userCallBack) {
      void (*cb)(ErrorCode::ErrCodeType, UserData);
      cb = (void (*)(ErrorCode::ErrCodeType, UserData))uData->userCallBack;
      /*! @TODO Temperarily using ret_code as errorcode, should be turned to be
       * a standrad one in the future */
      cb(retCode, uData->userData);
    }
  }
}

void CameraManagerAsyncSample::setTapZoomMultiplierCb(ErrorCode::ErrCodeType retCode,
                                                 UserData userData) {
  AsyncSampleData *uData = (AsyncSampleData *)userData;

  DSTATUS("retCode : %d", retCode);
  if (!uData) {
    DERROR("User data is a null value.");
    return;
  }
  if (retCode == ErrorCode::UnifiedErrCode::kNoError) {
    DSTATUS("Set tap zoom multiplier successfully ");
    if (uData->pm) {
      /*! set tap zoom enable */
      uData->pm->setTapZoomEnabledAsync(uData->index, true, setTapZoomEnableCb,
                                        uData);
    }

  } else {
    DERROR("Set tap zoom multiplier error. Error code : %d", retCode);
    if (uData->userCallBack) {
      void (*cb)(ErrorCode::ErrCodeType, UserData);
      cb = (void (*)(ErrorCode::ErrCodeType, UserData))uData->userCallBack;
      /*! @TODO Temperarily using ret_code as errorcode, should be turned to be
       * a standrad one in the future */
      cb(retCode, uData->userData);
    }
  }
}

void CameraManagerAsyncSample::setTapZoomPointAsyncSample(
    PayloadIndexType index, uint8_t multiplier, float x, float y,
    void (*UserCallBack)(ErrorCode::ErrCodeType retCode, UserData userData),
    UserData userData) {
  if (!vehicle || !vehicle->cameraManager) {
    DERROR("vehicle or cameraManager is a null value.");
    if (UserCallBack)
      UserCallBack(ErrorCode::UnifiedErrCode::kErrorInvalidParam, userData);
    return;
  }
  CameraManager *pm = vehicle->cameraManager;
  static AsyncSampleData uData;
  uData.index = index;
  uData.pm = pm;
  uData.userCallBack = (void *)UserCallBack;
  uData.userData = userData;
  CameraModule::TapZoomPosData tapZoomPos{x, y};
  memcpy(uData.dataTarget, &tapZoomPos, sizeof(tapZoomPos));

  /*!< set camera tap zoom multiplier parameter */
  DSTATUS("Set tap zoom multiplier = %d", multiplier);
  pm->setTapZoomMultiplierAsync(index, multiplier, setTapZoomMultiplierCb,
                                &uData);
}

void CameraManagerAsyncSample::startZoomAsyncSample(
    PayloadIndexType index, CameraModule::zoomDirectionData direction,
    CameraModule::zoomSpeedData speed,
    void (*UserCallBack)(ErrorCode::ErrCodeType retCode, UserData userData),
    UserData userData) {
  if (!vehicle || !vehicle->cameraManager) {
    DERROR("vehicle or cameraManager is a null value.");
    if (UserCallBack)
      UserCallBack(ErrorCode::UnifiedErrCode::kErrorInvalidParam, userData);
    return;
  }
  CameraManager *pm = vehicle->cameraManager;

  /*!< stop camera shooting photo */
  DSTATUS("start to continuous zoom");
  pm->startContinuousOpticalZoomAsync(index, direction, speed, UserCallBack,
                                      userData);
}

void CameraManagerAsyncSample::stopZoomAsyncSample(
    PayloadIndexType index,
    void (*UserCallBack)(ErrorCode::ErrCodeType retCode, UserData userData),
    UserData userData) {
  if (!vehicle || !vehicle->cameraManager) {
    DERROR("vehicle or cameraManager is a null value.");
    if (UserCallBack)
      UserCallBack(ErrorCode::UnifiedErrCode::kErrorInvalidParam, userData);
    return;
  }
  CameraManager *pm = vehicle->cameraManager;

  /*!< stop camera shooting photo */
  DSTATUS("stop continuous zoom");
  pm->stopContinuousOpticalZoomAsync(index, UserCallBack, userData);
}

void CameraManagerAsyncSample::setCameraModeForRecordVideoCb(
    ErrorCode::ErrCodeType retCode, UserData userData) {
  AsyncSampleData *uData = (AsyncSampleData *)userData;

  DSTATUS("retCode : %d", retCode);
  if (!uData) {
    DERROR("User data is a null value.");
    return;
  }
  if (retCode == ErrorCode::UnifiedErrCode::kNoError) {
    DSTATUS("Set camera work mode successfully ");
    if (uData->pm) {
      /*! start to record video */
      DSTATUS("Start to RECORD_VIDEO");
      uData->pm->startRecordVideoAsync(
          uData->index,
          (void (*)(ErrorCode::ErrCodeType, UserData))uData->userCallBack,
          uData->userData);
    }
  } else {
    DERROR("Start to record video error. Error code : %d", retCode);
    if (uData->userCallBack) {
      void (*cb)(ErrorCode::ErrCodeType, UserData);
      cb = (void (*)(ErrorCode::ErrCodeType, UserData))uData->userCallBack;
      /*! @TODO Temperarily using ret_code as errorcode, should be turned to be
       * a standrad one in the future */
      cb(retCode, uData->userData);
    }
  }
}

void CameraManagerAsyncSample::startRecordVideoAsyncSample(
    PayloadIndexType index,
    void (*UserCallBack)(ErrorCode::ErrCodeType retCode, UserData userData),
    UserData userData) {
  if (!vehicle || !vehicle->cameraManager) {
    DERROR("vehicle or cameraManager is a null value.");
    if (UserCallBack)
      UserCallBack(ErrorCode::UnifiedErrCode::kErrorInvalidParam, userData);
    return;
  }
  CameraManager *pm = vehicle->cameraManager;
  static AsyncSampleData uData;
  uData.index = index;
  uData.pm = pm;
  uData.userCallBack = (void *)UserCallBack;
  uData.userData = userData;

  /*!< set camera work mode as RECORD_VIDEO */
  DSTATUS("Set camera mode to RECORD_VIDEO");
  pm->setModeAsync(index, CameraModule::WorkMode::RECORD_VIDEO,
                   setCameraModeForRecordVideoCb, &uData);
}

void CameraManagerAsyncSample::stopRecordVideoAsyncSample(
    PayloadIndexType index,
    void (*UserCallBack)(ErrorCode::ErrCodeType retCode, UserData userData),
    UserData userData) {
  if (!vehicle || !vehicle->cameraManager) {
    DERROR("vehicle or cameraManager is a null value.");
    if (UserCallBack)
      UserCallBack(ErrorCode::UnifiedErrCode::kErrorInvalidParam, userData);
    return;
  }
  CameraManager *pm = vehicle->cameraManager;

  /*!< stop camera recording video */
  DSTATUS("stop recording video");
  pm->stopRecordVideoAsync(index, UserCallBack, userData);
}

void CameraManagerAsyncSample::setShootPhotoModeForSingleShootCb(
    ErrorCode::ErrCodeType retCode, UserData userData) {
  AsyncSampleData *uData = (AsyncSampleData *)userData;

  DSTATUS("retCode : %d", retCode);
  if (!uData) {
    DERROR("User data is a null value.");
    return;
  }
  if (retCode == ErrorCode::UnifiedErrCode::kNoError) {
    DSTATUS("Set shoot photo mode as Single successfully ");
    if (uData->pm) {
      /*! start to shoot SINGLE photo */
      uData->pm->startShootPhotoAsync(
          uData->index, CameraModule::ShootPhotoMode::SINGLE,
          (void (*)(ErrorCode::ErrCodeType retCode,
                    UserData userData))uData->userCallBack,
          uData->userData);
    }
  } else {
    DERROR("Set shoot photo mode error. Error code : %d", retCode);
    if (uData->userCallBack) {
      void (*cb)(ErrorCode::ErrCodeType, UserData);
      cb = (void (*)(ErrorCode::ErrCodeType, UserData))uData->userCallBack;
      /*! @TODO Temperarily using ret_code as errorcode, should be turned to be
       * a standrad one in the future */
      cb(retCode, uData->userData);
    }
  }
}

void CameraManagerAsyncSample::setCameraModeForSingleShootCb(
    ErrorCode::ErrCodeType retCode, UserData userData) {
  AsyncSampleData *uData = (AsyncSampleData *)userData;

  DSTATUS("retCode : %d", retCode);
  if (!uData) {
    DERROR("User data is a null value.");
    return;
  }
  if (retCode == ErrorCode::UnifiedErrCode::kNoError) {
    DSTATUS("Set camera work mode successfully ");
    if (uData->pm) {
      /*!< set shoot-photo mode */
      DSTATUS("set shoot-photo mode as SINGLE");
      uData->pm->setShootPhotoModeAsync(
          uData->index, CameraModule::ShootPhotoMode::SINGLE,
          setShootPhotoModeForSingleShootCb, uData);
    }
  } else {
    DERROR("Set camera mode error. Error code : %d", retCode);
    if (uData->userCallBack) {
      void (*cb)(ErrorCode::ErrCodeType, UserData);
      cb = (void (*)(ErrorCode::ErrCodeType, UserData))uData->userCallBack;
      /*! @TODO Temperarily using ret_code as errorcode, should be turned to be
       * a standrad one in the future */
      cb(retCode, uData->userData);
    }
  }
}

void CameraManagerAsyncSample::startShootSinglePhotoAsyncSample(
    PayloadIndexType index,
    void (*UserCallBack)(ErrorCode::ErrCodeType retCode, UserData userData),
    UserData userData) {
  if (!vehicle || !vehicle->cameraManager) {
    DERROR("vehicle or cameraManager is a null value.");
    if (UserCallBack)
      UserCallBack(ErrorCode::UnifiedErrCode::kErrorInvalidParam, userData);
    return;
  }
  CameraManager *pm = vehicle->cameraManager;
  static AsyncSampleData uData;
  uData.index = index;
  uData.pm = pm;
  uData.userCallBack = (void *)UserCallBack;
  uData.userData = userData;

  /*!< set camera work mode as shoot photo */
  DSTATUS("set camera work mode as SHOOT_PHOTO");
  pm->setModeAsync(index, CameraModule::WorkMode::SHOOT_PHOTO,
                   setCameraModeForSingleShootCb, &uData);
}

void CameraManagerAsyncSample::setPhotoBurstCountCb(ErrorCode::ErrCodeType retCode,
                                               UserData userData) {
  AsyncSampleData *uData = (AsyncSampleData *)userData;

  DSTATUS("retCode : %d", retCode);
  if (!uData) {
    DERROR("User data is a null value.");
    return;
  }
  if (retCode == ErrorCode::UnifiedErrCode::kNoError) {
    DSTATUS("Set photo burst count successfully ");
    if (uData->pm) {
      /*! set BURST parameter */
      uData->pm->startShootPhotoAsync(
          uData->index, CameraModule::ShootPhotoMode::BURST,
          (void (*)(ErrorCode::ErrCodeType retCode,
                    UserData userData))uData->userCallBack,
          uData->userData);
    }
  } else {
    DERROR("Set burst count error. Error code : %d", retCode);
    if (uData->userCallBack) {
      void (*cb)(ErrorCode::ErrCodeType, UserData);
      cb = (void (*)(ErrorCode::ErrCodeType, UserData))uData->userCallBack;
      /*! @TODO Temperarily using ret_code as errorcode, should be turned to be
       * a standrad one in the future */
      cb(retCode, uData->userData);
    }
  }
}

void CameraManagerAsyncSample::setShootPhotoModeForBurstShootCb(
    ErrorCode::ErrCodeType retCode, UserData userData) {
  AsyncSampleData *uData = (AsyncSampleData *)userData;

  DSTATUS("retCode : %d", retCode);
  if (!uData) {
    DERROR("User data is a null value.");
    return;
  }
  if (retCode == ErrorCode::UnifiedErrCode::kNoError) {
    DSTATUS("Set shoot photo mode as BURST successfully ");
    if (uData->pm) {
      /*! set BURST parameter */
      uData->pm->setPhotoBurstCountAsync(
          uData->index, *(CameraModule::PhotoBurstCount *)uData->dataTarget,
          setPhotoBurstCountCb, uData);
    }
  } else {
    DERROR("Set shoot photo mode as BURST error. Error code : %d", retCode);
    if (uData->userCallBack) {
      void (*cb)(ErrorCode::ErrCodeType, UserData);
      cb = (void (*)(ErrorCode::ErrCodeType, UserData))uData->userCallBack;
      /*! @TODO Temperarily using ret_code as errorcode, should be turned to be
       * a standrad one in the future */
      cb(retCode, uData->userData);
    }
  }
}

void CameraManagerAsyncSample::setCameraModeForBurstShootCb(
    ErrorCode::ErrCodeType retCode, UserData userData) {
  AsyncSampleData *uData = (AsyncSampleData *)userData;

  DSTATUS("retCode : %d", retCode);
  if (!uData) {
    DERROR("User data is a null value.");
    return;
  }
  if (retCode == ErrorCode::UnifiedErrCode::kNoError) {
    DSTATUS("Set camera work mode successfully ");
    if (uData->pm) {
      /*!< set shoot-photo mode */
      DSTATUS("set shoot-photo mode as BURST");
      uData->pm->setShootPhotoModeAsync(
          uData->index, CameraModule::ShootPhotoMode::BURST,
          setShootPhotoModeForBurstShootCb, uData);
    }
  } else {
    DERROR("Set camera mode error. Error code : %d", retCode);
    if (uData->userCallBack) {
      void (*cb)(ErrorCode::ErrCodeType, UserData);
      cb = (void (*)(ErrorCode::ErrCodeType, UserData))uData->userCallBack;
      /*! @TODO Temperarily using ret_code as errorcode, should be turned to be
       * a standrad one in the future */
      cb(retCode, uData->userData);
    }
  }
}

void CameraManagerAsyncSample::startShootBurstPhotoAsyncSample(
    PayloadIndexType index, CameraModule::PhotoBurstCount count,
    void (*UserCallBack)(ErrorCode::ErrCodeType retCode, UserData userData),
    UserData userData) {
  if (!vehicle || !vehicle->cameraManager) {
    DERROR("vehicle or cameraManager is a null value.");
    if (UserCallBack)
      UserCallBack(ErrorCode::UnifiedErrCode::kErrorInvalidParam, userData);
    return;
  }
  CameraManager *pm = vehicle->cameraManager;
  static AsyncSampleData uData;
  uData.index = index;
  uData.pm = pm;
  uData.userCallBack = (void *)UserCallBack;
  uData.userData = userData;
  memcpy(uData.dataTarget, &count, sizeof(count));

  /*!< set camera work mode as shoot photo */
  DSTATUS("set camera work mode as SHOOT_PHOTO");
  pm->setModeAsync(index, CameraModule::WorkMode::SHOOT_PHOTO,
                   setCameraModeForBurstShootCb, &uData);
}

void CameraManagerAsyncSample::setPhotoAEBCountCb(ErrorCode::ErrCodeType retCode,
                                             UserData userData) {
  AsyncSampleData *uData = (AsyncSampleData *)userData;

  DSTATUS("retCode : %d", retCode);
  if (!uData) {
    DERROR("User data is a null value.");
    return;
  }
  if (retCode == ErrorCode::UnifiedErrCode::kNoError) {
    DSTATUS("Set photo AEB count successfully ");
    if (uData->pm) {
      /*! set AEB parameter */
      uData->pm->startShootPhotoAsync(
          uData->index, CameraModule::ShootPhotoMode::AEB,
          (void (*)(ErrorCode::ErrCodeType retCode,
                    UserData userData))uData->userCallBack,
          uData->userData);
    }
  } else {
    DERROR("Set AEB count error. Error code : %d", retCode);
    if (uData->userCallBack) {
      void (*cb)(ErrorCode::ErrCodeType, UserData);
      cb = (void (*)(ErrorCode::ErrCodeType, UserData))uData->userCallBack;
      /*! @TODO Temperarily using ret_code as errorcode, should be turned to be
       * a standrad one in the future */
      cb(retCode, uData->userData);
    }
  }
}

void CameraManagerAsyncSample::setShootPhotoModeForAEBShootCb(
    ErrorCode::ErrCodeType retCode, UserData userData) {
  AsyncSampleData *uData = (AsyncSampleData *)userData;

  DSTATUS("retCode : %d", retCode);
  if (!uData) {
    DERROR("User data is a null value.");
    return;
  }
  if (retCode == ErrorCode::UnifiedErrCode::kNoError) {
    DSTATUS("Set shoot photo mode as AEB successfully ");
    if (uData->pm) {
      /*! set AEB parameter */
      uData->pm->setPhotoAEBCountAsync(
          uData->index, *(CameraModule::PhotoAEBCount *)uData->dataTarget,
          setPhotoAEBCountCb, uData);
    }
  } else {
    DERROR("Set shoot photo mode as AEB error. Error code : %d", retCode);
    if (uData->userCallBack) {
      void (*cb)(ErrorCode::ErrCodeType, UserData);
      cb = (void (*)(ErrorCode::ErrCodeType, UserData))uData->userCallBack;
      /*! @TODO Temperarily using ret_code as errorcode, should be turned to be
       * a standrad one in the future */
      cb(retCode, uData->userData);
    }
  }
}

void CameraManagerAsyncSample::setCameraModeForAEBShootCb(
    ErrorCode::ErrCodeType retCode, UserData userData) {
  AsyncSampleData *uData = (AsyncSampleData *)userData;

  DSTATUS("retCode : %d", retCode);
  if (!uData) {
    DERROR("User data is a null value.");
    return;
  }
  if (retCode == ErrorCode::UnifiedErrCode::kNoError) {
    DSTATUS("Set camera work mode successfully ");
    if (uData->pm) {
      /*!< set shoot-photo mode */
      DSTATUS("set shoot-photo mode as AEB");
      uData->pm->setShootPhotoModeAsync(uData->index,
                                        CameraModule::ShootPhotoMode::AEB,
                                        setShootPhotoModeForAEBShootCb, uData);
    }
  } else {
    DERROR("Set camera mode error. Error code : %d", retCode);
    if (uData->userCallBack) {
      void (*cb)(ErrorCode::ErrCodeType, UserData);
      cb = (void (*)(ErrorCode::ErrCodeType, UserData))uData->userCallBack;
      /*! @TODO Temperarily using ret_code as errorcode, should be turned to be
       * a standrad one in the future */
      cb(retCode, uData->userData);
    }
  }
}

void CameraManagerAsyncSample::startShootAEBPhotoAsyncSample(
    PayloadIndexType index, CameraModule::PhotoAEBCount count,
    void (*UserCallBack)(ErrorCode::ErrCodeType retCode, UserData userData),
    UserData userData) {
  if (!vehicle || !vehicle->cameraManager) {
    DERROR("vehicle or cameraManager is a null value.");
    if (UserCallBack)
      UserCallBack(ErrorCode::UnifiedErrCode::kErrorInvalidParam, userData);
    return;
  }
  CameraManager *pm = vehicle->cameraManager;
  static AsyncSampleData uData;
  uData.index = index;
  uData.pm = pm;
  uData.userCallBack = (void *)UserCallBack;
  uData.userData = userData;
  memcpy(uData.dataTarget, &count, sizeof(count));

  /*!< set camera work mode as shoot photo */
  DSTATUS("set camera work mode as SHOOT_PHOTO");
  pm->setModeAsync(index, CameraModule::WorkMode::SHOOT_PHOTO,
                   setCameraModeForAEBShootCb, &uData);
}

void CameraManagerAsyncSample::setPhotoIntervalCb(ErrorCode::ErrCodeType retCode,
                                             UserData userData) {
  AsyncSampleData *uData = (AsyncSampleData *)userData;

  DSTATUS("retCode : %d", retCode);
  if (!uData) {
    DERROR("User data is a null value.");
    return;
  }
  if (retCode == ErrorCode::UnifiedErrCode::kNoError) {
    DSTATUS("Set time interval parameter successfully ");
    if (uData->pm) {
      /*! start to shoot interval photo */
      uData->pm->startShootPhotoAsync(
          uData->index, CameraModule::ShootPhotoMode::INTERVAL,
          (void (*)(ErrorCode::ErrCodeType retCode,
                    UserData userData))uData->userCallBack,
          uData->userData);
    }
  } else {
    DERROR("Set time interval parameter error. Error code : %d", retCode);
    if (uData->userCallBack) {
      void (*cb)(ErrorCode::ErrCodeType, UserData);
      cb = (void (*)(ErrorCode::ErrCodeType, UserData))uData->userCallBack;
      /*! @TODO Temperarily using ret_code as errorcode, should be turned to be
       * a standrad one in the future */
      cb(retCode, uData->userData);
    }
  }
}

void CameraManagerAsyncSample::setShootPhotoModeForIntervalShootCb(
    ErrorCode::ErrCodeType retCode, UserData userData) {
  AsyncSampleData *uData = (AsyncSampleData *)userData;

  DSTATUS("retCode : %d", retCode);
  if (!uData) {
    DERROR("User data is a null value.");
    return;
  }
  if (retCode == ErrorCode::UnifiedErrCode::kNoError) {
    DSTATUS("Set shoot photo mode as INTERVAL successfully ");
    if (uData->pm) {
      /*! set timer interval parameter */
      uData->pm->setPhotoTimeIntervalSettingsAsync(
          uData->index, *(CameraModule::PhotoIntervalData *)uData->dataTarget,
          setPhotoIntervalCb, uData);
    }
  } else {
    DERROR("Set shoot photo mode as INTERVAL error. Error code : %d", retCode);
    if (uData->userCallBack) {
      void (*cb)(ErrorCode::ErrCodeType, UserData);
      cb = (void (*)(ErrorCode::ErrCodeType, UserData))uData->userCallBack;
      /*! @TODO Temperarily using ret_code as errorcode, should be turned to be
       * a standrad one in the future */
      cb(retCode, uData->userData);
    }
  }
}

void CameraManagerAsyncSample::setCameraModeForIntervalShootCb(
    ErrorCode::ErrCodeType retCode, UserData userData) {
  AsyncSampleData *uData = (AsyncSampleData *)userData;

  DSTATUS("retCode : %d", retCode);
  if (!uData) {
    DERROR("User data is a null value.");
    return;
  }
  if (retCode == ErrorCode::UnifiedErrCode::kNoError) {
    DSTATUS("Set camera work mode successfully ");
    if (uData->pm) {
      /*!< set shoot-photo mode */
      DSTATUS("set shoot-photo mode as INTERVAL");
      uData->pm->setShootPhotoModeAsync(
          uData->index, CameraModule::ShootPhotoMode::INTERVAL,
          setShootPhotoModeForIntervalShootCb, uData);
    }
  } else {
    DERROR("Set camera mode error. Error code : %d", retCode);
    if (uData->userCallBack) {
      void (*cb)(ErrorCode::ErrCodeType, UserData);
      cb = (void (*)(ErrorCode::ErrCodeType, UserData))uData->userCallBack;
      /*! @TODO Temperarily using ret_code as errorcode, should be turned to be
       * a standrad one in the future */
      cb(retCode, uData->userData);
    }
  }
}

void CameraManagerAsyncSample::startShootIntervalPhotoAsyncSample(
    PayloadIndexType index, CameraModule::PhotoIntervalData intervalData,
    void (*UserCallBack)(ErrorCode::ErrCodeType retCode, UserData userData),
    UserData userData) {
  if (!vehicle || !vehicle->cameraManager) {
    DERROR("vehicle or cameraManager is a null value.");
    if (UserCallBack)
      UserCallBack(ErrorCode::UnifiedErrCode::kErrorInvalidParam, userData);
    return;
  }
  CameraManager *pm = vehicle->cameraManager;
  static AsyncSampleData uData;
  uData.index = index;
  uData.pm = pm;
  uData.userCallBack = (void *)UserCallBack;
  uData.userData = userData;
  memcpy(uData.dataTarget, &intervalData, sizeof(intervalData));

  /*!< set camera work mode as shoot photo */
  DSTATUS("set camera work mode as SHOOT_PHOTO");
  pm->setModeAsync(index, CameraModule::WorkMode::SHOOT_PHOTO,
                   setCameraModeForIntervalShootCb, &uData);
}

void CameraManagerAsyncSample::stopShootPhotoAsyncSample(
    PayloadIndexType index,
    void (*UserCallBack)(ErrorCode::ErrCodeType retCode, UserData userData),
    UserData userData) {
  if (!vehicle || !vehicle->cameraManager) {
    DERROR("vehicle or cameraManager is a null value.");
    if (UserCallBack)
      UserCallBack(ErrorCode::UnifiedErrCode::kErrorInvalidParam, userData);
    return;
  }
  CameraManager *pm = vehicle->cameraManager;

  /*!< stop camera shooting photo */
  DSTATUS("stop shooting photo");
  pm->stopShootPhotoAsync(index, UserCallBack, userData);
}
