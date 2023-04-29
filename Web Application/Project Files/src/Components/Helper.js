import {
  ecfToLookAngles,
  propagate,
  gstime,
  twoline2satrec,
  eciToEcf,
  eciToGeodetic,
  degreesLong,
  degreesLat
} from "satellite.js";

///////////////////////////////////////////////////////////////
/////////    Satellite/Rotator Object Constructors     ////////
///////////////////////////////////////////////////////////////
export async function getSatellite(satelliteName) {
  // retrieve orbital data from Celestrak
  const OMM = await fetchOrbitalData(satelliteName, "JSON");
  const TLE = await fetchOrbitalData(satelliteName, "TLE");
  const satRec = getSatRec(TLE);
  const positionGd = getPositionGd(satRec);

  // create popup template
  const temp = {
    title: "{Name}",
    content: [
      {
        type: "fields",
        fieldInfos: [
          {
            fieldName: "Longitude",
            label: "Longitude",
            format: {
              digitSeparator: true,
              places: 0
            }
          },
          {
            fieldName: "Latitude",
            label: "Latitude",
            format: {
              digitSeparator: true,
              places: 0
            }
          },
          {
            fieldName: "Altitude",
            label: "Altitude",
            format: {
              digitSeparator: true,
              places: 0
            }
          },
          {
            fieldName: "Time",
            label: "Time (UTC)",
            format: {
              digitSeparator: true,
              places: 0
            }
          }
        ]
      }
    ]
  };

  //Construct satellite object
  const satellite = {
    Name: satelliteName,
    Time: getTimeUTC(positionGd),
    Longitude: degreesLong(positionGd.longitude),
    Latitude: degreesLat(positionGd.latitude),
    Altitude: positionGd.height,
    SatRec: satRec,
    Omm: OMM,
    Template: temp
  };

  return satellite;
}

export async function getRotator(IP) {
  if (isValid(IP)) {
    // retrieve rotator data
    const connectResponse = await fetchJson(IP, "Connect");
    if (connectResponse !== undefined) {
      // create popup template
      const temp = {
        title: "Observer: {Call_Sign}",
        content: [
          {
            type: "fields",
            fieldInfos: [
              {
                fieldName: "Longitude",
                label: "Longitude",
                format: {
                  digitSeparator: true,
                  places: 0
                }
              },
              {
                fieldName: "Latitude",
                label: "Latitude",
                format: {
                  digitSeparator: true,
                  places: 0
                }
              },
              {
                fieldName: "Model",
                label: "Model",
                format: {
                  digitSeparator: true,
                  places: 0
                }
              },
              {
                fieldName: "isTracking",
                label: "Tracking",
                format: {
                  digitSeparator: true,
                  places: 0
                }
              },
              {
                fieldName: "Time",
                label: "Local Time",
                format: {
                  digitSeparator: true,
                  places: 0
                }
              }
            ]
          }
        ]
      };

      // construct rotator object
      const rotator = {
        Name: connectResponse.NAME,
        IP: connectResponse.IP,
        Call_Sign: connectResponse.CALL_SIGN,
        Model: connectResponse.MODEL,
        Time: getLocalTime(connectResponse.LONGITUDE),
        Longitude: connectResponse.LONGITUDE,
        Latitude: connectResponse.LATITUDE,
        ObserverGd: {
          longitude: connectResponse.LONGITUDE,
          latitude: connectResponse.LATITUDE,
          height: connectResponse.HEIGHT
        },
        Look_Angles: null,
        isTracking: false,
        Template: temp
      };

      return rotator;
    } else {
      return "Request has Timed Out. Please check IP address.";
    }
  } else {
    return "Invalid IP address. Please enter a valid IP address.";
  }
}
///////////////////////////////////////////////////////////////
//////////////            Data Fetching           /////////////
///////////////////////////////////////////////////////////////
function fetchOrbitalData(satelliteName, format) {
  // contruct url from satellite name and format
  const url = `https://www.celestrak.com/NORAD/elements/gp.php?NAME=${satelliteName}&FORMAT=${format}`;
  // request document
  return fetch(url)
    .then((response) => response.text())
    .then((data) => {
      if (format === "TLE") {
        // retirn tle string
        return data;
      } else {
        // parse/return JSON into Object
        return JSON.parse(data)[0];
      }
    })
    .catch((error) => {
      console.error(error);
    });
}

