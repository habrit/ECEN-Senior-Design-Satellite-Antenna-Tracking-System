import React, { useState, useEffect, useRef } from "react";
import { createRoot } from "react-dom/client";
import { loadModules } from "esri-loader";
import { Button } from "@material-ui/core";
import { Build as BuildIcon } from "@material-ui/icons";
import {
  renderObserver,
  renderSatellite,
  renderGrid,
  renderDayNight,
  renderGroundTrack
} from "./MapHelper";
import MapSettings from "./MapSettings";
import "./Map.css";

function MercatorMap({ Satellite, Rotator }) {
  const MapElement = useRef(null);
  const [anchorEl, setAnchorEl] = useState(null);
  const [gridGraphic, setGridGraphic] = useState(null);
  const [groundTrackGraphic, setGroundTrackGraphic] = useState(null);
  const [dayNightGraphic, setDayNightGraphic] = useState(null);
  const [showGroundTrack, setShowGroundTrack] = useState(false);

  useEffect(() => {
    let view;

    loadModules(
      [
        "esri/config",
        "esri/views/MapView",
        "esri/Map",
        "esri/Graphic",
        "esri/widgets/Popup",
        "esri/geometry/Mesh",
        "esri/layers/GraphicsLayer"
      ],
      {
        css: true
      }
    ).then(([esriConfig, MapView, Map, Graphic]) => {
      esriConfig.apiKey =
        "AAPK74b83cbed7c040a4912534f0a31191eccop4MGikdDa5odq5tNJeSsCrLgGBy2Tp_I_7cOfpmGxeEfDZbPxCOi2xlOx9o7AF";
      // create map
      const map = new Map({
        basemap: "hybrid"
      });

      view = new MapView({
        container: MapElement.current,
        map: map,
        zoom: 4,
        constraints: {
          minZoom: 4
        }
      });

      //setup Grid
      renderGrid(view).then((graphic) => {
        setGridGraphic(graphic);
      });

      //setup Day/Night Terminator
      renderDayNight(view).then((graphic) => {
        setDayNightGraphic(graphic);
      });

      //setup rotator
      if (Rotator !== null) {
        var point = renderObserver(view, Rotator);
      }

      // setup groundtrack and satellite
      if (Satellite !== null) {
        var point = renderSatellite(view, Satellite);
        renderGroundTrack(view, Satellite.Longitude, Satellite.Latitude).then(
          (graphic) => {
            setShowGroundTrack(true);
            setGroundTrackGraphic(graphic);
          }
        );
      }

      // open/close map settings popup
      const handleToggleMapSettings = (event) => {
        setAnchorEl(event.currentTarget);
      };

      // create and add map settings popup to viewport
      var node = document.createElement("div");
      view.ui.add(node, "top-right");

      createRoot(node).render(
        <Button
          onClick={handleToggleMapSettings}
          style={{ backgroundColor: "white", width: "34px", minWidth: "0px" }}
        >
          <BuildIcon style={{ width: "100%" }} />
        </Button>
      );

      // move the zoom buttons to the top right
      view.ui.move("zoom", "top-right");
    });

    return () => {
      if (view) {
        view.destroy();
      }
    };
  }, [Satellite, Rotator]);

  return (
    <div
      style={{
        height: "calc(100% - 64px)",
        width: "100%",
        position: "absolute",
        top: 64,
        left: 0
      }}
      ref={MapElement}
    >
      <div style={{ position: "absolute", right: "100px" }}>
        <MapSettings
          anchorEl={anchorEl}
          setAnchorEl={setAnchorEl}
          gridGraphic={gridGraphic}
          groundTrackGraphic={groundTrackGraphic}
          dayNightGraphic={dayNightGraphic}
          showGroundTrack={showGroundTrack}
          setShowGroundTrack={setShowGroundTrack}
        />
      </div>
    </div>
  );
}

export default MercatorMap;
