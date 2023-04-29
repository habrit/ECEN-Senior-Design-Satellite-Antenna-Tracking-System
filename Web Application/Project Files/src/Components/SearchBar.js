import React, { useState } from "react";
import satellites from "./Satellites.json";
import { getSatellite, getLookAngles } from "./Helper";

import {
  InputBase,
  List,
  ListItem,
  ListItemText,
  ClickAwayListener
} from "@material-ui/core";

function SearchBar({ setSatellite, Satellite, Rotator }) {
  const [searchText, setSearchText] = useState("");
  const [suggestions, setSuggestions] = useState([]);

  const handleSearchTextChange = (event) => {
    // save search input
    const newSearchText = event.target.value;
    setSearchText(newSearchText);

    // compute new suggestions based on the search input
    const newSuggestions = satellites
      .filter((satellite) =>
        satellite.OBJECT_NAME.toLowerCase().startsWith(
          newSearchText.toLowerCase()
        )
      )
      .slice(0, 5);
    setSuggestions(newSuggestions);
  };

  const handleSuggestionClick = (suggestion) => async () => {
    // clear suggestions
    setSuggestions([]);

    // update satellite object
    const satellite = await getSatellite(suggestion.OBJECT_NAME);

    if (typeof satellite === "object") {
      // set look angles
      if (Boolean(Rotator) && Rotator.Look_Angles == null) {
        Rotator.Look_Angles = getLookAngles(
          Rotator.ObserverGd,
          satellite.SatRec
        );
      }
      //set satellite
      await setSatellite(satellite);
      console.log("Satellite Selected: " + satellite.Name);
      setSearchText(suggestion.OBJECT_NAME);
    }
  };

  const handleClickAway = () => {
    // clear suggestions
    setSuggestions([]);

    // clear search bar if user didn't choose a satellite
    if (Satellite == null) {
      setSearchText("");
    }
  };

  return (
    <ClickAwayListener onClickAway={handleClickAway}>
      <div style={{ width: "50%", position: "relative" }}>
        <InputBase
          placeholder="Search for satellite... "
          value={searchText}
          onChange={handleSearchTextChange}
          style={{
            margin: "0 0 0 25px",
            padding: "0 0 0 5px",
            borderBottom: "1px solid grey",
            width: "100%",
            display: "flex"
          }}
        />
        {suggestions.length > 0 && (
          <List
            style={{
              position: "absolute",
              top: "70px",
              left: "25px",
              zIndex: 1,
              backgroundColor: "#fff",
              borderRadius: 4,
              boxShadow: "0px 8px 16px 0px rgba(0,0,0,0.2)",
              width: "50%",
              padding: 0,
              margin: 0
            }}
          >
            {suggestions.map((suggestion) => (
              <ListItem
                button
                key={suggestion.OBJECT_NAME}
                onClick={handleSuggestionClick(suggestion)}
              >
                <ListItemText primary={suggestion.OBJECT_NAME} />
              </ListItem>
            ))}
          </List>
        )}
      </div>
    </ClickAwayListener>
  );
}

export default SearchBar;
