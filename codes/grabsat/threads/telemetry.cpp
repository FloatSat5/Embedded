#include "topics.h"
#include "magnet.h"
#include "telemetry.h"
#include "satellite.h"
#include "multimeter.h"
#include "telecommand.h"
#include "satellite_config.h"

// Subscriber stuffs
CommBuffer<telemetry_struct> telemetry_buffer;
Subscriber telemetry_subscriber(telemetry_topic, telemetry_buffer);
telemetry_struct telemetry_rx;

void TelemetryThread::init()
{
}

#define D2R 0.01745329251f
#define R2D 57.2957795131f

void TelemetryThread::run()
{
  while (1)
  {
    int len = 0;
    telemetry_buffer.get(telemetry_rx);

#if (WHO_IS_USER == 2)
    // Pack telemetry data to GrabSat-I format
    len = SNPRINTF(msg, sizeof(msg), "motav,%f;angve,%f,%f,%f;angpo,%f,%f,%f;magst,%d;batvo,%f;\n",
                   telemetry_rx.w,
                   telemetry_rx.g[0],
                   telemetry_rx.g[1],
                   telemetry_rx.g[2],
                   telemetry_rx.ypr[0] * R2D,
                   telemetry_rx.ypr[1] * R2D,
                   telemetry_rx.ypr[2] * R2D,
                   magnet::state,
                   multimeter::get_voltage());

    bluetooth.write(msg, len + 1);
#endif

#if (WHO_IS_USER == 1)
    bool is_transmit = true;

    if (current_mode == omega)
    {
      len = SNPRINTF(msg, sizeof(msg), "%f | %f | %f | %f | %f | %f\n",
                     telecommands[sangv].value,  // set-point
                     telemetry_rx.g[2],          // feedback
                     telemetry_rx.w,             // wheel omega
                     telecommands[gkpsw].value,  // kp
                     telecommands[gkisw].value,  // ki
                     multimeter::get_voltage()); // voltage
    }
    else if (current_mode == motor)
    {
      len = SNPRINTF(msg, sizeof(msg), "%f | %f | %f | %f\n",
                     telecommands[mosav].value,  // set-point
                     telemetry_rx.w,             // feedback
                     telecommands[gkpmw].value,  // kp
                     telecommands[gkimw].value); // ki
    }
    else if (current_mode == yaw)
    {
      len = SNPRINTF(msg, sizeof(msg), "%f | %f | %f |  %f | %f | %f | %f\r\n",
                     telecommands[sangp].value,  // set-point
                     telemetry_rx.ypr[0] * R2D,  // feedback
                     telemetry_rx.ypr[1],        // error
                     telemetry_rx.w,             // motor-omega
                     telecommands[gkpsa].value,  // kp
                     telecommands[gkisa].value,  // ki
                     telecommands[gkdsa].value); // kd
    }
    else
    {
      is_transmit = false;
    }

    if (is_transmit)
    {
      bluetooth.write(msg, len);
      is_transmit = false;
    }
#endif

    suspendCallerUntil(NOW() + PERIOD_TELEMETRY_CONTROL * MILLISECONDS);
  }
}
