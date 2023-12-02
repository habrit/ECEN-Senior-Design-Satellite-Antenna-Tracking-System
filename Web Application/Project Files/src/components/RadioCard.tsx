import Card from "@mui/material/Card";
import CardContent from "@mui/material/CardContent";
import Typography from "@mui/material/Typography";
import RotatorData from "../data/RotatorData.json";
import React, { useState } from "react";
import Stack from "@mui/material/Stack";
import Paper from "@mui/material/Paper";
import Grid from "@mui/material/Unstable_Grid2";
import List from "@mui/material/List";
import ListItem from "@mui/material/ListItem";
import LinearGauge from "./LinearGauge";
import ListItemText from "@mui/material/ListItemText";
import { useRotator } from "../classes/RotatorContext";

export default function BasicCard() {
  const [signalLevel, setSignalLevel] = useState(-85);
  const { rotator, selectedSatellite } = useRotator();
  return (
    <Paper
      elevation={0}
      variant="outlined"
      //border={1}
      sx={{
        minWidth: 300,
        width:"30%",
        padding: "8px",
        height: "100%",
        justifyContent: "center",
        display: "flex",
        alignItems: "center",
        backgroundColor: "transparent",
        borderRadius: "4px",
      }}
    >
      <Stack
        direction="column"
      >
        <Paper
          elevation={0}
          direction="row"
          //variant="outlined"
          border={1}
          sx={{
            padding: "8px",
            paddingBottom: "0px",
            //marginTop: "16px",
            justifyContent: "center",
            display: "flex",
            alignItems: "center",
            backgroundColor: "transparent",
            borderRadius: "4px",
          }}
        >
          <LinearGauge />
        </Paper>

        <Paper
          elevation={0}
          //direction="row"
          //variant="outlined"
          //border={1}
          
          sx={{
            padding: "8px",
            marginTop: "12px",
            justifyContent: "center",
            display: "flex",
            alignItems: "center",
            backgroundColor: "transparent",
            borderRadius: "4px",
          }}
        >
          <Stack  spacing={2} direction="row" >
            <Stack direction="column">
              <Stack direction="row">
                <Typography
                  variant="caption"
                  component="div"
                  sx={{
                    fontFamily: "Roboto Mono, monospace",
                    fontSize: 12,
                    paddingRight: "16px",
                  }}
                >
                  LOS
                </Typography>
                <Typography
                  variant="caption"
                  component="div"
                  sx={{
                    color: "#8C92A4",
                    fontFamily: "Roboto Mono, monospace",
                    fontSize: 12,
                  }}
                >
                  {/*TO-DO Request From MCU*/}
                  10:10:92
                </Typography>
              </Stack>
              <Stack direction="row">
                <Typography
                  variant="caption"
                  component="div"
                  sx={{
                    fontFamily: "Roboto Mono, monospace",
                    fontSize: 12,
                    paddingRight: "16px",
                  }}
                >
                  AOS
                </Typography>
                <Typography
                  variant="caption"
                  component="div"
                  sx={{
                    color: "#8C92A4",
                    fontFamily: "Roboto Mono, monospace",
                    fontSize: 12,
                  }}
                >
                  {/*TO-DO  Request From MCU*/}
                  03:39:10
                </Typography>
              </Stack>
            </Stack>
            <Stack direction="column">
              <Stack direction="row">
                <Typography
                  variant="caption"
                  component="div"
                  sx={{
                    fontFamily: "Roboto Mono, monospace",
                    fontSize: 12,
                    paddingRight: "16px",
                  }}
                >
                  Uplink
                </Typography>
                <Typography
                  variant="caption"
                  component="div"
                  sx={{
                    color: "#8C92A4",
                    fontFamily: "Roboto Mono, monospace",
                    fontSize: 12,
                  }}
                >
                  {selectedSatellite
                    ? selectedSatellite.uplink + " kHz"
                    : "0 kHz"}
                </Typography>
              </Stack>
              <Stack direction="row">
                <Typography
                  variant="caption"
                  component="div"
                  sx={{
                    fontFamily: "Roboto Mono, monospace",
                    fontSize: 12,
                    paddingRight: "16px",
                  }}
                >
                  Downlink
                </Typography>
                <Typography
                  variant="caption"
                  component="div"
                  sx={{
                    color: "#8C92A4",
                    fontFamily: "Roboto Mono, monospace",
                    fontSize: 12,
                  }}
                >
                  {selectedSatellite
                    ? selectedSatellite.downlink + " kHz"
                    : "0 kHz"}
                </Typography>
              </Stack>
            </Stack>
          </Stack>
        </Paper>
      </Stack>
    </Paper>
  );
}
