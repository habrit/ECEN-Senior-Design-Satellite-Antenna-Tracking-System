import React, { useState } from "react";
import Card from "@mui/material/Card";
import Stack from "@mui/material/Stack";
import Button from "@mui/material/Button";
import TextField from "@mui/material/TextField";
import CardContent from "@mui/material/CardContent";
import Typography from "@mui/material/Typography";
import Input from "@mui/material/Input";
import Paper from "@mui/material/Paper";
import ContentCopyIcon from "@mui/icons-material/ContentCopy";
import ArrowDropUpOutlinedIcon from "@mui/icons-material/ArrowDropUpOutlined";
import ArrowDropDownOutlinedIcon from "@mui/icons-material/ArrowDropDownOutlined";
import IconButton from "@mui/material/IconButton";
import { useRotator } from "../classes/RotatorContext";

function ManualTrackingCard({
  setTargetAzimuthText,
  setTargetElevationText,
  targetAzimuthText,
  targetElevationText,
}) {
  const { azimuth, elevation, isManualTracking } = useRotator();
  const AzimuthPresetButtons = [0, 45, 90, 135, 180, 225, 270, 325, 360];
  const ElevationPresetButtons = [0, 15, 45, 60, 90, -15, -45, -60, -90];
  const [targetAzimuthStep, setTargetAzimuthStep] = useState(1);
  const [targetElevationStep, setTargetElevationStep] = useState(1);

  const handleTargetAzimuth = (inputValue) => {
    if (inputValue === "copy") {
      setTargetAzimuthText(azimuth);
    } else {
      if (inputValue <= 360 && inputValue >= 0) {
        setTargetAzimuthText(inputValue);
      } else {
        console.log("invalid target azimuth");
      }
    }
  };

  const handleTargetElevation = (inputValue) => {
    if (inputValue === "copy") {
      setTargetElevationText(elevation);
    } else {
      if (inputValue <= 90 && inputValue >= -90) {
        setTargetElevationText(inputValue);
      } else {
        console.log("invalid target elevation");
      }
    }
  };

  const handleTargetAzimuthStep = (inputValue) => {
    if (inputValue > 0 && inputValue <= 360) {
      setTargetAzimuthStep(inputValue);
    } else {
      console.log("invalid target azimuth step size");
    }
  };

  const handleTargetElevationStep = (inputValue) => {
    if (inputValue > 0 && inputValue <= 90) {
      setTargetElevationStep(inputValue);
    } else {
      console.log("invalid target elevation step size");
    }
  };

  return (
    <>
      {/*Azimuth*/}
      <Stack direction="column">
        <Typography
          variant="h6"
          component="div"
          sx={{
            fontFamily: "Roboto Mono, monospace",
            fontSize: 16,
            fontWeight: "bold",
            paddingRight: "16px",
            lineHeight: "1.3rem",
          }}
        >
          Set Target
        </Typography>
        <Stack direction="row">
          <Stack direction="column">
            <Typography
              variant="caption"
              component="div"
              sx={{
                fontFamily: "Roboto Mono, monospace",
                fontSize: 12,
                paddingRight: "16px",
              }}
            >
              Azimuth
            </Typography>
            <Input
              disabled={isManualTracking}
              onChange={(e) => handleTargetAzimuth(e.target.value)}
              value={targetAzimuthText}
              type="number"
              inputProps={{
                step: targetAzimuthStep, // Adjust the step here
              }}
              sx={{
                fontFamily: "Roboto Mono, monospace",
                color: "white",
                fontWeight: 400,
                width: "85px",
                borderBottom: 0,
                "&&&:before": {
                  borderBottom: "none",
                },
                "&&:after": {
                  borderBottom: "none",
                },
                "& .MuiInputBase-input": {
                  textAlign: "center",
                  borderRadius: 1,
                  position: "relative",
                  backgroundColor: "#373C4B",
                  fontSize: 15,
                  padding: "1px 0px 1px 10px",
                  //alignItems: "right",
                  transition: "background-color 0.3s", // Add a transition for smooth hover effect
                },
                "& .MuiInputBase-input:hover": {
                  backgroundColor: "#242730"// Change the background color on hover
                },
                "& .MuiInputBase-selected": {
                  border: 0.5,
                  borderColor: "#007BFF", // Change the background color on hover
                },
              }}
            />
          </Stack>
          <Stack direction="column" sx={{ paddingLeft: "8px" }}>
            <Typography
              variant="caption"
              component="div"
              sx={{
                fontFamily: "Roboto Mono, monospace",
                fontSize: 12,
                paddingRight: "16px",
              }}
            >
              Step
            </Typography>
            <Input
              disabled={isManualTracking}
              onChange={(e) => handleTargetAzimuthStep(e.target.value)}
              value={targetAzimuthStep}
              //key={index}
              type="number"
              sx={{
                fontFamily: "Roboto Mono, monospace",
                color: "white",
                fontWeight: 10,
                width: "85px",
                borderBottom: 0,
                "&&&:before": {
                  borderBottom: "none",
                },
                "&&:after": {
                  borderBottom: "none",
                },
                "& .MuiInputBase-input": {
                  textAlign: "center",
                  borderRadius: 1,
                  position: "relative",
                  backgroundColor: "#373C4B",
                  fontSize: 15,
                  padding: "1px 0px 1px 10px",
                  //alignItems: "right",
                  transition: "background-color 0.3s", // Add a transition for smooth hover effect
                },
                "& .MuiInputBase-input:hover": {
                  backgroundColor: "#242730" // Change the background color on hover
                },
                "& .MuiInputBase-selected": {
                  border: 0.5,
                  borderColor: "#007BFF", // Change the background color on hover
                },
              }}
            />
          </Stack>
        </Stack>
        <Typography
          variant="caption"
          component="div"
          sx={{
            fontFamily: "Roboto Mono, monospace",
            fontSize: 12,
            paddingRight: "16px",
            marginTop: "4px",
            marginBottom: "4px",
          }}
        >
          Presets
        </Typography>
        <Stack direction="row">
          {/*Preset Buttons*/}
          {AzimuthPresetButtons.slice(
            0,
            Math.ceil(AzimuthPresetButtons.length / 2),
          ).map((azimuth) => (
            <Button
              disabled={isManualTracking}
              sx={{
                height: "24px",
                fontFamily: "Roboto Mono, monospace",
                backgroundColor: "#373C4B",
                color: "#8C92A4",
                "&.MuiButtonBase-root": {
                  p: "4px",
                  m: "0px 4px 4px 0px",
                  minWidth: "32px",
                  width: "32px", // Adjust the width as needed
                },
              }}
              key={azimuth}
              onClick={() => handleTargetAzimuth(azimuth)}
            >
              {azimuth}
            </Button>
          ))}
        </Stack>
        <Stack direction="row">
          {/*Preset Buttons*/}
          {AzimuthPresetButtons.slice(
            Math.ceil(AzimuthPresetButtons.length / 2),
          ).map((azimuth) => (
            <Button
              disabled={isManualTracking}
              sx={{
                height: "24px",
                fontFamily: "Roboto Mono, monospace",
                backgroundColor: "#373C4B",
                color: "#8C92A4",
                "&.MuiButtonBase-root": {
                  p: "4px",
                  m: "0px 4px 4px 0px",
                  minWidth: "32px",
                  width: "32px", // Adjust the width as needed
                },
              }}
              key={azimuth}
              onClick={() => handleTargetAzimuth(azimuth)}
            >
              {azimuth}
            </Button>
          ))}
          <Button
            disabled={isManualTracking}
            sx={{
              height: "24px",
              fontFamily: "Roboto Mono, monospace",
              backgroundColor: "#373C4B",
              color: "#8C92A4",
              "&.MuiButtonBase-root": {
                p: "4px",
                m: "0px 4px 4px 0px",
                minWidth: "32px",
                width: "32px", // Adjust the width as needed
              },
            }}
            onClick={() => handleTargetAzimuth("copy")}
          >
            <ContentCopyIcon sx={{ height: "14px", width: "14px" }} />
          </Button>
        </Stack>
      </Stack>

      {/*Elevation*/}
      <Stack direction="column" sx={{ paddingLeft: "8px" }}>
        <Typography
          variant="h6"
          component="div"
          sx={{
            fontFamily: "Roboto Mono, monospace",
            fontSize: 16,

            paddingRight: "16px",
            lineHeight: "1.3rem",
            color: "transparent",
          }}
        >
          o
        </Typography>
        <Stack direction="row">
          <Stack direction="column">
            <Typography
              variant="caption"
              component="div"
              sx={{
                fontFamily: "Roboto Mono, monospace",
                fontSize: 12,
                paddingRight: "16px",
              }}
            >
              Elevation
            </Typography>
            <Input
              disabled={isManualTracking}
              onChange={(e) => handleTargetElevation(e.target.value)}
              value={targetElevationText}
              type="number"
              inputProps={{
                step: targetElevationStep, // Adjust the step here
              }}
              //onChange={(e) => handleChange(index, e.target.value)}
              //value={value}
              //key={index}
              type="number"
              sx={{
                fontFamily: "Roboto Mono, monospace",
                color: "white",
                fontWeight: 10,
                width: "85px",
                borderBottom: 0,
                "&&&:before": {
                  borderBottom: "none",
                },
                "&&:after": {
                  borderBottom: "none",
                },
                "& .MuiInputBase-input": {
                  textAlign: "center",
                  borderRadius: 1,
                  position: "relative",
                  backgroundColor: "#373C4B",
                  fontSize: 15,
                  padding: "1px 0px 1px 10px",
                  //alignItems: "right",
                  transition: "background-color 0.3s", // Add a transition for smooth hover effect
                },
                "& .MuiInputBase-input:hover": {
                  backgroundColor: "#242730" // Change the background color on hover
                },
                "& .MuiInputBase-selected": {
                  border: 0.5,
                  borderColor: "#007BFF", // Change the background color on hover
                },
              }}
            />
          </Stack>
          <Stack direction="column" sx={{ paddingLeft: "8px" }}>
            <Typography
              variant="caption"
              component="div"
              sx={{
                fontFamily: "Roboto Mono, monospace",
                fontSize: 12,
                paddingRight: "16px",
              }}
            >
              Step
            </Typography>
            <Input
              disabled={isManualTracking}
              onChange={(e) => handleTargetElevationStep(e.target.value)}
              value={targetElevationStep}
              //onChange={(e) => handleChange(index, e.target.value)}
              //value={value}
              //key={index}
              type="number"
              sx={{
                fontFamily: "Roboto Mono, monospace",
                color: "white",
                fontWeight: 10,
                width: "85px",
                borderBottom: 0,
                "&&&:before": {
                  borderBottom: "none",
                },
                "&&:after": {
                  borderBottom: "none",
                },
                "& .MuiInputBase-input": {
                  textAlign: "center",
                  borderRadius: 1,
                  position: "relative",
                  backgroundColor: "#373C4B",
                  fontSize: 15,
                  padding: "1px 0px 1px 10px",
                  //alignItems: "right",
                  transition: "background-color 0.3s", // Add a transition for smooth hover effect
                },
                "& .MuiInputBase-input:hover": {
                  backgroundColor: "#242730"
                  // Change the background color on hover
                },
                "& .MuiInputBase-selected": {
                  border: 0.5,
                  borderColor: "#007BFF", // Change the background color on hover
                },
              }}
            />
          </Stack>
        </Stack>
        <Typography
          variant="caption"
          component="div"
          sx={{
            fontFamily: "Roboto Mono, monospace",
            fontSize: 12,
            paddingRight: "16px",
            marginTop: "4px",
            marginBottom: "4px",
          }}
        >
          Presets
        </Typography>
        <Stack direction="row">
          {/*Preset Buttons*/}
          {ElevationPresetButtons.slice(
            0,
            Math.ceil(ElevationPresetButtons.length / 2),
          ).map((elevation) => (
            <Button
            disabled={isManualTracking}
              sx={{
                height: "24px",
                fontFamily: "Roboto Mono, monospace",
                backgroundColor: "#373C4B",
                color: "#8C92A4",
                "&.MuiButtonBase-root": {
                  p: "4px",
                  m: "0px 4px 4px 0px",
                  minWidth: "32px",
                  width: "32px", // Adjust the width as needed
                },
              }}
              key={elevation}
              onClick={() => handleTargetElevation(elevation)}
            >
              {elevation}
            </Button>
          ))}
        </Stack>
        <Stack direction="row">
          {/*Preset Buttons*/}
          {ElevationPresetButtons.slice(
            Math.ceil(ElevationPresetButtons.length / 2),
          ).map((elevation) => (
            <Button
            disabled={isManualTracking}
              sx={{
                height: "24px",
                fontFamily: "Roboto Mono, monospace",
                backgroundColor: "#373C4B",
                color: "#8C92A4",
                "&.MuiButtonBase-root": {
                  p: "4px",
                  m: "0px 4px 4px 0px",
                  minWidth: "32px",
                  width: "32px", // Adjust the width as needed
                },
              }}
              key={elevation}
              onClick={() => handleTargetElevation(elevation)}
            >
              {elevation}
            </Button>
          ))}
          <Button
          disabled={isManualTracking}
            sx={{
              height: "24px",
              fontFamily: "Roboto Mono, monospace",
              backgroundColor: "#363D44",
              color: "#8C92A4",
              "&.MuiButtonBase-root": {
                p: "4px",
                m: "0px 4px 4px 0px",
                minWidth: "32px",
                width: "32px", // Adjust the width as needed
              },
            }}
            onClick={() => handleTargetElevation("copy")}
          >
            <ContentCopyIcon sx={{ height: "14px", width: "14px" }} />
          </Button>
        </Stack>
      </Stack>
    </>
  );
}

export default ManualTrackingCard;