export async function fetchJson(IP, requestType, payload) {
  try {
    if (isValid(IP)) {
      // verify that the ip doesnt timeout
      // const timeoutPromise = new Promise((_, reject) =>
      //  setTimeout(() => reject(new Error("Request timed out")), 10000)
      // );

      // construct url
      var url = "";

      if (Boolean(payload)) {
        url = `https://${IP}/${requestType}/r/${payload}`;
      } else {
        url = `https://${IP}/${requestType}/r`;
      }

      //dummy server
      console.log(requestType);
      if (requestType === "Connect") {
        return {
          NAME: "David's Rotator",
          IP: "192.50.100.1",
          CALL_SIGN: "W5QZ",
          MODEL: "Yaesu G-5500",
          LONGITUDE: "-96.3344",
          LATITUDE: "30.6280"
        };
      } else if (requestType === "StartAuto") {
        return {
          message: "Rotator has started auto-tracking"
        };
      } else if (requestType === "StartManual") {
        return {
          message: "Rotator has started auto-tracking"
        };
      } else if (requestType === "Stop") {
        return {
          message: "Rotator has stopped tracking"
        };
      } else {
        return null;
      }

      // request json from mcu server
      //const response = await Promise.race([fetch(url), timeoutPromise]);

      //const data = await response.json();
      //return data;
    } else {
      // invalid IP address error
      throw new Error("Invalid IP Address");
    }
  } catch (error) {
    console.error(error);
  }
}

///////////////////////////////////////////////////////////////
///////////       Orbital Dynamics/Other          /////////////
///////////////////////////////////////////////////////////////
export function getPositionGd(satRec) {
  // get time
  const gmst = gstime(new Date());

  // propgate position
  const positionEci = propagate(satRec, new Date()).position;

  // convert/return geodetic coordinates
  return eciToGeodetic(positionEci, gmst);
}

function getSatRec(tle) {
  // parse tle lines
  var line1 = tle.split("\n")[1];
  var line2 = tle.split("\n")[2];

  // create satrec object
  const satrec = twoline2satrec(line1, line2);

  return satrec;
}

export function getLookAngles(observerGd, satRec) {
  // get look angles
  const gmst = gstime(new Date());
  const positionEci = propagate(satRec, new Date()).position;
  const positionEcf = eciToEcf(positionEci, gmst);
  const lookAngles = ecfToLookAngles(observerGd, positionEcf);

  //return ObserverGd Object
  return {
    azimuth: lookAngles.azimuth,
    elevation: lookAngles.elevation,
    rangeSat: lookAngles.rangeSat
  };
}

function isValid(IP) {
  // verify that IP address is the format: [0-256].[0-256].[0-256].[0-256]
  const regex = /^(?:(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\.){3}(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)$/;
  return regex.test(IP);
}

function getLocalTime(Longitude) {
  // get time
  const date = new Date();

  // calculate offset
  const offset = Longitude / 15;

  // calculate the local time in UTC
  let utcTime = date.getTime() + date.getTimezoneOffset() * 60000;
  utcTime += 3600000 * offset;

  // adjust for daylight saving time
  const dst =
    date.getTimezoneOffset() <
    new Date(Date.UTC(date.getFullYear(), 0, 1)).getTimezoneOffset();
  if (dst) {
    utcTime += 3600000;
  }

  // convert UTC time to local time zone
  const localTime = new Date(utcTime);

  // format the time as a string
  const hours = localTime.getHours().toString().padStart(2, "0");
  const minutes = localTime.getMinutes().toString().padStart(2, "0");
  const seconds = localTime.getSeconds().toString().padStart(2, "0");
  const timeString = `${hours}:${minutes}:${seconds}`;

  return timeString;
}

function getTimeUTC(positionGd) {
  // get time from longitude
  const gmst = gstime(new Date());
  const now = new Date();
  const lmst = gmst + degreesLong(positionGd.longitude) / 15;
  const time = new Date(now.getTime() + (lmst - gmst) * 3600 * 1000);

  // parse time into hours,min,sec
  const hours = time.getUTCHours().toString().padStart(2, "0");
  const minutes = time.getUTCMinutes().toString().padStart(2, "0");
  const seconds = time.getUTCSeconds().toString().padStart(2, "0");

  // format time as a string
  const timeString = `${hours}:${minutes}:${seconds}`;

  return timeString;
}
