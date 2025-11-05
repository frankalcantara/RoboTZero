/**
 * Three.js scene management
 * Handles scene initialization, camera, lighting, and rendering
 */

import * as THREE from 'three';

let scene = null;
let camera = null;
let renderer = null;
let controls = null;

/**
 * Initialize Three.js scene
 * @param {HTMLElement} container - DOM element to attach renderer
 * @returns {Object} Object containing scene, camera, renderer references
 */
export function initScene(container) {
  // Create scene
  scene = new THREE.Scene();
  scene.background = new THREE.Color(0x1a1a1a);
  scene.fog = new THREE.Fog(0x1a1a1a, 50, 200);

  // Create camera
  const aspect = container.clientWidth / container.clientHeight;
  camera = new THREE.PerspectiveCamera(60, aspect, 0.1, 1000);
  camera.position.set(10, 15, 10);
  camera.lookAt(0, 0, 0);

  // Create renderer
  renderer = new THREE.WebGLRenderer({ antialias: true });
  renderer.setSize(container.clientWidth, container.clientHeight);
  renderer.setPixelRatio(window.devicePixelRatio);
  renderer.shadowMap.enabled = true;
  renderer.shadowMap.type = THREE.PCFSoftShadowMap;
  container.appendChild(renderer.domElement);

  // Add lighting
  setupLighting();

  // Add grid helper
  const gridHelper = new THREE.GridHelper(50, 50, 0x444444, 0x222222);
  scene.add(gridHelper);

  // Add axes helper (for debugging)
  const axesHelper = new THREE.AxesHelper(5);
  scene.add(axesHelper);

  // Handle window resize
  window.addEventListener('resize', () => {
    const width = container.clientWidth;
    const height = container.clientHeight;
    camera.aspect = width / height;
    camera.updateProjectionMatrix();
    renderer.setSize(width, height);
  });

  return { scene, camera, renderer };
}

/**
 * Setup scene lighting
 */
function setupLighting() {
  // Ambient light
  const ambientLight = new THREE.AmbientLight(0xffffff, 0.4);
  scene.add(ambientLight);

  // Directional light (sun)
  const directionalLight = new THREE.DirectionalLight(0xffffff, 0.8);
  directionalLight.position.set(10, 20, 10);
  directionalLight.castShadow = true;
  directionalLight.shadow.camera.left = -20;
  directionalLight.shadow.camera.right = 20;
  directionalLight.shadow.camera.top = 20;
  directionalLight.shadow.camera.bottom = -20;
  directionalLight.shadow.mapSize.width = 2048;
  directionalLight.shadow.mapSize.height = 2048;
  scene.add(directionalLight);

  // Hemisphere light for softer ambient
  const hemisphereLight = new THREE.HemisphereLight(0x87ceeb, 0x545454, 0.3);
  scene.add(hemisphereLight);
}

/**
 * Start animation loop
 * @param {Function} updateCallback - Function called each frame for updates
 */
export function startAnimation(updateCallback) {
  function animate() {
    requestAnimationFrame(animate);

    if (updateCallback) {
      updateCallback();
    }

    if (controls) {
      controls.update();
    }

    renderer.render(scene, camera);
  }

  animate();
}

/**
 * Add object to scene
 * @param {THREE.Object3D} object - Three.js object to add
 */
export function addToScene(object) {
  if (scene) {
    scene.add(object);
  }
}

/**
 * Remove object from scene
 * @param {THREE.Object3D} object - Three.js object to remove
 */
export function removeFromScene(object) {
  if (scene) {
    scene.remove(object);
  }
}

/**
 * Clear all objects from scene (except lights and helpers)
 */
export function clearScene() {
  if (!scene) return;

  const objectsToRemove = [];
  scene.traverse((object) => {
    if (object.userData.removable) {
      objectsToRemove.push(object);
    }
  });

  objectsToRemove.forEach((object) => {
    scene.remove(object);
    if (object.geometry) object.geometry.dispose();
    if (object.material) {
      if (Array.isArray(object.material)) {
        object.material.forEach((mat) => mat.dispose());
      } else {
        object.material.dispose();
      }
    }
  });
}

/**
 * Update camera position
 * @param {THREE.Vector3} position - New camera position
 * @param {THREE.Vector3} lookAt - Point to look at
 */
export function updateCamera(position, lookAt) {
  if (camera) {
    camera.position.copy(position);
    camera.lookAt(lookAt);
  }
}

/**
 * Get scene reference
 * @returns {THREE.Scene} Scene reference
 */
export function getScene() {
  return scene;
}

/**
 * Get camera reference
 * @returns {THREE.Camera} Camera reference
 */
export function getCamera() {
  return camera;
}

/**
 * Get renderer reference
 * @returns {THREE.Renderer} Renderer reference
 */
export function getRenderer() {
  return renderer;
}

/**
 * Dispose scene and cleanup resources
 */
export function disposeScene() {
  if (renderer) {
    renderer.dispose();
    if (renderer.domElement.parentElement) {
      renderer.domElement.parentElement.removeChild(renderer.domElement);
    }
  }

  clearScene();

  scene = null;
  camera = null;
  renderer = null;
  controls = null;
}
