import { loadModules } from "esri-loader";
const SatelliteIcon = require("./Assets/SatelliteIcon.png");
const CubeSatIcon = require("./Assets/CubeSat.png");
const MarkerIcon = require("./Assets/MarkerIcon.png");

///////////////////////////////////////////////////////////////
//////////////            Path Creators           /////////////
///////////////////////////////////////////////////////////////

export function getGround(x1, y1) {
  try {
    if (x1 < -180 || x1 > 180 || y1 < -90 || y1 > 90) {
      throw new Error("Latitude/Longitude Out of Range");
    }
    const paths = [];
    //compute phase needed to intersect with satellite
    const b = Math.acos(y1 / 90) * (180 / Math.PI) - 1 * x1;
    var y;
    // generate coordinate path
    for (let i = 0; i < 1000; i++) {
      const x = -180 + (i * 360) / 1000;
      if (Math.abs(y1) < 30) {
        y = 30 * Math.cos((1 * x + b) * (Math.PI / 180));
      } else if (Math.abs(y1) < 60) {
        y = 60 * Math.cos((1 * x + b) * (Math.PI / 180));
      } else {
        y = 70 * Math.cos((1 * x + b) * (Math.PI / 180));
      }
      paths.push([x, y]);
    }
    return paths;
  } catch (error) {
    console.log(error);
    return null;
  }
}

export function getGrid() {
  const grid = [];

  // generate latitudes
  for (let lat = -75; lat <= 75; lat += 15) {
    const line = [];
    for (let lon = -180; lon <= 180; lon += 1) {
      line.push([lon, lat]);
    }
    grid.push(line);
  }

  // generate longitudes
  for (let lat = -180; lat <= 180; lat += 15) {
    const line = [];
    for (let lon = -180; lon <= 180; lon += 15) {
      line.push([lat, lon]);
    }
    grid.push(line);
  }

  return grid;
}

export function getDayNight() {
  const numPoints = 100;
  const points = [];

  // get the current date and time
  const now = new Date();

  // calculate the current julian date
  const julianDate =
    now.getTime() / 86400000 - now.getTimezoneOffset() / 1440 + 2440587.5;

  // calculate the position of the Sun
  const meanLongitude = (280.46 + 0.9856474 * julianDate) % 360;
  const meanAnomaly = 357.528 + 0.9856003 * julianDate;
  const eclipticLongitude =
    meanLongitude +
    1.915 * Math.sin((meanAnomaly * Math.PI) / 180) +
    0.02 * Math.sin((2 * meanAnomaly * Math.PI) / 180);
  const obliquity = 23.439 - 0.0000004 * julianDate;
  const rightAscension =
    (Math.atan2(
      Math.cos((obliquity * Math.PI) / 180) *
        Math.sin((eclipticLongitude * Math.PI) / 180),
      Math.cos((eclipticLongitude * Math.PI) / 180)
    ) *
      180) /
    Math.PI;
  const declination =
    (Math.asin(
      Math.sin((obliquity * Math.PI) / 180) *
        Math.sin((eclipticLongitude * Math.PI) / 180)
    ) *
      180) /
    Math.PI;

  // calculate the position of the terminator
  const latitudes = [];
  const longitudes = [];

  //add boundary
  latitudes.push(90);
  longitudes.push(-180);
  latitudes.push(90);
  longitudes.push(180);

  for (let i = 0; i <= numPoints; i++) {
    const theta = (2 * Math.PI * i) / numPoints;
    const longitude = rightAscension - (theta * 180) / Math.PI;
    const latitude =
      (Math.atan(-Math.cos(theta) / Math.tan((declination * Math.PI) / 180)) *
        180) /
      Math.PI;
    latitudes.push(latitude);
    longitudes.push(longitude);
  }

  // construct coordinates array
  for (let i = 0; i < numPoints; i++) {
    points.push([longitudes[i], latitudes[i]]);
  }
  return points;
}

///////////////////////////////////////////////////////////////
//////////////        Component Renders           /////////////
///////////////////////////////////////////////////////////////

export function renderGrid(view) {
  return loadModules(["esri/Graphic"], {
    css: true
  }).then(([Graphic]) => {
    // create line geometry
    const polyline = {
      type: "polyline",
      paths: getGrid()
    };

    // create symbol
    const lineSymbol = {
      type: "simple-line",
      color: [100, 200, 200],
      width: 1
    };

    // create graphic
    const gridGraphic = new Graphic({
      geometry: polyline,
      symbol: lineSymbol,
      visible: false
    });

    // add graphic to viewport
    view.graphics.add(gridGraphic);

    return gridGraphic;
  });
}

export function renderDayNight(view) {
  return loadModules(["esri/Graphic"], {
    css: true
  }).then(([Graphic]) => {
    // create polygon geometry
    const polygon = {
      type: "polygon",
      rings: getDayNight()
    };

    // create symbol
    const simpleFillSymbol = {
      type: "simple-fill",
      color: [1, 1, 1, 0.2],
      outline: {
        color: [1, 1, 1, 0.0],
        width: 1
      }
    };

    // create graphic
    const dayNightGraphic = new Graphic({
      geometry: polygon,
      symbol: simpleFillSymbol,
      visible: false
    });

    // add graphic to viewport
    view.graphics.add(dayNightGraphic);

    return dayNightGraphic;
  });
}

export function renderGroundTrack(view, Longitude, Latitude) {
  return loadModules(["esri/Graphic"], {
    css: true
  }).then(([Graphic]) => {
    // create line geometry
    const polyline = {
      type: "polyline",
      paths: getGround(Longitude, Latitude)
    };

    // create symbol
    const lineSymbol = {
      type: "simple-line",
      color: [226, 119, 40],
      width: 4
    };

    // create graphic
    const groundTrackGraphic = new Graphic({
      geometry: polyline,
      symbol: lineSymbol
    });

    // add graphic to viewport
    view.graphics.add(groundTrackGraphic);

    return groundTrackGraphic;
  });
}

export function renderObserver(view, Rotator) {
  return loadModules(["esri/Graphic"], {
    css: true
  }).then(([Graphic]) => {
    // Plot Observer
    const point = {
      type: "point",
      longitude: parseFloat(Rotator.Longitude),
      latitude: parseFloat(Rotator.Latitude)
    };

    // create symbol
    const markerSymbol = {
      type: "picture-marker",
      url: MarkerIcon,
      width: "60px",
      height: "40px"
    };

    // create graphic
    const Observer = new Graphic({
      geometry: point,
      symbol: markerSymbol,
      attributes: Rotator,
      popupTemplate: Rotator.Template
    });

    // add graphic to viewport
    view.graphics.addMany([Observer]);

    return point;
  });
}

export function renderSatellite(view, Satellite) {
  return loadModules(["esri/Graphic"], {
    css: true
  }).then(([Graphic]) => {
    // create coordinate point
    const point = {
      type: "point",
      longitude: Satellite.Longitude,
      latitude: Satellite.Latitude
    };
    // create symbol
    let markerSymbol = null;

    if (Satellite.Name.includes("CUBE")) {
      markerSymbol = {
        type: "picture-marker",
        url: CubeSatIcon,
        width: "300",
        height: "100"
      };
    } else {
      markerSymbol = {
        type: "picture-marker",
        url: SatelliteIcon,
        width: "200",
        height: "300"
      };
    }

    // create graphic
    const SatGraphic = new Graphic({
      geometry: point,
      symbol: markerSymbol,
      attributes: Satellite,
      popupTemplate: Satellite.Template
    });

    // add graphic to viewport
    view.graphics.addMany([SatGraphic]);

    return point;
  });
}
