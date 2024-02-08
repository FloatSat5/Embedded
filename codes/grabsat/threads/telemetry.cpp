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
    bool is_transmit = true;
    telemetry_buffer.get(telemetry_rx);

    /*
      // Pack telemetry data to GrabSat-I format
      len = SNPRINTF(msg, sizeof(msg), "motav,%f,angve,%f,%f,%f,angpo,%f,%f,%f,magst,%d,batvo,%f\n",
              telemetry_rx.w,
              telemetry_rx.g[0],
              telemetry_rx.g[1],
              telemetry_rx.g[2],
              11.0,
              22.0,
              33.0,
              magnet::state,
              multimeter::get_voltage());

      bluetooth.write(msg, len+1);
    */

    if (current_mode == omega)
    {
      len = SNPRINTF(msg, sizeof(msg), "%f | %f | %f | %f | %f | %f\n",
                     telecommands[sangv].value,
                     telemetry_rx.g[2],
                     telemetry_rx.w,
                     telecommands[gkpsw].value,
                     telecommands[gkisw].value,
                     multimeter::get_voltage());
    }
    else if (current_mode == motor)
    {
      len = SNPRINTF(msg, sizeof(msg), "%f | %f\n", telecommands[mosav].value, telemetry_rx.w);
    }
    else if (current_mode == yaw)
    {
      len = SNPRINTF(msg, sizeof(msg), "%f | %f |  %f | %f | %f\n", telecommands[sangp].value, telemetry_rx.ypr[0] * R2D, telemetry_rx.w, telecommands[gkpsa].value, telecommands[gkisa].value);
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

    suspendCallerUntil(NOW() + PERIOD_TELEMETRY_CONTROL * MILLISECONDS);
  }
}
