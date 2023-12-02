import React, { useRef, useState } from "react";
import { Canvas, useFrame } from "@react-three/fiber";
import "../styles/styles.css";
import { OrbitControls, Grid } from "@react-three/drei";
import TextField from "@mui/material/TextField";
import { useRotator } from "../classes/RotatorContext";
import { useGamepads } from "react-gamepads";
import Box from "@mui/material/Box";
import {
  OrthographicCamera,
  PerspectiveCamera,
  QuadraticBezierCurve3,
  Vector3,
} from "three";
import Stack from "@mui/material/Stack";

function setupCamera(camera) {
  camera.position.set(0, 0.2, 0.2);
  camera.rotation.set(-0.154, 0, 0);
  console.log(viewCamera.position);
}
//viewCamera.zoom = 80;
//viewCamera.position.set(0, 1, 4);
//viewCamera.rotation.set(-10* Math.PI, 0, 0);

function useGamepad() {
  const [gamepads, setGamepads] = useState({});
  useGamepads((gamepads) => setGamepads(gamepads));

  return gamepads[0];
}

function clamp(x, min, max) {
  return Math.min(max, Math.max(x, min));
}

function sphericalToCartesian(r, theta, phi) {
  const t = r * Math.cos(phi);
  const y = r * Math.sin(phi);

  const x = t * Math.cos(theta);
  const z = t * Math.sin(theta);

  return [x, y, z];
}

function atLeast(x, min) {
  if (Math.abs(x) < min) return 0;
  return x;
}

function ObserverModel() {
  //const ref = useRef()
  const radiusRef = useRef();
  const heightRef = useRef();
  const widthRef = useRef();
  const octahedronRef = useRef();
  const azimuthRef = useRef();
  const { azimuth, elevation, updateAzimuth, updateElevation } = useRotator();

  /*
  const [{ theta }, set] = useControls(() => ({
    theta: {
      min: 0,
      max: 2 * Math.PI,
      value: 0,
    },
  }));*/
  //const [phi, setPhi] = useState({ min: 0, max: 1.57, value: 0 });
  /*
  const [{ theta, phi }, set] = useControls(() => ({
    theta: {
      min: 0,
      max: 2 * Math.PI,
      value: 0
    },
    phi: {
      min: 0,
      max: 0.5 * Math.PI,
      value: 0
    }
  }))*/

  const gamepad = useGamepad();

  useFrame((_, delta) => {
    const h = atLeast(gamepad?.axes[2], 0.1);
    const v = atLeast(gamepad?.axes[3], 0.1);
    let phi = (elevation * Math.PI) / 180;
    let theta = (azimuth * Math.PI) / 180;
    //const h = atLeast(Math.PI, 0.1)
    //const v = atLeast(Math.PI, 0.1)
    const newPhi = clamp(phi - v * delta * 5, -Math.PI * 0.3, Math.PI * 0.3);
    /*
    set({
      theta: theta + h * delta * 5,
    });

    set({
      phi: newPhi,
      theta: theta + h * delta * 5
    })*/

    octahedronRef.current.position.set(...sphericalToCartesian(1, theta, phi));

    if (radiusRef.current && octahedronRef.current) {
      // Set the tube's rotation based on the angle calculated from the direction vector
      radiusRef.current.rotation.y = -theta;
      radiusRef.current.rotation.z = phi;

      const T = 2 * Math.cos(phi); // distance to y-axis after being rotated up
      const Y = 2 * Math.sin(phi);

      const X = T * Math.cos(theta);
      const Z = T * Math.sin(theta);

      heightRef.current.scale.y = Y;
      heightRef.current.position.x = X;
      heightRef.current.position.z = Z;

      widthRef.current.rotation.y = -theta;
      widthRef.current.scale.y = 2 * Math.cos(phi);
      widthRef.current.scale.x = 2 * Math.cos(phi);

      azimuthRef.current.scale.y = 2 * Math.cos(phi);
      azimuthRef.current.scale.x = 2 * Math.cos(phi);
    }
  });

  return (
    <>
      <mesh ref={octahedronRef} scale={2} position={[0, 0, 0]} rotation={[0, 0, 0]}>
        <octahedronGeometry args={[0.02, 15]} />
        <meshBasicMaterial color="#ff0000" />
      </mesh>
      <mesh
        name="Horizon"
        scale={2}
        position={[0, 0, 0]}
        rotation={[-0.5 * Math.PI, 0, 0]}
      >
        <circleGeometry args={[1, 64, 0, 2 * Math.PI]} />
        <meshBasicMaterial color="#0a5b88" />
      </mesh>
      <mesh
        name="Meridian"
        scale={2}
        position={[0, 0, 0]}
        rotation={[0, -0.5 * Math.PI, 0]}
      >
        <torusGeometry args={[0.99, 0.02, 32, 64, Math.PI]} />
        <meshBasicMaterial color="#4bdae3" />
      </mesh>
      <mesh
        ref={azimuthRef}
        name="Azimuth"
        scale={2}
        position={[0, 0, 0]}
        rotation={[-0.5 * Math.PI, 0, 0]}
      >
        <torusGeometry args={[1, 0.02, 64, 64, 2 * Math.PI]} />
        <meshBasicMaterial color="#f37419" />
      </mesh>
      <mesh
        name="Orbit"
        scale={2}
        position={[0, 0, 0]}
        rotation={[-0.597, 1.0, 0.54]}
      >
        <torusGeometry args={[0.99, 0.02, 64, 32, Math.PI]} />
        <meshBasicMaterial color="#19b600" />
      </mesh>
      <mesh
        ref={radiusRef}
        name="Radius"
        scale={2}
        position={[0, 0, 0]}
        rotation={[0, 0, 0]}
      >
        <tubeGeometry
          args={[
            new QuadraticBezierCurve3(
              new Vector3(0, 0, 0),
              new Vector3(1, 0, 0),
              new Vector3(1, 0, 0),
            ),
            32,
            0.02,
            16,
            false,
          ]}
        />
        <meshBasicMaterial color="#ff0000" />
      </mesh>
      <mesh
        ref={heightRef}
        name="Height"
        scale={2}
        position={[0, 0, 0]}
        rotation={[0, 0, 0]}
      >
        <tubeGeometry
          args={[
            new QuadraticBezierCurve3(
              new Vector3(0, 0, 0),
              new Vector3(0, 1, 0),
              new Vector3(0, 1, 0),
            ),
            32,
            0.02,
            16,
            false,
          ]}
        />
        <meshBasicMaterial color="#ff0000" />
      </mesh>
      <mesh
        ref={widthRef}
        name="Width"
        scale={2}
        position={[0, 0, 0]}
        rotation={[0, 0, 0]}
      >
        <tubeGeometry
          args={[
            new QuadraticBezierCurve3(
              new Vector3(0, 0, 0),
              new Vector3(1, 0, 0),
              new Vector3(1, 0, 0),
            ),
            32,
            0.02,
            16,
            false,
          ]}
        />
        <meshBasicMaterial color="#ff0000" />
      </mesh>
    </>
  );
}

