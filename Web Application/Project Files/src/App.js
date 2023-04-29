import React, { useState } from "react";
import { createRoot } from "react-dom/client";
import DropdownMenu from "./Components/DropdownMenu";
import SearchBar from "./Components/SearchBar";
import RotatorConnector from "./Components/RotatorConnector";
import MercatorMap from "./Components/MercatorMap";
import { AppBar, Toolbar } from "@material-ui/core";

// render main app components; toolbar and the map
function App() {
  //intialize state object variables
  const [Satellite, setSatellite] = useState(null);
  const [Rotator, setRotator] = useState(null);

  return (
    <div>
      <AppBar
        position="fixed"
        color="default"
        elevation={1}
        style={{ minHeight: 64 }}
      >
        <Toolbar style={{ flexGrow: 1, justifyContent: "space-between" }}>
          <div style={{ display: "flex", flex: 1 }}>
            <DropdownMenu Rotator={Rotator} Satellite={Satellite} />
            <SearchBar
              Rotator={Rotator}
              Satellite={Satellite}
              setSatellite={setSatellite}
            />
          </div>
          <div style={{ justifyContent: "flex-end" }}>
            <RotatorConnector
              setRotator={setRotator}
              Rotator={Rotator}
              Satellite={Satellite}
            />
          </div>
        </Toolbar>
      </AppBar>

      <MercatorMap Rotator={Rotator} Satellite={Satellite} />
    </div>
  );
}

createRoot(document.getElementById("root")).render(<App />);
