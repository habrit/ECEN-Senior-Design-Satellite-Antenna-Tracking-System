import React, { useState } from "react";
import Paper from "@mui/material/Paper";
import InputBase from "@mui/material/InputBase";
import Divider from "@mui/material/Divider";
import IconButton from "@mui/material/IconButton";
import SearchIcon from "@mui/icons-material/Search";
import Button from "@mui/material/Button";
import FilterAltIcon from "@mui/icons-material/FilterAlt";
import List from "@mui/material/List";
import ListItem from "@mui/material/ListItem";
import ListItemText from "@mui/material/ListItemText";
import satelliteData from "../data/ActiveSatelliteDatabase.json";
import { ClickAwayListener } from "@mui/base/ClickAwayListener";
import { useRotator } from "../classes/RotatorContext";
import {getSatelliteData, getSatData} from "../utils/Helper.jsx";

export default function SearchBar() {
  const [searchText, setSearchText] = useState("");
  const [suggestions, setSuggestions] = useState([]);
  const {
    trackedSatellites,
    setTrackedSatellites,
    selectedSatellite,
    setSelectedSatellite,
  } = useRotator();

  const handleSearchTextChange = (event) => {
    // save search input
    const newSearchText = event.target.value;
    setSearchText(newSearchText);

    // compute new suggestions based on the search input
    const newSuggestions = satelliteData
      .filter((satelliteData) =>
        satelliteData.name
          .toLowerCase()
          .startsWith(newSearchText.toLowerCase()),
      )
      .slice(0, 4);
    setSuggestions(newSuggestions);
  };

  const handleSuggestionClick = async (sat) => {
    // Clear suggestions
    setSuggestions([]);

    // Check if the maximum number of tracked satellites has been reached (10).
    if (trackedSatellites.length === 10) {
      console.log("10 satellites are already being tracked, try removing one");
    } else if (
      trackedSatellites.some((trackedSat) => trackedSat.name === sat.name)
    ) {
      // Check that the satellite is not already in the list
      console.log("This satellite is already being tracked");
    } else {
      // Create a new satellite object to add to the trackedSatellites array.
      const initialData = await getSatelliteData(sat.name);
      const date = new Date().getTime();
      const newSatellite = {
        name: sat.name,
        norad_id: sat.norad_id,
        uplink: sat.uplink,
        downlink: sat.downlink,
        mode: sat.mode,
        tle: initialData.TLE,
        satRec: initialData.satRec,
        dataDate: date,
        nextPass: initialData.pass
      };

      // Clone the old list and add the new satellite to it.
      const newList = [...trackedSatellites, newSatellite];

      // Update the state with the new list of tracked satellites.
      setTrackedSatellites(newList);

      if (selectedSatellite === null) {
        setSelectedSatellite(newSatellite);
      }
    }
    
    setSearchText("");
  };

  const handleClickAway = () => {
    // clear suggestions
    setSuggestions([]);

    // clear search bar if user didn't choose a satellite
    setSearchText("");
  };

  return (
    <ClickAwayListener onClickAway={handleClickAway}>
      <div>
        <Paper
          elevation={0}
          component="form"
          sx={{
            height: "3rem",
            backgroundColor: "#181C20",
            boxShadow: "inset 0 0 10px #000000",
            p: "0px 4px",
            display: "flex",
            alignItems: "center",
            width: "100%",
            maxWidth: 280,
          }}
        >
          <InputBase
            sx={{
              ml: 1,
              flex: 1,
              color: "#8C92A4",
              fontFamily: "Roboto Mono, monospace",
            }}
            placeholder="Search..."
            value={searchText}
            onChange={handleSearchTextChange}
          />
          <IconButton
            type="button"
            sx={{ p: "10px", color: "#8C92A4" }}
            aria-label="search"
          >
            <SearchIcon />
          </IconButton>
        </Paper>

        {suggestions.length > 0 && (
          <List>
            {suggestions.map((suggestion) => (
              <ListItem
                key={suggestion.name}
                sx={{
                  p: "0px",
                  paddingLeft: "8px",
                  ".MuiListItemText-root": { m: "0px" },
                  ".MuiTypography-root": {
                    fontFamily: "Roboto Mono, monospace",
                  },
                }}
              >
                <ListItemText primary={suggestion.name} />

                <Button
                  sx={{
                    paddingTop: "0px",
                    paddingBottom: "0px",
                    color: "#8C92A4",
                    "&.MuiButton-root": {
                      fontFamily: "Roboto Mono, monospace",
                    },
                  }}
                  onClick={() => handleSuggestionClick(suggestion)}
                >
                  Add
                </Button>
              </ListItem>
            ))}
          </List>
        )}
      </div>
    </ClickAwayListener>
  );
}
