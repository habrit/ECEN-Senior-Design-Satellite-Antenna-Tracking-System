import * as React from "react";
import { useState } from "react";
import Container from "@mui/material/Container";
import Divider from "@mui/material/Divider";
import Console from "./components/Console";
import Box from "@mui/material/Box";
import "./styles/App.css";
import IconButton from "@mui/material/IconButton";
import ExpandMoreIcon from "@mui/icons-material/ExpandMore";
import { ThemeProvider, createTheme } from "@mui/material/styles";
import CssBaseline from "@mui/material/CssBaseline";
import { RotatorProvider } from "./classes/RotatorContext";
import MercatorMap from "./components/MercatorMap";
import { MapContainer, TileLayer, Marker, Popup } from "react-leaflet";
import RotatorModel from "./components/RotatorModel";
import StepInput from "./components/StepInput";
const darkTheme = createTheme({
  palette: {
    mode: "dark",
  },
});
export default function App() {


  return (
    <div>
      <ThemeProvider theme={darkTheme}>
        <RotatorProvider>
          <div className="mercator-map">
            <MercatorMap />
          </div>

          <div className="console">
           <Console />
            
          </div>
        </RotatorProvider>
      </ThemeProvider>
    </div>
  );
}
