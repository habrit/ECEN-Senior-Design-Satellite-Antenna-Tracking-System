import React, { useState } from "react";
import {
  Popover,
  List,
  ListItem,
  ListItemText,
  Switch
} from "@material-ui/core";

function MapSettings({
  anchorEl,
  setAnchorEl,
  gridGraphic,
  dayNightGraphic,
  groundTrackGraphic,
  showGroundTrack,
  setShowGroundTrack
}) {
  const open = Boolean(anchorEl);
  const [showGrid, setShowGrid] = useState(false);
  const [showDayNight, setShowDayNight] = useState(false);

  const handleClose = () => {
    setAnchorEl(null);
  };

  const handleShowGridChange = () => {
    // change grid layer visibility
    setShowGrid(!showGrid);
    gridGraphic.visible = !gridGraphic.visible;
  };

  const handleShowDayNightChange = () => {
    // change day/night terminator layer visibility
    setShowDayNight(!showDayNight);
    dayNightGraphic.visible = !dayNightGraphic.visible;
  };

  const handleShowGroundTrackChange = (event) => {
    // change ground track layer visibility is satellite is chosen
    if (groundTrackGraphic != null) {
      setShowGroundTrack(!showGroundTrack);
      groundTrackGraphic.visible = !groundTrackGraphic.visible;
    }
  };

  return (
    <Popover
      open={open}
      anchorEl={anchorEl}
      onClose={handleClose}
      anchorOrigin={{
        vertical: "top",
        horizontal: "left"
      }}
      transformOrigin={{
        vertical: "top",
        horizontal: "left"
      }}
      style={{ marginLeft: "-50px" }}
    >
      <List>
        <ListItem>
          <ListItemText primary="Grid" />
          <Switch
            checked={showGrid}
            onChange={handleShowGridChange}
            color="primary"
          />
        </ListItem>
        <ListItem>
          <ListItemText primary="Day/Night" />
          <Switch
            checked={showDayNight}
            onChange={handleShowDayNightChange}
            color="primary"
          />
        </ListItem>
        <ListItem>
          <ListItemText primary="Ground Track" />
          <Switch
            checked={showGroundTrack}
            onChange={handleShowGroundTrackChange}
            color="primary"
          />
        </ListItem>
      </List>
    </Popover>
  );
}

export default MapSettings;
