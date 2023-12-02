import CardContent from "@mui/material/CardContent";
import Typography from "@mui/material/Typography";
import Grid from "@mui/material/Unstable_Grid2";
import RotatorData from "../data/RotatorData.json";
import List from "@mui/material/List";
import ListItem from "@mui/material/ListItem";
import ListItemText from "@mui/material/ListItemText";
import Stack from "@mui/material/Stack";
import { useRotator } from "../classes/RotatorContext";
import Input from "@mui/material/Input";
import Scene from "./Scene.jsx";
import Button from "@mui/material/Button";
import Box from "@mui/material/Box";
import Paper from "@mui/material/Paper";
import Divider from "@mui/material/Divider";
import { useState, useEffect } from "react";
import AutoTrackingCard from "./AutoTrackingCard";
import ManualTrackingCard from "./ManualTrackingCard";
import IconButton from "@mui/material/IconButton";
import PowerOffSharpIcon from "@mui/icons-material/PowerOffSharp";
import RotatorModel from "./RotatorModel";
import StepInput from "./StepInput";
import { goToPosition } from "../utils/Helper.jsx";

const RadioListLabels = ["Heading", "Mode", "α-offset", "γ-offset"];

export default function RotatorCard() {
  const {
    updateHeading,
    updateMode,
    updateAzimuthOffset,
    updateElevationOffset,
    heading,
    mode,
    aOffset,
    yOffset,
    rotator,
    isAutoTracking,
    isManualTracking,
    updateIsManualTracking,
    updateIsAutoTracking,
    selectedSatellite,
    updateRotator,
  } = useRotator();
  const [RadioListButtons, setInputValues] = useState([
    heading,
    mode,
    aOffset,
    yOffset,
  ]);

  const [targetAzimuthText, setTargetAzimuthText] = useState(0);
  const [targetElevationText, setTargetElevationText] = useState(0);

  const handleChange = (index, newValue) => {
    const newInputValues = [...RadioListButtons];

    switch (index) {
      case 0:
        if(newValue >= -360 && newValue <= 360){
          updateHeading(newValue);
          newInputValues[index] = newValue;
          setInputValues(newInputValues);
        }
        break;
      case 2:
        if(newValue >= -360 && newValue <= 360){
          updateAzimuthOffset(newValue);
          newInputValues[index] = newValue;
          setInputValues(newInputValues);
        }
        
        break;
      case 3:
        if(newValue >= -90 && newValue <= 90){
          updateElevationOffset(newValue);
          newInputValues[index] = newValue;
          setInputValues(newInputValues);
        }
        
        break;
      default:
        break;
    }
  };

  function toggleMode() {
    // Assuming RadioListButtons is an array of strings
    const newInputValues = [...RadioListButtons];
    if (newInputValues[1] === "Auto") {
      newInputValues[1] = "Manual";
      updateMode("Manual");
    } else if (newInputValues[1] === "Manual") {
      newInputValues[1] = "Auto";
      updateMode("Auto");
    }
    setInputValues(newInputValues);
  }

  const startButtonHandler = () => {
    goToPosition(targetAzimuthText, targetElevationText);
    updateIsManualTracking(true);
  };

  const parkButtonHandler = () => {
    goToPosition(0,0);
    updateIsManualTracking(true);
    setTargetAzimuthText(0);
    setTargetElevationText(0);
    console.log(targetElevationText);
  };

  const trackButtonHandler = () => {
    updateIsAutoTracking(true);
  };

  const stopButtonHandler = () => {
    if (isAutoTracking) {
      updateIsAutoTracking(false);
    } else if (isManualTracking) {
      updateIsManualTracking(false);
    }
  };

  const disconnectHandler = () => {
    updateRotator(null);
  };
  return (
    <Paper
      elevation={0}
      direction="row"
      variant="outlined"
      border={1}
      sx={{
        width:"100%",
        padding: "8px",
        height: "100%",
        //marginTop: "16px",
        //alignItems="space-between",
        display: "flex",
        backgroundColor: "transparent",
        borderRadius: "4px",
      }}
    >
      <Stack
       sx={{width:"100%"}}
        direction="row"
        spacing={1}
        justifyContent="space-around"
        alignItems="space-around"
      >
        {/*Settings*/}

        <Stack
          direction="column"
          justifyContent="center"
          sx={{ minWidth: 180, maxWidth: 200 }}
        >
          <Stack direction="row" justifyContent="space-between">
            {/*Heading*/}
            <Stack direction="column" spacing={1.5} sx={{ width: "100%" }}>
              {RadioListLabels.map((label, index) => (
                <Typography
                  key={index}
                  variant="caption"
                  component="div"
                  sx={{
                    //fontSize: 14,
                    color: "white",
                    fontFamily: "Roboto Mono, monospace",
                  }}
                >
                  {label}
                </Typography>
              ))}
            </Stack>
            {/*Values*/}
            <Stack direction="column" spacing={1} justifyContent="fixed">
              {RadioListButtons.map((value, index) =>
                index === 1 ? (
                  <Button
                    disabled={isManualTracking}
                    sx={{
                      height: "25px",
                      fontFamily: "Roboto Mono, monospace",
                      backgroundColor: "#007BFF",
                      color: "white",
                      "&.MuiButtonBase-root": {
                        padding: "4px 4px 4px 4px",
                      },
                    }}
                    key={index}
                    onClick={() => toggleMode()}
                  >
                    {RadioListButtons[1]}
                  </Button>
                ) : (
                  <Stack
                    key={index}
                    direction="row"
                    sx={{
                      width: "80px",
                      backgroundColor: "#373C4B",
                      borderRadius: "4px",
                    }}
                  >
                    <Input
                      disabled={isManualTracking}
                      onChange={(e) => handleChange(index, e.target.value)}
                      value={value}
                      key={index}
                      type="number"
                      sx={{
                        fontFamily: "Roboto Mono, monospace",
                        color: "#8C92A4",
                        fontWeight: 10,
                        //width: "74.81px",
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
                          border: 0.5,
                          borderColor: "#373C4B",
                          position: "relative",
                          backgroundColor: "#373C4B",
                          fontSize: 10,
                          padding: "4px 0px 4px 10px",
                          //alignItems: "right",
                          transition: "background-color 0.3s", // Add a transition for smooth hover effect
                        },
                        "& .MuiInputBase-input:hover": {
                          border: 0.5,
                          borderColor: "#007BFF", // Change the background color on hover
                        },
                        "& .MuiInputBase-selected": {
                          border: 0.5,
                          borderColor: "#007BFF", // Change the background color on hover
                        },
                      }}
                    />
                  </Stack>
                ),
              )}
            </Stack>
          </Stack>
          <Button
            disabled={isManualTracking}
            sx={{
              marginTop: "8px",
              height: "20px",
              fontFamily: "Roboto Mono, monospace",
              backgroundColor: "#007BFF",
              color: "white",
              "&.MuiButtonBase-root": {
                padding: "4px 4px 4px 4px",
              },
            }}
            onClick={disconnectHandler}
          >
            disconnect
          </Button>
        </Stack>

        {selectedSatellite && (
          <Paper
            elevation={0}
            direction="row"
            //variant="outlined"
            //border={1}
            sx={{
              padding: "8px",
              width: 385,
              // marginLeft: "8px",
              justifyContent: "center",
              display: "flex",
              alignItems: "flex-start",
              backgroundColor: "transparent",
              borderRadius: "4px",
            }}
          >
            {/*Manual/Auto */}

            {mode === "Manual" ? (
              <ManualTrackingCard
                targetElevationText={targetElevationText}
                targetAzimuthText={targetAzimuthText}
                setTargetAzimuthText={setTargetAzimuthText}
                setTargetElevationText={setTargetElevationText}
              />
            ) : (
              <AutoTrackingCard />
            )}
          </Paper>
        )}

        {/*Control Buttons*/}
        <Stack
          direction="column"
          spacing={1}
          sx={{ minWidth: 70, paddingTop: "4px" }}
        >
          {!(isAutoTracking || isManualTracking) ? (
            mode === "Manual" ? (
              <Button
                sx={{
                  "&.MuiButtonBase-root": {
                    padding: "40px 4px 40px 5px",
                  },
                  color: "white",
                  backgroundColor: "#19B600",
                }}
                onClick={() => startButtonHandler()}
              >
                Go To{" "}
              </Button>
            ) : (
              <Button
                sx={{
                  "&.MuiButtonBase-root": {
                    padding: "40px 4px 40px 5px",
                  },
                  color: "white",
                  backgroundColor: "#19B600",
                }}
                onClick={() => trackButtonHandler()}
              >
                Track
              </Button>
            )
          ) : (
            <Button
              sx={{
                "&.MuiButtonBase-root": {
                  padding: "40px 4px 40px 5px",
                },
                color: "white",
                backgroundColor: "red",
              }}
              onClick={() => stopButtonHandler()}
            >
              Stop
            </Button>
          )}

          <Button onClick={() => parkButtonHandler()} disabled={isManualTracking} sx={{ color: "white", backgroundColor: "purple" }}>
            Park
          </Button>
        </Stack>

        <RotatorModel targetAzimuthText={targetAzimuthText} targetElevationText={targetElevationText}/>
      </Stack>
    </Paper>
  );
}
