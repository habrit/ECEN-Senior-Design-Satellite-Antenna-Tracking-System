import React, { useState } from "react";
import Alert from "@material-ui/lab/Alert";
import {
  Button,
  Menu,
  MenuItem,
  FormControlLabel,
  Switch,
  TextField,
  Snackbar
} from "@material-ui/core";
import { fetchJson } from "./Helper";
import { Tune as MenuIcon } from "@material-ui/icons";
import { Stop as StopIcon } from "@material-ui/icons";

function DropdownMenu({ Satellite, Rotator }) {
  const [anchorEl, setAnchorEl] = useState(null);
  const [isAutoTrackOn, setIsAutoTrackOn] = useState(false);
  const [azimuth, setAzimuth] = useState("");
  const [elevation, setElevation] = useState("");
  const [isTracking, setIsTracking] = useState(false);
  const [azimuthError, setAzimuthError] = useState("");
  const [elevationError, setElevationError] = useState("");
  const [Snack, setSnack] = useState(null);

  const handleToggleMenu = (event) => {
    if (Satellite !== null && Rotator !== null) {
      // only open control menu when satellite and rotator are chosen
      setAnchorEl(event.currentTarget);
    } else {
      // display error message
      setAnchorEl(event.currentTarget);
      setSnack({
        message:
          "Please select a satellite and connect a rotator to begin tracking",
        variant: "warning"
      });
    }
  };

  const handleCloseMenu = (event) => {
    if (Boolean(azimuthError) || Boolean(elevationError)) {
      // clear input fields
      setAzimuth("");
      setElevation("");
      setAzimuthError("");
      setElevationError("");
    }
    //close control menu
    setAnchorEl(null);
  };

  const handleCommand = async () => {
    if (isAutoTrackOn || (azimuth !== "" && elevation !== ""))
      if (isTracking) {
        // send stop tracking command to mcu
        const response = await fetchJson(Rotator.IP, "Stop");

        if (typeof response === "object") {
          console.log(response.message);
          // notify user with success response
          setSnack({ message: response.message, variant: "success" });
          setIsTracking(false);
          Rotator.isTracking = false;
        } else {
          // // notify user with error response
          setSnack({ message: response.message, variant: "error" });
        }
      } else {
        var response = "";
        setAnchorEl(null);
        if (isAutoTrackOn) {
          // serialize satellite object
          const auto_string = JSON.stringify(Satellite.Omm);
          // send auto-tracking command
          response = await fetchJson(Rotator.IP, "StartAuto", auto_string);
        } else {
          // serialize azimuth/elevation
          const manual_string = azimuth + " " + elevation;
          // send manual tracking command
          response = await fetchJson(Rotator.IP, "StartManual", manual_string);
        }
        if (typeof response === "object") {
          console.log(response.message);
          // notify user with success response
          setSnack({ message: response.message, variant: "success" });
          setIsTracking(true);
          Rotator.isTracking = true;
        } else {
          //// notify user with error response
          setSnack({ message: response.message, variant: "error" });
        }
      }
  };

  const handleCloseSnack = () => {
    // clear snack message popup
    setSnack(null);
    setAnchorEl(null);
  };

  const handleAzimuth = (value) => {
    //get azimuth from text field
    const azimuthValue = parseFloat(value);
    if (azimuthValue < 0 || azimuthValue > 450) {
      // display error
      setAzimuthError("Azimuth should be between 0 and 450");
      setAzimuth("");
    } else {
      // save text from input field
      setAzimuthError("");
      setAzimuth(value);
    }
  };

  const handleElevation = (value) => {
    //get elevation from text field
    const elevationValue = parseFloat(value);
    if (elevationValue < 0 || elevationValue > 180) {
      // display error
      setElevationError("Elevation should be between 0 and 180");
    } else {
      // save text from input field
      setElevationError("");
      setElevation(value);
    }
  };
  const handleToggleAutoTrack = () => {
    // switch tracking control state
    setIsAutoTrackOn((prevState) => !prevState);
    setAzimuth("");
    setElevation("");
  };

  return (
    <>
      {!isTracking ? (
        <Button onClick={handleToggleMenu}>
          <MenuIcon />
        </Button>
      ) : (
        <Button onClick={handleCommand}>
          <StopIcon />
        </Button>
      )}

      <Snackbar
        anchorOrigin={{ vertical: "top", horizontal: "left" }}
        open={Boolean(Snack)}
        onClick={handleCloseSnack}
        onClose={handleCloseSnack}
        autoHideDuration={5000}
        style={{ top: "80px" }}
      >
        <div>
          {Boolean(Snack) && (
            <Alert severity={Boolean(Snack) ? Snack.variant : "info"}>
              {Boolean(Snack) ? Snack.message : ""}
            </Alert>
          )}
        </div>
      </Snackbar>

      {!isTracking && !Snack && (
        <Menu
          anchorEl={anchorEl}
          open={Boolean(anchorEl)}
          onClose={handleCloseMenu}
          style={{ top: "70px" }}
        >
          <MenuItem>
            <FormControlLabel
              control={
                <Switch
                  checked={isAutoTrackOn}
                  onChange={handleToggleAutoTrack}
                  name="autoTrack"
                  color="primary"
                />
              }
              label="Auto Track"
            />
          </MenuItem>

          <MenuItem>
            <TextField
              label="Azimuth"
              type="number"
              value={azimuth}
              onChange={(e) => handleAzimuth(e.target.value)}
              disabled={isAutoTrackOn}
              onBlur={(e) => handleAzimuth(e.target.value)}
              error={Boolean(azimuthError)}
              helperText={azimuthError}
            />
          </MenuItem>

          <MenuItem>
            <TextField
              label="Elevation"
              type="number"
              value={elevation}
              onChange={(e) => handleElevation(e.target.value)}
              disabled={isAutoTrackOn}
              onBlur={(e) => handleElevation(e.target.value)}
              error={Boolean(elevationError)}
              helperText={elevationError}
            />
          </MenuItem>

          <MenuItem style={{ justifyContent: "center" }}>
            <Button onClick={handleCommand}>{"start tracking"}</Button>
          </MenuItem>
        </Menu>
      )}
    </>
  );
}

export default DropdownMenu;
