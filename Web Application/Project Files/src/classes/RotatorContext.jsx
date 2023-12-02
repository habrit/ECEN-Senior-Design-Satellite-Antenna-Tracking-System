import * as React from "react";
import Container from "@mui/material/Container";
import Divider from "@mui/material/Divider";
import Box from "@mui/material/Box";
import { ThemeProvider, createTheme } from "@mui/material/styles";
import CssBaseline from "@mui/material/CssBaseline";
import { createContext, useContext, ReactNode, useState } from "react";

const RotatorContext = createContext();

export function useRotator() {
  return useContext(RotatorContext);
}

export function RotatorProvider({ children }) {
  const [azimuth, setAzimuth] = useState(0);
  const [elevation, setElevation] = useState(0);
  const [heading, setHeading] = useState(0);
  const [mode, setMode] = useState(0);
  const [aOffset, setAOffset] = useState(0);
  const [yOffset, setYOffset] = useState(0);
  const [radio, setRadio] = useState(null);
  const [rotator, setRotator] = useState(null);
  const [selectedSatellite, setSelectedSatellite] = useState(null);
  const [trackedSatellites, setTrackedSatellites] = useState([]);
  const [isAutoTracking, setIsAutoTracking] = useState(false);
  const [isManualTracking, setIsManualTracking] = useState(false);

  const updateIsManualTracking = (value) => {
    setIsManualTracking(value);
  };

  const updateIsAutoTracking = (nvalue) => {
    setIsAutoTracking(nvalue);
  };

  const updateAzimuth = (newAzimuth) => {
    setAzimuth(newAzimuth);
  };

  const updateElevation = (newElevation) => {
    setElevation(newElevation);
  };

  const updateHeading = (newHeading) => {
    setHeading(newHeading);
  };

  const updateAzimuthOffset = (offset) => {
    setAOffset(offset);
  };

  const updateElevationOffset = (offset) => {
    setYOffset(offset);
  };
  const updateMode = (newMode) => {
    setMode(newMode);
  };

  const updateRotator = (newRotator) => {
    setRotator(newRotator);

    setAzimuth(newRotator.Rotator.Position.azimuth);
    setElevation(newRotator.Rotator.Position.elevation);

    setHeading(newRotator.Rotator.Heading);
    setMode(newRotator.Rotator.Mode);
    setAOffset(newRotator.Rotator.aOffset);
    setYOffset(newRotator.Rotator.yOffset);
  };

  return (
    <RotatorContext.Provider
      value={{
        azimuth,
        elevation,
        isAutoTracking,
        updateIsAutoTracking,
        rotator,
        isManualTracking,
        updateIsManualTracking,
        updateRotator,
        updateAzimuth,
        updateElevation,
        setSelectedSatellite,
        trackedSatellites,
        setTrackedSatellites,
        selectedSatellite,
        heading,
        mode,
        aOffset,
        yOffset,
        updateHeading,
        updateMode,
        updateAzimuthOffset,
        updateElevationOffset,
      }}
    >
      {children}
    </RotatorContext.Provider>
  );
}
