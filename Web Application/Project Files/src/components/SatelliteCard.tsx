import React, { useState, useEffect } from "react";
import CardContent from "@mui/material/CardContent";
import Typography from "@mui/material/Typography";
import Paper from "@mui/material/Paper";
import Stack from "@mui/material/Stack";
import { useRotator } from "../classes/RotatorContext";
import { getSatData } from "../utils/Helper.jsx";
import { propagate } from "satellite.js";

const getSatSpeed = (satRec) => {
  const currentTime = new Date(); // Replace with the desired time
  const velocity = propagate(satRec, currentTime).velocity;
  const speed = Math.sqrt(
    velocity.x * velocity.x + velocity.y * velocity.y + velocity.z * velocity.z,
  );
  return Math.floor(speed);
};

export default function BasicCard() {
  const { rotator, selectedSatellite } = useRotator();
  const satRec = selectedSatellite.satRec;
  var now = new Date();
  var hour = now.getUTCHours();
  var minute = now.getUTCMinutes();
  var second = now.getUTCSeconds();
  
  const initialUTCTime = `${String(hour).padStart(2, "0")}:${String(
    minute,
  ).padStart(2, "0")}:${String(second).padStart(2, "0")}`;

  let initialLocalTime = "";
  if (rotator) {
    const localHourOffset = rotator.Rotator.UTCoffset;
    var localHour = (hour + localHourOffset) % 24;
    if (localHour < 0) {
      localHour = 24 + localHour;
    }
    initialLocalTime = `${String(localHour).padStart(2, "0")}:${String(
    minute,
  ).padStart(2, "0")}:${String(second).padStart(2, "0")}`;
  }

  const [utcTime, setUtcTime] = useState(initialUTCTime);
  const [localTime, setLocalTime] = useState(initialLocalTime);
  const [satLongitude, setSatLongitude] = useState(
    getSatData("longitude", satRec),
  );
  const [satLatitude, setSatLatitude] = useState(
    getSatData("latitude", satRec),
  );
  const [satHeight, setSatHeight] = useState(getSatData("height", satRec));
  const [satSpeed, setSatSpeed] = useState(getSatSpeed(satRec));
  useEffect(() => {
    // Function to update UTC and local time every x seconds
    const updateTimes = () => {
      const newUtcTime = incrementTime(utcTime);
      setUtcTime(newUtcTime);
      const newLocalTime = incrementTime(localTime);
      setLocalTime(newLocalTime);
      const newSatLongitude = getSatData("longitude", satRec);
      setSatLongitude(newSatLongitude);
      const newSatLatitude = getSatData("latitude", satRec);
      setSatLatitude(newSatLatitude);
      const newSatHeight = getSatData("height", satRec);
      setSatHeight(newSatHeight);
      const newSatSpeed = getSatSpeed(satRec);
      setSatSpeed(newSatSpeed);
    };

    const interval = setInterval(updateTimes, 1000); // Update every 1 second

    return () => clearInterval(interval); // Clean up the interval when the component unmounts
  }, [utcTime, localTime]);

  const incrementTime = (time) => {
    if (time == "" && !rotator) {
      return "";
    } else if (time == "") {
      let localOffsetHours = rotator.Rotator.UTCoffset;
      var now = new Date();
      var hour = now.getUTCHours();
      var minute = now.getUTCMinutes();
      var second = now.getUTCSeconds();
      let localHour = (hour + localOffsetHours) % 24;
      if (localHour < 0) {
      localHour = 24 + localHour;
      }
      let initialLocalTime = `${String(localHour).padStart(2, "0")}:${String(
    minute,
  ).padStart(2, "0")}:${String(second).padStart(2, "0")}`;;
      return initialLocalTime;
    }
    // Parse time (HH:MM:SS) and increment by 1 second
    const [hours, minutes, seconds] = time.split(":").map(Number);
    let newSeconds = seconds + 1;
    let newMinutes = minutes;
    let newHours = hours;
    if (newSeconds === 60) {
      newSeconds = 0;
      newMinutes += 1;
      if (newMinutes === 60) {
        newMinutes = 0;
        newHours += 1;
        if (newHours === 24) {
          newHours = 0;
        }
      }
    }
    return `${String(newHours).padStart(2, "0")}:${String(newMinutes).padStart(
      2,
      "0",
    )}:${String(newSeconds).padStart(2, "0")}`;
  };

  return (
<Paper
    //elevation={0}
    direction="row"
    variant="outlined"
    border={1}
    sx={{
      padding: "8px",
      height: "100%",
      minWidth: 180,
      width: "10%",
      justifyContent: "center",
      display: "flex",
      alignItems: "center",
      backgroundColor: "transparent",
      borderRadius: "4px",
    }}
  >
      <Stack
        direction="column"
        alignItems="space-between"
        justifyContent="space-between"
      >
        <Typography
          variant="h6"
          component="div"
          sx={{ fontFamily: "Roboto Mono, monospace" }}
        >
          {selectedSatellite.name}
        </Typography>
        <Stack
          direction="row"
          spacing={2}
          justifyContent="space-between"
          alignItems="space-between"
        >
          <Stack
            direction="column"
            justifyContent="space-between"
            alignItems="space-between"
          >
            <Typography
              variant="caption"
              component="div"
              sx={{ fontFamily: "Roboto Mono, monospace" }}
            >
              UTC
            </Typography>
            {localTime != "" && (
              <Typography
                variant="caption"
                component="div"
                sx={{ fontFamily: "Roboto Mono, monospace" }}
              >
                Local
              </Typography>
            )}
            <Typography
              variant="caption"
              component="div"
              sx={{ fontFamily: "Roboto Mono, monospace" }}
            >
              Longitude
            </Typography>
            <Typography
              variant="caption"
              component="div"
              sx={{ fontFamily: "Roboto Mono, monospace" }}
            >
              Latitude
            </Typography>
            <Typography
              variant="caption"
              component="div"
              sx={{ fontFamily: "Roboto Mono, monospace" }}
            >
              Height
            </Typography>
            <Typography
              variant="caption"
              component="div"
              sx={{ fontFamily: "Roboto Mono, monospace" }}
            >
              Speed
            </Typography>
          </Stack>

          <Stack direction="column">
            <Typography
              variant="caption"
              component="div"
              sx={{ color: "#8C92A4", fontFamily: "Roboto Mono, monospace" }}
            >
              {utcTime}
            </Typography>
            <Typography
              variant="caption"
              component="div"
              sx={{ color: "#8C92A4", fontFamily: "Roboto Mono, monospace" }}
            >
              {localTime !== null ? localTime : "Loading..."}
            </Typography>
            <Typography
              variant="caption"
              component="div"
              sx={{ color: "#8C92A4", fontFamily: "Roboto Mono, monospace" }}
            >
              {satLongitude}°
            </Typography>
            <Typography
              variant="caption"
              component="div"
              sx={{ color: "#8C92A4", fontFamily: "Roboto Mono, monospace" }}
            >
              {satLatitude}°
            </Typography>
            <Typography
              variant="caption"
              component="div"
              sx={{ color: "#8C92A4", fontFamily: "Roboto Mono, monospace" }}
            >
              {satHeight} km
            </Typography>
            <Typography
              variant="caption"
              component="div"
              sx={{ color: "#8C92A4", fontFamily: "Roboto Mono, monospace" }}
            >
              {satSpeed} km/s
            </Typography>
          </Stack>
        </Stack>
      </Stack>
      </Paper>

  );
}
