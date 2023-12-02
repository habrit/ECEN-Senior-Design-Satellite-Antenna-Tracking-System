import React, { useRef, forwardRef } from "react";
import Box from "@mui/material/Box";
import Button from "@mui/material/Button";
import Typography from "@mui/material/Typography";
import RotatorIcon from "../icons/RotatorIcon";
import { useRotator } from "../classes/RotatorContext";

interface ConnectionModalProps {
  setOpen: (value: boolean) => void;
}

const Modal: React.FC<ConnectionModalProps> = forwardRef((props, ref) => {
  const { rotator, updateRotator } = useRotator();
  const modalRef = useRef();
  const handleSetNewRotator = (ref) => {
    const newRotator = {
      CallSign: "YourCallSign",
      Radio: "YourRadio",
      Rotator: {
        // Initial Rotator properties
        Mode: "Auto",
        Model: "YourRotator",
        isAutoTracking: false,
        isManualTracking: false,
        Heading: 0,
        aOffset: 0,
        yOffset: 0,
        Position: { azimuth: 0, elevation: 0 },
        Location: { Latitude: 30.628, Longitude: -96.3344 },
        isReady: false,
        UTCoffset: -5,
      },
    };
    updateRotator(newRotator);
    props.setOpen(false);
  };

  return (
    <Box
      sx={{
        position: "absolute" as "absolute",
        top: "50%",
        color: "white",
        left: "50%",
        transform: "translate(-50%, -50%)",
        width: 300,
        bgcolor: "#181C20",
        boxShadow: 24,
        borderRadius: "8px",
        p: 4,
      }}
    >
      <Typography
        id="modal-modal-title"
        variant="h6"
        component="h2"
        sx={{ color: "#8C92A4" }}
      >
        Connect a Rotator
      </Typography>
      <RotatorIcon />
      <Button onClick={handleSetNewRotator}>Connect</Button>
    </Box>
  );
});

export default Modal;
