// Copyright (c) 2021 Juan Miguel Jimeno
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef DEFAULT_IMU
#define DEFAULT_IMU

// include IMU base interface
#include "imu_interface.h"

// include sensor API headers
#include "I2Cdev.h"
#include "ADXL345.h"
#include "ITG3200.h"
#include "HMC5883L.h"
#include "MPU6050.h"
#include "MPU9250.h"
#include "QMI8658.h"
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>

class GY85IMU : public IMUInterface
{
private:
    // constants specific to the sensor
    const float accel_scale_ = 1 / 256.0;
    const float gyro_scale_ = 1 / 14.375;

    // driver objects to be used
    ADXL345 accelerometer_;
    ITG3200 gyroscope_;

    // returned vector for sensor reading
    geometry_msgs__msg__Vector3 accel_;
    geometry_msgs__msg__Vector3 gyro_;

public:
    GY85IMU()
    {
        // accel_cov_ = 0.001; //you can overwrite the convariance values here
        // gyro_cov_ = 0.001; //you can overwrite the convariance values here
    }

    bool startSensor() override
    {
        // here you can override startSensor() function and use the sensor's driver API
        // to initialize and test the sensor's connection during boot time
        bool ret;
        accelerometer_.initialize();
        ret = accelerometer_.testConnection();
        if (!ret)
            return false;

        gyroscope_.initialize();
        ret = gyroscope_.testConnection();
        if (!ret)
            return false;

        return true;
    }

    geometry_msgs__msg__Vector3 readAccelerometer() override
    {
        // here you can override readAccelerometer function and use the sensor's driver API
        // to grab the data from accelerometer and return as a Vector3 object
        int16_t ax, ay, az;

        accelerometer_.getAcceleration(&ax, &ay, &az);

        accel_.x = ax * (double)accel_scale_ * g_to_accel_;
        accel_.y = ay * (double)accel_scale_ * g_to_accel_;
        accel_.z = az * (double)accel_scale_ * g_to_accel_;

        return accel_;
    }

    geometry_msgs__msg__Vector3 readGyroscope() override
    {
        // here you can override readAccelerometer function and use the sensor's driver API
        // to grab the data from gyroscope and return as a Vector3 object
        int16_t gx, gy, gz;

        gyroscope_.getRotation(&gx, &gy, &gz);

        gyro_.x = gx * (double)gyro_scale_ * DEG_TO_RAD;
        gyro_.y = gy * (double)gyro_scale_ * DEG_TO_RAD;
        gyro_.z = gz * (double)gyro_scale_ * DEG_TO_RAD;

        return gyro_;
    }
};

class MPU6050IMU : public IMUInterface
{
private:
    const float accel_scale_ = 1 / 16384.0;
    const float gyro_scale_ = 1 / 131.0;

    MPU6050 accelgyro_;

    geometry_msgs__msg__Vector3 accel_;
    geometry_msgs__msg__Vector3 gyro_;

public:
    MPU6050IMU()
    {
    }

    bool startSensor() override
    {
        bool ret;
        accelgyro_.initialize();
        ret = accelgyro_.testConnection();
        if (!ret)
            return false;

        accelgyro_.CalibrateAccel();
        accelgyro_.CalibrateGyro();
        return true;
    }

    geometry_msgs__msg__Vector3 readAccelerometer() override
    {
        int16_t ax, ay, az;

        accelgyro_.getAcceleration(&ax, &ay, &az);

        accel_.x = ax * (double)accel_scale_ * g_to_accel_;
        accel_.y = ay * (double)accel_scale_ * g_to_accel_;
        accel_.z = az * (double)accel_scale_ * g_to_accel_;

        return accel_;
    }

    geometry_msgs__msg__Vector3 readGyroscope() override
    {
        int16_t gx, gy, gz;

        accelgyro_.getRotation(&gx, &gy, &gz);

        gyro_.x = gx * (double)gyro_scale_ * DEG_TO_RAD;
        gyro_.y = gy * (double)gyro_scale_ * DEG_TO_RAD;
        gyro_.z = gz * (double)gyro_scale_ * DEG_TO_RAD;

        return gyro_;
    }
};

class MPU9250IMU : public IMUInterface
{
private:
    const float accel_scale_ = 1 / 16384.0;
    const float gyro_scale_ = 1 / 131.0;

    MPU9250 accelgyro_;

    geometry_msgs__msg__Vector3 accel_;
    geometry_msgs__msg__Vector3 gyro_;

public:
    MPU9250IMU()
    {
    }

    bool startSensor() override
    {
        bool ret;
        accelgyro_.initialize();
        ret = accelgyro_.testConnection();
        if (!ret)
            return false;

        return true;
    }

    geometry_msgs__msg__Vector3 readAccelerometer() override
    {
        int16_t ax, ay, az;

        accelgyro_.getAcceleration(&ax, &ay, &az);

        accel_.x = ax * (double)accel_scale_ * g_to_accel_;
        accel_.y = ay * (double)accel_scale_ * g_to_accel_;
        accel_.z = az * (double)accel_scale_ * g_to_accel_;

        return accel_;
    }

    geometry_msgs__msg__Vector3 readGyroscope() override
    {
        int16_t gx, gy, gz;

        accelgyro_.getRotation(&gx, &gy, &gz);

        gyro_.x = gx * (double)gyro_scale_ * DEG_TO_RAD;
        gyro_.y = gy * (double)gyro_scale_ * DEG_TO_RAD;
        gyro_.z = gz * (double)gyro_scale_ * DEG_TO_RAD;

        return gyro_;
    }
};

