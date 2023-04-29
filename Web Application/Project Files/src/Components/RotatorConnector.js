import React, { useState } from "react";
import { getRotator, getLookAngles } from "./Helper";
import {
  Button,
  Dialog,
  DialogTitle,
  DialogContent,
  DialogActions,
  TextField,
  CircularProgress
} from "@material-ui/core";
import RotatorIcon from "./Assets/RotatorIcon";
function RotatorConnector({ setRotator, Satellite, Rotator }) {
  const [connected, setConnected] = useState(false);
  const [openDialog, setOpenDialog] = useState(false);
  const [ipAddress, setIpAddress] = useState("");
  const [loading, setLoading] = useState(false);
  const [error, setError] = useState("");

  const handleConnect = async () => {
    setError("");
    // begin connection procedure
    setLoading(true);

    // attempt to connect to submitted ip address
    const rotator = await getRotator(ipAddress);

    if (typeof rotator === "object") {
      //set looking angles
      if (Boolean(Satellite) && rotator.Look_Angles == null) {
        rotator.Look_Angles = getLookAngles(
          rotator.ObserverGd,
          Satellite.SatRec
        );
      }

      //set rotator object
      setRotator(rotator);
      console.log("Rotator Connected: " + rotator.Name);
      console.log(rotator);

      // display success message and close Dialog Box
      setConnected(true);
      setLoading(false);
      setTimeout(() => {
        setOpenDialog(false);
      }, 1000);
    } else {
      setIpAddress("");
      setError(rotator);
      setLoading(false);
    }
  };

  const handleCancel = () => {
    // clear IP address input field
    setIpAddress("");
    // close dialog Box
    setOpenDialog(false);
    // clear errors
    setError("");
  };

  const handleClick = () => {
    if (!connected) {
      // open Dialog Box
      setOpenDialog(true);
    } else if (Rotator.isTracking) {
      // prevent user from disconnecting while tracking
      console.log("Please stop tracking before disconnecting rotator");
    } else {
      // disconnect from rotator
      setConnected(false);
      setRotator(null);
      setIpAddress("");
    }
  };

  const handleInputChange = (event) => {
    setIpAddress(event.target.value);
  };

  return (
    <div
      style={{
        display: "flex",
        alignItems: "center",
        justifyContent: "flex-end"
      }}
    >
      <Button
        variant="contained"
        color={connected ? "secondary" : "primary"}
        onClick={handleClick}
        disabled={loading}
      >
        {connected ? "Disconnect" : "Connect Rotator"}
      </Button>

      <Dialog
        open={openDialog}
        onClose={handleCancel}
        aria-labelledby="form-dialog-title"
      >
        {!loading & !connected ? <RotatorIcon /> : ""}

        {!connected && (
          <DialogTitle id="form-dialog-title">
            {loading ? "Connecting..." : "Connect to Rotator"}
          </DialogTitle>
        )}
        <DialogContent>
          {connected && "Connection Successful!"}
          {!loading && !connected && (
            <TextField
              autoFocus
              margin="dense"
              label="IP Address"
              value={ipAddress}
              onChange={handleInputChange}
              fullWidth
              error={Boolean(error)}
              helperText={error}
              InputProps={{
                style: {
                  borderColor: error ? "red" : ""
                }
              }}
            />
          )}
        </DialogContent>
        <DialogActions
          style={{ justifyContent: "center", alignItems: "center" }}
        >
          {loading && <CircularProgress />}
          {!loading && !connected && (
            <>
              <Button onClick={handleCancel} color="primary">
                Cancel
              </Button>
              <Button onClick={handleConnect} color="primary">
                Connect
              </Button>
            </>
          )}
        </DialogActions>
      </Dialog>
    </div>
  );
}

export default RotatorConnector;