function Scene() {
  const { azimuth, elevation, updateAzimuth, updateElevation } = useRotator();

  return (
    <>
      <Stack
        direction="row"
        spacing={2}
        sx={{ width: "100%" }}
        justifyContent="space-between"
      >
        {/*<Controller
          label="Azimuth"
          fillColor="#FF9900"
          min={0}
          max={360}
          step={1}
          angle={azimuth}
          setAngle={updateAzimuth}
        />
        <Controller
          label="Elevation"
          fillColor="#FF0000"
          min={0}
          max={90}
          step={1}
          angle={elevation}
          setAngle={updateElevation}
  />*/}
        <Box>
          <Canvas
            orthographic
            camera={{zoom:40, position: [-3.5, 2,3.5] }}
            style={{ borderRadius: "6px" }}
          >
            <Ground />

            <ObserverModel />
            {/* Uncomment the following line if you want to display axes */}
            {/* <axesHelper /> */}
            <color attach="background" args={["#000B6D"]} />
            <OrbitControls target={[0, 1, 0]} />
          </Canvas>
        </Box>
      </Stack>
    </>
  );
}

function Ground() {
  const gridConfig = {
    cellSize: 0.5,
    cellThickness: 0.5,
    cellColor: "#6f6f6f",
    sectionSize: 3,
    sectionThickness: 1,
    sectionColor: "#9d4b4b",
    fadeDistance: 10,
    fadeStrength: 1,
    followCamera: false,
    infiniteGrid: true,
  };
  return <Grid position={[0, -0.01, 0]} args={[10.5, 10.5]} {...gridConfig} />;
}

export default Scene;
