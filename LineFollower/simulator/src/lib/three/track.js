/**
 * Three.js track visualization
 * Creates and manages the 3D representation of the line follower track
 */

import * as THREE from 'three';

/**
 * Create track surface and line from track data
 * @param {Object} trackData - Track configuration with points
 * @returns {THREE.Group} Track group object
 */
export function createTrack(trackData) {
  const trackGroup = new THREE.Group();
  trackGroup.userData.removable = true;

  if (!trackData.points || trackData.points.length < 2) {
    return trackGroup;
  }

  // Create track surface
  const surface = createTrackSurface(trackData);
  trackGroup.add(surface);

  // Create line path
  const linePath = createLinePath(trackData.points);
  trackGroup.add(linePath);

  // Create track boundaries
  const boundaries = createTrackBoundaries(trackData);
  trackGroup.add(boundaries);

  return trackGroup;
}

/**
 * Create track surface (ground plane)
 * @param {Object} trackData - Track configuration
 * @returns {THREE.Mesh} Track surface mesh
 */
function createTrackSurface(trackData) {
  // Calculate bounding box of track
  const bounds = calculateBounds(trackData.points);

  const width = bounds.maxX - bounds.minX + 4;
  const height = bounds.maxZ - bounds.minZ + 4;

  const geometry = new THREE.PlaneGeometry(width, height);
  const material = new THREE.MeshStandardMaterial({
    color: 0xeeeeee,
    roughness: 0.9,
    metalness: 0.1
  });

  const surface = new THREE.Mesh(geometry, material);
  surface.rotation.x = -Math.PI / 2;
  surface.position.set(
    (bounds.maxX + bounds.minX) / 2,
    0,
    (bounds.maxZ + bounds.minZ) / 2
  );
  surface.receiveShadow = true;

  return surface;
}

/**
 * Create black line path on track
 * @param {Array<Object>} points - Array of track points
 * @returns {THREE.Mesh} Line path mesh
 */
function createLinePath(points) {
  const lineWidth = 0.05;
  const curve = createCurveFromPoints(points);

  // Create extruded line geometry
  const extrudeSettings = {
    steps: 200,
    bevelEnabled: false,
    extrudePath: curve
  };

  const shape = new THREE.Shape();
  shape.moveTo(-lineWidth / 2, 0);
  shape.lineTo(lineWidth / 2, 0);
  shape.lineTo(lineWidth / 2, 0.002);
  shape.lineTo(-lineWidth / 2, 0.002);

  const geometry = new THREE.ExtrudeGeometry(shape, extrudeSettings);
  const material = new THREE.MeshStandardMaterial({
    color: 0x000000,
    roughness: 0.8,
    metalness: 0.2
  });

  const line = new THREE.Mesh(geometry, material);
  line.castShadow = true;

  return line;
}

/**
 * Create curve from track points
 * @param {Array<Object>} points - Array of {x, y} points
 * @returns {THREE.CatmullRomCurve3} Curve object
 */
function createCurveFromPoints(points) {
  const vectors = points.map((p) => new THREE.Vector3(p.x, 0.001, p.y));
  return new THREE.CatmullRomCurve3(vectors, false, 'catmullrom', 0.5);
}

/**
 * Create track boundary markers
 * @param {Object} trackData - Track configuration
 * @returns {THREE.Group} Boundaries group
 */
function createTrackBoundaries(trackData) {
  const boundariesGroup = new THREE.Group();

  // Simple boundary visualization (can be enhanced later)
  const bounds = calculateBounds(trackData.points);

  const boxGeometry = new THREE.BoxGeometry(0.1, 0.2, 0.1);
  const boxMaterial = new THREE.MeshStandardMaterial({
    color: 0xff0000,
    emissive: 0xff0000,
    emissiveIntensity: 0.2
  });

  // Corner markers
  const corners = [
    { x: bounds.minX, z: bounds.minZ },
    { x: bounds.maxX, z: bounds.minZ },
    { x: bounds.maxX, z: bounds.maxZ },
    { x: bounds.minX, z: bounds.maxZ }
  ];

  corners.forEach((corner) => {
    const marker = new THREE.Mesh(boxGeometry, boxMaterial);
    marker.position.set(corner.x, 0.1, corner.z);
    marker.castShadow = true;
    boundariesGroup.add(marker);
  });

  return boundariesGroup;
}

/**
 * Calculate bounding box of track points
 * @param {Array<Object>} points - Array of track points
 * @returns {Object} Bounds with minX, maxX, minZ, maxZ
 */
function calculateBounds(points) {
  let minX = Infinity,
    maxX = -Infinity;
  let minZ = Infinity,
    maxZ = -Infinity;

  points.forEach((p) => {
    minX = Math.min(minX, p.x);
    maxX = Math.max(maxX, p.x);
    minZ = Math.min(minZ, p.y);
    maxZ = Math.max(maxZ, p.y);
  });

  return { minX, maxX, minZ, maxZ };
}

/**
 * Update track visualization
 * @param {THREE.Group} trackGroup - Track group object
 * @param {Object} trackData - Updated track data
 */
export function updateTrack(trackGroup, trackData) {
  // Remove old track
  trackGroup.children.forEach((child) => {
    if (child.geometry) child.geometry.dispose();
    if (child.material) child.material.dispose();
  });
  trackGroup.clear();

  // Create new track
  const newTrack = createTrack(trackData);
  newTrack.children.forEach((child) => {
    trackGroup.add(child);
  });
}

/**
 * Create checkpoint markers along track
 * @param {Array<Object>} checkpoints - Array of checkpoint positions
 * @returns {THREE.Group} Checkpoints group
 */
export function createCheckpoints(checkpoints) {
  const checkpointGroup = new THREE.Group();
  checkpointGroup.userData.removable = true;

  const geometry = new THREE.CylinderGeometry(0.1, 0.15, 0.3, 8);
  const material = new THREE.MeshStandardMaterial({
    color: 0x00ff00,
    emissive: 0x00ff00,
    emissiveIntensity: 0.3
  });

  checkpoints.forEach((cp, index) => {
    const marker = new THREE.Mesh(geometry, material.clone());
    marker.position.set(cp.x, 0.15, cp.y);
    marker.userData.checkpointIndex = index;
    marker.castShadow = true;
    checkpointGroup.add(marker);
  });

  return checkpointGroup;
}

/**
 * Dispose track and cleanup resources
 * @param {THREE.Group} trackGroup - Track group object
 */
export function disposeTrack(trackGroup) {
  if (!trackGroup) return;

  trackGroup.traverse((object) => {
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