class FakeIMU : public IMUInterface
{
private:
    geometry_msgs__msg__Vector3 accel_;
    geometry_msgs__msg__Vector3 gyro_;

public:
    FakeIMU()
    {
    }

    bool startSensor() override
    {
        return true;
    }

    geometry_msgs__msg__Vector3 readAccelerometer() override
    {
        accel_.z = 9.8;
        return accel_;
    }

    geometry_msgs__msg__Vector3 readGyroscope() override
    {
        return gyro_;
    }
};

class QMI8658IMU : public IMUInterface
{
private:
    QMI8658 qmi8658_;

    geometry_msgs__msg__Vector3 accel_;
    geometry_msgs__msg__Vector3 gyro_;

public:
    QMI8658IMU()
    {
    }

    bool startSensor() override
    {
        if (qmi8658_.begin() == 0)
        {
            // Serial.println("qmi8658_init fail");
            return false;
        }
        return true;
    }

    geometry_msgs__msg__Vector3 readAccelerometer() override
    {
        float ac[3];
        qmi8658_.read_acc(ac);
        accel_.x = ac[0];
        accel_.y = ac[1];
        accel_.z = ac[2];
        return accel_;
    }

    geometry_msgs__msg__Vector3 readGyroscope() override
    {
        float gy[3];
        qmi8658_.read_gyro(gy);
        gyro_.x = gy[0];
        gyro_.y = gy[1];
        gyro_.z = gy[2];
        return gyro_;
    }
};

class BNO055IMU : public IMUInterface
{
private:
    Adafruit_BNO055 bno = Adafruit_BNO055(55, 0x28);
    geometry_msgs__msg__Vector3 accel_data;
    geometry_msgs__msg__Vector3 gyro_data;
    geometry_msgs__msg__Quaternion orientation_data;

public:
    BNO055IMU()
    {
    }

    bool startSensor() override
    {
        if (!bno.begin())
        {
            return false;
        }
        bno.setExtCrystalUse(true);
        bno.setMode(adafruit_bno055_opmode_t::OPERATION_MODE_NDOF);
        bno.setAxisRemap(Adafruit_BNO055::adafruit_bno055_axis_remap_config_t::REMAP_CONFIG_P0);
        bno.setAxisSign(Adafruit_BNO055::adafruit_bno055_axis_remap_sign_t::REMAP_SIGN_P1);
        return true;
    }
    geometry_msgs__msg__Vector3 readAccelerometer() override
    {
        sensors_event_t event;
        bno.getEvent(&event, Adafruit_BNO055::VECTOR_LINEARACCEL);

        accel_data.x = event.acceleration.x;
        accel_data.y = event.acceleration.y;
        accel_data.z = event.acceleration.z;

        return accel_data;
    }

    geometry_msgs__msg__Vector3 readGyroscope() override
    {
        sensors_event_t event;
        bno.getEvent(&event, Adafruit_BNO055::VECTOR_GYROSCOPE);
        gyro_data.x = event.gyro.x * DEG_TO_RAD;
        gyro_data.y = event.gyro.y * DEG_TO_RAD;
        gyro_data.z = event.gyro.z * DEG_TO_RAD;
        return gyro_data;
    }

    geometry_msgs__msg__Quaternion readOrientation() override
    {
        sensors_event_t event;
        imu::Quaternion quat = bno.getQuat();

        orientation_data.w = quat.w();
        orientation_data.x = quat.x();
        orientation_data.y = quat.y();
        orientation_data.z = quat.z();
        return orientation_data;
    }

    sensor_msgs__msg__Imu getData()
    {
        imu_msg_.angular_velocity = readGyroscope();
        imu_msg_.angular_velocity_covariance[0] = gyro_cov[0];
        imu_msg_.angular_velocity_covariance[4] = gyro_cov[1];
        imu_msg_.angular_velocity_covariance[8] = gyro_cov[2];

        imu_msg_.linear_acceleration = readAccelerometer();
        imu_msg_.linear_acceleration_covariance[0] = accel_cov[0];
        imu_msg_.linear_acceleration_covariance[4] = accel_cov[1];
        imu_msg_.linear_acceleration_covariance[8] = accel_cov[2];

        imu_msg_.orientation = readOrientation();
        imu_msg_.orientation_covariance[0] = ori_cov[0];
        imu_msg_.orientation_covariance[4] = ori_cov[1];
        imu_msg_.orientation_covariance[8] = ori_cov[2];

        return imu_msg_;
    }
};

#endif
// ADXL345 https://www.sparkfun.com/datasheets/Sensors/Accelerometer/ADXL345.pdf
// HMC8553L https://cdn-shop.adafruit.com/datasheets/HMC5883L_3-Axis_Digital_Compass_IC.pdf
// ITG320 https://www.sparkfun.com/datasheets/Sensors/Gyro/PS-ITG-3200-00-01.4.pdf

// MPU9150 https://www.invensense.com/wp-content/uploads/2015/02/PS-MPU-9250A-01-v1.1.pdf
// MPU9250 https://www.invensense.com/wp-content/uploads/2015/02/MPU-9150-Datasheet.pdf
// MPU6050 https://store.invensense.com/datasheets/invensense/MPU-6050_DataSheet_V3%204.pdf

// http://www.sureshjoshi.com/embedded/invensense-imus-what-to-know/
// https://stackoverflow.com/questions/19161872/meaning-of-lsb-unit-and-unit-lsb
