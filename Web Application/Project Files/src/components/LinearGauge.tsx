import React, { useState, useEffect } from "react";
import Box from "@mui/material/Box";
import Stack from "@mui/material/Stack";
import Typography from "@mui/material/Typography";

function toPercent(dB) {
  return 0.909091 * dB + 109.091;
}

function toDb(percent) {
  return -1.1 * percent - 10;
}

const LinearGauge = () => {
  const [xPosition, setXPosition] = useState(0);
  const [xPositionLabel, setXPositionLabel] = useState(0);
  const directionRef = React.useRef(1); // 1 for increasing, -1 for decreasing

  useEffect(() => {
    const updatePosition = () => {
      if (xPosition === 0) {
        directionRef.current = 1; // Start increasing
      } else if (xPosition === 100) {
        directionRef.current = -1; // Start decreasing
      }

      const increment = directionRef.current === 1 ? 1 : -1;
      setXPosition((prevXPosition) => prevXPosition + increment);
      setXPositionLabel((prevXPositionLabel) => prevXPositionLabel + increment);
    };

    const interval = setInterval(updatePosition, 50); // Adjust the interval duration as needed

    return () => {
      clearInterval(interval);
    };
  }, [xPosition, xPositionLabel]);

  return (
    <div style={{ width: "100%" }}>
      <Stack
        width="100%"
        direction="row"
        justifyContent="space-between"
        alignItems="center"
        spacing={2}
      >
        <Typography
          variant="caption"
          component="h1"
          sx={{ fontFamily: "Roboto Mono, monospace" }}
        >
          -120
        </Typography>
        <Typography
          variant="caption"
          component="h2"
          sx={{ fontFamily: "Roboto Mono, monospace" }}
        >
          Signal Strength (dB)
        </Typography>
        <Typography
          variant="caption"
          component="h3"
          sx={{ fontFamily: "Roboto Mono, monospace" }}
        >
          -10
        </Typography>
      </Stack>

      <svg width={"100%"} height={"45px"}>
        {/* Red */}
        <rect x={0} y={0} fill="#FF0000" width={"18.182%"} height={"25px"} />
        {/* Orange */}
        <rect
          x={"18.182%"}
          y={0}
          fill="#FF9900"
          width={"27.273%"}
          height={"25px"}
        />
        {/* Green */}
        <rect
          x={"45.455%"}
          y={0}
          fill="#19B600"
          width={"54.545%"}
          height={"25px"}
        />
        <g>
          {/* Gauge Indicator */}
          <rect
            x={`${xPosition}%`}
            y={0}
            fill="white"
            width={"0.15rem"}
            height={"32px"}
          />
          {/* Label */}
          <text
            x={`${xPosition-4}%`}
            y={45}
            fill="white"
            fontSize="12"
            fontFamily="Roboto Mono, monospace"

          >
            {Math.floor(toDb(xPositionLabel))}
          </text>
        </g>
      </svg>
    </div>
  );
};

export default LinearGauge;

