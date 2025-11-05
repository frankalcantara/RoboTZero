/**
 * Three.js robot model
 * Creates and manages the 3D representation of the line follower robot
 */

import * as THREE from 'three';

/**
 * Create robot model
 * @param {Object} config - Robot configuration
 * @returns {THREE.Group} Robot group object
 */
export function createRobot(config) {
  const robot = new THREE.Group();
  robot.userData.removable = true;

  // Robot body
  const bodyGeometry = new THREE.BoxGeometry(
    config.wheelbase * 1.5,
    0.03,
    config.wheelbase
  );
  const bodyMaterial = new THREE.MeshStandardMaterial({
    color: 0x667eea,
    metalness: 0.3,
    roughness: 0.7
  });
  const body = new THREE.Mesh(bodyGeometry, bodyMaterial);
  body.castShadow = true;
  body.receiveShadow = true;
  body.position.y = config.wheelDiameter / 2;
  robot.add(body);

  // Wheels
  const wheelGeometry = new THREE.CylinderGeometry(
    config.wheelDiameter / 2,
    config.wheelDiameter / 2,
    0.02,
    16
  );
  const wheelMaterial = new THREE.MeshStandardMaterial({
    color: 0x333333,
    metalness: 0.5,
    roughness: 0.5
  });

  // Left wheel
  const leftWheel = new THREE.Mesh(wheelGeometry, wheelMaterial);
  leftWheel.rotation.z = Math.PI / 2;
  leftWheel.position.set(
    -config.wheelbase / 2,
    config.wheelDiameter / 2,
    config.wheelbase / 2
  );
  leftWheel.castShadow = true;
  robot.add(leftWheel);

  // Right wheel
  const rightWheel = new THREE.Mesh(wheelGeometry, wheelMaterial);
  rightWheel.rotation.z = Math.PI / 2;
  rightWheel.position.set(
    config.wheelbase / 2,
    config.wheelDiameter / 2,
    config.wheelbase / 2
  );
  rightWheel.castShadow = true;
  robot.add(rightWheel);

  // Sensors array
  const sensorGroup = createSensors(config);
  robot.add(sensorGroup);

  // Direction indicator
  const arrowGeometry = new THREE.ConeGeometry(0.02, 0.06, 8);
  const arrowMaterial = new THREE.MeshStandardMaterial({
    color: 0x764ba2,
    emissive: 0x764ba2,
    emissiveIntensity: 0.5
  });
  const arrow = new THREE.Mesh(arrowGeometry, arrowMaterial);
  arrow.rotation.x = Math.PI / 2;
  arrow.position.set(0, 0.05, -config.wheelbase / 2 - 0.03);
  robot.add(arrow);

  return robot;
}

/**
 * Create sensor array visualization
 * @param {Object} config - Robot configuration
 * @returns {THREE.Group} Sensor group object
 */
function createSensors(config) {
  const sensorGroup = new THREE.Group();

  const sensorGeometry = new THREE.CylinderGeometry(0.005, 0.005, 0.01, 8);
  const sensorMaterial = new THREE.MeshStandardMaterial({
    color: 0xff0000,
    emissive: 0xff0000,
    emissiveIntensity: 0.5
  });

  const totalWidth = (config.sensors.count - 1) * config.sensors.spacing;
  const startX = -totalWidth / 2;

  for (let i = 0; i < config.sensors.count; i++) {
    const sensor = new THREE.Mesh(sensorGeometry, sensorMaterial.clone());
    sensor.position.set(
      startX + i * config.sensors.spacing,
      config.sensors.height,
      -config.wheelbase / 2
    );
    sensor.userData.sensorIndex = i;
    sensorGroup.add(sensor);
  }

  return sensorGroup;
}

/**
 * Update robot position and orientation
 * @param {THREE.Group} robot - Robot group object
 * @param {Object} state - Robot state (position, heading)
 */
export function updateRobotState(robot, state) {
  if (!robot) return;

  robot.position.set(state.position.x, 0, state.position.y);
  robot.rotation.y = state.heading;
}

/**
 * Update sensor visualization based on readings
 * @param {THREE.Group} robot - Robot group object
 * @param {Array<number>} sensorReadings - Array of sensor values (0-1)
 */
export function updateSensorVisualization(robot, sensorReadings) {
  if (!robot) return;

  robot.traverse((object) => {
    if (object.userData.sensorIndex !== undefined) {
      const index = object.userData.sensorIndex;
      const reading = sensorReadings[index] || 0;

      // Update sensor color based on reading
      // Green = on line (high reading), Red = off line (low reading)
      const color = new THREE.Color().setHSL(reading * 0.33, 1.0, 0.5);
      object.material.color = color;
      object.material.emissive = color;
      object.material.emissiveIntensity = 0.5 + reading * 0.5;
    }
  });
}

/**
 * Create trajectory trail
 * @param {Array<Object>} trajectoryPoints - Array of {x, y} positions
 * @returns {THREE.Line} Trajectory line object
 */
export function createTrajectoryLine(trajectoryPoints) {
  if (!trajectoryPoints || trajectoryPoints.length < 2) {
    return null;
  }

  const points = trajectoryPoints.map(
    (p) => new THREE.Vector3(p.x, 0.01, p.y)
  );

  const geometry = new THREE.BufferGeometry().setFromPoints(points);
  const material = new THREE.LineBasicMaterial({
    color: 0x00ff00,
    linewidth: 2,
    opacity: 0.6,
    transparent: true
  });

  const line = new THREE.Line(geometry, material);
  line.userData.removable = true;

  return line;
}

/**
 * Dispose robot and cleanup resources
 * @param {THREE.Group} robot - Robot group object
 */
export function disposeRobot(robot) {
  if (!robot) return;

  robot.traverse((object) => {
    if (object.geometry) {
      object.geometry.dispose();
    }
    if (object.material) {
      if (Array.isArray(object.material)) {
        object.material.forEach((mat) => mat.dispose());
      } else {
        object.material.dispose();
      }
    }
  });
}
