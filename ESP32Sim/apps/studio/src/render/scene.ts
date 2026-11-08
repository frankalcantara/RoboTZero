// Render three.js (docs/03 §7.3). Nenhuma regra de emulação aqui: a cena só
// recebe o estado que a bridge já agregou.
import * as THREE from 'three';
import { OrbitControls } from 'three/addons/controls/OrbitControls.js';
import { GLTFLoader } from 'three/addons/loaders/GLTFLoader.js';
import type { Tokens } from './tokens';

const BOARD_URL = `${import.meta.env.BASE_URL}board/c3-devkitm1.gltf`;

export type StudioScene = {
  render(): void;
  setLed(on: boolean): void;
  dispose(): void;
};

export async function createScene(
  canvas: HTMLCanvasElement,
  tokens: Tokens,
  reducedMotion: boolean,
): Promise<StudioScene> {
  const renderer = new THREE.WebGLRenderer({ canvas, antialias: true });
  renderer.setPixelRatio(Math.min(devicePixelRatio, 2));

  const scene = new THREE.Scene();
  scene.background = new THREE.Color(tokens.background);

  // Unidades em milímetros e origem no canto da placa (§7.3).
  const camera = new THREE.PerspectiveCamera(35, 1, 1, 500);
  camera.position.set(48, 42, 58);

  const controls = new OrbitControls(camera, canvas);
  controls.target.set(12.7, 1, 8.9);
  controls.enableDamping = !reducedMotion;
  controls.update();

  scene.add(new THREE.HemisphereLight(tokens.light, tokens.background, 2));
  const key = new THREE.DirectionalLight(tokens.light, 2);
  key.position.set(30, 60, 20);
  scene.add(key);

  const boardMaterial = new THREE.MeshStandardMaterial({ color: tokens.board, roughness: 0.8 });
  const ledMaterial = new THREE.MeshStandardMaterial({
    color: tokens.ledOff,
    roughness: 0.4,
    emissive: tokens.ledOn,
    emissiveIntensity: 0,
  });

  const board = await new GLTFLoader().loadAsync(BOARD_URL);
  board.scene.traverse((object) => {
    if (object instanceof THREE.Mesh) {
      object.material = object.name === 'LED' ? ledMaterial : boardMaterial;
    }
  });
  scene.add(board.scene);

  let lastWidth = 0;
  let lastHeight = 0;
  const resize = () => {
    const { clientWidth: width, clientHeight: height } = canvas;
    if (width === 0 || height === 0) return;
    if (width === lastWidth && height === lastHeight) return;
    lastWidth = width;
    lastHeight = height;
    renderer.setSize(width, height, false);
    camera.aspect = width / height;
    camera.updateProjectionMatrix();
  };
  const observer = new ResizeObserver(resize);
  observer.observe(canvas);
  resize();

  return {
    render() {
      controls.update();
      renderer.render(scene, camera);
    },
    setLed(on) {
      ledMaterial.emissiveIntensity = on ? 1 : 0;
    },
    dispose() {
      observer.disconnect();
      controls.dispose();
      renderer.dispose();
    },
  };
}
